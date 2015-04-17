// Win32DialogBasedApplication.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "MainDlg.h"
#include "Win32DialogBasedApplication.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	InitCommonControls();
 	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, Main_Proc);
	return 0;
}