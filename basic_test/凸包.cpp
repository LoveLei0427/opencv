//ת�����Ҷ� ׼������ֵ  ���������õ���ѡ�� ����API ����
#include<iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace std;
using namespace cv;

Mat srcImg, grayImg;
int threshold_value = 100;
int threshold_max = 255;
void Threshold_Callback(int, void*);
RNG rng(12345);

const char* outputwindow = "convexImg";

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

	const char* trackbar_label = "Threshold:";
	cvtColor(srcImg, grayImg, CV_BGR2GRAY);
	blur(grayImg, grayImg, Size(3, 3), Point(-1, -1),BORDER_DEFAULT);
	namedWindow("�Ҷ�ͼ��", CV_WINDOW_AUTOSIZE);
	imshow("�Ҷ�ͼ��", grayImg);


	createTrackbar(trackbar_label, outputwindow, &threshold_value, threshold_max, Threshold_Callback);
	Threshold_Callback(0,0);

	cvWaitKey(0);
}

void Threshold_Callback(int, void*){

	Mat convexImg, binaryImg;
	//binaryImg = Mat::zeros(srcImg.size(), CV_8UC1);
	vector<vector<Point>> contours;
	vector<Vec4i> hierachy;

	//ͨ����ֵ�����ѻҶ�ͼ��ɶ�ֵͼ
	threshold(grayImg,binaryImg,threshold_value,threshold_max,THRESH_BINARY);

	findContours(binaryImg, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));


	vector<vector<Point>>convexs(contours.size());

	for (size_t i = 0; i < contours.size(); ++i){
		convexHull(contours[i], convexs[i], false, true);
	}

	//����
	convexImg = Mat::zeros(srcImg.size(), CV_8UC3);
	vector<Vec4i> empty(0);

	for (size_t k = 0; k < contours.size(); ++k){
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(convexImg, contours, k, color, 2, 8, hierachy,0,Point(0,0));
		drawContours(convexImg, convexs, k, color, 2, 8, Mat(), 0, Point(0, 0));

	};
	imshow(outputwindow, convexImg);
}
