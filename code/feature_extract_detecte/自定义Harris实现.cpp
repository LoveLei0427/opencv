//参数过多难以调优 角点图像离群点较多 自定义改进
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
	//表示角点强度的32位浮点图像
	cv::Mat cornerStrength;
	//阈值化后角度的32位浮点图像
	cv::Mat cornerTh;
	//内部局部极大值图像
	cv::Mat localMax;

	//表示平滑的相邻像素的尺寸
	int neighbourhood;
	//梯度计算的滤波器孔径大小
	int aperturesize;
	//Harris参数
	double k;

	//阈值计算的最大强度
	double maxStrength;
	//计算得到的内部阈值
	double innerThreshold;
	//非极大值抑制的相邻像素的尺寸
	int nonMaxSize;
	//非极大值抑制的核
	cv::Mat kernel;
	
public:
	HarrisDetector():neighbourhood(3), aperturesize(3), k(0.01), maxStrength(0.0), innerThreshold(0.01), nonMaxSize(3){
		//setLocalMaxWindowSize(nonMaxSize);//创建非极大值抑制的核
	}
	
	
public:
	//计算每个像素的Harris值
	void detect(const cv::Mat image);

	//基于特定的阈值获取特征点 
	cv::Mat getCornerMap(double qualityLevel);
	//返回二值的角点图像
	void getCorners(std::vector<cv::Point>&points, double qualityLevel);
	//void getCorners(std::vector<cv::Point>&points, const cv::Mat& cornerMap);

	//特征点的位置绘制圆形
	void drawCircle(cv::Mat &image, const std::vector<cv::Point>&points, cv::Scalar color, int radius, int thickness);
};


int main(void){

	srcImg = imread("image1.jpg");
	if (!srcImg.data)
	{
		cout << "图像加载失败" << endl;
		return -1;
	}
	namedWindow("输入图像", CV_WINDOW_AUTOSIZE);
	imshow("输入图像", srcImg);
	//namedWindow(outputwindow, CV_WINDOW_AUTOSIZE);

	HarrisDetector harris;
	harris.detect(srcImg);
	std::vector<cv::Point>pts;
	harris.getCorners(pts, 0.01);
	harris.drawCircle(srcImg, pts,color,3,2);

	cvWaitKey(0);

}

//1、计算每个像素的Harris值
void HarrisDetector::detect(const cv::Mat image){
	//Harris计算
	cv::cornerHarris(srcImg, cornerStrength, neighbourhood, aperturesize, k);
	//内部阈值计算
	double minStrength;
	cv::minMaxLoc(cornerStrength,&minStrength,&maxStrength);
	
	//局部最大值计算
	cv::Mat tempImg;
	cv::dilate(cornerStrength, tempImg, cv::Mat());
	cv::compare(cornerStrength, tempImg, localMax, cv::CMP_EQ);
}

//2、阈值被设置为计算得到最大Harris值的部分
cv::Mat HarrisDetector::getCornerMap(double qualityLevel){
	cv::Mat cornerMap;
	//对角点图像进行阈值化
	innerThreshold = qualityLevel * maxStrength;
	cv::threshold(cornerStrength, cornerTh, innerThreshold, 255, THRESH_BINARY);
	//转换为8位图
	cornerTh.convertTo(cornerMap, CV_8U);
	//非极大值抑制 
	bitwise_and(cornerMap, localMax, cornerMap);
	return cornerMap;
}

//3.1、由Harris值得到的特征点
void HarrisDetector::getCorners(std::vector<cv::Point>&points, double qualityLevel){
	//得到角点图
	cv::Mat cornerMap = getCornerMap(qualityLevel);
	//得到角点
	//getCorners(points, cornerMap);
}
////3.2由角点图像获取特征点
//void HarrisDetector::getCorners(std::vector<cv::Point>&points, const cv::Mat& cornerMap){
//	//遍历像素得到所有特征
//	for (int y = 0; y < cornerMap.rows; ++y){
//		const uchar* rowPtr = cornerMap.ptr<uchar>(y);
//		for (int x = 0; x < cornerMap.cols; ++x){
//			//如果是特征点
//			if (rowPtr[x]){
//				points.push_back(cv::Point(x, y));
//			}
//		}
//	}
//}

//4、特征点的位置绘制圆形
void HarrisDetector::drawCircle(cv::Mat &image, const std::vector<cv::Point>&points, Scalar color, int radius, int  thickness){
	std::vector<cv::Point>::const_iterator it = points.begin();
	//对所有角点绘制圆形
	if (it != points.end()){
		cv::circle(image, *it, radius, color, thickness);
		++it;
	}
}
