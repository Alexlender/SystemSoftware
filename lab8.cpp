#include<iostream>
#include<fstream>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/wait.h>
#include<sched.h>

using namespace std;

#define STSIZE 10000

int len;
char* str;

char* rp;
int n;
char* stack;

int count = 0;

void prnt_str(int n, char* str){
	for(int i = 0; i < n; i++)
		cout << str[i];
	cout << endl;
}

int repl(void* k){
	
	int p = *(int *)k;
	cout << rp[p*2] << "->" << rp[p*2 + 1] << endl;
	for(int i = 0; i < len; i++)
		if(str[i] == rp[p*2])
			str[i] = rp[p*2 + 1];
	count++;
	return 0;

}

int main(){

	setlocale(LC_ALL, "Russian");
	
	ifstream fin;
	fin.open("data", ios::in);
	if (!fin.is_open()) {
		cout << "Файл data не удалось открыть" << endl;
		return 1;
	}

	len;
	fin >> len;

	str = new char[len];

	for (int i = 0; i < len; i++)
		fin >> str[i];
	prnt_str(len, str);

	n;

	fin >> n;
	rp = new char[n*2];
	for (int i = 0; i < n; i++){
		fin >> rp[i*2] >> rp[i*2 + 1];
	}

	stack = new char[n*STSIZE];
	int *arr = new int[n];
	for(int i = 0; i < n; i++){
		arr[i] = i;
		cout << clone(repl,(void*)(stack + STSIZE*(i+1) - 1), CLONE_VM, (void*)(arr + i)) << " i=" << i << endl; 
	}
	while(count < n);
	prnt_str(len, str);

	
	delete[] str;
	delete[] rp;
        delete[] stack;
	delete[] arr;

	return 0;

}

