#include<opencv2\opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main(void)
{
	Mat src, dst;
	src = imread("test.jpg");
	if (src.empty())
	{
		cout << "加载图像失败" << endl;
		return -1;
	}
	namedWindow("srcimage", WINDOW_AUTOSIZE);
	imshow("srcimage", src);
	////方法1
	//dst = Mat(src.size(), src.type());
	//dst = Scalar(0, 0, 0); //Scalar对每个通道赋值
	
	////方法2
	//Mat dst2 = src.clone();

	
	////方法3
	//Mat dst3;
	//src.cotyTo(dst3);
	
	Mat dst2;
	namedWindow("outputimage2", WINDOW_AUTOSIZE);
	cvtColor(src, dst2, CV_BGR2GRAY);
	cout << "原图像通道数:" << src.channels() << endl;
	cout << "操作后图像通道数：" << dst2.channels() << endl;
	

	int cols = dst2.cols;
	int rows = dst2.rows;
	const uchar* firstpixel = dst2.ptr<uchar>(0);
	//cout << "第一个像素点像素值:" << *firstpixel << endl;
	printf("第一个像素点像素值是：%d\n", *firstpixel);
	imshow("outputimage2", dst2);

	Mat mat(3, 3, CV_8UC3, Scalar(0, 0, 255));
	cout << mat << endl;

	waitKey(0);
}