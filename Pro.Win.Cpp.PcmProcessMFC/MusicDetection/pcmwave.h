#include <stdio.h>
#include <string.h>

#include "fft.h"

namespace pcm {
	struct RIFF {
		char fccID[4]; // "RIFF"
		unsigned long dwSize; // 文件长度
		char fccType[4]; // "WAVE"
	} riff;

	struct FMT {
		char fccID[4]; // "fmt "
		unsigned long dwSize; // 0x10 for PCM
		unsigned short wFormatTag; // 1,编码方式
		unsigned short wChannels; // 2
		unsigned long dwSamplesPerSec; // 0xAC44
		unsigned long dwAvgBytesPerSec; // nByte/s
		unsigned short wBlockAlign; // 采样位数
		unsigned short uiBitsPerSample; // 每个采样bit数
	} fmt;

	struct DATA {
		char fccID[4]; // "data"
		unsigned long dwSize; // 语音数据长度
	} data;

	char header[60];
	FILE *fp;

	bool loadPcmWave(const char *path) {
		if (fopen_s(&fp,path,"rb"))
			return false;
		for (int i=0;i<60;i++)
			header[i]=fgetc(fp);

		int pfmt,pdata;
		for (pfmt=0;pfmt<60 && strncmp(header+pfmt,"fmt ",4);pfmt++);
		for (pdata=0;pdata<60 && strncmp(header+pdata,"data",4);pdata++);

		rewind(fp);
		fread(&riff,sizeof(RIFF),1,fp);
		fseek(fp,pfmt,0);
		fread(&fmt,sizeof(FMT),1,fp);
		fseek(fp,pdata,0);
		fread(&data,sizeof(DATA),1,fp);

		if (pfmt<0 || pdata<0)
			return false;
		if (fmt.wChannels!=1)
			return false;
		return true;
	}

	unsigned long getSampleCount() {
		return (data.dwSize<<3)/fmt.uiBitsPerSample/fmt.wChannels;
	}

	float *getPcmDatas() {
		int nSamples=getSampleCount();
		float *tDat=new float[nSamples];
		short mix;
		for (unsigned i=0;i<nSamples;i++) {
			if (fmt.uiBitsPerSample==8)
				tDat[i]=char(fgetc(fp)^(1<<7))/128.0f;
			else {
				fread(&mix,2,1,fp);
				tDat[i]=mix/32768.0f;
			}
		}
		return tDat;
	}

	void preEmphasis(float *tDat,unsigned length) {
		for (int i=length-1;i>0;i--)
			tDat[i]-=0.96f*tDat[i-1];
	}

	float *getPcmFrequence(float *pcmDatas,unsigned &length,void(*trigger)(float,float,bool) = NULL) {
		const unsigned WPROJ=11, WINSZ = 1<<WPROJ, STEP = WINSZ>>2;
		length=(getSampleCount()-WINSZ)/STEP+1;
		static float pcmFrame[WINSZ];

		float *freq=new float[length];

		static float ceof[WINSZ];
		for (unsigned j=0;j<WINSZ;j++)
			ceof[j]=(0.54-0.46*cos(acos(-1.0)*(j<<1)/(WINSZ-1)));

		float maxData=0.0;
		for (unsigned j=getSampleCount();j;--j)
			maxData=max(maxData,pcmDatas[j-1]);

		maxData*=0.1;
		unsigned ubound=getSampleCount()-WINSZ;
		for (unsigned i=0;i<=ubound;i+=STEP) {
			if (trigger)
				trigger(0,1,0);

			float frameMaxData=0.0;
			for (int j=0;j<WINSZ;j++) {
				pcmFrame[j]=pcmDatas[i+j]*ceof[j]; // 汉明窗处理
				if (frameMaxData<pcmFrame[j])
					frameMaxData=pcmFrame[j];
			}

			if (frameMaxData>=maxData) {
				// 傅里叶变换
				fft::fft2b(pcmFrame,WPROJ);

				float maxFrame=0.0;
				for (int j=0;j<WINSZ;j++)
					maxFrame=max(maxFrame,pcmFrame[j]);
				maxFrame*=0.02;

				float best=-1,answer=0;
				static float energy[WINSZ],fz[WINSZ];
				for (unsigned j=1; j+j+1<WINSZ; j++) {
					energy[j]=0;
					if (pcmFrame[j]<maxFrame || pcmFrame[j-1]>pcmFrame[j] || pcmFrame[j]<pcmFrame[j+1])
						continue;
					fz[j]=float(j)*fmt.dwSamplesPerSec/WINSZ;
					if (fz[j]>=2048 || fz[j]<64)
						continue;
					energy[j] = pcmFrame[j];
					if (best<0)
						best=energy[j], answer=fz[j]; // 最高频率
				}
				for (unsigned j=1; j+j+1<WINSZ; j++)
					if (energy[j]>0) {
						for (unsigned k=j+j;k<WINSZ;k+=j) {
							const unsigned MED=30;
							unsigned low=max(k-MED,j+1),high=min(k+MED,WINSZ);
							for (unsigned s=low;s<=high;s++)
								energy[s]=0;
						}
						if (trigger)
							trigger(fz[j],energy[j],0);
					}
				freq[i/STEP]=answer;
			}else
				freq[i/STEP]=0;
			if (trigger)
				trigger(0,0,1);

		}

		return freq;
	}

	/*
	// 自相关性
	double sub=0,hz;
	for (int k=0;k<WINSZ;k++) {
	ceof[k]=0;
	for (int j=0;j<WINSZ;j++)
	ceof[k]+=pcmDatas[i+j]*pcmDatas[i+j+k];
	hz=k;
	trigger(hz,ceof[k]*ceof[k],0);
	sub=max(sub,ceof[k]);
	}
	trigger(0,0,1);

	//
	double *histoR=new double[WINSZ], *histoI=new double[WINSZ];
	double *blankDatas=new double[WINSZ];


	memset(blankDatas,0,sizeof(blankDatas));


	double heaviest=0;
	for (unsigned i=0;i<data.dwSize;i++)
	heaviest=max(heaviest,abs(pcmDatas[i]));
	heaviest*=0.7;
	for (unsigned i=0;i<data.dwSize;i++) {
	if (pcmDatas[i]<-heaviest)
	pcmDatas[i]-=heaviest;
	else if (pcmDatas[i]>heaviest)
	pcmDatas[i]+=heaviest;
	}

	double cef[]={0.0063642,-0.0079932,0.0060935,0.0060935,-0.0079932,0.0063642};
	double cmf[]={1,-4.1226,7.272,-6.7917,3.3507,-0.69941};
	for (unsigned i=0;i<data.dwSize;i++) {
	double z=1,ans=0,moth=0;
	for (int u=0;u<6;u++) {
	ans+=cef[u]/z;
	moth+=cmf[u]/z;
	z*=pcmDatas[i];
	}
	pcmDatas[i]*=ans/moth;
	}

	delete []ceof;
	delete []histoR;
	delete []histoI;
	delete []blankDatas;
	}
	*/
}
