#include "opencv2/opencv.hpp"

using namespace cv;
int main() {
	// query image
	Mat simg = imread("Trillium_s.jpg", IMREAD_GRAYSCALE);
	// template image
	Mat timg = imread("Trillium_t.jpg", IMREAD_GRAYSCALE);

	// rotate by 30 degrees
	cv::Mat M = cv::getRotationMatrix2D(cv::Point2f(simg.cols / 2.0, simg.rows / 2.0), 30, 1);
	cv::warpAffine(simg, simg, M, simg.size());

	// find match location
	Mat msp;
	msp.create(simg.cols - timg.cols + 1, simg.rows - timg.rows + 1, CV_32FC1);

	matchTemplate(simg, timg, msp, CV_TM_CCORR_NORMED);

	normalize(msp, msp, 0, 1, NORM_MINMAX, -1, Mat());

	// create rectangle
	double minVal, maxVal;
	Point minLoc, maxLoc, matchLoc;

	minMaxLoc(msp, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	matchLoc = maxLoc;

	rectangle(simg, matchLoc, Point(matchLoc.x + timg.cols, matchLoc.y + timg.rows), Scalar::all(0), 2, 8, 0);

	imshow("Query Image", simg);

	cv::waitKey(0);
	
	/*Mat gt = msp;

	int gtlx = gt.x, flx = matchLoc.x, gthx = gt.x + timg.cols, fbx = matchLoc.x + timg.cols;

	for (int i = min(gtlx, flx); i < max(gthx, fhx); i++) {
		for (int j = min(gtly, fly); j < max(gthy, fhy); j++) {
			bool withinGT = (j >= gtlx) && (j < gthx) && (j >= gtly) && (j < gthy);

			if (withinGT && withinF) TP++;
		}
	}*/


}