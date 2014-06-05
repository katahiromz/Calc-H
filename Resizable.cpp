////////////////////////////////////////////////////////////////////////////
// Resizable.cpp -- Win32 resizable window and/or dialog realizer
// This file is part of MZC3.  See file "ReadMe.txt" and "License.txt".
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef MZC_NO_INLINING
    #undef MZC_INLINE
    #define MZC_INLINE  /*empty*/
    #include "Resizable_inl.h"
#endif

////////////////////////////////////////////////////////////////////////////

void MResizable::ShowSizeGrip(BOOL bShow/* = TRUE*/)
{
    assert(m_hwndParent);
    assert(::IsWindow(m_hwndParent));

    if (bShow)
    {
        // size grip aleady exists?
        if (m_hwndSizeGrip == NULL || !::IsWindow(m_hwndSizeGrip))
        {
            // create size grip
            MRect ClientRect;
            ::GetClientRect(m_hwndParent, &ClientRect);
            int cx = ::GetSystemMetrics(SM_CXVSCROLL);
            int cy = ::GetSystemMetrics(SM_CYHSCROLL);
            m_hwndSizeGrip = ::CreateWindow(TEXT("SCROLLBAR"), NULL,
                WS_CHILD | WS_CLIPSIBLINGS | SBS_SIZEGRIP,
                ClientRect.right - cx, ClientRect.bottom - cy, cx, cy,
                m_hwndParent, reinterpret_cast<HMENU>(123456789),
                ::GetModuleHandle(NULL), NULL);
            assert(::IsWindow(m_hwndSizeGrip));
        }

        MoveSizeGrip();
        ::ShowWindow(m_hwndSizeGrip, SW_SHOWNOACTIVATE);
    }
    else
    {
        if (::IsWindow(m_hwndSizeGrip))
            ::ShowWindow(m_hwndSizeGrip, SW_HIDE);
    }
}

void MResizable::MoveSizeGrip()
{
    assert(m_hwndParent);
    assert(::IsWindow(m_hwndParent));

    if (m_hwndSizeGrip)
    {
        assert(::IsWindow(m_hwndSizeGrip));

        // move it
        MRect ClientRect;
        ::GetClientRect(m_hwndParent, &ClientRect);
        int cx = ::GetSystemMetrics(SM_CXVSCROLL);
        int cy = ::GetSystemMetrics(SM_CYHSCROLL);
        ::SetWindowPos(m_hwndSizeGrip, NULL,
            ClientRect.right - cx, ClientRect.bottom - cy,
            cx, cy, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);
    }
}

void MResizable::ModifyParentStyle(BOOL bEnableResize)
{
    // style
    DWORD style = ::GetWindowLong(m_hwndParent, GWL_STYLE);
    if (bEnableResize)
    {
        style &= ~DS_MODALFRAME;
        style |= WS_THICKFRAME;
        ::SetWindowLong(m_hwndParent, GWL_STYLE, style);
    }
    else
    {
        style &= ~WS_THICKFRAME;
        style |= DS_MODALFRAME;
        ::SetWindowLong(m_hwndParent, GWL_STYLE, style);
    }
    // ex.style
    style = ::GetWindowLong(m_hwndParent, GWL_EXSTYLE);
    if (bEnableResize)
    {
        style &= ~WS_EX_DLGMODALFRAME;
        style |= 0;
        ::SetWindowLong(m_hwndParent, GWL_EXSTYLE, style);
    }
    else
    {
        style &= ~0;
        style |= WS_EX_DLGMODALFRAME;
        ::SetWindowLong(m_hwndParent, GWL_EXSTYLE, style);
    }
}

void MResizable::EnableResize(BOOL bEnableResize)
{
    ShowSizeGrip(bEnableResize);
    ModifyParentStyle(bEnableResize);
}

void MResizable::OnParentCreate(HWND hwndParent, BOOL bEnableResize)
{
    assert(hwndParent);
    assert(::IsWindow(hwndParent));

    m_hwndParent = hwndParent;

    ClearLayouts();

    assert(::GetWindowLong(hwndParent, GWL_STYLE) & WS_THICKFRAME);
    EnableResize(bEnableResize);
}

void MResizable::ArrangeLayout(const MRect& ClientRect)
{
    assert(m_hwndParent);
    assert(::IsWindow(m_hwndParent));

    const int count = int(m_layouts.size());
    if (count == 0)
        return;

    HDWP hDwp = ::BeginDeferWindowPos(count);
    if (hDwp == NULL)
        return;

    const UINT uFlags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREPOSITION;
    for (int i = 0; i < count; ++i)
    {
        const layout_type& layout = m_layouts[i];
        HWND hwndCtrl = layout.m_hwndCtrl;
        if (!::IsWindow(hwndCtrl))
            continue;

        MRect ChildRect, NewRect;

        ::GetWindowRect(hwndCtrl, &ChildRect);
        ::MapWindowPoints(NULL, m_hwndParent,
                          reinterpret_cast<LPPOINT>(&ChildRect), 2);

        NewRect.left = layout.m_sizMargin1.cx +
                       ClientRect.Width() * layout.m_sizLA_1.cx / 100;
        NewRect.top = layout.m_sizMargin1.cy +
                      ClientRect.Height() * layout.m_sizLA_1.cy / 100;
        NewRect.right = layout.m_sizMargin2.cx +
                        ClientRect.Width() * layout.m_sizLA_2.cx / 100;
        NewRect.bottom = layout.m_sizMargin2.cy +
                         ClientRect.Height() * layout.m_sizLA_2.cy / 100;

        if (NewRect != ChildRect)
        {
            hDwp = ::DeferWindowPos(hDwp, hwndCtrl, NULL,
                NewRect.left, NewRect.top,
                NewRect.Width(), NewRect.Height(), uFlags);
        }

        TCHAR szClassName[8];
        ::GetClassName(hwndCtrl, szClassName, 8);
        if (::lstrcmpi(szClassName, TEXT("STATIC")) == 0)
            ::InvalidateRect(hwndCtrl, NULL, TRUE);
    }

    ::EndDeferWindowPos(hDwp);
}

void MResizable::SetLayoutAnchor(
    HWND hwndCtrl, MSize sizLA_1, MSize sizLA_2/* = mzcLA_NO_ANCHOR*/)
{
    assert(m_hwndParent);
    assert(::IsWindow(m_hwndParent));
    assert(sizLA_1 != mzcLA_NO_ANCHOR);

    MRect ClientRect, ChildRect;
    ::GetClientRect(m_hwndParent, &ClientRect);
    ::GetWindowRect(hwndCtrl, &ChildRect);
    ::MapWindowPoints(NULL, m_hwndParent,
                      reinterpret_cast<LPPOINT>(&ChildRect), 2);

    if (sizLA_2 == mzcLA_NO_ANCHOR)
        sizLA_2 = sizLA_1;

    MSize sizMargin1, sizMargin2;
    sizMargin1.cx = ChildRect.left - ClientRect.Width() * sizLA_1.cx / 100;
    sizMargin1.cy = ChildRect.top - ClientRect.Height() * sizLA_1.cy / 100;
    sizMargin2.cx = ChildRect.right - ClientRect.Width() * sizLA_2.cx / 100;
    sizMargin2.cy = ChildRect.bottom - ClientRect.Height() * sizLA_2.cy / 100;

    // search hwndCtrl
    layout_type *pLayout = CtrlLayout(hwndCtrl);
    if (pLayout)
        return;

    // if not found
    layout_type layout(hwndCtrl, sizLA_1, sizMargin1, sizLA_2, sizMargin2);
    m_layouts.push_back(layout);
}

MResizable::layout_type *MResizable::CtrlLayout(HWND hwndCtrl)
{
    const int count = int(m_layouts.size());
    for (int i = 0; i < count; ++i)
    {
        if (m_layouts[i].m_hwndCtrl == hwndCtrl)
            return &m_layouts[i];
    }
    return NULL;
}

MResizable::layout_type *MResizable::CtrlLayout(UINT nCtrlID)
{
    HWND hwndCtrl = ::GetDlgItem(m_hwndParent, nCtrlID);
    const int count = int(m_layouts.size());
    for (int i = 0; i < count; ++i)
    {
        if (m_layouts[i].m_hwndCtrl == hwndCtrl)
            return &m_layouts[i];
    }
    return NULL;
}

const MResizable::layout_type *MResizable::CtrlLayout(HWND hwndCtrl) const
{
    const int count = int(m_layouts.size());
    for (int i = 0; i < count; ++i)
    {
        if (m_layouts[i].m_hwndCtrl == hwndCtrl)
            return &m_layouts[i];
    }
    return NULL;
}

const MResizable::layout_type *MResizable::CtrlLayout(UINT nCtrlID) const
{
    HWND hwndCtrl = ::GetDlgItem(m_hwndParent, nCtrlID);
    const int count = int(m_layouts.size());
    for (int i = 0; i < count; ++i)
    {
        if (m_layouts[i].m_hwndCtrl == hwndCtrl)
            return &m_layouts[i];
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////////
