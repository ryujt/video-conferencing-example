﻿#ifndef RYU_VIDEOZIP_HPP
#define RYU_VIDEOZIP_HPP

#include <vpx/vpx_codec.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <ryulib/yuv_tools.hpp>

#define interfaceEnc (vpx_codec_vp8_cx())
#define interfaceDec (vpx_codec_vp8_dx())

using namespace std;

const int BUFFER_SIZE = 32 * 1024;

class VideoZip {
public:
	VideoZip()
	{
		data_ = malloc(BUFFER_SIZE);
	}

	~VideoZip()
	{
		if (data_ != nullptr) delete data_;
		//vpx_img_free(&img);
	}

	bool open(int width, int height)
	{
		if (!vpx_img_alloc(&img_, VPX_IMG_FMT_I420 , width, height, 1)) {
			printf("error - vpx_img_alloc \n");
			return false;
		}

		if (vpx_codec_enc_config_default(interfaceEnc, &cfgEnc_, 0)) {
			printf("error - vpx_codec_enc_config_default \n");
			return false;
		}

		cfgEnc_.g_pass = VPX_RC_ONE_PASS;
		cfgEnc_.rc_dropframe_thresh = 0;
		cfgEnc_.rc_end_usage = VPX_CBR;
		cfgEnc_.rc_min_quantizer = 8;
		cfgEnc_.rc_max_quantizer = 56;
		cfgEnc_.rc_undershoot_pct = 100;
		cfgEnc_.rc_overshoot_pct  = 0;
		cfgEnc_.g_w = width;
		cfgEnc_.g_h = height;
		//cfgEnc_.rc_target_bitrate = 

		if (vpx_codec_enc_init(&codec_, interfaceEnc, &cfgEnc_, 0)) {
			printf("error - vpx_codec_enc_init \n");
			return false;
		}

		return true;
	}

	bool encode(void* bitmap, int depth)
	{
		RGBtoYUV420((unsigned char*) bitmap, img_.planes[0], cfgEnc_.g_w, cfgEnc_.g_h, 4);

		int frame_cnt = 0;
		int flags = 0;
		unsigned long deadline = VPX_DL_REALTIME;
		if (vpx_codec_encode(&codec_, &img_, frame_cnt, 1, flags, deadline)) {
			return false;
		}

		const vpx_codec_cx_pkt_t* packet;
		vpx_codec_iter_t iter = NULL;
		unsigned char* frame = (unsigned char*) data_;
		int* frame_size;
		
		size_ = 0;
		while ((packet = (vpx_codec_get_cx_data(&codec_, &iter)))) {
			pts_ = packet->data.frame.duration;

			switch (packet->kind) {
				case VPX_CODEC_CX_FRAME_PKT:
					frame_size = (int*) frame;
					*frame_size = packet->data.frame.sz;
					frame = frame + sizeof(int);

					memcpy(frame, packet->data.frame.buf, packet->data.frame.sz);
					frame = frame + packet->data.frame.sz;

					size_ = size_ + sizeof(int) + packet->data.frame.sz;
					break;

				default: break;
			}
		}

		return true;
	}
private:
	long long pts_;
	void* data_ = nullptr;
	int size_;
	vpx_image_t	img_;
	vpx_codec_enc_cfg cfgEnc_;
	vpx_codec_ctx_t codec_;
};

#endif  // RYU_VIDEOZIP_HPP