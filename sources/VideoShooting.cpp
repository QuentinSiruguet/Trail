#include "VideoShooting.h"


VideoShooting::VideoShooting(Archiving* archive, TCPClient* distantClient)
{
	this->archive = archive;
	this->distantClient = distantClient;

	this->capture = new cv::VideoCapture(0);
	this->frame_width = this->capture->get(cv::CAP_PROP_FRAME_WIDTH);
	this->frame_height = this->capture->get(cv::CAP_PROP_FRAME_HEIGHT);

	std::cout << "INIT CAPTURE::FILLING BUFFER"<< std::endl;
	for (int i = 0; i < (this->seconds * this->fps); i++)
		this->saveFrame();
	std::cout << "ENDING INITIALISATION" << std::endl;
}

bool VideoShooting::isRunning()
{
	if (this->capture->isOpened())
		return true;
	else
		return false;
}

bool VideoShooting::isDetectingPerson()
{
	this->saveFrame();

	int64 t = cv::getTickCount();
	std::vector<cv::Rect> found = detector.detect(frame);
	t = cv::getTickCount() - t;

	for (std::vector<cv::Rect>::iterator i = found.begin(); i != found.end(); ++i)
	{
		cv::Rect& r = *i;
		this->detector.adjustRect(r);
		rectangle(frame, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
	}

	if (found.size() >= 1)
		return true;
	else
		return false;
}

void VideoShooting::saveFrame()
{
	*this->capture >> this->frame;
	cv::Mat copiedFrame = this->frame.clone();
	this->frameBuffer.push_back(copiedFrame);

	if (this->frameBuffer.size() >= (this->fps * this->seconds))
		this->frameBuffer.erase(this->frameBuffer.begin());
}

void VideoShooting::savingVideo()
{
	for (int i = 0; i < (this->fps * this->seconds); i++)
	{
		this->preEncodingFrames.push_back(this->frameBuffer.front());
		this->saveFrame();
	}
	std::thread thread(&VideoShooting::encodingVideo, this);
	thread.join();
}

void VideoShooting::encodingVideo()
{
	std::cout << "===START ENCODING===" << std::endl;

	std::stringstream filename;
	filename << "/home/pi/save/PV" << archive->getPiCamNumber() << "_" << archive->getStringTime() << ".avi";
	this->fileName = filename.str();


	std::cout << this->fileName << std::endl;

	std::vector<cv::Mat> copiedFrames = this->preEncodingFrames;
	preEncodingFrames.clear();

	cv::VideoWriter video(fileName, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), this->fps, cv::Size(frame_width, frame_height));

	for (auto& pef : copiedFrames)
		video.write(pef);

	video.release();

	std::cout << "===END ENCODING===" << std::endl << std::endl;


	std::string outText;
	outText = this->findDossard.findNumber(copiedFrames.at(copiedFrames.size() / 2));
	std::stringstream frameToSend;
	frameToSend << "<D>" << outText << "-" << archive->getPiCamNumber() << "-" << archive->getStringTime() << "<F";


	if (outText != "null")
	{
		std::string ipAddr = "matteo.ddns.net";
		this->distantClient->sendFrame(ipAddr, 8888, frameToSend.str());
	}
}
