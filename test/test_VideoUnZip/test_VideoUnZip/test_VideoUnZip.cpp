#include <iostream>
#include <opencv2/opencv.hpp>
#include <ryulib/VideoZip.hpp>
#include <ryulib/VideoUnZip.hpp>
#include <ryulib/sdl_window.hpp>

using namespace cv;

int main(int argc, char* args[])
{
	int width  = 320;
	int height = 240;

	VideoCapture cap;
	cap.set(CAP_PROP_CONVERT_RGB, true);
	cap.set(CAP_PROP_FRAME_WIDTH, width);
	cap.set(CAP_PROP_FRAME_HEIGHT, height);
	cap.open(0);
	if (cap.isOpened() == false) {
		printf("error! \n");
		return 0;
	}

	int cam_width  = cap.get(CAP_PROP_FRAME_WIDTH);
	int cam_height = cap.get(CAP_PROP_FRAME_HEIGHT);

	printf("cam_width: %d, cam_height: %d \n", cam_width, cam_height);

	VideoZip video_zip;
	video_zip.open(width, height);

	VideoUnZip video_unzip;
	video_unzip.open(width, height);

	WindowSDL window;
	window.open("VideoZip test", width, height);

	Mat img_src;
	Mat img_dst;

	while (true) {
		cap.read(img_src);
		resize(img_src, img_dst, Size(width, height), 0, 0, INTER_LINEAR);

		if (video_zip.encode(img_dst.data, 24)) {
			printf("encode: %d \n", video_zip.getSize());

			if (video_unzip.decode(video_zip.getData(), video_zip.getSize())) {
				window.showBitmap32(video_unzip.getBitmap());
			}
		}
	}

	return 0;
}