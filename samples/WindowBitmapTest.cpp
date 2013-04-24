#include "WindowBitmap.h"

#define _WIN32_WINNT    0x0501        //��XP������ϵͳ��Ч
#include <windows.h>
#include <iostream>

void PrintWindow(HWND hWnd)
{
	HDC hDCMem = CreateCompatibleDC(NULL);

	RECT rect;

	GetWindowRect(hWnd, & rect);

	HBITMAP hBmp = NULL;

	{
		HDC hDC = GetDC(hWnd);
		hBmp = CreateCompatibleBitmap(hDC, rect.right - rect.left, rect.bottom - rect.top);
		ReleaseDC(hWnd, hDC);
	}

	HGDIOBJ hOld = SelectObject(hDCMem, hBmp);
	SendMessage(hWnd, WM_PRINT, (WPARAM) hDCMem, PRF_CHILDREN | PRF_CLIENT | PRF_ERASEBKGND | PRF_NONCLIENT | PRF_OWNED);

	SelectObject(hDCMem, hOld);
	DeleteObject(hDCMem);

	OpenClipboard(hWnd);

	EmptyClipboard(); 
	SetClipboardData(CF_BITMAP, hBmp);
	CloseClipboard();
}
int main()
{ 
   WindowBitmap* windowBitmap = WindowBitmap::getWindowBitmap("Weiqi.exe");
#if 0
   HWND hwnd;
   //hwnd = FindWindow(TEXT("Notepad"), NULL);    //ע�ⴰ�ڲ�����С��
   hwnd = windowBitmap->_hWnd;
   //PrintWindow(hwnd);
   if (hwnd == NULL)
	{
		std::cout << "�Ҳ������±�����" << std::endl;
		return 0;
	}

	//��ȡ������Ϣ
	WINDOWINFO wi;
	wi.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &wi);

	//����ͻ�������
	RECT rc = {
//		wi.rcWindow.left - wi.rcWindow.left, 
//		wi.rcWindow.top - wi.rcWindow.top, 
//		wi.rcWindow.right - wi.rcWindow.left, 
//		wi.rcWindow.bottom - wi.rcWindow.top
		wi.rcClient.left - wi.rcWindow.left, 
		wi.rcClient.top - wi.rcWindow.top, 
		wi.rcClient.right - wi.rcWindow.left, 
		wi.rcClient.bottom - wi.rcWindow.top
	};

	//����
	HDC hdcScreen = GetWindowDC(hwnd);
	HDC hdc = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, 
		wi.rcWindow.right - wi.rcWindow.left, 
		wi.rcWindow.bottom - wi.rcWindow.top);
	SelectObject(hdc, hbmp);

	HBITMAP hbmp2 = CreateCompatibleBitmap(hdcScreen, 
		wi.rcClient.right - wi.rcClient.left, 
		wi.rcClient.bottom - wi.rcClient.top);
	HDC hdc2 = CreateCompatibleDC(hdcScreen);
	SelectObject(hdc2, hbmp2);

	//������������
	//PrintWindow(hwnd, hdc, 0);
	BitBlt(hdc, 
		0, 0, rc.right - rc.left, rc.bottom - rc.top, 
		hdcScreen, 
		rc.left, rc.top, 
		SRCCOPY);

	//���ƿͻ���
	BitBlt(hdc2, 
		0, 0, rc.right - rc.left, rc.bottom - rc.top, 
		hdc, 
		rc.left, rc.top, 
		SRCCOPY);

	//���Ƶ�ճ����
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, hbmp2);
	CloseClipboard();

	//�ͷ�
	DeleteDC(hdc);
	DeleteObject(hbmp);

	DeleteDC(hdc2);
	DeleteObject(hbmp2);

	ReleaseDC(NULL, hdcScreen);

	std::cout << "�ɹ��Ѽ��±����ڸ��Ƶ�ճ����,��ճ����Windows��ͼ����" << std::endl;
#endif
	return 0;
}
#if 0
int main()
{
   WindowBitmap* windowBitmap = WindowBitmap::getWindowBitmap("Weiqi.exe");
   return 0;
}
#endif
