// (Japanese, Shift_JIS)
#include "stdafx.h"

HINSTANCE   ch_hInstance = NULL;
HWND        ch_hMainWnd = NULL;
WNDPROC     ch_fnOldEditWndProc = NULL;
MResizable  ch_resizable;
HBRUSH      ch_hbrBack = NULL;

std::vector<std::string> ch_history;
std::size_t              ch_history_index = 0;
std::size_t              ch_history_count = 0;

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
            ::DrawTextA(hdc, "�Ђ炪�Ȃœ��͂��ĉ�����", -1,
                        &rcClient, DT_LEFT | DT_VCENTER);
            ReleaseDC(hwnd, hdc);
        }
        return result;

    default:
        return ::CallWindowProc(ch_fnOldEditWndProc, hwnd, uMsg, wParam, lParam);
    }
}

void ChAddOutput(HWND hwnd, const char *text)
{
    std::string str(text);
    ChReplaceString(str, "\n", "\r\n");
    ChReplaceString(str, "\r\r", "\r");
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
    ChAddOutput(hwnd, contents.c_str());
    contents = ch_comment;
    contents += "\n\n";
    ChAddOutput(hwnd, contents.c_str());

    ch_resizable.OnParentCreate(hwnd, TRUE);
    ch_resizable.SetLayoutAnchor(edt1, mzcLA_TOP_LEFT, mzcLA_BOTTOM_RIGHT);
    ch_resizable.SetLayoutAnchor(stc1, mzcLA_BOTTOM_LEFT);
    ch_resizable.SetLayoutAnchor(edt2, mzcLA_BOTTOM_LEFT, mzcLA_BOTTOM_RIGHT);
    ch_resizable.SetLayoutAnchor(IDOK, mzcLA_BOTTOM_RIGHT);

    HICON hIcon;
    hIcon = ::LoadIcon(ch_hInstance, MAKEINTRESOURCE(1));
    ::SendMessageA(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));
    hIcon = reinterpret_cast<HICON>(
        ::LoadImageA(ch_hInstance, MAKEINTRESOURCE(1), IMAGE_ICON,
            ::GetSystemMetrics(SM_CXSMICON),
            ::GetSystemMetrics(SM_CYSMICON), 0));
    ::SendMessageA(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));

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

BOOL ChOnOK(HWND hwnd)
{
    CHAR buffer[1024];
    ::GetDlgItemTextA(hwnd, edt2, buffer, 1024);

    std::string str(buffer);

    std::string query = str;
    CrTrimString(query);
    if (query.empty())
        return TRUE;

    ch_history.push_back(query);
    ch_history_index = ch_history_count = ch_history.size();

    std::string result = ChJustDoIt(query);

    std::string contents("�ɂイ��傭�F");
    contents += str;
    contents += '\n';
    contents += result;
    contents += '\n';
    ChAddOutput(hwnd, contents.c_str());

    if (result.find("���イ��傤���܂�") != std::string::npos)
        return FALSE;

    ::SetDlgItemTextA(hwnd, edt2, NULL);

    return TRUE;
}

void ChOnSize(HWND hwnd)
{
    ch_resizable.OnSize();
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

extern "C"
INT_PTR CALLBACK 
ChDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    const COLORREF clrBack = RGB(255, 255, 192);
    const COLORREF clrFore = RGB(0, 0, 0);

    switch (uMsg)
    {
    case WM_INITDIALOG:
        ch_hbrBack = ::CreateSolidBrush(clrBack);
        return ChOnInitDialog(hwnd);

    case WM_SIZE:
        ChOnSize(hwnd);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            if (!ChOnOK(hwnd))
            {
                ChOnExit(hwnd);
                ::EndDialog(hwnd, IDOK);
            }
            break;

        case IDCANCEL:
            ChOnExit(hwnd);
            ::EndDialog(hwnd, IDCANCEL);
            break;
        }

    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORSTATIC:
        {
            HWND hwndCtrl = reinterpret_cast<HWND>(lParam);
            if (hwndCtrl == ::GetDlgItem(hwnd, edt1))
            {
                HDC hdc = reinterpret_cast<HDC>(wParam);
                ::SetTextColor(hdc, clrFore);
                ::SetBkColor(hdc, clrBack);
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

    ch_hCtrlAHook = ::SetWindowsHookEx(WH_MSGFILTER,
        ChCtrlAMessageProc, NULL, ::GetCurrentThreadId());

    ::DialogBox(hInstance, MAKEINTRESOURCE(1), NULL, ChDialogProc);

    ::UnhookWindowsHookEx(ch_hCtrlAHook);
    ch_hCtrlAHook = NULL;

    return 0;
}