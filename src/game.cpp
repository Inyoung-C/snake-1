#include <cstdio>
#include <curses.h>
#include <winsock2.h>
#include <fcntl.h>

#include <errno.h>

#include "field.h"
#include "header.h"



#define X 20
#define Y 20


#define START_LEN 3
#define FRAME_CHANGE_IT 1 // добавить выбор рамки

void printer(char * arr, size_t size_x, size_t size_y, int from_x, int from_y) {
	for (size_t i = 0; i < size_x; ++i) {
		for (size_t j = 0; j < size_y; ++j) {
			switch(arr[i * size_x + j]) {
				case FIELD:
					mvaddch(i + from_x, j + from_y, ' ');
					break;
				case SNAKE:
					mvaddch(i + from_x, j + from_y, '*');
					break;
				case FOOD:
					mvaddch(i + from_x, j + from_y, '+');
					break;
				case FRAME:
					mvaddch(i + from_x, j + from_y, '0');
					break;
				case NOFRAME:
					mvaddch(i + from_x, j + from_y, '.');
					break;
			}
		}
	}
}


int single_game() {
	//	initscr();
	fprintf(log, "single_game...\n");
	Field field = Field(X, Y, FRAME_CHANGE_IT, START_LEN, 10);
	fprintf(log, "Object Field field created\n");
	clear();
	printw("Single Game\n");
	getch();
	clear();
	mvprintw(0, 0, "Press Any Key To Play\n");
	field.print_field(3, 3);
	getch();
	field.add_food();
	mvprintw(0, 0, "                      \n");
	refresh();
	
	while (!field.move()) {
		field.print_field(3, 3);
	}
	fprintf(log, "game over\n"); fflush(log);
//	endwin();
	return -2;
}


int multi_game_create() {
	
	//	initscr();
	clear();
	printw("Multi Game\n");
	getch();
	
	WSAData wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	struct sockaddr_in local;
	int s;
	int s1;
	int rc;
	
	local.sin_family = AF_INET;
	local.sin_port   = htons(7500);
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		pexit("socket");
	}
	
	/*u_long iMode = 0;
	ioctlsocket(s, FIONBIO, &iMode);*/
	
	rc = bind(s, (struct sockaddr*)&local, sizeof(local));
	if (rc < 0) {
		pexit("bind");
	}
	
	rc = listen(s, 5);
	if (rc) {
		pexit("listen");
	}
	
	clear();
	printw("Waiting for connect\n");
	refresh();
	s1 = accept(s, NULL, NULL);
	if (s1 < 0) {
		pexit("accept");
	}
	printw("Success\n");
	refresh();
	getch();
	u_long iMode1 = 1;	
	ioctlsocket(s1, FIONBIO, &iMode1);
	
	
	Field field = Field(X, Y, FRAME_CHANGE_IT, START_LEN, 10);
	fprintf(log, "Object Field field created\n");
	clear();
	printw("Multi game\n");
	getch();
	clear();
	mvprintw(0, 0, "Press Any Key To Play\n");
	field.print_field(3, 3);
	getch();
	field.add_food();
	mvprintw(0, 0, "                      \n");
	refresh();
		
	
	char buf_recv[1024] = "";
	char * buf_send;
	while (!field.move()) {
		field.print_field(3, 3);
		
		rc = recv(s1, buf_recv, sizeof(buf_recv), 0);
		if (rc < 0) {
			//pexit("recv");
		}
		else {
			print(buf_recv, field.get_x(), field.get_y(), 3, 30);
		}
		
		buf_send = field.field_copy();
		rc = send(s1, buf_send, field.get_x() * field.get_y(), 0);
		if (rc < 0) {
			//pexit("send");
		}
	
	}

	WSACleanup();
	
	#ifdef RELEASE
	system("pause");
	#endif
	
	return -2;
}



int multi_game_join() {
	char * ip = get_ip(MULTI_GAME_CONFIG("ip"));
	
	WSAData wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	struct sockaddr_in peer;
	int s;
	int rc;
	peer.sin_family = AF_INET;
	peer.sin_port   = htons(7500);
	peer.sin_addr.s_addr = inet_addr(ip);
	
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		pexit("socket");
	}
	
	
	
	
	printw("Try to connect (%s)...\n", ip); refresh();
	
	rc = connect(s, (struct sockaddr*)&peer, sizeof(peer)); 
	
	if (rc == SOCKET_ERROR) {
		pexit("connect");
	} 
	
	u_long iMode = 1;
	ioctlsocket(s, FIONBIO, &iMode);
	
	printw("Connected\n"); refresh();
	getch();
	clear();
	
	
	
	Field field = Field(X, Y, FRAME_CHANGE_IT, START_LEN, 10);
	fprintf(log, "Object Field field created\n");
	clear();
	printw("Multi game\n");
	getch();
	clear();
	mvprintw(0, 0, "Press Any Key To Play\n");
	field.print_field(3, 3);
	getch();
	field.add_food();
	mvprintw(0, 0, "                      \n");
	refresh();
	
	
	char * buf_send;
	char buf_recv[1024] = "";
	while (!field.move()) {
		field.print_field(3, 3);
		buf_send = field.field_copy();
		rc = send(s, buf_send, field.get_x() * field.get_y(), 0);
		if (rc < 0) {
			//pexit("send");
		}
		
		rc = recv(s, buf_recv, field.get_x() * field.get_y(), 0);
		if (rc < 0) {
			//pexit("recv");
		}
		else {
			print(buf_recv, field.get_x(), field.get_y(), 3, 30);
		}
	}
	fprintf(log, "game over\n"); fflush(log);
	
	WSACleanup();
	
	#ifdef RELEASE
	system("pause");
	#endif
	
	
	return -2;
}


