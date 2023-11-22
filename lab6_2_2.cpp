#include<iostream>
#include<fstream>
#include<string>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/wait.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/msg.h>
#include<sys/ipc.h>

#include"my_msg.h"

using namespace std;


int main(){

	setlocale(LC_ALL, "Russian");
	
	
	int key;
	cin >> key;


	int msgid = msgget(key, IPC_EXCL);
	cout << msgid << endl;

	message msg;

	while(msgrcv(msgid, &msg, sizeof(msg), 0,  IPC_NOWAIT) >= 0) 
	{ 
		cout << msg.num << endl;

	} 
	return 0;

}

