#include<iostream>
#include<string>
#include<unistd.h>
#include<sys/wait.h>

using namespace std;

int main(){

	setlocale(LC_ALL, "Russian");
	char ans;
	string command;
	int status;
	while (true){
		cout << "1. Выполнить введенную команду" << endl;
		cout << "2. Запустить на исполнение файл со сценарием" << endl;
		cout << "3. Завершить процесс по имени" << endl;
		cout << "4. Выход" << endl;
		cin >> ans;

		switch(ans){
			case '1': cout << "Введите команду" << endl;
				 
				  cin.ignore(10000, '\n');
				  getline(cin, command);
				  if(fork() == 0)
				  	if(execl(("/bin/" + command).c_str(), NULL) == -1)
						exit(1);
	  			  wait(&status);
				  if(status != 0)
				  	cout << "Что-то пошло не так, код ошибки " << status << endl;			  
				  break;
			case '2': cout << "Запускается на исполнение файл со сценарием" << endl;
				  if(fork() == 0)
                                        if(execl("/bin/sh", "sh", "lab1.sh", NULL) == -1)
                                                exit(1);
                                  wait(&status);
                                  if(status != 0)
                                        cout << "Что-то пошло не так, код ошибки " << status << endl;
                                  break;
			case '3': cout << "Введите имя завершаемого процесса" << endl;
				  cin.ignore(1000, '\n');
				  getline(cin, command);
				  if(fork() == 0)
                                        if(execl("/bin/pkill", "pkill", command.c_str(), NULL) == -1)
                                                exit(1);
                                  wait(&status);
                                  if(status != 0)
                                        cout << "Что-то пошло не так, код ошибки " << status << endl;
				  
                                  break;
			case '4': cout << "До свидания" << endl;
                                  return 0;
			default: cout << "Нет" << endl;
		}
	}
	return 0;

}

