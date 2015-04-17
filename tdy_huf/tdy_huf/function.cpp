#include "stdafx.h"

extern Node HufNode[MAX_NODE],tmp;


double compress(TCHAR*openfile ,TCHAR*outputfile,TCHAR*huffile,HWND hwnd)
{
	char buf[MAX_NODE];
	unsigned char c;
	int i,j,m,n,t;
	int flen1,flen2,flen=0;//最小结点、原文件长度、压缩后文件长度
	double rate;       //压缩比率
	FILE *ifp,*ofp,*hfp;//分别为输入、输出、哈弗曼文件指针
	HWND hwndlist;
	char str[MAX_CHARS] = {0};

	ifp=fopen(openfile,"rb");
	ofp=fopen(outputfile,"wb");
    hfp=fopen(huffile,"wb");

	//统计词频
	while(!feof(ifp))
	{
		fread(&c,1,1,ifp);
		HufNode[c].count++;
		flen++;
	}
	flen--;
	HufNode[c].count--;

	//初始化每个节点 每个ascii码值按顺序对应到HufNode数组中
	for(i=0;i<MAX_NODE;i++)
	{
		if(HufNode[i].count!=0) HufNode[i].b=(unsigned char)i;
		else HufNode[i].b=0;
		HufNode[i].parent=-1;
		HufNode[i].lch=HufNode[i].rch=-1;
		HufNode[i].code_len=0;
	}

	//按词频给每个节点排序（降序）
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
	m=2*n-1;//n种字符对应的哈夫曼树要2n-1个节点

	//根据每个节点权重和哈夫曼算法构造哈夫曼树
	for(i=n;i<m;i++)
	{
		flen1=MAXIMUM;
		for(j=0;j<i;j++)
		{
			if(HufNode[j].parent!=-1) continue; //找到當前不存在哈夫曼樹中的結點
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

	//根据左0右1的规则给每个节点制定哈弗曼编码
	for(i=0;i<n;i++)
	{
		t=i;
		HufNode[i].code[0]=0;  //根节点编码置0
		HufNode[i].code_len++;
		while(HufNode[t].parent!=-1)
		{
			j=t;
			t=HufNode[t].parent;
			if(HufNode[t].lch==j)
			{
				j=strlen(HufNode[i].code);
				memmove(HufNode[i].code+1,HufNode[i].code,j+1);
				HufNode[i].code[0]='0'; //哈弗曼编码右移一位再给第一个赋值0 即在编码最左端插入0
				HufNode[i].code_len++;
			}
			else
			{
				j=strlen(HufNode[i].code);
				memmove(HufNode[i].code+1,HufNode[i].code,j+1);
				HufNode[i].code[0]='1'; //哈弗曼编码右移一位再给第一个赋值1 即在编码最左端插入1
				HufNode[i].code_len++;
			}
		}
	}
	fseek(ifp,0,SEEK_SET);  //定位到源文件开头
	fwrite(&flen,sizeof(int),1,ofp);
	fseek(ofp,8,SEEK_SET);//留8个字节 前四个写入源文件字符数 后四个字节写入目的文件字符数
	buf[0]=0;  //初始化首地址

	//写入目的文件
	for(t=0,flen2=0;!feof(ifp)||t==flen;t++)
	{
		c=fgetc(ifp);
		for(i=0;i<n;i++)  //找到字符对应的节点
		{
			if(c==HufNode[i].b) break;
		}
		strcat(buf,HufNode[i].code);
		j=strlen(buf);
		c=0;
		while(j>=8)         //每8位一处理 先处理先前的8位
		{
			for(i=0;i<8;i++)
			{
				if(buf[i]=='1') c=(c<<1)|1; //最后一位插入 1
				else c=c<<1;   //最后一位插入 0
			}
			fwrite(&c,1,1,ofp); //每满足8位 写入
			flen2++;
			strcpy(buf,buf+8); //多余的位数接到buf后 留给下次循环处理
			j=strlen(buf);
		}
	}

	if(j>0)        //最后一段不足8位
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
	fwrite(&flen2,sizeof(int),1,ofp);  //写入目的文件字符数
	hwndlist = GetDlgItem(hwnd, IDC_LIST1);
	wsprintf(str, "字符  频度   编码");
	ListBox_InsertString(hwndlist, -1, str);
	for(i=0;i<n;i++)
	{
		wsprintf(str, "%c      %d     %s",HufNode[i].b,HufNode[i].count,HufNode[i].code);
		ListBox_InsertString(hwndlist, -1, str);
	}
	//写入哈夫曼树
	fwrite(&n,sizeof(int),1,hfp); //写入字符总数
	for(i=0;i<n;i++)
	{
		fwrite(&(HufNode[i].b),1,1,hfp);  //写入字符
		c=strlen(HufNode[i].code);
		fwrite(&c,1,1,hfp);      //写入哈弗曼编码长度
		j=c;
		if(j%8!=0)              //写入哈弗曼编码
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

	fread(&flen,sizeof(int),1,ifp);//源文件长度
	fread(&flen1,sizeof(int),1,ifp);//目的文件长度
	fread(&n,sizeof(int),1,hfp);//字符总数
    rate = double(flen1)/flen*100;
	//读取哈夫曼树
	for(i=0;i<n;i++)
	{
		fread(&HufNode[i].b,1,1,hfp);
		fread(&c,1,1,hfp);
		hlen=(int)c;
		HufNode[i].code_len=hlen;
		HufNode[i].code[0]=0;
		if(hlen%8!=0) m=hlen/8+1; //先找出该字符对应的哈弗曼编码的相关的字节里面的内容（最后一个字节可能不完全是该哈弗曼编码）
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
		HufNode[i].code[hlen]=0; //再截取该哈弗曼编码的长度（即截取有效段）
	}

	//找到最长的哈弗曼编码长度
	for(i=0,hlen=0;i<n;i++)
	{
		if(strlen(HufNode[i].code)>hlen) hlen=strlen(HufNode[i].code);
	}
	fseek(ifp,8,SEEK_SET);//找到文件开头（除掉源文件长度和目的文件长度的8个字节）
	buf2[0]=0;

	//译码
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
		for(i=0;i<n;i++)//找到对应的字符
		{
			if(memcmp(HufNode[i].code,buf2,HufNode[i].code_len)==0) break;
		}
		strcpy(buf2,buf2+HufNode[i].code_len); //删掉之前读取的字符对应的哈弗曼编码
		c=HufNode[i].b;
		fwrite(&c,1,1,ofp);  //写入字符
	}
	hwndlist = GetDlgItem(hwnd, IDC_LIST1);
	wsprintf(str, "字符   编码");
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