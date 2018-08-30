#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
 
using namespace cv;
using namespace std;
 
// ����ȫ�ֱ���
const string ShiTomasi_winName = "Custom Shi-Tomasi Corners Detector";
Mat src_img, gray_img;                  // src_img��ʾԭͼ, gray_img��ʾ�Ҷ�ͼ
Mat ShiTomasi_dst_img;                  // ShiTomasi_dst_img�����洢ÿ�����ض�Ӧ������ؾ������С����ֵ
double min_ShiTomasi_value;            // ��С���������е���Сֵ
double max_ShiTomasi_value;		      // ��С���������е����ֵ
int ShiTomasi_qualityValue = 30;
int max_qualityValue = 100;
RNG  random_number_generator;                        // ����һ�������������
void self_defining_ShiTomasi_Demo(int, void*);      //TrackBar�ص���������
 
// ������
int main( )
{
	src_img = imread("test11.png");
	if (src_img.empty())
	{
		printf("could not load the image...\n");
		return -1;
	}
	namedWindow("ԭͼ", CV_WINDOW_AUTOSIZE);
	imshow("ԭͼ", src_img);
	cvtColor(src_img, gray_img, COLOR_BGR2GRAY);      //����ɫͼת��Ϊ�Ҷ�ͼ
	
	// ��������ֵ
	int blockSize = 3;
	int ksize = 3;
 
	// ������С����ֵ
	ShiTomasi_dst_img = Mat::zeros(src_img.size(), CV_32FC1);
	cornerMinEigenVal(gray_img, ShiTomasi_dst_img, blockSize, ksize, 4);            // ����ÿ�����ض�Ӧ������ؾ������С����ֵ
	minMaxLoc(ShiTomasi_dst_img, &min_ShiTomasi_value, &max_ShiTomasi_value, 0, 0, Mat());           //������С���ֵ
	namedWindow(ShiTomasi_winName, CV_WINDOW_AUTOSIZE);
	createTrackbar("Quality:", ShiTomasi_winName, &ShiTomasi_qualityValue, max_qualityValue, self_defining_ShiTomasi_Demo);
	self_defining_ShiTomasi_Demo(0, 0);
 
	waitKey(0);
	return 0;
}
 
 
//  �ص�����ʵ��
void self_defining_ShiTomasi_Demo(int, void*)
{
	if (ShiTomasi_qualityValue < 10)
	{
		ShiTomasi_qualityValue = 10;       // ����qualitylevel������ֵ
	}                     
	Mat result_img = src_img.clone();    // ���ͼ��
	float threshold_value = min_ShiTomasi_value + (((double)ShiTomasi_qualityValue) / max_qualityValue)*(max_ShiTomasi_value - min_ShiTomasi_value);
	for (int row = 0; row <result_img.rows; row++)
	{
		for (int col = 0; col < result_img.cols; col++)
		{
			float resp_value = ShiTomasi_dst_img.at<float>(row, col);
		if (resp_value > threshold_value)
			{
				circle(result_img, Point(col, row), 2, Scalar(random_number_generator.uniform(0, 255),
					random_number_generator.uniform(0, 255), random_number_generator.uniform(0, 255)), 2, 8, 0);
		}
		}
	}
	imshow(ShiTomasi_winName, result_img);
}
 