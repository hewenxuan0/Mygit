#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/un.h>
#include<errno.h>
#include<stddef.h>
#include<unistd.h>
#include<string>
#include <cstdlib>
#include<thread>

using namespace std;

#define BUFFER_SIZE 1024
const char *filename="uds-tmp";

struct sockaddr_un sockClient;
int client_fd;

void sendmes() {
	string tmp;
    cout<<"GPU memory resource:"<<endl;//how many GB or MB
    char mem[100]="";
    tmp = "\n";
	getline(cin, tmp);
    strcat(mem, tmp.data());
    send(client_fd, mem, strlen(mem), 0);

    cout<<"GPU computing resource:"<<endl;//how many percent, up to 100
    char compute[100]="";
    tmp = "\n";
	getline(cin, tmp);
    strcat(compute, tmp.data());
    send(client_fd, compute, strlen(compute), 0);

	while (true) {
		char sendData[1000] = "";
		tmp = "\n";
		getline(cin, tmp);
        strcat(sendData, tmp.data());
        //cout<<sendData<<endl;
		int sendRes = send(client_fd, sendData, strlen(sendData), 0);
	}
}

int main()
{
    char buffer[BUFFER_SIZE] = {1,2,3};
    sockClient.sun_family = AF_UNIX;
    strcpy(sockClient.sun_path,filename);
    client_fd = socket(AF_UNIX,SOCK_STREAM,0);
    if(client_fd < 0){
        printf("Request socket failed\n");
        return -1;
    }
    if(connect(client_fd,(struct sockaddr *)&sockClient,sizeof(sockClient)) < 0){
        printf("connect socket failed\n");
        return -1;
    }
    std::thread t=std::thread(sendmes);
    while(true) {
		char revSerData[1000] = "";
		int res = recv(client_fd, revSerData, sizeof(revSerData), 0);	
		if (res > 0) {
			string t = revSerData;
			if (t == "!exist") {
				cout << "the client does not exist!" << endl;
				continue;
			}
			cout<< revSerData << endl;	
		}
	}

    close(client_fd);
    return 0;
}
