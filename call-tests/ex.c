#include <stdio.h>

int main()
{
	char buffer[1024];
	float buffer2[256];
	printf("buffer  %d\nsizeof buffer  %d\nsizeof(buffer)  %d\n", buffer, sizeof buffer, sizeof(buffer));
	printf("buffer2 %d\nsizeof buffer2 %d\nsizeof(buffer2) %d\n", buffer2, sizeof buffer2, sizeof(buffer2));
	return 0;
}
