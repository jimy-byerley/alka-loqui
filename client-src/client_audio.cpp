inline int audio::report_error()
{
	Pa_Terminate();
	fprintf( stderr, "An error occured while using the portaudio stream\n" );
	fprintf( stderr, "Error number: %d\n", err );
	fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
	return -1;
}

int init()
{
	
	err = Pa_Initialize();
	if( err != paNoError ) return report_error();
	
	input_param.channelCount = 2; // stereo
	
}