#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
 
using namespace cv;
using namespace std;
 
// 定义全局变量
const string ShiTomasi_winName = "Custom Shi-Tomasi Corners Detector";
Mat src_img, gray_img;                  // src_img表示原图, gray_img表示灰度图
Mat ShiTomasi_dst_img;                  // ShiTomasi_dst_img用来存储每个像素对应的自相关矩阵的最小特征值
double min_ShiTomasi_value;            // 最小特征矩阵中的最小值
double max_ShiTomasi_value;		      // 最小特征矩阵中的最大值
int ShiTomasi_qualityValue = 30;
int max_qualityValue = 100;
RNG  random_number_generator;                        // 定义一个随机数发生器
void self_defining_ShiTomasi_Demo(int, void*);      //TrackBar回调函数声明
 
// 主函数
int main( )
{
	src_img = imread("test11.png");
	if (src_img.empty())
	{
		printf("could not load the image...\n");
		return -1;
	}
	namedWindow("原图", CV_WINDOW_AUTOSIZE);
	imshow("原图", src_img);
	cvtColor(src_img, gray_img, COLOR_BGR2GRAY);      //将彩色图转化为灰度图
	
	// 计算特征值
	int blockSize = 3;
	int ksize = 3;
 
	// 计算最小特征值
	ShiTomasi_dst_img = Mat::zeros(src_img.size(), CV_32FC1);
	cornerMinEigenVal(gray_img, ShiTomasi_dst_img, blockSize, ksize, 4);            // 计算每个像素对应的自相关矩阵的最小特征值
	minMaxLoc(ShiTomasi_dst_img, &min_ShiTomasi_value, &max_ShiTomasi_value, 0, 0, Mat());           //计算最小最大值
	namedWindow(ShiTomasi_winName, CV_WINDOW_AUTOSIZE);
	createTrackbar("Quality:", ShiTomasi_winName, &ShiTomasi_qualityValue, max_qualityValue, self_defining_ShiTomasi_Demo);
	self_defining_ShiTomasi_Demo(0, 0);
 
	waitKey(0);
	return 0;
}
 
 
//  回调函数实现
void self_defining_ShiTomasi_Demo(int, void*)
{
	if (ShiTomasi_qualityValue < 10)
	{
		ShiTomasi_qualityValue = 10;       // 控制qualitylevel的下限值
	}                     
	Mat result_img = src_img.clone();    // 输出图像
	float threshold_value = min_ShiTomasi_value + (((double)ShiTomasi_qualityValue) / max_qualityValue)*(max_ShiTomasi_value - min_ShiTomasi_value);
	for (int row = 0; row <result_img.rows; row++)
	{
		for (int col = 0; col < result_img.cols; col++)
		{
			float resp_value = ShiTomasi_dst_img.at<float>(row, col);
		if (resp_value > threshold_value)
			{
				circle(result_img, Point(col, row), 2, Scalar(random_number_generator.uniform(0, 255),
					random_number_generator.uniform(0, 255), random_number_generator.uniform(0, 255)), 2, 8, 0);
		}
		}
	}
	imshow(ShiTomasi_winName, result_img);
}
 