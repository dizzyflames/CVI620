#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

#define LENGTH 20

int calcPR(bool* pred, bool* gt, int len, double* P, double* R) {

	double TP = 0, FP = 0, TN = 0, FN = 0;
	bool p, g;

	for (int i = 0; i < len; i++) {
		p = *(pred + i);
		g = *(gt + i);

		if (p && g)
			TP++;
		else if (!p && g)
			FN++;
		else if (p && !g)
			FP++;
		else if (!p && !g)
			TN++;
	}

	*R = TP / (TP + FN);
	*P = TP / (TP + FP);

	return *R && *P;
}

double calcFb(double P, double R, double beta) {
	return (1 + beta * beta) * ((P * R) / ((beta * beta * P) + R));
}

void thresh_v(double* A, bool* B, int len, double thresh) {
	for (int i = 0; i < len; i++) {
		*(B + i) = (*(A + i) > thresh);
	}
}

void readDBL(std::string filename, double *dblData) {
	std::ifstream dbl;
	dbl.open(filename);

	if (!dbl.is_open()) {
		std::cout << "could not open file " << filename << std::endl;
	}
	else {
		std::string line;
		int i = 0;
		while (std::getline(dbl, line)) {
			*(dblData + i) = std::stod(line);
			i++;
		}

		dbl.close();
	}

}

int main() {
	std::ifstream alg_bin;
	std::ifstream gt;
	alg_bin.open("alg_bin.csv");
	gt.open("gt.csv");

	if (!alg_bin.is_open() || !gt.is_open()) {
		std::cout << "one of the files could not be opened";
	}
	else {
		std::string line;
		bool *p = new bool[LENGTH];
		bool *g = new bool[LENGTH];
		int i = 0;
		while (std::getline(alg_bin, line)) {
			*(p + i) = stoi(line);
			i++;
		}
		i = 0;
		while (std::getline(gt, line)) {
			*(g + i) = stoi(line);
			i++;
		}

		double P = 0, R = 0;

		calcPR(p, g, LENGTH, &P, &R);
		double F1 = calcFb(P, R, 1);

		std::cout << "precision: " << P << std::endl;
		std::cout << "recall: " << R << std::endl;
		std::cout << "F1 measure: " << F1 << std::endl;

		// PART 2
		double* dblData = new double[LENGTH];
		bool* B = new bool[LENGTH];

		readDBL("alg_dbl.csv", dblData);
		
		std::ofstream PR;
		PR.open("PR.csv");

		if (!PR.is_open()) {
			std::cout << "file PR.csv could not be opened." << std::endl;
		}
		else {
			double threshold = 0.1;
			PR << "threshold,precision,recall,F1 Measure" << std::endl;

			while (threshold < 0.9) {
				thresh_v(dblData, B, LENGTH, threshold);
				calcPR(B, g, LENGTH, &P, &R);
				F1 = calcFb(P, R, 1);

				PR << threshold << "," << P << "," << R << "," << F1 << std::endl;

				threshold += 0.1;
			}

			PR.close();
		}
		alg_bin.close();
		gt.close();
	}

	

	return 0;
}