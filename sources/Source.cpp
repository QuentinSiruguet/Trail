#include "VideoShooting.h"

int main()
{
	Archiving archive;
	TCPClient client;
	VideoShooting video(&archive, &client);

	while (video.isRunning())
		if (video.isDetectingPerson())
			video.savingVideo();
			
	return 0;
}
