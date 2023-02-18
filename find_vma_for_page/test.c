#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main(void)
{
#define ALLOC_SIZE (4096*66)
	pid_t pid = 0;
	unsigned int i = 0;
	char *ptr = malloc(ALLOC_SIZE);
	for(i = 0; i < ALLOC_SIZE/2; i++)
		ptr[i] = i%255;
	pid = fork();
	if(pid == 0){
		//child
		printf("chile ptr:0x%lx\n", (unsigned long)ptr);
		sleep(300);
	}else{
		printf("parent ptr:0x%lx\n", (unsigned long)ptr);
		sleep(200);
		free(ptr);
	}
}
