#pragma once

#include <opencv2/opencv.hpp>
#include <ryulib/Scheduler.hpp>
#include <ryulib/VideoZip.hpp>
#include <ryulib/VideoUnZip.hpp>
#include <functional>

using namespace std;

typedef function<void(const void*, int)> VideoDataEvent;

const int VIDEO_WIDTH  = 320;
const int VIDEO_HEIGHT = 240;
const int TASK_START = 1;
const int TASK_STOP = 2;

class VideoUnit {
public:
	VideoUnit()
	{
		capture = new cv::VideoCapture();
		capture->set(cv::CAP_PROP_CONVERT_RGB,  true);
		capture->set(cv::CAP_PROP_FRAME_WIDTH,  VIDEO_WIDTH);
		capture->set(cv::CAP_PROP_FRAME_HEIGHT, VIDEO_HEIGHT);

		videoZip = new VideoZip();
		videoZip->open(VIDEO_WIDTH, VIDEO_HEIGHT);

		videoUnzip = new VideoUnZip();
		videoUnzip->open(VIDEO_WIDTH, VIDEO_HEIGHT);

		scheduler = new Scheduler();

		scheduler->setOnTask([&](int task, const string text, const void* data, int size, int tag) {
			switch (task) {
				case TASK_START:
				{
					capture->open(0);
					scheduler->start();
					break;
				}
				case TASK_STOP:
				{
					scheduler->stop();
					break;
				}
			}
		});

		scheduler->setOnRepeat([&]() {
			cv::Mat img_src;
			cv::Mat img_dst;

			capture->read(img_src);
			resize(img_src, img_dst, cv::Size(VIDEO_WIDTH, VIDEO_HEIGHT), 0, 0, cv::INTER_LINEAR);

			if (videoZip->encode(img_dst.data, 24)) {
				if (onData != nullptr) onData(videoZip->getData(), videoZip->getSize());
			}

			scheduler->sleep(50);
		});
	}

	void start()
	{
		scheduler->add(TASK_START);
	}

	void stop()
	{
		scheduler->add(TASK_STOP);
	}

	void play(const void* data, int size)
	{
		if (videoUnzip->decode((void*) data, size)) {
			// TODO:
			printf("VideoUnit - bitmap is ready. \n");			
		}
	}

	void setOnData(VideoDataEvent event) { onData = event; }

private:
	cv::VideoCapture* capture = nullptr;
	VideoZip* videoZip = nullptr;
	VideoUnZip* videoUnzip = nullptr;
	Scheduler* scheduler = nullptr;
	VideoDataEvent onData = nullptr;
};