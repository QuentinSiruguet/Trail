#pragma once
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <iomanip>

class Detector
{
	cv::HOGDescriptor hog;
public:
	Detector();

	std::vector<cv::Rect> detect(cv::InputArray img);

	void adjustRect(cv::Rect& r);
};

