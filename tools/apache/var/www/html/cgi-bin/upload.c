#include<stdio.h> 
#include<string.h> 
#include<unistd.h> 
#include<fcntl.h> 
#include<sys/stat.h> 
#include"cgic.h" 
#define BufferLen 1024 
int cgiMain(void){ 
	cgiFilePtr file; 
	int targetFile; 
	mode_t mode; 
	char name[128]; 
	char fileNameOnServer[64]; 
	char contentType[1024]; 
	char buffer[BufferLen]; 
	char *tmpStr=NULL; 
	int size; 
	int got,t; 
	cgiHeaderContentType("text/html"); 
	//ȡ��htmlҳ����fileԪ�ص�ֵ��Ӧ�����ļ��ڿͻ����ϵ�·���� 
	if (cgiFormFileName("file", name, sizeof(name)) !=cgiFormSuccess) { 
		fprintf(stderr,"could not retrieve filename/n"); 
		goto FAIL; 
	} 
	cgiFormFileSize("file", &size); 
	//ȡ���ļ����ͣ����������в�δʹ�� 
	cgiFormFileContentType("file", contentType, sizeof(contentType)); 
	//Ŀǰ�ļ�������ϵͳ��ʱ�ļ����У�ͨ��Ϊ/tmp��ͨ�����������ʱ�ļ�����ʱ�ļ����������û��ļ������ֲ�ͬ�����Բ���ͨ��·��/tmp/userfilename�ķ�ʽ����ļ� 
	if (cgiFormFileOpen("file", &file) != cgiFormSuccess) { 
		fprintf(stderr,"could not open the file/n"); 
		goto FAIL; 
	}

	t=-1; 
	//��·�����������û��ļ��� 
	while(1){ 
		tmpStr=strstr(name+t+1,"//"); 
		if(NULL==tmpStr) 
			tmpStr=strstr(name+t+1,"/");//if "//" is not path separator, try "/" 
		if(NULL!=tmpStr) 
			t=(int)(tmpStr-name); 
		else 
			break; 
	} 
	strcpy(fileNameOnServer,name+t+1); 
	mode=S_IRWXU|S_IRGRP|S_IROTH; 
	//�ڵ�ǰĿ¼�½����µ��ļ�����һ������ʵ������·�������˴��ĺ�������cgi�������ڵ�Ŀ¼����ǰĿ¼�����������ļ� 
	targetFile=open("/root/www/cgi-bin/file",O_RDWR|O_CREAT|O_TRUNC|O_APPEND,mode); 
	if(targetFile<0){ 
	fprintf(cgiOut,"could not create the new file,%s/n",fileNameOnServer); 
		goto FAIL; 
	} 
	fprintf(cgiOut,"12222\n");
	//��ϵͳ��ʱ�ļ��ж����ļ����ݣ����ŵ��մ�����Ŀ���ļ��� 
	while (cgiFormFileRead(file, buffer, BufferLen, &got) ==cgiFormSuccess){ 
		if(got>0) 
			write(targetFile,buffer,got); 
	} 
	cgiFormFileClose(file); 
	close(targetFile); 
	goto END; 
FAIL: 
	fprintf(stderr,"Failed to upload"); 
	return 1; 
END: 
	printf("File /%s/ has been uploaded",fileNameOnServer); 
	return 0; 
} 
