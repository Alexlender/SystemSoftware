#include<iostream>
#include<fstream>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/wait.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/msg.h>
#include<sys/ipc.h>


using namespace std;

int num;

bool isSym;		       

void sig_handler1(int signum){
	num++;
	cout << "1" << endl;
	signal(SIGUSR1, sig_handler1);
}


void sig_handler2(int signum){
        num++;
	cout << "2" << endl;
        signal(SIGUSR2, sig_handler2);
	isSym = false;
}

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

	for(int j = 0; j < n - i - 1; j++){
		
		if(matrix[i * n + j] != matrix[(n - j - 1) * n + (n - i - 1)]){
			kill(getppid(), SIGUSR2); 
			return;
		}
	}
	kill(getppid(), SIGUSR1); 
}

int main(){

	setlocale(LC_ALL, "Russian");
	
	ifstream fin;
	fin.open("matrix", ios::in);
	if (!fin.is_open()) {
		cout << "Файл matrix не удалось открыть" << endl;
		return 1;
	}

	isSym = true; 
	
	struct sigaction act;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = sig_handler1;
	sigaction(SIGUSR1,&act,0); 
	
	act.sa_handler = sig_handler2;
        sigaction(SIGUSR2,&act,0);
	
	num = 0;
	int n;
	fin >> n;

	int* matrix = new int[n];

	matrix_init(fin, n, matrix);
	matrix_print(n, matrix);


	for(int i = 0; i < n - 1; i++)
		if(fork() == 0){
			check_row(n, matrix, i);
			return 1;

		}

	while(num < n - 1)
		 pause();


	cout << "Всё " << (isSym ? "хорошо" : "плохо") << endl;

		
	delete[] matrix;
	return 0;

}

