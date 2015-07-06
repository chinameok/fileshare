/*************************************************************************
	> File Name: fileshareserver.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2015年07月02日 星期四 14时05分53秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<strings.h>
#include<sys/wait.h>
#include<string.h>

int main(int argc, char ** argv)
{
	int sockfd,new_fd;/* 监听socket: sock_fd,数据传输socket: new_fd */
	struct sockaddr_in my_addr;/* 本机地址信息 */
	struct sockaddr_in their_addr;/* 客户地址信息 */
	unsigned int sin_size, myport, lisnum;
	char cbuf[4096] = {0};
	char rbuf[4096] = {0};
	int cLen = 0;

	daemon(0,0);

	if(argv[1])
		myport = atoi(argv[1]);
	else
		myport = 8800;

	if(argv[2])
		lisnum = atoi(argv[2]);
	else
		lisnum = 2;

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}
	printf("socket %d ok \n",myport);
	
	my_addr.sin_family = PF_INET;
	my_addr.sin_port = htons(myport);
	my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&(my_addr.sin_zero), 0);
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}
	printf("bind ok \n");
	if (listen(sockfd, lisnum) == -1)
	{
		perror("listen");
		exit(1);
	}
	printf("listen ok \n");
	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1)
		{
			perror("accept");
			continue;
		}
		printf("server: got connection from %s\n",inet_ntoa(their_addr.sin_addr));
		printf("before cbuf: %s\n",cbuf);
		cLen = recv(new_fd, cbuf, sizeof(cbuf), 0);
		printf("behind cbuf: %s\n",rbuf);
		sprintf(rbuf,"shareconfig \'%s\'",cbuf);
		printf("rbuf: %s\n",rbuf);
		cLen = system(rbuf);
		printf("system: %d",cLen);
		close(new_fd);
	}
}
