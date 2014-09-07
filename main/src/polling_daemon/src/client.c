#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	if(argv[1][0]!='0'&&argv[1][0]!='1') {
		printf("usage: ./client [1/0]\n");
		return 1;
	}

	struct sockaddr_un addr;
	int fd, rc;
	char buffer[100];

	fd = socket(AF_UNIX, SOCK_STREAM,0);
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, "/tmp/sock.tmp");

	if(connect(fd, (struct sockaddr*)&addr, sizeof(addr))==-1) {
		printf("connect error\n");
	}

	printf("send: %s\n", argv[1]);
	if(send(fd, argv[1], 2,0)<0) {
		printf("send error\n");
	}

	if(recv(fd, buffer, sizeof(buffer), 0)<0) {
		printf("read error\n");
	}

	printf("server: %s\n", buffer);
	return 0;
}
