/*
* Name: Jason Ho
* Student No: 129410171
*/

#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"

int main(int argc, char** argv)
{

	// read images
	cv::Mat img_1 = cv::imread("Project_4_files/alphabet_03.png", cv::IMREAD_COLOR); //alphabet_01.png 2, 3
	cv::Mat img_2 = cv::imread("Project_4_files/template_Z.png", cv::IMREAD_COLOR); //template_A.png F, P, Z

	if (!img_1.data || !img_2.data)
	{
		std::cout << " failed to read one of the images " << std::endl;
		return -1;
	}

	// adding noise to img_1
	cv::Mat x(img_1.rows, img_1.cols, CV_32FC1);

	cv::RNG rng;
	rng.fill(x, cv::RNG::UNIFORM, 0, 1.0);

	float y;

	// adding salt & pepper noise
	for (int row = 0; row < img_1.rows; row++) {
		for (int col = 0; col < img_1.cols; col++) {
			y = rng.uniform(0.0f, 1.0f);
			if (x.at<float>(row, col) < 0) { // probability
				img_1.at<cv::Vec3b>(row, col)[0] = (int)(0 + (y * (255 - 0)));
				img_1.at<cv::Vec3b>(row, col)[1] = (int)(0 + (y * (255 - 0)));
				img_1.at<cv::Vec3b>(row, col)[2] = (int)(0 + (y * (255 - 0)));
			}
			else {
				img_1.at<cv::Vec3b>(row, col)[0] = img_1.at<cv::Vec3b>(row, col)[0];
				img_1.at<cv::Vec3b>(row, col)[1] = img_1.at<cv::Vec3b>(row, col)[1];
				img_1.at<cv::Vec3b>(row, col)[2] = img_1.at<cv::Vec3b>(row, col)[2];
			}
		}
	}

	// create detector, key points, and descriptors
	cv::Ptr<cv::ORB> detector = cv::ORB::create(500, 1.20000048F, 8, 15, 0, 2, cv::ORB::HARRIS_SCORE);
	std::vector<cv::KeyPoint> kp1, kp2;
	cv::Mat desc1, desc2;

	// compute keypoints and descriptors using ORB detector
	detector->detectAndCompute(img_1, cv::Mat(), kp1, desc1);
	detector->detectAndCompute(img_2, cv::Mat(), kp2, desc2);

	// confirm keypoints are valid
	if (kp1.empty() || kp2.empty()) {
		std::cout << "one of the key point vectors is empty" << std::endl;
		return -1;
	}

	// use BF matching
	cv::BFMatcher bf = cv::BFMatcher(cv::NORM_HAMMING, true);
	std::vector<cv::DMatch> matches;
	bf.match(desc1, desc2, matches);

	// print maximum and minimmum distance
	double max_dist = 0; double min_dist = 100;
	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < matches.size(); i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	printf("Max distance: %f \n", max_dist);
	printf("Min distance: %f \n", min_dist);

	//output the top 15 matches
	sort(matches.begin(), matches.end(), [](const cv::DMatch& x, const cv::DMatch& y) { return x.distance < y.distance; });
	int size = matches.size() > 15 ? 15 : matches.size();

	std::vector< cv::DMatch > good_matches;
	for (int i = 0; i < size; i++)
	{
		good_matches.push_back(matches[i]);
	}

	// output match image
	cv::Mat img_3;
	cv::drawMatches(img_1, kp1, img_2, kp2, good_matches, img_3, cv::Scalar_<double>::all(-1), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS, std::vector<char>(), 2);

	cv::resize(img_3, img_3, cv::Size(934, 454));
	cv::imshow("key point BF matching", img_3);

	for (int i = 0; i < (int)good_matches.size(); i++)
	{
		printf("-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx);
	}

	cv::waitKey(0);
	return 0;
}