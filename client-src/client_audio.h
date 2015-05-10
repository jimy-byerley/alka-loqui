#include <stdio.h>
#include "portudio.h"

namespace audio
{
	// parametres
	PaStreamParameters input_param
	PaStreamParameters output_param;
	// peripherique entrée/sortie
	PaStream * stream;
	// sortie d'erreur
	PaError err;

	// initialize portaudio, doi etre appelé avant la premiere utilisation, retourne -1 si une erreur survient
	int init();
	// fonction internet, affichage d'erreur de portaudio
	inline int report_error();

}