//gcc -Wall -o Client Client.c
//./Client
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
int main()
{
	FILE* my_file;
	int size = 513;
	char send_data[size];
	my_file = fopen("file.txt", "r");
	if (my_file == NULL) {printf("Error: can't open the file!\n"); return -1;}
	int n = fread(send_data, sizeof(char), size, my_file);
	send_data[n] = '\0';
	int descriptor_client;
	struct sockaddr_in addr;
	descriptor_client = socket(AF_INET, SOCK_STREAM, 0);
	if(descriptor_client < 0)
	{
		printf("Error: create socket\n");
		return -2;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(20076);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	if(connect(descriptor_client, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		printf("Error: Connect\n");
		return -3;
	}
	
	send(descriptor_client, send_data, sizeof(send_data), 0);
	close(descriptor_client);
	fclose(my_file);
	return 0;
}
