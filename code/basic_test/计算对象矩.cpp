//��������
//Canny��ȡ��Ե �����ֿ� ����ÿ����������ľ�  ����ÿ��������������� ��� ����
#include<iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace std;
using namespace cv;

Mat srcImg, grayImg, gaussImg;
int threshold_value = 20;
int threshold_max = 255;

void Contours_Callback(int, void*);
RNG rng(12345);

const char* outputwindow = "resultImg";

int main(void){

	srcImg = imread("image5.jpg");
	if (!srcImg.data)
	{
		cout << "ͼ�����ʧ��" << endl;
		return -1;
	}
	namedWindow("����ͼ��", CV_WINDOW_AUTOSIZE);
	imshow("����ͼ��", srcImg);
	namedWindow(outputwindow, CV_WINDOW_AUTOSIZE);
	const char* trackbar_label = "Threshold Value:";
	//��ͼ��ת��Ϊ�Ҷ�ͼ
	cvtColor(srcImg, grayImg, CV_BGR2GRAY);
	//ʹ�ø�˹�˲���������
	GaussianBlur(grayImg, gaussImg, Size(3, 3),0,0);

	namedWindow("��˹ͼ��", CV_WINDOW_AUTOSIZE);
	imshow("��˹ͼ��", gaussImg);


	createTrackbar(trackbar_label, outputwindow, &threshold_value, threshold_max, Contours_Callback);
	Contours_Callback(0, 0);

	cvWaitKey(0);
}

void Contours_Callback(int, void*){

	Mat resultImg;
	Mat cannyImg;
	vector<vector<Point>>contours;
	vector<Vec4i>hierachy;

	//Canny������ȡ
	Canny(gaussImg, cannyImg, threshold_value, threshold_value * 2, 3, false);
	//��ȡ��ͨ����߽�
	findContours(cannyImg, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	//����moment��������
	vector<Moments>contours_moments(contours.size());
	vector<Point2f>ccs(contours.size());//��������λ��

	for (size_t i = 0; i < contours.size(); ++i){
		contours_moments[i] = moments(contours[i], false);
	
		//��������λ�� x y����
		ccs[i] = Point(static_cast<float>(contours_moments[i].m10 / contours_moments[i].m00),
			static_cast<float>(contours_moments[i].m01 / contours_moments[i].m00));
	}
	//���� ����λ��
	resultImg = Mat::zeros(srcImg.size(), CV_8UC3);
	for (size_t k = 0; k < contours.size(); ++k){
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		/*printf("center point x : %.2f y : %.2f\n", ccs[k].x, ccs[k].y);
		printf("contours %d area : %.2f   arc length : %.2f\n", k,contourArea(contours[k]), arcLength(contours[k], true));*/ 
		drawContours(resultImg, contours, k, color, 2, 8, hierachy, 0, Point(0, 0));
		circle(resultImg, ccs[k], 2, color, 2, 8);
	}
	imshow(outputwindow, resultImg);
}