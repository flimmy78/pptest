#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	char* test[10];
	int i;
	for(i=0; i<10; i++)
	{
		test[i] = new char[1024*1024];
		//test[i] = (char*) malloc(1024*1024);
		usleep(1000*1000);
		printf("malloc count : %d\n", i);
	}

	if(argc > 1)
	{
		for(i=0; i<10; i++)
		{
			delete[] (test[i]);
			usleep(1000*1000);			
			printf("free count : %d\n", i);
		}
	}
}