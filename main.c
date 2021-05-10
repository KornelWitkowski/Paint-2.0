#include <windows.h>
#include <stdio.h>
#include <commctrl.h>
#include <stdbool.h>
#include <math.h>

#include "define.h"
#include "prototypes.h"

//#include <stdlib.h>



// Kolory
HBITMAP hRedImage, hYellowImage, hGreenImage, hBlueImage, hPurpleImage, hOrangeImage,
hBrownImage, hBlackImage, hWhiteImage, hGreyImage, hLightGreyImage, hPinkImage, hPaletteImage;

HWND hRed, hYellow, hGreen, hBlue, hPurple, hOrange,
hBrown, hBlack, hWhite, hGrey, hLightGrey, hPink, hPalette;

bool palettecall=false;

CHOOSECOLOR ColorsDialog;
COLORREF ColorTable[ 16 ];
//

//Kszta³ty pêdzla
HBITMAP hCircleImage, hSquareImage, hDiamondImage, hHexagonImage, hLineImage;
HWND hCircle, hSquare, hDiamond, hHexagon, hLine;
bool Circle=false, Square=false, Diamond=false, Hexagon=false, Line=false;
//

//Wype³nienie, kontur, narzêdzia
bool Fill=true, Outline=true;
HWND hGetColor, hFill, hOutline, hUndo, hPour;
HBITMAP hGetColorImage, hOutlineImage, hFillImage, hUndoImage, hPourImage;
bool gettingcolor=false;
bool pouring=false;
//


//Przybory do rysowania i tablice kolorów w RGB
int SelectedColor=0;
int Red [2] ={0, 255}, Green [2] = {0,255}, Blue [2] = {0,255};
HPEN Pen, Color1Pen, Color2Pen;
HBRUSH Brush, Color1Brush, Color2Brush;
HWND hSize, hThickness; 
int Size=10;
int Thickness=2;

//Zmienne do rysowania po klikniêciu
bool rightclicked;
bool leftclicked;
bool linestarted;
POINT LastCoursorPosition; // Pozycja ostatniego klikniecia do rysowania linii
//

float const Pi=3.1415;
bool createcall=false;

//Wymiary p³ótna
int xcanvas=1500, ycanvas=550;
int xcanvasold=1500, ycanvasold=550;
//

//Okna dialogowe i menu
HWND DialogHelp, DialogResize;
HMENU hMenu;
//

//Do otwierania pliku i rysowania
HWND hOpen;
HBITMAP hOpenImage;
//

//Zmiana rozmiaru p³ótna
HWND hWidth, hWidthValue, hHeight, hHeightValue;
//

//Help
HBITMAP hOwlImage;
HWND hOwl;
//

int xcolor=-5, ycolor=-15; //Zmienne do przesuwania pozycji kontrolek. Nazwa historyczna.

HBITMAP hScreen[100]; //Do zapamiêtania poprzednich rysunków

HWND hwnd; 			//Uchwyt do g³ównego okna
WNDCLASSW wc = {0};

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow)
{
	
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc  = WndProc;

	if(!RegisterClassW(&wc))
		return -1;

	hwnd=CreateWindowW(L"myWindowClass",L"Paint 2.0", WS_OVERLAPPEDWINDOW |WS_CLIPCHILDREN| WS_VISIBLE, 100, 100, 1600, 1000, NULL, NULL, NULL, NULL);
	SendMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);

    CreateStatic(hwnd); 
    CreateCheckBoxes(hwnd);
	SetCanvas(hwnd);
    DrawSelectedColors(hwnd);
    SaveCanvasChanges(hwnd);
    

	int i=0;
	
	while(i<100)  //Zerujemy pamiêc o poprzednich obrazach
	{
		hScreen[i]=NULL;
		i++;
	}
	
   
	MSG msg = {0};

	while(GetMessage(&msg, NULL, 0, 0) > 0) 
	{ 
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	
	switch( Message) {
		
		case WM_COMMAND:
			
			switch(wParam)

			{		
				case TOOL_UNDO:
 				{
					DrawCanvas(hwnd);
					int i=0;
					
					while(i<99)
					{
						hScreen[i]=hScreen[i+1];
						i++;
					}
                    break;
                    
           		}
           		case WM_HSCROLL:
				{		
			       break;
				}
				case TOOL_GETCOLOR:
				{
					if(!gettingcolor)
					{
						hGetColorImage = (HBITMAP) LoadImageW(NULL, L"images\\tools\\getcolor_active.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
						DestroyWindow(hGetColor);
						AddTools(hwnd);
						wc.hCursor		 = LoadCursor(NULL, IDC_CROSS); // to zrobienia #####
						gettingcolor=true;
					}
					else
					{
						gettingcolor=false;
						hGetColorImage = (HBITMAP) LoadImageW(NULL, L"images\\tools\\getcolor.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
						DestroyWindow(hGetColor);
						AddTools(hwnd);
					}
					break;
				}
				case TOOL_POUR:
				{
					if(!pouring)
					{
						hPourImage = (HBITMAP) LoadImageW(NULL, L"images\\tools\\Pour_active.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
						DestroyWindow(hPour);
						AddTools(hwnd);
						pouring=true;
					}
					else
					{
						pouring=false;
						hPourImage = (HBITMAP) LoadImageW(NULL, L"images\\tools\\Pour.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
						DestroyWindow(hPour);
						AddTools(hwnd);
					}
					break;
				}
                case SHAPE_CIRCLE: 
                {
                    loadImages();
                    Circle=true;
                    hCircleImage = (HBITMAP) LoadImageW(NULL, L"images\\shapes\\Circle_Active.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
                    DestroyShapes();
                    AddSpecificShapes(hwnd);
                    Circle=true, Square=false, Diamond=false, Hexagon=false, Line=false;
                    break;
            	}
                case SHAPE_SQUARE:
                {
                    loadImages();
                    Square=true;
                    hSquareImage = (HBITMAP) LoadImageW(NULL, L"images\\shapes\\Square_Active.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
                    DestroyShapes();
                    AddSpecificShapes(hwnd);
                    Circle=false, Square=true, Diamond=false, Hexagon=false, Line=false;
                    break;
				}
                case SHAPE_DIAMOND:
                {
                	loadImages();
                	Diamond=true;
                    hDiamondImage = (HBITMAP) LoadImageW(NULL, L"images\\shapes\\Diamond_Active.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
                    DestroyShapes();
                    AddSpecificShapes(hwnd);
                    Circle=false, Square=false, Diamond=true, Hexagon=false,Line=false;
                    break;
            	}
                case SHAPE_HEXAGON:
                {
                    loadImages();
                    Hexagon=true;
                    hHexagonImage = (HBITMAP) LoadImageW(NULL, L"images\\shapes\\Hexagon_Active.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
                    DestroyShapes();
                    AddSpecificShapes(hwnd);
                    Circle=false, Square=false, Diamond=false, Hexagon=true,Line=false;
                    break;
          		}
                case SHAPE_LINE:
                {
                    loadImages();
                    Line=true;
                    hLineImage = (HBITMAP) LoadImageW(NULL, L"images\\shapes\\Line_Active.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
                    DestroyShapes();
                    AddSpecificShapes(hwnd);
                    Circle=false, Square=false, Diamond=false, Hexagon=false, Line=true;
                    break;
            	}
			    case COLOR_1:
			    {
                    SelectedColor=0;
                    CheckDlgButton( hwnd, COLOR_1, BST_CHECKED );
                    CheckDlgButton( hwnd, COLOR_2, BST_UNCHECKED );
                    break;
            	}
			    case COLOR_2:
			    {
                    SelectedColor=1;
                    CheckDlgButton( hwnd, COLOR_2, BST_CHECKED );
                    CheckDlgButton( hwnd, COLOR_1, BST_UNCHECKED );
                    break;
            	}
                case FILL:
                {
                	if(!Fill)
                    	CheckDlgButton( hwnd, FILL, BST_CHECKED );
                    else
                    	CheckDlgButton( hwnd, FILL, BST_UNCHECKED );
                    Fill=!Fill;
                    break;
            	}
                case OUTLINE:
                {
                	if(!Outline)
                    	CheckDlgButton( hwnd, OUTLINE, BST_CHECKED );
                    else
                    	CheckDlgButton( hwnd, OUTLINE, BST_UNCHECKED );
                    Outline=!Outline;
                    break;
            	}
                case COLOR_RED:
                {
                    Red[SelectedColor] = 255;
                    Green[SelectedColor] = 0;
                    Blue[SelectedColor] = 0;
                    break;
				}
                case COLOR_YELLOW:
                {
                    Red[SelectedColor] = 255;
                    Green[SelectedColor] = 255;
                    Blue[SelectedColor] = 0;
                    break;
            	}
                case COLOR_GREEN:
                {
                    Red[SelectedColor] = 34;
                    Green[SelectedColor] = 177;
                    Blue[SelectedColor] = 76;
                    break;
            	}
                case COLOR_BLUE:
                {
                    Red[SelectedColor] = 0;
                    Green[SelectedColor] = 0;
                    Blue[SelectedColor] = 255;
                    break;
        	    }
                case COLOR_PURPLE:
                {
                    Red[SelectedColor] = 128;
                    Green[SelectedColor] = 0;
                    Blue[SelectedColor] = 128;
                    break;
          		}
                case COLOR_ORANGE:
                {
                    Red[SelectedColor] = 255;
                    Green[SelectedColor] = 128;
                    Blue[SelectedColor] = 0;
                    break;
				}
                case COLOR_BROWN:
                {
                    Red[SelectedColor] = 128;
                    Green[SelectedColor] = 64;
                    Blue[SelectedColor] = 0;
                    break;
				}
                case COLOR_BLACK:
                {
                    Red[SelectedColor] = 0;
                    Green[SelectedColor] = 0;
                    Blue[SelectedColor] = 0;
                    break;
            	}
                case COLOR_WHITE:
                {
                    Red[SelectedColor] = 255;
                    Green[SelectedColor] = 255;
                    Blue[SelectedColor] = 255;
                    break;
          		}
                case COLOR_GREY:
                {	
                    Red[SelectedColor] = 128;
                    Green[SelectedColor] = 128;
                    Blue[SelectedColor] = 128;
                    break;
           		}
                case COLOR_LIGHTGREY:
                {
                    Red[SelectedColor] = 192;
                    Green[SelectedColor] = 192;
                    Blue[SelectedColor] = 192;
                    break;
           		}
                case COLOR_PINK:
                {
                    Red[SelectedColor] = 255;
                    Green[SelectedColor] = 128;
                    Blue[SelectedColor] = 192;
          		}
                    break;
                case COLOR_PALETTE:   
                {
                 	if(!palettecall) //Wype³niamy strukturê jednorazowo, jak zajdzie taka potrzeba.
                 	{
						ZeroMemory( & ColorsDialog, sizeof( CHOOSECOLOR ) );
						ColorsDialog.lStructSize = sizeof( CHOOSECOLOR );
						ColorsDialog.hwndOwner = hwnd;
						ColorsDialog.lpCustColors = ColorTable;
						ColorsDialog.Flags = CC_ANYCOLOR;
						
						palettecall=true;
					}
					bool ChooseColorResult;
                    ChooseColorResult = ChooseColor( & ColorsDialog );
                    
                    if(ChooseColorResult)
                    {
                    	Red[SelectedColor] = GetRValue(ColorsDialog.rgbResult);
                    	Green[SelectedColor] = GetGValue(ColorsDialog.rgbResult);
                    	Blue[SelectedColor] = GetBValue(ColorsDialog.rgbResult);
					}
                	break;
				}
				case FILE_MENU_EXIT:
				{
					DestroyWindow(hwnd);
					break;
				}
				case FILE_MENU_NEW:
				{
					int MessBox = MessageBoxW(NULL, L"Rozpoczynajac nowy stracisz biezacy rysunek. Czy jestes pewien?", L"Nowy", MB_ICONWARNING |  MB_YESNO);
					
					switch (MessBox)
				    {
				   		case IDYES:
				   		{
				        	SetCanvas(hwnd);
				        	SaveCanvasChanges(hwnd);
				        	break;
				   		}
				    	case IDNO:
				        	break;
				    }
				    
				    break;
				}
				case FILE_MENU_OPEN:
				{
					OPENFILENAME ofn;
	    			char FileName[ MAX_PATH ] = "";
	    				
					ZeroMemory( & ofn, sizeof( ofn ) );
					ofn.lStructSize = sizeof( ofn );
					ofn.lpstrFilter = L"(*.bmp)\0*.bmp\0 \0*.*\0";
					ofn.nMaxFile = MAX_PATH;
					ofn.lpstrFile = FileName;
					ofn.lpstrDefExt = L"bmp";
					ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

					if(!GetOpenFileNameW( & ofn ))
						break;
						
					hOpenImage = (HBITMAP) LoadImageW(NULL, FileName,IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
					hScreen[0]=hOpenImage;
					DrawOpenFile(hwnd);
				    break;
				}
    			case FILE_MENU_SAVE:
				{	

					OPENFILENAME ofn;
    				WCHAR szFileName[MAX_PATH] = L"";
  					ZeroMemory(&ofn, sizeof(ofn));
				    ofn.lStructSize = sizeof(ofn); 
				    ofn.hwndOwner = NULL;
				    ofn.lpstrFilter = "bmp";
				    ofn.lpstrFile = szFileName;
				    ofn.nMaxFile = MAX_PATH;
				    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				    ofn.lpstrDefExt = "bmp";
				
				    if(!GetSaveFileName(&ofn))
						break;	

					SaveBitmapToFile(hScreen[0], ofn.lpstrFile);
					break;	
				}
				case MENU_PRINT:
				{
					PrintCurrentCanvas(hwnd);
					break;
				}
				case MENU_RESIZE:
				{
					DialogResize = CreateDialog(GetModuleHandle(NULL), MENU_RESIZE,
		            hwnd, DlgResize);
		            
		            if(DialogResize != NULL)
		            	ShowWindow(DialogResize, SW_SHOW);
		        
					break;
				}  
				case MENU_HELP:
				{
					DialogHelp = CreateDialog(GetModuleHandle(NULL), MENU_HELP, hwnd, DlgHelp);
		            
		           	if(DialogHelp != NULL)
		        		ShowWindow(DialogHelp, SW_SHOW);
		        	
					break;
				}
				
			
				break;
			}		
		case WM_CREATE:
		    {
            	DrawSelectedColors(hwnd);
            	
	            if(!createcall)
	            {
	            	
					CreateTrackBars(hwnd);
					loadImages();
					ShowCursorPosition(hwnd, lParam);
					AddMenus(hwnd);
		            AddColors(hwnd);
		            AddShapes(hwnd);
		            AddTools(hwnd);
		            createcall=true;
	            }
	            
           		break;
		    }
		case WM_RBUTTONDOWN:
            {
                rightclicked = true;
                linestarted = false;
                SendMessage(hwnd, WM_MOUSEMOVE, wParam, lParam );
            	break;
       		 }
		case WM_RBUTTONUP:
			{
	        	rightclicked = false;
	        	break;
	    	}
		case WM_LBUTTONDOWN:
            {
            	
            	if(gettingcolor)
            	{
            		GetColor(hwnd, lParam);
				}
				else if(pouring)
            	{
            		HDC hdc = GetDC(hwnd);
            		PourToTop(hwnd, LOWORD( lParam ), HIWORD( lParam ),GetPixel(hdc, LOWORD( lParam ), HIWORD( lParam )));
            		pouring=false;
            		hPourImage = (HBITMAP) LoadImageW(NULL, L"images\\tools\\Pour.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
					DestroyWindow(hPour);
					AddTools(hwnd);
					SaveCanvasChanges(hwnd);
            		ReleaseDC( hwnd, hdc );
				}
				else if(Line && linestarted)
				{
					if((HIWORD( lParam ) > 160 + ycolor && HIWORD( lParam ) < 160 + ycolor + ycanvas ) &&
					 (LOWORD( lParam ) > 23 + xcolor && LOWORD( lParam ) < 23 + xcolor + xcanvas ))
					{
						GetTrackBarsPosition();
						SetPaint1(hwnd);
						POINT point;
	            		HDC hdc = GetDC(hwnd);
	            		
						Pen = ( HPEN ) SelectObject(hdc, Color2Pen);
						MoveToEx(hdc, LOWORD( lParam), HIWORD( lParam), &point);
						LineTo( hdc, LastCoursorPosition.x, LastCoursorPosition.y );
						ReleaseDC( hwnd, hdc );
						linestarted = false;
						SaveCanvasChanges(hwnd);
					}
				}
            	else if(Line)
            	{
            		if((HIWORD( lParam ) > 160 + ycolor && HIWORD( lParam ) < 160 + ycolor + ycanvas ) &&
					 (LOWORD( lParam ) > 23 + xcolor && LOWORD( lParam ) < 23 + xcolor + xcanvas ))
					{
	            		HDC hdc = GetDC(hwnd);
	            		linestarted=true;
	            		LastCoursorPosition.x=LOWORD( lParam);
	            		LastCoursorPosition.y=HIWORD( lParam);	
	            		ReleaseDC( hwnd, hdc );	
	            	}
				}
				else
				{
	                leftclicked = true;
	                SendMessage( hwnd, WM_MOUSEMOVE, wParam, lParam );
           		 }
            }
            break;

	    case WM_LBUTTONUP:
	    	{
		        leftclicked = false;
		        break;
			}
    	case WM_MOUSEMOVE:
    		{

		    	if((HIWORD( lParam ) > 160 + ycolor && HIWORD( lParam ) < 160 + ycolor + ycanvas ) && 
				(LOWORD( lParam ) > 23 + xcolor && LOWORD( lParam ) < 23 + xcolor + xcanvas ))
		    	ShowCursorPosition(hwnd, lParam);
		    	
       			if( rightclicked )
        		{
		            HDC hdc = GetDC( hwnd );
		            GetTrackBarsPosition();
		            SetPaint1(hwnd);
            
		            Brush =( HBRUSH ) SelectObject( hdc, Color1Brush );
					Pen =( HPEN ) SelectObject( hdc, Color2Pen );
            
            
	            	if((HIWORD( lParam ) > 160 + ycolor && HIWORD( lParam ) < 160 + ycolor + ycanvas ) && 
					(LOWORD( lParam ) > 23 + xcolor && LOWORD( lParam ) < 23 + xcolor + xcanvas ))
					{
			            if(Circle)
			            {
			            	
			            	Ellipse( hdc, LOWORD( lParam )-Size, HIWORD( lParam )-Size, LOWORD( lParam ) + Size, HIWORD( lParam ) + Size );
			            	SaveCanvasChanges(hwnd);
			            	ReleaseDC( hwnd, hdc );
			            }
			            else if(Square)
			            {
			            	Rectangle( hdc, LOWORD( lParam )-Size, HIWORD( lParam )-Size, LOWORD( lParam ) + Size, HIWORD( lParam ) + Size );
			            	SaveCanvasChanges(hwnd);
			            	ReleaseDC( hwnd, hdc );
			            }
			            else if(Diamond)
			            {
			            	DrawDiamond(hdc, lParam);
			            	SaveCanvasChanges(hwnd);
			            	ReleaseDC( hwnd, hdc );
			            }
			            else if(Hexagon)
			            {
			            	DrawHexagon(hdc, lParam);
			            	SaveCanvasChanges(hwnd);
			            	ReleaseDC( hwnd, hdc );
			            }
			            
			            if(HIWORD( lParam ) < 190 + ycolor && (LOWORD( lParam ) < 203 + xcolor))
			            	DrawSelectedColors(hwnd);
			       		}
        		}
        		else if( leftclicked)
         		{
         			HDC hdc = GetDC( hwnd );
            
		            GetTrackBarsPosition();
		            SetPaint2(hwnd);
            
           			Brush = ( HBRUSH ) SelectObject( hdc, Color2Brush );
					Pen = ( HPEN ) SelectObject( hdc, Color1Pen );
            
           			if((HIWORD( lParam ) > 160 + ycolor && HIWORD( lParam ) < 160 + ycolor + ycanvas )
			 			&& (LOWORD( lParam ) > 23 + xcolor && LOWORD( lParam ) < 23 + xcolor + xcanvas ))
					{
			            if(Circle)
			            {
			            	
			            	Ellipse( hdc, LOWORD( lParam )-Size, HIWORD( lParam )-Size, LOWORD( lParam ) + Size, HIWORD( lParam ) + Size );
			            	SaveCanvasChanges(hwnd);
			            	ReleaseDC( hwnd, hdc );
			            }
			            else if(Square)
			            {
			            	Rectangle( hdc, LOWORD( lParam )-Size, HIWORD( lParam )-Size, LOWORD( lParam ) + Size, HIWORD( lParam ) + Size );
			            	SaveCanvasChanges(hwnd);
			            	ReleaseDC( hwnd, hdc );
			            }
			                else if(Diamond)
			            {
			            	DrawDiamond(hdc, lParam);
			            	SaveCanvasChanges(hwnd);
			            	ReleaseDC( hwnd, hdc );
			            }
			             else if(Hexagon)
			            {
			            	DrawHexagon(hdc, lParam);
			            	SaveCanvasChanges(hwnd);
			            	ReleaseDC( hwnd, hdc );
			            }
			            if(HIWORD( lParam ) < 190 + ycolor && (LOWORD( lParam ) < 203 + xcolor))
			            DrawSelectedColors(hwnd);
						}
        			}
        
    			break;
			}
		case WM_DESTROY: 
			{
				PostQuitMessage(0);
				break;
			default:
				return DefWindowProcW(hwnd, Message, wParam, lParam);
			}

	}
	return 0;
}

BOOL CALLBACK DlgHelp( HWND hHelp, UINT Msg, WPARAM wParam, LPARAM lParam )
{
    switch( Msg )
    {
    	case WM_PAINT:
    	{
		  	hOwl = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER| SS_BITMAP, 1, 1, 551, 531, hHelp, NULL, NULL, NULL);
		    SendMessageW(hOwl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hOwlImage);
            break;
       	}
   		case WM_CLOSE: 
		{
			EndDialog( hHelp, IDCANCEL );
			break;
		}
        default: return FALSE;
    } 
}

BOOL CALLBACK DlgResize( HWND hPasek, UINT Msg, WPARAM wParam, LPARAM lParam )
{
    switch( Msg )
    {
	case WM_HSCROLL:
	{		
       UpdateResize();
       break;
	}
   	case WM_PAINT:
   	{
   		CreateResizeControls(hPasek);	
   		break;
   	}
    case WM_COMMAND:
    {	
            switch( LOWORD( wParam ) )
            {
				case IDCANCEL:	 
				{
					EndDialog( hPasek, IDCANCEL );
					break;
				}
				case IDOK:
				{
					int a,h;
					xcanvasold = xcanvas;
					ycanvasold = ycanvas;
				
					xcanvas = SendMessageW(hWidth, TBM_GETPOS, 0, 0);
					ycanvas = SendMessageW(hHeight, TBM_GETPOS, 0, 0);
				
					SetCanvas(hwnd);
					
					a=xcanvas;
					h=ycanvas;
					
					xcanvas=xcanvasold;
					ycanvas=ycanvasold;
					
					DrawCanvas(hwnd);
					
					xcanvas=a;
					ycanvas=h;
					
					EndDialog( hPasek, IDCANCEL );
					break;
				}
            }
        }
        break;
        
   	case WM_CLOSE: 
	    {
			EndDialog( hPasek, IDCANCEL );
			break;
		}
       
    default: return FALSE;
    }
   
}

void AddColors(HWND hwnd)
{
    hRed = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD|WS_BORDER | BS_BITMAP, xcolor + 120, ycolor + 50, 25, 25, hwnd, (HMENU) COLOR_RED, NULL, NULL);
        SendMessageW(hRed,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hRedImage);
	hYellow = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER | BS_BITMAP, xcolor + 150, ycolor + 50, 25, 25, hwnd, (HMENU) COLOR_YELLOW, NULL, NULL);
        SendMessageW(hYellow,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hYellowImage);
	hGreen = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER| BS_BITMAP, xcolor + 180, ycolor + 50, 25, 25, hwnd, (HMENU) COLOR_GREEN, NULL, NULL);
        SendMessageW(hGreen,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hGreenImage);
    hBlue = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD|WS_BORDER | BS_BITMAP, xcolor + 210, ycolor + 50, 25, 25, hwnd, (HMENU) COLOR_BLUE, NULL, NULL);
        SendMessageW(hBlue,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hBlueImage);
	hPurple = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER | BS_BITMAP, xcolor + 240, ycolor + 50, 25, 25, hwnd, (HMENU) COLOR_PURPLE, NULL, NULL);
        SendMessageW(hPurple,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hPurpleImage);
	hOrange = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER| BS_BITMAP, xcolor + 270, ycolor + 50, 25, 25, hwnd, (HMENU) COLOR_ORANGE, NULL, NULL);
        SendMessageW(hOrange,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hOrangeImage);

     hBrown = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD|WS_BORDER | BS_BITMAP, xcolor + 120, ycolor +  80, 25, 25, hwnd, (HMENU)COLOR_BROWN, NULL, NULL);
        SendMessageW(hBrown,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hBrownImage);
	hBlack = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER | BS_BITMAP, xcolor + 150, ycolor + 80, 25, 25, hwnd, (HMENU)COLOR_BLACK, NULL, NULL);
        SendMessageW(hBlack,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hBlackImage);
	hWhite = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER| BS_BITMAP, xcolor + 180, ycolor + 80, 25, 25, hwnd,(HMENU)COLOR_WHITE, NULL, NULL);
        SendMessageW(hWhite,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hWhiteImage);
    hGrey = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD|WS_BORDER | BS_BITMAP, xcolor + 210, ycolor + 80, 25, 25, hwnd, (HMENU)COLOR_GREY, NULL, NULL);
        SendMessageW(hGrey,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hGreyImage);
	hLightGrey = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER | BS_BITMAP, xcolor + 240, ycolor + 80, 25, 25, hwnd, (HMENU)COLOR_LIGHTGREY, NULL, NULL);
        SendMessageW(hLightGrey,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hLightGreyImage);
	hPink = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER| BS_BITMAP, xcolor + 270, ycolor + 80, 25, 25, hwnd, (HMENU)COLOR_PINK, NULL, NULL);
        SendMessageW(hPink,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hPinkImage);

	hPalette = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER| BS_BITMAP, xcolor + 315, ycolor + 60, 33, 33, hwnd, (HMENU)COLOR_PALETTE, NULL, NULL);
        SendMessageW(hPalette,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hPaletteImage);
}

void AddShapes(HWND hwnd)
{
    hSquare = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER | BS_BITMAP, xcolor + 368, ycolor + 55, 45, 45, hwnd, (HMENU)SHAPE_SQUARE, NULL, NULL);
        SendMessageW(hSquare,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hSquareImage);
	hCircle = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER| BS_BITMAP, xcolor + 418, ycolor + 55, 45, 45, hwnd, (HMENU)SHAPE_CIRCLE, NULL, NULL);
        SendMessageW(hCircle,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hCircleImage);
    hDiamond = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER| BS_BITMAP, xcolor + 468, ycolor + 55, 45, 45, hwnd, (HMENU)SHAPE_DIAMOND, NULL, NULL);
        SendMessageW(hDiamond,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hDiamondImage);
    hHexagon = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER| BS_BITMAP, xcolor + 518, ycolor + 55, 45, 45, hwnd, (HMENU)SHAPE_HEXAGON, NULL, NULL);
        SendMessageW(hHexagon,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hHexagonImage);
	hLine = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER| BS_BITMAP, xcolor + 574, ycolor + 55, 90, 45, hwnd, (HMENU)SHAPE_LINE, NULL, NULL);
        SendMessageW(hLine,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hLineImage);
        

}

void AddSpecificShapes(HWND hwnd)
{
	
	if(Square)
	{
	hSquare = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER | BS_BITMAP, xcolor + 368, ycolor + 55, 45, 45, hwnd, (HMENU)SHAPE_SQUARE, NULL, NULL);
        SendMessageW(hSquare,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hSquareImage);
    }
    if(Circle)
    {
	hCircle = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER| BS_BITMAP, xcolor + 418, ycolor + 55, 45, 45, hwnd, (HMENU)SHAPE_CIRCLE, NULL, NULL);
        SendMessageW(hCircle,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hCircleImage);
    }
    if(Diamond)
    {
    hDiamond = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER| BS_BITMAP, xcolor + 468, ycolor + 55, 45, 45, hwnd, (HMENU)SHAPE_DIAMOND, NULL, NULL);
        SendMessageW(hDiamond,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hDiamondImage);
    }
    if(Hexagon)
    {
    hHexagon = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER| BS_BITMAP, xcolor + 518, ycolor + 55, 45, 45, hwnd, (HMENU)SHAPE_HEXAGON, NULL, NULL);
        SendMessageW(hHexagon,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hHexagonImage);
    }
    if(Line)
    {
	hLine = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER| BS_BITMAP, xcolor + 574, ycolor + 55, 90, 45, hwnd, (HMENU)SHAPE_LINE, NULL, NULL);
        SendMessageW(hLine,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hLineImage);
    }
}

void AddTools(HWND hwnd)
{
    
	hGetColor = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER | BS_BITMAP, xcolor + 968, ycolor + 55, 45, 45, hwnd, (HMENU)TOOL_GETCOLOR, NULL, NULL);
        SendMessageW(hGetColor,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hGetColorImage);
    hUndo = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER | BS_BITMAP, xcolor + 1018, ycolor + 55, 45, 45, hwnd, (HMENU)TOOL_UNDO, NULL, NULL);
        SendMessageW(hUndo,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hUndoImage);
    hPour = CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER | BS_BITMAP, xcolor + 1068, ycolor + 55, 45, 45, hwnd, (HMENU)TOOL_POUR, NULL, NULL);
        SendMessageW(hPour,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hPourImage);
}

void DrawSelectedColors(HWND hwnd)
{
	HDC hdcWindow = GetDC( hwnd );
	HPEN  BlackPen, GreyPen;
	HBRUSH GreyBrush, WhiteBrush;

	Color1Brush = CreateSolidBrush(  RGB( Red[0], Green[0], Blue[0]));
	Color2Brush = CreateSolidBrush(  RGB( Red[1], Green[1], Blue[1]));

	GreyBrush = CreateSolidBrush(  RGB( 240, 240, 240));
	GreyPen = CreatePen( PS_SOLID, 1, RGB(240, 240, 240));
	Brush =( HBRUSH ) SelectObject( hdcWindow, GreyBrush );
	Pen =( HPEN ) SelectObject( hdcWindow, GreyPen );
    
    
	Rectangle( hdcWindow, 0 + xcolor, 40 + ycolor, 155 + xcolor, 94+ ycolor );


	WhiteBrush = CreateSolidBrush(  RGB( 255, 255, 255));
  	BlackPen = CreatePen( PS_SOLID, 1, RGB(0, 0, 0));

    Brush =( HBRUSH ) SelectObject( hdcWindow, WhiteBrush );
    Pen =( HPEN ) SelectObject( hdcWindow, BlackPen );

	
    Rectangle( hdcWindow, 21 + xcolor, 50 + ycolor, 55 + xcolor, 84+ ycolor );
    Rectangle( hdcWindow, 71 + xcolor, 50 + ycolor, 104 + xcolor, 84+ ycolor );


	Brush =( HBRUSH ) SelectObject( hdcWindow, Color1Brush );
	Pen =( HPEN ) SelectObject( hdcWindow, BlackPen );
	Rectangle( hdcWindow, 23 + xcolor, 52 + ycolor, 53 + xcolor, 82+ ycolor );

	Brush =( HBRUSH ) SelectObject( hdcWindow, Color2Brush );
	Rectangle( hdcWindow, 73 + xcolor, 52 + ycolor, 102 + xcolor, 82+ ycolor );

	DeleteObject( BlackPen );
	DeleteObject( GreyPen );
	DeleteObject( WhiteBrush );
	DeleteObject( GreyBrush );
	ReleaseDC( hwnd, hdcWindow );
}

void loadImages()
{
    hRedImage = (HBITMAP) LoadImageW(NULL, L"images\\colors\\Red.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    hYellowImage = (HBITMAP) LoadImageW(NULL, L"images\\colors\\Yellow.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    hGreenImage = (HBITMAP) LoadImageW(NULL, L"images\\colors\\Green.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    hBlueImage = (HBITMAP) LoadImageW(NULL, L"images\\colors\\Blue.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    hPurpleImage = (HBITMAP) LoadImageW(NULL, L"images\\colors\\Purple.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    hOrangeImage = (HBITMAP) LoadImageW(NULL, L"images\\colors\\Orange.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);

    hBrownImage = (HBITMAP) LoadImageW(NULL, L"images\\colors\\Brown.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    hBlackImage = (HBITMAP) LoadImageW(NULL, L"images\\colors\\Black.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    hWhiteImage = (HBITMAP) LoadImageW(NULL, L"images\\colors\\White.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    hGreyImage = (HBITMAP) LoadImageW(NULL, L"images\\colors\\Grey.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    hLightGreyImage = (HBITMAP) LoadImageW(NULL, L"images\\colors\\LightGrey.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    hPinkImage = (HBITMAP) LoadImageW(NULL, L"images\\colors\\Pink.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);

    hPaletteImage = (HBITMAP) LoadImageW(NULL, L"images\\colors\\Palette.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);

    hCircleImage = (HBITMAP) LoadImageW(NULL, L"images\\shapes\\Circle.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    hSquareImage = (HBITMAP) LoadImageW(NULL, L"images\\shapes\\Square.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    hDiamondImage = (HBITMAP) LoadImageW(NULL, L"images\\shapes\\Diamond.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    hHexagonImage = (HBITMAP) LoadImageW(NULL, L"images\\shapes\\Hexagon.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    hLineImage = (HBITMAP) LoadImageW(NULL, L"images\\shapes\\Line.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    
    hGetColorImage = (HBITMAP) LoadImageW(NULL, L"images\\tools\\getcolor.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    hUndoImage = (HBITMAP) LoadImageW(NULL, L"images\\tools\\Undo.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    hPourImage = (HBITMAP) LoadImageW(NULL, L"images\\tools\\Pour.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    
    hFillImage = (HBITMAP) LoadImageW(NULL, L"images\\tools\\outline.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    hOutlineImage = (HBITMAP) LoadImageW(NULL, L"images\\tools\\fill.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
    
    hOwlImage = (HBITMAP) LoadImageW(NULL, L"images\\owl.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
}

void AddMenus(HWND hwnd)
{
	hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();

	AppendMenu(hFileMenu,MF_STRING,FILE_MENU_NEW,"Nowy");
	AppendMenu(hFileMenu,MF_STRING,FILE_MENU_OPEN,"Otwórz");
	AppendMenu(hFileMenu,MF_STRING,FILE_MENU_SAVE,"Zapisz");
	AppendMenu(hFileMenu,MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu,MF_STRING,FILE_MENU_EXIT,"WyjdŸ");



	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu,"Plik");
	AppendMenu(hMenu, MF_POPUP, MENU_PRINT,"Drukuj");
	AppendMenu(hMenu, MF_POPUP, MENU_RESIZE,"Zmien rozmiar");
	AppendMenu(hMenu, MF_POPUP, MENU_HELP,"Pomoc");

	SetMenu(hwnd, hMenu);
}


void CreateStatic(HWND hwnd)
{

	HWND STitle[13];
	
	CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD |BS_GROUPBOX, 115+xcolor,22+ycolor, 1550, 88, hwnd,NULL,NULL,NULL);

	CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD, 0, 84+ycolor, 116+xcolor, 66, hwnd,NULL,NULL,NULL);
	CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD |BS_GROUPBOX, 10+xcolor, 45+ycolor, 1, 65, hwnd,NULL,NULL,NULL);
	CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD |BS_GROUPBOX, 62+xcolor, 45+ycolor, 1, 65, hwnd,NULL,NULL,NULL);
	CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD |BS_GROUPBOX, 10+xcolor, 109+ycolor, 106, 1, hwnd,NULL,NULL,NULL);
	
	STitle[0] = CreateWindowW(L"static",L"Kolor 1", WS_VISIBLE | WS_CHILD |WS_BORDER | SS_CENTER, 10+xcolor, 22+ycolor, 53, 24, hwnd,NULL,NULL,NULL);
	STitle[1] =	CreateWindowW(L"static",L"Kolor 2", WS_VISIBLE | WS_CHILD |WS_BORDER | SS_CENTER, 62+xcolor, 22+ycolor, 54, 24, hwnd,NULL,NULL,NULL);
	
    CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD |BS_GROUPBOX, 115+xcolor, 45+ycolor, 185, 65, hwnd,NULL,NULL,NULL);
	STitle[2] = CreateWindowW(L"static",L"Kolory", WS_VISIBLE | WS_CHILD | SS_CENTER |WS_BORDER, 115+xcolor, 22+ycolor, 185, 24, hwnd,NULL,NULL,NULL);
    
    CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD | SS_CENTER |BS_GROUPBOX, 363+xcolor, 45+ycolor, 205, 65, hwnd,NULL,NULL,NULL);
    STitle[3] = CreateWindowW(L"static",L"Ksztalty pedzla", WS_VISIBLE | WS_CHILD | SS_CENTER |WS_BORDER, 363+xcolor, 22+ycolor, 205, 24, hwnd,NULL,NULL,NULL);

    STitle[4] = CreateWindowW(L"static",L"Paleta", WS_VISIBLE | WS_CHILD | SS_CENTER |WS_BORDER, 299+xcolor, 22+ycolor, 65, 24, hwnd,NULL,NULL,NULL);
    CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD |BS_GROUPBOX, 299+xcolor, 45+ycolor, 65, 65, hwnd,NULL,NULL,NULL);
    
    CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 567+xcolor, 45+ycolor, 105, 65, hwnd,NULL,NULL,NULL);
	STitle[5] = CreateWindowW(L"static",L"Linia", WS_VISIBLE | WS_CHILD | SS_CENTER |WS_BORDER, 567+xcolor, 22+ycolor, 105, 24, hwnd,NULL,NULL,NULL);
	
	CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD |BS_GROUPBOX, 671+xcolor, 45+ycolor, 111, 65, hwnd,NULL,NULL,NULL);
	CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD |BS_GROUPBOX, 671+xcolor, 45+ycolor, 58, 65, hwnd,NULL,NULL,NULL);
	STitle[6] = CreateWindowW(L"static",L"Wypelnij", WS_VISIBLE | WS_CHILD |WS_BORDER | SS_CENTER, 671+xcolor, 22+ycolor, 58, 24, hwnd,NULL,NULL,NULL);
	STitle[7] =	CreateWindowW(L"static",L"Kontur", WS_VISIBLE | WS_CHILD |WS_BORDER | SS_CENTER, 728+xcolor, 22+ycolor, 54, 24, hwnd,NULL,NULL,NULL);
		
    hOutline = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER| SS_BITMAP, xcolor + 681, ycolor + 70, 31, 31, hwnd, (HMENU)SHAPE_LINE, NULL, NULL);
        SendMessageW(hOutline,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hOutlineImage);
        
    hFill = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD |WS_BORDER| SS_BITMAP, xcolor + 737, ycolor + 70, 31, 31, hwnd, (HMENU)SHAPE_HEXAGON, NULL, NULL);
        SendMessageW(hFill,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hFillImage);


    CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 781+xcolor, 45+ycolor, 180, 65, hwnd,NULL,NULL,NULL);
	STitle[8] = CreateWindowW(L"static",L"Rozmiar pedzla i linii", WS_VISIBLE | WS_CHILD | SS_CENTER |WS_BORDER, 781+xcolor, 22+ycolor, 180, 24, hwnd,NULL,NULL,NULL);
	
	CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 960+xcolor, 45+ycolor, 160, 65, hwnd,NULL,NULL,NULL);
	STitle[9] = CreateWindowW(L"static",L"Narzedzia", WS_VISIBLE | WS_CHILD | SS_CENTER |WS_BORDER, 960+xcolor, 22+ycolor, 160, 24, hwnd,NULL,NULL,NULL);
	
	CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 1119+xcolor, 45+ycolor, 120, 65, hwnd,NULL,NULL,NULL);
	STitle[10] = CreateWindowW(L"static",L"Pozycja kursora", WS_VISIBLE | WS_CHILD | SS_CENTER |WS_BORDER, 1119+xcolor, 22+ycolor, 120, 24, hwnd,NULL,NULL,NULL);
	STitle[11] = CreateWindowW(L"static",L"x:", WS_VISIBLE | WS_CHILD | SS_CENTER, 1131+xcolor, 52+ycolor, 15, 20, hwnd,NULL,NULL,NULL);
	STitle[12] = CreateWindowW(L"static",L"y:", WS_VISIBLE | WS_CHILD | SS_CENTER, 1131+xcolor, 77+ycolor, 15, 20, hwnd,NULL,NULL,NULL);
	

	HFONT hFont = CreateFont (19, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
	 OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("MS Sans Serif"));
	 	
	int i=0;
	
	while( i<13)
	{
		SendMessage(STitle[i], WM_SETFONT, hFont, TRUE);
		i++;
	}

}

void SetCanvas(HWND hwnd)
{
	
	static HWND Canvas1, Canvas2, Canvas3, Canvas4, Canvas5,Canvas6;
	static bool call = false;
	
	if(call)
	{
		DestroyWindow(Canvas1);
		DestroyWindow(Canvas2);
		DestroyWindow(Canvas3);
		DestroyWindow(Canvas4);
		DestroyWindow(Canvas5);
		DestroyWindow(Canvas6);
	}

	HDC hdcWindow = GetDC( hwnd );

	HBRUSH WhiteBrush = CreateSolidBrush(  RGB( 255, 255, 255));
	HPEN WhitePen = CreatePen( PS_SOLID, 1, RGB(255, 255, 255));

	Canvas1 = CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD, 0, ycolor+110, xcanvas+200, 50, hwnd,NULL,NULL,NULL);
	Canvas2 = CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD, 0, ycolor+110, 23+xcolor, ycanvas+200, hwnd,NULL,NULL,NULL);
	Canvas3 = CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD, 0, ycolor+ycanvas+160, 23+xcolor+xcanvas, ycanvas+200, hwnd,NULL,NULL,NULL);
	Canvas4 = CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD, 0, ycolor+ycanvas+160, 23+xcolor+xcanvas+200, ycanvas+800, hwnd,NULL,NULL,NULL);
	Canvas5 = CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD, 23 + xcolor+xcanvas, ycolor+111, 22+xcolor+xcanvas+1400, ycolor+ycanvas+200, hwnd,NULL,NULL,NULL);
	Canvas6 = CreateWindowW(L"static",NULL, WS_VISIBLE | WS_CHILD, 23 + xcolor+xcanvas, ycolor+ycanvas+160, 1400, 500, hwnd,NULL,NULL,NULL);

	(HPEN) SelectObject(hdcWindow, WhitePen);
	Rectangle( hdcWindow, 23 + xcolor, 160 + ycolor, 23 + xcolor+xcanvas, 160 + ycolor+ycanvas );

	call=true;
	
	DeleteObject( WhitePen );
	DeleteObject( WhiteBrush );
}

void GetColor(HWND hwnd, LPARAM lParam)
{	
	HDC hdc = GetDC(hwnd);
	 
	COLORREF color = GetPixel(hdc, LOWORD( lParam ), HIWORD( lParam ));
    Red[SelectedColor] = GetRValue(color);
    Green[SelectedColor] = GetGValue(color);
    Blue[SelectedColor] = GetBValue(color);
            
    DrawSelectedColors(hwnd);
    
	hGetColorImage = (HBITMAP) LoadImageW(NULL, L"images\\tools\\getcolor.bmp",IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
	DestroyWindow(hGetColor);
	AddTools(hwnd);
					
    gettingcolor=false;    
}

void SetPaint1(HWND hwnd) 
{
	DeleteObject( Color2Pen );
	DeleteObject( Color1Brush );

	if(Fill)
		Color1Brush = CreateSolidBrush(  RGB(Red[0], Green[0], Blue[0]));
	else
		Color1Brush = GetStockObject(HOLLOW_BRUSH);
	
	if(Outline)
		Color2Pen = CreatePen( PS_SOLID, Thickness, RGB(Red[1], Green[1], Blue[1]));
	else
		Color2Pen = GetStockObject(NULL_PEN);
}

void SetPaint2(HWND hwnd)
{
	DeleteObject( Color1Pen );
	DeleteObject( Color2Brush );

	if(Fill)
		Color2Brush = CreateSolidBrush(  RGB(Red[1], Green[1], Blue[1]));
	else
		Color2Brush = GetStockObject(HOLLOW_BRUSH);

	if(Outline)
		Color1Pen = CreatePen( PS_SOLID, Thickness, RGB(Red[0], Green[0], Blue[0]));
	else
		Color1Pen = GetStockObject(NULL_PEN);
}

void CreateTrackBars(HWND hwnd)
{
	hSize = CreateWindowW(TRACKBAR_CLASSW, L"Trackbar Control", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
		795 + xcolor, 47 + ycolor, 140, 30, hwnd, (HMENU) 3, NULL, NULL); 

	// http://zetcode.com/gui/winapi/controlsII/
	SendMessageW(hSize, TBM_SETRANGE,  TRUE, MAKELONG(0, 100)); 
	SendMessageW(hSize, TBM_SETPAGESIZE, 0,  10); 
    SendMessageW(hSize, TBM_SETTICFREQ, 10, 0); 
    SendMessageW(hSize, TBM_SETPOS, FALSE, 0); 


	hThickness = CreateWindowW(TRACKBAR_CLASSW, L"Trackbar Control", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
        795 + xcolor, 77 + ycolor, 140, 30, hwnd, (HMENU) 3, NULL, NULL); 

	
	SendMessageW(hThickness, TBM_SETRANGE,  TRUE, MAKELONG(0, 20)); 
    SendMessageW(hThickness, TBM_SETPAGESIZE, 0,  2); 
    SendMessageW(hThickness, TBM_SETTICFREQ, 2, 0); 
    SendMessageW(hThickness, TBM_SETPOS, FALSE, 0); 
}


void CreateResizeControls(HWND hwnd)
{	
	HFONT hFont = CreateFont (19, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, 
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("MS Sans Serif"));
	
		
	hWidthValue = CreateWindowW(L"Static", L"150", WS_CHILD | WS_VISIBLE, 95, 40, 35, 30, hwnd, (HMENU)3, NULL, NULL);
	hWidth = CreateWindowW(TRACKBAR_CLASSW, L"Trackbar Control", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, 220, 45, 140, 40, hwnd, (HMENU) 3, NULL, NULL); 
	HWND hLeftWidthLabel = CreateWindowW(L"Static", L"150", WS_CHILD | WS_VISIBLE, 0, 0, 25, 30, hwnd, (HMENU)1, NULL, NULL);
	HWND hRightWidthLabel = CreateWindowW(L"Static", L"1500", WS_CHILD | WS_VISIBLE, 0, 0, 35, 30, hwnd, (HMENU)2, NULL, NULL);
    
    SendMessage(hWidthValue, WM_SETFONT, hFont, TRUE);	
	SendMessage(hWidth, WM_SETFONT, hFont, TRUE);
	SendMessage(hLeftWidthLabel, WM_SETFONT, hFont, TRUE);
	SendMessage(hRightWidthLabel, WM_SETFONT, hFont, FALSE);
 	SendMessageW(hWidth, TBM_SETRANGE,  TRUE, MAKELONG(150, 1500)); 
    SendMessageW(hWidth, TBM_SETPAGESIZE, 0,  10); 
    SendMessageW(hWidth, TBM_SETTICFREQ, 100, 0); 
    SendMessageW(hWidth, TBM_SETPOS, FALSE, 0); 
    SendMessageW(hWidth, TBM_SETBUDDY, TRUE, (LPARAM) hLeftWidthLabel); 
    SendMessageW(hWidth, TBM_SETBUDDY, FALSE, (LPARAM) hRightWidthLabel); 
    
    hHeightValue = CreateWindowW(L"Static", L"150", WS_CHILD | WS_VISIBLE, 95, 100, 80, 30, hwnd, (HMENU)3, NULL, NULL);
    hHeight = CreateWindowW(TRACKBAR_CLASSW, L"Trackbar Control", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, 220, 103, 140, 40, hwnd, (HMENU) 3, NULL, NULL);
	HWND hLeftHeightLabel = CreateWindowW(L"Static", L"150", WS_CHILD | WS_VISIBLE, 0, 0, 25, 30, hwnd, (HMENU)1, NULL, NULL);
	HWND hRightHeightLabel = CreateWindowW(L"Static", L"600", WS_CHILD | WS_VISIBLE, 0, 0, 30, 30, hwnd, (HMENU)2, NULL, NULL); 
    
    SendMessage(hHeightValue, WM_SETFONT, hFont, TRUE);
    SendMessage(hHeight, WM_SETFONT, hFont, TRUE);
	SendMessage(hLeftHeightLabel, WM_SETFONT, hFont, TRUE);
	SendMessage(hRightHeightLabel, WM_SETFONT, hFont, FALSE); 
    SendMessageW(hHeight, TBM_SETRANGE,  TRUE, MAKELONG(150, 600)); 
    SendMessageW(hHeight, TBM_SETPAGESIZE, 0,  10); 
    SendMessageW(hHeight, TBM_SETTICFREQ, 50, 0); 
    SendMessageW(hHeight, TBM_SETPOS, FALSE, 0); 
	SendMessageW(hHeight, TBM_SETBUDDY, TRUE, (LPARAM) hLeftHeightLabel); 
    SendMessageW(hHeight, TBM_SETBUDDY, FALSE, (LPARAM) hRightHeightLabel);    
}



void DrawDiamond(HDC hdc,  LPARAM lParam)
{
	POINT Node[4];
	Node[0].x=LOWORD( lParam )-Size;
	Node[0].y=HIWORD( lParam );
	Node[1].x=LOWORD( lParam );
	Node[1].y=HIWORD( lParam )-Size;
	Node[2].x=LOWORD( lParam )+Size;
	Node[2].y=HIWORD( lParam );
	Node[3].x=LOWORD( lParam );
	Node[3].y=HIWORD( lParam )+Size;
	
	Polygon(hdc, Node, 4);	
}

void DrawHexagon(HDC hdc,  LPARAM lParam)
{	
	POINT Node[6];
	int i=0;
	
	while( i<6 )
	{
		Node[i].x=LOWORD( lParam )+cos(2*Pi*30/360+2*Pi*60/360*i)*Size;
		Node[i].y=HIWORD( lParam )+sin(2*Pi*30/360+2*Pi*60/360*i)*Size;
		i++;
	}	
	
	Polygon(hdc, Node, 6);	
}

void DestroyShapes()
{
	if(Circle)
   	 	DestroyWindow(hCircle);
	if(Square)
		DestroyWindow(hSquare);
	if(Diamond)
		DestroyWindow(hDiamond);
	if(Hexagon)
		DestroyWindow(hHexagon);
	if(Line)
		DestroyWindow(hLine);
}



void GetTrackBarsPosition() 
{
    LRESULT pos = SendMessageW(hSize, TBM_GETPOS, 0, 0);
    Size=pos;
   	pos = SendMessageW(hThickness, TBM_GETPOS, 0, 0);
    Thickness = pos;
}

void SaveCanvasChanges(HWND hwnd)
{
	
HDC hScreenDC = GetDC(hwnd);       
HDC hMemoryDC = CreateCompatibleDC(hScreenDC);


int i=0;
HBITMAP a[100];
while(i<100)
{
	a[i]=hScreen[i];
	i++;
}

i=1;

while(i<100)
{
	hScreen[i]=a[i-1];
	i++;
} 

hScreen[0] = CreateCompatibleBitmap(hScreenDC, xcanvas, ycanvas);	
HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemoryDC, hScreen[0]);	
BitBlt(hMemoryDC, -23 - xcolor, -160 - ycolor, xcanvas + 23 + xcolor, ycanvas + 160 + ycolor, hScreenDC, 0, 0, SRCCOPY);
hScreen[0] = (HBITMAP) SelectObject(hMemoryDC, hOldBitmap);

DeleteDC(hMemoryDC);
DeleteDC(hScreenDC);		
}

void DrawCanvas(HWND hwnd)
{

    BITMAP          bitmap;
    HDC             hdcMem;
    HGDIOBJ         oldBitmap;
    HDC hdc = GetDC(hwnd);

    hdcMem = CreateCompatibleDC(hdc);
    oldBitmap = SelectObject(hdcMem, hScreen[0]);
	GetObject(hScreen[0], sizeof(bitmap), &bitmap);
    		
	BitBlt(hdc, 23 + xcolor, 160 + ycolor, xcanvas, ycanvas, hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, oldBitmap);
    DeleteDC(hdcMem);

}

void DrawOpenFile(HWND hwnd)
{

	
        HDC             hdc;
        BITMAP          bitmap;
        HDC             hdcMem;
        HGDIOBJ         oldBitmap;

        hdc = GetDC(hwnd);

        hdcMem = CreateCompatibleDC(hdc);
        oldBitmap = SelectObject(hdcMem, hScreen[0]);

        GetObject(hScreen[0], sizeof(bitmap), &bitmap);
        xcanvas=bitmap.bmWidth;
        ycanvas= bitmap.bmHeight;
        int width=bitmap.bmWidth;
        int height=bitmap.bmHeight;
        
        if(width>1500)
        {
        	width = 1500;
        	xcanvas = 1500;
    	}
        if(height>600)
        {
        	height=600;
        	ycanvas = 600;
    	}
    	
    	SetCanvas(hwnd);
    	
        BitBlt(hdc, 23 + xcolor, 160 + ycolor, width, height, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);

}

ShowCursorPosition(HWND hwnd, LPARAM lParam) //wersja robocza
{
	static HWND hXpos[4], hYpos[3];
	HWND xpix, ypix;
	static bool call=false;
	
	if(!call)
	{
		 hXpos[0] = CreateWindowW(L"static",L"0", WS_VISIBLE|WS_CLIPCHILDREN | WS_CHILD | SS_CENTER, 1176+xcolor, 52+ycolor, 12, 20, hwnd,NULL,NULL,NULL);
		 hXpos[1] = CreateWindowW(L"static",L"0", WS_VISIBLE|WS_CLIPCHILDREN | WS_CHILD | SS_CENTER, 1167+xcolor, 52+ycolor, 12, 20, hwnd,NULL,NULL,NULL);
		 hXpos[2] = CreateWindowW(L"static",L"0", WS_VISIBLE|WS_CLIPCHILDREN | WS_CHILD | SS_CENTER, 1158+xcolor, 52+ycolor, 12, 20, hwnd,NULL,NULL,NULL);
		 hXpos[3] = CreateWindowW(L"static",L"0", WS_VISIBLE|WS_CLIPCHILDREN | WS_CHILD | SS_CENTER, 1149+xcolor, 52+ycolor, 12, 20, hwnd,NULL,NULL,NULL);
		 xpix=CreateWindowW(L"static",L"piks.", WS_VISIBLE|WS_CLIPCHILDREN | WS_CHILD | SS_CENTER, 1188+xcolor, 52+ycolor, 42, 20, hwnd,NULL,NULL,NULL);
		 hYpos[0] = CreateWindowW(L"static",L"0", WS_VISIBLE | WS_CHILD | SS_CENTER, 1176+xcolor, 77+ycolor, 12, 20, hwnd,NULL,NULL,NULL);
		 hYpos[1] = CreateWindowW(L"static",L"0", WS_VISIBLE | WS_CHILD | SS_CENTER, 1167+xcolor, 77+ycolor, 12, 20, hwnd,NULL,NULL,NULL);
		 hYpos[2] = CreateWindowW(L"static",L"0", WS_VISIBLE | WS_CHILD | SS_CENTER, 1158+xcolor, 77+ycolor, 12, 20, hwnd,NULL,NULL,NULL);
		 ypix=CreateWindowW(L"static",L"piks.", WS_VISIBLE|WS_CLIPCHILDREN | WS_CHILD | SS_CENTER, 1188+xcolor, 77+ycolor, 42, 20, hwnd,NULL,NULL,NULL);
		 
		 HFONT hFont = CreateFont (19, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
			 OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("MS Sans Serif"));
			 
		SendMessage(hXpos[3], WM_SETFONT, hFont, TRUE); 
		SendMessage(xpix, WM_SETFONT, hFont, TRUE); 
		SendMessage(ypix, WM_SETFONT, hFont, TRUE); 
		
		int i=0;
		
		while( i<3)
		{
			SendMessage(hYpos[i], WM_SETFONT, hFont, TRUE);
			SendMessage(hXpos[i], WM_SETFONT, hFont, TRUE);
			i++;
		}
		
		call = true;
	}

	int x[4]={0,0,0,0};	
	int y[3]={0,0,0};

	LPARAM param[11];
	param[0]= L"0";
	param[1]= L"1";
	param[2]= L"2";
	param[3]= L"3"; 
	param[4]= L"4";
	param[5]= L"5";
	param[6]= L"6";
	param[7]= L"7";
	param[8]= L"8";
	param[9]= L"9";
	param[10]= L" ";

	int xpos=LOWORD( lParam )-24 - xcolor;
	int ypos=HIWORD( lParam )-161 - ycolor;
	
	x[0]=xpos%10;
	x[1]=((xpos-xpos%10)/10)%10;
	x[2]=((xpos-xpos%100)/100)%10;
	x[3]=((xpos-xpos%1000)/1000)%10;
	y[0]=ypos%10;
	y[1]=((ypos-ypos%10)/10)%10;;
	y[2]=((ypos-ypos%100)/100)%10;
 
	if(y[2]==0)
	{
	 	y[2]=10;
	 	if(y[1]==0)
	 		y[1]=10;
	}
	 
	if(x[3]==0)
	{
		x[3]=10;
		
	 	if(x[2]==0)
	 	{
	 		x[2]=10;
	 		if(x[1]==0)
	 		x[1]=10;
	 	}
	}
 

	SendMessageW(hXpos[0], WM_SETTEXT, FALSE, param[x[0]]);
	SendMessageW(hXpos[1], WM_SETTEXT, FALSE, param[x[1]]);
	SendMessageW(hXpos[2], WM_SETTEXT, FALSE, param[x[2]]);
	SendMessageW(hXpos[3], WM_SETTEXT, FALSE, param[x[3]]);
	SendMessageW(hYpos[0], WM_SETTEXT, FALSE, param[y[0]]);
	SendMessageW(hYpos[1], WM_SETTEXT, FALSE, param[y[1]]);
	SendMessageW(hYpos[2], WM_SETTEXT, FALSE, param[y[2]]);
}

void UpdateResize() 
{
	int const arraysize = 6;
	WCHAR pszDest[arraysize]; 
	size_t cbDest = arraysize * sizeof(WCHAR);

	LPCTSTR pszFormat = L"%s %d";
	WCHAR* pszTxt = L"";

	LRESULT pos = SendMessageW(hWidth, TBM_GETPOS, 0, 0);
	StringCbPrintfW(pszDest, cbDest, pszFormat, pszTxt, pos);
	SetWindowTextW(hWidthValue, pszDest);

	pos = SendMessageW(hHeight, TBM_GETPOS, 0, 0);
	StringCbPrintfW(pszDest, cbDest, pszFormat, pszTxt, pos);
	SetWindowTextW(hHeightValue, pszDest);
}

void CreateCheckBoxes(HWND hwnd)
{
	CreateWindowW( L"BUTTON", NULL, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 32+xcolor, 88+ycolor, 24,  20, hwnd, (HMENU)COLOR_1, NULL, NULL );
    CreateWindowW( L"BUTTON", NULL, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 82+xcolor, 88+ycolor, 24,  20, hwnd, (HMENU)COLOR_2, NULL, NULL );

	CreateWindowW( L"BUTTON", NULL, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 692+xcolor, 48+ycolor, 24,  20, hwnd, (HMENU)FILL, NULL, NULL );
    CreateWindowW( L"BUTTON", NULL, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 748+xcolor, 48+ycolor, 24,  20, hwnd, (HMENU)OUTLINE, NULL, NULL );
	CheckDlgButton( hwnd, FILL, BST_CHECKED );
	CheckDlgButton( hwnd, OUTLINE, BST_CHECKED );
	CheckDlgButton( hwnd, COLOR_1, BST_CHECKED );
}

void PourToTop(HWND hwnd, int xpos, int ypos, COLORREF startingcolor)
{
	HDC hdc = GetDC(hwnd);
	
	while(ypos>159 + ycolor && startingcolor == GetPixel(hdc, xpos, ypos-1))
	{
		SetPixelV(hdc, xpos, ypos - 1, RGB(Red[0], Green[0], Blue[0]));
		if(startingcolor == GetPixel(hdc, xpos + 1, ypos))
			{
				PourToRight(hwnd, xpos+1, ypos, startingcolor);
				SetPixelV(hdc, xpos+1, ypos, RGB(Red[0], Green[0], Blue[0]));
			}
		if(startingcolor == GetPixel(hdc, xpos - 1, ypos))
			{
				PourToLeft(hwnd, xpos-1, ypos, startingcolor);
				SetPixelV(hdc, xpos-1, ypos, RGB(Red[0], Green[0], Blue[0]));
			}
		if(startingcolor == GetPixel(hdc, xpos, ypos+1))
			{
				PourToBottom(hwnd, xpos, ypos+1, startingcolor);
				SetPixelV(hdc, xpos-1, ypos, RGB(Red[0], Green[0], Blue[0]));
			}
			
			Red[0]++;
			Red[0]++;
			if(Red[0]>255)
				Red[0]=0;
				Color2Pen = CreatePen( PS_SOLID, Thickness, RGB(Red[0], Green[0], Blue[0]));
		ypos--;
	}
	
	ReleaseDC( hwnd, hdc );
}

void PourToBottom(HWND hwnd, int xpos, int ypos, COLORREF startingcolor)
{
	HDC hdc = GetDC(hwnd);

	while(ypos < 160 + ycolor + ycanvas + ycolor && startingcolor == GetPixel(hdc, xpos, ypos+1))
	{
		SetPixel(hdc, xpos, ypos+1, RGB(Red[0], Green[0], Blue[0]));
		if(startingcolor == GetPixel(hdc, xpos + 1, ypos))
		{
			SetPixel(hdc, xpos+1, ypos, RGB(Red[0], Green[0], Blue[0]));
			PourToRight(hwnd, xpos+1, ypos, startingcolor);
		}
		if(startingcolor == GetPixel(hdc, xpos - 1, ypos))
		{
			SetPixel(hdc, xpos-1, ypos, RGB(Red[0], Green[0], Blue[0]));
			PourToLeft(hwnd, xpos-1, ypos, startingcolor);
		}
		if(startingcolor == GetPixel(hdc, xpos, ypos-1))
		{
			SetPixel(hdc, xpos, ypos-1, RGB(Red[0], Green[0], Blue[0]));
			PourToTop(hwnd, xpos, ypos-1, startingcolor);
		}
		ypos++;
	}
	
	ReleaseDC( hwnd, hdc );
}

void PourToRight(HWND hwnd, int xpos, int ypos, COLORREF startingcolor)
{
	HDC hdc = GetDC(hwnd);

	while(xpos< 23 + xcolor + xcanvas && startingcolor == GetPixel(hdc, xpos+1, ypos))
	{	
		SetPixelV(hdc, xpos+1, ypos, RGB(Red[0], Green[0], Blue[0]));
		if(startingcolor == GetPixel(hdc, xpos, ypos-1))
		{
			 PourToTop(hwnd, xpos, ypos-1, startingcolor);
			 SetPixelV(hdc, xpos, ypos-1, RGB(Red[0], Green[0], Blue[0]));
		}
		if(startingcolor == GetPixel(hdc, xpos, ypos+1))
		{
			PourToBottom(hwnd, xpos, ypos+1, startingcolor);
			SetPixelV(hdc, xpos, ypos+1, RGB(Red[0], Green[0], Blue[0]));
		}
		if(startingcolor == GetPixel(hdc, xpos - 1, ypos))
		{
			PourToLeft(hwnd, xpos-1, ypos, startingcolor);
			SetPixelV(hdc, xpos-1, ypos, RGB(Red[0], Green[0], Blue[0]));
		}
		xpos++;
	}
	
	ReleaseDC( hwnd, hdc );
}

void PourToLeft(HWND hwnd, int xpos, int ypos, COLORREF startingcolor)
{
	HDC hdc = GetDC(hwnd);

	while(xpos > 22 + xcolor && startingcolor == GetPixel(hdc, xpos-1, ypos))
	{
		SetPixelV(hdc, xpos-1, ypos, RGB(Red[0], Green[0], Blue[0]));
		if(startingcolor == GetPixel(hdc, xpos, ypos-1))
		{
			PourToTop(hwnd, xpos, ypos-1, startingcolor);
			SetPixelV(hdc, xpos, ypos-1, RGB(Red[0], Green[0], Blue[0]));
		}
		if(startingcolor == GetPixel(hdc, xpos, ypos+1))
		{
			PourToBottom(hwnd, xpos, ypos+1, startingcolor);
			SetPixelV(hdc, xpos, ypos+1, RGB(Red[0], Green[0], Blue[0]));
		}
		if(startingcolor == GetPixel(hdc, xpos + 1, ypos))
		{
			PourToRight(hwnd, xpos+1, ypos, startingcolor);
			SetPixelV(hdc, xpos+1, ypos, RGB(Red[0], Green[0], Blue[0]));
		}
		xpos--;
	}
	
	ReleaseDC( hwnd, hdc );
}

void SaveBitmapToFile(HBITMAP hBitmap, LPCTSTR FileName)
{
	HDC hDC;
	int Bits;
	WORD BitCount;
	DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
	BITMAP Bitmap0; // Bitmapa na któr¹ zapiszemy hBitmapê
	BITMAPFILEHEADER BitmapFileHeader;
	BITMAPINFOHEADER BitmapInfo; //Zawiera informacje o wymiarach i kolarach bitmapy
	LPBITMAPINFOHEADER lpbi;
	HANDLE hFile, hDib, hPalette, hOldPalette = NULL;
	
	hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	Bits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES); //Bity na piksel
	DeleteDC(hDC);
	if (Bits <= 1) //wBitcount musi zawieraæ siê w zbiorze: (0,1,4,8,16,24,32)
		BitCount = 1; // Przypadek monochromatyczny
	else if (Bits <= 4)
		BitCount = 4; // 2^4
	else if (Bits <= 8)
		BitCount = 8; // 2^8
	else
		BitCount = 24; // 2^24
		
	GetObject(hBitmap, sizeof(Bitmap0), (LPSTR)&Bitmap0); // Przepisujemy hBitmap na bitmapê
	//Wype³niamy strukturê nag³ówka
	BitmapInfo.biSize = sizeof(BITMAPINFOHEADER);
	BitmapInfo.biWidth = Bitmap0.bmWidth;
	BitmapInfo.biHeight = Bitmap0.bmHeight;
	BitmapInfo.biPlanes = 1;
	BitmapInfo.biBitCount = BitCount; //Liczba bitów potrzebna na piksel
	BitmapInfo.biCompression = BI_RGB; //Rodzaj kompresji. BI_RGB = brak kompresji.
	BitmapInfo.biSizeImage = 0;
	BitmapInfo.biXPelsPerMeter = 0;
	BitmapInfo.biYPelsPerMeter = 0;
	BitmapInfo.biClrImportant = 0;
	BitmapInfo.biClrUsed = 256; //Liczba indeksów w tabeli kolorów. (0,255)
	
	dwBmBitsSize = ((Bitmap0.bmWidth * BitCount + 31) & ~31) / 8 * Bitmap0.bmHeight; //Wyliczamy rozmiar bitmapy w bitach
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = BitmapInfo;

	hPalette = GetStockObject(DEFAULT_PALETTE); //Uzyskujemy uchwyt do palety
	if (hPalette) //Jeœli siê powiod³o, czyli wartoœæ jest inna od NULL
	{
		hDC = GetDC(NULL);
		hOldPalette = SelectPalette(hDC, (HPALETTE)hPalette, FALSE); //Wybieramy palete
		RealizePalette(hDC);
	}
 //GetDiBits pobiera bity i przypisuje je do bufora, do którego jest podany wskaŸnik. DIB_RGB_COLORS oznacza, ¿e kolory maj¹ byæ czytane jako RGB.The GetDIBits function retrieves the bits of the specified compatible bitmap and copies them into a buffer as a DIB using the specified format.
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap0.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
		+ dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);

	if (hOldPalette)
	{
		SelectPalette(hDC, (HPALETTE)hOldPalette, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}

	hFile = CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (hFile == INVALID_HANDLE_VALUE) //Jeœli nie uda³o siê stworzyæ pliku
		return FALSE;

	BitmapFileHeader.bfType = 0x4D42; // "BM" - tylko ta wartoœæ mo¿e byæ wprowadzona
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	BitmapFileHeader.bfSize = dwDIBSize; //Rozmiar bitmapy w bitach
	BitmapFileHeader.bfReserved1 = 0; // Musi byæ 0
	BitmapFileHeader.bfReserved2 = 0; // Musi byæ 0
	BitmapFileHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize; //Rozmiar bez bitów bitmapy

	WriteFile(hFile, (LPSTR)&BitmapFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); //Wpisujemy nag³ówek
	WriteFile(hFile, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL); //Wpisujemy bitmapê
	
	GlobalUnlock(hDib); // Sprz¹tamy
	GlobalFree(hDib);
	CloseHandle(hFile);
	
}

void PrintCurrentCanvas(HWND hwnd)
{
	int r = 0;
	HRESULT hResult;
	LPPRINTDLGEX pPDX = NULL;
	LPPRINTPAGERANGE pPageRanges = NULL;
	
	// Alokujemy strukturê dynamicznie
	pPDX =( LPPRINTDLGEX ) GlobalAlloc( GPTR, sizeof( PRINTDLGEX ) );
	if( !pPDX )
	     return E_OUTOFMEMORY;
	pPageRanges =( LPPRINTPAGERANGE ) GlobalAlloc( GPTR, sizeof( PRINTPAGERANGE ) );
	if( !pPageRanges )
	     return E_OUTOFMEMORY;
	 
	// Wype³nianie struktury    
	pPDX->lStructSize = sizeof( PRINTDLGEX );
	pPDX->hwndOwner = hwnd;
	pPDX->hDevMode = NULL; //Preferencje drukowania.
	pPDX->hDevNames = NULL;
	pPDX->hDC = NULL; // hDC zostaje zdefiniowane dopiero po wciœniêciu drukuj
	pPDX->Flags = PD_RETURNDC | PD_COLLATE;
	pPDX->Flags2 = 0;
	pPDX->ExclusionFlags = 0;
	pPDX->nPageRanges = 0;
	pPDX->nMaxPageRanges = 1;
	pPDX->lpPageRanges = pPageRanges;
	pPDX->nMinPage = 1;
	pPDX->nMaxPage = 1; //Maksymalnie jedna strona. Odpowiada to rysunkowi
	pPDX->nCopies = 1; //Domyœlnie jedna kopia
	pPDX->hInstance = 0;
	pPDX->lpPrintTemplateName = NULL;
	pPDX->lpCallback = NULL;
	pPDX->nPropertyPages = 0;
	pPDX->lphPropertyPages = NULL;
	pPDX->nStartPage = START_PAGE_GENERAL;
	pPDX->dwResultAction = 0;
	
	hResult = PrintDlgEx( pPDX ); //Wywo³ujemy dialog drukowania
	
	
		
	if( hResult == S_OK && pPDX->dwResultAction == PD_RESULT_PRINT ) //Jeœli u¿ytkownik klikn¹³ drukuj
	{
		DOCINFO docInfo;
		ZeroMemory( & docInfo, sizeof( docInfo ) );
		docInfo.cbSize = sizeof( docInfo );
		docInfo.lpszDocName = "Canvas";
		
		r = StartDoc( pPDX->hDC, & docInfo ); //StartDoc zaczyna drukowanie
		
		if( r != SP_ERROR )
		{
		    r = StartPage( pPDX->hDC );
		    
		    if( r > 0 )
		    {
		    	
			    BITMAP          bitmap;
			    HDC             hdcMem;
			    HGDIOBJ         oldBitmap;
			
			    hdcMem = CreateCompatibleDC(pPDX->hDC);
			    oldBitmap = SelectObject(hdcMem, hScreen[0]);
				GetObject(hScreen[0], sizeof(bitmap), &bitmap); // Bierzemy aktualny obraz na p³ótnie
	
				StretchBlt(pPDX->hDC, 300, 300, 2*xcanvas, 2*ycanvas, hdcMem, 0, 0, xcanvas, ycanvas, SRCCOPY); //Rozci¹gamy aby dopasow¹æ do kartki
			    SelectObject(hdcMem, oldBitmap);
			    DeleteDC(hdcMem);
			
				r = EndPage( pPDX->hDC ); //W przypadku gdyby coœ posz³o nie tak. EndPage zwraca wartoœæ 0 lub mniejsz¹. Nast¹pi to np. wtedy gdy skoñcza siê strony do drukowania
				
					if( r > 0 )
					    r = EndDoc( pPDX->hDC ); // Koñczy drukowanie.
			}
		}
	}
	
	//Czyœcimy
	if( pPDX->hDC )
	     DeleteDC( pPDX->hDC );
	
	if( pPDX->hDevMode )
	     GlobalFree( pPDX->hDevMode );
	
	if( pPDX->hDevNames )
	     GlobalFree( pPDX->hDevNames );
	
	GlobalFree( pPDX );
	GlobalFree( pPageRanges );
}



