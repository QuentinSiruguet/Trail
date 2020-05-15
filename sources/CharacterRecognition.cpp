#include "CharacterRecognition.h"

CharacterRecognition::CharacterRecognition()
{
	ocr = new tesseract::TessBaseAPI();
	ocr->Init(NULL, "eng", tesseract::OEM_DEFAULT);
	ocr->SetPageSegMode(tesseract::PSM_AUTO);
}

std::string CharacterRecognition::findNumber(cv::Mat image)
{

	std::vector<std::vector<cv::Point>> squares;

	cv::Mat pyr, timg, gray0(image.size(), CV_8U), gray;
	// down-scale and upscale the image to filter out the noise
	pyrDown(image, pyr, cv::Size(image.cols / 2, image.rows / 2));
	pyrUp(pyr, timg, image.size());
	std::vector<std::vector<cv::Point> > contours;
	// find squares in every color plane of the image
	for (int c = 0; c < 3; c++)
	{
		int ch[] = { c, 0 };
		mixChannels(&timg, 1, &gray0, 1, ch, 1);
		// try several threshold levels
		for (int l = 0; l < N; l++)
		{
			// hack: use Canny instead of zero threshold level.
			// Canny helps to catch squares with gradient shading
			if (l == 0)
			{
				// apply Canny. Take the upper threshold from slider
				// and set the lower to 0 (which forces edges merging)
				Canny(gray0, gray, 0, thresh, 5);
				// dilate canny output to remove potential
				// holes between edge segments
				dilate(gray, gray, cv::Mat(), cv::Point(-1, -1));
			}
			else
			{
				// apply threshold if l!=0:
				//     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
				gray = gray0 >= (l + 1) * 255 / N;
			}
			// find contours and store them all as a list
			findContours(gray, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
			std::vector<cv::Point> approx;
			// test each contour
			for (size_t i = 0; i < contours.size(); i++)
			{
				// approximate contour with accuracy proportional
				// to the contour perimeter
				approxPolyDP(contours[i], approx, arcLength(contours[i], true) * 0.02, true);
				// square contours should have 4 vertices after approximation
				// relatively large area (to filter out noisy contours)
				// and be convex.
				// Note: absolute value of an area is used because
				// area may be positive or negative - in accordance with the
				// contour orientation
				if (approx.size() == 4 &&
					fabs(contourArea(approx)) > 1000 &&
					isContourConvex(approx))
				{
					double maxCosine = 0;
					for (int j = 2; j < 5; j++)
					{
						// find the maximum cosine of the angle between joint edges
						double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
						maxCosine = MAX(maxCosine, cosine);
					}
					// if cosines of all angles are small
					// (all angles are ~90 degree) then write quandrange
					// vertices to resultant sequence
					if (maxCosine < 0.3)
					{
						squares.push_back(approx);
					}
				}
			}
		}
	}
	cv::Mat im;

	for (size_t i = 0; i < squares.size(); i++)
	{
		const cv::Point* p = &squares[i][0];
		int n = (int)squares[i].size();
		polylines(image, &p, &n, 1, true, cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
		polylines(im, &p, &n, 1, true, cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
	}


	cv::bitwise_and(image, image, im, im);

	//ocr->SetImage(printOut.data, printOut.cols, printOut.rows, 3, printOut.step);

	cv::imwrite("/home/pi/done.png", im);

	return std::string(ocr->GetUTF8Text());
}
