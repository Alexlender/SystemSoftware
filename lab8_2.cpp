#include<iostream>
#include<fstream>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/wait.h>
#include<sched.h>

using namespace std;

#define STSIZE 10000

int* matrix;
int n;
char* stack;
int m_min;

int* results;

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


int check(void* k){
	int p = *(int *)k;
	results[p] = 0;	
	for(int i = 0; i < n; i++)
		results[p] += (matrix[p*n + i] == m_min);

	return 0;

}

int main(){

	setlocale(LC_ALL, "Russian");
	
	ifstream fin;
	fin.open("matrix", ios::in);
	if (!fin.is_open()) {
		cout << "Файл matrix не удалось открыть" << endl;
		return 1;
	}

	n;
	fin >> n;
	
	matrix = new int[n*n];
        matrix_init(fin, n, matrix);
        matrix_print(n, matrix);
	
	results = new int[n];

	m_min = matrix[0];
	for(int i = 0; i < n*n; i++)
		if(matrix[i] < m_min)
			m_min = matrix[i];

	cout << m_min << endl;
	

	stack = new char[n*STSIZE];
	int *arr = new int[n];

	for(int i = 0; i < n; i++){
		arr[i] = i;
		cout << clone(check,(void*)(stack + STSIZE*(i+1) - 1), CLONE_VM|CLONE_VFORK, (void*)(arr + i)) << " i=" << i << endl; 
	}
	
	
	int max_row = 0;
	for(int i = 0; i < n; i++){
		cout << results[i] << " ";
		if(results[max_row] < results[i])
			max_row = i;
	}
	cout << endl;
	cout << "Больше всего минимальных элементов в " << max_row + 1 << " строке" << endl;

		
	delete[] results;	
	delete[] matrix;
        delete[] stack;
	delete[] arr;

	return 0;

}

