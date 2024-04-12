#include<iostream>

#include <cstdlib>
#include<string>
#include"threadpool.h"

using namespace std;

void *printHello1(void* a){
    //long i=*((long*)a);
    cout<<(long)a<<endl;
    //cout<<a<<endl;
}

void *printHello2(void* a){
    //long i=*((long*)a);
    
    pthread_t t;
    pthread_create(&t,NULL,printHello1,a);
    cout<<(long)a<<endl;
    //cout<<a<<endl;
}

int main(){
    long x=3,y=4;
    //threadpool_t *thp = threadpool_create(10, 100, 100);
//     int indexes[5];// 用数组来保存i的值
//    int i;
//    for( i=0; i < 5; i++ ){      
//       //cout << "main() : 创建线程, " << i << endl;
//       indexes[i] = i; //先保存i的值
//       // 传入的时候必须强制转换为void* 类型，即无类型指针        
//       threadpool_add_task(thp, printHello, (void*)&(indexes[i]));
      
//    }
    // for(int i=0;i<10;i++){
    //     threadpool_add_task(thp, printHello1, (void*)i);
    // }
    // pthread_t t;
    // pthread_create(&t,NULL,printHello2,(void*)x);
    
    // threadpool_add_task(thp, printHello1, (void*)x);
    // threadpool_add_task(thp, printHello2, (void*)y);
}