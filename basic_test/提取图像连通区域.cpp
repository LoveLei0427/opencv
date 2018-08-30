#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
using namespace std;
using namespace cv;

Mat src,dst;
int threshold_max = 255;
int threshold_value = 100;
RNG rng;

const char* INPUT_T = "����ͼ��";
const char* OUTPUT_T = "������";

void Contours_Demo(int, void*);

int main(int argc, char** argv) {
	src = imread("image2.jpg");
	if (src.empty()) {
		printf("��ȡͼ��ʧ��");
		return -1;
	}
	namedWindow(INPUT_T, CV_WINDOW_AUTOSIZE);
	namedWindow(OUTPUT_T, CV_WINDOW_AUTOSIZE);

	imshow(INPUT_T, src);
	cvtColor(src, src, CV_BGR2GRAY); //����ת�� Ϊ��ֵͼ��
	dst = Mat::zeros(src.size(), CV_8UC3);
	
	const char* trackbar_title = "��ֵ:";
	createTrackbar(trackbar_title, OUTPUT_T, &threshold_value, threshold_max, Contours_Demo);
	Contours_Demo(0, 0);

	waitKey(0);
	return 0;
}

void Contours_Demo(int, void*) {
	Mat canny_output;
	vector<vector<Point>>contours;//��������Ϊ��ά�ĸ�����
	vector<Vec4i> hierachy; //�߽�����
	Canny(src, canny_output, threshold_value, threshold_value * 2, 3, false);
	findContours(canny_output, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	RNG rng(12345);
	for (size_t i = 0; i < contours.size(); ++i){
		//Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		Scalar color = Scalar(0,0,255);

		drawContours(dst, contours,i, color, 2, 8, hierachy, 0, Point(0, 0));
	}
	imshow(OUTPUT_T, dst);
}
