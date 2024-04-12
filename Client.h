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
#include <cstdlib>
#include <thread>
#include<mutex>
#include <chrono>
#include <condition_variable>

#include"threadpool.h"

using namespace std;

enum THREAD_CONTROL
{
    THREAD_RUN,
    THREAD_PAUSE,
    THREAD_STOP,

    THREAD_INVALID
};

class Client {
public:
	//客户端定义
	struct sockaddr_un sockClient;
    int client_fd = socket(AF_UNIX,SOCK_STREAM,0);

    THREAD_CONTROL status=THREAD_INVALID;
	thread cli_t;

	bool run=false;

	mutex mu_t;
	condition_variable cv;
	unique_lock<mutex> lck;
    

    int mem;//显存资源
    int compute;//算力资源
	//string name;
	//bool ifonline = true;


};