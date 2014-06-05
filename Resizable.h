////////////////////////////////////////////////////////////////////////////
// Resizable.h -- Win32 resizable window and/or dialog realizer
// This file is part of MZC3.  See file "ReadMe.txt" and "License.txt".
////////////////////////////////////////////////////////////////////////////

#ifndef __MZC3_RESIZABLE__
#define __MZC3_RESIZABLE__

////////////////////////////////////////////////////////////////////////////
// MCtrlLayout --- control layout

struct MCtrlLayout EXTENDS_MOBJECT
{
    HWND m_hwndCtrl;
    MSize m_sizLA_1, m_sizMargin1, m_sizLA_2, m_sizMargin2;

    MCtrlLayout();
    MCtrlLayout(HWND hwndCtrl, SIZE sizLA_1, SIZE sizMargin1,
        SIZE sizLA_2, SIZE sizMargin2);
};

////////////////////////////////////////////////////////////////////////////
// layout anchors for MResizable::SetLayoutAnchor

#define mzcLA_NO_ANCHOR        MSize(-1, -1)    // don't resize
#define mzcLA_TOP_LEFT         MSize(0, 0)      // upper left
#define mzcLA_TOP_CENTER       MSize(50, 0)     // upper center
#define mzcLA_TOP_RIGHT        MSize(100, 0)    // upper right
#define mzcLA_MIDDLE_LEFT      MSize(0, 50)     // middle left
#define mzcLA_MIDDLE_CENTER    MSize(50, 50)    // middle center
#define mzcLA_MIDDLE_RIGHT     MSize(100, 50)   // middle right
#define mzcLA_BOTTOM_LEFT      MSize(0, 100)    // lower left
#define mzcLA_BOTTOM_CENTER    MSize(50, 100)   // lower center
#define mzcLA_BOTTOM_RIGHT     MSize(100, 100)  // lower right

////////////////////////////////////////////////////////////////////////////
// MResizable

struct MResizable EXTENDS_MOBJECT
{
public:
    typedef MCtrlLayout                 layout_type;
    typedef std::vector<MCtrlLayout>    layouts_type;

    MResizable();
    MResizable(const MResizable& rsz);
    MResizable& operator=(const MResizable& rsz);
    virtual ~MResizable();

    // NOTE: Please call OnParentCreate after parent's WM_CREATE.
    void OnParentCreate(HWND hwnd, BOOL bEnableResize);

    // NOTE: Please call OnSize on parent's WM_SIZE.
    void OnSize();

    // NOTE: Please call SetLayoutAnchor after OnParentCreate.
    // NOTE: sizLA_1 is upper left anchor, sizLA_2 is lower right anchor.
    void SetLayoutAnchor(HWND hwndCtrl,
                         MSize sizLA_1, MSize sizLA_2 = mzcLA_NO_ANCHOR);
    void SetLayoutAnchor(UINT nCtrlID,
                         MSize sizLA_1, MSize sizLA_2 = mzcLA_NO_ANCHOR);

          layout_type *CtrlLayout(HWND hwndCtrl);
          layout_type *CtrlLayout(UINT nCtrlID);
    const layout_type *CtrlLayout(HWND hwndCtrl) const;
    const layout_type *CtrlLayout(UINT nCtrlID) const;

    void EnableResize(BOOL bEnableResize);
    void ClearLayouts();

protected:
    HWND            m_hwndParent;
    HWND            m_hwndSizeGrip;
    layouts_type    m_layouts;

    void ShowSizeGrip(BOOL bShow = TRUE);
    void ModifyParentStyle(BOOL bEnableResize);
    void MoveSizeGrip();
    void ArrangeLayout();
    void ArrangeLayout(const MRect& ClientRect);
};

////////////////////////////////////////////////////////////////////////////

#ifndef MZC_NO_INLINING
    #undef MZC_INLINE
    #define MZC_INLINE inline
    #include "Resizable_inl.h"
#endif

#endif  // ndef __MZC3_RESIZABLE__
