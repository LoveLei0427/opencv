#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

// ����ȫ�ֱ���
const string harris_winName = "�Զ���ǵ���";
Mat src_img, gray_img;                       // src_img��ʾԭͼ, gray_img��ʾ�Ҷ�ͼ
Mat harris_dst_img, harris_response_img;     // harris_dst_img�洢����ؾ���M������ֵ������������harris_response_img�洢��Ӧ�����Ľ��

double min_respense_value;			  // ��Ӧ�����Ľ�������е���Сֵ
double max_respense_value;			  // ��Ӧ�����Ľ�������е����ֵ

int qualityValue = 30;
int max_qualityValue = 100;              // ͨ��qualityValue/max_qualityValue�Ľ����Ϊqualitylevel��������ֵ
RNG  random_number_generator;             // ����һ�������������
void self_defining_Harris_Demo(int, void*);      //TrackBar�ص���������

// ������
int main()
{
	src_img = imread("image4.jpg");
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
	double k = 0.04;
	harris_dst_img = Mat::zeros(src_img.size(), CV_32FC(6));
	// Ŀ��ͼ��harris_dst_img�洢����ؾ���M������ֵ������������
	// ����������(��1, ��2, x1, y1, x2, y2)����ʽ�洢�����Ц�1, ��2��Mδ�������������ֵ��
	// x1, y1�Ƕ�Ӧ�ڦ�1������������x2, y2�Ƕ�Ӧ�ڦ�2������������
	// ���Ŀ�����Ϊ6ͨ������ CV_32FC(6)�ľ���

	harris_response_img = Mat::zeros(src_img.size(), CV_32FC1);
	// harris_response_img�����洢ͨ��ÿ������ֵ����Ӧ������ؾ���������õ�����Ӧֵ

	cornerEigenValsAndVecs(gray_img, harris_dst_img, blockSize, ksize, 4);
	// �ú�����������ÿ������ֵ��Ӧ������ؾ��������ֵ����������

	// ������Ӧ����ֵ
	for (int row = 0; row < harris_dst_img.rows; ++row)
	{
		for (int col = 0; col < harris_dst_img.cols; ++col)
		{
			double eigenvalue1 = harris_dst_img.at<Vec6f>(row, col)[0];     // ��ȡ����ֵ1
			double eigenvalue2 = harris_dst_img.at<Vec6f>(row, col)[1];		// ��ȡ����ֵ2
			harris_response_img.at<float>(row, col) = eigenvalue1* eigenvalue2 - k*pow((eigenvalue1 + eigenvalue2), 2);
			// ͨ����Ӧ��ʽR=��1*��2 - k*(��1+��2)*(��1+��2)������ÿ�����ض�Ӧ����Ӧֵ
		}
	}
	minMaxLoc(harris_response_img, &min_respense_value, &max_respense_value, 0, 0, Mat());   // Ѱ����Ӧ�����е���Сֵ�����ֵ
	namedWindow(harris_winName, CV_WINDOW_AUTOSIZE);
	createTrackbar("Quality Value:", harris_winName, &qualityValue, max_qualityValue, self_defining_Harris_Demo);    //����TrackBar
	self_defining_Harris_Demo(0, 0);

	waitKey(0);
	return 0;
}


//  �ص�����ʵ��
void self_defining_Harris_Demo(int, void*)
{
	if (qualityValue < 10)
	{
		qualityValue = 10;       // ����qualitylevel������ֵ
	}
	Mat result_img = src_img.clone();    // ���ͼ��
	float threshold_value = min_respense_value + (((double)qualityValue) / max_qualityValue)*(max_respense_value - min_respense_value);
	for (int row = 0; row <result_img.rows; row++)
	{
		for (int col = 0; col < result_img.cols; col++)
		{
			float resp_value = harris_response_img.at<float>(row, col);
			if (resp_value > threshold_value)
			{
				circle(result_img, Point(col, row), 2, Scalar(random_number_generator.uniform(0, 255),
					random_number_generator.uniform(0, 255), random_number_generator.uniform(0, 255)), 2, 8, 0);
			}
		}
	}
	imshow(harris_winName, result_img);
}