//拍摄的图像非规则正方形 需要透视变换为正确形状
//二值分割 形态学处理  Hough直线    透视变换
#include <opencv2/opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

Mat srcImage, grayImage, binaryImage, dstImage,resultImage;

int main(void)
{
	srcImage = imread("case5.png");// , IMREAD_GRAYSCALE);
	if (!srcImage.data)
	{
		cout << "加载图像失败！" << endl;
	}
	namedWindow("输入图像", CV_WINDOW_AUTOSIZE);
	imshow("输入图像", srcImage);

	//1、转换为灰度
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);

	//2、二值分割
	threshold(grayImage, binaryImage, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	imshow("二值图像", binaryImage);

	//3、形态学操作
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	//闭操作将小的间隙填充
	morphologyEx(binaryImage, dstImage, MORPH_CLOSE, kernel, Point(-1, -1), 3);
	imshow("形态学腐蚀图像", dstImage);

	//4、最大轮廓发现
	bitwise_not(dstImage, dstImage, Mat());
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;
	findContours(dstImage, contours, hireachy, CV_RETR_TREE, CHAIN_APPROX_SIMPLE, Point());

	//5、轮廓绘制
	int width = srcImage.cols;
	int height = srcImage.rows;
	Mat drawImage = Mat::zeros(srcImage.size(), CV_8UC3);
	for (size_t t = 0; t < contours.size(); t++) {
		Rect rect = boundingRect(contours[t]);
		if (rect.width > width / 2 && rect.width < width - 5) {
			drawContours(drawImage, contours, static_cast<int>(t), Scalar(0, 0, 255), 2, 8, hireachy, 0, Point());
		}
	}
	imshow("轮廓图像", drawImage);

	vector<Vec4i> lines;
	Mat contoursImg;
	int accu = min(width*0.5, height*0.5);
	cvtColor(drawImage, contoursImg, COLOR_BGR2GRAY);
	HoughLinesP(contoursImg, lines, 1, CV_PI / 180.0, accu, accu, 0);
	Mat linesImage = Mat::zeros(srcImage.size(), CV_8UC3);
	for (size_t t = 0; t < lines.size(); t++) {
		Vec4i ln = lines[t];
		line(linesImage, Point(ln[0], ln[1]), Point(ln[2], ln[3]), Scalar(0, 0, 255), 2, 8, 0);
	}
	printf("直线数量 : %d\n", lines.size());
	imshow("直线图像", linesImage);

	//6、寻找与定位上下左右四条直线
	int deltah = 0;
	//定义四条直线
	Vec4i topLine, bottomLine,leftLine, rightLine;

	for (int i = 0; i < lines.size(); i++) {
		Vec4i ln = lines[i];
		deltah = abs(ln[3] - ln[1]);//y坐标相减
		if (ln[3] < height / 2.0 && ln[1] < height / 2.0 && deltah < accu - 1) {
			if (topLine[3] > ln[3] && topLine[3]>0) {
				topLine = lines[i];
			}
			else {
				topLine = lines[i];
			}
		}
		if (ln[3] > height / 2.0 && ln[1] > height / 2.0 && deltah < accu - 1) {
			bottomLine = lines[i];
		}
		if (ln[0] < width / 2.0 && ln[2] < width / 2.0) {
			leftLine = lines[i];
		}
		if (ln[0] > width / 2.0 && ln[2] > width / 2.0) {
			rightLine = lines[i];
		}
	}
	cout << "上方直线 : p1(x, y) = " << topLine[0] << "," << topLine[1] << " p2(x, y) = " << topLine[2] << "," << topLine[3] << endl;
	cout << "底部直线 : p1(x, y) = " << bottomLine[0] << "," << bottomLine[1] << " p2(x, y) = " << bottomLine[2] << "," << bottomLine[3] << endl;
	cout << "左侧直线 : p1(x, y) = " << leftLine[0] << "," << leftLine[1] << " p2(x, y) = " << leftLine[2] << "," << leftLine[3] << endl;
	cout << "右侧直线 : p1(x, y) = " << rightLine[0] << "," << rightLine[1] << " p2(x, y) = " << rightLine[2] << "," << rightLine[3] << endl;

	//7、拟合四条直线方程 根据 y=kx+b 求得
	float k1, c1;
	k1 = float(topLine[3] - topLine[1]) / float(topLine[2] - topLine[0]);
	c1 = topLine[1] - k1*topLine[0];
	float k2, c2;
	k2 = float(bottomLine[3] - bottomLine[1]) / float(bottomLine[2] - bottomLine[0]);
	c2 = bottomLine[1] - k2*bottomLine[0];
	float k3, c3;
	k3 = float(leftLine[3] - leftLine[1]) / float(leftLine[2] - leftLine[0]);
	c3 = leftLine[1] - k3*leftLine[0];
	float k4, c4;
	k4 = float(rightLine[3] - rightLine[1]) / float(rightLine[2] - rightLine[0]);
	c4 = rightLine[1] - k4*rightLine[0];

	//8、四条直线交点
	Point p1; // 左上角
	p1.x = static_cast<int>((c1 - c3) / (k3 - k1));
	p1.y = static_cast<int>(k1*p1.x + c1);
	Point p2; // 右上角
	p2.x = static_cast<int>((c1 - c4) / (k4 - k1));
	p2.y = static_cast<int>(k1*p2.x + c1);
	Point p3; // 左下角
	p3.x = static_cast<int>((c2 - c3) / (k3 - k2));
	p3.y = static_cast<int>(k2*p3.x + c2);
	Point p4; // 右下角
	p4.x = static_cast<int>((c2 - c4) / (k4 - k2));
	p4.y = static_cast<int>(k2*p4.x + c2);
	cout << "p1(x, y)=" << p1.x << "," << p1.y << endl;
	cout << "p2(x, y)=" << p2.x << "," << p2.y << endl;
	cout << "p3(x, y)=" << p3.x << "," << p3.y << endl;
	cout << "p4(x, y)=" << p4.x << "," << p4.y << endl;

	//9、显示四个点坐标
	circle(linesImage, p1, 2, Scalar(255, 0, 0), 2, 8, 0);
	circle(linesImage, p2, 2, Scalar(255, 0, 0), 2, 8, 0);
	circle(linesImage, p3, 2, Scalar(255, 0, 0), 2, 8, 0);
	circle(linesImage, p4, 2, Scalar(255, 0, 0), 2, 8, 0);
	line(linesImage, Point(topLine[0], topLine[1]), Point(topLine[2], topLine[3]), Scalar(0, 255, 0), 2, 8, 0);
	imshow("四个角点", linesImage);

	//10、透视变换
	vector<Point2f> src_corners(4);
	src_corners[0] = p1;
	src_corners[1] = p2;
	src_corners[2] = p3;
	src_corners[3] = p4;

	vector<Point2f> dst_corners(4);
	dst_corners[0] = Point(0, 0);
	dst_corners[1] = Point(width, 0);
	dst_corners[2] = Point(0, height);
	dst_corners[3] = Point(width, height);

	//11、获取透视变换矩阵
	Mat warpmatrix = getPerspectiveTransform(src_corners, dst_corners);
	warpPerspective(srcImage, resultImage, warpmatrix, resultImage.size(), INTER_LINEAR);
	namedWindow("最终结果", CV_WINDOW_AUTOSIZE);
	imshow("最终结果", resultImage);
	cvWaitKey(0);
}