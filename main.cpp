#include <stdio.h>
#include <iostream>
#include <string>
#include<cstring>
#include<fstream>
#include<istream>
#include<vector>
#include<math.h>


using namespace std;

extern "C"
void useCUDA(int *a,int *b,int *c,int width);

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

int main()
{
    ifstream f("1.txt");
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
    useCUDA(a,a,res,sqrt(size));
    // do
    // {
        
    
        
    // }while(getline(f,temp));
    // int a=stringToInt("123");
    // cout<<a;
    // std::cout<<"Hello C++"<<std::endl;
    // for(int i=0;i<5;i++)
    //     useCUDA(i);
    // return 0;
}
