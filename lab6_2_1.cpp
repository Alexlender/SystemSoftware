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

int msgid;

void matrix_init(ifstream &fin, int n, int* matrix){
	int buf;
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++){
			fin >> buf;
			matrix[i*n + j] = buf;	
		
		}
}


void matrix_print(int n, int* matrix){
        for(int i = 0; i < n; i++){
                for(int j = 0; j < n; j++)
                        cout << matrix[i*n + j] << " ";
		cout << endl;
	}
}

bool isPrime(int number){
    if(number < 2) return false;
    if(number == 2) return true;
    if(number % 2 == 0) return false;
    for(int i=3; (i*i)<=number; i+=2){
        if(number % i == 0 ) return false;
    }
    return true;
}

int main(){

	setlocale(LC_ALL, "Russian");
	
	ifstream fin;
	fin.open("matrix", ios::in);
	if (!fin.is_open()) {
		cout << "Файл matrix не удалось открыть" << endl;
		return 1;
	}
	
	int n;
	fin >> n;

	int* matrix = new int[n];

	matrix_init(fin, n, matrix);
	matrix_print(n, matrix);

	int key;
        cin >> key;

	msgid = msgget(key, IPC_CREAT | 0666 | IPC_EXCL);
	cout << "msgid = " << msgid << endl;	
	
	message msg;

	msg.mtype = 1;

	for(int i = 0; i < n * n; i++)
		if(isPrime(matrix[i])){
			msg.num = matrix[i];
			cout << msgsnd(msgid, (void*)&msg, sizeof(msg.num), 0) << endl;
		}
	delete[] matrix;
	return 0;

}

