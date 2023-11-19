#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main(){
	int sd,num ; 
	char buf[60] ; //для посылки и получения данных
	sprintf(buf,"Привет!") ;
	struct sockaddr_in saddr ; //адрес сервера
	bzero(&saddr, sizeof(saddr)) ; //очистить адрес
	saddr.sin_family=AF_INET ;
	saddr.sin_port = 199;
	saddr.sin_addr.s_addr=INADDR_ANY;
	       
	sd=socket(AF_INET, SOCK_STREAM, 0);
	if (sd<0) { 
		fprintf(stdout, "\nНевозможно создать сокет");
		return 0; 
	} 
	if (connect(sd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0){ 
		fprintf(stdout,"\nНевозможно установить соединение с сервером");
		return 0;
	} 
	num = send(sd, buf, sizeof(buf), 0); //послать сообщение серверу
	if (num < 0) {
		fprintf(stdout,"\nОшибка"); 
		return 0;
	}
	num=recv(sd, buf, 60, 0); //получить ответ от сервера if
	if (num<0) { 
		fprintf(stdout,"\nОшибка") ; 
		return 0 ; 
	}
	else 
		fprintf(stdout,"\nПолучен ответ%s", buf); 
	return 1;
}
