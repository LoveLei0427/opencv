//������Χ���ƾ��ο��Բ�ο�
//ת�����Ҷ� ����ģ��  ת������ֵͼ��  �õ�����  ���������õ���ѡ�� ����API ����
#include<iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace std;
using namespace cv;

Mat srcImg, grayImg;
int threshold_value = 100;
int threshold_max = 255;

void Contours_Callback(int, void*);
RNG rng(12345);

const char* outputwindow = "resultImg";

int main(void){

	srcImg = imread("image1.jpg");
	if (!srcImg.data)
	{
		cout << "ͼ�����ʧ��" << endl;
		return -1;
	}
	namedWindow("����ͼ��", CV_WINDOW_AUTOSIZE);
	imshow("����ͼ��", srcImg);
	namedWindow(outputwindow, CV_WINDOW_AUTOSIZE);
	const char* trackbar_label = "Threshold Value:";

	cvtColor(srcImg, grayImg, CV_BGR2GRAY);
	blur(grayImg, grayImg, Size(3, 3), Point(-1, -1),BORDER_DEFAULT);

	namedWindow("�Ҷ�ͼ��", CV_WINDOW_AUTOSIZE);
	imshow("�Ҷ�ͼ��", grayImg);


	createTrackbar(trackbar_label, outputwindow, &threshold_value, threshold_max, Contours_Callback);
	Contours_Callback(0, 0);

	cvWaitKey(0);
}

void Contours_Callback(int, void*){

	Mat resultImg, binaryImg;
	
	//����߽��еĵ�
	vector<vector<Point>> contours;
	//���������
	vector<Vec4i> hierachy;

	//ͨ����ֵ�����ѻҶ�ͼ��ɶ�ֵͼ
	threshold(grayImg,binaryImg,threshold_value,threshold_max,THRESH_BINARY);
	//Ѱ������
	findContours(binaryImg, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	
	vector<vector<Point>>contours_ploy(contours.size());//�洢������εĵ㼯
	vector<Rect>ploy_rects(contours.size()); //��С���εĵ㼯
	vector<Point2f>ccs(contours.size()); //Բ��
	vector<float>radius(contours.size());//�뾶




	//�õ�������Χ��С���κ���С����Բ��
	for (size_t i = 0; i < contours.size(); ++i){
		approxPolyDP(Mat(contours[i]), contours_ploy[i], 3, true);
		ploy_rects[i] = boundingRect(contours_ploy[i]);
		minEnclosingCircle(contours_ploy[i], ccs[i], radius[i]);
	}
	//������С���κ���С����Բ��
	//srcImg.copyTo(resultImg);
	resultImg = Mat::zeros(srcImg.size(), srcImg.type());
	for (size_t k = 0; k < contours.size(); ++k){
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		rectangle(resultImg, ploy_rects[k],color, 2, 8);
		circle(resultImg, ccs[k],radius[k], color, 2, 8);
	}
	imshow(outputwindow, resultImg);
}