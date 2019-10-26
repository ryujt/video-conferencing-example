#ifndef RYU_VIDEOZIP_HPP
#define RYU_VIDEOZIP_HPP

#include <vpx/vpx_codec.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <ryulib/yuv_tools.hpp>

#define interfaceEnc (vpx_codec_vp8_cx())

using namespace std;

class VideoZip {
public:
	VideoZip()
	{
		data_ = malloc(BUFFER_SIZE);
	}

	~VideoZip()
	{
		if (data_ != nullptr) delete data_;
		close();
	}

	bool open(int width, int height)
	{
		close();

		if (!vpx_img_alloc(&img_, VPX_IMG_FMT_I420 , width, height, 1)) {
			printf("error - vpx_img_alloc \n");
			return false;
		}

		if (vpx_codec_enc_config_default(interfaceEnc, &cfgEnc_, 0)) {
			printf("error - vpx_codec_enc_config_default \n");
			vpx_img_free(&img_);
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
			vpx_img_free(&img_);
			return false;
		}

		is_opened_ = true;
		return true;
	}

	void close()
	{
		if (is_opened_) {
			is_opened_ = false;
			vpx_img_free(&img_);
			vpx_codec_destroy(&codec_);
		}
	}

	bool encode(void* bitmap, int depth)
	{
		RGBtoYUV420((unsigned char*) bitmap, img_.planes[0], cfgEnc_.g_w, cfgEnc_.g_h, PIXEL_SIZE);

		int frame_cnt = 0;
		int flags = 0;
		unsigned long deadline = VPX_DL_GOOD_QUALITY;
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

	void* getData() { return data_; }
	int getSize() { return size_; }

private:
	static const int BUFFER_SIZE = 1024 * 1024;
	static const int PIXEL_SIZE = 3;

	void* data_ = nullptr;
	vpx_codec_enc_cfg cfgEnc_;
	vpx_codec_ctx_t codec_;
	vpx_image_t	img_;

	bool is_opened_ = false;

	long long pts_;
	int size_;
};

#endif  // RYU_VIDEOZIP_HPP