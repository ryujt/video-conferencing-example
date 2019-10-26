#ifndef RYU_VIDEOUNZIP_HPP
#define RYU_VIDEOUNZIP_HPP

#include <vpx/vpx_codec.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8dx.h>
#include <ryulib/yuv_tools.hpp>

#define interfaceDec (vpx_codec_vp8_dx())

using namespace std;

class VideoUnZip {
public:
	VideoUnZip()
	{
	}

	~VideoUnZip()
	{
		close();
	}

	/** 비디오 디코더를 준비시킵니다.
	@param width 디코딩 할 영상의 넓이 (가로 크기)
	@param height 디코딩 할 영상의 높이 (세로 크기)
	@return 디코딩 준비 중 에러가 없으면 true가 리턴됩니다.
	*/
	bool open(int width, int height)
	{
		close();

		cfgDec_.w = width;
		cfgDec_.h = height;

		int flags = 0;
		if (vpx_codec_dec_init(&codec_, interfaceDec, &cfgDec_, flags)) {
			printf("error - vpx_codec_dec_init \n");
			return false;
		}

		bitmap_ = malloc(width * height * PIXEL_SIZE);

		is_opened_ = true;
		return true;
	}

	/** 디코더를 초기화 시킵니다. 화상 채팅 중 상대가 접속이 끊겨서 다시 입장한 경우 등에 활용할 수 있습니다. */
	void refresh()
	{
		int flags = 0;
		vpx_codec_dec_init(&codec_, interfaceDec, &cfgDec_, flags);
	}

	/** 디코더 사용을 중단합니다. */
	void close()
	{
		if (is_opened_) {
			is_opened_ = false;
			vpx_codec_destroy(&codec_);
			if (bitmap_ != nullptr) delete bitmap_;
		}
	}

	/** 영상을 디코딩 합니다.
	@param bitmap 디코딩 할 데이터
	@param size 디코딩 할 데이터의 크기
	@return 디코딩 중 에러가 없으면 true가 리턴됩니다.
	*/
	bool decode(void* data, int size)
	{
		unsigned char* frame = (unsigned char *) data;
		int* frame_size;
		int count = 0;
		vpx_image_t* img;
		vpx_codec_iter_t iter;

		while (count < size) {
			frame_size = (int *) frame;
			frame = frame + sizeof(int);

			if (vpx_codec_decode(&codec_, (unsigned char*) frame, *frame_size, NULL, 0)) {
				return false;
			}

			frame = frame + *frame_size;
			count = count + sizeof(int) + *frame_size;

			iter = NULL;
			while((img = vpx_codec_get_frame(&codec_, &iter))) {
				I420ToARGB(
					(unsigned char *) img->planes[0], img->stride[0],
					(unsigned char *) img->planes[1], img->stride[1],
					(unsigned char *) img->planes[2], img->stride[2],
					(unsigned char *) bitmap_, 
					img->d_w * PIXEL_SIZE,
					img->d_w, img->d_h
				);
			}
		}

		return true;	
	}

	/** 디코딩이 완료된 BITMAP의 데이터 */
	void* getBitmap() { return bitmap_; }

private:
	static const int PIXEL_SIZE = 4;

	void* bitmap_ = nullptr;
	vpx_codec_dec_cfg cfgDec_;
	vpx_codec_ctx_t codec_;
	vpx_image_t	img_;

	bool is_opened_ = false;
};

#endif  // RYU_VIDEOUNZIP_HPP