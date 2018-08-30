#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

int main(int argc, char** argv) {
	Mat img1 = imread("image1.jpg", IMREAD_GRAYSCALE);
	Mat img2 = imread("image2.jpg", IMREAD_GRAYSCALE);
	if (!img1.data || !img2.data) {
		return -1;
	}
	imshow("object image", img1);
	imshow("object in scene", img2);

	//SURF特征提取描述子
	int minHessian = 400;
	Ptr<SURF> detector = SURF::create(minHessian);
	//定义描述子和场景
	vector<KeyPoint> keypoints_obj;
	vector<KeyPoint> keypoints_scene;
	Mat descriptor_obj, descriptor_scene;
	detector->detectAndCompute(img1, Mat(), keypoints_obj, descriptor_obj);
	detector->detectAndCompute(img2, Mat(), keypoints_scene, descriptor_scene);

	// 匹配 定义FLANN描述子
	FlannBasedMatcher matcher;
	vector<DMatch> matches;
	matcher.match(descriptor_obj, descriptor_scene, matches);

	// 发现最适合的点
	double minDist = 1000;
	double maxDist = 0;
	for (int i = 0; i < descriptor_obj.rows; i++) {
		double dist = matches[i].distance;
		if (dist > maxDist) {
			maxDist = dist;
		}
		if (dist < minDist) {
			minDist = dist;
		}
	}
	printf("max distance : %f\n", maxDist);
	printf("min distance : %f\n", minDist);
	vector<DMatch> goodMatches;
	for (int i = 0; i < descriptor_obj.rows; i++) {
		double dist = matches[i].distance;
		//距离越小越好 此处定义小于最小值的3倍
		if (dist < max(3 * minDist, 0.02)) {
			goodMatches.push_back(matches[i]);
		}
	}

	Mat matchesImg;
	/*drawMatches其中参数如下：
	* img1 – 源图像1
	* keypoints1 –源图像1的特征点.
	* img2 – 源图像2.
	* keypoints2 – 源图像2的特征点
	* matches1to2 – 源图像1的特征点匹配源图像2的特征点[matches[i]] .
	* outImg – 输出图像具体由flags决定.
	* matchColor – 匹配的颜色（特征点和连线),若matchColor==Scalar::all(-1)，颜色随机.
	* singlePointColor – 单个点的颜色，即未配对的特征点，若matchColor==Scalar::all(-1)，颜色随机.
	matchesMask – Mask决定哪些点将被画出，若为空，则画出所有匹配点.
	* flags – Fdefined by DrawMatchesFlags.
	*/
	drawMatches(img1, keypoints_obj, img2, keypoints_scene, goodMatches, matchesImg, Scalar::all(-1),
		Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS
		);
	imshow("Flann Matching Result", matchesImg);

	waitKey(0);
	return 0;
}