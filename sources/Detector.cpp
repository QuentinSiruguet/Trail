#include "Detector.h"

Detector::Detector()
{
	hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
}

std::vector<cv::Rect> Detector::detect(cv::InputArray img)
{
	// Run the detector with default parameters. to get a higher hit-rate
		// (and more false alarms, respectively), decrease the hitThreshold and
		// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
	std::vector<cv::Rect> found;
	hog.detectMultiScale(img, found, 0, cv::Size(8, 8), cv::Size(), 1.05, 2, false);

	return found;
}

void Detector::adjustRect(cv::Rect& r)
{
	// The HOG detector returns slightly larger rectangles than the real objects,
	// so we slightly shrink the rectangles to get a nicer output.
	r.x += cvRound(r.width * 0.1);
	r.width = cvRound(r.width * 0.8);
	r.y += cvRound(r.height * 0.07);
	r.height = cvRound(r.height * 0.8);
}
