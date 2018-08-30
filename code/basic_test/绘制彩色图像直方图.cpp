//���Ʋ�ɫͼ���ֱ��ͼ
#include<iostream>
#include<opencv2\opencv.hpp>
#include <math.h>
using namespace std;
using namespace cv;

Mat srcImg;// histImg;


int main(void)
{
	srcImg = imread("image4.jpg", 1);
	if (!srcImg.data)
	{
		cout << "����ͼ��ʧ�ܣ�" << endl;
	}

	//����ֱ��ͼ��С������ֵ��С
	int histSize = 256;

	//ֱ��ͼ��С(ֱ��ͼ��ʾ�������޸Ĳ���)
	int hist_height = 512;
	int hist_width = 1013;

	//����ͨ��
	vector<Mat>bgr_channels;
	split(srcImg, bgr_channels);

	//����ֱ��ͼ
	float range[] = { 0, 256 };
	const float *hist_range = { range };
	Mat b_hist, g_hist, r_hist;
	calcHist(&bgr_channels[0], 1, 0, Mat(), b_hist, 1, &histSize, &hist_range, true, false);
	calcHist(&bgr_channels[1], 1, 0, Mat(), g_hist, 1, &histSize, &hist_range, true, false);
	calcHist(&bgr_channels[2], 1, 0, Mat(), r_hist, 1, &histSize, &hist_range, true, false);
	
	//��һ������
	int hpt = hist_width / hist_height;//
	Mat histImg(hist_width, hist_height, CV_8UC3, Scalar(0, 0, 0));
	normalize(b_hist, b_hist, 0, hist_height, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, hist_height, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, hist_height, NORM_MINMAX, -1, Mat());
	
	int  i;
	//�ֱ����BGR3ͨ��ֱ��ͼ
	for (i = 1; i < histSize; ++i){
		line(histImg,
			Point((i - 1)*hpt, hist_height - cvRound(b_hist.at<float>(i-1))),
			Point((i)*hpt, hist_height - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 
			2, 
			1);

		line(histImg,
			Point((i - 1)*hpt, hist_height - cvRound(g_hist.at<float>(i - 1))),
			Point((i)*hpt, hist_height - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 
			2, 
			1);

		line(histImg,
			Point((i - 1)*hpt, hist_height - cvRound(r_hist.at<float>(i - 1))),
			Point((i)*hpt, hist_height - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 
			2, 
			1);

	}

	namedWindow("Դͼ��", CV_WINDOW_AUTOSIZE);
	imshow("Դͼ��", srcImg);
	namedWindow("ֱ��ͼ", CV_WINDOW_NORMAL);
	imshow("ֱ��ͼ", histImg);
	cvWaitKey(0);
}