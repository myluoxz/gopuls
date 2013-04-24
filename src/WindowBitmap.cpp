#include <windows.h>
#include <Tlhelp32.h>

#include "WindowBitmap.h"

WindowBitmap::WindowBitmap(HWND hWnd, bool directx):
   _hWnd(hWnd),
   m_bits(NULL)
{
   HDC hdcWindow = GetWindowDC(hWnd);
   HDC hdcBuffer = CreateCompatibleDC(hdcWindow);

   RECT windowRect;
   int windowWidth, windowHeight;

   GetWindowRect(hWnd, &windowRect);
   windowWidth = windowRect.right - windowRect.left;
   windowHeight = windowRect.bottom - windowRect.top;

   _hBitmap = CreateCompatibleBitmap(hdcWindow, windowWidth, windowHeight);
   HBITMAP hBitTemp = (HBITMAP) SelectObject(hdcBuffer, _hBitmap);

   if(directx){
      BitBlt(hdcBuffer, 0, 0, windowWidth, windowHeight, hdcWindow, 0, 0, SRCCOPY);
   }else{
      PrintWindow(hWnd, hdcBuffer, 0);  
   }

   _hBitmap = (HBITMAP) SelectObject(hdcBuffer, hBitTemp);
   //WindowBitmap::saveHBitmap2File(_hBitmap, "my.bmp");

   GetObject( _hBitmap, sizeof(BITMAP), &m_bmp);
   
   LONG bytes = m_bmp.bmWidthBytes * m_bmp.bmHeight;
   m_bits = new BYTE[bytes];

   GetBitmapBits( _hBitmap, bytes, m_bits );

   //WindowBitmap::saveHBitmap2File(_hBitmap, "my.bmp");

   DeleteObject(hBitTemp);
   DeleteDC(hdcBuffer);
   ReleaseDC(hWnd, hdcWindow);
}

WindowBitmap::~WindowBitmap()
{
   DeleteObject(_hBitmap);
   delete [] m_bits;
}

void WindowBitmap::click(int x, int y)
{
   SendMessage(_hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG(x,y)); 
   SendMessage(_hWnd, WM_LBUTTONUP, MK_LBUTTON, MAKELONG(x,y)); 
}

void WindowBitmap::clickScreenRelateToWindow(int x, int y)
{
   RECT windowRect;
   GetWindowRect(_hWnd, &windowRect);

   POINT lpPoint;
   GetCursorPos(&lpPoint);

   SetCursorPos(x + windowRect.left, y + windowRect.top);
   mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

   SetCursorPos(lpPoint.x, lpPoint.y);
}

void WindowBitmap::clickScreen(HWND hWnd, int x, int y)
{
   RECT windowRect;
   GetWindowRect(hWnd, &windowRect);

   POINT lpPoint;
   GetCursorPos(&lpPoint);

   SetCursorPos(x + windowRect.left, y + windowRect.top);
   mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

   SetCursorPos(lpPoint.x, lpPoint.y);
}

void WindowBitmap::clickScreen(int x, int y)
{
   POINT lpPoint;
   GetCursorPos(&lpPoint);

   SetCursorPos(x, y);
   mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

   SetCursorPos(lpPoint.x, lpPoint.y);
}

void WindowBitmap::drawRectangle(int left, int top, int right, int bottom)
{
   // ��ȡһ���ɹ���ͼ��DC���������ֱ������������
   HDC hdc = GetWindowDC( _hWnd );

   //HDC hdc = CreateDC("DISPLAY", NULL, NULL, NULL);
   // ����ʵ�߻���
   HPEN hpen1 = CreatePen( PS_SOLID, 1, RGB(255,0,0) );
   // ����һ��ʵ�廭ˢ
   HBRUSH hbrush1 = CreateSolidBrush( RGB(255,0,0) );

   // ��hpen1��hbrush1ѡ��HDC��������HDCԭ���Ļ��ʺͻ�ˢ
   HPEN hpen_old = (HPEN)SelectObject( hdc, hpen1 );
   HBRUSH hbrush_old = (HBRUSH)SelectObject( hdc, hbrush1 );

   // ��һ������
   Rectangle( hdc, left, top, right, bottom );

   // �ָ�ԭ���Ļ��ʺͻ�ˢ
   SelectObject( hdc, hpen_old );
   SelectObject( hdc, hbrush_old );
   ReleaseDC(_hWnd, hdc);
}

//void GoBoard::draw_debug()
//{
//   // ��ȡһ���ɹ���ͼ��DC���������ֱ������������
//   HDC hdc = GetWindowDC( GetDesktopWindow() );
//
//   //HDC hdc = CreateDC("DISPLAY", NULL, NULL, NULL);
//   // ����ʵ�߻���
//   HPEN hpen1 = CreatePen( PS_SOLID, 1, RGB(255,0,0) );
//   // ����һ��ʵ�廭ˢ
//   HBRUSH hbrush1 = CreateSolidBrush( RGB(255,0,0) );
//
//   // ��hpen1��hbrush1ѡ��HDC��������HDCԭ���Ļ��ʺͻ�ˢ
//   HPEN hpen_old = (HPEN)SelectObject( hdc, hpen1 );
//   HBRUSH hbrush_old = (HBRUSH)SelectObject( hdc, hbrush1 );
//
//   int x, y;
//   for(y=0; y<_goGameFrame.boardsize();y++)
//   {
//      for(x=0;x<_goGameFrame.boardsize();x++){
//         int i,j;
//         int kind = _goGameFrame.getPieceKind(x,y);
//         if( GoGameRule::isKindEmpty(kind) ) continue;
//         i = _leftTopX + x * _gridLen;
//         j = _leftTopY + y * _gridLen;
//         // ��һ������
//         Rectangle( hdc, i-_gridLen/2, j-_gridLen/2, i+_gridLen/2, j+_gridLen/2 );
//
//
////         int kind = _goGameFrame.getPieceKind(j,i);
////         if( GoGameRule::isKindEmpty(kind) ) continue;
////
////         std::string str = GoGameRule::strOfCoordinate(_goGameFrame.boardsize(), j, i);
////         _ostr << "play" << " "
////    << GoGameRule::strOfKind(kind) << " " 
////               << str << std::endl;
////         // TODO: Error Handler
////
////         readDumpResponse();
//      }
//   }
//
//   // �ָ�ԭ���Ļ��ʺͻ�ˢ
//   SelectObject( hdc, hpen_old );
//   SelectObject( hdc, hbrush_old );
//   ReleaseDC(hdc);
//}

int WindowBitmap::width()
{
   return m_bmp.bmWidth;
}

int WindowBitmap::height()
{
   return m_bmp.bmHeight;
}

BYTE WindowBitmap::getR(int x, int y)
{
   return *(m_bits + m_bmp.bmWidthBytes * y + 4 * x + 2);
}

BYTE WindowBitmap::getG(int x, int y)
{
   return *(m_bits + m_bmp.bmWidthBytes * y + 4 * x + 1);
}

BYTE WindowBitmap::getB(int x, int y)
{
   return *(m_bits + m_bmp.bmWidthBytes * y + 4 * x);
}

BOOL CALLBACK matchProcessWindowEnumProc(HWND hWnd, LPARAM lParam) 
{ 
   DWORD ProcessID;
   DWORD ThreadID;
   HWndProcess *hWndProcess = (HWndProcess *)lParam;
   
   // ���ɼ������ɼ���Ĵ��ڲ������ǡ�
   if ( !IsWindowVisible(hWnd) ){
      return true;
   }

   if ( !IsWindowEnabled(hWnd) ){
      return true;
   }

   // ����ʽ���ڣ���û�б���Ĳ������ǡ�
   LONG gwl_style = GetWindowLong( hWnd, GWL_STYLE );
   if ((gwl_style & WS_POPUP) && !(gwl_style & WS_CAPTION)){
      return true;
   }

   // �������ǿɼ���ɼ���Ĵ��ڲ������ǡ�
   HWND hParent = (HWND)GetWindowLong( hWnd, GWL_HWNDPARENT );
   if ( IsWindowEnabled(hParent) ){
      return true;
   }
   if ( IsWindowVisible(hParent) ){
      return true;
   }

//
//   // Shell_TrayWnd��Ĵ��ڣ���������ģ��������ǡ�
//   TCHAR szClassName[MAX_PATH];
//   GetClassName( hWnd, szClassName, MAX_PATH );
//   if ( !wcscmp(szClassName, _T("Shell_TrayWnd")) ){
//      return true;
//   }

   ThreadID = GetWindowThreadProcessId(hWnd,&ProcessID);
   if( ProcessID == hWndProcess->processID ){
	  hWndProcess->hWnd = hWnd;
	  RECT rect;
	  GetWindowRect(hWnd, &rect);
      return false;
   }
   return true;
}

BOOL CALLBACK matchProcessMessageBoxWindowEnumProc(HWND hWnd, LPARAM lParam) 
{ 
   DWORD ProcessID;
   DWORD ThreadID;
   HWndProcess *hWndProcess = (HWndProcess *)lParam;
   
   // ���ɼ������ɼ���Ĵ��ڲ������ǡ�
   if ( !IsWindowVisible(hWnd) ){
      return true;
   }

   if ( !IsWindowEnabled(hWnd) ){
      return true;
   }

   // ����ʽ���ڣ���û�б���Ĳ������ǡ�
   LONG gwl_style = GetWindowLong( hWnd, GWL_STYLE );
   if ((gwl_style & WS_POPUP) && !(gwl_style & WS_CAPTION)){
      return true;
   }
   
   // message box can not MINIMIZE and MAXIMIZE
   if ((gwl_style & WS_MINIMIZEBOX) || (gwl_style & WS_MAXIMIZEBOX)){
      return true;
   }

   // �������ǿɼ���ɼ���Ĵ��ڲ������ǡ�
   HWND hParent = (HWND)GetWindowLong( hWnd, GWL_HWNDPARENT );
   if ( IsWindowEnabled(hParent) ){
      return true;
   }
   //if ( IsWindowVisible(hParent) ){
   //   return true;
   //}

//
//   // Shell_TrayWnd��Ĵ��ڣ���������ģ��������ǡ�
//   TCHAR szClassName[MAX_PATH];
//   GetClassName( hWnd, szClassName, MAX_PATH );
//   if ( !wcscmp(szClassName, _T("Shell_TrayWnd")) ){
//      return true;
//   }

   ThreadID = GetWindowThreadProcessId(hWnd,&ProcessID);
   if( ProcessID == hWndProcess->processID ){
	  hWndProcess->hWnd = hWnd;
	  RECT rect;
	  GetWindowRect(hWnd, &rect);
      return false;
   }
   return true;
}

DWORD getProcessIDFromName(const std::string& processName)   
{         
   HANDLE hProcessSnap = NULL;
   PROCESSENTRY32 pe32 = {0};
   DWORD processID = -1;
   
   hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   if( hProcessSnap==(HANDLE)-1 ){
      return -1;
   }
   
   pe32.dwSize=sizeof(PROCESSENTRY32);
   if(Process32First(hProcessSnap,&pe32)){
      do{ 
         if( processName == pe32.szExeFile ){ 
            processID = pe32.th32ProcessID;
            goto end;
         }
      }
      while(Process32Next(hProcessSnap,&pe32)); 
   }

  end:
   CloseHandle(hProcessSnap); 
   return processID;
}

std::list<WindowBitmap> WindowBitmap::getWindowBitmapList(const std::string& process, bool directx)
{
   DWORD processID;
   std::list<WindowBitmap> windowBitmapList;
   HWndProcess hWndProcess;

   if( (processID = getProcessIDFromName(process)) == -1 ){
      goto end;
   }

   hWndProcess.processID = processID;
   if( false == EnumWindows(matchProcessWindowEnumProc, (LPARAM)&hWndProcess) ){
      //SendMessage(hWndProcess.hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG(100,100)); 
      //SendMessage(hWndProcess.hWnd, WM_LBUTTONUP, MK_LBUTTON, MAKELONG(100,100)); 
      windowBitmapList.push_back(WindowBitmap(hWndProcess.hWnd, directx));
   }
  end:
   return windowBitmapList;
}

WindowBitmap* WindowBitmap::getWindowBitmap(const std::string& process, bool directx)
{
   DWORD processID = -1;
   HWndProcess hWndProcess;
   WindowBitmap* windowBitmap = NULL;

   if( (processID = getProcessIDFromName(process)) == -1 ){
      return NULL;
   }

   hWndProcess.processID = processID;
   if( false == EnumWindows(matchProcessWindowEnumProc, (LPARAM)&hWndProcess) ){
      return new WindowBitmap(hWndProcess.hWnd, directx);
   }

   return NULL;
}

bool WindowBitmap::rgbEqual(int x1, int y1, int x2, int y2)
{
   int r1 = getR(x1, y1);
   int g1 = getG(x1, y1);
   int b1 = getB(x1, y1);

   int r2 = getR(x2, y2);
   int g2 = getG(x2, y2);
   int b2 = getB(x2, y2);

   return (r1 == r2) && (g1 == g2) && (b1 == b2);
}

bool WindowBitmap::rgbEqual(int x1, int y1, BYTE r, BYTE g, BYTE b)
{
   int r1 = getR(x1, y1);
   int g1 = getG(x1, y1);
   int b1 = getB(x1, y1);

   return (r1 == r) && (g1 == g) && (b1 == b);
}

bool WindowBitmap::rgbNear(int x1, int y1, BYTE r, BYTE g, BYTE b)
{
   int r1 = getR(x1, y1);
   int g1 = getG(x1, y1);
   int b1 = getB(x1, y1);

   return (abs(r1-r)<=10) && (abs(g1-g)<=10) && (abs(b1-b)<=10);
}


bool WindowBitmap::rgbLess(int x1, int y1, BYTE r, BYTE g, BYTE b)
{
   int r1 = getR(x1, y1);
   int g1 = getG(x1, y1);
   int b1 = getB(x1, y1);

   return (r1 < r) && (g1 < g) && (b1 < b);
}

bool WindowBitmap::rgbLarger(int x1, int y1, BYTE r, BYTE g, BYTE b)
{
   int r1 = getR(x1, y1);
   int g1 = getG(x1, y1);
   int b1 = getB(x1, y1);

   return (r1 > r) && (g1 > g) && (b1 > b);
}


bool WindowBitmap::saveHBitmap2File(HBITMAP hBitmap, const std::string& filepath)         
{
	HDC     hDC;

	//��ǰ�ֱ�����ÿ������ռ�ֽ���
	int     iBits;

	//λͼ��ÿ������ռ�ֽ���
	WORD     wBitCount;

	//�����ɫ���С��     λͼ�������ֽڴ�С     ��λͼ�ļ���С     ��     д���ļ��ֽ���  
	DWORD     dwPaletteSize=0,   dwBmBitsSize=0,   dwDIBSize=0,   dwWritten=0;  

	//λͼ���Խṹ  
	BITMAP     Bitmap;      

	//λͼ�ļ�ͷ�ṹ
	BITMAPFILEHEADER     bmfHdr;      

	//λͼ��Ϣͷ�ṹ  
	BITMAPINFOHEADER     bi;      

	//ָ��λͼ��Ϣͷ�ṹ      
	LPBITMAPINFOHEADER     lpbi;      

	//�����ļ��������ڴ�������ɫ����  
	HANDLE     fh,   hDib,   hPal,hOldPal=NULL;  

	//����λͼ�ļ�ÿ��������ռ�ֽ���  
	hDC  = CreateDC("DISPLAY",   NULL,   NULL,   NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);

	if(iBits <=  1){
		wBitCount = 1;
    }
	else  if(iBits <=  4){
		wBitCount  = 4;  
    }
	else if(iBits <=  8){
       wBitCount  = 8;
    }
	else{
       wBitCount  = 24;
    }

	GetObject(hBitmap,   sizeof(Bitmap),   (LPSTR)&Bitmap);
	bi.biSize= sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight =  Bitmap.bmHeight;
	bi.biPlanes =  1;
	bi.biBitCount = wBitCount;
	bi.biCompression= BI_RGB;
	bi.biSizeImage=0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed =  0;

	dwBmBitsSize  = ((Bitmap.bmWidth *wBitCount+31) / 32)*4* Bitmap.bmHeight;

	//Ϊλͼ���ݷ����ڴ�  
	hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));  
	lpbi  = (LPBITMAPINFOHEADER)GlobalLock(hDib);  
	*lpbi  = bi;  

	//     �����ɫ��      
	hPal  = GetStockObject(DEFAULT_PALETTE);  
	if (hPal)  
	{  
		hDC  = ::GetDC(NULL);  
		hOldPal = ::SelectPalette(hDC,(HPALETTE)hPal, FALSE);  
		RealizePalette(hDC);  
	}

	//     ��ȡ�õ�ɫ�����µ�����ֵ  
	GetDIBits(hDC,hBitmap, 0,(UINT)Bitmap.bmHeight,  
		(LPSTR)lpbi+ sizeof(BITMAPINFOHEADER)+dwPaletteSize,   
		(BITMAPINFO *)lpbi, DIB_RGB_COLORS);  

	//�ָ���ɫ��      
	if (hOldPal)  
	{  
		::SelectPalette(hDC,   (HPALETTE)hOldPal,   TRUE);  
		RealizePalette(hDC);  
		::ReleaseDC(NULL,   hDC);  
	}  

	//����λͼ�ļ�      
	fh  = CreateFile(filepath.c_str(),   GENERIC_WRITE,0,   NULL,   CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,   NULL);  

	if (fh == INVALID_HANDLE_VALUE) return true;  

	//     ����λͼ�ļ�ͷ  
	bmfHdr.bfType  = 0x4D42;     //     "BM"  
	dwDIBSize  = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize;      
	bmfHdr.bfSize  = dwDIBSize;  
	bmfHdr.bfReserved1  = 0;  
	bmfHdr.bfReserved2  = 0;  
	bmfHdr.bfOffBits  = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER)+dwPaletteSize;  
	//     д��λͼ�ļ�ͷ  
	WriteFile(fh,   (LPSTR)&bmfHdr,   sizeof(BITMAPFILEHEADER),   &dwWritten,   NULL);  
	//     д��λͼ�ļ���������  
	WriteFile(fh,   (LPSTR)lpbi,   dwDIBSize,   &dwWritten,   NULL);  
	//���      
	GlobalUnlock(hDib);  
	GlobalFree(hDib);  
	CloseHandle(fh);  

	return TRUE;
}
