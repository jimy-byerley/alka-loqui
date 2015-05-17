#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include <stdlib.h>
#include <string.h>
#include "errno.h"

#define SOUND_SIZE  256

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
	void * userData);


typedef struct {
	/*
	  structure rassemblant toutes les données d'un packet (la taille max du total ne doit pas exceder 65507 octets)
	*/
	float sound[SOUND_SIZE];
} sound_packet;


/*
 * copie le son du buffer d'un packet à un buffer de son.
 * le buffer de son doit avoir la taille definie par SOUND_SIZE.
 */
inline void packet2sound(sound_packet * packet, float * buffer);

/*
  copie le son du buffer de son à un packet.
  le buffer de son doit avoir la taille definie par SOUND_SIZE.
*/
inline void sound2packet(float * buffer, sound_packet * packet);

#endif