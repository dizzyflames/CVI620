#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv) {

	cv::Mat image = cv::imread("./image1.jpg", cv::IMREAD_COLOR);
	//cv::Mat image2;

	//cv::namedWindow("my image", cv::WINDOW_AUTOSIZE);

	if (image.empty()) {
		std::cout << "could not find image" << std::endl;
	}
	else {
		cv::imshow("original", image);
		
		cv::cvtColor(image, image, CV_BGR2GRAY);

		cv::imshow("grayscale", image);

		cv::equalizeHist(image, image);

		cv::imshow("equalized", image);
	}
	cv::waitKey(0);
	return 0;
}