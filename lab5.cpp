#include<iostream>
#include<fstream>
#include<string>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/wait.h>
#include<sys/sem.h>
#include<sys/shm.h>

using namespace std;

sembuf P1 = {0, 1, 0};
sembuf M1 = {0, -1, 0};

int sems;
int shmid;

void Plus1(int n){
	P1.sem_num = n;
	semop(sems, &P1, 1);
}


void Minus1(int n){
        M1.sem_num = n;
        semop(sems, &M1, 1);
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


void row_sort(int n, int* matrix, int i){
	int max_index = 0;
	for(int j = 1; j < n; j++)
		if(matrix[i*n + j] > matrix[i*n + max_index])
			max_index = j;
	swap(matrix[i*n + max_index], matrix[i*n + i]);
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
	shmid = shmget(IPC_PRIVATE, sizeof(int) * n * n, IPC_CREAT);

	if (shmid < 0 ) {
	       cout <<	"Ошибка, всё плохо с памятью";
	       return 1;
	}

	int* matrix = (int*)shmat(shmid, NULL, 0);

	matrix_init(fin, n, matrix);
	matrix_print(n, matrix);

	int sems = semget(IPC_PRIVATE, n, IPC_CREAT);
	for(int i = 0; i < n; i++)
		Plus1(i);
	

	for(int i = 0; i < n; i++){
		if(fork() == 0){
			Minus1(i);
			row_sort(n, matrix, i);
			Plus1(i);
			exit(0);
		}
		
	}
	for(int i = 0; i < n; i++)
		wait(NULL);	

	cout << "------------------------" << endl;
	matrix_print(n, matrix);
	shmdt(matrix);
	return 0;

}

