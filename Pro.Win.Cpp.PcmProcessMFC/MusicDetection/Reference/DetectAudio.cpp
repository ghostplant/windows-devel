#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

vector<string> musicList;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////             ƥ���㷨           ////////////////////////////////////////////
int gcd(int a,int b)
{
	if(b==0)return a;
	return gcd(b,a%b);
}
int lcm(int a,int b)
{
	return a/gcd(a,b)*b;
}
int sq(int n)
{
	return n*n;
}
float sq(float n)
{
	return n*n;
}
int dataset=0;
// �󳤶�Ϊl1�Ļ�������t1�볤��Ϊl2�Ļ�������t2��ƥ�����ֵ��c1[i]��ʾt1[i]�ĳ������ȣ�c2[i]ͬ��
float MatchDifferences(int l1,unsigned char *t1,unsigned int *c1,int l2,unsigned char *t2,unsigned int *c2) {
	int len1=0,len2=0;
	dataset++;
	/*
	FILE *fp;
	if(dataset==1)//�����ƥ�������
	{
		fp=fopen("D:\\trend.txt","a");
		fprintf(fp,"%d\n",l1);
		for(int i=0;i<l1;i++)fprintf(fp,"%d %u\n",t1[i],c1[i]);
		fclose(fp);
	}
	//������ݿ��������
	fp=fopen("D:\\trend1.txt","a");
	fprintf(fp,"DataSet #%d:\n",dataset);
	fprintf(fp,"%d\n",l2);
	for(int i=0;i<l2;i++)fprintf(fp,"%d %u\n",t2[i],c2[i]);
	fclose(fp);
	*/
	//���������ݵ�������������LCM��������
	for(int i=0;i<l1;i++)len1+=c1[i];
	for(int i=0;i<l2;i++)len2+=c2[i];
	int LCM=lcm(len1,len2);
	int ext1=LCM/len1,ext2=LCM/len2;
	for(int i=0;i<l1;i++)c1[i]*=ext1;
	for(int i=0;i<l2;i++)c2[i]*=ext2;
	int len=0,cnt1=0,cnt2=0;
	float sum=0,avr=0;
	/*
	for(int i=0,j=0;i<l1;){
		if(t1[i]!=255 && t2[j]!=255){
			avr+=(int)t1[i]-t2[j];
			len++;
		}
		cnt1++;
		if(cnt1==c1[i]){
			cnt1=0;
			i++;
		}
		cnt2++;
		if(cnt2==c2[j]){
			cnt2=0;
			j++;
		}
	}
	avr/=len;
	avr=floor(avr+0.5);
	
	//����λ��
	vector<int> dis;
	for(int i=0,j=0;i<l1;){
		if(t1[i]!=255 && t2[j]!=255){
			dis.push_back((int)t1[i]-t2[j]);
			len++;
		}
		cnt1++;
		if(cnt1==c1[i]){
			cnt1=0;
			i++;
		}
		cnt2++;
		if(cnt2==c2[j]){
			cnt2=0;
			j++;
		}
	}
	//�����������Ϊ���������м�������������м���������ƽ��ֵ
	sort(dis.begin(), dis.end());
	if(dis.size()&1)avr=dis[dis.size()/2];
	else avr=(dis[dis.size()/2]+dis[dis.size()/2+1])/2;
	*/
	//*/
	//�����������������Ψһ��ȡ��λ��
	map<int,int> mymap;
	for(int i=0,j=0;i<l1;){
		if(t1[i]!=255 && t2[j]!=255){
			mymap[(int)t1[i]-t2[j]]++;
			len++;
		}
		cnt1++;
		if(cnt1==c1[i]){
			cnt1=0;
			i++;
		}
		cnt2++;
		if(cnt2==c2[j]){
			cnt2=0;
			j++;
		}
	}
	int cntmax=-1,nummax=0;
	for(map<int,int>::iterator it=mymap.begin();it!=mymap.end();it++){
		if(it->second>cntmax){
			cntmax=it->second;
			avr=it->first;
			nummax=1;
		}
		else if(it->second==cntmax){
			nummax++;
		}
	}
	if(nummax>1){
		vector<int> dis;
		for(int i=0,j=0;i<l1;){
			if(t1[i]!=255 && t2[j]!=255){
				dis.push_back((int)t1[i]-t2[j]);
				len++;
			}
			cnt1++;
			if(cnt1==c1[i]){
				cnt1=0;
				i++;
			}
			cnt2++;
			if(cnt2==c2[j]){
				cnt2=0;
				j++;
			}
		}
		//�����������Ϊ���������м�������������м���������ƽ��ֵ
		sort(dis.begin(), dis.end());
		if(dis.size()&1)avr=dis[dis.size()/2];
		else avr=(dis[dis.size()/2]+dis[dis.size()/2+1])/2;
		dis.clear();
	}
	mymap.clear();
	//*/
	
	
	//�������������������Ľ�Сֵ������һ������һ��
	/*
	vector<int> dis1,dis2;
	for(int i=0,j=0;i<l1;){
		if(t1[i]!=255 && t2[j]!=255){
			dis1.push_back((int)t1[i]);
			dis2.push_back((int)t2[i]);
			len++;
		}
		cnt1++;
		if(cnt1==c1[i]){
			cnt1=0;
			i++;
		}
		cnt2++;
		if(cnt2==c2[j]){
			cnt2=0;
			j++;
		}
	}
	sort(dis1.begin(),dis1.end());
	sort(dis2.begin(),dis2.end());
	if(abs(dis1[0]-dis2[0]) < abs(dis1[dis1.size()-1]-dis2[dis2.size()-1]))avr=dis1[0]-dis2[0];
	else avr=dis1[dis1.size()-1]-dis2[dis2.size()-1];
	len=cnt1=cnt2=0;
	vector<int> vt2;
	for(int i=0;i<l2;i++){
		vt2.push_back(t2[i]+avr);
	}
	for(int i=0,j=0;i<l1;){
		if(t1[i]!=255 && vt2[j]!=255){
			sum+=abs((int)t1[i]-vt2[j])%12;
			len++;
		}
		cnt1++;
		if(cnt1==c1[i]){
			cnt1=0;
			i++;
		}
		cnt2++;
		if(cnt2==c2[j]){
			cnt2=0;
			j++;
		}
	}
	*/

	//*
	//�������avr�Ĳ�ľ���ֵ�ĺͣ��ٳ��Գ��ȣ������յ�sumֵ
	//float phi_distribute[]={0.9987, 0.9970, 0.9938, 0.9878, 0.9772, 0.9599, 0.9332, 0.8944, 0.8413, 0.7734, 0.6915, 0.5987};//��0.2Ϊ���ȡ��̬�ֲ�
	len=cnt1=cnt2=0;
	int cnt_zero=0,cnt_one=0,cnt_11=0,cnt_12=0,cnt_13=0,tmpdis;//ͳ�Ʋ�Ϊ0,1,11,12,13�ĸ���д��d:\trend2.txt��������
	for(int i=0,j=0;i<l1;){
		if(t1[i]!=255 && t2[j]!=255){
			tmpdis= (int)(abs((int)t1[i]-t2[j]-avr)) %12;
			//sum += tmpdis*phi_distribute[tmpdis];
			sum += tmpdis;
			if(abs((int)t1[i]-t2[j]-avr)==0)cnt_zero++;
			if(abs((int)t1[i]-t2[j]-avr)==1)cnt_one++;
			if(abs((int)t1[i]-t2[j]-avr)==11)cnt_11++;
			if(abs((int)t1[i]-t2[j]-avr)==12)cnt_12++;
			if(abs((int)t1[i]-t2[j]-avr)==13)cnt_13++;
			len++;
		}
		cnt1++;
		if(cnt1==c1[i]){
			cnt1=0;
			i++;
		}
		cnt2++;
		if(cnt2==c2[j]){
			cnt2=0;
			j++;
		}
	}
	//*/

	
	//fp=fopen("D:\\trend2.txt","a");
	//fprintf(fp,"DataSet #%d:\n0: %.6f\n1: %.6f\n11: %.6lf\n12: %.6lf\n13: %.6lf\n\n",dataset,1.0*cnt_zero/len,1.0*cnt_one/len,1.0*cnt_11/len,1.0*cnt_12/len,1.0*cnt_13/len);
	//fclose(fp);
	
	sum/=len;
	for(int i=0;i<l1;i++)c1[i]/=ext1;
	for(int i=0;i<l2;i++)c2[i]/=ext2;
	return sum;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** ���ǰ������������ǰ�ߵ����ߵ��ں��ߣ�
 * ��ô��fft֮��ǰ�ߵ��������׵õ��������ӣ����ߵ��������׵õ��������٣�
 * ���Ե������������ʱ��Ӧ�ü���ǰ�ߵ����������Ӻ��ߵ�����
 */
void tidy(unsigned char *tone,unsigned int *last,int &lcnt) {
	int a=0,b=0;
	for (int i=1;i<lcnt-1;i++) {
		if (tone[i-1]<tone[i] && tone[i]!=tone[i+1]){
			if (last[i-1]>last[i]){
				a=(int)last[i-1]/last[i];
				last[i-1]-=a;
				last[i]+=a;
			}
		}
	}
	for (int i=lcnt-2;i>=1;i--) {
		if (tone[i-1]<tone[i] && tone[i]!=tone[i+1]){
			if (last[i-1]>last[i]){
				b=(int)last[i-1]/last[i];
				last[i-1]-=b;
				last[i]+=b;
			}
		}
	}
}

void smooth(unsigned char *tone,unsigned int *last,int &lcnt) { // lcnt��tone��last����ͬMatchDifferences�����е�l1��t1��c1

	// ȥ����ͷ�ͽ�β��������Ϊ��ͷ��β����û�ã�
	while(tone[lcnt-1]==255)lcnt--;
	int it;
	for (it=0;it<lcnt;it++) {
		if (tone[it]!=255)
			break;
	}
	for(int i=it;i<lcnt;i++){
		tone[i-it]=tone[i];
		last[i-it]=last[i];
	}
	lcnt-=it;
	
	//*/
	// �����仯ƽ���������м�ľ������滻Ϊ����֮ǰ����
	for (int j=0;j<lcnt-1;j++) {
		if (tone[j]!=255 && tone[j+1]==255)
			tone[j+1] = tone[j];
	}
	//*/
	// ��ǰ��������������12����������������Ϊ��������12
	for (int i=0;i<lcnt-1;i++) {
		if (abs(tone[i+1]-tone[i])>12){
			if(tone[i+1]>tone[i])tone[i+1] -= 12;
			else tone[i] -= 12;
		}
	}
	for(int i=lcnt-2;i>=0;i--){
		if (abs(tone[i+1]-tone[i])>=12){
			if(tone[i+1]>tone[i])tone[i+1] -= 12;
			else tone[i] -= 12;
		}
	}
	// ��ǰ�������������Ϊ12������Ϊ��ͬ���������߽���12
	for (int i=0;i<lcnt-1;i++) {
		if (abs(tone[i+1]-tone[i])==12){
			if(tone[i+1]>tone[i])tone[i+1] -= 12;
			else tone[i] -= 12;
		}
	}
	for(int i=lcnt-2;i>=0;i--){
		if (abs(tone[i+1]-tone[i])==12){
			if(tone[i+1]>tone[i])tone[i+1] -= 12;
			else tone[i] -= 12;
		}
	}
	//
	for (int i=1;i<lcnt-1;i++) {
		if (abs(tone[i]-tone[i-1])>=3 && abs(tone[i+1]-tone[i])>=3 && last[i]<=3)
			tone[i]=(tone[i+1]+tone[i-1])/2;
	}

	for (int i=1;i<lcnt-2;i++) {
		if (abs(tone[i]-tone[i-1])>=3 && abs(tone[i+2]-tone[i+1])>=3 && abs(tone[i+1]-tone[i])<=1 &&  last[i]<=3 && last[i+1]<=3){
			tone[i]=(tone[i+2]+tone[i-1])/2;
			tone[i+1]=(tone[i+2]+tone[i-1])/2;
		}
	}

	for(int i=1;i<lcnt-1;i++){
		if(tone[i-1]==tone[i+1] && abs(tone[i]-tone[i-1])==1 && last[i]*2<last[i-1]+last[i+1]){
			tone[i]=tone[i-1];
		}
	}
	//*/
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd ) {
	// ����Ҫƥ�����������
	FILE *fp=fopen("toneseq.bin","rb"),*fs=fopen("filelist.bin","r");
	if (!fp || !fs) {
		MessageBox(0,"�ó���û�з�����ȷ��λ�����У�����MusicDetection������ǰû������Ҫƥ�����Ƶ��","����",MB_ICONWARNING);
		return 1;
	}
	int lcnt;
	fread(&lcnt,4,1,fp);
	unsigned char *tones=new unsigned char[lcnt];
	unsigned int *lasts=new unsigned int[lcnt];
	fread(tones,1,lcnt,fp);
	fread(lasts,4,lcnt,fp);
	smooth(tones,lasts,lcnt); // ��Ҫƥ�����������smooth����
	fclose(fp);
	static char filename[MAX_PATH];
	while (~fscanf(fs,"%s",filename))
		musicList.push_back(filename);


	FILE *fres=fopen("matchdiff.bin","w");
	fprintf(fres,"%d\n",musicList.size());
	for (unsigned i=0;i<musicList.size();i++) {
		// �������ݿ���ĳ������������
		FILE *fp=fopen(musicList[i].c_str(),"rb");
		int len;
		fread(&len,4,1,fp);
		unsigned char *ftone=new unsigned char[len];
		unsigned int *flast=new unsigned int[len];
		fread(ftone,1,len,fp);
		fread(flast,4,len,fp);
		tidy(ftone,flast,len); // �����ݿ����������������tidy����
		smooth(ftone,flast,len); // �����ݿ����������������smooth����
		float diff=MatchDifferences(lcnt,tones,lasts,len,ftone,flast); // ���������������ݵ���MatchDifferences�õ�һ������ȹ���
		fprintf(fres,"%s %f\n",musicList[i].c_str(),diff);
		delete []ftone;
		delete []flast;
		fclose(fp);
	}
	fclose(fres);
	WinExec("matcher",1); // ͨ���ⲿmatcher������ʾ����

	delete []tones;
	delete []lasts;
	return 0;
}