#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
	int sock, sock1;
	int n=0;
	char recvBuff[5];
	struct sockaddr_in serv_addr;
	struct sockaddr_in serv_addr1;
	int buff;

	memset(recvBuff, 0, sizeof(recvBuff));
	sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8888);
	inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
	connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	sock1 = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr1, '0', sizeof(serv_addr1));
	serv_addr1.sin_family = AF_INET;
	serv_addr1.sin_port = htons(8889);
	inet_pton(AF_INET, argv[2], &serv_addr1.sin_addr);
	connect(sock1, (struct sockaddr*)&serv_addr1, sizeof(serv_addr1));

	while((n = read(sock, recvBuff, sizeof(recvBuff)-1))>0) {
		printf("n=%d\n", n);
		recvBuff[strlen(recvBuff)] = 0;
		printf("%s\n", recvBuff);
		buff = atoi(recvBuff);
		if((n=write(sock1, recvBuff, sizeof(recvBuff-1)))<0) {
			printf("error\n");
		}
		printf("button %s pressed and printed\n", recvBuff);
		memset(recvBuff, 0, sizeof(recvBuff));
	}
	return 0;
}
