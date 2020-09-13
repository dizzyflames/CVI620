// Example 2-10. The same object can load videos from a camera or a file
// 
#include <opencv2/opencv.hpp>
#include <iostream>
#include<Windows.h>

int main(int argc, char** argv) {

	cv::Mat m = cv::Mat::zeros(480, 640, CV_8UC3);
	cv::Size s = m.size();

	cv::namedWindow("my image", cv::WINDOW_AUTOSIZE);
	cv::imshow("my image", m);

	int x1, y1, x2, y2;

	bool coordNotValid = false;
	char cont = 'n';
	int px, py;

	do {
		// get two coordinates
		coordNotValid = false;
		std::cout << "Enter the coordinates of the top left corner (x y): ";
		std::cin >> x1 >> y1;
		std::cout << "Enter the coordinates of the bottom left corner (x y): ";
		std::cin >> x2 >> y2;

		if (y1 > s.height || x2 > s.width) {
			coordNotValid = true;
		}

		// get color
		char color;
		std::cout << "Enter a letter for color (x : random, r : red, g : green,  b : blue, k : black, w : white, y : yellow, c : cyan, m : magenta): ";
		std::cin >> color;

		bool validColor = (color == 'k' || color == 'w' || color == 'r' || color == 'g' || color == 'b' || color == 'c' || color == 'y' || color == 'm' || color == 'x');
		if(!coordNotValid && !validColor){
			coordNotValid = true;
		}

		// get random BGR values
		srand(time(NULL));
		int randB = rand() % 256;
		int randG = rand() % 256;
		int randR = rand() % 256;

		// color pixels to create rectangle
		if (!coordNotValid) {
			switch (color) {
			case 'w':
				for (int i = y1; i < y2; i++) {
					for (int j = x1; j < x2; j++) {
						m.at<cv::Vec3b>(i, j)[0] = 255;
						m.at<cv::Vec3b>(i, j)[1] = 255;
						m.at<cv::Vec3b>(i, j)[2] = 255;
					}
				}
				break;
			case'r':
				for (int i = y1; i < y2; i++) {
					for (int j = x1; j < x2; j++) {
						m.at<cv::Vec3b>(i, j)[0] = 0;
						m.at<cv::Vec3b>(i, j)[1] = 0;
						m.at<cv::Vec3b>(i, j)[2] = 255;
					}
				}
				break;
			case'g':
				for (int i = y1; i < y2; i++) {
					for (int j = x1; j < x2; j++) {
						m.at<cv::Vec3b>(i, j)[0] = 0;
						m.at<cv::Vec3b>(i, j)[1] = 255;
						m.at<cv::Vec3b>(i, j)[2] = 0;
					}
				}
				break;
			case'b':
				for (int i = y1; i < y2; i++) {
					for (int j = x1; j < x2; j++) {
						m.at<cv::Vec3b>(i, j)[0] = 255;
						m.at<cv::Vec3b>(i, j)[1] = 0;
						m.at<cv::Vec3b>(i, j)[2] = 0;
					}
				}
				break;
			case'c':
				for (int i = y1; i < y2; i++) {
					for (int j = x1; j < x2; j++) {
						m.at<cv::Vec3b>(i, j)[0] = 255;
						m.at<cv::Vec3b>(i, j)[1] = 255;
						m.at<cv::Vec3b>(i, j)[2] = 0;
					}
				}
				break;
			case'y':
				for (int i = y1; i < y2; i++) {
					for (int j = x1; j < x2; j++) {
						m.at<cv::Vec3b>(i, j)[0] = 0;
						m.at<cv::Vec3b>(i, j)[1] = 255;
						m.at<cv::Vec3b>(i, j)[2] = 255;
					}
				}
				break;
			case'm':
				for (int i = y1; i < y2; i++) {
					for (int j = x1; j < x2; j++) {
						m.at<cv::Vec3b>(i, j)[0] = 255;
						m.at<cv::Vec3b>(i, j)[1] = 0;
						m.at<cv::Vec3b>(i, j)[2] = 255;
					}
				}
				break;
			case'x':
				for (int i = y1; i < y2; i++) {
					for (int j = x1; j < x2; j++) {
						m.at<cv::Vec3b>(i, j)[0] = randB;
						m.at<cv::Vec3b>(i, j)[1] = randG;
						m.at<cv::Vec3b>(i, j)[2] = randR;
					}
				}
				break;
			}
			cv::Rect r = cv::Rect(x1, x2, x2 - x1, y1 - y2);
			cv::imshow("my image", m);
			cv::Size s = m.size();

			// get width, height, and area
			int width = x2 - x1, height = y1 - y2;
			width = width < 0 ? width * -1 : width;
			height = height < 0 ? height * -1 : height;
			cv::Rect rect = cv::Rect(x1, y1, width, height);
			std::cout << "Area: " << rect.area() << " Width: " << width << " Height: " << height << std::endl;

			std::cout << "Enter the coordinate of a point: ";
			std::cin >> px >> py;

			// see if point is in rectangle
			std::cout << "This point (" << px << ", " << py << ") is ";
			//px >= x1 && px <= x2 && py >= y1 && py <= y2
			cv::Point point = cv::Point(px, py);
			if (rect.contains(point)) {
				std::cout << "inside the rectangle" << std::endl;
			}
			else {
				std::cout << "not inside the rectangle" << std::endl;
			}

			// see if user wants another rectangle
			std::cout << "do you wish to continue? (y) ";
			std::cin >> cont;
			if (cont == 'y') {
				coordNotValid = true;
			}
		}

		
	} while (coordNotValid);

	// create image
	cv::imwrite("../Lab03.jpg", m);
	cv::waitKey(0);

	return 0;

}