//�ǵ���Կ�����������Ե����ֱ�����Ϲ��ɵĻ�Ԫ
//Harris������ת������ �����г߶Ȳ�����  ���ڿ��Ծ�ȷ��λ�Ķ�ά���� λ��������Ե�Ľ��㴦

#include<iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace std;
using namespace cv;

Mat srcImg, grayImg, dstImg;



RNG rng(12345);
const char* outputwindow = "resultImg";
void onCornerHarris(int, void*);//�ص�����
int threshold_value = 30;
int threshold_max = 175;

int main(void){

	srcImg = imread("image1.jpg");
	if (!srcImg.data)
	{
		cout << "ͼ�����ʧ��" << endl;
		return -1;
	}
	namedWindow("����ͼ��", CV_WINDOW_AUTOSIZE);
	imshow("����ͼ��", srcImg);
	namedWindow(outputwindow, CV_WINDOW_AUTOSIZE);
	cvtColor(srcImg,grayImg,CV_BGR2GRAY);

	createTrackbar("��ֵ��", outputwindow, &threshold_value, threshold_max, onCornerHarris);
	onCornerHarris(0, 0);

	cvWaitKey(0);
}
void onCornerHarris(int, void*){
	Mat normImg;
	Mat scaledImage;//���Ա任���8λ�޷�������ͼ
	Mat tempImg = srcImg.clone();

	dstImg = Mat::zeros(srcImg.size(), CV_32FC1);
	//�ǵ���
	cornerHarris(grayImg, dstImg, 2, 3, 0.04, BORDER_DEFAULT);
	//��һ����ת��
	normalize(dstImg, normImg, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(normImg, scaledImage);
	//���ƣ�����⵽�ķ�����ֵ�����Ľǵ���Ƴ���
	for (int j = 0; j < normImg.rows; j++){
		for (int i = 0; i < normImg.cols; i++){
			if ((int)normImg.at<float>(j, i) > (threshold_value + 80)){
				circle(tempImg, Point(i, j), 5, Scalar(10, 10, 255), 2, 8, 0);
				circle(scaledImage, Point(i, j), 5, Scalar(0, 10, 255), 2, 8, 0);
			}
		}
	}
	imshow("�ǵ���", tempImg);
	imshow("���б任", scaledImage);

}