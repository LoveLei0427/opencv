//计算对象矩
//Canny提取边缘 发现轮库 计算每个轮廓对象的矩  计算每个轮廓对象的中心 面积 弧长
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
		cout << "图像加载失败" << endl;
		return -1;
	}
	namedWindow("输入图像", CV_WINDOW_AUTOSIZE);
	imshow("输入图像", srcImg);
	namedWindow(outputwindow, CV_WINDOW_AUTOSIZE);
	const char* trackbar_label = "Threshold Value:";
	//将图像转换为灰度图
	cvtColor(srcImg, grayImg, CV_BGR2GRAY);
	//使用高斯滤波降低噪声
	GaussianBlur(grayImg, gaussImg, Size(3, 3),0,0);

	namedWindow("高斯图像", CV_WINDOW_AUTOSIZE);
	imshow("高斯图像", gaussImg);


	createTrackbar(trackbar_label, outputwindow, &threshold_value, threshold_max, Contours_Callback);
	Contours_Callback(0, 0);

	cvWaitKey(0);
}

void Contours_Callback(int, void*){

	Mat resultImg;
	Mat cannyImg;
	vector<vector<Point>>contours;
	vector<Vec4i>hierachy;

	//Canny轮廓提取
	Canny(gaussImg, cannyImg, threshold_value, threshold_value * 2, 3, false);
	//提取连通区域边界
	findContours(cannyImg, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	//定义moment对象数组
	vector<Moments>contours_moments(contours.size());
	vector<Point2f>ccs(contours.size());//保存中心位置

	for (size_t i = 0; i < contours.size(); ++i){
		contours_moments[i] = moments(contours[i], false);
	
		//计算中心位置 x y坐标
		ccs[i] = Point(static_cast<float>(contours_moments[i].m10 / contours_moments[i].m00),
			static_cast<float>(contours_moments[i].m01 / contours_moments[i].m00));
	}
	//绘制 中心位置
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