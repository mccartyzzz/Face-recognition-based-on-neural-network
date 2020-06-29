#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include <iomanip> 
#include <windows.h>
#include <fstream>
using namespace std;

const int N = 32*32;//�������Ԫ���� 
const int L = 16*16;//��������Ԫ���� 
const int M = 16;//�������Ԫ���� 

double epsilon = 0.01;//����
double alpha = 0.2;//ѧϰ�� 

double V[N][L];//����㵽�������Ȩֵ 
double W[L][M];//�����㵽������Ȩֵ 
double phi[L];//���������ֵ
double theta[M];//��������ֵ
	
double delteV[N][L];//������ֵʱ V�ı仯�� 
double delteW[L][M];//������ֵʱ W�ı仯�� 
double deltephi[L];//������ֵʱ��������ֵ�ı仯�� 
double deltetheta[M];//������ֵʱ�������ֵ�ı仯��  

void Initialization();//��ʼ������ 
double F(double x);//�������� 
void GetPixel(double *array, char *imagepath);//���ͼƬ�Ҷ�ֵ 
int Training(double *X, double *D);//������ѵ�� 
int Match(double *result, double *X);//ƥ��־Ը�߱�� 

int main()
{
	cout<<"���к��������Ե� ~ QAQ"<<endl;
	cout<<"�������㣬�����Ҫ45s����..."<<endl; 
	Initialization();//���ú�������ʼ������ 
	int times = 200;//����ѭ�����������ѭ�� 200�Σ������û�дﵽ���Ⱦͷ��� 
	while(times-->0){
		for(int p = 1;p <= 5; p++){//����־Ը�ߵĵ�һ����Ƭ��ֻѵ��ÿ��־Ը�ߵ�ǰ5��ͼƬ
			for(int volunteer = 1;volunteer < M; volunteer++){;//����־Ը�߱�ţ���Ŵ� 1-15 
				char imagepath[50];//���� char�����飬���ڱ���ͼƬ��·�� 
				memset(imagepath,0,sizeof(imagepath));
				char start_num[3];//־Ը�߱�� 
				char end_num[3];//ͼƬ��� 
				memset(start_num,0,sizeof(start_num));
				memset(end_num,0,sizeof(end_num));
				int x1 = volunteer % 10;//��ȡ־Ը�߱�ŵ���λ���� 
				int x2 = volunteer / 10;
				start_num[0] = (x2+'0');//ת�����ַ��ͱ����������У�ʮλ������ 
				start_num[1] = (x1+'0');//��λ������ 
				start_num[2] = '\0';//����ֹ������ֹ��ӡ������ 
				
				if(p<10){//�����Ƭ���С��10 
					int y1 = p % 10;
					end_num[0] = (y1+'0');//ֱ�ӱ���һλ������ 
					end_num[1] = '\0';
				}else{//�����ʮλ����ͬ־Ը�߱�� 
					int y1 = p%10;
					int y2 = p / 10;
					end_num[0] = (y2+'0');
					end_num[1] = (y1+'0');
					end_num[2] = '\0';
				}//��Ϊ��Ƭ�Ĳ���Ҫǰ��� 0�������������Ƭ��ֱ������ 3��־Ը�߱������Ҫ 03 
				strcat(imagepath,"./YALE/subject");//ʹ��strcat���������ҵ���ȷ��·�� 
				strcat(imagepath,start_num);
				strcat(imagepath,"_");
				strcat(imagepath,end_num);
				strcat(imagepath,".bmp");
				
				double X[N];//������Ҫ����ͼƬ�Ҷ�ֵ������ 
				memset(X,0,sizeof(X));
				GetPixel(X,imagepath);//���ú�����ȡ�Ҷ�ֵ 
				double D[M];//��ʾ��������� 
				memset(D,0,sizeof(D));
				D[volunteer] = 1.0;//����ǵ� volunteer��־Ը����ϣ��ƥ����Ϊ 100%��� volunteerλ��Ϊ 1 
				int flag = Training(X, D);//���ú���������ѵ�� 
				if(flag) {break;}//����ﵽ����������Ҫ������һ�ε�ѵ�� 
			}
		}
	}
	
	int num = 10;	
	while(num--){//ѭ������������� 
		cout<<endl;
		cout<<"��ֱ�����־Ը�߱�� �� ��Ƭ��� --> ����(01 , 1)����λ��־Ը�߱����Ҫǰ����� 0"<<endl;//�ֱ�����־Ը�߱�ź���Ƭ������ 
		char t1[3];
		char t2[3];
		memset(t1,0,sizeof(t1));
		memset(t2,0,sizeof(t2));
		cin>>t1;//����־Ը�߱�� 
		cin>>t2;//��Ƭ��� 
		char imagepath[50];//����·�� 
		memset(imagepath,0,sizeof(imagepath));
		strcat(imagepath,"./YALE/subject");
		strcat(imagepath,t1);
		strcat(imagepath,"_");
		strcat(imagepath,t2);
		strcat(imagepath,".bmp");//ʹ��strcat����������� 
		cout<<"ͼƬ·��Ϊ��"<<endl;
		cout<<imagepath<<endl;//��ӡ���� 
		double X[N];//������Ҫ����ͼƬ�Ҷ�ֵ������ 
		memset(X,0,sizeof(X));
		double result[M];//�������㱣���������Ľ�� 
		memset(result,0,sizeof(result));
		GetPixel(X, imagepath);//��ûҶ�ֵ 
		int index = Match(result, X);//���н�����ж� ������־Ը�߱�� 
		cout<<"�����־Ը����Ƭƥ��������"<<endl;
		for(int i=1; i<M; i++){
			cout<<"volunteer"<<i<<" --> "<<(double)result[i]*100.0<<"%"<<endl;//��ӡƥ���� 
		}
		cout<<"����ƥ�䵽��־Ը�߱���ǣ�"<<index<<endl<<endl;//��ӡ־Ը�߱�� 
	}
	return 0;
} 

/*
	Name:Initialization()
	Date: 06/06/20 15:42
	Description:û�з���ֵ�����ڳ�ʼ��ȫ�ֱ������� 
				�������ز㵽�����Ķ�ά���� W��������㵽���صĶ�ά���� V 
				�Լ�һά������������ֵ���飬һά�����������ֵ���� 
				Ϊ��������һЩ�ӽ��� 0�����ֵ 
*/
void Initialization(){
	for(int i = 0; i < N; i++){
		for(int j = 0; j < L; j++){
			V[i][j] = (rand()%100/10000.0);//���ɺܽӽ��� 0�������������ΪС�����4λС�� 
		}
	}
	for(int j = 0; j < L; j++){
		for(int k = 1; k < M; k++){
			W[j][k] = (rand()%100/10000.0);
		}
	}
	for(int j = 0; j < L; j++){
		phi[j] = (rand()%100/10000.0);
	}
	for(int k = 1; k < M; k++){
		theta[k] = (rand()%100/10000.0);
	}
}

/*
	Name: F() 
	Date: 06/06/20 15:48 
	Description:˫���ȷ���ֵ�����ڼ��㼤��������ֵ f(x) = 1/(1+e^-x)
*/
double F(double x){
	return 1.0 / (1.0 + exp(-1.0*x));   
}

/*
	Name:GetPixel()
	Date: 06/06/20 16:52 
	Description:û�з���ֵ����������һ��˫���ȶ�ά���飬һ��char������
				���ڽ� imagepath·���µ�ͼƬת���ɻҶ�ֵ�İٷֱȣ����ұ��浽array������
				��ָ�뷵��
				����ʹ�� FILE�����һ��ָ�� fp�� imagepath·���µ��ļ���Ȼ��ʹ�� fseek��������.bmpͼƬ��ʽ��ͷ�ṹ
				���� headָ���ȡ��ͼƬ�ı�Ҫ����Ϣ������ͼƬ�Ŀ�͸�
				��ȡ BIBitCount��������ÿ��������ռ�ֽ�������ȡ�Ҷ�ͼ�����ɫ���ж����ֽ�
				������ɫ��ָ�룬����ռ䣬��ɫ�ʶ����ڴ�
				Ѱ�ҵ� fpָ���ļ�����ɫ������λͼ����ָ�룬ͨ�����ָ���ȡͼƬ�ĻҶ�ֵ
				����ת��Ϊ�ٷֱȷ������� 
*/
void GetPixel(double *array, char *imagepath){
	FILE *fp = fopen(imagepath,"rb");//��·���µ��ļ� 
	if(fp == NULL){
		cout<<"phil to open the file !"<<endl;
		return ;
	}
	fseek(fp, sizeof(BITMAPFILEHEADER),0);//����λͼ�ļ�ͷ�ṹBITMAPFILEHEADER
	BITMAPINFOHEADER head;  
	fread(&head, sizeof(BITMAPINFOHEADER), 1,fp);//��ȡͼ����ߡ�ÿ������ռλ������Ϣ
	int bmpWidth = head.biWidth;//ͼƬ�Ŀ� 
	int bmpHeight = head.biHeight;//ͼƬ�ĸ� 
	
	int biBitCount = head.biBitCount;//�������������ͼ��ÿ��������ռ���ֽ�����������4�ı�����
	int lineByte = (bmpWidth * biBitCount/8+3)/4*4;//�Ҷ�ͼ������ɫ������ɫ�����Ϊ256
	RGBQUAD *pColorTable;//��ɫ��ָ��
	if(biBitCount == 8)
	{
		//������ɫ������Ҫ�Ŀռ䣬����ɫ����ڴ�
		pColorTable = new RGBQUAD[256];
		fread(pColorTable,sizeof(RGBQUAD),256,fp);
	}
	//����λͼ��������Ҫ�Ŀռ䣬��λͼ���ݽ��ڴ�
	unsigned char *pBmpBuf;//����ͼ�����ݵ�ָ��
	pBmpBuf = new unsigned char[lineByte * bmpHeight];//����ȡ�����ݱ��浽�ڴ����棬ʹ��ָ�� pBmpBufָ�� 
	fread(pBmpBuf,1,lineByte * bmpHeight,fp);//��Ѱ�ķ�ʽ�� pBmpBufָ�� fp�ڣ�������ͼƬ�ĳ��ȣ�һ��һ���ֽڵķ�ʽ����Ѱ�� 
	
	int k = 0;//����һ�����ȣ�ÿ��ȡһ���ֽ����鳤�Ⱦ� +1 
	for(int i = 50-M; i < 50+M; i++){
		for(int j = 40-M; j < 40+M; j++){
			array[k++] = (int)pBmpBuf[(i-1)*80+j] / 255.0;//��ȡ������ת���� int�ʹ������� array�У�����ת���ɻҶ�ֵ�ٷֱ� 
		}
	}
	fclose(fp);//�ر��ļ�
}

/*
	Name:Training() 
	Date: 06/06/20 16:06
	Description:ѵ�����������飬������������һ����ȡ��ͼƬ�ĻҶ�ֵ��һ��˫���ȵ�����ָ�룬���������ֵ
				���� bool�ͣ�����ﵽ�����򷵻� True���򷵻� false
				�����趨���ȣ���ӡ���������ֶ���4λС��
				����˫�������� h�� y�ֱ�������������㵽���ص����ֵ�����ز㵽�����Ľ��
				���������������������Լ������ز�֮��������
				����Ȩֵ V�� W�ı仯�㣬�������ز�������������
				���������߽��е��� 
*/
int Training(double *X, double *D){
		cout<<setiosflags(ios::fixed)<<setprecision(4);//�淶С����ӡ��λ����С�����4λ 
		double h[L];//˫���� h��������㵽���ز�Ľ�� 
		memset(h,0,sizeof(h));
		for(int j = 0; j < L; j++){
			double delte = 0.0;//���չ�ʽ���������� Xi*Vij���ۻ��� 
			for(int i = 0; i < N; i++){
				delte += X[i]*V[i][j];
			}
			h[j] = F(delte+phi[j]);//������ֵ 
		}
		double y[M];//˫���� h�������ز㵽�����Ľ�� 
		memset(y,0,sizeof(y));
		for(int k = 1; k < M; k++){
			double delte = 0.0;//���չ�ʽ���������� hj*Wij���ۻ���
			for(int j = 0; j < L; j++){
				delte += h[j]*W[j][k];
			}
			y[k] = F(delte+theta[k]);//������ֵ 
		}
		
		double e = 0.0;//���屣��������С�� 
		for(int k = 1; k < M; k++){
			e += (D[k] - y[k])*(D[k] - y[k]);//��������� 
		}
		if(e*0.5 < epsilon){//�������1/2*(e)���㾫���򷵻�True 
			return 1;
		}
		
		double delteK[M];//˫�������������������������� 
		memset(delteK,0,sizeof(delteK));
		for(int k = 1; k < M; k++){
			delteK[k] = (D[k]-y[k])*y[k]*(1-y[k]);//����� 
		}

		double delteJ[L];//˫��������������������ز����� 
		memset(delteJ,0,sizeof(delteJ));
		for(int j = 0; j < L; j++){
			double beta = 0.0;//��������ܺ��� 
			for(int k = 1; k < M; k++){
				beta += delteK[k]*W[j][k];
			}
			delteJ[j] = h[j]*(1-h[j])*beta;//���չ�ʽ������� 
		}
		
		for(int j = 0; j < L; j++){
			deltephi[j] = alpha*delteJ[j];//������ֵ�ĵ�����
//			deltephi[j] = (alpha/(1.0+L))*(deltephi[j]+1)*delteJ[j]; ??����е��?? 
			phi[j] += deltephi[j];//�������������ֵ 
		}
		
		for(int i = 0; i < N; i++){
			for(int j = 0; j < L; j++){
				delteV[i][j] = alpha*delteJ[j]*X[i];//����Ȩֵ�ĵ����� 
//				delteV[i][j] = (alpha/(1.0+N))*(delteV[i][j]+1)*delteJ[j]*X[i];
				V[i][j] += delteV[i][j];//���� VȨֵ
			}
		}
		
		for(int j = 0; j < L; j++){
			for(int k = 1; k < M; k++){
				delteW[j][k] = alpha*delteK[k]*h[j];//����Ȩֵ�ĵ����� 
//				delteW[j][k] = (alpha/(1.0+L))*(delteW[j][k]+1)*delteK[k]*h[j];
				W[j][k] += delteW[j][k];//���� WȨֵ 
			}
		}
		for(int k = 1; k < M; k++){
			deltetheta[k] = alpha*delteK[k];//������ֵ�ĵ����� 
//			deltetheta[k] = (alpha/(1.0+L))*(deltetheta[k]+1)*delteK[k];
			theta[k] += deltetheta[k];//������������ֵ 
		}
	return 0;
}

/*
	Name:Match()
	Date: 06/06/20 16:20
	Description:����־Ը�߱�ţ�һ������������Ҫ����ƥ���ͼƬ�ĻҶ�ֵ
				�ж�����ƥ�������һ��־Ը�ߣ�ͬ���ȼ������
				Ȼ��ֱ���ÿһλ֧Ԯ�ߵ�ͼƬ������ֵ���жԱȣ��ҵ����ֵ��С
				�����ֵ��Сʱ�����ظ�־Ը�߱�� 
*/
int Match(double *result, double *X){
		int index = 0;//���յ�����Ľ����־Ը�߱�� 
		double h[L]; //˫���� h��������㵽���ز�Ľ�� 
		memset(h,0,sizeof(h));
		for(int j = 0; j<L; j++){
			double delte = 0.0;//���չ�ʽ���������� Xi*Vij���ۻ��� 
			for(int i = 0; i<N; i++){
				delte += X[i]*V[i][j];
			}
			h[j] = F(delte-phi[j]);//������ֵ 
		}
		double y[M];//˫���� h�������ز㵽�����Ľ�� 
		memset(y,0,sizeof(y));
		for(int k = 1; k < M; k++){
			double delte = 0.0;//���չ�ʽ���������� hj*Wij���ۻ���
			for(int j = 0; j < L; j++){
				delte += h[j]*W[j][k];
			}
			y[k] = F(delte-theta[k]);//������ֵ 
		}
		
		double min = 9999999.0;//����һ���ܴ�������������ĶԱ� 
		for(int n=1; n<M; n++){//ѭ������15��־Ը�� 
			double e = 0.0;//�����뵱ǰ־Ը��֮�������� 
			double D[M];//������������ 
			memset(D,0,sizeof(D));
			D[n] = 1.0;///��ǰ�ǵڼ�λ־Ը�ߣ��ڴ���֮ƥ�䣬������������� 
			for(int k = 1; k < M; k++){
				e += abs((D[k] - y[k])*(D[k] - y[k]));//���㵱ǰ�����ֵ 
			}
			result[n] = 1.0/exp(e*0.5);//���ڱ���������ʽ�����ԽС���ԽСƥ���Խ�� 
			if(e*0.5 < min){
				min = e*0.5;//��¼��С����� 
				index = n;//��¼־Ը�߱�� 
			}
		}
		return index;//����־Ը�߱�� 
	}
