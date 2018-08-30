#include<iostream>
#include<opencv2\opencv.hpp>

using namespace std;
using namespace cv;

int main(void){

	Mat src, result;
	src = imread("test.jpg", 1);
	if (!src.data)
	{
		cout << "ͼ�����ʧ��" << endl;
		return -1;
	}

	int offsetx = src.channels();
	
	//��ȡͼ��ʵ�ʵĿ�͸�
	int rows = src.rows;
	int cols = (src.cols - 1) * offsetx;// src.channels();
	namedWindow("srcimage", WINDOW_AUTOSIZE);
	imshow("srcimage", src);
	
	result = Mat::zeros(src.size(), src.type());
	
	double time = getTickCount();
	
	for (int row = 1; row < (rows - 1); row++)
	{
		//��ȡ��ǰ�� ���������е���ָ��
		const uchar* previous = src.ptr<uchar>(row - 1);
		const uchar* current = src.ptr<uchar>(row);
		const uchar* next = src.ptr<uchar>(row + 1);
		uchar* output = result.ptr<uchar>(row);
		for (int col = 0; col < cols; col++)
		{
			output[col] = saturate_cast<uchar>(5 * current[col] - (current[col - offsetx] + current[col + offsetx]
				+ previous[col] + next[col]));  //Ҫ��ͨ���������� ��������ֵ�����ж�
		}
	}

	double timeconsume = (double)((getTickCount() - time) / getTickFrequency());//�����ʱ
	cout << "��ʱ:" << timeconsume << "��" << endl;
	/*
	���� 2.  ����OpenCV_APIʵ����Ĥ����
	Mat kernel = (Mat_<char>(3,3)<<0,-1,0,0,5,0,0,-1,0);
	filter2D(src,result,src.depth(),kernel); src.depth()��ʾλͼ��� ��32 24 8 
	*/
	namedWindow("��Ĥ������ͼ��", WINDOW_AUTOSIZE);
	imshow("dstimage",result);
	waitKey();
}