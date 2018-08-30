#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

Mat srcImage,roiImage,dstImage;

int threshold_value = 50;
int threshold_max = 255;
char* outputWindow = "����任���ֱ��";
void detectLines(int, void*);
void morhpologyLines(int, void*);

int main(void)
{
	srcImage = imread("case2.jpg", IMREAD_GRAYSCALE);
	if (!srcImage.data)
	{
		cout << "����ͼ��ʧ�ܣ�" << endl;
	}
	namedWindow("����ͼ��", CV_WINDOW_AUTOSIZE);
	imshow("����ͼ��", srcImage);
	Rect roi = Rect(10, 10, srcImage.cols - 20, srcImage.rows - 20);
	roiImage = srcImage(roi);
	imshow("ROI image", roiImage);

	//ֱ��ʹ�û���任���Ч������
	/*createTrackbar("Threshold:", outputWindow, &threshold_value, threshold_max, detectLines);
	detectLines(0, 0);*/
	//ʹ����̬ѧ������ʹ�û���任
	morhpologyLines(0, 0);
	waitKey(0);
}



//���ջ���任���ֱ�ߣ�Ч�����ã�
void detectLines(int, void*){
	Canny(roiImage, dstImage, threshold_value, threshold_value * 2, 3, false);
	//threshold(roiImage, dstImage, 0, 255, THRESH_BINARY | THRESH_OTSU);
	vector<Vec4i> lines;
	HoughLinesP(dstImage, lines, 1, CV_PI / 180.0, 30, 20.0, 0);
	cvtColor(dstImage, dstImage, COLOR_GRAY2BGR);
	for (size_t t = 0; t < lines.size(); t++) {
		Vec4i ln = lines[t];
		line(dstImage, Point(ln[0], ln[1]), Point(ln[2], ln[3]), Scalar(0, 0, 255), 2, 8, 0);
	}
	imshow(outputWindow, dstImage);
}
//��������ͨ��ͼ����̬ѧ������Ѱ��ֱ�� ����任��ȡλ����Ϣ�����ӻ�
void morhpologyLines(int, void*){
	//��ֵ��ͼ��
	Mat binaryImage, morhpImage;
	threshold(roiImage, binaryImage, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	imshow("��ֵͼ��", binaryImage);
	//��̬ѧ���� ������
	Mat kernel = getStructuringElement(MORPH_RECT,Size(20,1),Point(-1,-1));
	morphologyEx(binaryImage, morhpImage, MORPH_OPEN, kernel, Point(-1, -1));

	//��һ���Լ�⵽��ֱ�߽������Ͳ��� �ǵ���ʾ����
	kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	dilate(morhpImage, morhpImage, kernel);
	imshow("��̬ѧ����ֱ��", morhpImage);

	//����任���ֱ��
	vector<Vec4i> lines;
	HoughLinesP(morhpImage, lines, 1, CV_PI / 180.0, 40, 20.0, 0);
	Mat resultImage = roiImage.clone();
	cvtColor(resultImage, resultImage, COLOR_GRAY2BGR);
	for (size_t t = 0; t < lines.size(); t++) {
		Vec4i ln = lines[t];
		line(resultImage, Point(ln[0], ln[1]), Point(ln[2], ln[3]), Scalar(0, 0, 255), 2, 8, 0);
	}
	imshow(outputWindow, resultImage);
	return;
}
