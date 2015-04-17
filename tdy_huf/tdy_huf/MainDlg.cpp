#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "Resource.h"
#include "MainDlg.h"


BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        HANDLE_MSG(hWnd, WM_INITDIALOG, Main_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Main_OnCommand);
		HANDLE_MSG(hWnd,WM_CLOSE, Main_OnClose);
    }

    return FALSE;
}

BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    return TRUE;
}

void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch(id)
    {
        case IDC_COMPRESS:
		{
			CLEARSCREEN(hwnd , IDC_LIST1 );
			YASUO( hwnd);
		}
        break;
		case IDC_UNCOMPRESS:
			{
				CLEARSCREEN(hwnd , IDC_LIST1 );
				JIEYA( hwnd);
			}
		case IDC_HP:
			{
				MessageBox(hwnd,TEXT("1.ѹ���ͽ�ѹʱ����˳��ѡ��Դ�ļ���Ŀ���ļ��͹��������ļ�\n2.�������߶�win32����Ĳ���ͨ���ļ�������ܵ������ı�������ʾʱ�������Ҫʵ�ִ��ļ���ѹ����ѹ���ܣ���ʹ��dos�����ĳ���\n"),TEXT("����"),MB_OK);
				break;
			}
		case IDC_EXIT:
			{
				EndDialog(hwnd, 0);
				break;
			}
		default:
			break;
		
    }
}

void Main_OnClose(HWND hwnd)
{
    EndDialog(hwnd, 0);
}