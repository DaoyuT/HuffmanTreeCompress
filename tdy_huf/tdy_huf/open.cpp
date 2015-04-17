#include "stdafx.h"

BOOL  YASUO(HWND hwnd)					
{
	double rate;

	OPENFILENAME ofn;
	TCHAR szFile1[MAX_PATH];		//�ļ���
	TCHAR szFile2[MAX_PATH];
	TCHAR szFile3[MAX_PATH];
	SYSTEMTIME time1,time2;
	int hour,min,sec,msec,run_time;
	char rate_buf[10] = {0};
	char str[MAX_CHARS] = {0};
	
	ZeroMemory(&ofn,sizeof(ofn));										//���ýṹ�����ֵ
	ofn.lStructSize = sizeof(ofn);
	//ofn.lpstrFile = szFile;
	//ofn.lpstrFile[0] = TEXT('\0');
	//ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT("�ı��ļ�(*.txt)\0*.txt\0�����ļ�(*.cod)\0*.cod\0��������(*.hfm)\0*.hfm\0�����ļ�(*.*)\0*.*\0");
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = hwnd;
	ofn.Flags = OFN_EXPLORER |OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.nFilterIndex = 1;
	
	ofn.lpstrFile = szFile1;
	ofn.nMaxFile = sizeof(szFile1);
	ofn.lpstrFile[0] = TEXT('\0');
	if (!GetOpenFileName(&ofn))
	{
		return FALSE;
	}
	ofn.lpstrFile = szFile2;
	ofn.nMaxFile = sizeof(szFile2);
	ofn.lpstrFile[0] = TEXT('\0');
	if (!GetSaveFileName(&ofn))
	{
		return FALSE;
	}
	MessageBox(hwnd,TEXT("��������ָ�����������ļ���ŵ�ַ"),TEXT("��Ϣ"),MB_OK);
	ofn.lpstrFile = szFile3;
	ofn.nMaxFile = sizeof(szFile3);
	ofn.lpstrFile[0] = TEXT('\0');
	if (GetSaveFileName(&ofn))
	{
		GetLocalTime(&time1);
		rate=compress(szFile1,szFile2,szFile3,hwnd);
		GetLocalTime(&time2);
		hour=time2.wHour-time1.wHour;
		min=time2.wMinute-time1.wMinute;
		sec=time2.wSecond-time1.wSecond;
		msec=time2.wMilliseconds-time1.wMilliseconds;
		if(min<0)
		{
			hour--;
			min+=60;
		}
		if(sec<0)
		{
			min--;
			sec+=60;
		}
		if(msec<0)
		{
			sec--;
			msec+=1000;
		}
		run_time=min*60*1000+sec*1000+msec;
		gcvt(rate, 6, rate_buf);

	}
	
	read(hwnd, szFile1);
	wsprintf(str, "ѹ���ɹ���\r\nѹ����Ϊ%s%%\r\nѹ��ʱ��Ϊ:%dms",rate_buf, run_time);
	SetDlgItemText(hwnd, IDC_EDIT4, str);

	return TRUE;
}
BOOL  JIEYA(HWND hwnd)		
{
	double rate;
	
	OPENFILENAME ofn;
	TCHAR szFile1[MAX_PATH];		//�ļ���
	TCHAR szFile2[MAX_PATH];
	TCHAR szFile3[MAX_PATH];
	SYSTEMTIME time1,time2;
	int hour,min,sec,msec,run_time;
	char rate_buf[10] = {0};
	char str[MAX_CHARS] = {0};

	ZeroMemory(&ofn,sizeof(ofn));										//���ýṹ�����ֵ
	ofn.lStructSize = sizeof(ofn);
	//ofn.lpstrFile = szFile;
	//ofn.lpstrFile[0] = TEXT('\0');
	//ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT("�ı��ļ�(*.txt)\0*.txt\0�����ļ�(*.cod)\0*.cod\0��������(*.hfm)\0*.hfm\0�����ļ�(*.*)\0*.*\0");
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = hwnd;
	ofn.Flags = OFN_EXPLORER |OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.nFilterIndex = 1;

	ofn.lpstrFile = szFile1;
	ofn.nMaxFile = sizeof(szFile1);
	ofn.lpstrFile[0] = TEXT('\0');
	if (!GetOpenFileName(&ofn))
	{
		return FALSE;
	}
	ofn.lpstrFile = szFile2;
	ofn.nMaxFile = sizeof(szFile2);
	ofn.lpstrFile[0] = TEXT('\0');
	if (!GetSaveFileName(&ofn))
	{
		return FALSE;
	}
	MessageBox(hwnd,TEXT("��������ָ�����������ļ���ŵ�ַ"),TEXT("��Ϣ"),MB_OK);
	ofn.lpstrFile = szFile3;
	ofn.nMaxFile = sizeof(szFile3);
	ofn.lpstrFile[0] = TEXT('\0');
	if (GetOpenFileName(&ofn))
	{
		GetLocalTime(&time1);
		rate=uncompress(szFile1,szFile2,szFile3,hwnd);
		GetLocalTime(&time2);
		hour=time2.wHour-time1.wHour;
		min=time2.wMinute-time1.wMinute;
		sec=time2.wSecond-time1.wSecond;
		msec=time2.wMilliseconds-time1.wMilliseconds;
		if(min<0)
		{
			hour--;
			min+=60;
		}
		if(sec<0)
		{
			min--;
			sec+=60;
		}
		if(msec<0)
		{
			sec--;
			msec+=1000;
		}
		run_time=min*60*1000+sec*1000+msec;
		gcvt(rate, 6, rate_buf);
	}

	read(hwnd, szFile2);
	wsprintf(str, "��ѹ�ɹ���\r\nѹ����Ϊ%s%%\r\n��ѹʱ��Ϊ:%dms", rate_buf,run_time);
	SetDlgItemText(hwnd, IDC_EDIT4, str);
	
	return TRUE;
}