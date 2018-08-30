//提取一幅图像中的圆形  二值分割+形态学处理+连通区域计算+纵横比计算

#include <opencv2/opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

Mat srcImage,binaryImage,dstImage;

int main(void)
{
	srcImage = imread("case3.jpg", IMREAD_GRAYSCALE);
	if (!srcImage.data)
	{
		cout << "加载图像失败！" << endl;
	}
	namedWindow("输入图像", CV_WINDOW_AUTOSIZE);
	imshow("输入图像", srcImage);
	//对图像进行二值化
	threshold(srcImage, binaryImage, 0, 255, THRESH_BINARY);
	imshow("二值图像", binaryImage);
	//形态学操作
	//闭操作 填充小的缝隙 获取大块区域
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(binaryImage, dstImage, MORPH_CLOSE, kernel, Point(-1, -1));
	imshow("闭操作图像", dstImage);
	//开操作去除小的噪声
	kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(dstImage, dstImage, MORPH_OPEN, kernel, Point(-1, -1));
	imshow("开操作图像", dstImage);
	
	//寻找轮廓
	vector<vector<Point>>contours;
	vector<Vec4i>hireachy;
	findContours(dstImage, contours, hireachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
	Point xy;//圆心坐标
	Mat resultImage = Mat::zeros(srcImage.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); ++i){
		// 面积过滤 掉小区域
		double area = contourArea(contours[i]);
		if (area < 10) {
			continue;
		}
		//根据圆形的横纵比例为1（半径之比）进行过滤
		Rect rect = boundingRect(contours[i]);
		float ratio = float(rect.width) / float(rect.height);
		if (ratio < 1.1 && ratio > 0.9){
			drawContours(resultImage, contours, i, Scalar(0, 0, 255), -1, 8, Mat(), 0, Point());
			printf("Circle Area is : %f\n", area);
			printf("Circle Length is : %f\n", arcLength(contours[i], true));
			int x = rect.x + rect.width / 2;
			int y = rect.y + rect.height / 2;
			xy = Point(x, y);
			circle(resultImage, xy, 2, Scalar(0, 0, 255), 2, 8, 0);
		}
		//cout << contours.size() << endl;
	}

	//vector<Vec3f> myCircles;
	//Mat gray_result;
	//cvtColor(resultImage, gray_result, COLOR_BGR2GRAY);
	//HoughCircles(gray_result, myCircles, CV_HOUGH_GRADIENT, 1, 7, 50, 20, 23, 100);

	Mat circleImage = srcImage.clone();
	cvtColor(circleImage, circleImage, COLOR_GRAY2BGR);
	circle(circleImage, xy, 2, Scalar(0, 0, 255), 2, 8, 0);
	/*for (int i = 0; i < myCircles.size(); i++) {
	Vec3f circleInfo = myCircles[i];
	circle(circleImage, Point(circleInfo[0], circleInfo[1]), circleInfo[2], Scalar(0, 0, 255), 1, 8, 0);
	}*/

	imshow("轮廓结果图像", circleImage);
	waitKey(0);
}