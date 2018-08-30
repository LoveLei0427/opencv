//扫描到的文件需要切去多余的空白边缘
/*
方法一、 通过边缘检测+轮廓发现或者直线检测 最大外接矩形实现
方法二、通过二值分割+形态学方法+Hough直线找到最大外接矩形
*/
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<iostream>
#include<math.h>

using namespace cv;
using namespace std;

Mat srcImg,grayImg,dstImg;
char* outputWindow = "轮廓检测结果";
char* resultWindow = "最终结果";
int threshold_value = 100;
int threshold_max = 255;
void CallBackDemo(int, void*);

int main(void)
{
	srcImg = imread("case1.jpg");
	if (!srcImg.data)
	{
		cout << "加载图像失败！" << endl;
	}
	namedWindow("输入图像", CV_WINDOW_AUTOSIZE);
	imshow("输入图像", srcImg);



	namedWindow(outputWindow, CV_WINDOW_AUTOSIZE);
	namedWindow(resultWindow, CV_WINDOW_AUTOSIZE);

	//imshow("结果图像", dstImg);
	createTrackbar("Threshold:", outputWindow, &threshold_value, threshold_max, CallBackDemo);
	CallBackDemo(0, 0);
	waitKey(0);
}
//按照方法一的流程进行切边
void CallBackDemo(int, void*){
	//转换为灰度空间
	cvtColor(srcImg, grayImg, COLOR_BGR2GRAY);
	Mat cannyImg;
	//Canny边缘检测提取边缘
	Canny(grayImg, cannyImg, threshold_value, threshold_value * 2, 3, false);
	//保存连通区域的轮廓数组和拓扑信息
	vector<vector<Point>>contours;
	vector<Point>hireachy;
	findContours(cannyImg, contours, hireachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	 
	int minWidth = srcImg.cols * 0.75;
	int minHeight = srcImg.rows * 0.75;
	RNG rng(12345);
	Mat drawImg = Mat::zeros(srcImg.size(), CV_8UC3);
	Rect boundary_box;
	//寻找最小外接矩形
	for (size_t t = 0; t < contours.size(); ++t){
		RotatedRect minRect = minAreaRect(contours[t]);
		//定义旋转角度
		float rectangle = abs(minRect.angle);
		printf("current angle is:%f", rectangle);
		if (minRect.size.width > minWidth && minRect.size.height >minHeight && minRect.size.width < (srcImg.cols - 5)){
			//画出矩形的四个角点
			Point2f pts[4];
			minRect.points(pts);
			boundary_box = minRect.boundingRect();
			Scalar scalar_color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			for (int i = 0; i < 4; ++i){
				line(drawImg, pts[i], pts[(i + 1) % 4], 2, 8, 0);
			}
		}
	}
	//将最终找到的ROI区域截取并显示
	if (boundary_box.width > 0 && boundary_box.height > 0){
		Mat ROIImg = srcImg(boundary_box);
		//imshow(outputWindow, drawImg);
		imshow(resultWindow, ROIImg);
	}

}
