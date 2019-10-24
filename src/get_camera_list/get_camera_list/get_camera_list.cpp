#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
	Mat image;

	VideoCapture cap;
	//cap.set(CAP_PROP_FRAME_WIDTH, 320);
	//cap.set(CAP_PROP_FRAME_HEIGHT, 240);
	//cap.set(CAP_PROP_FRAME_WIDTH, 1280);
	//cap.set(CAP_PROP_FRAME_HEIGHT, 720);
	cap.open(0);
	if (cap.isOpened() == false) {
		printf("error! \n");
		return;
	}

	while (true) {
		cap.read(image);

		//size_t sizeInBytes = image.total() * image.elemSize() / (1280 * 720);
		//printf("sizeInBytes: %d \n", sizeInBytes);

		//Mat dst;
		//dst = rotate(image, 90);
		//dst = rotate(image, 180);

		imshow("Camera View", image);
		//imshow("Camera View", dst);

		if (waitKey(20) == 0x1B) break;	
	}
}
