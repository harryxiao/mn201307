#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>

#define MAX_FILE_LEN  (1024*30)
#define DOWNLOAD_FILE_PATH  "/root/www/cgi-bin"
#define DOWNLOAD_FILE_NAME   "file"

int main(){
FILE *fp;
char filebuf[MAX_FILE_LEN];
char cmd[512];
struct stat sb;
sprintf(cmd, "%s%s", DOWNLOAD_FILE_PATH, DOWNLOAD_FILE_NAME);
stat(cmd, &sb); //ȡ�������ļ��Ĵ�С
//���HTTPͷ��Ϣ��������������ļ����ļ������Լ���������
printf("Content-Disposition:attachment;filename=%s", DOWNLOAD_FILE_NAME);
printf("\r\n"); 
printf("Content-Length:%d", sb.st_size);
printf("\r\n");
//
printf("Content-Type:application/octet-stream %c%c", 13,10); 
printf("Content-Type:application/octet-stream\r\n");
printf("\r\n");
sprintf(cmd, "%s%s", DOWNLOAD_FILE_PATH, DOWNLOAD_FILE_NAME);
if(fp=fopen(cmd, "r+b")){  
//�ɹ����ļ�����ȡ�ļ�����
do{
int rs = fread(filebuf, 1, sizeof(filebuf), fp);
fwrite(filebuf, rs, 1, stdout);
}while(!feof(fp));
fclose(fp);
}

return 1;
}
