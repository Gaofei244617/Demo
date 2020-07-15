#include <opencv2/opencv.hpp>
#include <iostream>
#include <rapidjson/document.h>
#include <sstream>
#include <fstream>

using namespace std;

std::vector<std::vector<cv::Point>> lanePts;
std::vector<std::vector<cv::Point>> regionPts;
std::vector<std::vector<cv::Point>> direcPts;

void draw(rapidjson::Document& doc, cv::Mat& img)
{
	auto& lanes = doc["lane"];
	auto& region = doc["region"];

	// lane
	for (int i = 0; i < lanes.Size(); i++)
	{
		lanePts.emplace_back();
		for (int j = 0; j < lanes[i]["coords"].Size(); j += 2)
		{
			auto x = lanes[i]["coords"][j].GetInt();
			auto y = lanes[i]["coords"][j + 1].GetInt();
			lanePts.back().emplace_back(cv::Point(x, y));
		}

		direcPts.emplace_back();
		auto x1 = lanes[i]["direction"]["start"][0].GetInt();
		auto y1 = lanes[i]["direction"]["start"][1].GetInt();
		auto x2 = lanes[i]["direction"]["end"][0].GetInt();
		auto y2 = lanes[i]["direction"]["end"][1].GetInt();
		direcPts.back().emplace_back(cv::Point(x1, y1));
		direcPts.back().emplace_back(cv::Point(x2, y2));
	}

	// region
	for (int i = 0; i < region.Size(); i++)
	{
		regionPts.emplace_back();
		for (int j = 0; j < region[i]["coords"].Size(); j += 2)
		{
			auto x = region[i]["coords"][j].GetInt();
			auto y = region[i]["coords"][j + 1].GetInt();
			regionPts.back().emplace_back(cv::Point(x, y));
		}
	}

	cv::polylines(img, lanePts, true, cv::Scalar(0, 0, 255), 2);
	cv::polylines(img, regionPts, true, cv::Scalar(0, 255, 0), 2);
	cv::polylines(img, direcPts, false, cv::Scalar(255,0,0), 2);

	for (const auto& line : direcPts)
	{
		cv::circle(img, line.back(), 5, cv::Scalar(0, 100, 100), -1);
	}
	
}

int main(int argc, char** argv) 
{
	fstream f("line.json");
	stringstream ss;
	ss << f.rdbuf();
	rapidjson::Document doc;
	doc.Parse(ss.str().c_str());

	cv::Mat img = cv::imread("G:\\test.jpg");
	if (img.empty()) 
	{
		std::cout << "could not load image..." << std::endl;
		return -1;
	}
	std::cout << "Width: " << img.cols << ", Height: " << img.rows << std::endl;

	draw(doc, img);
	cv::namedWindow("input image", cv::WINDOW_AUTOSIZE);
	cv::imshow("input image", img);

	cv::waitKey(0);
	return 0;
}