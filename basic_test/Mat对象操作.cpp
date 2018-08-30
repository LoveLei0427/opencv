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
		cout << "����ͼ��ʧ��" << endl;
		return -1;
	}
	namedWindow("srcimage", WINDOW_AUTOSIZE);
	imshow("srcimage", src);
	////����1
	//dst = Mat(src.size(), src.type());
	//dst = Scalar(0, 0, 0); //Scalar��ÿ��ͨ����ֵ
	
	////����2
	//Mat dst2 = src.clone();

	
	////����3
	//Mat dst3;
	//src.cotyTo(dst3);
	
	Mat dst2;
	namedWindow("outputimage2", WINDOW_AUTOSIZE);
	cvtColor(src, dst2, CV_BGR2GRAY);
	cout << "ԭͼ��ͨ����:" << src.channels() << endl;
	cout << "������ͼ��ͨ������" << dst2.channels() << endl;
	

	int cols = dst2.cols;
	int rows = dst2.rows;
	const uchar* firstpixel = dst2.ptr<uchar>(0);
	//cout << "��һ�����ص�����ֵ:" << *firstpixel << endl;
	printf("��һ�����ص�����ֵ�ǣ�%d\n", *firstpixel);
	imshow("outputimage2", dst2);

	Mat mat(3, 3, CV_8UC3, Scalar(0, 0, 255));
	cout << mat << endl;

	waitKey(0);
}