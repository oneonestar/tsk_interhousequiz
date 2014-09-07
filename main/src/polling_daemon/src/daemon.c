#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/signal.h>
#include <sys/prctl.h>
#include "stack.h"
#include "ajax.h"

// global variable
pid_t pid;
struct Stack stack;
int done = 1;
char msg[5];


void term(int signum) {
	printf("%d: received signal %d, exiting...\n", getpid(), signum);
	done = 0;
}

void sigchld_handler(int signum) {
	while(waitpid((pid_t)(-1), 0, WNOHANG)>0) {}
}

int main(int argc, char *argv[]) {

	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = term;
	sigaction(SIGTERM, &action, NULL);

	//address
	strcpy(address, argv[1]);

	// initialize stack
	new_stack(&stack);
	FILE *fp = NULL;
	pid_t process_id = 0;
	pid_t sid = 0;

	// fork daemon
	process_id = fork();
	if(process_id<0) {
		printf("fork failed\n");
		exit(1);
	}

	// parent exit
	if(process_id > 0) {
		printf("PID: %d\n", process_id);
		exit(0);
	}

	// new session ID
	umask(0);
	sid = setsid();
	if(sid<0) {
		exit(1);
	}
	//chdir("/home/steven/");
	//close(STDIN_FILENO);
	//close(STDOUT_FILENO);
	//close(STDERR_FILENO);

	fp = fopen("log.txt", "w+");
	/*** main ***/
	struct sockaddr_un addr;
	int fd, client, rc;
	socklen_t len;
	char buffer[100];

	if((fd=socket(AF_UNIX, SOCK_STREAM, 0))==-1) {
		fprintf(fp, "cannot create socket\n");
		exit(1);
	}

	// prepare sockaddr_un
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, "/tmp/sock.tmp");
	unlink("/tmp/sock.tmp");

	//bind socket to addr
	if(bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un))==-1) {
		fprintf(fp, "cannot bind socket\n");
		exit(1);
	}

	// start listening
	if(listen(fd, 5)==-1) {
		fprintf(fp, "listen error\n");
		exit(1);
	}
	fclose(fp);

	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	sigaction(SIGCHLD, &sa, 0);

	//wait for connection
	while((client=accept(fd, (struct sockaddr*)&addr, &len))>-1 && done) {

		//proceed if read successful
		if((rc=recv(client, buffer, sizeof(buffer),0))>0) {

			//0: for a new server; 1: kill newest server
			switch(atoi(buffer)) {
				case 0:
					// fork and push pid to stack
					if(stack.pointer==9) {
						send(client, "cannot spawn more child", sizeof("cannot spawn more child")+1, 0);
						close(client);
						break;
					}
					pid = fork();
					push(&stack, pid);

					if(pid<0) {
						printf("fail to fork after accept\n");
						exit(1);
					}
					else if(pid==0) {
						close(fd);
						close(client);
						prctl(PR_SET_PDEATHSIG, SIGTERM);

						// prepare data structures
						time_t rawtime;
						struct tm * timeinfo;
						char file[5];
						int i=0;
						sprintf(file, "%d.log", getpid());
						fp = fopen(file, "w");

						// infinite loop
						while(done) {
							/*
							time(&rawtime);
							timeinfo = localtime(&rawtime);
							strcpy(buffer, asctime(timeinfo));
							buffer[strlen(buffer)] = 0;
							fprintf(fp, "%d: %s\n", i, buffer);
							i++;
							*/
							ajax();
						}
						fclose(fp);
						exit(0);
					}
					else {
						// return forked pid to client
						char buffer[6];
						int i=0;
						sprintf(buffer, "%d", pid);

						fp = fopen("server.log", "a");
						fprintf(fp, "%s\n", buffer);

						// return pid to client
						if(send(client, buffer, sizeof(buffer), 0)<0) {
							fprintf(fp, "write pid fail\n");
						}

						close(client);
						fclose(fp);
					}
					break;
				case 1:
					// check if there's running service
					if(stack.pointer>=0) {
						int tmp = pop(&stack);
						char buffer[100];
						sprintf(buffer, "%d killed", tmp);
						kill(tmp, SIGTERM);
						send(client, buffer, sizeof(buffer), 0);
					} else {
						send(client, "nothing to kill", sizeof("nothing to kill")+1 ,0);
					}
					close(client);
					break;
				default:
					break;
			}
		}
	}
	close(fd);
	return 0;
}
