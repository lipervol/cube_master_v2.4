#include<stdio.h>
#include<windows.h>
#include<string.h> 
int main(){
    HANDLE hcom;
    hcom = CreateFile("COM5",GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING 
                        ,FILE_ATTRIBUTE_NORMAL,NULL);
    if (hcom == INVALID_HANDLE_VALUE)
    {
       
		fprintf(stderr, "\n�򿪴���ʧ�ܣ�\n");
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
	printf("��ʽΪ:");
	printf("%s\n",data);
	fclose(fp);
	printf("�����������");
	char a;
	a=getchar();
    DWORD dwWrittenLen = 0;
	int k=0;
    if(!WriteFile(hcom,data,250,&dwWrittenLen,NULL))
    {
       	fprintf(stderr, "\n��������ʧ�ܣ�\n");
    }      
	printf("\n�����ڷ������ݳɹ���");
    return 0;
}

