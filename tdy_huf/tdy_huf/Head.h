#pragma  once
#define MAX_CHARS 256  //ascii��ֵ�ķ�Χ��255
#define MAX_NODE 512 //����256��Ҷ�ӽڵ����ȫ����������511���ڵ�
#define MAXIMUM 999999

typedef struct Node
{
	unsigned char b;//�ַ�
	int count; //�ַ����ֵĴ������ڵ�Ȩ�أ�
	int parent,lch,rch; //�ڵ��˫�׽ڵ� ���� �Һ���
	char code[20]; //�ڵ�����Ӧ�Ĺ���������
	int code_len; //���������볤��
}Node ;/*HufNode[MAX_NODE],tmp;*/


double compress(TCHAR* ,TCHAR* ,TCHAR*,HWND);
double uncompress(TCHAR* ,TCHAR* ,TCHAR*,HWND);
int read (HWND ,TCHAR* );
BOOL  YASUO(HWND);
BOOL  JIEYA(HWND);
BOOL CLEARSCREEN(HWND , int );