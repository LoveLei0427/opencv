//轮廓周围绘制矩形框和圆形框
//转换到灰度 进行模糊  转换到二值图像  得到轮廓  发现轮廓得到候选点 调用API 绘制
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
		cout << "图像加载失败" << endl;
		return -1;
	}
	namedWindow("输入图像", CV_WINDOW_AUTOSIZE);
	imshow("输入图像", srcImg);
	namedWindow(outputwindow, CV_WINDOW_AUTOSIZE);
	const char* trackbar_label = "Threshold Value:";

	cvtColor(srcImg, grayImg, CV_BGR2GRAY);
	blur(grayImg, grayImg, Size(3, 3), Point(-1, -1),BORDER_DEFAULT);

	namedWindow("灰度图像", CV_WINDOW_AUTOSIZE);
	imshow("灰度图像", grayImg);


	createTrackbar(trackbar_label, outputwindow, &threshold_value, threshold_max, Contours_Callback);
	Contours_Callback(0, 0);

	cvWaitKey(0);
}

void Contours_Callback(int, void*){

	Mat resultImg, binaryImg;
	
	//保存边界中的点
	vector<vector<Point>> contours;
	//输出点向量
	vector<Vec4i> hierachy;

	//通过阈值操作把灰度图变成二值图
	threshold(grayImg,binaryImg,threshold_value,threshold_max,THRESH_BINARY);
	//寻找轮廓
	findContours(binaryImg, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	
	vector<vector<Point>>contours_ploy(contours.size());//存储外界多边形的点集
	vector<Rect>ploy_rects(contours.size()); //最小矩形的点集
	vector<Point2f>ccs(contours.size()); //圆心
	vector<float>radius(contours.size());//半径




	//得到轮廓周围最小矩形和最小区域圆形
	for (size_t i = 0; i < contours.size(); ++i){
		approxPolyDP(Mat(contours[i]), contours_ploy[i], 3, true);
		ploy_rects[i] = boundingRect(contours_ploy[i]);
		minEnclosingCircle(contours_ploy[i], ccs[i], radius[i]);
	}
	//绘制最小矩形和最小区域圆形
	//srcImg.copyTo(resultImg);
	resultImg = Mat::zeros(srcImg.size(), srcImg.type());
	for (size_t k = 0; k < contours.size(); ++k){
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		rectangle(resultImg, ploy_rects[k],color, 2, 8);
		circle(resultImg, ccs[k],radius[k], color, 2, 8);
	}
	imshow(outputwindow, resultImg);
}