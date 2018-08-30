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

	//SURF������ȡ������
	int minHessian = 400;
	Ptr<SURF> detector = SURF::create(minHessian);
	//���������Ӻͳ���
	vector<KeyPoint> keypoints_obj;
	vector<KeyPoint> keypoints_scene;
	Mat descriptor_obj, descriptor_scene;
	detector->detectAndCompute(img1, Mat(), keypoints_obj, descriptor_obj);
	detector->detectAndCompute(img2, Mat(), keypoints_scene, descriptor_scene);

	// ƥ�� ����FLANN������
	FlannBasedMatcher matcher;
	vector<DMatch> matches;
	matcher.match(descriptor_obj, descriptor_scene, matches);

	// �������ʺϵĵ�
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
		//����ԽСԽ�� �˴�����С����Сֵ��3��
		if (dist < max(3 * minDist, 0.02)) {
			goodMatches.push_back(matches[i]);
		}
	}

	Mat matchesImg;
	/*drawMatches���в������£�
	* img1 �C Դͼ��1
	* keypoints1 �CԴͼ��1��������.
	* img2 �C Դͼ��2.
	* keypoints2 �C Դͼ��2��������
	* matches1to2 �C Դͼ��1��������ƥ��Դͼ��2��������[matches[i]] .
	* outImg �C ���ͼ�������flags����.
	* matchColor �C ƥ�����ɫ�������������),��matchColor==Scalar::all(-1)����ɫ���.
	* singlePointColor �C ���������ɫ����δ��Ե������㣬��matchColor==Scalar::all(-1)����ɫ���.
	matchesMask �C Mask������Щ�㽫����������Ϊ�գ��򻭳�����ƥ���.
	* flags �C Fdefined by DrawMatchesFlags.
	*/
	drawMatches(img1, keypoints_obj, img2, keypoints_scene, goodMatches, matchesImg, Scalar::all(-1),
		Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS
		);
	imshow("Flann Matching Result", matchesImg);

	waitKey(0);
	return 0;
}