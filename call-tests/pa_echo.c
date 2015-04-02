#include <stdio.h>
#include <math.h>
#include "portaudio.h"

#define pi 3.141592

typedef struct {
	float volume;
} stream_opts;


int stream_callback( const void *input, void *output,
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void * userData)
{
	float *in = (float*)input;
	float *out = (float*)output;
	unsigned int i;
	stream_opts * params = (stream_opts*)userData;
	for (i=0; i<frameCount; i++)
	{
		out[i] = in[i] * params->volume;
	}
	return 0;
}


int main(int argc, char * argv[])
{
	char c;
	PaStream * stream;
	PaError err;
	stream_opts parameters;
	parameters.volume = 1;
	
	/* init pa */
	err = Pa_Initialize();
	if (err != paNoError) goto error;
	err = Pa_OpenDefaultStream( 
		&stream,
		1,         /* input channel */
		1,         /* stero input */
		paFloat32, /* 32 bit float */
		7100, /* taux d'echantillonnage : 14400 */
		256,   /* echantillons par buffer, utiliser paFramesPerBufferUnspecified pour laisser pa choisir le meilleur */
		stream_callback,
		&parameters /* pointeur vers les données passées au callback */
	);
	if (err != paNoError) goto error;
	
	/* maintenant le callback va etre utilisé */
	err = Pa_StartStream(stream);
	if (err != paNoError) goto error;
	
	/* appui sur une touche pour quitter */
	printf("appuyer sur une 'q' pour arreter. (U et I pour modifier le volume)\n");
	do
	{
		c = getchar();
		if (c == 'u') parameters.volume -= 0.1;
		else if (c == 'i') parameters.volume += 0.1;
	} while (c != 'q');
	
	/* arret de pa */
	/*
	Pa_AbortStream(stream);
	if (err != paNoError) goto error;
	*/
	
	/* end */
	err = Pa_CloseStream(stream);
	if(err != paNoError) goto error;
	Pa_Terminate();
	return 0;

	error:
		printf( "PortAudio error: %s\n", Pa_GetErrorText(err));
		return -1;
}
