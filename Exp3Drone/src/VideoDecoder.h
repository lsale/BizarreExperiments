/*
 * VideoDecoder.h
 *
 *  Created on: 29 Aug 2013
 *      Author: doturner
 */

#ifndef VIDEODECODER_H_
#define VIDEODECODER_H_

#include <QThread>
#include <bps/screen.h>

// include math.h otherwise it will get included
// by avformat.h and cause duplicate definition
// errors because of C vs C++ functions
#include <math.h>

extern "C"
{
#define UINT64_C uint64_t
#define INT64_C int64_t
#include <libavformat/avformat.h>
}

class VideoDecoder: public QThread
{
public:
	VideoDecoder(const QString &foreignWindowGroup, const QString &foreignWindowId);
	virtual ~VideoDecoder();
	virtual void run();
	void stop();

private:
	bool mShouldStop;
	int stride;
	screen_context_t screen_context;
	screen_window_t screen_window;
	screen_buffer_t screen_buffer;
	screen_pixmap_t screen_pixmap;
	screen_buffer_t screen_pixel_buffer;

	void display_frame(AVFrame* frame);
};

#endif /* VIDEODECODER_H_ */
