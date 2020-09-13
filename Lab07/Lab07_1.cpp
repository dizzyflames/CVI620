#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv) {

	cv::Mat image = cv::imread("./image.jpg", cv::IMREAD_GRAYSCALE);
	float p = 0.0f;

	if (image.empty()) {
		std::cout << "could not find image" << std::endl;
	}
	else {

		cv::imshow("Grayscale", image);
		cv::imwrite("Grayscale.jpg", image);

		cv::waitKey(1);

		// finding vertical edges using 3x3 vertical kernel
		cv::Mat vertical;
		cv::Matx<float, 3, 3> vKernel = cv::Matx<float, 3, 3>(-1, 0, 1, -2, 0, 2, -1, 0, 1);
		cv::filter2D(image, vertical, CV_8U, vKernel);

		cv::imshow("Vertical Edges", vertical);
		cv::imwrite("Vertical.jpg", vertical);


		// finding horizontal edges using 3x3 horizontal kernel
		cv::Mat horizontal;
		cv::Matx<float, 3, 3> hKernel = cv::Matx<float, 3, 3>(-1, -2, -1, 0, 0, 0, 1, 2, 1);
		cv::filter2D(image, horizontal, CV_8U, hKernel);

		cv::imshow("Horizontal Edges", horizontal);
		cv::imwrite("Horizontal.jpg", horizontal);

		// finding the magnitude 
		//cv::Mat mag;
		//cv::magnitude(vertical, horizontal, mag);

		/*cv::imshow("Magnitude", mag);
		cv::imwrite("Magnitude.jpg", mag);*/

		// finding edges in the image using Sobel
		cv::Mat sobel;
		cv::Sobel(image, sobel, CV_8U, 1, 1, 3);

		cv::imshow("Sobel", sobel);
		cv::imwrite("Sobel.jpg", sobel);

		// use the Laplacian filter instead and compare
		cv::Mat laplacian;
		cv::Laplacian(image, laplacian, CV_8U, 3);

		cv::imshow("Laplacian", laplacian);
		cv::imwrite("Laplacian.jpg", laplacian);

		// using canny edge detector
		cv::Mat canny;
		cv::Canny(image, canny, 125, 250);

		cv::imshow("Canny", canny);
		cv::imwrite("Canny.jpg", canny);

	}
	cv::waitKey(0);
	return 0;
}