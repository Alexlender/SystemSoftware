#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<strings.h>
#include <unistd.h>
#include<arpa/inet.h>
#include<sys/shm.h>
#include<cstring>
#include"field.cpp"
using namespace std;

class Player{
	public:
	int uid;
	char ip[16];
	int sc;
	
	bool isStep;

	int room;
	int lir; //Last Invited Room

	Player(){
		int lir = -1;
		
	}
};

class Room{

public:
	Player* roomPl[2];

	int whoWhite;
	int whoStep;

	Field fd;


};

void wait_for(Room *room, Player* bro, char* buf){


	while(bro->isStep){}
	
	sprintf(buf, room->fd.lastStep);
}


int main(){
	
	int PL_COUNT = 20;
	int RM_COUNT = 5;

	int shmid = shmget(IPC_PRIVATE, sizeof(Player) * PL_COUNT, IPC_CREAT);
	Player* players = (Player*)shmat(shmid, NULL, 0);

	shmid = shmget(IPC_PRIVATE, sizeof(Room) * RM_COUNT, IPC_CREAT);
	Room* rooms = (Room*)shmat(shmid, NULL, 0);

	int sc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sc == -1)
		throw "Problem with socket";

	struct sockaddr_in saddr;
	bzero(&saddr, sizeof(saddr)); // ?зачем?
	saddr.sin_family=AF_INET;
	saddr.sin_port = htons(9199);
	saddr.sin_addr.s_addr=0;
	if(bind(sc, (struct sockaddr *)&saddr, sizeof(saddr)) < 0){
		cout << "Problem with binding .." << endl;
		return 0;
	}

	if(listen(sc, PL_COUNT) < 0)
		throw "Probleb with listening";

	cout << "Listening.." << endl;
	
	struct sockaddr caddr;

	while(true){
		
		bzero(&caddr, sizeof(caddr));
		int sd_cl;
		socklen_t addrlen;
		sd_cl = accept(sc, &caddr, &addrlen);
		struct sockaddr_in sin = *(sockaddr_in *)(&caddr);
	       	
		if(sd_cl > 0){
			cout << "New connection" << endl;
			Player *pl;
			for(int i = 0; i < PL_COUNT; i++)
				if(players[i].uid == 0){
					pl = &players[i];
					pl->uid = i + 1;
					break;
				}
					

			if(fork() == 0){
				close(sc);
				pl->sc = sd_cl;
				char cl_ip[16];
				inet_ntop(AF_INET, &sin.sin_addr, pl->ip, 16);
				cout << pl->uid << " - " << pl->ip << endl;

				char buf[128];

				sprintf(buf,"Wellcome! Your uid - %d", pl->uid);
				send(sd_cl, buf, sizeof(buf), 0);
				int n = 1;	
				while(n){
					
					n = recv(sd_cl, buf, sizeof(buf), 0);
					int id;
					switch(buf[0]){
						case 'l':
							sprintf(buf,"Players list:\n");
							for(int i = 0; i < PL_COUNT; i++)
								if(players[i].uid != 0)
									sprintf(buf+strlen(buf), "%d - (%s - %d) \n", players[i].uid, players[i].ip, players[i].isStep);
							
							break;
						case 'r':
							sprintf(buf, "Rooms list:\n");
							for(int i = 0; i < RM_COUNT; i++){
								if(rooms[i].roomPl[0]){
									sprintf(buf+strlen(buf), "Room %d - Player %d ", i + 1, rooms[i].roomPl[0]->uid);
									if(rooms[i].roomPl[1])
										sprintf(buf+strlen(buf), "Player %d", rooms[i].roomPl[1]->uid);
									sprintf(buf + strlen(buf), "\n");
								}
							}
							break;
						case 'q':
							sprintf(buf,"Goodbye");
							send(sd_cl, buf, sizeof(buf), 0);
							pl->uid = 0;
							close(sd_cl);
							cout << "Close connection " << pl->ip << endl;
							return 0;
						case 'n':
							//id = atoi(buf+1);
							if(buf[1] == '+'){
							
								for(int i = 0; i < RM_COUNT; i++)
									if(rooms[i].roomPl[0] == nullptr){
										rooms[i].roomPl[0] = pl;
										pl->room = i;
										sprintf(buf, "Room %d was created", i + 1);
										pl->isStep = true;
										break;
									}
									else
										sprintf(buf, "No free room slots");
							}
							else{
								for(int i = 0; i < RM_COUNT; i++)
									if(rooms[i].roomPl[0] == pl){
										rooms[i].roomPl[0] = nullptr;
										pl->room = -1;
										sprintf(buf, "Room %d was deleted", i + 1);
										break;
									}
							}

							break;
						case 's':
							
							if(pl->room >= 0 && pl->room <= RM_COUNT && pl->isStep){
								
								try{
									
									//Player *bro = &players[1];

									string gg = rooms[pl->room].fd.move({buf[2], buf[3]}, {buf[5], buf[6]});
									sprintf(buf ,gg.c_str());
									pl->isStep = false;
									players[1].isStep = true;
									//wait_for(&rooms[pl->room - 1], bro, buf);	
									//pl->isStep = true;
								}
								catch(const char * em){
									sprintf(buf, em);
								}
								
							}		
							else
								sprintf(buf, "NO");
							break;
					
						case 'i':
							id = atoi(buf + 1);
							if(id > 0 && id <= PL_COUNT)
								if(players[id - 1].uid != 0)
									if(pl->room >= 0 && pl->room < RM_COUNT){
										players[id - 1].lir = pl->room;
										sprintf(buf, "OK");
										break;
									}
							sprintf(buf, "NO :(");
							break;
						
						case 'e':
							id = atoi(buf + 1);
							if(id > 0 && id <= RM_COUNT)
								if(rooms[id - 1].roomPl[0]){
									rooms[id - 1].roomPl[1] = pl;
									pl->room = id;
									pl->isStep = false;
									sprintf(buf, "OK");
								}
							else
								sprintf(buf, "NO");
							break;
						default:
							sprintf(buf,"l - players list\nr - rooms list\nn+ - new room\ni [uid] - invite player in room\ne [room] - enter the room\nn- - leave room\nq - quit");
					
					}

					if(pl->lir > 0 && pl->lir <= RM_COUNT)
						sprintf(buf+strlen(buf), "\nYou invited to room %d\n", pl->lir);


					send(sd_cl, buf, sizeof(buf), 0);
				}

				
				pl->uid = 0;
				close(sd_cl);
				cout << "Forced closed connection " << pl->ip << endl;
				return 0;
			}
		}

			
		 /*try{
			cin >> oldPos;
			cin >> newPos;
			fd.move({oldPos[0], oldPos[1]}, {newPos[0], newPos[1]});
			fd.print();
		}
		catch(const char* error_message){
			cout << error_message << endl;
		}*/
	}
	close(sc);

	return 0;
}

