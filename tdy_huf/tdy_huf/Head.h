#pragma  once
#define MAX_CHARS 256  //ascii码值的范围是255
#define MAX_NODE 512 //至多256个叶子节点的完全二叉树至多511个节点
#define MAXIMUM 999999

typedef struct Node
{
	unsigned char b;//字符
	int count; //字符出现的次数（节点权重）
	int parent,lch,rch; //节点的双亲节点 左孩子 右孩子
	char code[20]; //节点数对应的哈夫曼编码
	int code_len; //哈弗曼编码长度
}Node ;/*HufNode[MAX_NODE],tmp;*/


double compress(TCHAR* ,TCHAR* ,TCHAR*,HWND);
double uncompress(TCHAR* ,TCHAR* ,TCHAR*,HWND);
int read (HWND ,TCHAR* );
BOOL  YASUO(HWND);
BOOL  JIEYA(HWND);
BOOL CLEARSCREEN(HWND , int );