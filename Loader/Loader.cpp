
#include <Windows.h>
#include <windowsx.h>

#include "C:\Users\Supreme1337\source\repos\Direct3D\Direct2D\DrawUtils.h"
#include "C:\Users\Supreme1337\source\repos\Direct3D\Direct2D\GuiEntities\CheckBox.h"
#include "C:\Users\Supreme1337\source\repos\Direct3D\Direct2D\GuiEntities\Button.h"
#include "C:\Users\Supreme1337\source\repos\Direct3D\Direct2D\GuiEntities\Slider.h"
#include "C:\Users\Supreme1337\source\repos\Direct3D\Direct2D\GuiEntities\CheckBox.h"
#include "C:\Users\Supreme1337\source\repos\Direct3D\Direct2D\GuiEntities\StaticSlider.h"
#include "C:\Users\Supreme1337\source\repos\Direct3D\Direct2D\GuiEntities\TextBox.h"
#include "C:\Users\Supreme1337\source\repos\Direct3D\Direct3D\Time.h"
#include "C:\Users\Supreme1337\source\repos\Direct3D\Direct3D\Logger.h"


#include <thread>
#include "resource.h"
#include "GraphicsLibrary/KeyBoard.h"
#include "GraphicsLibrary/Mouse.h"
//#include "GraphicsLibrary/MiscConversions.h"


class LogIn : public DrawUtils //singleton design
{

public:

	static  inline int LastButtonPushed = NULL;
	bool ShouldDrawCursor = false;

	mutable struct BitmapRects
	{
		D2D1_RECT_F KeyBmp;
		D2D1_RECT_F EmailBmp;
		D2D1_RECT_F FPSBmp;

	}BitmapRects;

	struct Buttons
	{
		static constexpr int Email = 1;
		static constexpr int Password = 2;
		static constexpr int SignIn = 3;
		static constexpr int SignUp = 4;
	}Buttons;

	mutable struct Strings
	{
		std::wstring EmailStr;
		std::wstring PasswordStr;
	}Strings;

	mutable struct TextRects
	{
		D2D1_RECT_F Title;
		D2D1_RECT_F Or;
		D2D1_RECT_F fps;
	}TextRects;

	HWND hwnd;
	LARGE_INTEGER p;

	Button<LogIn>* ButtonArray[2];
	Button<LogIn>* SignIn = ButtonArray[0];
	Button<LogIn>* NewUser = ButtonArray[1];


	TextBox<LogIn>* TextBoxArray[2];
	TextBox<LogIn>* EmailBox = TextBoxArray[0];
	TextBox<LogIn>* PasswordBox = TextBoxArray[1];


	CheckBox<LogIn>* checkbox;
	StaticSlider<LogIn>* slider;
	Slider<LogIn>* Slide;
	Time time;
	FLOAT RefereshRate;
	bool Frame = false;

	LogIn(HWND hWnd) : DrawUtils(hWnd)
	{
		LoadBitmapFromResource(IDB_PNG1, L"PNG" ,&Bitmaps.pBitmapEmail);
		LoadBitmapFromResource(IDB_PNG2, L"PNG", &Bitmaps.pBitmapKey);
		LoadBitmapFromResource(IDB_PNG3, L"PNG" ,&Bitmaps.pBitmapBgr);
		LoadBitmapFromResource(IDB_PNG4, L"PNG", &Bitmaps.pBitmapFPS);
		LoadBitmapFromResource(IDB_PNG5, L"PNG", &Bitmaps.pBitmapLoading);
		SignIn = new Button(this, Brushes.pBlackBrush, L"Sign In", WriteFormats.pTextFormatNormal, WriteFormats.pTextFormatSmaller, Brushes.pWhiteBrush, DWRITE_TEXT_ALIGNMENT_CENTER, 0);
		NewUser = new Button(this, Brushes.pBlackBrush, L"Sign up as a new user", WriteFormats.pTextFormatNormal, WriteFormats.pTextFormatSmaller, Brushes.pWhiteBrush, DWRITE_TEXT_ALIGNMENT_CENTER, 1);
		EmailBox = new TextBox(this, Brushes.pWhiteBrush, Brushes.pBlackBrush, Strings.EmailStr, L" -Email", WriteFormats.pTextFormatNormal, DWRITE_TEXT_ALIGNMENT_LEADING);
		PasswordBox = new TextBox(this, Brushes.pWhiteBrush, Brushes.pBlackBrush, Strings.PasswordStr, L" -Password", WriteFormats.pTextFormatNormal, DWRITE_TEXT_ALIGNMENT_LEADING);
		checkbox = new CheckBox(this, { 100,100,130,130 }, Brushes.pWhiteBrush, 2);
		slider = new StaticSlider(this, Brushes.pWhiteBrush, { 100,140,190,180 });
		Slide = new Slider(this, Brushes.pWhiteBrush, { 100,200,590,220 });
		this->hwnd = hWnd;
	}

	D2D1_RECT_F RectToD2DRect(RECT rect)
	{
		return { (FLOAT)rect.left , (FLOAT)rect.top , (FLOAT)rect.right , (FLOAT)rect.bottom };
	}

	void UpdateTextRects()
	{
		NewUser->rect = { width * 1 / 3,    height * 6 / 19,        width * 2 / 3,         height * 8 / 19 };
		SignIn->rect = { width * 1 / 3,    height * 13 / 17,       width * 2 / 3,         height * 16 / 19 };


		PasswordBox->rect_f = { width * 3 / 10,   height * 12 / 19,       width * 7 / 10 ,       height * 13 / 18 };
		PasswordBox->RectText = PasswordBox->rect_f;
		PasswordBox->RectText.left += 10;

		EmailBox->rect_f = { width * 3 / 10,   height * 21 / 40,       width * 7 / 10,        height * 11 / 18 };
		EmailBox->RectText = EmailBox->rect_f;
		EmailBox->RectText.left += 10;

		TextRects.Or = { width * 3 / 10,   height * 8 / 19,        width * 7 / 10,        height * 10 / 19 };
		TextRects.Title = { width * 3 / 10,   height * 2 / 19,        width * 7 / 10,        height * 3 / 19 };

		TextRects.fps = { BitmapRects.FPSBmp.left,BitmapRects.FPSBmp.top + 30,BitmapRects.FPSBmp.right,BitmapRects.FPSBmp.bottom + 30 };
	}
	void UpdateBitmapRects()
	{
		BitmapRects.KeyBmp = { width * 4 / 18,    height * 12 / 19,       width * 3 / 10 ,        height * 13 / 18 };
		BitmapRects.EmailBmp = { width * 2 / 9,     height * 10 / 20 + 8,   width * 3 / 11 + 8 ,    height * 12 / 19 };
		BitmapRects.FPSBmp = { width * 1 / 40, height - 70, width * 3 / 40,height - 25 };
	}
	void UpdateStrings()
	{
		if (LastButtonPushed == Buttons.SignIn)
		{
			Strings.EmailStr.clear();
			Strings.PasswordStr.clear();

			PasswordBox->IsActive = false;
			EmailBox->IsActive = false;
		}

		EmailBox->text = Strings.EmailStr;
		PasswordBox->text = Strings.PasswordStr;
	}
	void UpdateGUIEntities()
	{
		UpdateTextRects();
		UpdateBitmapRects();
		UpdateStrings();
	}

	void DrawTexts()
	{
		drawText(WriteFormats.pTextFormatNormal, L"or", TextRects.Or, Brushes.pWhiteBrush, DWRITE_TEXT_ALIGNMENT_CENTER);
		drawText(WriteFormats.pTextFormatHugeItalic, L"Yamasu Cheats", TextRects.Title, Brushes.pWhiteBrush, DWRITE_TEXT_ALIGNMENT_CENTER);
		drawText(WriteFormats.pTextFormatNormal, std::to_wstring(Fps), TextRects.fps, Brushes.pWhiteBrush, DWRITE_TEXT_ALIGNMENT_CENTER);
	}
	void DrawBitmaps()
	{
		pRenderTarget->FillRectangle(&RectToD2DRect(rc), Brushes.pLinearGradientBrush);
		++i;
		pRenderTarget->DrawBitmap(Bitmaps.pBitmapBgr, RectToD2DRect(rc), 0.15);

		D2D1_POINT_2F imageCenter = D2D1::Point2F(width / 2, height / 2);
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(i, imageCenter));
		pRenderTarget->DrawBitmap(Bitmaps.pBitmapLoading, { width/2 - 100,height/2 - 100,width/2 + 100,height /2 +  100}, 1);

		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(0, imageCenter));
		pRenderTarget->DrawBitmap(Bitmaps.pBitmapKey, BitmapRects.KeyBmp, 1);
		pRenderTarget->DrawBitmap(Bitmaps.pBitmapFPS, BitmapRects.FPSBmp, 1);

		pRenderTarget->DrawBitmap(Bitmaps.pBitmapEmail, BitmapRects.EmailBmp, 1);
	}
	void DrawButtons()
	{
		SignIn->DrawButton(5.f, 5.f);
		NewUser->DrawButton(5.f, 5.f);
	}
	void DrawTextBoxes()
	{
		EmailBox->DrawBox(5.f, 5.f);
		PasswordBox->DrawBox(5.f, 5.f);
	}
	void DrawTextCursor()
	{
		switch (LastButtonPushed)
		{
		case 1:
		{
			if (ShouldDrawCursor && EmailBox->text.empty())
			{
				EmailBox->DrawCursor();
			}
			break;
		}

		case 2:
		{
			if (ShouldDrawCursor && PasswordBox->text.empty())
			{
				PasswordBox->DrawCursor();
			}
			break;
		}
		}
	}
	int i = 0;
	void Draw()
	{
		SetFps();
		pRenderTarget->BeginDraw();
		pRenderTarget->FillRectangle(&RectToD2DRect(rc), Brushes.pLinearGradientBrush);
		DrawBitmaps();
		DrawTexts();
		DrawButtons();
		DrawTextBoxes();
		DrawTextCursor();
		checkbox->DrawCheckBox();
		slider->DrawSlider();
		Slide->DrawSlider(hwnd);
		D2D1_POINT_2F imageCenter = { 250,250 };//D2D1::Point2F(width / 2, height / 2);
		i += 2;
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(i, imageCenter));
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(0, imageCenter));
		pRenderTarget->EndDraw();
	}


	void CursorHandler(HWND hwnd)
	{
		if (CursorBelongsToRect(hwnd, SignIn->rect) || CursorBelongsToRect(hwnd, NewUser->rect))
		{
			SetCursor(Cursors.hCursorHand);
			return;
		}
		SetCursor(Cursors.hCursorArrow);
	}
	void CheckLogInClicks(HWND hWnd)
	{
		if (CursorBelongsToRect(hWnd, EmailBox->rect_f))
		{
			LastButtonPushed = Buttons.Email;
			EmailBox->IsActive = true;
			PasswordBox->IsActive = false;
		}

		else if (CursorBelongsToRect(hWnd, PasswordBox->rect_f))
		{
			LastButtonPushed = Buttons.Password;
			PasswordBox->IsActive = true;
			EmailBox->IsActive = false;
		}

		else if (CursorBelongsToRect(hWnd, NewUser->rect))
		{
			LastButtonPushed = Buttons.SignUp;
			NewUser->ShouldAnimate = true;
			HINSTANCE result = ShellExecuteA(0, NULL, "http://www.yamasu.co/register", NULL, NULL, SW_SHOWNORMAL);
		}

		else if (CursorBelongsToRect(hWnd, SignIn->rect))
		{
			LastButtonPushed = Buttons.SignIn;
			SignIn->ShouldAnimate = true;
		}

		else
		{
			LastButtonPushed = 0;
			EmailBox->IsActive = false;
			PasswordBox->IsActive = false;
		}
	}
};


std::shared_ptr<LogIn> LogInPtr;


LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case  WM_LBUTTONDOWN:

		LogInPtr->CheckLogInClicks(hWnd);
		break;
	
	case WM_LBUTTONUP:

		if (LogInPtr->NewUser->ShouldAnimate)
		{
			LogInPtr->NewUser->ShouldAnimate = false;
		}

		else if (LogInPtr->SignIn->ShouldAnimate)
		{
			LogInPtr->SignIn->ShouldAnimate = false;
		}

	case WM_CHAR:

		switch (LogInPtr->LastButtonPushed)
		{
		case 1:
		{
			if (wParam == VK_BACK && !LogInPtr->Strings.EmailStr.empty())
			{
				LogInPtr->RemoveChar(LogInPtr->Strings.EmailStr);
				break;
			}

			LogInPtr->PushChar((wchar_t)wParam, LogInPtr->Strings.EmailStr, 40);
			break;
		}

		case 2:
		{
			if (wParam == VK_BACK && !LogInPtr->Strings.PasswordStr.empty())
			{
				LogInPtr->RemoveChar(LogInPtr->Strings.PasswordStr);
				break;
			}

			LogInPtr->PushChar((wchar_t)wParam, LogInPtr->Strings.PasswordStr, 40);
			break;
		}
		}
		break;

	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_RETURN:

			LogInPtr->LastButtonPushed = LogInPtr->Buttons.SignIn;
			break;
		}

	case WM_MOUSEMOVE:

		LogInPtr->Points.point = { GET_X_LPARAM(lParam) ,GET_Y_LPARAM(lParam) };
		LogInPtr->CursorHandler(hWnd);
		break;


	case WM_PAINT:

		LogInPtr->UpdateBasicRsrces(hWnd);
		LogInPtr->UpdateGUIEntities();
		LogInPtr->Draw();
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		break;

	default:

		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
}



int CALLBACK  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdSHow)
{
	std::wstring ClassName = L"LoaderClass";
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCWSTR)&ClassName;
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindowEx(0, (LPCWSTR)&ClassName, L"Yamasu Loader v1.0", WS_OVERLAPPEDWINDOW, 600, 300, 1100, 700, nullptr, nullptr, hInstance, nullptr);
	LogInPtr = std::make_shared<LogIn>(hWnd);

	//IntializeObjects(hWnd);
	//MemoryPtr->MapSharedMemory();

	std::thread CursorTiming([&]()
		{
			while (1)
			{
				LogInPtr->ShouldDrawCursor = !LogInPtr->ShouldDrawCursor;
				Sleep(500);
			}
		});



	ShowWindow(hWnd, SW_SHOW);
	SetFocus(hWnd);
	MSG msg;

	while (1)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) 
			{ break; }

			UpdateWindow(hWnd);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			Sleep(3);
		}
		break;
	}
	UnregisterClass(ClassName.c_str(), hInstance);
	return 0;
}

