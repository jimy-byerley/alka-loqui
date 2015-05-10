
typedef struct {
	float * soundbuffer;
	float recv_volume;
	float send_volume;
} stream_opts;


int stream_callback( const void *input, void *output,
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void * userData)
{
	float *out = (float*)output;
	unsigned int i;
	stream_opts * params = (stream_opts*)userData;
	for (i=0; i<frameCount; i++)
	{
		out[i] = params->soundbuffer[i] * params->volume;
		params in[i]
	}
	return 0;
}
