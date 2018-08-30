//距离变换(不断膨胀/腐蚀得到；基于倒角距离)
//基于分水岭变换的图像分割(基于浸泡原理)
#include<iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace std;
using namespace cv;

Mat srcImg;


RNG rng(12345);

const char* outputwindow = "图像分割结果";
int threshold_value = 40;
int threshold_max = 255;

int main(void){

	srcImg = imread("image6.jpg");
	if (!srcImg.data)
	{
		cout << "图像加载失败" << endl;
		return -1;
	}
	namedWindow("输入图像", CV_WINDOW_AUTOSIZE);
	imshow("输入图像", srcImg);
	namedWindow(outputwindow, CV_WINDOW_AUTOSIZE);
	//1、白色背景变成黑色 为后续显示做准备
	for (int row = 0; row < srcImg.rows; ++row){
		for (int col = 0; col < srcImg.cols; ++col){
			if (srcImg.at<Vec3b>(row, col) == Vec3b(255, 255, 255)){
				srcImg.at<Vec3b>(row, col)[0] = 0;
				srcImg.at<Vec3b>(row, col)[1] = 0;
				srcImg.at<Vec3b>(row, col)[2] = 0;

			}
		}

	}
	namedWindow("黑色背景", CV_WINDOW_AUTOSIZE);
	imshow("黑色背景", srcImg);

	//2、锐化 使用filter2D和拉普拉斯算子实现图像对比度提高
	Mat kernel = (Mat_<float>(3, 3) << 1, 1, 1, 1, -7, 1, 1, 1, 1);
	Mat laplanceImg;
	Mat sharpImg = srcImg;
	filter2D(srcImg, laplanceImg, CV_32F, kernel, Point(-1, -1), 0, BORDER_CONSTANT);
	srcImg.convertTo(sharpImg, CV_32F);

	Mat resultImg = sharpImg - laplanceImg;
	
	resultImg.convertTo(resultImg, CV_8UC3);
	namedWindow("锐化结果", CV_WINDOW_AUTOSIZE);
	imshow("锐化结果", resultImg);

	//3、将锐化结果图像转化为二值图像
	Mat tempImg,binaryImg;
	cvtColor(srcImg, resultImg, CV_BGR2GRAY);
	threshold(resultImg, binaryImg, threshold_value, threshold_max, THRESH_BINARY | THRESH_OTSU);
	namedWindow("二值图像",CV_WINDOW_AUTOSIZE);
	imshow("二值图像",binaryImg);
	
	//4、距离变换    图像上越亮的点，代表了离零点的距离越远
	Mat distImg;

	distanceTransform(binaryImg, distImg, CV_DIST_L1, 3);

	//5、距离归一化到0-1
	normalize(distImg, distImg, 0, 1, NORM_MINMAX);
	imshow("距离图像", distImg);

	//6、使用阈值再次二值化
	threshold(distImg, distImg, .4, 1, THRESH_BINARY);
	Mat k1 = Mat::ones(13, 13, CV_8UC1);

	//7、腐蚀得到
	erode(distImg, distImg, k1, Point(-1, -1));
	imshow("二次阈值腐蚀图像", distImg);

	//8、得到标记
	Mat dist_8u;
	distImg.convertTo(dist_8u, CV_8U);
	vector<vector<Point>> contours;
	findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	//9、构建标记矩阵
	Mat markers = Mat::zeros(srcImg.size(), CV_32SC1);
	for (size_t i = 0; i < contours.size(); i++) {
		drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i)+1), -1);
	}
	circle(markers, Point(5, 5), 3, Scalar(255, 255, 255), -1);
	imshow("标记", markers * 1000);
	
	//10、分水岭算法分割
	watershed(srcImg, markers);
	Mat mark = Mat::zeros(markers.size(), CV_8UC1);
	markers.convertTo(mark, CV_8UC1);
	bitwise_not(mark, mark, Mat());
	imshow("分水岭图像", mark);

	//11、 生成随机颜色
	vector<Vec3b> colors;
	for (size_t i = 0; i < contours.size(); i++) {
		int r = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int b = theRNG().uniform(0, 255);
		colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}

	//12、可视化显示
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
	imshow("最终图像", dstImg);
	cvWaitKey(0);
}