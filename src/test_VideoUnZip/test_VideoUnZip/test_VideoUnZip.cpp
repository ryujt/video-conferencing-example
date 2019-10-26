#include <iostream>
#include <opencv2/opencv.hpp>
#include <ryulib/VideoZip.hpp>
#include <ryulib/VideoUnZip.hpp>
#include <ryulib/sdl_window.hpp>

using namespace cv;

int main(int argc, char* args[])
{
	int width  = 640;
	int height = 480;

	VideoCapture cap;
	cap.set(CAP_PROP_CONVERT_RGB, true);
	cap.set(CAP_PROP_FRAME_WIDTH, width);
	cap.set(CAP_PROP_FRAME_HEIGHT, height);
	cap.open(0);
	if (cap.isOpened() == false) {
		printf("error! \n");
		return 0;
	}

	width  = cap.get(CAP_PROP_FRAME_WIDTH);
	height = cap.get(CAP_PROP_FRAME_HEIGHT);

	VideoZip video_zip;
	video_zip.open(width, height);

	VideoUnZip video_unzip;
	video_unzip.open(width, height);

	WindowSDL window;
	window.open("VideoZip test", width, height);

	Mat image;
	while (true) {
		cap.read(image);

		if (video_zip.encode(image.data, 24)) {
			printf("encode: %d \n", video_zip.getSize());

			if (video_unzip.decode(video_zip.getData(), video_zip.getSize())) {
				window.showBitmap32(video_unzip.getBitmap());
			}
		}

		if (waitKey(20) == 0x1B) break;	
	}

	return 0;
}