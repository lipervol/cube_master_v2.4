#include<stdio.h>
#include<windows.h>
#include<string.h> 
int main(){
    HANDLE hcom;
    hcom = CreateFile("COM5",GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING 
                        ,FILE_ATTRIBUTE_NORMAL,NULL);
    if (hcom == INVALID_HANDLE_VALUE)
    {
       
		fprintf(stderr, "\n打开串口失败！\n");
        exit(0);
    }
    SetupComm(hcom,1024,1024);
    DCB dcb;
    GetCommState(hcom,&dcb);
    dcb.BaudRate = 9600;
    dcb.ByteSize = 8;
    dcb.Parity = 0;
    dcb.StopBits = 1;
    SetCommState(hcom,&dcb);
    FILE *fp;
 	fp=fopen("SOLUTION.txt","r+");
	char data[100];
	fscanf(fp,"%s",&data);
	printf("公式为:");
	printf("%s\n",data);
	fclose(fp);
	printf("按任意键发送");
	char a;
	a=getchar();
    DWORD dwWrittenLen = 0;
	int k=0;
    if(!WriteFile(hcom,data,250,&dwWrittenLen,NULL))
    {
       	fprintf(stderr, "\n发送数据失败！\n");
    }      
	printf("\n往串口发送数据成功！");
    return 0;
}

