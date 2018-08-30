#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

Mat srcImage,roiImage,dstImage;

int threshold_value = 50;
int threshold_max = 255;
char* outputWindow = "霍夫变换检测直线";
void detectLines(int, void*);
void morhpologyLines(int, void*);

int main(void)
{
	srcImage = imread("case2.jpg", IMREAD_GRAYSCALE);
	if (!srcImage.data)
	{
		cout << "加载图像失败！" << endl;
	}
	namedWindow("输入图像", CV_WINDOW_AUTOSIZE);
	imshow("输入图像", srcImage);
	Rect roi = Rect(10, 10, srcImage.cols - 20, srcImage.rows - 20);
	roiImage = srcImage(roi);
	imshow("ROI image", roiImage);

	//直接使用霍夫变换检测效果不好
	/*createTrackbar("Threshold:", outputWindow, &threshold_value, threshold_max, detectLines);
	detectLines(0, 0);*/
	//使用形态学操作在使用霍夫变换
	morhpologyLines(0, 0);
	waitKey(0);
}



//按照霍夫变换检测直线（效果不好）
void detectLines(int, void*){
	Canny(roiImage, dstImage, threshold_value, threshold_value * 2, 3, false);
	//threshold(roiImage, dstImage, 0, 255, THRESH_BINARY | THRESH_OTSU);
	vector<Vec4i> lines;
	HoughLinesP(dstImage, lines, 1, CV_PI / 180.0, 30, 20.0, 0);
	cvtColor(dstImage, dstImage, COLOR_GRAY2BGR);
	for (size_t t = 0; t < lines.size(); t++) {
		Vec4i ln = lines[t];
		line(dstImage, Point(ln[0], ln[1]), Point(ln[2], ln[3]), Scalar(0, 0, 255), 2, 8, 0);
	}
	imshow(outputWindow, dstImage);
}
//方法二、通过图像形态学操作来寻找直线 霍夫变换获取位置信息并可视化
void morhpologyLines(int, void*){
	//二值化图像
	Mat binaryImage, morhpImage;
	threshold(roiImage, binaryImage, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	imshow("二值图像", binaryImage);
	//形态学操作 开、闭
	Mat kernel = getStructuringElement(MORPH_RECT,Size(20,1),Point(-1,-1));
	morphologyEx(binaryImage, morhpImage, MORPH_OPEN, kernel, Point(-1, -1));

	//进一步对检测到的直线进行膨胀操作 是的显示明显
	kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	dilate(morhpImage, morhpImage, kernel);
	imshow("形态学操作直线", morhpImage);

	//霍夫变换检测直线
	vector<Vec4i> lines;
	HoughLinesP(morhpImage, lines, 1, CV_PI / 180.0, 40, 20.0, 0);
	Mat resultImage = roiImage.clone();
	cvtColor(resultImage, resultImage, COLOR_GRAY2BGR);
	for (size_t t = 0; t < lines.size(); t++) {
		Vec4i ln = lines[t];
		line(resultImage, Point(ln[0], ln[1]), Point(ln[2], ln[3]), Scalar(0, 0, 255), 2, 8, 0);
	}
	imshow(outputWindow, resultImage);
	return;
}
