//ɨ�赽���ļ���Ҫ��ȥ����Ŀհױ�Ե
/*
����һ�� ͨ����Ե���+�������ֻ���ֱ�߼�� �����Ӿ���ʵ��
��������ͨ����ֵ�ָ�+��̬ѧ����+Houghֱ���ҵ������Ӿ���
*/
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<iostream>
#include<math.h>

using namespace cv;
using namespace std;

Mat srcImg,grayImg,dstImg;
char* outputWindow = "���������";
char* resultWindow = "���ս��";
int threshold_value = 100;
int threshold_max = 255;
void CallBackDemo(int, void*);

int main(void)
{
	srcImg = imread("case1.jpg");
	if (!srcImg.data)
	{
		cout << "����ͼ��ʧ�ܣ�" << endl;
	}
	namedWindow("����ͼ��", CV_WINDOW_AUTOSIZE);
	imshow("����ͼ��", srcImg);



	namedWindow(outputWindow, CV_WINDOW_AUTOSIZE);
	namedWindow(resultWindow, CV_WINDOW_AUTOSIZE);

	//imshow("���ͼ��", dstImg);
	createTrackbar("Threshold:", outputWindow, &threshold_value, threshold_max, CallBackDemo);
	CallBackDemo(0, 0);
	waitKey(0);
}
//���շ���һ�����̽����б�
void CallBackDemo(int, void*){
	//ת��Ϊ�Ҷȿռ�
	cvtColor(srcImg, grayImg, COLOR_BGR2GRAY);
	Mat cannyImg;
	//Canny��Ե�����ȡ��Ե
	Canny(grayImg, cannyImg, threshold_value, threshold_value * 2, 3, false);
	//������ͨ��������������������Ϣ
	vector<vector<Point>>contours;
	vector<Point>hireachy;
	findContours(cannyImg, contours, hireachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	 
	int minWidth = srcImg.cols * 0.75;
	int minHeight = srcImg.rows * 0.75;
	RNG rng(12345);
	Mat drawImg = Mat::zeros(srcImg.size(), CV_8UC3);
	Rect boundary_box;
	//Ѱ����С��Ӿ���
	for (size_t t = 0; t < contours.size(); ++t){
		RotatedRect minRect = minAreaRect(contours[t]);
		//������ת�Ƕ�
		float rectangle = abs(minRect.angle);
		printf("current angle is:%f", rectangle);
		if (minRect.size.width > minWidth && minRect.size.height >minHeight && minRect.size.width < (srcImg.cols - 5)){
			//�������ε��ĸ��ǵ�
			Point2f pts[4];
			minRect.points(pts);
			boundary_box = minRect.boundingRect();
			Scalar scalar_color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			for (int i = 0; i < 4; ++i){
				line(drawImg, pts[i], pts[(i + 1) % 4], 2, 8, 0);
			}
		}
	}
	//�������ҵ���ROI�����ȡ����ʾ
	if (boundary_box.width > 0 && boundary_box.height > 0){
		Mat ROIImg = srcImg(boundary_box);
		//imshow(outputWindow, drawImg);
		imshow(resultWindow, ROIImg);
	}

}
