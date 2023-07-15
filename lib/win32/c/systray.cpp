#ifndef EXE
#define UNICODE
#endif

#include <windows.h>
#include <map>
#include <vector>

#ifdef SYSTRAY_IMPORT
#define DLLIF __declspec(dllimport)
#else
#define DLLIF extern "C" __declspec(dllexport) 
#endif

typedef void (*Action)();

DLLIF void Systray(const wchar_t* tip, const wchar_t* icon_path, void (*Action)());
DLLIF void Quit();
DLLIF void* MenuPopup(const wchar_t* text, void (*Action)());
DLLIF void* MenuItem(const wchar_t* text, void (*Action)());
DLLIF void* MenuSeparator();

static Action _MenuCreateCallbackType;
static std::vector<HMENU> vecHMENU;
static std::map<int, Action> map_MenuCreateCallbackType;
static std::map<HMENU, int> s_counter;
static int menu_id = 100;
static HWND s_hwnd = 0;
#define ID_MYTRAY (WM_APP + 1)
#define MYTRAY_MESSAGE (WM_APP + 2)

DLLIF void Quit()
{
	PostMessage(s_hwnd, WM_QUIT, 0, 0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {

	if (msg == WM_COMMAND)
	{
		auto fn = map_MenuCreateCallbackType[LOWORD(wp)];
		if (fn) {
			fn();
		}
	}
	if (msg == MYTRAY_MESSAGE) {
		if (lp == WM_RBUTTONUP)
		{
			s_counter.clear();
			menu_id = 0;
			vecHMENU.clear();

			auto hMenu = CreateMenu();
			vecHMENU.push_back(hMenu);
			s_counter[hMenu] = 0;
			HMENU sub = (HMENU)MenuPopup(L"", []() {
				_MenuCreateCallbackType();
			});

			POINT pt;
			GetCursorPos(&pt);
			SetForegroundWindow(hwnd);
			TrackPopupMenu(sub, TPM_NONOTIFY | TPM_LEFTALIGN | TPM_TOPALIGN, pt.x, pt.y, 0, hwnd, 0);
		}
	}

	return DefWindowProc(hwnd, msg, wp, lp);
}

// ウインドウ作成
// ウインドウプロシージャ
// コールバック登録

void Systray(const wchar_t* tip, const wchar_t* icon_path, Action mcb)
{
	_MenuCreateCallbackType = mcb;
	HWND hwnd;
	WNDCLASS winc;

	winc.style = CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc = WndProc;
	winc.cbClsExtra = winc.cbWndExtra = 0;
	winc.hInstance = 0;
	winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winc.hCursor = LoadCursor(NULL, IDC_ARROW);
	winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName = NULL;
	winc.lpszClassName = TEXT("KITTY");

	if (!RegisterClass(&winc)) return;

	hwnd = CreateWindow(
		TEXT("KITTY"), TEXT("Kitty on your lap"),
		WS_OVERLAPPEDWINDOW | ~WS_VISIBLE,
		100, 100, 200, 200, NULL, NULL,
		0, NULL
	);

	HICON hicon = 0;
	if (icon_path != 0) {
		//hicon = LoadIcon(0, icon_path);
		hicon = (HICON)LoadImageW(0, icon_path, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE);
		int n = GetLastError();
		int nn = 0;
	}
	else {
		hicon = ExtractIcon(0, L"C:\\WINDOWS\\SYSTEM32\\INETCPL.CPL", 21); // # Green tick
	}
	
	NOTIFYICONDATA dt;

	dt.cbSize = sizeof(dt);
	dt.hWnd = (HWND)hwnd;
	dt.uID = ID_MYTRAY;
	dt.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	dt.hIcon = hicon; // @icon;
	dt.uCallbackMessage = MYTRAY_MESSAGE;
	wcscpy_s(dt.szTip, tip);

	Shell_NotifyIcon(NIM_ADD, &dt);

	MSG msg;
	BOOL bRet;

	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	Shell_NotifyIcon(NIM_DELETE, &dt);

	return;
}

DLLIF void* MenuPopup(const wchar_t* text, Action mcb)
{
	auto hMenu = CreateMenu();
	s_counter[hMenu] = 0;
	vecHMENU.push_back(hMenu);
	mcb();
	vecHMENU.pop_back();

	MENUITEMINFO mii;
	::ZeroMemory(&mii, sizeof(mii));
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_TYPE | MIIM_ID | MIIM_STATE;
	mii.fMask |= MIIM_SUBMENU; // if @menu
	mii.fType = MFT_STRING;
	mii.hSubMenu = hMenu; //if @menu
	mii.fState = 0; ///@check ? MFS_CHECKED : 0;
	mii.wID = menu_id++;
	mii.dwTypeData = (LPWSTR)text;

	auto parent = vecHMENU.back();
	
	InsertMenuItem(parent, s_counter[parent], 1, &mii);
	s_counter[parent] += 1;

	return hMenu;
}

DLLIF void* MenuItem(const wchar_t* text, Action mcb)
{
	map_MenuCreateCallbackType[menu_id] = mcb;

	MENUITEMINFO mii;
	::ZeroMemory(&mii, sizeof(mii));
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_TYPE | MIIM_ID | MIIM_STATE;
	//mii.fMask |= MIIM_SUBMENU; // if @menu
	mii.fType = MFT_STRING;
	//mii.hSubMenu = hMenu; //if @menu
	mii.fState = 0; ///@check ? MFS_CHECKED : 0;
	mii.wID = menu_id++;
	mii.dwTypeData = (LPWSTR)text;

	auto parent = vecHMENU.back();

	InsertMenuItem(parent, s_counter[parent], 1, &mii);
	s_counter[parent] += 1;

	return 0;
}

DLLIF void* MenuSeparator()
{
	MENUITEMINFO mii;
	::ZeroMemory(&mii, sizeof(mii));
	mii.fMask = MIIM_TYPE;
	mii.cbSize = sizeof(mii);
	mii.fType = MF_SEPARATOR;

	auto parent = vecHMENU.back();
	InsertMenuItem(parent, s_counter[parent], 1, &mii);
	s_counter[parent] += 1;

	return 0;
}

#ifdef EXE
static void MenuCreateCallbackTypeFn()
{
	MenuPopup(L"テスト", []() {
		MenuItem(L"テスト", []() {
			int n = 0;
			});
		});
	MenuItem(L"テスト2", []() {
		int n = 0;
		});
	MenuSeparator();
	MenuItem(L"終了", []() {
		int n = 0;
		Quit();
		});
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow)
{
	Systray(L"tip", L"pnow.ico", MenuCreateCallbackTypeFn);
	
	return 0;
}

#endif
