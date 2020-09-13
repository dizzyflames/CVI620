#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;


Point getCrossPoint(Point line1a, Point line1b, Point line2a, Point line2b) {
	double k1 = (line1a.y - line1b.y) / (line1a.x - line1b.x);
	double k2 = (line2a.y - line2b.y) / (line2a.x - line2b.x);
	double c1 = line1a.y - k1 * line1a.x;
	double c2 = line2a.y - k2 * line2a.x;
	Point crossPoint;
	crossPoint.x = (c2 - c1) / (k1 - k2);
	crossPoint.y = (k1 * crossPoint.x) + c1;
	return crossPoint;
	
}
int main(int argc, char** argv) {
	cv::Mat image = imread("Yonge_Street.jpg", IMREAD_GRAYSCALE);

	if (image.empty()) {
		std::cout << "could not find image" << std::endl;
	}
	else {
		Mat dst, cdst;
		Canny(image, dst, 50, 200, 3);

		cv::imshow("Canny", dst);
		cv::imwrite("CannyYonge.jpg", dst);
		waitKey(1);
		

		int thresholdNumber=0;
		Point line1a, line1b, line2a, line2b;
		while (thresholdNumber != -1)
		{
			std::cout << "input threshold number, input -1 to quit " << std::endl;
			std::cin >> thresholdNumber;

			if (thresholdNumber!=-1)
			{
				Canny(image, dst, 50, 200, 3);
				cvtColor(dst, cdst, COLOR_GRAY2BGR);
				std::vector<Vec2f> lines;
				cv::HoughLines(dst, lines, 1, CV_PI / 180, thresholdNumber, 0, 0);
				for (size_t i = 0; i < lines.size(); i++)
				{
					float rho = lines[i][0], theta = lines[i][1];
					Point pt1, pt2;
					double a = cos(theta), b = sin(theta);
					double x0 = a * rho, y0 = b * rho;
					pt1.x = cvRound(x0 + 1000 * (-b));
					pt1.y = cvRound(y0 + 1000 * (a));
					pt2.x = cvRound(x0 - 1000 * (-b));
					pt2.y = cvRound(y0 - 1000 * (a));
					line(cdst, pt1, pt2, Scalar(0, 0, 255), 2, CV_AA);
					if (lines.size()==2&&i==0)
					{
						line1a = pt1;
						line1b = pt2;
					}
					if (lines.size()==2&&i==1)
					{
						line2a = pt1;
						line2b = pt2;
						Point crossPoint = getCrossPoint(line1a, line1b, line2a, line2b);
						std::cout<<"(" << crossPoint.x << " , " << crossPoint.y << ")" << std::endl;
					}
				}

				imshow("Lines", cdst);
				cv::imwrite("Lines.jpg", cdst);
				waitKey(1);
			}
		} 
		while (thresholdNumber != -2)
		{
			std::cout << "input threshold number for circle, input -2 to quit " << std::endl;
			std::cin >> thresholdNumber;

			if (thresholdNumber != -2)
			{
				
				std::vector<Vec3f> Circles;
				HoughCircles(dst, Circles, cv::HOUGH_GRADIENT, 1, 30, 250, 30, 10, 50);
			
				for (size_t i = 0; i < Circles.size(); i++)
				{
					Point center(cvRound(Circles[i][0]), cvRound(Circles[i][1]));
					int radius = cvRound(Circles[i][2]);
					// circle center
					circle(cdst, center, 3, Scalar(0, 255, 0), -1, 8, 0);
					// circle outline
					circle(cdst, center, radius, Scalar(0, 0, 255), 3, 8, 0);
				
				}

				imshow("Circle", cdst);
				cv::imwrite("Circle.jpg", cdst);
				waitKey(1);
			}
		}
		
		
	}
	cv::waitKey(0);
	return 0;
}