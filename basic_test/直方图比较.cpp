//直方图比较
#include<opencv2\opencv.hpp>
#include <math.h>
using namespace std;
using namespace cv;

Mat srcImg;// histImg;


int main(void)
{
	srcImg= imread("image1.jpg", 1);
	if (!srcImg.data)
	{
		cout << "加载图像失败！" << endl;
	}
	Mat test1 = imread("image1.jpg", 1);
	Mat test2 = imread("image2.jpg", 1);
	// 使用0 1两个通道     
	int channels[] = { 0, 1 };
	MatND hist_base;
	MatND hist_test1;
	MatND hist_test2;
	Mat hsvbase, hsvtest1, hsvtest2;

	//将彩色空间转换为HSV空间
	cvtColor(srcImg, hsvbase, CV_BGR2HSV);
	cvtColor(test1, hsvtest1, CV_BGR2HSV);
	cvtColor(test2, hsvtest2, CV_BGR2HSV);

	//定义HSV空间参数
	int h_bins = 50; int s_bins = 60;
	int histSize[] = { h_bins, s_bins };

	// hue 变量从 0 to 179, saturation变量从 0 to 255     
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };
	const float* ranges[] = { h_ranges, s_ranges };

	//计算直方图并归一化
	calcHist(&hsvbase, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false);
	normalize(hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&hsvtest1, 1, channels, Mat(), hist_test1, 2, histSize, ranges, true, false);
	normalize(hist_test1, hist_test1, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&hsvtest2, 1, channels, Mat(), hist_test2, 2, histSize, ranges, true, false);
	normalize(hist_test2, hist_test2, 0, 1, NORM_MINMAX, -1, Mat());

	double basebase = compareHist(hist_base, hist_base, CV_COMP_INTERSECT);
	double basetest1 = compareHist(hist_base, hist_test1, CV_COMP_INTERSECT);
	double basetest2 = compareHist(hist_base, hist_test2, CV_COMP_INTERSECT);
	double tes1test2 = compareHist(hist_test1, hist_test2, CV_COMP_INTERSECT);
	printf("test1 compare with test2 correlation value :%f", tes1test2);



	namedWindow("test1", CV_WINDOW_AUTOSIZE);
	imshow("test1", test1);

	namedWindow("test2", CV_WINDOW_AUTOSIZE);
	imshow("test2", test2);

	cvWaitKey(0);


}
