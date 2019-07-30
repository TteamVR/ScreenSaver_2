#include <windows.h>
#include <stdio.h>

#include "ScrSaver_2.h"

#define TIMERNUMBER 103
#define TIMERFREQ    60


namespace Lego
{
	
	HFONT 			  ScreenSaverhFont;
	int 		  ScreenSaverTextColor;
	
	extern  int              MouseMove;                          	                                                    
	extern  HWND                  hWnd;    
    extern  RECT             rc_Global;	
}

using namespace Lego;

namespace 
{
	LRESULT CALLBACK Timer_103(HWND hWnd, UINT message, UINT_PTR idEvent, DWORD dwTime);
	
	class SCREENSAVER_CODE
	{
	private:
		
		int     	     x;
		int     	     y;
		HDC     	   hdc;
		HBRUSH       brush;
		int	       BkColor;
		char   	    Symbol;
		TEXTMETRIC      tm;
		FILE    	 *file;
	
	public: int LocalMouseMove;
	
	private:
		
		void Logic      ();
		void Drawing    ();
	
	public:
		
		SCREENSAVER_CODE(const char* path);
		~SCREENSAVER_CODE();
	    
		void Restart     ();
		void InitSizeText();
		void Update 	 ();
	
	};	

	SCREENSAVER_CODE::SCREENSAVER_CODE(const char* path)
	{
		file = fopen(path, "r");
		
		ScreenSaverhFont = CreateFont(30, 10, 0, 0, 0, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 
  	 	OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    	DEFAULT_PITCH | FF_DONTCARE, "Microsoft Sans Serif");
			
		ScreenSaverTextColor = RGB(0, 255, 0);
	}
	
	SCREENSAVER_CODE::~SCREENSAVER_CODE()
	{
		fclose(file);	
	}
	
	void SCREENSAVER_CODE::Restart()
	{
		InvalidateRect(hWnd, &rc_Global, 1);
		fseek(file, SEEK_SET, 0);
		x = 0; 
		y = 0;		
	}
	
	void SCREENSAVER_CODE::InitSizeText()
	{
		hdc = GetDC(hWnd);
		
		SelectObject(hdc, ScreenSaverhFont);
		GetTextMetrics(hdc, &tm);
	 	
		ReleaseDC(hWnd, hdc);
	}
	
	void SCREENSAVER_CODE::Logic()
	{	
		if(Symbol == '\n') 
		{
			y += tm.tmHeight; 
			x = 0;	
		}
		else x += tm.tmAveCharWidth * 2;
		
		if((x + (tm.tmAveCharWidth * 2)) >  rc_Global.right)     { y += tm.tmHeight;                    x = 0; }		
		if((y + (tm.tmHeight * 2)) 			 > rc_Global.bottom) { InvalidateRect(hWnd, &rc_Global, 1); y = 0; }
	}

	
	void SCREENSAVER_CODE::Drawing()
	{
		hdc = GetDC(hWnd);
		
		SetBkColor  (hdc,   BkColor);
		SetTextColor(hdc, ScreenSaverTextColor);
		
		SelectObject(hdc, ScreenSaverhFont);
		TextOut(hdc, x, y, (char*)&Symbol, 1);
		
		ReleaseDC(hWnd, hdc);
	}
	
	void SCREENSAVER_CODE::Update()
	{
		if(LocalMouseMove == MouseMove)
		{
			if(!feof(file))
			{
				Symbol = fgetc(file);
				Logic  ();
				Drawing();
			}
			else Restart();
		}
		else
		{
			InvalidateRect(hWnd, &rc_Global, 1);
			KillTimer(hWnd, TIMERNUMBER);	
		}
	}
}

namespace Lego
{			
	SCREENSAVER_CODE ScreenSaver_Code("C:\\Users\\admin\\Desktop\\UserProject_1\\ScrSaver_2\\ScrSaver_2.cpp");
									  
	void StartScreenSaver_2()
	{	
		ScreenSaver_Code.InitSizeText();
		ScreenSaver_Code.LocalMouseMove = MouseMove;
		ScreenSaver_Code.Restart();
		
		SetTimer(hWnd, TIMERNUMBER, TIMERFREQ, (TIMERPROC)Timer_103);
	}	
}



namespace 
{
	LRESULT CALLBACK Timer_103(HWND hWnd, UINT message, UINT_PTR idEvent, DWORD dwTime)
	{
		ScreenSaver_Code.Update();	
	}
}
