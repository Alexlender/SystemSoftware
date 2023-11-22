#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>


using namespace std;

int main(){
	int sd,num ; 

	const char ADDR[] = "77.246.159.31";

	struct in_addr addr;

	inet_pton(AF_INET, ADDR, &addr);

	char buf[128];
	struct sockaddr_in saddr ; //адрес сервера
	bzero(&saddr, sizeof(saddr)) ; //очистить адрес
	saddr.sin_family=AF_INET ;
	saddr.sin_port = htons(9199);
	saddr.sin_addr = addr;
	       
	sd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sd<0) { 
		fprintf(stdout, "\nНевозможно создать сокет\n");
		return 0; 
	} 
	if (connect(sd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0){ 
		fprintf(stdout,"\nНевозможно установить соединение с сервером\n");
		return 0;
	}
	
	int n = recv(sd, buf, sizeof(buf), 0);

	while(n != 0){

		cout << buf << endl;

		cin.getline(buf, sizeof(buf));

		send(sd, buf, sizeof(buf), 0);
		
		n = recv(sd, buf, sizeof(buf), 0);

	}

	close(sd);
	return 1;
}
