#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
using namespace cv;
using namespace cv::xfeatures2d;

int main()
{
	Mat src1, src2;
	src1 = imread("image1.jpg");
	src2 = imread("image2.jpg");
	if (src1.empty() || src2.empty())
	{
		printf("can ont load images....\n");
		return -1;
	}
	imshow("image1", src1);
	imshow("image2", src2);

	int minHessian = 400;
	//选择SURF特征
	Ptr<SURF>detector = SURF::create(minHessian);
	std::vector<KeyPoint>keypoints1;
	std::vector<KeyPoint>keypoints2;
	Mat descriptor1, descriptor2;
	//检测关键点并计算描述符
	detector->detectAndCompute(src1, Mat(), keypoints1, descriptor1);
	detector->detectAndCompute(src2, Mat(), keypoints2, descriptor2);

	//基于Flann的描述符匹配器
	FlannBasedMatcher matcher;
	std::vector<DMatch>matches;
	//从查询集中查找每个描述符的最佳匹配
	matcher.match(descriptor1, descriptor2, matches);
	double minDist = 1000;
	double maxDist = 0;
	for (int i = 0; i < descriptor1.rows; i++)
	{
		double dist = matches[i].distance;
		printf("%f \n", dist);
		if (dist > maxDist)
		{
			maxDist = dist;
		}
		if (dist < minDist)
		{
			minDist = dist;
		}

	}
	//DMatch类用于匹配关键点描述符的
	std::vector<DMatch>goodMatches;
	for (int i = 0; i < descriptor1.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < max(2 * minDist, 0.02))
		{
			goodMatches.push_back(matches[i]);
		}
	}
	Mat matchesImg;
	drawMatches(src1, keypoints1, src2, keypoints2, goodMatches, matchesImg, Scalar::all(-1),
		Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	std::vector<Point2f>point1, point2;
	for (int i = 0; i < goodMatches.size(); i++)
	{
		point1.push_back(keypoints1[goodMatches[i].queryIdx].pt);
		point2.push_back(keypoints2[goodMatches[i].trainIdx].pt);
	}

	Mat H = findHomography(point1, point2, RANSAC);
	std::vector<Point2f>cornerPoints1(4);
	std::vector<Point2f>cornerPoints2(4);
	cornerPoints1[0] = Point(0, 0);
	cornerPoints1[1] = Point(src1.cols, 0);
	cornerPoints1[2] = Point(src1.cols, src1.rows);
	cornerPoints1[3] = Point(0, src1.rows);
	perspectiveTransform(cornerPoints1, cornerPoints2, H);

	//绘制出变换后的目标轮廓，由于左侧为图像src2故坐标点整体右移src1.cols
	line(matchesImg, cornerPoints2[0] + Point2f(src1.cols, 0), cornerPoints2[1] + Point2f(src1.cols, 0), Scalar(0, 255, 255), 4, 8, 0);
	line(matchesImg, cornerPoints2[1] + Point2f(src1.cols, 0), cornerPoints2[2] + Point2f(src1.cols, 0), Scalar(0, 255, 255), 4, 8, 0);
	line(matchesImg, cornerPoints2[2] + Point2f(src1.cols, 0), cornerPoints2[3] + Point2f(src1.cols, 0), Scalar(0, 255, 255), 4, 8, 0);
	line(matchesImg, cornerPoints2[3] + Point2f(src1.cols, 0), cornerPoints2[0] + Point2f(src1.cols, 0), Scalar(0, 255, 255), 4, 8, 0);

	//在原图上绘制出变换后的目标轮廓
	line(src2, cornerPoints2[0], cornerPoints2[1], Scalar(0, 255, 255), 4, 8, 0);
	line(src2, cornerPoints2[1], cornerPoints2[2], Scalar(0, 255, 255), 4, 8, 0);
	line(src2, cornerPoints2[2], cornerPoints2[3], Scalar(0, 255, 255), 4, 8, 0);
	line(src2, cornerPoints2[3], cornerPoints2[0], Scalar(0, 255, 255), 4, 8, 0);

	imshow("output", matchesImg);
	imshow("output2", src2);

	waitKey();
	return 0;
}