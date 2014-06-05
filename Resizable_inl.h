////////////////////////////////////////////////////////////////////////////
// Resizable_inl.h -- Win32 resizable window and/or dialog realizer
// This file is part of MZC3.  See file "ReadMe.txt" and "License.txt".
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// MCtrlLayout

MZC_INLINE MCtrlLayout::MCtrlLayout()
{
}

MZC_INLINE MCtrlLayout::MCtrlLayout(HWND hwndCtrl,
    SIZE sizLA_1, SIZE sizMargin1, SIZE sizLA_2, SIZE sizMargin2) :
    m_hwndCtrl(hwndCtrl),
    m_sizLA_1(sizLA_1), m_sizMargin1(sizMargin1),
    m_sizLA_2(sizLA_2), m_sizMargin2(sizMargin2)
{
}

////////////////////////////////////////////////////////////////////////////
// MResizable

MZC_INLINE MResizable::MResizable() :
    m_hwndParent(NULL), m_hwndSizeGrip(NULL)
{
}

MZC_INLINE MResizable::MResizable(const MResizable& rsz) :
    m_hwndParent(rsz.m_hwndParent),
    m_hwndSizeGrip(rsz.m_hwndSizeGrip),
    m_layouts(rsz.m_layouts)
{
}

MZC_INLINE MResizable& MResizable::operator=(const MResizable& rsz)
{
    m_hwndParent = rsz.m_hwndParent;
    m_hwndSizeGrip = rsz.m_hwndSizeGrip;
    m_layouts = rsz.m_layouts;
    return *this;
}

MZC_INLINE /*virtual*/ MResizable::~MResizable()
{
}

MZC_INLINE void MResizable::ClearLayouts()
{
    m_layouts.clear();
}

MZC_INLINE void MResizable::ArrangeLayout()
{
    MRect ClientRect;
    ::GetClientRect(m_hwndParent, ClientRect);
    ArrangeLayout(&ClientRect);
}

MZC_INLINE void MResizable::SetLayoutAnchor(
    UINT nCtrlID,  MSize sizLA_1, MSize sizLA_2/* = mzcLA_NO_ANCHOR*/)
{
    assert(m_hwndParent);
    assert(::IsWindow(m_hwndParent));
    assert(sizLA_1 != mzcLA_NO_ANCHOR);
    MResizable::SetLayoutAnchor(
        ::GetDlgItem(m_hwndParent, nCtrlID), sizLA_1, sizLA_2);
}

MZC_INLINE void MResizable::OnSize()
{
    assert(m_hwndParent);
    assert(::IsWindow(m_hwndParent));

    ArrangeLayout();
    MoveSizeGrip();
}

////////////////////////////////////////////////////////////////////////////
