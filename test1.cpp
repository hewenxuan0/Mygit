#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <chrono>
#include <condition_variable>

#include"Client.h"

using namespace std;

#define MAX_CLI_NUM 3

Client *clients=new Client[MAX_CLI_NUM];

//THREAD_CONTROL g_threadStatus = THREAD_INVALID;
// std::mutex g_threadMutex;
// std::condition_variable g_threadcv;

void schedule(){
    cout<<"schedule begin"<<endl;
    
    int count=0;
    while(1){
        clients[count].run=true;
        clients[count].cv.notify_all();
        sleep(clients[count].compute);
        clients[count].run=false;
        count=(count+1)%MAX_CLI_NUM;
    }
}

void printThreadID(int num)
{
    while(1){
        while(!clients[num].run)
            clients[num].cv.wait(clients[num].lck);
        while(clients[num].run)
        {
            
            cout << "the thread NO is:" << num << endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    
}

int main()
{
    for(int i=0;i<MAX_CLI_NUM;i++){
        clients[i].lck=unique_lock<mutex>(clients[i].mu_t);
        clients[i].compute=i+1;
    }
    thread t=thread(schedule);
    
    // 使用容器管理线程
    //std::vector<std::thread> vecThread;
    for (int i = 0; i < MAX_CLI_NUM; ++i)
    {
        clients[i].cli_t=thread(printThreadID,i);
    }
    t.join();
    
    for (int i = 0; i < MAX_CLI_NUM; ++i)
    {
        if (clients[i].cli_t.joinable())
        {
            clients[i].cli_t.join();
        }
    }

    return 0;
}
