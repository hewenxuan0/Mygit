#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
 
 
__global__ void MatMul(int *M,int *N,int *P,int width)
{
	int x = threadIdx.x;
	int y = threadIdx.y;
	
	
	float elem1 = 0.0,elem2 = 0.0,value = 0.0;
	for(int i = 0;i < width;i++)
	{
		elem1 = M[y * width + i];//取M矩阵的一行
		elem2 = N[i * width + x];//取N矩阵的一列
		
		value += elem1 * elem2;//求和
	}
	
	P[y * width + x] = value;
}

void useCUDA(int *a,int *b,int *c,int width)
{
    printf("width=%d",width);
    for(int i=0;i<width*width;i++)
        printf("a=%d",a[i]);
    
    //const int width = 30;
	//int a[width][width],b[width][width],c[width][width];
	int *M,*N,*P;
	
	//int width = width;
	//int NUM = 900;
	dim3 blockSize(width,width);
	
	cudaEvent_t start,stop;
	float elapsedTime = 0;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	
	//设备端内存分配
	cudaMalloc((void**)&M,width * width * sizeof(int));
	cudaMalloc((void**)&N,width * width * sizeof(int));
	cudaMalloc((void**)&P,width * width * sizeof(int));
	
	//初始化
	
	
	int Size = width * width;
	//数据拷贝，主机到设备
	cudaMemcpy(M,a,Size * sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(N,b,Size * sizeof(int),cudaMemcpyHostToDevice);
	
	cudaEventRecord(start,0);
	MatMul<<<1,blockSize>>>(M,N,P,width);//调用核函数
	cudaDeviceSynchronize();
	cudaEventRecord(stop,0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedTime,start,stop);
	
	cudaMemcpy(c,P,Size * sizeof(int),cudaMemcpyDeviceToHost);
	
	for(int i=0;i<width;i++){
		for(int j=0;j<width;j++){
			//printf("c=%d \n",c[0][0]);
		}
		printf("\n");
	}
	//printf("time=%d \n",elapsedTime);
	
	
	//释放设备内存
	cudaFree(M);
	cudaFree(N);
	cudaFree(P);

}
 
int main()
{
	const int ND = 30;
	//int a[ND][ND],b[ND][ND],c[ND][ND];
	int *a=new int[ND*ND],*b=new int[ND*ND],*c=new int[ND*ND];

	for(int i = 0;i < ND;i++)
	{
		for(int j = 0;j < ND;j++)
		{
			// a[i][j] = 2;
			// b[i][j] = 3;
			a[i*ND+j]=2;
			b[i*ND+j]=3;
		}
	}

	useCUDA(a,b,c,ND);
	// int *M,*N,*P;
	
	// int width = ND;
	// //int NUM = 900;
	// dim3 blockSize(ND,ND);
	
	// cudaEvent_t start,stop;
	// float elapsedTime = 0;
	// cudaEventCreate(&start);
	// cudaEventCreate(&stop);
	
	// //设备端内存分配
	// cudaMalloc((void**)&M,ND * ND * sizeof(int));
	// cudaMalloc((void**)&N,ND * ND * sizeof(int));
	// cudaMalloc((void**)&P,ND * ND * sizeof(int));
	
	// //初始化
	
	
	// int Size = ND * ND;
	// //数据拷贝，主机到设备
	// cudaMemcpy(M,a,Size * sizeof(int),cudaMemcpyHostToDevice);
	// cudaMemcpy(N,b,Size * sizeof(int),cudaMemcpyHostToDevice);
	
	// cudaEventRecord(start,0);
	// MatMul<<<1,blockSize>>>(M,N,P,width);//调用核函数
	// cudaDeviceSynchronize();
	// cudaEventRecord(stop,0);
	// cudaEventSynchronize(stop);
	// cudaEventElapsedTime(&elapsedTime,start,stop);
	
	// cudaMemcpy(c,P,Size * sizeof(int),cudaMemcpyDeviceToHost);
	
	// // for(int i=0;i<ND;i++){
	// // 	for(int j=0;j<ND;j++){
	// // 		//printf("c=%d \n",c[0][0]);
	// // 	}
	// // 	printf("hello\n");
	// // }
	// printf("time=%f \n",elapsedTime);
	
	
	// //释放设备内存
	// cudaFree(M);
	// cudaFree(N);
	// cudaFree(P);
	
	// return 0;
}