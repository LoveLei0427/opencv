//计算图像中对象的个数 
//二值分割 形态学处理  距离变换    连通区域计算
#include <opencv2/opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

Mat srcImage, grayImage, binaryImage, distImage,dstImage;

int main(void)
{
	srcImage = imread("case4.jpg");// , IMREAD_GRAYSCALE);
	if (!srcImage.data)
	{
		cout << "加载图像失败！" << endl;
	}
	namedWindow("输入图像", CV_WINDOW_AUTOSIZE);
	imshow("输入图像", srcImage);

	//1、转换为灰度
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);

	//2、二值分割
	threshold(grayImage, binaryImage, 0, 255, CV_THRESH_BINARY|CV_THRESH_OTSU);
	imshow("二值图像", binaryImage);
	
	//3、形态学操作
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	dilate(binaryImage, binaryImage, kernel, Point(-1, -1), 1);
	imshow("形态学腐蚀图像", binaryImage);
	
	// 4、距离变换
	bitwise_not(binaryImage, binaryImage);//矩阵中的每一位进行取反
	//void distanceTransform( InputArray src, OutputArray dst,int distanceType, int maskSize );
	distanceTransform(binaryImage, distImage, CV_DIST_L2, 3);
	normalize(distImage, distImage, 0, 1.0, NORM_MINMAX);
	imshow("距离变换图像", distImage);
	
	//5、对距离变换后的结果图像进行二值化分割
	Mat distImage_8U;
	distImage.convertTo(distImage_8U, CV_8U);
	// adaptiveThreshold( InputArray src, OutputArray dst,double maxValue, int adaptiveMethod,int thresholdType, int blockSize, double C );
	adaptiveThreshold(distImage_8U, distImage_8U, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 85, 0.0);
	kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	//膨胀操作 使非连续区域相连
	dilate(distImage_8U, distImage_8U, kernel, Point(-1, -1), 2);
	imshow("距离变换-二值化", distImage_8U);

	//6、连通区域计数
	vector<vector<Point>> contours;
	findContours(distImage_8U, contours, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	// 可视化结果
	Mat resultImage = Mat::zeros(srcImage.size(), CV_8UC3);
	RNG rng(12345);
	for (size_t t = 0; t < contours.size(); t++) {
		drawContours(resultImage, contours, static_cast<int>(t), Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)),
			-1, 8, Mat());
	}
	printf("角点数量 : %d", contours.size());
	imshow("最终结果", resultImage);
	cvWaitKey(0);
}