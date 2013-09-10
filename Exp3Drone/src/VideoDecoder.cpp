/*
 * VideoDecoder.cpp
 *
 *  Created on: 29 Aug 2013
 *      Author: doturner
 */

#include "VideoDecoder.h"

#include <QDebug>


VideoDecoder::VideoDecoder(const QString &foreignWindowGroup, const QString &foreignWindowId) :
		mShouldStop(false)
{
	qDebug() << "[VideoDecoder] Constructor - start";

	int sourceVideoWidth = 640;
	int sourceVideoHeight = 360;

	int sourceRectangle[4] =
	{ 0, 0, sourceVideoWidth, sourceVideoHeight };

	QByteArray groupByteArray = foreignWindowGroup.toAscii();
	QByteArray idByteArray = foreignWindowId.toAscii();

	screen_buffer = NULL;

	screen_create_context(&screen_context, SCREEN_APPLICATION_CONTEXT);

	screen_create_window_type(&screen_window, screen_context, SCREEN_CHILD_WINDOW);
	screen_join_window_group(screen_window, groupByteArray.constData());
	screen_set_window_property_cv(screen_window, SCREEN_PROPERTY_ID_STRING, idByteArray.length(), idByteArray.constData());

	//Tell the screen how it's going to be used
	int usage = SCREEN_USAGE_NATIVE;
	screen_set_window_property_iv(screen_window, SCREEN_PROPERTY_USAGE, &usage);

	//Set the screen to grey
	int color = 0xff999999;
	screen_set_window_property_iv(screen_window, SCREEN_PROPERTY_COLOR, &color);

	// define the resolution of the window we are expecting
	screen_set_window_property_iv(screen_window, SCREEN_PROPERTY_BUFFER_SIZE, sourceRectangle + 2);

	// create the window buffers
	screen_create_window_buffers(screen_window, 1);
	screen_get_window_property_pv(screen_window, SCREEN_PROPERTY_RENDER_BUFFERS, (void **) &screen_buffer);

	screen_create_pixmap(&screen_pixmap, screen_context);

	usage = SCREEN_USAGE_WRITE | SCREEN_USAGE_NATIVE;
	screen_set_pixmap_property_iv(screen_pixmap, SCREEN_PROPERTY_USAGE, &usage);

	int format = SCREEN_FORMAT_YUV420;
	screen_set_pixmap_property_iv(screen_pixmap, SCREEN_PROPERTY_FORMAT, &format);

	int z = -5; // use negative Z order so that the window
				// appears under the main window
	screen_set_window_property_iv(screen_window, SCREEN_PROPERTY_ZORDER, &z);

	//Display the grey window until we start decoding the video
	//screen_post_window(screen_window, screen_buffer, 1, sourceRectangle, 0);

	//Register all formats and codecs
	av_register_all();
	avcodec_register_all();
	avformat_network_init();

	qDebug() << "[VideoDecoder] Constructor - end";

}

VideoDecoder::~VideoDecoder()
{

}

void VideoDecoder::run()
{

	const char* drone_addr = "http://192.168.1.1:5555";

	AVFormatContext* pFormatCtx = NULL;
	AVInputFormat* pInputFormat = av_find_input_format("h264");

	qDebug() << "[VideoDecoder] Got input format: " << pInputFormat->long_name;
	qDebug() << "[VideoDecoder] Opening video feed from: " << drone_addr;

	//Note: This line will fail if ffmpeg hasn't been configured with --enable-memalign-hack
	int result = avformat_open_input(&pFormatCtx, drone_addr, pInputFormat, NULL);

	if (result != 0)
	{
		char errorMessage[100];
		av_strerror(result, errorMessage, 100);
		qDebug() << "Could not open the video file. Error: " << errorMessage;
	}
	else
	{
		// Retrieve stream information
		avformat_find_stream_info(pFormatCtx, NULL);
		// Dump information to standard output
		av_dump_format(pFormatCtx, 0, drone_addr, 0);

		// Get a pointer to the codec context for the video stream
		// and find the decoder for the video stream
		AVCodecContext *pCodecCtx;
		AVCodec *pCodec;
		pCodecCtx = pFormatCtx->streams[0]->codec;
		pCodec = avcodec_find_decoder(pCodecCtx->codec_id);

		// Open the codec
		avcodec_open2(pCodecCtx, pCodec, NULL);
		qDebug() << "Codec open. Width:" << pCodecCtx->width << " Height:" << pCodecCtx->height;

		//Create a window of this height
		int video_size[] =
		{ pCodecCtx->width, pCodecCtx->height };
		screen_set_window_property_iv(screen_window, SCREEN_PROPERTY_BUFFER_SIZE, video_size);
		screen_set_window_property_iv(screen_window, SCREEN_PROPERTY_SOURCE_SIZE, video_size);

		screen_set_pixmap_property_iv(screen_pixmap, SCREEN_PROPERTY_BUFFER_SIZE, video_size);
		screen_create_pixmap_buffer (screen_pixmap);
		screen_get_pixmap_property_pv(screen_pixmap, SCREEN_PROPERTY_RENDER_BUFFERS, (void**) &screen_pixel_buffer);
		screen_get_buffer_property_iv(screen_pixel_buffer, SCREEN_PROPERTY_STRIDE, &stride);

		// Decode the video stream
		// get video frames
		AVPacket packet;
		AVFrame *pFrame;
		pFrame = avcodec_alloc_frame();

		int frameDecoded;

		while (!mShouldStop)
		{
			// read frame
			if (av_read_frame(pFormatCtx, &packet) < 0)
			{
				qDebug() << "[VideoDecoder] - Could not read frame!";
				continue;
			}

			// decode the frame
			if (avcodec_decode_video2(pCodecCtx, pFrame, &frameDecoded, &packet) < 0)
			{
				qDebug() << "Could not decode frame!";
				continue;
			}

			if (frameDecoded)
			{
				//Convert the frame to a pixel buffer for displaying
				display_frame(pFrame);
			}
		}
	}
}

void VideoDecoder::display_frame(AVFrame *frame)
{
	unsigned char *ptr = NULL;
	screen_get_buffer_property_pv(screen_pixel_buffer, SCREEN_PROPERTY_POINTER, (void**) &ptr);

	int width = frame->width;
	int height = frame->height;

	uint8_t *srcy = frame->data[0];
	uint8_t *srcu = frame->data[1];
	uint8_t *srcv = frame->data[2];

	unsigned char *y = ptr;
	unsigned char *u = y + (height * stride);
	unsigned char *v = u + (height * stride) / 4;

	for (int i = 0; i < height; i++)
	{
		int doff = i * stride;
		int soff = i * frame->linesize[0];
		memcpy(&y[doff], &srcy[soff], frame->width);
	}

	for (int i = 0; i < height / 2; i++)
	{
		int doff = i * stride / 2;
		int soff = i * frame->linesize[1];
		memcpy(&u[doff], &srcu[soff], frame->width / 2);
	}

	for (int i = 0; i < height / 2; i++)
	{
		int doff = i * stride / 2;
		int soff = i * frame->linesize[2];
		memcpy(&v[doff], &srcv[soff], frame->width / 2);
	}

	screen_buffer_t screen_buffer;
	screen_get_window_property_pv(screen_window, SCREEN_PROPERTY_RENDER_BUFFERS, (void**) &screen_buffer);

	int attribs[] =
	{ SCREEN_BLIT_SOURCE_WIDTH, width, SCREEN_BLIT_SOURCE_HEIGHT, height, SCREEN_BLIT_END };
	screen_blit(screen_context, screen_buffer, screen_pixel_buffer, attribs);

	int dirty_rects[] =
	{ 0, 0, width, height };
	screen_post_window(screen_window, screen_buffer, 1, dirty_rects, 0);
}

void VideoDecoder::stop()
{
	mShouldStop = true;
}
