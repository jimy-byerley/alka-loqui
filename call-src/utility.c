#include "utility.h"

void sleep_ms(int milliseconds) // cross-platform sleep function
{
	#ifdef WIN32
		Sleep(milliseconds);
	#elif _POSIX_C_SOURCE >= 199309L
		struct timespec ts;
		ts.tv_sec = 0;
		ts.tv_nsec = milliseconds * 1000000;
		nanosleep(&ts, NULL);
	#else
		usleep(milliseconds * 1000);
	#endif
}

/* uniquement pour windows, mais sera présent dans la boucle principale : init et end */
void network_init(void)
{
	#ifdef WIN32
		WSADATA wsa;
		int err = WSAStartup(MAKEWORD(2, 2), &wsa);
		if(err < 0)
		{
			puts("WSAStartup failed !");
			exit(EXIT_FAILURE);
		}
	#endif
}

void network_end(void)
{
	#ifdef WIN32
		WSACleanup();
	#endif
}


int set_nonblocking(const int descriptor)
{
	#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
	
	int nonBlocking = 1;
	if ( fcntl( descriptor, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
	{
		printf( "failed to set non-blocking socket\n" );
		return -1;
	}
	
	#elif PLATFORM == PLATFORM_WINDOWS
	
	DWORD nonBlocking = 1;
	if ( ioctlsocket( descriptor, FIONBIO, &nonBlocking ) != 0 )
	{
		printf( "failed to set non-blocking socket\n" );
		return -1;
	}
	
	#endif
	return 0;
}

int set_blocking(const int descriptor)
{
	#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
	
	int nonBlocking = 0;
	if ( fcntl( descriptor, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
	{
		printf( "failed to set non-blocking socket\n" );
		return -1;
	}
	
	#elif PLATFORM == PLATFORM_WINDOWS
	
	DWORD nonBlocking = 0;
	if ( ioctlsocket( descriptor, FIONBIO, &nonBlocking ) != 0 )
	{
		printf( "failed to set non-blocking socket\n" );
		return -1;
	}
	
	#endif
	return 0;
}
