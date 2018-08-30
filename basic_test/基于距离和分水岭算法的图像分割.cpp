//����任(��������/��ʴ�õ������ڵ��Ǿ���)
//���ڷ�ˮ��任��ͼ��ָ�(���ڽ���ԭ��)
#include<iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace std;
using namespace cv;

Mat srcImg;


RNG rng(12345);

const char* outputwindow = "ͼ��ָ���";
int threshold_value = 40;
int threshold_max = 255;

int main(void){

	srcImg = imread("image6.jpg");
	if (!srcImg.data)
	{
		cout << "ͼ�����ʧ��" << endl;
		return -1;
	}
	namedWindow("����ͼ��", CV_WINDOW_AUTOSIZE);
	imshow("����ͼ��", srcImg);
	namedWindow(outputwindow, CV_WINDOW_AUTOSIZE);
	//1����ɫ������ɺ�ɫ Ϊ������ʾ��׼��
	for (int row = 0; row < srcImg.rows; ++row){
		for (int col = 0; col < srcImg.cols; ++col){
			if (srcImg.at<Vec3b>(row, col) == Vec3b(255, 255, 255)){
				srcImg.at<Vec3b>(row, col)[0] = 0;
				srcImg.at<Vec3b>(row, col)[1] = 0;
				srcImg.at<Vec3b>(row, col)[2] = 0;

			}
		}

	}
	namedWindow("��ɫ����", CV_WINDOW_AUTOSIZE);
	imshow("��ɫ����", srcImg);

	//2���� ʹ��filter2D��������˹����ʵ��ͼ��Աȶ����
	Mat kernel = (Mat_<float>(3, 3) << 1, 1, 1, 1, -7, 1, 1, 1, 1);
	Mat laplanceImg;
	Mat sharpImg = srcImg;
	filter2D(srcImg, laplanceImg, CV_32F, kernel, Point(-1, -1), 0, BORDER_CONSTANT);
	srcImg.convertTo(sharpImg, CV_32F);

	Mat resultImg = sharpImg - laplanceImg;
	
	resultImg.convertTo(resultImg, CV_8UC3);
	namedWindow("�񻯽��", CV_WINDOW_AUTOSIZE);
	imshow("�񻯽��", resultImg);

	//3�����񻯽��ͼ��ת��Ϊ��ֵͼ��
	Mat tempImg,binaryImg;
	cvtColor(srcImg, resultImg, CV_BGR2GRAY);
	threshold(resultImg, binaryImg, threshold_value, threshold_max, THRESH_BINARY | THRESH_OTSU);
	namedWindow("��ֵͼ��",CV_WINDOW_AUTOSIZE);
	imshow("��ֵͼ��",binaryImg);
	
	//4������任    ͼ����Խ���ĵ㣬�����������ľ���ԽԶ
	Mat distImg;

	distanceTransform(binaryImg, distImg, CV_DIST_L1, 3);

	//5�������һ����0-1
	normalize(distImg, distImg, 0, 1, NORM_MINMAX);
	imshow("����ͼ��", distImg);

	//6��ʹ����ֵ�ٴζ�ֵ��
	threshold(distImg, distImg, .4, 1, THRESH_BINARY);
	Mat k1 = Mat::ones(13, 13, CV_8UC1);

	//7����ʴ�õ�
	erode(distImg, distImg, k1, Point(-1, -1));
	imshow("������ֵ��ʴͼ��", distImg);

	//8���õ����
	Mat dist_8u;
	distImg.convertTo(dist_8u, CV_8U);
	vector<vector<Point>> contours;
	findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	//9��������Ǿ���
	Mat markers = Mat::zeros(srcImg.size(), CV_32SC1);
	for (size_t i = 0; i < contours.size(); i++) {
		drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i)+1), -1);
	}
	circle(markers, Point(5, 5), 3, Scalar(255, 255, 255), -1);
	imshow("���", markers * 1000);
	
	//10����ˮ���㷨�ָ�
	watershed(srcImg, markers);
	Mat mark = Mat::zeros(markers.size(), CV_8UC1);
	markers.convertTo(mark, CV_8UC1);
	bitwise_not(mark, mark, Mat());
	imshow("��ˮ��ͼ��", mark);

	//11�� ���������ɫ
	vector<Vec3b> colors;
	for (size_t i = 0; i < contours.size(); i++) {
		int r = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int b = theRNG().uniform(0, 255);
		colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}

	//12�����ӻ���ʾ
	Mat dstImg = Mat::zeros(markers.size(), CV_8UC3);
	for (int row = 0; row < markers.rows; row++) {
		for (int col = 0; col < markers.cols; col++) {
			int index = markers.at<int>(row, col);
			if (index > 0 && index <= static_cast<int>(contours.size())) {
				dstImg.at<Vec3b>(row, col) = colors[index - 1];
			}
			else {
				dstImg.at<Vec3b>(row, col) = Vec3b(0, 0, 0);
			}
		}
	}
	imshow("����ͼ��", dstImg);
	cvWaitKey(0);
}