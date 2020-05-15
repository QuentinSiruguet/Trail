#pragma once
#include "Detector.h"

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}

class CharacterRecognition
{
private:
	int thresh = 50, N = 11;
	tesseract::TessBaseAPI* ocr;

public:
	CharacterRecognition();

	std::string findNumber(cv::Mat image);
};

