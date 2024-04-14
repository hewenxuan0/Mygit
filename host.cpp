#include<stdio.h>
#include<iostream>
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
#include<cstring>
#include<fstream>
#include<istream>
#include<math.h>
#include <cstdlib>
#include<thread>
#include<vector>

#include"Client.h"

using namespace std;

#define MAX_CONNECT_NUM 10
#define BUFFER_SIZE 1024
const char *filename="uds-tmp";


std::thread tids[1000];
static char start[] = "You have successfully connected to the host";
static int c = 0;
static Client client[100];

extern "C"
void useCUDA(int *a,int *b,int *c,int width,int num);


int charToInt(char* c){
    int num=0;
    //for(i=0;c[i]!='/n';i++);
    //cout<<c<<endl<<strlen(c)<<endl;
    for(int j=0;j<strlen(c);j++){
        num*=10;
        num+=c[j]-48;
        //cout<<num<<endl;
    }
    return num;
}

int stringToInt(std::string s){
    int res=0;
    for(int i=0;i<s.length();i++){
        res*=10;
        res+=s[i]-48;
    }
    return res;
}

vector<int> split(const string& str, const string& delim) {  
	vector<int> res;  
	if("" == str) return res;  
	//先将要切割的字符串从string类型转换为char*类型  
	char * strs = new char[str.length() + 1] ; //不要忘了  
	strcpy(strs, str.c_str());
 
	char * d = new char[delim.length() + 1];  
	strcpy(d, delim.c_str());  
 
	char *p = strtok(strs, d);  
	while(p) {  
		string s = p; //分割得到的字符串转换为string类型  
		res.push_back(stringToInt(s)); //存入结果数组  
		p = strtok(NULL, d);  
	}  
 
	return res;  
}

void schedule(){
    cout<<"schedule begin"<<endl;
    
    int count=0;
    while(c!=0){
        client[count].run=true;
        //client[count].cv.notify_all();
        sleep(client[count].compute);
        client[count].run=false;
        count=(count+1)%c;
    }
}



void ClientThread(int num) {
    
	sockaddr_un sockrev = client[num].sockClient;
    int sock_fd = client[num].client_fd;
	send(sock_fd, start, sizeof(start), 0);
    char cli_mem[255]="";
    char cli_com[255]="";
    recv(sock_fd, cli_mem, 255, 0);
    recv(sock_fd, cli_com, 255, 0);
    cout<<charToInt(cli_mem)<<endl<<charToInt(cli_com)<<endl;
    client[num].mem=charToInt(cli_mem);
    client[num].compute=charToInt(cli_com);

	//bool flag = false;
    cout<<"recv begin"<<endl;
	while (true) {
		char revData[255] = "";
		char sendData[1000] = "";
        
		int res = recv(sock_fd, revData, 255, 0);
		if (res > 0) {
            //cout<<"succ"<<endl;
			//string title;
			string tmp = revData;
			cout << tmp << endl;
            ifstream f(tmp);
            string temp;
            getline(f,temp);
            vector<int> mat=split(temp," ");
            
            int size=mat.size();
            int *a=mat.data();
            // cout<<size<<endl;
            // for(int i=0;i<size;i++)
            //     cout<<a[i]<<endl;
            mat.clear();
            int *res=new int[size];
            useCUDA(a,a,res,sqrt(size),num);
		}
		else {
            //cout<<"fail"<<endl;
			//cout << client[num].name << " just quits" << endl;
			//client[num].ifonline = false;
			//pthread_exit(0);
		}
		//closesocket(sockrev);
	}
}

int main()
{
    int fd,len,i,ret;
    struct sockaddr_un un;
    fd = socket(AF_UNIX,SOCK_STREAM,0);
    if(fd < 0){
        printf("Request socket failed!\n");
        return -1;
    }
    un.sun_family = AF_UNIX;
    unlink(filename);
    strcpy(un.sun_path,filename);
    if(bind(fd,(struct sockaddr *)&un,sizeof(un)) <0 ){
        printf("bind failed!\n");
        return -1;
    }
    if(listen(fd,MAX_CONNECT_NUM) < 0){
        printf("listen failed!\n");
        return -1;
    }
    thread t=thread(schedule);
    t.detach();
    while(1){
        struct sockaddr_un client_addr;
        char buffer[BUFFER_SIZE];
        memset(buffer,0,BUFFER_SIZE);
        len = sizeof(client_addr);
        //new_fd = accept(fd,(struct sockaddr *)&client_addr,&len);
        client[c].client_fd = accept(fd,NULL,NULL);
        if(client[c].client_fd < 0){
            printf("accept failed\n");
            return -1;
        }
        tids[i]=std::thread(ClientThread, c);
        tids[i].detach();
        c++;
    }
    //close(fd);
}
