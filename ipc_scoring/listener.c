#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>

int main(void)
{
	int sock, newsock;
	int n, cli_size;
	char recvBuff[5];
	struct sockaddr_in serv_addr, cli_addr;
	int score;

	memset(recvBuff, 0, sizeof(recvBuff));
	sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(8888);
	bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	cli_size = sizeof(cli_addr);
	while(1) {
		listen(sock, 1);
	
		newsock = accept(sock, (struct sockaddr*)&cli_addr, &cli_size);
		if(newsock<0)
			printf("error\n");

		n = read(newsock, recvBuff, sizeof(recvBuff));
		if(n<0)
			printf("error\n");

		printf("%s\n", recvBuff);
	}
	return 0;
}
