//����ͼ���ж���ĸ��� 
//��ֵ�ָ� ��̬ѧ����  ����任    ��ͨ�������
#include <opencv2/opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

Mat srcImage, grayImage, binaryImage, distImage,dstImage;

int main(void)
{
	srcImage = imread("case4.jpg");// , IMREAD_GRAYSCALE);
	if (!srcImage.data)
	{
		cout << "����ͼ��ʧ�ܣ�" << endl;
	}
	namedWindow("����ͼ��", CV_WINDOW_AUTOSIZE);
	imshow("����ͼ��", srcImage);

	//1��ת��Ϊ�Ҷ�
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);

	//2����ֵ�ָ�
	threshold(grayImage, binaryImage, 0, 255, CV_THRESH_BINARY|CV_THRESH_OTSU);
	imshow("��ֵͼ��", binaryImage);
	
	//3����̬ѧ����
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	dilate(binaryImage, binaryImage, kernel, Point(-1, -1), 1);
	imshow("��̬ѧ��ʴͼ��", binaryImage);
	
	// 4������任
	bitwise_not(binaryImage, binaryImage);//�����е�ÿһλ����ȡ��
	//void distanceTransform( InputArray src, OutputArray dst,int distanceType, int maskSize );
	distanceTransform(binaryImage, distImage, CV_DIST_L2, 3);
	normalize(distImage, distImage, 0, 1.0, NORM_MINMAX);
	imshow("����任ͼ��", distImage);
	
	//5���Ծ���任��Ľ��ͼ����ж�ֵ���ָ�
	Mat distImage_8U;
	distImage.convertTo(distImage_8U, CV_8U);
	// adaptiveThreshold( InputArray src, OutputArray dst,double maxValue, int adaptiveMethod,int thresholdType, int blockSize, double C );
	adaptiveThreshold(distImage_8U, distImage_8U, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 85, 0.0);
	kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	//���Ͳ��� ʹ��������������
	dilate(distImage_8U, distImage_8U, kernel, Point(-1, -1), 2);
	imshow("����任-��ֵ��", distImage_8U);

	//6����ͨ�������
	vector<vector<Point>> contours;
	findContours(distImage_8U, contours, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	// ���ӻ����
	Mat resultImage = Mat::zeros(srcImage.size(), CV_8UC3);
	RNG rng(12345);
	for (size_t t = 0; t < contours.size(); t++) {
		drawContours(resultImage, contours, static_cast<int>(t), Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)),
			-1, 8, Mat());
	}
	printf("�ǵ����� : %d", contours.size());
	imshow("���ս��", resultImage);
	cvWaitKey(0);
}