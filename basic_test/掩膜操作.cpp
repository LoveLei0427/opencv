#include<iostream>
#include<opencv2\opencv.hpp>

using namespace std;
using namespace cv;

int main(void){

	Mat src, result;
	src = imread("test.jpg", 1);
	if (!src.data)
	{
		cout << "图像加载失败" << endl;
		return -1;
	}

	int offsetx = src.channels();
	
	//获取图像实际的宽和高
	int rows = src.rows;
	int cols = (src.cols - 1) * offsetx;// src.channels();
	namedWindow("srcimage", WINDOW_AUTOSIZE);
	imshow("srcimage", src);
	
	result = Mat::zeros(src.size(), src.type());
	
	double time = getTickCount();
	
	for (int row = 1; row < (rows - 1); row++)
	{
		//获取当前行 和上下两行的行指针
		const uchar* previous = src.ptr<uchar>(row - 1);
		const uchar* current = src.ptr<uchar>(row);
		const uchar* next = src.ptr<uchar>(row + 1);
		uchar* output = result.ptr<uchar>(row);
		for (int col = 0; col < cols; col++)
		{
			output[col] = saturate_cast<uchar>(5 * current[col] - (current[col - offsetx] + current[col + offsetx]
				+ previous[col] + next[col]));  //要把通道计算在内 并对像素值进行判断
		}
	}

	double timeconsume = (double)((getTickCount() - time) / getTickFrequency());//计算耗时
	cout << "耗时:" << timeconsume << "秒" << endl;
	/*
	方法 2.  调用OpenCV_API实现掩膜操作
	Mat kernel = (Mat_<char>(3,3)<<0,-1,0,0,5,0,0,-1,0);
	filter2D(src,result,src.depth(),kernel); src.depth()表示位图深度 有32 24 8 
	*/
	namedWindow("掩膜操作后图像", WINDOW_AUTOSIZE);
	imshow("dstimage",result);
	waitKey();
}