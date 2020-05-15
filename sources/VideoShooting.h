#pragma once
#include "CharacterRecognition.h"
#include "Archiving.h"
#include "TCPClient.h"



class VideoShooting
{
private:
	
	Detector detector;
	Archiving* archive;
	TCPClient* distantClient;
	CharacterRecognition findDossard;

	cv::Mat frame;
	std::vector<cv::Mat> frameBuffer;
	std::vector<cv::Mat> preEncodingFrames;

	std::string fileName;

	cv::VideoCapture* capture;
	int frame_width;
	int	frame_height;

	int fps = 30;
	int seconds = 10;

	void encodingVideo();
public:
	VideoShooting(Archiving* archive, TCPClient* distantClient);

	bool isRunning();
	bool isDetectingPerson();
	void saveFrame();
	void savingVideo();
	
	
};

