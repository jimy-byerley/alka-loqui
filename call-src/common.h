#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include <stdlib.h>
#include <string.h>
#include "errno.h"

typedef struct {
	float * recordbuffer;
	float * playbuffer;
	float record_volume;
	float play_volume;
} stream_opts;


int stream_callback( const void *input, void *output,
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void * userData)
{
	float *in  = (float*)input;
	float *out = (float*)output;
	unsigned int i;
	stream_opts * params = (stream_opts*)userData;
	for (i=0; i<frameCount; i++)
	{
		out[i] = params->playbuffer[i] * params->play_volume;
		params->recordbuffer = in[i] * params->record_volume;
	}
	return 0;
}


#endif