#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STRING 1024
typedef char string[MAX_STRING];
string param,page;

const char *msg[]={
	"Error format of arguements!",
	"Page file must begin with one macro define!",
	"Error format of macro define!",
	"Cannot find input file!",
	"Cannot create output file!",
	"Type of input file is illegal!",
};

void error(int code) {
	fprintf(stderr,"error (%d): %s\n",code,msg[code]);
}

bool ignore(int next) {
	return (next==' ' || next=='\t' || next=='\n' || next=='\r');
}

int nextBegin(FILE *file) {
	int ch=fgetc(file);
	while (ignore(ch))
		ch=fgetc(file);
	return ch;
}

int release(FILE *file,int begin,char *buf,int end) {
	*buf=begin;
	int id=1,ch=fgetc(file);
	while (!ignore(ch) && ch!=end)
		buf[id++]=ch, ch=fgetc(file);
	buf[id]=0;
	return ch;
}

bool readProperty(char *name,char *value,FILE *file) {
	int ch=nextBegin(file);
	if (ch=='@')
		return false;
	int next=release(file,ch,name,'=');
	if (next!='=') {
		int begin=nextBegin(file);
		if (begin!='=') {
			error(2);
			return false;
		}
	}
	ch=nextBegin(file);
	if (ch!='\'' && ch!='"') {
		error(2);
		return false;
	}
	int comic=ch,id=0;
	while (ch=fgetc(file),!feof(file) && ch!=comic)
		value[id++]=ch;
	value[id]=0;
	if (feof(file)) {
		error(2);
		return false;
	}
	return true;
}

void convert(FILE *in,FILE *out) {
	fprintf(out,"#include \"string.h\"\n");
	int state=0, next;
	bool flag=false;
	static string propName, propValue;
	while (next=fgetc(in),!feof(in)) {
		if (ignore(next))
			continue;
		switch (state) {
			case 0:
				if (next=='<')
					state=1;
				else {
					error(1);
					return;
				}
				break;
			case 1:
				if (next=='@') {
					while (readProperty(propName,propValue,in)) {
						if (!strcmp(propName,"include"))
							fprintf(out,"#include \"%s\"\n",propValue);
						else if (!strcmp(propName,"lib"))
							fprintf(out,"#pragma comment(lib,\"%s\")\n",propValue);
						else if (!strcmp(propName,"param"))
							strcpy(param,propValue);
						else if (!strcmp(propName,"page"))
							strcpy(page,propValue);
						else {
							error(2);
							return;
						}
					}
					state=2;
				} else {
					error(1);
					return;
				}
				break;
			case 2:
				if (next=='>') {
					flag=true;
					state=0;
					break;
				} else {
					error(2);
					return;
				}
				break;
		}
		if (flag)
			break;
	}
	if (state) {
		error(2);
		return;
	}
	fprintf(out,"extern \"C\" void servlet(char* %s, char* %s) {\n",param,page);

	int pos=ftell(in);
	fseek(in,0,SEEK_END);
	int end=ftell(in);
	fseek(in,pos,0);
	char *datas=new char[end-pos+2];
	int iterater=0;
	while (next=fgetc(in),!feof(in))
		datas[iterater++]=next;
	datas[iterater]=' ', datas[iterater+1]=0;
	for (int j=0,i=0;i<=iterater;i++) {
		if (datas[i]=='"') {
			state=!state;
		} else if (i==iterater || !state && datas[i]=='<' && datas[i+1]=='%') {
			datas[i]=0;
			fprintf(out,"strcat(%s,\"",page);
			for (int k=j;k<i;k++) {
				if (datas[k]=='\n')
					fputc(' ',out);
				else if (datas[k]=='"')
					fputc('\\',out),fputc('"',out);
				else
					fputc(datas[k],out);
			}
			fprintf(out,"\");\n");
			datas[i]='<';
			for (j=i+2;j<=iterater;j++)
				if (datas[j]=='%' && datas[j+1]=='>') {
					datas[j]=0;
					int off=i+2;
					while (ignore(datas[off]))
						++off;
					fprintf(out,"%s\n",datas+off);
					datas[j]='%';
					j+=2,i=j-1;
					break;
				}
		}
	}
	fprintf(out,"}\n");
	delete []datas;
}

int main(int argc,char *argv[]) {
	FILE *fin,*fout;
	if (argc!=2) {
		error(0);
		return 0;
	}
	int length=strlen(argv[1]);
	if (length<4 || strcmp(argv[1]+length-4,".cph")) {
		error(5);
		return 0;
	}
	if ((fin=fopen(argv[1],"r"))==NULL)
		error(3);

	char *Export=new char[length+1];
	for (int i=0;i<length;i++)
		Export[i]=argv[1][i];
	Export[length]=0, Export[length-1]='p';
	if ((fout=fopen(Export,"w"))==NULL)
		error(4);
	convert(fin,fout);
	fclose(fin);
	fclose(fout);
	Export[length-4]=0;
	static char cmdline[1024];
	sprintf(cmdline,"g++ -shared \"%s.cpp\" -o \"%s.dll\"",Export,Export);
	system(cmdline);
	sprintf(cmdline,"%s.cpp",Export);
	remove(cmdline);
	return 0;
}