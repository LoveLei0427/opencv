//�����������Ե��� �ǵ�ͼ����Ⱥ��϶� �Զ���Ľ�
#include<iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace std;
using namespace cv;



Mat srcImg;
int radius = 3;
int thickness = 2;
Scalar color = Scalar(0, 0, 255);

const char* outputwindow = "resultImg";


class HarrisDetector{
private:
	//��ʾ�ǵ�ǿ�ȵ�32λ����ͼ��
	cv::Mat cornerStrength;
	//��ֵ����Ƕȵ�32λ����ͼ��
	cv::Mat cornerTh;
	//�ڲ��ֲ�����ֵͼ��
	cv::Mat localMax;

	//��ʾƽ�����������صĳߴ�
	int neighbourhood;
	//�ݶȼ�����˲����׾���С
	int aperturesize;
	//Harris����
	double k;

	//��ֵ��������ǿ��
	double maxStrength;
	//����õ����ڲ���ֵ
	double innerThreshold;
	//�Ǽ���ֵ���Ƶ��������صĳߴ�
	int nonMaxSize;
	//�Ǽ���ֵ���Ƶĺ�
	cv::Mat kernel;
	
public:
	HarrisDetector():neighbourhood(3), aperturesize(3), k(0.01), maxStrength(0.0), innerThreshold(0.01), nonMaxSize(3){
		//setLocalMaxWindowSize(nonMaxSize);//�����Ǽ���ֵ���Ƶĺ�
	}
	
	
public:
	//����ÿ�����ص�Harrisֵ
	void detect(const cv::Mat image);

	//�����ض�����ֵ��ȡ������ 
	cv::Mat getCornerMap(double qualityLevel);
	//���ض�ֵ�Ľǵ�ͼ��
	void getCorners(std::vector<cv::Point>&points, double qualityLevel);
	//void getCorners(std::vector<cv::Point>&points, const cv::Mat& cornerMap);

	//�������λ�û���Բ��
	void drawCircle(cv::Mat &image, const std::vector<cv::Point>&points, cv::Scalar color, int radius, int thickness);
};


int main(void){

	srcImg = imread("image1.jpg");
	if (!srcImg.data)
	{
		cout << "ͼ�����ʧ��" << endl;
		return -1;
	}
	namedWindow("����ͼ��", CV_WINDOW_AUTOSIZE);
	imshow("����ͼ��", srcImg);
	//namedWindow(outputwindow, CV_WINDOW_AUTOSIZE);

	HarrisDetector harris;
	harris.detect(srcImg);
	std::vector<cv::Point>pts;
	harris.getCorners(pts, 0.01);
	harris.drawCircle(srcImg, pts,color,3,2);

	cvWaitKey(0);

}

//1������ÿ�����ص�Harrisֵ
void HarrisDetector::detect(const cv::Mat image){
	//Harris����
	cv::cornerHarris(srcImg, cornerStrength, neighbourhood, aperturesize, k);
	//�ڲ���ֵ����
	double minStrength;
	cv::minMaxLoc(cornerStrength,&minStrength,&maxStrength);
	
	//�ֲ����ֵ����
	cv::Mat tempImg;
	cv::dilate(cornerStrength, tempImg, cv::Mat());
	cv::compare(cornerStrength, tempImg, localMax, cv::CMP_EQ);
}

//2����ֵ������Ϊ����õ����Harrisֵ�Ĳ���
cv::Mat HarrisDetector::getCornerMap(double qualityLevel){
	cv::Mat cornerMap;
	//�Խǵ�ͼ�������ֵ��
	innerThreshold = qualityLevel * maxStrength;
	cv::threshold(cornerStrength, cornerTh, innerThreshold, 255, THRESH_BINARY);
	//ת��Ϊ8λͼ
	cornerTh.convertTo(cornerMap, CV_8U);
	//�Ǽ���ֵ���� 
	bitwise_and(cornerMap, localMax, cornerMap);
	return cornerMap;
}

//3.1����Harrisֵ�õ���������
void HarrisDetector::getCorners(std::vector<cv::Point>&points, double qualityLevel){
	//�õ��ǵ�ͼ
	cv::Mat cornerMap = getCornerMap(qualityLevel);
	//�õ��ǵ�
	//getCorners(points, cornerMap);
}
////3.2�ɽǵ�ͼ���ȡ������
//void HarrisDetector::getCorners(std::vector<cv::Point>&points, const cv::Mat& cornerMap){
//	//�������صõ���������
//	for (int y = 0; y < cornerMap.rows; ++y){
//		const uchar* rowPtr = cornerMap.ptr<uchar>(y);
//		for (int x = 0; x < cornerMap.cols; ++x){
//			//�����������
//			if (rowPtr[x]){
//				points.push_back(cv::Point(x, y));
//			}
//		}
//	}
//}

//4���������λ�û���Բ��
void HarrisDetector::drawCircle(cv::Mat &image, const std::vector<cv::Point>&points, Scalar color, int radius, int  thickness){
	std::vector<cv::Point>::const_iterator it = points.begin();
	//�����нǵ����Բ��
	if (it != points.end()){
		cv::circle(image, *it, radius, color, thickness);
		++it;
	}
}
