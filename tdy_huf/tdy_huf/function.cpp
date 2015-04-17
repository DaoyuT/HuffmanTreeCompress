#include "stdafx.h"

extern Node HufNode[MAX_NODE],tmp;


double compress(TCHAR*openfile ,TCHAR*outputfile,TCHAR*huffile,HWND hwnd)
{
	char buf[MAX_NODE];
	unsigned char c;
	int i,j,m,n,t;
	int flen1,flen2,flen=0;//��С��㡢ԭ�ļ����ȡ�ѹ�����ļ�����
	double rate;       //ѹ������
	FILE *ifp,*ofp,*hfp;//�ֱ�Ϊ���롢������������ļ�ָ��
	HWND hwndlist;
	char str[MAX_CHARS] = {0};

	ifp=fopen(openfile,"rb");
	ofp=fopen(outputfile,"wb");
    hfp=fopen(huffile,"wb");

	//ͳ�ƴ�Ƶ
	while(!feof(ifp))
	{
		fread(&c,1,1,ifp);
		HufNode[c].count++;
		flen++;
	}
	flen--;
	HufNode[c].count--;

	//��ʼ��ÿ���ڵ� ÿ��ascii��ֵ��˳���Ӧ��HufNode������
	for(i=0;i<MAX_NODE;i++)
	{
		if(HufNode[i].count!=0) HufNode[i].b=(unsigned char)i;
		else HufNode[i].b=0;
		HufNode[i].parent=-1;
		HufNode[i].lch=HufNode[i].rch=-1;
		HufNode[i].code_len=0;
	}

	//����Ƶ��ÿ���ڵ����򣨽���
	for(i=0;i<MAX_CHARS;i++)
	{
		for(j=i+1;j<MAX_CHARS;j++)
		{
			if(HufNode[i].count<HufNode[j].count)
			{
				tmp=HufNode[i];
				HufNode[i]=HufNode[j];
				HufNode[j]=tmp;
			}
		}
	}

	for(i=0;i<MAX_CHARS;i++) if(HufNode[i].count==0) break;
	n=i;
	m=2*n-1;//n���ַ���Ӧ�Ĺ�������Ҫ2n-1���ڵ�

	//����ÿ���ڵ�Ȩ�غ͹������㷨�����������
	for(i=n;i<m;i++)
	{
		flen1=MAXIMUM;
		for(j=0;j<i;j++)
		{
			if(HufNode[j].parent!=-1) continue; //�ҵ���ǰ�����ڹ��������еĽY�c
			if(flen1>HufNode[j].count)
			{
				flen2=j;
				flen1=HufNode[j].count;
			}
		}
		HufNode[i].count=HufNode[flen2].count;
		HufNode[flen2].parent=i;
		HufNode[i].lch=flen2;
		flen1=MAXIMUM;
		for(j=0;j<i;j++)
		{
			if(HufNode[j].parent!=-1) continue;
			if(flen1>HufNode[j].count)
			{
				flen2=j;
				flen1=HufNode[j].count;
			}
		}
		HufNode[i].count+=HufNode[flen2].count;
		HufNode[i].rch=flen2;
		HufNode[flen2].parent=i;
	}

	//������0��1�Ĺ����ÿ���ڵ��ƶ�����������
	for(i=0;i<n;i++)
	{
		t=i;
		HufNode[i].code[0]=0;  //���ڵ������0
		HufNode[i].code_len++;
		while(HufNode[t].parent!=-1)
		{
			j=t;
			t=HufNode[t].parent;
			if(HufNode[t].lch==j)
			{
				j=strlen(HufNode[i].code);
				memmove(HufNode[i].code+1,HufNode[i].code,j+1);
				HufNode[i].code[0]='0'; //��������������һλ�ٸ���һ����ֵ0 ���ڱ�������˲���0
				HufNode[i].code_len++;
			}
			else
			{
				j=strlen(HufNode[i].code);
				memmove(HufNode[i].code+1,HufNode[i].code,j+1);
				HufNode[i].code[0]='1'; //��������������һλ�ٸ���һ����ֵ1 ���ڱ�������˲���1
				HufNode[i].code_len++;
			}
		}
	}
	fseek(ifp,0,SEEK_SET);  //��λ��Դ�ļ���ͷ
	fwrite(&flen,sizeof(int),1,ofp);
	fseek(ofp,8,SEEK_SET);//��8���ֽ� ǰ�ĸ�д��Դ�ļ��ַ��� ���ĸ��ֽ�д��Ŀ���ļ��ַ���
	buf[0]=0;  //��ʼ���׵�ַ

	//д��Ŀ���ļ�
	for(t=0,flen2=0;!feof(ifp)||t==flen;t++)
	{
		c=fgetc(ifp);
		for(i=0;i<n;i++)  //�ҵ��ַ���Ӧ�Ľڵ�
		{
			if(c==HufNode[i].b) break;
		}
		strcat(buf,HufNode[i].code);
		j=strlen(buf);
		c=0;
		while(j>=8)         //ÿ8λһ���� �ȴ�����ǰ��8λ
		{
			for(i=0;i<8;i++)
			{
				if(buf[i]=='1') c=(c<<1)|1; //���һλ���� 1
				else c=c<<1;   //���һλ���� 0
			}
			fwrite(&c,1,1,ofp); //ÿ����8λ д��
			flen2++;
			strcpy(buf,buf+8); //�����λ���ӵ�buf�� �����´�ѭ������
			j=strlen(buf);
		}
	}

	if(j>0)        //���һ�β���8λ
	{
		strcat(buf,"00000000");
		for(i=0;i<8;i++)
		{
			if(buf[i]=='1') c=(c<<1)|1;
			else c=c<<1;
		}
		fwrite(&c,1,1,ofp);
		flen2++;
	}
	flen2--;
	
	fseek(ofp,4,SEEK_SET);
	fwrite(&flen2,sizeof(int),1,ofp);  //д��Ŀ���ļ��ַ���
	hwndlist = GetDlgItem(hwnd, IDC_LIST1);
	wsprintf(str, "�ַ�  Ƶ��   ����");
	ListBox_InsertString(hwndlist, -1, str);
	for(i=0;i<n;i++)
	{
		wsprintf(str, "%c      %d     %s",HufNode[i].b,HufNode[i].count,HufNode[i].code);
		ListBox_InsertString(hwndlist, -1, str);
	}
	//д���������
	fwrite(&n,sizeof(int),1,hfp); //д���ַ�����
	for(i=0;i<n;i++)
	{
		fwrite(&(HufNode[i].b),1,1,hfp);  //д���ַ�
		c=strlen(HufNode[i].code);
		fwrite(&c,1,1,hfp);      //д����������볤��
		j=c;
		if(j%8!=0)              //д�����������
		{
			for(t=j%8;t<8;t++)
				strcat(HufNode[i].code,"0");
		}
		while(HufNode[i].code[0]!=0)
		{
			c=0;
			for(j=0;j<8;j++)
			{
				if(HufNode[i].code[j]=='1') c=(c<<1)|1;
				else c=c<<1;
			}
			strcpy(HufNode[i].code,HufNode[i].code+8);
			fwrite(&c,1,1,hfp);
		}
	}
	for(i=0;i<n;i++)
	{
		HufNode[i].b = 0;
		HufNode[i].count = 0; 
		HufNode[i].parent = 0;
		HufNode[i].lch = 0;
		HufNode[i].rch = 0;
		memset(HufNode[i].code,0,20*sizeof(char));
		HufNode[i].code_len = 0;
	}
	fclose(ifp);
	fclose(ofp);
	fclose(hfp);
	rate=(double)flen2/flen*100;
	return rate;
}
double uncompress(TCHAR*openfile ,TCHAR*outputfile,TCHAR*huffile,HWND hwnd)
{
	char buf[MAX_CHARS],buf2[MAX_CHARS];
	unsigned char c;
	int i,j,n,m,f,l,hlen;
	int flen,flen1;
	FILE *ifp,*ofp,*hfp;
	HWND hwndlist;
	char str[MAX_CHARS] = {0};
	double rate;

	ifp=fopen(openfile,"rb");
	hfp=fopen(huffile,"rb");
	ofp=fopen(outputfile,"wb");

	fread(&flen,sizeof(int),1,ifp);//Դ�ļ�����
	fread(&flen1,sizeof(int),1,ifp);//Ŀ���ļ�����
	fread(&n,sizeof(int),1,hfp);//�ַ�����
    rate = double(flen1)/flen*100;
	//��ȡ��������
	for(i=0;i<n;i++)
	{
		fread(&HufNode[i].b,1,1,hfp);
		fread(&c,1,1,hfp);
		hlen=(int)c;
		HufNode[i].code_len=hlen;
		HufNode[i].code[0]=0;
		if(hlen%8!=0) m=hlen/8+1; //���ҳ����ַ���Ӧ�Ĺ������������ص��ֽ���������ݣ����һ���ֽڿ��ܲ���ȫ�Ǹù��������룩
		else m=hlen/8;
		for(j=0;j<m;j++)
		{
			fread(&c,1,1,hfp);
			f=c;
			itoa(f,buf,2);
			f=strlen(buf);
			for(l=8;l>f;l--)
			{
				strcat(HufNode[i].code,"0");
			}
			strcat(HufNode[i].code,buf);
		}
		HufNode[i].code[hlen]=0; //�ٽ�ȡ�ù���������ĳ��ȣ�����ȡ��Ч�Σ�
	}

	//�ҵ���Ĺ��������볤��
	for(i=0,hlen=0;i<n;i++)
	{
		if(strlen(HufNode[i].code)>hlen) hlen=strlen(HufNode[i].code);
	}
	fseek(ifp,8,SEEK_SET);//�ҵ��ļ���ͷ������Դ�ļ����Ⱥ�Ŀ���ļ����ȵ�8���ֽڣ�
	buf2[0]=0;

	//����
	for(j=0;j!=flen;j++)
	{

		while(strlen(buf2)<(unsigned int)hlen)
		{
			fread(&c,1,1,ifp);
			f=c;
			itoa(f,buf,2);
			f=strlen(buf);
			for(l=8;l>f;l--)
			{
				strcat(buf2,"0");
			}
			strcat(buf2,buf);
		}
		for(i=0;i<n;i++)//�ҵ���Ӧ���ַ�
		{
			if(memcmp(HufNode[i].code,buf2,HufNode[i].code_len)==0) break;
		}
		strcpy(buf2,buf2+HufNode[i].code_len); //ɾ��֮ǰ��ȡ���ַ���Ӧ�Ĺ���������
		c=HufNode[i].b;
		fwrite(&c,1,1,ofp);  //д���ַ�
	}
	hwndlist = GetDlgItem(hwnd, IDC_LIST1);
	wsprintf(str, "�ַ�   ����");
	ListBox_InsertString(hwndlist, -1, str);
	for(i=0;i<n;i++)
	{
		wsprintf(str, "%c  %s",HufNode[i].b,HufNode[i].code);
		ListBox_InsertString(hwndlist, -1, str);
	}
	for(i=0;i<n;i++)
	{
		HufNode[i].b = 0;
		HufNode[i].count = 0; 
		HufNode[i].parent = 0;
		HufNode[i].lch = 0;
		HufNode[i].rch = 0; 
		memset(HufNode[i].code,0,20*sizeof(char));
		HufNode[i].code_len = 0;
	}
	fclose(ifp);
	fclose(ofp);
	fclose(hfp);
	return rate;
}

int read (HWND hwnd,TCHAR* filename)
{
	TCHAR a[10000];
	int count  = 0;
	unsigned char c;
	FILE* fp;
	
	fp=fopen(filename,"rb");
	while(!feof(fp))
	{
		fread(a + count,1,1,fp);
		if (!feof(fp)) count++;		
	}
	
	SetDlgItemText(hwnd,IDC_EDIT1,a);
	return 0;

}

BOOL CLEARSCREEN(HWND hwnd, int CTRID)
{
	int count, i;
	HWND hwndlist;

	hwndlist = GetDlgItem(hwnd, CTRID);
	count = ListBox_GetCount(hwndlist);
	for (i = 0; i < count; i++)
	{
		ListBox_DeleteString(hwndlist, 0);
	}
	return TRUE;
}