#include "common.h"

int stream_callback( const void *input, void *output,
					 unsigned long frameCount,
					 const PaStreamCallbackTimeInfo* timeInfo,
					 PaStreamCallbackFlags statusFlags,
					 void * userData)
{
	(void)timeInfo;   // delete warning
	(void)statusFlags;
	float *in  = (float*)input;
	float *out = (float*)output;
	unsigned int i;
	stream_opts * params = (stream_opts*)userData;
	for (i=0; i<frameCount; i++)
	{
		out[i] = params->playbuffer[i] * params->play_volume;
		params->recordbuffer[i] = in[i] * params->record_volume;
	}
	return 0;
}

/*
 * copie le son du buffer d'un packet à un buffer de son.
 * le buffer de son doit avoir la taille definie par SOUND_SIZE.
 */
inline void packet2sound(sound_packet * packet, float * buffer)
{
	memcpy(buffer, packet->sound, SOUND_SIZE*sizeof(float));
}

/*
 * copie le son du buffer de son à un packet.
 * le buffer de son doit avoir la taille definie par SOUND_SIZE.
 */
inline void sound2packet(float * buffer, sound_packet * packet)
{
	memcpy(packet->sound, buffer, SOUND_SIZE*sizeof(float));
}

