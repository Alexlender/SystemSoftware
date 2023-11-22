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


using namespace std;

int msgid;

typedef struct m{
	long mtype;        
	bool fl;
	} message;
			       


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

void check_row(int n, int* matrix, int i){
	message msg;
	msg.mtype = 1;
	msg.fl = true;

	for(int j = 0; j < n - i - 1; j++){
		
		if(matrix[i * n + j] != matrix[(n - j - 1) * n + (n - i - 1)]){
			msg.fl = false;
			break;
		}
	}
	msgsnd(msgid, (void *)&msg, 1, 0);		
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

	msgid = msgget(IPC_PRIVATE, IPC_CREAT|0666);
	cout << msgid << endl;

	for(int i = 0; i < n - 1; i++)
		if(fork() == 0){
			check_row(n, matrix, i);
			return 1;

		}

	message buf;
	bool fl = true;
	cout << "n = " << n << endl;
	for(int i = 0; i < n - 1; i++){
		msgrcv(msgid, &buf, 1, 1, 0);
		if(!buf.fl)
			fl = false;
	}


	cout << "Всё " << (fl ? "хорошо" : "плохо") << endl;

		
	delete[] matrix;
	return 0;

}

