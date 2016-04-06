////////////////////////////////////////////////////////////////////////////
// win.cpp -- Hiragana Calculator Calc-H for Windows
// This file is part of Calc-H.  See file "ReadMe.txt" and "License.txt".
////////////////////////////////////////////////////////////////////////////
// (Japanese, Shift_JIS)
#include "stdafx.h"

HINSTANCE   ch_hInstance = NULL;
HWND        ch_hMainWnd = NULL;
WNDPROC     ch_fnOldEditWndProc = NULL;
MResizable  ch_resizable;
HBRUSH      ch_hbrBack = NULL;
bool        ch_is_running = false;

std::vector<std::string> ch_history;
size_t              ch_history_index = 0;
size_t              ch_history_count = 0;

// voice
shared_ptr<WinVoice> ch_voice;

// mute
HBITMAP ch_hbmMute = NULL;
HBITMAP ch_hbmMuteOff = NULL;

// hook for Ctrl+A
HHOOK ch_hCtrlAHook = NULL;

// hook proc for Ctrl+A
extern "C"
LRESULT CALLBACK ChCtrlAMessageProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0)
        return ::CallNextHookEx(ch_hCtrlAHook, nCode, wParam, lParam);

    MSG *pMsg = reinterpret_cast<MSG *>(lParam);
    CHAR szClassName[64];

    if (pMsg->message == WM_KEYDOWN)
    {
        if (int(pMsg->wParam) == 'A' &&
            ::GetAsyncKeyState(VK_CONTROL) < 0 &&
            ::GetAsyncKeyState(VK_SHIFT) >= 0 &&
            ::GetAsyncKeyState(VK_MENU) >= 0)
        {
            // Ctrl+A is pressed
            HWND hwnd = ::GetFocus();
            if (hwnd != NULL)
            {
                ::GetClassNameA(hwnd, szClassName, 64);
                if (::lstrcmpiA(szClassName, TEXT("EDIT")) == 0)
                {
                    ::SendMessageA(hwnd, EM_SETSEL, 0, -1);
                    return 1;
                }
            }
        }
    }

    return ::CallNextHookEx(ch_hCtrlAHook, nCode, wParam, lParam);
}

extern "C"
LRESULT CALLBACK
ChEditWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result;
    static BOOL s_flag = TRUE;
    assert(ch_fnOldEditWndProc);

    switch (uMsg)
    {
    case WM_IME_STARTCOMPOSITION:
        s_flag = FALSE;
        result = ::CallWindowProc(ch_fnOldEditWndProc, hwnd, uMsg, wParam, lParam);
        InvalidateRect(hwnd, NULL, FALSE);
        return result;

    case WM_IME_ENDCOMPOSITION:
        s_flag = TRUE;
        result = ::CallWindowProc(ch_fnOldEditWndProc, hwnd, uMsg, wParam, lParam);
        InvalidateRect(hwnd, NULL, FALSE);
        return result;

    case WM_KEYDOWN:
        if (s_flag)
        {
            switch (wParam)
            {
            case VK_UP:
                if (1 <= ch_history_index)
                {
                    --ch_history_index;
                    std::string& str = ch_history[ch_history_index];
                    ::SetWindowTextA(hwnd, str.c_str());
                    ::SendMessageA(hwnd, EM_SETSEL, int(str.size()), int(str.size()));
                }
                return 0;

            case VK_DOWN:
                if (ch_history_index + 1 < ch_history_count)
                {
                    ++ch_history_index;
                    std::string& str = ch_history[ch_history_index];
                    ::SetWindowTextA(hwnd, str.c_str());
                    ::SendMessageA(hwnd, EM_SETSEL, int(str.size()), int(str.size()));
                }
                else
                {
                    if (ch_history_index < ch_history_count)
                        ++ch_history_index;
                    ::SetWindowTextA(hwnd, NULL);
                    ::SendMessageA(hwnd, EM_SETSEL, 0, 0);
                }
                return 0;
            }
        }
        result = ::CallWindowProc(ch_fnOldEditWndProc, hwnd, uMsg, wParam, lParam);
        return result;

    case WM_CHAR:
    case WM_PAINT:
        result = ::CallWindowProc(ch_fnOldEditWndProc, hwnd, uMsg, wParam, lParam);
        if (::GetWindowTextLengthA(hwnd) == 0 && s_flag)
        {
            RECT rcClient;
            ::GetClientRect(hwnd, &rcClient);
            HDC hdc = ::GetDC(hwnd);
            ::SetTextColor(hdc, ::GetSysColor(COLOR_GRAYTEXT));
            ::SetBkColor(hdc, ::GetSysColor(COLOR_WINDOW));
            ::DrawTextA(hdc, "ひらがなで入力して下さい", -1,
                        &rcClient, DT_LEFT | DT_VCENTER);
            ReleaseDC(hwnd, hdc);
        }
        return result;

    default:
        return ::CallWindowProc(ch_fnOldEditWndProc, hwnd, uMsg, wParam, lParam);
    }
}

static const size_t ch_max_size = 16777216;

void ChAddOutput(HWND hwnd, const char *text)
{
    std::string str(text);
    ChReplaceString(str, "\n", "\r\n");
    ChReplaceString(str, "\r\r", "\r");
    if (str.size() > ch_max_size)
    {
        size_t excess = str.size() - ch_max_size;
        size_t i = 0, j;
        do
        {
            j = str.find("\r\n", i + 1);
            if (j == std::string::npos)
                break;
            i = j;
        } while (j < excess);
        str = str.substr(i + 2);
    }
    ::SendDlgItemMessageA(hwnd, edt1, WM_SETREDRAW, FALSE, 0);
    int cch = ::GetWindowTextLengthA(::GetDlgItem(hwnd, edt1));
    ::SendDlgItemMessageA(hwnd, edt1, EM_SETSEL, cch, cch);
    ::SendDlgItemMessageA(hwnd, edt1, EM_REPLACESEL, FALSE,
        reinterpret_cast<LPARAM>(str.c_str()));
    cch = ::GetWindowTextLengthA(::GetDlgItem(hwnd, edt1));
    ::SendDlgItemMessageA(hwnd, edt1, EM_SETSEL, cch, cch);
    int lines = ::SendDlgItemMessageA(hwnd, edt1, EM_GETLINECOUNT, 0, 0);
    ::SendDlgItemMessageA(hwnd, edt1, EM_LINESCROLL, 0, lines);
    ::SendDlgItemMessageA(hwnd, edt1, WM_SETREDRAW, TRUE, 0);
}

BOOL ChOnInitDialog(HWND hwnd)
{
    ch_hMainWnd = hwnd;

    std::string contents;
    contents += ch_logo;
    contents += "\n";
    contents += ch_feature;
    if (ch_voice && ch_voice->IsAvailable()) {
        ch_voice->Speak(ch_feature);
    }
    ChAddOutput(hwnd, contents.c_str());

    ch_resizable.OnParentCreate(hwnd, TRUE);
    ch_resizable.SetLayoutAnchor(edt1, mzcLA_TOP_LEFT, mzcLA_BOTTOM_RIGHT);
    ch_resizable.SetLayoutAnchor(stc1, mzcLA_BOTTOM_LEFT);
    ch_resizable.SetLayoutAnchor(edt2, mzcLA_BOTTOM_LEFT, mzcLA_BOTTOM_RIGHT);
    ch_resizable.SetLayoutAnchor(IDOK, mzcLA_BOTTOM_RIGHT);
    ch_resizable.SetLayoutAnchor(psh1, mzcLA_BOTTOM_LEFT);

    ch_hbmMute = LoadBitmap(ch_hInstance, MAKEINTRESOURCE(100));
    ch_hbmMuteOff = LoadBitmap(ch_hInstance, MAKEINTRESOURCE(101));
    HBITMAP hbm;
    if (ch_voice->IsMute()) {
        hbm = ch_hbmMute;
    } else {
        hbm = ch_hbmMuteOff;
    }
    SendDlgItemMessageA(hwnd, psh1, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm);

    HICON hIcon;
    hIcon = ::LoadIcon(ch_hInstance, MAKEINTRESOURCE(1));
    ::SendMessageA(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));
    hIcon = reinterpret_cast<HICON>(
        ::LoadImageA(ch_hInstance, MAKEINTRESOURCE(1), IMAGE_ICON,
            ::GetSystemMetrics(SM_CXSMICON),
            ::GetSystemMetrics(SM_CYSMICON), 0));
    ::SendMessageA(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));

    ::SendDlgItemMessageA(hwnd, edt2, EM_SETLIMITTEXT, 1000, 0);

    HWND hwndEdt2 = ::GetDlgItem(hwnd, edt2);
    ch_fnOldEditWndProc =
        reinterpret_cast<WNDPROC>(
            ::SetWindowLongPtr(hwndEdt2, GWLP_WNDPROC,
                reinterpret_cast<LONG_PTR>(ChEditWndProc)));

    ::SetWindowPos(hwnd, NULL, 0, 0, 640, 360,
        SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER |
        SWP_NOREDRAW | SWP_NOZORDER);
    ::SendMessageA(hwnd, DM_REPOSITION, 0, 0);

    ::SetFocus(::GetDlgItem(hwnd, edt2));

    return FALSE;
}

void ChOnExit(HWND hwnd)
{
    ::DeleteObject(ch_hbrBack);
    ch_hbrBack = NULL;

    HWND hwndEdt2 = ::GetDlgItem(hwnd, edt2);
    ::SetWindowLongPtr(hwndEdt2, GWLP_WNDPROC,
                       reinterpret_cast<LONG_PTR>(ch_fnOldEditWndProc));
    ch_fnOldEditWndProc = NULL;
}


unsigned __stdcall CalcThreadProc(void *p)
{
    HWND hwnd = reinterpret_cast<HWND>(p);
    ::EnableWindow(::GetDlgItem(hwnd, IDOK), FALSE);

    CHAR buffer[1024];
    ::GetDlgItemTextA(hwnd, edt2, buffer, 1024);
    ::SetDlgItemTextA(hwnd, edt2, "計算中です...");
    ::EnableWindow(::GetDlgItem(hwnd, edt2), FALSE);

    std::string str(buffer);
    std::string query = str;
    CrTrimString(query);
    if (query.empty())
        return TRUE;

    ch_history.push_back(query);
    ch_history_index = ch_history_count = ch_history.size();

    std::string result;
    if (query.find("しゃべれ") != std::string::npos ||
        query.find("こえをだ") != std::string::npos ||
        query.find("おとをだ") != std::string::npos ||
        query.find("みゅーとかいじょ") != std::string::npos ||
        query.find("みゅーとをかいじょ") != std::string::npos)
    {
        if (ch_voice && ch_voice->IsAvailable()) {
            ch_voice->SetMute(FALSE);
        }
        result = "こたえ：みゅーとをかいじょしました。";
    } else if (query.find("だまれ") != std::string::npos ||
        query.find("しずかに") != std::string::npos ||
        query.find("おとをけ") != std::string::npos ||
        query.find("こえをけ") != std::string::npos ||
        query.find("しょうおん") != std::string::npos ||
        query.find("みゅーと") != std::string::npos)
    {
        if (ch_voice && ch_voice->IsAvailable()) {
            ch_voice->SetMute(TRUE);
        }
        result = "こたえ：みゅーとしました。";
    } else {
        result = ChJustDoIt(query);
    }

    std::string result_copy = result;
    ChFixResultForDisplay(result);

    std::string contents("にゅうりょく：");
    contents += str;
    contents += '\n';
    contents += result;
    contents += '\n';
    ChAddOutput(hwnd, contents.c_str());

    ::SetDlgItemTextA(hwnd, edt2, NULL);
    ::EnableWindow(::GetDlgItem(hwnd, edt2), TRUE);
    ::EnableWindow(::GetDlgItem(hwnd, IDOK), TRUE);

    result = result_copy;
    ChFixResultForVoice(result);

    if (result.find("しゅうりょうします") != std::string::npos)
    {
        ::SetDlgItemTextA(hwnd, edt2, "終了中です...");
        ::EnableWindow(::GetDlgItem(hwnd, edt2), FALSE);
        if (ch_voice && ch_voice->IsAvailable()) {
            ch_voice->Speak(result);
            ch_voice->WaitUntilDone(3000);
        }
        ChOnExit(hwnd);
        ::EndDialog(hwnd, IDCANCEL);
    }
    else 
    {
        if (ch_voice && ch_voice->IsAvailable()) {
            ch_voice->Speak(result);
        }
    }

    ch_is_running = false;

    return 0;
}

void ChOnOK(HWND hwnd)
{
    if (ch_is_running)
        return;

    ch_is_running = true;
    HANDLE hCalcThread = reinterpret_cast<HANDLE>(
        _beginthreadex(NULL, 0, CalcThreadProc, hwnd, 0, NULL));
    ::CloseHandle(hCalcThread);
}

void ChOnSize(HWND hwnd)
{
    ch_resizable.OnSize();
}

extern "C"
INT_PTR CALLBACK 
ChDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static COLORREF s_clrBack = RGB(255, 255, 192);
    static COLORREF s_clrFore = RGB(0, 0, 0);

    switch (uMsg)
    {
    case WM_INITDIALOG:
        if (::GetSysColor(COLOR_WINDOW) == RGB(0, 0, 0))
        {
            s_clrBack = RGB(0, 0, 0);
            s_clrFore = ::GetSysColor(COLOR_WINDOWTEXT);
        }
        else
        {
            s_clrBack = RGB(255, 255, 192);
            s_clrFore = RGB(0, 0, 0);
        }
        ch_hbrBack = ::CreateSolidBrush(s_clrBack);
        return ChOnInitDialog(hwnd);

    case WM_SIZE:
        ChOnSize(hwnd);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            ChOnOK(hwnd);
            break;

        case IDCANCEL:
            ChOnExit(hwnd);
            ::EndDialog(hwnd, IDCANCEL);
            break;

        case psh1:
            if (HIWORD(wParam) == BN_CLICKED) {
                if (ch_voice && ch_voice->IsAvailable()) {
                    if (ch_voice->IsMute()) {
                        ch_voice->SetMute(FALSE);
                        SendDlgItemMessageA(hwnd, psh1, BM_SETIMAGE,
                            IMAGE_BITMAP, (LPARAM)ch_hbmMuteOff);
                    } else {
                        ch_voice->SetMute(TRUE);
                        SendDlgItemMessageA(hwnd, psh1, BM_SETIMAGE,
                            IMAGE_BITMAP, (LPARAM)ch_hbmMute);
                    }
                }
            }
        }
        break;

    case WM_SYSCOLORCHANGE:
        if (::GetSysColor(COLOR_WINDOW) == RGB(0, 0, 0))
        {
            s_clrBack = RGB(0, 0, 0);
            s_clrFore = ::GetSysColor(COLOR_WINDOWTEXT);
        }
        else
        {
            s_clrBack = RGB(255, 255, 192);
            s_clrFore = RGB(0, 0, 0);
        }
        ::DeleteObject(ch_hbrBack);
        ch_hbrBack = ::CreateSolidBrush(s_clrBack);
        break;

    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORSTATIC:
        {
            HWND hwndCtrl = reinterpret_cast<HWND>(lParam);
            if (hwndCtrl == ::GetDlgItem(hwnd, edt1))
            {
                HDC hdc = reinterpret_cast<HDC>(wParam);
                ::SetTextColor(hdc, s_clrFore);
                ::SetBkColor(hdc, s_clrBack);
                return reinterpret_cast<INT_PTR>(ch_hbrBack);
            }
        }
        break;
    }
    return 0;
}

extern "C"
int WINAPI WinMain(
    HINSTANCE   hInstance,
    HINSTANCE   hPrevInstance,
    LPSTR       lpCmdLine,
    INT         nCmdShow)
{
    ch_hInstance = hInstance;

    HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
    {
        if (SUCCEEDED(hr)) {
            ch_voice = make_shared<WinVoice>();
        }

        ch_hCtrlAHook = ::SetWindowsHookEx(WH_MSGFILTER,
            ChCtrlAMessageProc, NULL, ::GetCurrentThreadId());

        ::DialogBox(hInstance, MAKEINTRESOURCE(1), NULL, ChDialogProc);

        ::UnhookWindowsHookEx(ch_hCtrlAHook);
        ch_hCtrlAHook = NULL;
    }
    if (SUCCEEDED(hr)) {
        if (ch_voice) {
            ch_voice.reset();
        }
        ::CoUninitialize();
    }

    return 0;
}
