#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp> //


#include <iostream>
using namespace cv;
using namespace std;


int main(int argc, const char *argv[]){
	Mat image1 = imread("image1.jpg", 0);// load as grayscale
	Mat image2 = imread("image1.jpg", 0);

	//sift特征检测器
	SiftFeatureDetector detector;
	vector<KeyPoint> keypoints1, keypoints2;
	detector.detect(image1, keypoints1);
	detector.detect(image2, keypoints2);
	cout << "# keypoints of image1 :" << keypoints1.size() << endl;
	cout << "# keypoints of image2 :" << keypoints2.size() << endl;
	
	Mat descriptors1, descriptors2;
	//描述子提取器
	Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create("SIFT");
	extractor->compute(image1, keypoints1, descriptors1);
	extractor->compute(image2, keypoints2, descriptors2);
	//描述子匹配器
	BFMatcher bfmatcher(NORM_L2, true);
	vector<DMatch> matches;
	bfmatcher.match(descriptors1, descriptors2, matches);
	cout << "# matches : " << matches.size() << endl;


	//显示匹配结果图像
	Mat output;
	drawMatches(image1, keypoints1, image2, keypoints2, matches, output);
	imshow("image matches result", output);
	waitKey(0);


	return 0;
}
