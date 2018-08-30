#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

// 定义全局变量
const string harris_winName = "自定义角点检测";
Mat src_img, gray_img;                       // src_img表示原图, gray_img表示灰度图
Mat harris_dst_img, harris_response_img;     // harris_dst_img存储自相关矩阵M的特征值和特征向量，harris_response_img存储响应函数的结果

double min_respense_value;			  // 响应函数的结果矩阵中的最小值
double max_respense_value;			  // 响应函数的结果矩阵中的最大值

int qualityValue = 30;
int max_qualityValue = 100;              // 通过qualityValue/max_qualityValue的结果作为qualitylevel来计算阈值
RNG  random_number_generator;             // 定义一个随机数发生器
void self_defining_Harris_Demo(int, void*);      //TrackBar回调函数声明

// 主函数
int main()
{
	src_img = imread("image4.jpg");
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
	double k = 0.04;
	harris_dst_img = Mat::zeros(src_img.size(), CV_32FC(6));
	// 目标图像harris_dst_img存储自相关矩阵M的特征值和特征向量，
	// 并将它们以(λ1, λ2, x1, y1, x2, y2)的形式存储。其中λ1, λ2是M未经过排序的特征值；
	// x1, y1是对应于λ1的特征向量；x2, y2是对应于λ2的特征向量。
	// 因此目标矩阵为6通道，即 CV_32FC(6)的矩阵。

	harris_response_img = Mat::zeros(src_img.size(), CV_32FC1);
	// harris_response_img用来存储通过每个像素值所对应的自相关矩阵所计算得到的响应值

	cornerEigenValsAndVecs(gray_img, harris_dst_img, blockSize, ksize, 4);
	// 该函数用来计算每个像素值对应的自相关矩阵的特征值和特征向量

	// 计算响应函数值
	for (int row = 0; row < harris_dst_img.rows; ++row)
	{
		for (int col = 0; col < harris_dst_img.cols; ++col)
		{
			double eigenvalue1 = harris_dst_img.at<Vec6f>(row, col)[0];     // 获取特征值1
			double eigenvalue2 = harris_dst_img.at<Vec6f>(row, col)[1];		// 获取特征值2
			harris_response_img.at<float>(row, col) = eigenvalue1* eigenvalue2 - k*pow((eigenvalue1 + eigenvalue2), 2);
			// 通过响应公式R=λ1*λ2 - k*(λ1+λ2)*(λ1+λ2)来计算每个像素对应的响应值
		}
	}
	minMaxLoc(harris_response_img, &min_respense_value, &max_respense_value, 0, 0, Mat());   // 寻找响应矩阵中的最小值和最大值
	namedWindow(harris_winName, CV_WINDOW_AUTOSIZE);
	createTrackbar("Quality Value:", harris_winName, &qualityValue, max_qualityValue, self_defining_Harris_Demo);    //创建TrackBar
	self_defining_Harris_Demo(0, 0);

	waitKey(0);
	return 0;
}


//  回调函数实现
void self_defining_Harris_Demo(int, void*)
{
	if (qualityValue < 10)
	{
		qualityValue = 10;       // 控制qualitylevel的下限值
	}
	Mat result_img = src_img.clone();    // 输出图像
	float threshold_value = min_respense_value + (((double)qualityValue) / max_qualityValue)*(max_respense_value - min_respense_value);
	for (int row = 0; row <result_img.rows; row++)
	{
		for (int col = 0; col < result_img.cols; col++)
		{
			float resp_value = harris_response_img.at<float>(row, col);
			if (resp_value > threshold_value)
			{
				circle(result_img, Point(col, row), 2, Scalar(random_number_generator.uniform(0, 255),
					random_number_generator.uniform(0, 255), random_number_generator.uniform(0, 255)), 2, 8, 0);
			}
		}
	}
	imshow(harris_winName, result_img);
}