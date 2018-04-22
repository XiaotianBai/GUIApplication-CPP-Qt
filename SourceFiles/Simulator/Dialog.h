// Simulator_XiaotianDlg.h : header file
//

#if !defined(AFX_Simulator_XiaotianDLG_H__B66372BD_7B45_4494_BC80_3C99B5AF9608__INCLUDED_)
#define AFX_Simulator_XiaotianDLG_H__B66372BD_7B45_4494_BC80_3C99B5AF9608__INCLUDED_

#include "OpenGLView.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSimulator_XiaotianDlg dialog

class CSimulator_XiaotianDlg : public CDialog
{
// Construction
public:
	int Flag[9];
	int JudgeAnswer(double vnx, double vny, double vnz, double vox, double voy, double voz,double vax,double vay,double vaz,double vpx, double vpy, double vpz);
	int judge(int i);
	CString DetAngScal(double aa,double bb,double cc,double dd,double ee, double ff);
	~CSimulator_XiaotianDlg();
	COpenGLView *m_pDisplay;
	void RunNijie();
	void RunZheng();
	void RunTrack(int i);
	CSimulator_XiaotianDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSimulator_XiaotianDlg)
	enum { IDD = IDD_Simulator_Xiaotian_DIALOG };
	double m_lmt1 = 0;
	double m_lmt2 = 0;
	double m_lmt3 = 0;
	double m_lmt4 = 0;
	double m_lmt5 = 0;
	double m_lmt6 = 0;
	double m_lmt7 = 0;
	double m_lmt8 = 0;
	double m_lmt9 = 0;
	double m_lmt10 = 0;
	double m_lmt11= 0;
	double m_lmt12 = 0;
	double m_step = 0;
	double m_step2 = 0;
	double  m_time = 0;
	double	m_ax = 0;
	double	m_ay=0;
	double	m_az=0;
	double	m_nx=0;
	double	m_ny=0.0;
	double	m_nz=0.0;
	double	m_ino1=0;
	double	m_ino2=0;
	double	m_ino3=0;
	double	m_ino4=0;
	double	m_ino5=0;
	double	m_ino6=0;
	double	m_ox=0.0;
	double	m_oy=0.0;
	double	m_oz=0.0;
	double	m_px=0.0;
	double	m_py=0.0;
	double	m_pz=0.0;
	double	m_OutO11=0.0;
	double	m_OutO12=0.0;
	double	m_OutO13=0.0;
	double	m_OutO14=0.0;
	double	m_OutO15=0.0;
	double	m_OutO16=0.0;
	double	m_OutO21=0.0;
	double	m_OutO23=0.0;
	double	m_OutO22=0.0;
	double	m_OutO24=0.0;
	double	m_OutO25=0.0;
	double	m_OutO26=0.0;
	double	m_OutO31=0.0;
	double	m_OutO32=0.0;
	double	m_OutO33=0.0;
	double	m_OutO34=0.0;
	double	m_OutO35=0.0;
	double	m_OutO36=0.0;
	double	m_OutO41=0.0;
	double	m_OutO42=0.0;
	double	m_OutO43=0.0;
	double	m_OutO44=0.0;
	double	m_OutO45=0.0;
	double	m_OutO46=0.0;
	double	m_OutO51=0.0;
	double	m_OutO52=0.0;
	double	m_OutO53=0.0;
	double	m_OutO54=0.0;
	double	m_OutO55=0.0;
	double	m_OutO56=0.0;
	double	m_OutO61=0.0;
	double	m_OutO62=0.0;
	double	m_OutO63 = 0.0;
	double	m_OutO64 = 0.0;
	double	m_OutO65 = 0.0;
	double	m_OutO66 = 0.0;
	double	m_OutO71 = 0.0;
	double	m_OutO72 = 0.0;
	double	m_OutO73 = 0.0;
	double	m_OutO74 = 0.0;
	double	m_OutO75 = 0.0;
	double	m_OutO76 = 0.0;
	double	m_OutO81 = 0.0;
	double	m_OutO82 = 0.0;
	double	m_OutO83 = 0.0;
	double	m_OutO84=0.0;
	double	m_OutO85=0.0;
	double	m_OutO86=0.0;
	double m_L1 = 0.0;
	double m_L2=0.0;
	double m_L3=0.0;
	double m_L4=0.0;
	double m_L5=0.0;
	double m_L6=0.0;
	double m_px2 = 0.0;
	double m_py2 = 0.0;
	double m_pz2 = 0.0;
	CString	m_explain2;
	CString	m_explain4;
	CString	m_explain6;
	CString	m_explain8;
	CString	m_explain1;
	CString	m_explain3;
	CString	m_explain5;
	CString	m_explain7;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimulator_XiaotianDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSimulator_XiaotianDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRunZhengJie();
	afx_msg void OnRunNi();
	afx_msg void OnDisAns1();
	afx_msg void OnDisAns2();
	afx_msg void OnDisAns3();
	afx_msg void OnDisAns4();
	afx_msg void OnDisAns5();
	afx_msg void OnDisAns6();
	afx_msg void OnDisAns7();
	afx_msg void OnDisAns8();
	afx_msg void OnCalLinePos();
	afx_msg void OnCalLinePosNei();
	afx_msg void OnDisScaleOut();
	afx_msg void OnDrawOut();
	afx_msg void OnDrawIn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeIno6();
	afx_msg void OnBnClickedL();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedClr();
	afx_msg void OnBnClickedTrack();
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedStepchange();
	afx_msg void OnBnClickedCont();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Simulator_XiaotianDLG_H__B66372BD_7B45_4494_BC80_3C99B5AF9608__INCLUDED_)
