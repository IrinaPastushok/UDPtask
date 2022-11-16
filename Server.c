//gcc -w -pthread -o Server Server.c
//./Server
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
char buffer[513];
char receive_data[513];
int is_ready1 = 0;
int is_ready2 = 0;
int res_size = 0;
int get_size(char* arr)
{
	int size =0;
	for (int i = 0; arr[i] != '\0'; i++)
	{
		size++;
	}
	return size;
}
void *run_task2(void *ptr)
{
	char* pointer = &buffer;
	while(1)
	{
		res_size = get_size(pointer) - 1;
		if (is_ready1 == 1)
		{
			printf("%d\n", res_size);
			puts(buffer);
			buffer[0] = 0;
			is_ready2 = 1;
			break;
		}
		usleep(100);//100000
	}
	return 0;	
}
void *run_task1( void *ptr )
{
	int size_data = get_size(receive_data);
	for (int i = 0; i < size_data; i++)
	{
		buffer[i] = receive_data[i];
		buffer[i+1] = '\0';
		usleep(10000);
	}
	return 0;	
}
int main()
{
	buffer[0] = '\0';
	pthread_t thread1, thread2; //int iter1, iter2;
	char *message1 = "Thread";
	int iter2 =  pthread_create( &thread2, NULL, run_task2, (void*) message1);
    	int sock, listener;
	struct sockaddr_in addr;
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if(listener < 0)
	{
		printf("Error: create socket\n");
		return -1;;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(20076);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    	{
        	printf("Error: bind\n");
        	return -2;
    	}
	listen(listener, 1);
    	while(1)
    	{
        	sock = accept(listener, NULL, NULL);
        	if(sock < 0)
        	{
            		printf("Error: accept\n");
            		return -3;
        	}
		recv(sock, receive_data, 513, 0);
		int iter1 =  pthread_create( &thread1, NULL, run_task1, (void*) message1);
        	pthread_join( thread1, NULL);
        	is_ready1 = 1;
     		close(sock);
    	}
    	return 0;
}
