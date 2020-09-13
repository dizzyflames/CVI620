#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv) {
	char option;
	cv::Mat image, image2, image3;
	float alpha = 0;

	std::cout << "Select either option a or b: ";
	std::cin >> option;
	

	if (option == 'a') {
		image = cv::imread("./image1.jpg", cv::IMREAD_COLOR);

		cv::namedWindow("my image", cv::WINDOW_AUTOSIZE);

		if (image.empty()) {
			std::cout << "could not find image" << std::endl;
		}
		else {
			cv::imshow("my image", image);
			cv::waitKey(1);

			image.copyTo(image2);
			image2 += cv::Scalar(100, 100, 100);

			cv::imshow("added", image2);
			cv::waitKey(1);
			image2.copyTo(image3);
			image3 *= 1.5;

			cv::imshow("multiplied", image3);
			cv::waitKey(1);
			std::cout << "original" << std::endl;
			std::cout << (int)image.at<cv::Vec3b>(10, 20)[0] << std::endl;
			std::cout << (int)image.at<cv::Vec3b>(10, 20)[1] << std::endl;
			std::cout << (int)image.at<cv::Vec3b>(10, 20)[2] << std::endl << std::endl;

			std::cout << "addition" << std::endl;
			std::cout << (int)image2.at<cv::Vec3b>(10, 20)[0] << std::endl;
			std::cout << (int)image2.at<cv::Vec3b>(10, 20)[1] << std::endl;
			std::cout << (int)image2.at<cv::Vec3b>(10, 20)[2] << std::endl << std::endl;

			std::cout << "multiplication" << std::endl;
			std::cout << (int)image3.at<cv::Vec3b>(10, 20)[0] << std::endl;
			std::cout << (int)image3.at<cv::Vec3b>(10, 20)[1] << std::endl;
			std::cout << (int)image3.at<cv::Vec3b>(10, 20)[2] << std::endl << std::endl;

			cv::waitKey(0);
		}
	}
	else if (option == 'b') {
		image = cv::imread("./image1.jpg", cv::IMREAD_COLOR);

		cv::imshow("image1", image);

		image2 = cv::imread("./image2.jpg", cv::IMREAD_COLOR);

		cv::imshow("image2", image2);

		cv::waitKey(1);

		do {
			std::cout << "enter a value between 0 and 1: ";
			std::cin >> alpha;
		} while (alpha < 0 || alpha > 1);

		cv::Mat blend = (1 - alpha) * image + alpha * image2;
		
		cv::MatND hist;
		int hbins = 8, sbins = 8;
		int histSize[] = { hbins, sbins };

		float hranges[] = { 0, 180 };
		float sranges[] = { 0, 256 };
		const float* ranges[] = { hranges, sranges };

		int channels[] = { 0 };
		cv::calcHist(&blend, 1, channels, cv::Mat(), hist, 1, histSize, ranges, true, false);

		cv::imshow("blended", blend);
		cv::waitKey(0);

		for (int i = 0; i < hbins; i++) {

			std::cout << hist.at<float>(i) << std::endl;
		}
		
	}

	return 0;
}