#include <iostream>
#include <fstream>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <random>

using namespace std;

// This function accepts nothing and returns nothing.
// it creates a csv file containing the data set with the image name and ground truth
void createData() {
	ofstream myFile;
	myFile.open("data.csv");

	int dataCount = 1555;

	if (!myFile.is_open()) {
		cout << "cannot open file";
	}
	else {
		myFile << "image name" << "," << "ground truth" << endl;
		ifstream expected;

		for (int i = 1; i <= dataCount; i++) {

			expected.open("datasettxt/poly_gt_img" + to_string(i) + ".txt");
			if (expected.is_open()) {
				myFile << "img" << i << ",";
				std::stringstream buffer;
				buffer << expected.rdbuf();
				std::string str = buffer.str() + "\n";

				while (str.find("\n")) {
					string word = str.substr(str.find("transcriptions") + 19);
					word = word.substr(0, word.find("'"));
					myFile << word << " ";
					str = str.substr(str.find("\n") + 1);
				}
				myFile << endl;

				expected.close();
			}
		}
	}
}

// this function accepts nothing and returns nothing
// it takes the ground truths and image name from the csv file and creates a vector of strings containing all the data
vector<string> getData() {
	ifstream myFile;
	myFile.open("data.csv");

	vector<string> dataset;
	if (!myFile.is_open()) {
		cout << "File could not be opened";
	}
	else {
		string line;
		getline(myFile, line);

		while (getline(myFile, line)) {
			dataset.push_back(line);
		}

		myFile.close();
	}
	
	return dataset;
}

// This function accepts a vector of strings containing the image names and ground truth in the data set
// it returns nothing
// this function splits the data set into a training and test folder and splits the data csv
// 70% of the data set goes into the training folder and 30% goes into the test folder
// data split is randomly selected.
void splitTestTraining(vector<string> &data) {
	ofstream test;
	test.open("test.csv");

	ofstream train;
	train.open("training.csv");
	
	vector<string> testData;
	vector<string> trainData;

	int dataCount = 1555;
	int trainCount = dataCount * 0.7;

	if (!train.is_open()) {
		cout << "cannot open train file";

	}
	else {

		if (!test.is_open()) {
			cout << "cannot open test file";
		}
		else {
			shuffle(data.begin(), data.end(), default_random_engine());

			for (int i = 0; i < trainCount; i++) {
				trainData.push_back(data[i]);
			}

			for (int i = trainCount; i < dataCount; i++) {
				testData.push_back(data[i]);
			}

			sort(trainData.begin(), trainData.end(), [](string a, string b) {
				int x = stoi(a.substr(3));
				int y = stoi(b.substr(3));
				return x < y; 
				});
			sort(testData.begin(), testData.end(), [](string a, string b) {
				int x = stoi(a.substr(3));
				int y = stoi(b.substr(3));
				return x < y;
				});

			int size = trainData.size();
			cv::Mat image;
			string imagename;
			cout << "splitting 70% of dataset into training folder ..." << endl;
			train << "image name,ground truth" << endl;
			for (int i = 0; i < size; i++) {
				train << trainData[i] << endl;
				imagename = trainData[i].substr(0, trainData[i].find(","));
				image = cv::imread("dataset/" + imagename + ".jpg", cv::IMREAD_COLOR);
				cv::imwrite("training/" + imagename + ".jpg", image);
			}
			cout << "done" << endl;

			cout << "splitting 30% of dataset into test folder ..." << endl;
			test << "image name,ground truth" << endl;
			size = testData.size();
			for (int i = 0; i < size; i++) {
				test << testData[i] << endl;
				imagename = testData[i].substr(0, testData[i].find(","));
				image = cv::imread("dataset/" + imagename + ".jpg", cv::IMREAD_COLOR);
				cv::imwrite("test/" + imagename + ".jpg", image);
			}
			cout << "done" << std::endl;
			test.close();
		}

		train.close();
	}
}

int main() {
	createData();
	vector<string> data = getData();

	splitTestTraining(data);

	return 0;
}

