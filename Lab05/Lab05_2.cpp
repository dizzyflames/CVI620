#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv) {

	cv::Mat image = cv::imread("./image2.jpg", cv::IMREAD_COLOR);
	float p = 0.0f;

	if (image.empty()) {
		std::cout << "could not find image" << std::endl;
	}
	else {

		cv::imshow("original", image);
		cv::imwrite("original.jpg", image);

		cv::waitKey(1);
		do {
			std::cout << "Enter a probability value beween 0 and 1: ";
			std::cin >> p;
		} while (p < 0 || p > 1);

		// adding noise
		cv::Mat x(image.rows, image.cols, CV_32FC1);

		cv::RNG rng;
		rng.fill(x, cv::RNG::UNIFORM, 0, 1.0);

		cv::Mat noise;
		image.copyTo(noise);

		float y;
		
		for (int row = 0; row < image.rows; row++) {
			for (int col = 0; col < image.cols; col++) {
				y = rng.uniform(0.0f, 1.0f);
				if (x.at<float>(row, col) < p) {
					noise.at<cv::Vec3b>(row, col)[0] = (int)(0 + (y * (255 - 0)));
					noise.at<cv::Vec3b>(row, col)[1] = (int)(0 + (y * (255 - 0)));
					noise.at<cv::Vec3b>(row, col)[2] = (int)(0 + (y * (255 - 0)));
				}
				else {
					noise.at<cv::Vec3b>(row, col)[0] = image.at<cv::Vec3b>(row, col)[0];
					noise.at<cv::Vec3b>(row, col)[1] = image.at<cv::Vec3b>(row, col)[1];
					noise.at<cv::Vec3b>(row, col)[2] = image.at<cv::Vec3b>(row, col)[2];
				}
			}
		}

		cv::imshow("noise", noise);
		cv::imwrite("noise.jpg", noise);

		// blurring image
		cv::Mat blurred;
		cv::blur(noise, blurred, cv::Size(3, 3));

		cv::imshow("blurred", blurred);
		cv::imwrite("blurred.jpg", blurred);

		// (50, 50) pixel blue value
		std::cout << "Blue value at pixel (50, 50): " << (int)image.at<cv::Vec3b>(50, 50)[0] << std::endl;

		int calcBlur = (int)image.at<cv::Vec3b>(50, 50)[0];

		// neighbour's blue value
		for (int row = 49; row < 52; row++) {
			for (int col = 49; col < 52; col++) {
				if (row != 50 || col != 50) {
					calcBlur += (int)image.at<cv::Vec3b>(row, col)[0];
					std::cout << "Blue value at pixel (" << row << ", " << col << "): " << (int)image.at<cv::Vec3b>(row, col)[0] << std::endl;
				}
			}
		}
		
		calcBlur /= 9;
		std::cout << "calculated pixel value: " << calcBlur << std::endl;

		// blurred pixel value
		std::cout << "Blurred Blue value at pixel (50, 50): " << (int)blurred.at<cv::Vec3b>(50, 50)[0] << std::endl;

		// Yes it is the same

		// de noising with bilinear filter
		cv::Mat bilinear;
		cv::Matx<float, 3, 3> kernel = (1 / 16.0f) * cv::Matx<float, 3, 3>(1, 2, 1, 2, 4, 2, 1, 2, 1);
		cv::filter2D(noise, bilinear, CV_8U, kernel);

		cv::imshow("bilinear", bilinear);
		cv::imwrite("bilinear.jpg", bilinear);

		// de noising with median filter
		cv::Mat median;
		cv::medianBlur(noise, median, 3);

		cv::imshow("median", median);
		cv::imwrite("median.jpg", median);

	}
	cv::waitKey(0);
	return 0;
}