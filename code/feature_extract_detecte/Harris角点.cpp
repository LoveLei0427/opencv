//角点可以看做由两个边缘近似直角相结合构成的基元
//Harris具有旋转不变性 不具有尺度不变性  属于可以精确定位的二维特征 位于两条边缘的交点处

#include<iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace std;
using namespace cv;

Mat srcImg, grayImg, dstImg;



RNG rng(12345);
const char* outputwindow = "resultImg";
void onCornerHarris(int, void*);//回调函数
int threshold_value = 30;
int threshold_max = 175;

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
	cvtColor(srcImg,grayImg,CV_BGR2GRAY);

	createTrackbar("阈值：", outputwindow, &threshold_value, threshold_max, onCornerHarris);
	onCornerHarris(0, 0);

	cvWaitKey(0);
}
void onCornerHarris(int, void*){
	Mat normImg;
	Mat scaledImage;//线性变换后的8位无符号整形图
	Mat tempImg = srcImg.clone();

	dstImg = Mat::zeros(srcImg.size(), CV_32FC1);
	//角点检测
	cornerHarris(grayImg, dstImg, 2, 3, 0.04, BORDER_DEFAULT);
	//归一化与转换
	normalize(dstImg, normImg, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(normImg, scaledImage);
	//绘制：将检测到的符合阈值条件的角点绘制出来
	for (int j = 0; j < normImg.rows; j++){
		for (int i = 0; i < normImg.cols; i++){
			if ((int)normImg.at<float>(j, i) > (threshold_value + 80)){
				circle(tempImg, Point(i, j), 5, Scalar(10, 10, 255), 2, 8, 0);
				circle(scaledImage, Point(i, j), 5, Scalar(0, 10, 255), 2, 8, 0);
			}
		}
	}
	imshow("角点结果", tempImg);
	imshow("现行变换", scaledImage);

}