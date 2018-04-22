#if !defined(AFX_OPENGLVIEW_H__B2569DEE_65BC_4084_8973_6D8A4D12BE4D__INCLUDED_)
#define AFX_OPENGLVIEW_H__B2569DEE_65BC_4084_8973_6D8A4D12BE4D__INCLUDED_

#include "DrawArm.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenGLView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpenGLView window

class COpenGLView : public CWnd
{
// Construction
public:
	COpenGLView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenGLView)
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnPaint2();
	CPoint pointcurrent;
	BOOL lbuttondown;
	CDrawArm drawarm;
	BOOL SetupLighting();
	BOOL SetupViewingTransform();
	BOOL SetupViewingFrustum(GLdouble aspect_ratio);
	BOOL SetupViewPort(int cx,int cy);
	BOOL SetupPixelFormat(HDC hdc);
	HGLRC hglrc;
	HDC hdc;
	virtual ~COpenGLView();

	// Generated message map functions
protected:
	//{{AFX_MSG(COpenGLView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGLVIEW_H__B2569DEE_65BC_4084_8973_6D8A4D12BE4D__INCLUDED_)
