#include "stdafx.h"
#include "Simulator_Xiaotian.h"
#include "Dialog.h"
#include "math.h"
#include "windows.h"
#include<iostream>
using namespace std;

#include <fstream>
#include<shellapi.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  PI 3.1415926535897932384626433 
double d1 =11;
double d2 =11.5;
double a2=10;
double d4=10;
double d5=4;
double d6=4.5;
double oo[9][7],o[61][7] = { 0 };

double px2, py2, pz2, ax2,ay2,az2,nx2,ny2,nz2,ox2,oy2,oz2,L = 0.0;
double x[61], y[61], z[61] = { 0.0 };
double dif[9];
double d = 3;
double od = 10;
double oL = 0;
int Nx = 0;
int trackflag = 0;

//////////////////////////////////////////////////////////
//define variable
//////////////////////////////////////////////////////////

double ino[7] = { 0.0 };//输入的角度值
double nx=0,ny=0,nz=0,ox=0,oy=0,oz=0,ax=0,ay=0,az=0,px=0,py=0,pz=0;//转换矩阵的值
double outo[9][7] = { 0.0 };//输出角度值
//int pointnum=0;//记录是第几行的数据
//int DrawFlag[2] = { 0 };//判断是否要显示外维空间和内维空间的数据
//int ranum[9] = { 0 };//记录正确数据组的个数
//int flagg[9] = { 0 };//传递标志位值

extern float CS_Left[6] ;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimulator_XiaotianDlg dialog

CSimulator_XiaotianDlg::CSimulator_XiaotianDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimulator_XiaotianDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimulator_XiaotianDlg)
	m_lmt1 = -135;
	m_lmt2 = 135;
	m_lmt3 = -135;
	m_lmt4 = 30;
	m_lmt5 = -135;
	m_lmt6 = 20;
	m_lmt7 = -135;
	m_lmt8 = 135;
	m_lmt9 = -135;
	m_lmt10 = 135;
	m_lmt11 = -135;
	m_lmt12 = 135;
	m_step = d;
	m_step2 = od;
	m_time = 0.5;
	m_L1 = d1;
	m_L2 = d2;
	m_L3 = a2;
	m_L4 = d4;
	m_L5 = d5;
	m_L6 = d6;
	m_ax = 0.0;
	m_ay = 0.0;
	m_az = 0.0;
	m_nx = 0.0;
	m_ny = 0.0;
	m_nz = 0.0;
	m_ino1 = 0.0;
	m_ino2 = 0.0;
	m_ino3 = 0.0;
	m_ino4 = 0.0;
	m_ino5 = 0.0;
	m_ino6 = 0.0;
	m_ox = 0.0;
	m_oy = 0.0;
	m_oz = 0.0;
	m_px = 0.0;
	m_py = 0.0;
	m_pz = 0.0;
	m_OutO11 = 0.0;
	m_OutO12 = 0.0;
	m_OutO13 = 0.0;
	m_OutO14 = 0.0;
	m_OutO15 = 0.0;
	m_OutO16 = 0.0;
	m_OutO21 = 0.0;
	m_OutO23 = 0.0;
	m_OutO22 = 0.0;
	m_OutO24 = 0.0;
	m_OutO25 = 0.0;
	m_OutO26 = 0.0;
	m_OutO31 = 0.0;
	m_OutO32 = 0.0;
	m_OutO33 = 0.0;
	m_OutO34 = 0.0;
	m_OutO35 = 0.0;
	m_OutO36 = 0.0;
	m_OutO41 = 0.0;
	m_OutO42 = 0.0;
	m_OutO43 = 0.0;
	m_OutO44 = 0.0;
	m_OutO45 = 0.0;
	m_OutO46 = 0.0;
	m_OutO51 = 0.0;
	m_OutO52 = 0.0;
	m_OutO53 = 0.0;
	m_OutO54 = 0.0;
	m_OutO55 = 0.0;
	m_OutO56 = 0.0;
	m_OutO61 = 0.0;
	m_OutO62 = 0.0;
	m_OutO63 = 0.0;
	m_OutO64 = 0.0;
	m_OutO65 = 0.0;
	m_OutO66 = 0.0;
	m_OutO71 = 0.0;
	m_OutO72 = 0.0;
	m_OutO73 = 0.0;
	m_OutO74 = 0.0;
	m_OutO75 = 0.0;
	m_OutO76 = 0.0;
	m_OutO81 = 0.0;
	m_OutO82 = 0.0;
	m_OutO83 = 0.0;
	m_OutO84 = 0.0;
	m_OutO85 = 0.0;
	m_OutO86 = 0.0;
	m_px2 = 0.0;
	m_py2 = 0.0;
	m_pz2 = 0.0;
	m_explain2 = _T("");
	m_explain4 = _T("");
	m_explain6 = _T("");
	m_explain8 = _T("");
	m_explain1 = _T("");
	m_explain3 = _T("");
	m_explain5 = _T("");
	m_explain7 = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pDisplay=new COpenGLView;
	/*pointnum=0;
	for (int i=1;i<9;i++)
	{
		Flag[i]=1;
		ranum[i]=0;
	}
	DrawFlag[0]=0;
	DrawFlag[1]=0;
	*/
}

void CSimulator_XiaotianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimulator_XiaotianDlg)
	DDX_Text(pDX, IDC_AX, m_ax);
	DDX_Text(pDX, IDC_AY, m_ay);
	DDX_Text(pDX, IDC_AZ, m_az);
	DDX_Text(pDX, IDC_NX, m_nx);
	DDX_Text(pDX, IDC_NY, m_ny);
	DDX_Text(pDX, IDC_NZ, m_nz);
	DDX_Text(pDX, IDC_InO1, m_ino1);
	DDX_Text(pDX, IDC_InO2, m_ino2);
	DDX_Text(pDX, IDC_InO3, m_ino3);
	DDX_Text(pDX, IDC_InO4, m_ino4);
	DDX_Text(pDX, IDC_InO5, m_ino5);
	DDX_Text(pDX, IDC_InO6, m_ino6);
	DDX_Text(pDX, IDC_OX, m_ox);
	DDX_Text(pDX, IDC_OY, m_oy);
	DDX_Text(pDX, IDC_OZ, m_oz);
	DDX_Text(pDX, IDC_PX, m_px);
	DDX_Text(pDX, IDC_PY, m_py);
	DDX_Text(pDX, IDC_PZ, m_pz);
	DDX_Text(pDX, IDC_OutO11, m_OutO11);
	DDX_Text(pDX, IDC_OutO12, m_OutO12);
	DDX_Text(pDX, IDC_OutO13, m_OutO13);
	DDX_Text(pDX, IDC_OutO14, m_OutO14);
	DDX_Text(pDX, IDC_OutO15, m_OutO15);
	DDX_Text(pDX, IDC_OutO16, m_OutO16);
	DDX_Text(pDX, IDC_OutO21, m_OutO21);
	DDX_Text(pDX, IDC_OutO23, m_OutO23);
	DDX_Text(pDX, IDC_OutO22, m_OutO22);
	DDX_Text(pDX, IDC_OutO24, m_OutO24);
	DDX_Text(pDX, IDC_OutO25, m_OutO25);
	DDX_Text(pDX, IDC_OutO26, m_OutO26);
	DDX_Text(pDX, IDC_OutO31, m_OutO31);
	DDX_Text(pDX, IDC_OutO32, m_OutO32);
	DDX_Text(pDX, IDC_OutO33, m_OutO33);
	DDX_Text(pDX, IDC_OutO34, m_OutO34);
	DDX_Text(pDX, IDC_OutO35, m_OutO35);
	DDX_Text(pDX, IDC_OutO36, m_OutO36);
	DDX_Text(pDX, IDC_OutO41, m_OutO41);
	DDX_Text(pDX, IDC_OutO42, m_OutO42);
	DDX_Text(pDX, IDC_OutO43, m_OutO43);
	DDX_Text(pDX, IDC_OutO44, m_OutO44);
	DDX_Text(pDX, IDC_OutO45, m_OutO45);
	DDX_Text(pDX, IDC_OutO46, m_OutO46);
	DDX_Text(pDX, IDC_OutO51, m_OutO51);
	DDX_Text(pDX, IDC_OutO52, m_OutO52);
	DDX_Text(pDX, IDC_OutO53, m_OutO53);
	DDX_Text(pDX, IDC_OutO54, m_OutO54);
	DDX_Text(pDX, IDC_OutO55, m_OutO55);
	DDX_Text(pDX, IDC_OutO56, m_OutO56);
	DDX_Text(pDX, IDC_OutO61, m_OutO61);
	DDX_Text(pDX, IDC_OutO62, m_OutO62);
	DDX_Text(pDX, IDC_OutO63, m_OutO63);
	DDX_Text(pDX, IDC_OutO64, m_OutO64);
	DDX_Text(pDX, IDC_OutO65, m_OutO65);
	DDX_Text(pDX, IDC_OutO66, m_OutO66);
	DDX_Text(pDX, IDC_OutO71, m_OutO71);
	DDX_Text(pDX, IDC_OutO72, m_OutO72);
	DDX_Text(pDX, IDC_OutO73, m_OutO73);
	DDX_Text(pDX, IDC_OutO74, m_OutO74);
	DDX_Text(pDX, IDC_OutO75, m_OutO75);
	DDX_Text(pDX, IDC_OutO76, m_OutO76);
	DDX_Text(pDX, IDC_OutO81, m_OutO81);
	DDX_Text(pDX, IDC_OutO82, m_OutO82);
	DDX_Text(pDX, IDC_OutO83, m_OutO83);
	DDX_Text(pDX, IDC_OutO84, m_OutO84);
	DDX_Text(pDX, IDC_OutO85, m_OutO85);
	DDX_Text(pDX, IDC_OutO86, m_OutO86);
	DDX_Text(pDX, IDC_Explain2, m_explain2);
	DDX_Text(pDX, IDC_Explain4, m_explain4);
	DDX_Text(pDX, IDC_Explain6, m_explain6);
	DDX_Text(pDX, IDC_Explain8, m_explain8);
	DDX_Text(pDX, IDC_Explain1, m_explain1);
	DDX_Text(pDX, IDC_Explain3, m_explain3);
	DDX_Text(pDX, IDC_Explain5, m_explain5);
	DDX_Text(pDX, IDC_Explain7, m_explain7);
	DDX_Text(pDX, IDC_L1, m_L1);
	DDX_Text(pDX, IDC_L2, m_L2);
	DDX_Text(pDX, IDC_L3, m_L3);
	DDX_Text(pDX, IDC_L4, m_L4);
	DDX_Text(pDX, IDC_L5, m_L5);
	DDX_Text(pDX, IDC_L6, m_L6);
	DDX_Text(pDX, IDC_PXX, m_px2);
	DDX_Text(pDX, IDC_PYY, m_py2);
	DDX_Text(pDX, IDC_PZZ, m_pz2);
	DDX_Text(pDX, IDC_TIME, m_time);
	DDX_Text(pDX, IDC_STEP, m_step);
	DDX_Text(pDX, IDC_STEP2, m_step2);
	DDX_Text(pDX, IDC_LMT1, m_lmt1);
	DDX_Text(pDX, IDC_LMT2, m_lmt2);
	DDX_Text(pDX, IDC_LMT3, m_lmt3);
	DDX_Text(pDX, IDC_LMT4, m_lmt4);
	DDX_Text(pDX, IDC_LMT5, m_lmt5);
	DDX_Text(pDX, IDC_LMT6, m_lmt6);
	DDX_Text(pDX, IDC_LMT7, m_lmt7);
	DDX_Text(pDX, IDC_LMT8, m_lmt8);
	DDX_Text(pDX, IDC_LMT9, m_lmt9);
	DDX_Text(pDX, IDC_LMT10, m_lmt10);
	DDX_Text(pDX, IDC_LMT11, m_lmt11);
	DDX_Text(pDX, IDC_LMT12, m_lmt12);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSimulator_XiaotianDlg, CDialog)
	//{{AFX_MSG_MAP(CSimulator_XiaotianDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RunZhengJie, OnRunZhengJie)
	ON_BN_CLICKED(IDC_RunNi, OnRunNi)
	ON_BN_CLICKED(IDC_DisAns1, OnDisAns1)
	ON_BN_CLICKED(IDC_DisAns2, OnDisAns2)
	ON_BN_CLICKED(IDC_DisAns3, OnDisAns3)
	ON_BN_CLICKED(IDC_DisAns4, OnDisAns4)
	ON_BN_CLICKED(IDC_DisAns5, OnDisAns5)
	ON_BN_CLICKED(IDC_DisAns6, OnDisAns6)
	ON_BN_CLICKED(IDC_DisAns7, OnDisAns7)
	ON_BN_CLICKED(IDC_DisAns8, OnDisAns8)

	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_UPDATE, &CSimulator_XiaotianDlg::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_CLR, &CSimulator_XiaotianDlg::OnBnClickedClr)
	ON_BN_CLICKED(IDC_TRACK, &CSimulator_XiaotianDlg::OnBnClickedTrack)
	ON_BN_CLICKED(IDC_OPEN, &CSimulator_XiaotianDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_STEPCHANGE, &CSimulator_XiaotianDlg::OnBnClickedStepchange)
	ON_BN_CLICKED(IDC_CONT, &CSimulator_XiaotianDlg::OnBnClickedCont)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimulator_XiaotianDlg message handlers

BOOL CSimulator_XiaotianDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CRect rect(600,3,900,370);
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	m_pDisplay->Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE,rect,this,0);

	CS_Left[0] = 0;
	CS_Left[1] = 90;
	CS_Left[2] = 0;
	CS_Left[3] = 0;
	CS_Left[4] = 90;
	CS_Left[5] =0;
	m_pDisplay->OnPaint2();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSimulator_XiaotianDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSimulator_XiaotianDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSimulator_XiaotianDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSimulator_XiaotianDlg::RunZheng()
{
	double s1,s2,s3,s4,s5,s6,c1,c2,c3,c4,c5,c6,s23,c23;//这里的s23为sin(o2-o3),c23为cos(o2-o3)
	

		
	s1=sin(ino[1]);
	s2=sin(ino[2]);
	s3=sin(ino[3]);
	s4=sin(ino[4]);
	s5=sin(ino[5]);
	s6=sin(ino[6]);

	c1=cos(ino[1]);
	c2=cos(ino[2]);
	c3=cos(ino[3]);
	c4=cos(ino[4]);
	c5=cos(ino[5]);
	c6=cos(ino[6]);

	s23=s2*c3+s3*c2;
	c23=c2*c3-s2*s3;

	
////////////////求转换矩阵各个元素的值

//	nx = -1*s1*(s4*c5*c6+c4*s6) + c1*c23*c4*c5*c6 + c1*s23*s5*c6 - c1*c23*s4*s6;
	nx = c1*c23*(c4*c5*c6-s4*s6) - c1*s23*s5*c6 + s1*(s4*c5*c6 + c4*s6);
	if (nx<0.0000000001&&nx>-0.0000000001)
	{nx=0;
	}

	
//	ny = c1*(s4*c5*c6+c4*s6) + s1*c23*c4*c5*c6 + s1*s23*s5*c6 - s1*c23*s4*s6;
	ny = -1*c1*(s4*c5*c6 + c4*s6) + s1*c23*c4*c5*c6 - s1*s23*s5*c6 - s1*c23*s4*s6;
	if (ny<0.0000000001&&ny>-0.0000000001)
	{ny=0;
	}

	//nz = -1*s23*c4*c5*c6 + c23*s5*c6 + s23*s4*s6;
	nz = -1*s23*c4*c5*c6 - c23*s5*c6 + s23*s4*s6;
	if (nz<0.0000000001&&nz>-0.0000000001)
	{nz=0;
	}




	//ox = -1*s1*(s4*c5*s6-c4*c6) + c1*c23*c4*c5*s6 + c1*s23*s5*s6 + c1*c23*s4*c6;
	ox = -1*s1*(s4*c5*s6 - c4*c6) - c1*c23*c4*c5*s6 + c1*s23*s5*s6 - c1*c23*s4*c6;
	if (ox<0.0000000001&&ox>-0.0000000001)
	{ox=0;
	}

	//oy = c1*(s4*c5*s6-c4*c6) + s1*c23*c4*c5*s6 + s1*s23*s5*s6 + s1*c23*s4*c6;
	oy = c1*(s4*c5*s6 - c4*c6) - s1*c23*c4*c5*s6 + s1*s23*s5*s6 - s1*c23*s4*c6;
	if (oy<0.0000000001&&oy>-0.0000000001)
	{oy=0;
	}

	//oz = -1*s23*c4*c5*s6 + c23*s5*s6 - s23*s4*c6;
	oz = s23*c4*c5*s6 + c23*s5*s6 + s23*s4*c6;
	if (oz<0.0000000001&&oz>-0.0000000001)
	{oz=0;
	}

	//ax = c1*c23*c4*s5 - c1*s23*c5 - s1*s4*s5;
	ax = -c1*c23*c4*s5 - c1*s23*c5 - s1*s4*s5;
	if (ax<0.0000000001&&ax>-0.0000000001)
	{ax=0;
	}

	//ay = s1*c23*c4*s5 - s1*s23*c5 + c1*s4*s5;
	ay = -s1*c23*c4*s5 - s1*s23*c5 + c1*s4*s5;
	if (ay<0.0000000001&&ay>-0.0000000001)
	{ay=0;
	}

	//az = -1*s23*c4*s5 - c23*c5;
	az = s23*c4*s5 - c23*c5;
	if (az<0.0000000001&&az>-0.0000000001)
	{az=0;
	}

	//px = -1*L6*c1*c23*c4*s5 + L6*s1*s4*s5 + L6*c1*s23*c5 + L4*c1*s23 + L3*c1*c23 + L2*c1*c2 + L1*c1;
	px = -1 * d6*c1*c23*c4*s5 - d6*s1*s4*s5 - d6*c1*s23*c5 - d5*c1*s23 - d4*c1*s23 + a2*c1*c2;
	if (px<0.0000000001&&px>-0.0000000001)
	{px=0;
	}

	//py = -1*L6*s1*c23*c4*s5 - L6*c1*s4*s5 + L6*s1*s23*c5 + L4*s1*s23 + L3*s1*c23 + L2*s1*c2 + L1*s1;
	py = -1 * d6*s1*c23*c4*s5 + d6*c1*s4*s5 - d6*s1*s23*c5 - d5*s1*s23 - d4*s1*s23 + a2*s1*c2;
	if (py<0.0000000001&&py>-0.0000000001)
	{py=0;
	}

	//pz = L6*s23*c4*s5 + L6*c23*c5 + L4*c23 - L3*s23 - L2*s2;
	pz = d6*s23*c4*s5 - d6*c23*c5 - d5*c23 - d4*c23 - a2*s2 + d2+d1;
	if (pz<0.0000000001&&pz>-0.0000000001)
	{pz=0;
	}

}

void CSimulator_XiaotianDlg::RunNijie()//求逆解
{
	double s1[3],s2[5],s3[9],s4[9],s5[9],c1[3],c2[5],c3[9],c4[9],c5[9],s23[9],c23[9],c6[9],s6[9];//这里的s23为sin(o2+o3),c23为cos(o2+o3)
	double t11,t12,k2[3], k23[9],t21, t22[3],o23[9],c234[5],s234[5],x[9],y[9];
	double v=0;
	double L6 = d6;
	double L4 = d5;
	double L3 = d4;
	double L2 = a2;
	double L1 = d2;
//算o1,有两个解
	outo[1][1] = atan2(ay*d6 - py, ax*d6 - px);
	c1[1] = cos(outo[1][1]);
	s1[1] = sin(outo[1][1]);

	outo[2][1] = outo[1][1];
	outo[3][1] = outo[1][1];
	outo[4][1] = outo[1][1];

	outo[5][1] = atan2(-ay*d6 + py, -ax*d6 + px);
	//outo[5][1] = atan2(ay*d6 - py, ax*L6 - px) + atan2(0, 1);
	c1[2] = cos(outo[5][1]);
	s1[2] = sin(outo[5][1]);

	outo[6][1] = outo[5][1];
	outo[7][1] = outo[5][1];
	outo[8][1] = outo[5][1];
	
	//算o2，由于o1有两个解，o2有四个解,
	t21 = 2 * a2*(pz - d1 - d6*az - d2);
	t22[1] = 2 * a2*(px*c1[1] + py*s1[1] - d6*ax*c1[1] - d6*ay*s1[1]);
	t22[2] = 2 * a2*(px*c1[2] + py*s1[2] - d6*ax*c1[2] - d6*ay*s1[2]);
	k2[1] = (t21*t21) / (4 * a2*a2) + (t22[1] * t22[1]) / (4 * a2*a2) + a2*a2 - (d4 + d5)*(d4 + d5);
	k2[2] = (t21*t21) / (4 * a2*a2) + (t22[2] * t22[2]) / (4 * a2*a2) + a2*a2 - (d4 + d5)*(d4 + d5);
	//k2[1] = pow((px*c1[1] + py*s1[1] - d6*ax*c1[1] - d6*ay*s1[1]), 2) + pow((pz - d1 - d6*az - d2), 2) + a2*a2 - (d4 + d5)*(d4 + d5);
	//k2[2] = pow((px*c1[2] + py*s1[2] - d6*ax*c1[2] - d6*ay*s1[2]), 2) + pow((pz - d1 - d6*az - d2), 2) + a2*a2 - (d4 + d5)*(d4 + d5);
	outo[1][2] = atan2(t22[1], t21) - atan2(k2[1], sqrt(t21*t21 + t22[1] * t22[1] - k2[1] * k2[1]));
	//outo[1][2] = acos(k2[1] / (sqrt(t21*t21 + t22[1] * t22[1]))) - atan2(t21, t22[1]);
	outo[2][2] = outo[1][2];
	c2[1] = cos(outo[1][2]);
	s2[1] = sin(outo[1][2]);
	outo[3][2] = atan2(t22[1], t21) - atan2(k2[1], -sqrt(t21*t21 + t22[1] * t22[1] - k2[1] * k2[1]));
	//outo[3][2] = -acos(k2[1] / (sqrt(t21*t21 + t22[1] * t22[1]))) - atan2(t21, t22[1]);
	outo[4][2] = outo[3][2];
	c2[2] = cos(outo[3][2]);
	s2[2] = sin(outo[3][2]);
	outo[5][2] = atan2(t22[2], t21) - atan2(k2[2], sqrt(t21*t21 + t22[2] * t22[2] - k2[2] * k2[2]));
	//outo[5][2] = acos(sqrt((k2[2] * k2[2]) / (t21*t21 + t22[2] * t22[2]))) - atan2(t21, t22[2]);
	//outo[5][2] = acos(k2[2] / (sqrt(t21*t21 + t22[2] * t22[2]))) - atan2(t21, t22[2]);
	outo[6][2] = outo[5][2];
	c2[3] = cos(outo[5][2]);
	s2[3] = sin(outo[5][2]);
	outo[7][2] = atan2(t22[2], t21) - atan2(k2[2], -sqrt(t21*t21 + t22[2] * t22[2] - k2[2] * k2[2]));
	//outo[7][2] = -acos(sqrt((k2[2] * k2[2]) / (t21*t21 + t22[2] * t22[2]))) - atan2(t21, t22[2]);
	//outo[7][2] = -acos(k2[2] / (sqrt(t21*t21 + t22[2] * t22[2]))) - atan2(t21, t22[1]);
	outo[8][2] = outo[7][2];
	c2[4] = cos(outo[7][2]);
	s2[4] = sin(outo[7][2]);
/*	for (int i = 1; i < 9; i++)
	{
		if (outo[i][2] * 180 / PI < -180)
		{
			outo[i][2] = outo[i][2] + 2 * PI;
		}
		else if (outo[i][2] * 180 / PI > 180)
		{
			outo[i][2] = outo[i][2] - 2 * PI;
		}
	}
*/
	
//计算角3
	k23[1] = px*c1[1] + py*s1[1] - d6*(ax*c1[1] + ay*s1[1]) - a2*c2[1] + d1 + d2 - a2*s2[1] + d6*az - pz;
	k23[2] = px*c1[1] + py*s1[1] - d6*(ax*c1[1] + ay*s1[1]) - a2*c2[2] + d1 + d2 - a2*s2[2] + d6*az - pz;
	k23[3] = px*c1[2] + py*s1[2] - d6*(ax*c1[2] + ay*s1[2]) - a2*c2[3] + d1 + d2 - a2*s2[3] + d6*az - pz;
	k23[4] = px*c1[2] + py*s1[2] - d6*(ax*c1[2] + ay*s1[2]) - a2*c2[4] + d1 + d2 - a2*s2[4] + d6*az - pz;
	
	//o23[1] = acos(k23[1] / ((d4 + d5)*sqrt(2))) - atan2(1, 1);
	//o23[2] = acos(k23[1] / ((d4 + d5)*sqrt(2))) - atan2(1, 1);
	//o23[3] = acos(k23[2] / ((d4 + d5)*sqrt(2))) - atan2(1, 1);
	//o23[4] = acos(k23[2] / ((d4 + d5)*sqrt(2))) - atan2(1, 1);
	//o23[5] = acos(k23[3] / ((d4 + d5)*sqrt(2))) - atan2(1, 1);
	//o23[6] = acos(k23[3] / ((d4 + d5)*sqrt(2))) - atan2(1, 1);
	//o23[7] = acos(k23[4] / ((d4 + d5)*sqrt(2))) - atan2(1, 1);
	//o23[8] = acos(k23[4] / ((d4 + d5)*sqrt(2))) - atan2(1, 1);
	
	//o23[1]= acos(sqrt((k23[1] * k23[1]) / (2*(d4+d5)*(d4+d5)))) - atan2((d4+d5),(d4+d5));
	 o23[1] = atan2(d4 + d5, d4 + d5) - atan2(k23[1], sqrt(2 * (d4 + d5)*(d4 + d5) - k23[1] * k23[1]));
	//o23[2] =-acos(sqrt((k23[1] * k23[1]) / (2 * (d4 + d5)*(d4 + d5)))) - atan2((d4 + d5), (d4 + d5));
	o23[2] = atan2(d4 + d5, d4 + d5) - atan2(k23[1], -sqrt(2 * (d4 + d5)*(d4 + d5) - k23[1] * k23[1]));
//	o23[3] = acos(sqrt((k23[2] * k23[2]) / (2 * (d4 + d5)*(d4 + d5)))) - atan2((d4 + d5), (d4 + d5));
	o23[3] = atan2(d4 + d5, d4 + d5) - atan2(k23[2], sqrt(2 * (d4 + d5)*(d4 + d5) - k23[2] * k23[2]));
	//o23[4] = -acos(sqrt((k23[2] * k23[2]) / (2 * (d4 + d5)*(d4 + d5)))) - atan2((d4 + d5), (d4 + d5));
	o23[4] = atan2(d4 + d5, d4 + d5) - atan2(k23[2], -sqrt(2 * (d4 + d5)*(d4 + d5) - k23[2] * k23[2]));
	
	o23[5] = atan2(d4 + d5, d4 + d5) - atan2(k23[3], sqrt(2 * (d4 + d5)*(d4 + d5) - k23[3] * k23[3]));
	o23[6] = atan2(d4 + d5, d4 + d5) - atan2(k23[3], -sqrt(2 * (d4 + d5)*(d4 + d5) - k23[3] * k23[3]));
	o23[7] = atan2(d4 + d5, d4 + d5) - atan2(k23[4], sqrt(2 * (d4 + d5)*(d4 + d5) - k23[4] * k23[4]));
	o23[8] = atan2(d4 + d5, d4 + d5) - atan2(k23[4], -sqrt(2 * (d4 + d5)*(d4 + d5) - k23[4] * k23[4]));
	if((sin(o23[1])-(c1[1]*px+s1[1]*py-d6*(c1[1]*ax+s1[1]*ay)-a2*c2[1])/(-d4-d5))<0.01)
	{
		o23[2] = o23[1];
	}
	else o23[1] = o23[2];
	
	if ((sin(o23[3]) - (c1[1] * px + s1[1] * py - d6*(c1[1] * ax + s1[1] * ay) - a2*c2[2]) / (-d4 - d5))<0.01)
	{
		o23[4] = o23[3];
	}
	else o23[3] = o23[4];
	if ((sin(o23[5]) - (c1[2] * px + s1[2] * py - d6*(c1[2] * ax + s1[2] * ay) - a2*c2[3]) / (-d4 - d5))<0.01)
	{
		o23[6] = o23[5];
	}
	else o23[5] = o23[6];
	if ((sin(o23[7]) - (c1[2] * px + s1[2] * py - d6*(c1[2] * ax + s1[2] * ay) - a2*c2[4]) / (-d4 - d5))<0.01)
	{
		o23[8] = o23[7];
	}
	else o23[7] = o23[8];
/*	for (int i = 1; i < 9; i++)
	{for(int j = 1; j < 9; j++)
		{
		if (outo[i][j] * 180 / PI < -180)
		{
			outo[i] = outo[i] + 2 * PI;
		}
		else if (outo[i][j] * 180 / PI > 180)
		{
			outo[i][j]= outo[i][j] - 2 * PI;
		}
		}
	}
*/
	s23[1] = sin(o23[1]);
	c23[1] = cos(o23[1]);
	s23[2] = sin(o23[2]);
	c23[2] = cos(o23[2]);
	s23[3] = sin(o23[3]);
	c23[3] = cos(o23[3]);
	s23[4] = sin(o23[4]);
	c23[4] = cos(o23[4]);
	s23[5] = sin(o23[5]);
	c23[5] = cos(o23[5]);
	s23[6] = sin(o23[6]);
	c23[6] = cos(o23[6]);
	s23[7] = sin(o23[7]);
	c23[7] = cos(o23[7]);
	s23[8] = sin(o23[8]);
	c23[8] = cos(o23[8]);


		
	outo[1][3] = o23[1]-outo[1][2];
	outo[2][3] = o23[2] -outo[1][2];
	outo[3][3] = o23[3] -outo[3][2];
	outo[4][3] = o23[4] -outo[3][2];
	outo[5][3] = o23[5] -outo[5][2];
	outo[6][3] = o23[6] -outo[5][2];
	outo[7][3] = o23[7] -outo[7][2];
	outo[8][3] = o23[8] -outo[7][2];

/*	for (int i = 1; i < 9; i++)
	{
		if (outo[i][3] * 180 / PI < -180)
		{
			outo[i][3] = outo[i][3] + 2 * PI;
		}
		else if (outo[i][3] * 180 / PI > 180)
		{
			outo[i][3] = outo[i][3] - 2 * PI;
		}
	}
*/
	outo[1][4] = atan2( -1 * ax*s1[1] + ay*c1[1],-ax*c1[1] * c23[1] - ay*s1[1] * c23[1] + az*s23[1]);
	//outo[2][4] = atan2(-1 * ax*s1[1] + ay*c1[1], -ax*c1[1] * c23[2] - ay*s1[1] * c23[2] + az*s23[2]);
	outo[2][4] = atan2(1 * ax*s1[1] - ay*c1[1], ax*c1[1] * c23[2] + ay*s1[1] * c23[2] - az*s23[2]);
	outo[3][4] = atan2(-1 * ax*s1[1] + ay*c1[1], -ax*c1[1] * c23[3] - ay*s1[1] * c23[3] + az*s23[3]);
	//outo[4][4] = atan2(-1 * ax*s1[1] + ay*c1[1], -ax*c1[1] * c23[4] - ay*s1[1] * c23[4] + az*s23[4]);
	outo[4][4] = atan2(1 * ax*s1[1] - ay*c1[1], ax*c1[1] * c23[4] + ay*s1[1] * c23[4] - az*s23[4]);
	outo[5][4] = atan2(-1 * ax*s1[2] + ay*c1[2], -ax*c1[2] * c23[5] - ay*s1[2] * c23[5] + az*s23[5]);
	//outo[6][4] = atan2(-1 * ax*s1[2] + ay*c1[2], -ax*c1[2] * c23[6] - ay*s1[2] * c23[6] + az*s23[6]);
	outo[6][4] = atan2(1 * ax*s1[2] - ay*c1[2], ax*c1[2] * c23[6] +ay*s1[2] * c23[6] - az*s23[6]);
	outo[7][4] = atan2(-1 * ax*s1[2] + ay*c1[2], -ax*c1[2] * c23[7] - ay*s1[2] * c23[7] + az*s23[7]);
	//outo[8][4] = atan2(-1 * ax*s1[2] + ay*c1[2], -ax*c1[2] * c23[8] - ay*s1[2] * c23[8] + az*s23[8]);
	outo[8][4] = atan2(1 * ax*s1[2] - ay*c1[2], ax*c1[2] * c23[8] + ay*s1[2] * c23[8] - az*s23[8]);
	c4[1] = cos(outo[1][4]);
	s4[1] = sin(outo[1][4]);
	c4[2] = cos(outo[2][4]);
	s4[2] = sin(outo[2][4]);
	c4[3] = cos(outo[3][4]);
	s4[3] = sin(outo[3][4]);
	c4[4] = cos(outo[4][4]);
	s4[4] = sin(outo[4][4]);
	c4[5] = cos(outo[5][4]);
	s4[5] = sin(outo[5][4]);
	c4[6] = cos(outo[6][4]);
	s4[6] = sin(outo[6][4]);
	c4[7] = cos(outo[7][4]);
	s4[7] = sin(outo[7][4]);
	c4[8] = cos(outo[8][4]);
	s4[8] = sin(outo[8][4]);

	//计算角5
	s5[1] = -ax*(c1[1] * c4[1] * c23[1] + s1[1] * s4[1]) - ay*(s1[1] * c23[1] * c4[1] - c1[1] * s4[1]) + az*s23[1] * c4[1];
	c5[1] = -ax*c1[1] * s23[1] - ay*s1[1] * s23[1] - az*c23[1];
	s5[2] = -ax*(c1[1] * c4[2] * c23[2] + s1[1] * s4[2]) - ay*(s1[1] * c23[2] * c4[2] - c1[1] * s4[2]) + az*s23[2] * c4[2];
	c5[2] = -ax*c1[1] * s23[2] - ay*s1[1] * s23[2] - az*c23[2];
	s5[3] = -ax*(c1[1] * c4[3] * c23[3] + s1[1] * s4[3]) - ay*(s1[1] * c23[3] * c4[3] - c1[1] * s4[3]) + az*s23[3] * c4[3];
	c5[3] = -ax*c1[1] * s23[3] - ay*s1[1] * s23[3] - az*c23[3];
	s5[4] = -ax*(c1[1] * c4[4] * c23[4] + s1[1] * s4[4]) - ay*(s1[1] * c23[4] * c4[4] - c1[1] * s4[4]) + az*s23[4] * c4[4];
	c5[4] = -ax*c1[1] * s23[4] - ay*s1[1] * s23[4] - az*c23[4];
	s5[5] = -ax*(c1[2] * c4[5] * c23[5] + s1[2] * s4[5]) - ay*(s1[2] * c23[5] * c4[5] - c1[2] * s4[5]) + az*s23[5] * c4[5];
	c5[5] = -ax*c1[2] * s23[5] - ay*s1[2] * s23[5] - az*c23[5];
	s5[6] = -ax*(c1[2] * c4[6] * c23[6] + s1[2] * s4[6]) - ay*(s1[2] * c23[6] * c4[6] - c1[2] * s4[6]) + az*s23[6] * c4[6];
	c5[6] = -ax*c1[2] * s23[6] - ay*s1[2] * s23[6] - az*c23[6];
	s5[7] = -ax*(c1[2] * c4[7] * c23[7] + s1[2] * s4[7]) - ay*(s1[2] * c23[7] * c4[7] - c1[2] * s4[7]) + az*s23[7] * c4[7];
	c5[7] = -ax*c1[2] * s23[7] - ay*s1[2] * s23[7] - az*c23[7];
	s5[8] = -ax*(c1[2] * c4[8] * c23[8] + s1[2] * s4[8]) - ay*(s1[2] * c23[8] * c4[8] - c1[2] * s4[8]) + az*s23[8] * c4[8];
	c5[8] = -ax*c1[2] * s23[8] - ay*s1[2] * s23[8] - az*c23[8];
	outo[1][5] = atan2(s5[1], c5[1]);
	outo[2][5] = atan2(s5[2], c5[2]);
	outo[3][5] = atan2(s5[3], c5[3]);
	outo[4][5] = atan2(s5[4], c5[4]);
	outo[5][5] = atan2(s5[5], c5[5]);
	outo[6][5] = atan2(s5[6], c5[6]);
	outo[7][5] = atan2(s5[7], c5[7]);
	outo[8][5] = atan2(s5[8], c5[8]);

	//计算角6
	c6[1] = nx*(c5[1] * (c1[1] * c23[1] * c4[1] + s1[1] * s4[1]) - c1[1] * s23[1] * s5[1]) + ny*(c5[1] * (s1[1] * c23[1] * c4[1] - c1[1] * s4[1]) - s1[1] * s23[1] * s5[1]) - nz*(s23[1] * c4[1] * c5[1] + s5[1] * c23[1]);
	s6[1] = -ox*(c5[1] * (c1[1] * c23[1] * c4[1] + s1[1] * s4[1]) - c1[1] * s23[1] * s5[1]) - oy*(c5[1] * (s1[1] * c23[1] * c4[1] - c1[1] * s4[1]) - s1[1] * s23[1] * s5[1]) + oz*(s23[1] * c4[1] * c5[1] + s5[1] * c23[1]);
	c6[2] = nx*(c5[2] * (c1[1] * c23[2] * c4[2] + s1[1] * s4[2]) - c1[1] * s23[2] * s5[2]) + ny*(c5[2] * (s1[1] * c23[2] * c4[2] - c1[1] * s4[2]) - s1[1] * s23[2] * s5[2]) - nz*(s23[2] * c4[2] * c5[2] + s5[2] * c23[2]);
	s6[2] = -ox*(c5[2] * (c1[1] * c23[2] * c4[2] + s1[1] * s4[2]) - c1[1] * s23[2] * s5[2]) - oy*(c5[2] * (s1[1] * c23[2] * c4[2] - c1[1] * s4[2]) - s1[1] * s23[2] * s5[2]) + oz*(s23[2] * c4[2] * c5[2] + s5[2] * c23[2]);
	c6[3] = nx*(c5[3] * (c1[1] * c23[3] * c4[3] + s1[1] * s4[3]) - c1[1] * s23[3] * s5[3]) + ny*(c5[3] * (s1[1] * c23[3] * c4[3] - c1[1] * s4[3]) - s1[1] * s23[3] * s5[3]) - nz*(s23[3] * c4[3] * c5[3] + s5[3] * c23[3]);
	s6[3] = -ox*(c5[3] * (c1[1] * c23[3] * c4[3] + s1[1] * s4[3]) - c1[1] * s23[3] * s5[3]) - oy*(c5[3] * (s1[1] * c23[3] * c4[3] - c1[1] * s4[3]) - s1[1] * s23[3] * s5[3]) + oz*(s23[3] * c4[3] * c5[3] + s5[3] * c23[3]);
	c6[4] = nx*(c5[4] * (c1[1] * c23[4] * c4[4] + s1[1] * s4[4]) - c1[1] * s23[4] * s5[4]) + ny*(c5[4] * (s1[1] * c23[4] * c4[4] - c1[1] * s4[4]) - s1[1] * s23[4] * s5[4]) - nz*(s23[4] * c4[4] * c5[4] + s5[4] * c23[4]);
	s6[4] = -ox*(c5[4] * (c1[1] * c23[4] * c4[4] + s1[1] * s4[4]) - c1[1] * s23[4] * s5[4]) - oy*(c5[4] * (s1[1] * c23[4] * c4[4] - c1[1] * s4[4]) - s1[1] * s23[4] * s5[4]) + oz*(s23[4] * c4[4] * c5[4] + s5[4] * c23[4]);
	c6[5] = nx*(c5[5] * (c1[2] * c23[5] * c4[5] + s1[2] * s4[5]) - c1[2] * s23[5] * s5[5]) + ny*(c5[5] * (s1[2] * c23[5] * c4[5] - c1[2] * s4[5]) - s1[2] * s23[5] * s5[5]) - nz*(s23[5] * c4[5] * c5[5] + s5[5] * c23[5]);
	s6[5] = -ox*(c5[5] * (c1[2] * c23[5] * c4[5] + s1[2] * s4[5]) - c1[2] * s23[5] * s5[5]) - oy*(c5[5] * (s1[2] * c23[5] * c4[5] - c1[2] * s4[5]) - s1[2] * s23[5] * s5[5]) + oz*(s23[5] * c4[5] * c5[5] + s5[5] * c23[5]);
	c6[6] = nx*(c5[6] * (c1[2] * c23[6] * c4[6] + s1[2] * s4[6]) - c1[2] * s23[6] * s5[6]) + ny*(c5[6] * (s1[2] * c23[6] * c4[6] - c1[2] * s4[6]) - s1[2] * s23[6] * s5[6]) - nz*(s23[6] * c4[6] * c5[6] + s5[6] * c23[6]);
	s6[6] = -ox*(c5[6] * (c1[2] * c23[6] * c4[6] + s1[2] * s4[6]) - c1[2] * s23[6] * s5[6]) - oy*(c5[6] * (s1[2] * c23[6] * c4[6] - c1[2] * s4[6]) - s1[2] * s23[6] * s5[6]) + oz*(s23[6] * c4[6] * c5[6] + s5[6] * c23[6]);
	c6[7] = nx*(c5[7] * (c1[2] * c23[7] * c4[7] + s1[2] * s4[7]) - c1[2] * s23[7] * s5[7]) + ny*(c5[7] * (s1[2] * c23[7] * c4[7] - c1[2] * s4[7]) - s1[2] * s23[7] * s5[7]) - nz*(s23[7] * c4[7] * c5[7] + s5[7] * c23[7]);
	s6[7] = -ox*(c5[7] * (c1[2] * c23[7] * c4[7] + s1[2] * s4[7]) - c1[2] * s23[7] * s5[7]) - oy*(c5[7] * (s1[2] * c23[7] * c4[7] - c1[2] * s4[7]) - s1[2] * s23[7] * s5[7]) + oz*(s23[7] * c4[7] * c5[7] + s5[7] * c23[7]);
	c6[8] = nx*(c5[8] * (c1[2] * c23[8] * c4[8] + s1[2] * s4[8]) - c1[2] * s23[8] * s5[8]) + ny*(c5[8] * (s1[2] * c23[8] * c4[8] - c1[2] * s4[8]) - s1[2] * s23[8] * s5[8]) - nz*(s23[8] * c4[8] * c5[8] + s5[8] * c23[8]);
	s6[8] = -ox*(c5[8] * (c1[2] * c23[8] * c4[8] + s1[2] * s4[8]) - c1[2] * s23[8] * s5[8]) - oy*(c5[8] * (s1[2] * c23[8] * c4[8] - c1[2] * s4[8]) - s1[2] * s23[8] * s5[8]) + oz*(s23[8] * c4[8] * c5[8] + s5[8] * c23[8]);
	outo[1][6] = atan2(s6[1], c6[1]);
	outo[2][6] = atan2(s6[2], c6[2]);
	outo[3][6] = atan2(s6[3], c6[3]);
	outo[4][6] = atan2(s6[4], c6[4]);
	outo[5][6] = atan2(s6[5], c6[5]);
	outo[6][6] = atan2(s6[6], c6[6]);
	outo[7][6] = atan2(s6[7], c6[7]);
	outo[8][6] = atan2(s6[8], c6[8]);

	x[1] = -1 * d6*c1[1] * c23[1] * c4[1] * s5[1] - d6*s1[1] * s4[1] * s5[1] - d6*c1[1] * s23[1] * c5[1] - d5*c1[1] * s23[1] - d4*c1[1]*s23[1] + a2*c1[1]*c2[1];
	x[2] = -1 * d6*c1[1] * c23[2] * c4[1] * s5[1] - d6*s1[1] * s4[1] * s5[1] - d6*c1[1] * s23[1] * c5[1] - d5*c1[1] * s23[1] - d4*c1[1] * s23[1] + a2*c1[1] * c2[1];

	for (int i = 1; i < 9; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			if (outo[i][j] * 180 / PI < -180)
			{
				outo[i][j] = outo[i][j] + 2 * PI;
			}
			else if (outo[i][j] * 180 / PI > 180)
			{
				outo[i][j] = outo[i][j] - 2 * PI;
			}
		}
	}
	oo[1][1] = outo[1][1] * 180 / PI;
	oo[1][2] = outo[1][2] * 180 / PI;
	oo[1][3] = outo[1][3] * 180 / PI;
	oo[1][4] = outo[1][4] * 180 / PI;
	oo[1][5] = outo[1][5] * 180 / PI;
	oo[1][6] = outo[1][6] * 180 / PI;

	oo[2][1] = outo[2][1] * 180 / PI;
	oo[2][2] = outo[2][2] * 180 / PI;
	oo[2][3] = outo[2][3] * 180 / PI;
	oo[2][4] = outo[2][4] * 180 / PI;
	oo[2][5] = outo[2][5] * 180 / PI;
	oo[2][6] = outo[2][6] * 180 / PI;

	oo[3][1] = outo[3][1] * 180 / PI;
	oo[3][2] = outo[3][2] * 180 / PI;
	oo[3][3] = outo[3][3] * 180 / PI;
	oo[3][4] = outo[3][4] * 180 / PI;
	oo[3][5] = outo[3][5] * 180 / PI;
	oo[3][6] = outo[3][6] * 180 / PI;

	oo[4][1] = outo[4][1] * 180 / PI;
	oo[4][2] = outo[4][2] * 180 / PI;
	oo[4][3] = outo[4][3] * 180 / PI;
	oo[4][4] = outo[4][4] * 180 / PI;
	oo[4][5] = outo[4][5] * 180 / PI;
	oo[4][6] = outo[4][6] * 180 / PI;

	oo[5][1] = outo[5][1] * 180 / PI;
	oo[5][2] = outo[5][2] * 180 / PI;
	oo[5][3] = outo[5][3] * 180 / PI;
	oo[5][4] = outo[5][4] * 180 / PI;
	oo[5][5] = outo[5][5] * 180 / PI;
	oo[5][6] = outo[5][6] * 180 / PI;

	oo[6][1] = outo[6][1] * 180 / PI;
	oo[6][2] = outo[6][2] * 180 / PI;
	oo[6][3] = outo[6][3] * 180 / PI;
	oo[6][4] = outo[6][4] * 180 / PI;
	oo[6][5] = outo[6][5] * 180 / PI;
	oo[6][6] = outo[6][6] * 180 / PI;

	oo[7][1] = outo[7][1] * 180 / PI;
	oo[7][2] = outo[7][2] * 180 / PI;
	oo[7][3] = outo[7][3] * 180 / PI;
	oo[7][4] = outo[7][4] * 180 / PI;
	oo[7][5] = outo[7][5] * 180 / PI;
	oo[7][6] = outo[7][6] * 180 / PI;

	oo[8][1] = outo[8][1] * 180 / PI;
	oo[8][2] = outo[8][2] * 180 / PI;
	oo[8][3] = outo[8][3] * 180 / PI;
	oo[8][4] = outo[8][4] * 180 / PI;
	oo[8][5] = outo[8][5] * 180 / PI;
	oo[8][6] = outo[8][6] * 180 / PI;

	}

void CSimulator_XiaotianDlg::OnRunZhengJie() //进行正解计算
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	ino[1]=m_ino1*PI/180;
	ino[2]=m_ino2*PI/180;
	ino[3]=m_ino3*PI/180;
	ino[4]=m_ino4*PI/180;

	if ((int )m_ino5 % 180 == 0)
	{
		MessageBox("sinθ5 cannot be 0 (See my thesis)", "Warning",0);
	}
	
	ino[5]=m_ino5*PI/180;
	ino[6]=m_ino6*PI/180;

	CS_Left[0]=m_ino1;
	CS_Left[1]=m_ino2+90;
	CS_Left[2]=m_ino3;
	CS_Left[3]=m_ino4;
	CS_Left[4]=m_ino5+90;
	CS_Left[5]=m_ino6;

	m_pDisplay->OnPaint2();
	RunZheng();
	
	m_nx=nx;
	m_ny=ny;
	m_nz=nz;

	m_ox=ox;
	m_oy=oy;
	m_oz=oz;

	m_ax=ax;
	m_ay=ay;
	m_az=az;

	m_px=px;
	m_py=py;
	m_pz=pz;

	UpdateData(false);

}

void CSimulator_XiaotianDlg::OnRunNi() 
{
	// TODO: Add your control notification handler code here

	UpdateData(true);
	nx=m_nx;
	ny=m_ny;
	nz=m_nz;
	
	ox=m_ox;
	oy=m_oy;
	oz=m_oz;
	
	ax=m_ax;
	ay=m_ay;
	az=m_az;
	
	px=m_px;
	py=m_py;
	pz=m_pz;
	
	m_explain1="";
	m_explain2="";
	m_explain3="";
	m_explain4="";
	m_explain5="";
	m_explain6="";
	m_explain7="";
	m_explain8="";
	RunNijie();

	m_OutO11=outo[1][1]*180/PI;
	m_OutO12=outo[1][2]*180/PI;
	m_OutO13=outo[1][3]*180/PI;
	m_OutO14=outo[1][4]*180/PI;
	m_OutO15=outo[1][5]*180/PI;
	m_OutO16=outo[1][6]*180/PI;

	m_OutO21=outo[2][1]*180/PI;
	m_OutO22=outo[2][2]*180/PI;
	m_OutO23=outo[2][3]*180/PI;
	m_OutO24=outo[2][4]*180/PI;
	m_OutO25=outo[2][5]*180/PI;
	m_OutO26=outo[2][6]*180/PI;

	m_OutO31=outo[3][1]*180/PI;
	m_OutO32=outo[3][2]*180/PI;
	m_OutO33=outo[3][3]*180/PI;
	m_OutO34=outo[3][4]*180/PI;
	m_OutO35=outo[3][5]*180/PI;
	m_OutO36=outo[3][6]*180/PI;

	m_OutO41=outo[4][1]*180/PI;
	m_OutO42=outo[4][2]*180/PI;
	m_OutO43=outo[4][3]*180/PI;
	m_OutO44=outo[4][4]*180/PI;
	m_OutO45=outo[4][5]*180/PI;
	m_OutO46=outo[4][6]*180/PI;

	m_OutO51=outo[5][1]*180/PI;
	m_OutO52=outo[5][2]*180/PI;
	m_OutO53=outo[5][3]*180/PI;
	m_OutO54=outo[5][4]*180/PI;
	m_OutO55=outo[5][5]*180/PI;
	m_OutO56=outo[5][6]*180/PI;

	m_OutO61=outo[6][1]*180/PI;
	m_OutO62=outo[6][2]*180/PI;
	m_OutO63=outo[6][3]*180/PI;
	m_OutO64=outo[6][4]*180/PI;
	m_OutO65=outo[6][5]*180/PI;
	m_OutO66=outo[6][6]*180/PI;

	m_OutO71=outo[7][1]*180/PI;
	m_OutO72=outo[7][2]*180/PI;
	m_OutO73=outo[7][3]*180/PI;
	m_OutO74=outo[7][4]*180/PI;	
	m_OutO75=outo[7][5]*180/PI;
	m_OutO76=outo[7][6]*180/PI;

	m_OutO81=outo[8][1]*180/PI;
	m_OutO82=outo[8][2]*180/PI;
	m_OutO83=outo[8][3]*180/PI;
	m_OutO84=outo[8][4]*180/PI;	
	m_OutO85=outo[8][5]*180/PI;
	m_OutO86=outo[8][6]*180/PI;
	
	m_explain1+=DetAngScal(m_OutO11,m_OutO12,m_OutO13,m_OutO14,m_OutO15,m_OutO16);
	m_explain2+=DetAngScal(m_OutO21,m_OutO22,m_OutO23,m_OutO24,m_OutO25,m_OutO26);
	m_explain3+=DetAngScal(m_OutO31,m_OutO32,m_OutO33,m_OutO34,m_OutO35,m_OutO36);
	m_explain4+=DetAngScal(m_OutO41,m_OutO42,m_OutO43,m_OutO44,m_OutO45,m_OutO46);
	m_explain5+=DetAngScal(m_OutO51,m_OutO52,m_OutO53,m_OutO54,m_OutO55,m_OutO56);
	m_explain6+=DetAngScal(m_OutO61,m_OutO62,m_OutO63,m_OutO64,m_OutO65,m_OutO66);
	m_explain7+=DetAngScal(m_OutO71,m_OutO72,m_OutO73,m_OutO74,m_OutO75,m_OutO76);
	m_explain8+=DetAngScal(m_OutO81,m_OutO82,m_OutO83,m_OutO84,m_OutO85,m_OutO86);
	
	UpdateData(false);
}

CSimulator_XiaotianDlg::~CSimulator_XiaotianDlg()
{
	if (m_pDisplay)
	{
		delete m_pDisplay;
	}
}

void CSimulator_XiaotianDlg::OnDisAns1() 
{
	// TODO: Add your control notification handler code here
	CS_Left[0]=m_OutO11;
	CS_Left[1]=m_OutO12+90;
	CS_Left[2]=m_OutO13;
	CS_Left[3]=m_OutO14;
	CS_Left[4]=m_OutO15+90;
	CS_Left[5]=m_OutO16;
	m_pDisplay->OnPaint2();
}

void CSimulator_XiaotianDlg::OnDisAns2() 
{
	// TODO: Add your control notification handler code here
	CS_Left[0]=m_OutO21;
	CS_Left[1]=m_OutO22+90;
	CS_Left[2]=m_OutO23;
	CS_Left[3]=m_OutO24;
	CS_Left[4]=m_OutO25+90;
	CS_Left[5]=m_OutO26;
	m_pDisplay->OnPaint2();
}

void CSimulator_XiaotianDlg::OnDisAns3() 
{
	// TODO: Add your control notification handler code here
	CS_Left[0]=m_OutO31;
	CS_Left[1]=m_OutO32+90;
	CS_Left[2]=m_OutO33;
	CS_Left[3]=m_OutO34;
	CS_Left[4]=m_OutO35+90;
	CS_Left[5]=m_OutO36;
	m_pDisplay->OnPaint2();
}

void CSimulator_XiaotianDlg::OnDisAns4() 
{
	// TODO: Add your control notification handler code here
	CS_Left[0]=m_OutO41;
	CS_Left[1]=m_OutO42+90;
	CS_Left[2]=m_OutO43;
	CS_Left[3]=m_OutO44;
	CS_Left[4]=m_OutO45+90;
	CS_Left[5]=m_OutO46;
	m_pDisplay->OnPaint2();
}

void CSimulator_XiaotianDlg::OnDisAns5() 
{
	// TODO: Add your control notification handler code here
	CS_Left[0]=m_OutO51;
	CS_Left[1]=m_OutO52+90;
	CS_Left[2]=m_OutO53;
	CS_Left[3]=m_OutO54;
	CS_Left[4]=m_OutO55+90;
	CS_Left[5]=m_OutO56;
	m_pDisplay->OnPaint2();
}

void CSimulator_XiaotianDlg::OnDisAns6() 
{
	// TODO: Add your control notification handler code here
	CS_Left[0]=m_OutO61;
	CS_Left[1]=m_OutO62+90;
	CS_Left[2]=m_OutO63;
	CS_Left[3]=m_OutO64;
	CS_Left[4]=m_OutO65+90;
	CS_Left[5]=m_OutO66;
	m_pDisplay->OnPaint2();
}

void CSimulator_XiaotianDlg::OnDisAns7() 
{
	// TODO: Add your control notification handler code here
	CS_Left[0]=m_OutO71;
	CS_Left[1]=m_OutO72+90;
	CS_Left[2]=m_OutO73;
	CS_Left[3]=m_OutO74;
	CS_Left[4]=m_OutO75+90;
	CS_Left[5]=m_OutO76;
	m_pDisplay->OnPaint2();
}

void CSimulator_XiaotianDlg::OnDisAns8() 
{
	// TODO: Add your control notification handler code here
	CS_Left[0]=m_OutO81;
	CS_Left[1]=m_OutO82+90;
	CS_Left[2]=m_OutO83;
	CS_Left[3]=m_OutO84;
	CS_Left[4]=m_OutO85+90;
	CS_Left[5]=m_OutO86;
	m_pDisplay->OnPaint2();
}







CString CSimulator_XiaotianDlg::DetAngScal(double aa, double bb, double cc, double dd, double ee, double ff)
{
	int i=0;
	CString str=" ";
	CString str0 = " ";
	if (aa<m_lmt1||aa>m_lmt2)
	{i=1;
	str+="θ1";
	}

	if (bb<m_lmt3||bb>m_lmt4)
	{i=1;
	str+="θ2";
	}

	if (cc<m_lmt5 ||cc>m_lmt6)
	{i=1;
	str+="θ3";
	}
	
	if (dd<m_lmt7 ||dd>m_lmt8)
	{i=1;
	str+="θ4";
	}

	if (ee<m_lmt9||ee>m_lmt10)
	{i=1;
	str+="θ5";
	}
	if (ff<m_lmt11 || ff>m_lmt12)
	{
		i = 1;
		str += "θ6";
	}
	
	if (i)
	{str+="Out of range";
	}
	if(IsDlgButtonChecked(IDC_ANG))
	return str;
	else return str0;
}

void CSimulator_XiaotianDlg::OnCalLinePos() 
{
	// TODO: Add your control notification handler code here
	double zo,yo,czo,szo,cyo,syo;
	UpdateData(true);
	px=m_px;
	py=m_py;
	pz=m_pz;

	zo=atan2(py,px);
	szo=sin(zo);
	czo=cos(zo);

	yo=atan2(sqrt(px*px+py*py),pz);
	syo=sin(yo);
	cyo=cos(yo);

	nx=cyo*czo;
	ny=szo*cyo;
	nz=-syo;
	ox=-szo;
	oy=czo;
	oz=0;
	ax=syo*czo;
	ay=szo*syo;
	az=cyo;

	m_nx=nx;
	m_ny=ny;
	m_nz=nz;
	
	m_ox=ox;
	m_oy=oy;
	m_oz=oz;
	
	m_ax=ax;
	m_ay=ay;
	m_az=az;
	UpdateData(false);

}

void CSimulator_XiaotianDlg::OnCalLinePosNei() 
{
	// TODO: Add your control notification handler code here
	double zo,yo,czo,szo,cyo,syo;
	UpdateData(true);
	px=-m_px;
	py=-m_py;
	pz=-m_pz;
	
	zo=atan2(py,px);
	szo=sin(zo);
	czo=cos(zo);
	
	yo=atan2(sqrt(px*px+py*py),pz);
	syo=sin(yo);
	cyo=cos(yo);
	
	nx=cyo*czo;
	ny=szo*cyo;
	nz=-syo;
	ox=-szo;
	oy=czo;
	oz=0;
	ax=syo*czo;
	ay=szo*syo;
	az=cyo;
	
	m_nx=nx;
	m_ny=ny;
	m_nz=nz;
	
	m_ox=ox;
	m_oy=oy;
	m_oz=oz;
	
	m_ax=ax;
	m_ay=ay;
	m_az=az;
	UpdateData(false);
}


/*
void CSimulator_XiaotianDlg::OnDisScaleOut() 
{
	double s_o,s_q,s_r;//s_o为与z轴的夹角，s_q为与x轴的夹角,s_r表示长度
	double wpx,wpy,wpz;//点的位置
	double wnx,wny,wnz,wox,woy,woz,wax,way,waz;//姿态
	double czo,szo,cyo,syo;
	int f;
	double dpox=0,dpoy=0,dpoz=0,r;
	ofstream outfile;
	outfile.open("DSInn.txt",ios::out);
	outfile.close();
	outfile.open("DSInn.txt",ios::out|ios::app);

	pointnum=0;

	for (s_q=0;s_q<=PI*2;s_q+=PI/18)
	{
		for (s_o=0;s_o<=PI;s_o+=PI/18)
		{
		//	for (s_r=2000;s_r>300;s_r-=100)//求向内的
			for (s_r=500;s_r<1500;s_r+=10)//求向外的
			{
				//算姿态
				for (int k=1;k<9;k++)//判断解是否正确的语句清零
				{
					Flag[k]=1;
				}
				
		
				wpx=s_r*sin(s_o)*cos(s_q);
				wpy=s_r*sin(s_o)*sin(s_q);
				wpz=s_r*cos(s_o);
				
				//求向外的
				czo=cos(s_q);
				szo=sin(s_q);
				cyo=cos(s_o);
				syo=sin(s_o);
		/*	
				//求向内的
				czo=cos(PI+s_q);
				szo=sin(PI+s_q);
				cyo=cos(PI-s_o);
				syo=sin(PI-s_o);
		

				wnx=cyo*czo;
				wny=szo*cyo;
				wnz=-syo;
				wox=-szo;
				woy=czo;
				woz=0;
				wax=syo*czo;
				way=szo*syo;
				waz=cyo;
	
				f=JudgeAnswer(wnx,wny,wnz,wox,woy,woz,wax,way,waz,wpx,wpy,wpz);//判断是否有可实现的解

				if (f)
				{
					dpox=wpx;
					dpoy=wpy;
					dpoz=wpz;
					r=s_r;
					for (int p=1;p<9;p++)
					{
						flagg[p]=Flag[p];
					}
				}

		}
			
			if ((dpox!=0)||(dpoy!=0)||(dpoz!=0))
			{	
				if (1==flagg[1])
				{
					ranum[1]++;
				}
				else
				{
					for (int n=2;n<9;n++)
					{
						if (1==flagg[n])
						{
							ranum[n]++;
						}
					}
				}
				outfile<<dpox<<" "<<dpoy<<" "<<dpoz+550<<endl;
				TRACE("num=%d,s_r=%lf,dpox=%lf,dpoy=%lf,dpoz=%lf",pointnum,r,dpox,dpoy,dpoz);
				pointnum++;
				dpox=0;dpoy=0;dpoz=0;
			}
		}
	}
	outfile.close();
	m_pDisplay->OnPaint2();
	for (int m=1;m<9;m++)
	{
		TRACE("%d",ranum[m]);
	}


	
}
*/
int CSimulator_XiaotianDlg::judge(int i)
{
	int s = 0;
	CString str = " ";
		str = DetAngScal(oo[i][1],oo[i][2],oo[i][3],oo[i][4],oo[i][5], oo[i][6]);
		if (str != " ")
			return 0;
		else return 1;

	}


int CSimulator_XiaotianDlg::JudgeAnswer(double vnx, double vny, double vnz, double vox, double voy, double voz, double vax, double vay, double vaz, double vpx, double vpy, double vpz)
{
	int i,s=0;
	CString str=" ";

	nx=vnx;
	ny=vny;
	nz=vnz;

	ox=vox;
	oy=voy;
	oz=voz;

	ax=vax;
	ay=vay;
	az=vaz;

	px=vpx;
	py=vpy;
	pz=vpz;
	
	RunNijie();

	for (i=1;i<9;i++)
	{
		str=DetAngScal(outo[i][1]*180/PI,outo[i][2]*180/PI,outo[i][3]*180/PI,outo[i][4]*180/PI,outo[i][5]*180/PI, outo[i][6] * 180 / PI);
		if (str!=" ")
		{
			Flag[i]=0;
		}
/*		if (1==i&&1==Flag[1])
		{
			ranum[1]++;
		}
		if (i>1&&1!=Flag[1]&&1==Flag[i])
		{
			ranum[i]++;
		}*/

		s=s+Flag[i];
	}
	

	if (s==0) return 0;
	else return 1;
	

}



/*void CSimulator_XiaotianDlg::OnDrawOut() 
{
	if (0==DrawFlag[0])
	{
		DrawFlag[0]=1;
	}
	else DrawFlag[0]=0;
	m_pDisplay->OnPaint2();
}

void CSimulator_XiaotianDlg::OnDrawIn() 
{
	if (0==DrawFlag[1])
	{
		DrawFlag[1]=1;
	}
	else DrawFlag[1]=0;
	m_pDisplay->OnPaint2();
}
*/





void CSimulator_XiaotianDlg::OnBnClickedUpdate()
{
	UpdateData(true);
	d1 = m_L1;
	d2 = m_L2;
	a2 = m_L3;
	d4 = m_L4;
	d5 = m_L5;
	d6 = m_L6;
	UpdateData(false);
}


void CSimulator_XiaotianDlg::OnBnClickedClr()
{
	UpdateData(true);
	ax = 0.0;
	ay = 0.0;
	az = 0.0;
	nx = 0.0;
	ny = 0.0;
	nz = 0.0;
	ox = 0.0;
	oy = 0.0;
	oz = 0.0;
	px = 0.0;
	py = 0.0;
	pz = 0.0;
	
	m_ax = 0.0;
	m_ay = 0.0;
	m_az = 0.0;
	m_nx = 0.0;
	m_ny = 0.0;
	m_nz = 0.0;
	m_ino1 = 0.0;
	m_ino2 = 0.0;
	m_ino3 = 0.0;
	m_ino4 = 0.0;
	m_ino5 = 0.0;
	m_ino6 = 0.0;
	m_ox = 0.0;
	m_oy = 0.0;
	m_oz = 0.0;
	m_px = 0.0;
	m_py = 0.0;
	m_pz = 0.0;
	m_OutO11 = 0.0;
	m_OutO12 = 0.0;
	m_OutO13 = 0.0;
	m_OutO14 = 0.0;
	m_OutO15 = 0.0;
	m_OutO16 = 0.0;
	m_OutO21 = 0.0;
	m_OutO23 = 0.0;
	m_OutO22 = 0.0;
	m_OutO24 = 0.0;
	m_OutO25 = 0.0;
	m_OutO26 = 0.0;
	m_OutO31 = 0.0;
	m_OutO32 = 0.0;
	m_OutO33 = 0.0;
	m_OutO34 = 0.0;
	m_OutO35 = 0.0;
	m_OutO36 = 0.0;
	m_OutO41 = 0.0;
	m_OutO42 = 0.0;
	m_OutO43 = 0.0;
	m_OutO44 = 0.0;
	m_OutO45 = 0.0;
	m_OutO46 = 0.0;
	m_OutO51 = 0.0;
	m_OutO52 = 0.0;
	m_OutO53 = 0.0;
	m_OutO54 = 0.0;
	m_OutO55 = 0.0;
	m_OutO56 = 0.0;
	m_OutO61 = 0.0;
	m_OutO62 = 0.0;
	m_OutO63 = 0.0;
	m_OutO64 = 0.0;
	m_OutO65 = 0.0;
	m_OutO66 = 0.0;
	m_OutO71 = 0.0;
	m_OutO72 = 0.0;
	m_OutO73 = 0.0;
	m_OutO74 = 0.0;
	m_OutO75 = 0.0;
	m_OutO76 = 0.0;
	m_OutO81 = 0.0;
	m_OutO82 = 0.0;
	m_OutO83 = 0.0;
	m_OutO84 = 0.0;
	m_OutO85 = 0.0;
	m_OutO86 = 0.0;
	UpdateData(false);
}


void CSimulator_XiaotianDlg::OnBnClickedTrack()
{
	CString str = "Reached Destination! Total Steps:";
	double time;
	trackflag = 0;
	if (!IsDlgButtonChecked(IDC_CONT))
	{
		fstream myfile;
		myfile.open("\Solutions.txt", ios::out);
		myfile.close();
	}
	UpdateData(true);
	time = m_time * 1000;
	//转角插值法
	if (((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck())
	{
	RunNijie();
	for (int i = 1; i < 9; i++)
	{
		if (judge(i))
		{
			for (int j = 1; j < 7; j++)
				o[1][j] = oo[i][j];
			break;
		}
	}
	nx = m_nx;
	ny = m_ny;
	nz = m_nz;

	ox = m_ox;
	oy = m_oy;
	oz = m_oz;

	ax = m_ax;
	ay = m_ay;
	az = m_az;

	px = m_px;
	py = m_py;
	pz = m_pz;
	RunNijie();
	trackflag = 1;
	for (int i = 1; i < 9; i++)
	{
		if (judge(i))
		{
			for (int j = 1; j < 7; j++)
				o[2][j] = oo[i][j];
			trackflag = 0;
			break;
		}
	}
	if (trackflag)
	{
		MessageBox("Cannot reach destination!", "Warning", 0);
		/*	myfile.open("\Solutions.txt", ios::app);
		myfile << "该坐标无法到达！"<< endl;
		myfile.close();
		*/
		return;
	}

	for (int i = 1; i < 7; i++)
	{
		dif[i] = abs(o[2][i] - o[1][i]);
	}
	for (int i = 1; i < 7; i++)
	{
		if ((dif[i] >= dif[1]) && (dif[i] >= dif[2]) && (dif[i] >= dif[3]) && (dif[i] >= dif[4]) && (dif[i] >= dif[5]) && (dif[i] >= dif[6]))
			oL= dif[i] ;
	}
	Nx = ceil(oL / od) + 1;
	for (int i = 1; i < 7; i++)
	{
		o[Nx][i] = o[2][i];
	}
	for (int i = 1; i < Nx + 1; i++)
	{
		Sleep(time);

		RunTrack(i);
	}
	/*
	myfile.open("\Solutions.txt", ios::app);
	myfile << "坐标成功到达！" << endl;
	myfile.close();
	*/
	CString s;
	s.Format("%d", Nx);
	str += s;
	MessageBox(str, "Warning", 0);
}
	//直线插补法
	else
	{
		nx = m_nx;
		ny = m_ny;
		nz = m_nz;

		ox = m_ox;
		oy = m_oy;
		oz = m_oz;

		ax = m_ax;
		ay = m_ay;
		az = m_az;

		px = m_px;
		py = m_py;
		pz = m_pz;
		px2 = m_px2;
		py2 = m_py2;
		pz2 = m_pz2;
		L = sqrt((px - px2)*(px - px2) + (py - py2)*(py - py2) + (pz - pz2)*(pz - pz2));
		Nx = ceil(L / d) + 1;



		for (int i = 1; i < Nx + 1; i++)
		{
			Sleep(time);
			if (trackflag)
			{
				MessageBox("Cannot reach destination!", "Warning", 0);
				/*	myfile.open("\Solutions.txt", ios::app);
					myfile << "该坐标无法到达！"<< endl;
					myfile.close();
					*/
				return;
			}

			RunTrack(i);
		}
		/*
		myfile.open("\Solutions.txt", ios::app);
		myfile << "坐标成功到达！" << endl;
		myfile.close();
		*/
		CString s;
		s.Format("%d", Nx);
		str += s;
		MessageBox(str, "Warning", 0);
		if (IsDlgButtonChecked(IDC_CONT))
		{
			m_px = m_px2;
			m_py = m_py2;
			m_pz = m_pz2;
			UpdateData(false);

		}
	}
}
void CSimulator_XiaotianDlg::RunTrack(int z)
{
	double t1, t2, t3 = 0;
	double time = 0;
	UpdateData(true);
	time = m_time*1000;
/*
	px = m_px;
	py = m_py;
	pz = m_pz;

	px2 = m_px2;
	py2 = m_py2;
	pz2 = m_pz2;
*/
	if (((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())
	{
	/*	L = sqrt((px - px2)*(px - px2) + (py - py2)*(py - py2) + (pz - pz2)*(pz - pz2));
		Nx = int(L / d) + 1;
*/
		px = m_px + (z - 1)*((m_px2 - m_px) / (Nx-1));
		py = m_py + (z - 1)*((m_py2 - m_py) / (Nx-1));
		pz = m_pz + (z - 1)*((m_pz2 - m_pz) / (Nx-1));
		RunNijie();
		for (int i = 1; i < 9; i++)
		{
			//if (judge(i)) dif[i] = abs(oo[i][2] - o[z - 1][2]) + abs(oo[i][3] - o[z - 1][3]);
			if (judge(i)) dif[i] = (oo[i][2] + 135) + (oo[i][3] + 135);
			else dif[i] = 99999;
		}
		if ((dif[1] == 99999) && (dif[2] == 99999) && (dif[3] == 99999) && (dif[4] == 99999) && (dif[5] == 99999) && (dif[6] == 99999) && (dif[7] == 99999) && (dif[8] == 99999))
		{
			trackflag = 1;
			return;
		}
		for (int i = 1; i < 9; i++)
		{

			if (!(dif[i] < 100000))
			{
				trackflag = 1;
				return;
			}
		}



		for (int i = 1; i < 9; i++)
		{
			if ((dif[i] <= dif[1]) && (dif[i] <= dif[2]) && (dif[i] <= dif[3]) && (dif[i] <= dif[4]) && (dif[i] <= dif[5]) && (dif[i] <= dif[6]) &&
				(dif[i] <= dif[7]) && (dif[i] <= dif[8]))
				//if ((dif[i] >= dif[1]) && (dif[i] >= dif[2]) && (dif[i] >= dif[3]) && (dif[i] >= dif[4]) && (dif[i] >= dif[5]) && (dif[i] >= dif[6]) &&
				//	(dif[i] >= dif[7]) && (dif[i] >= dif[8]))
			{
				for (int j = 1; j < 7; j++)
					o[z][j] = oo[i][j];
			}
		}
		CS_Left[0] = o[z][1];
		CS_Left[1] = o[z][2] + 90;
		CS_Left[2] = o[z][3];
		CS_Left[3] = o[z][4];
		CS_Left[4] = o[z][5] + 90;
		CS_Left[5] = o[z][6];
		m_pDisplay->OnPaint2();
		fstream myfile;
		myfile.open("\Solutions.txt", ios::app);
		myfile << "#0P" << int(1500 + o[z][1] * 1000 / 135) << "T" << int(time) << "!" << "#1P" << int(1500 - o[z][2] * 1000 / 135) << "T" << int(time) << "!" << "#2P" << int(1500 - o[z][3] * 1000 / 135) << "T" << int(time) << "!" << "#3P" << int(1500 - o[z][4] * 1000 / 135) << "T" << int(time) << "!" << "#4P" << int(1500 + o[z][5] * 1000 / 135) << "T" << int(time) << "!" << "#5P" << int(1500 + o[z][6] * 1000 / 135) << "T" << int(time) << "!" << endl;
		myfile.close();
	}
	else
	{
		o[z][1]=o[1][1]+(z - 1)*((o[Nx][1] -o[1][1] ) / (Nx - 1));
		o[z][2] = o[1][2] + (z - 1)*((o[Nx][2] - o[1][2]) / (Nx - 1));
		o[z][3] = o[1][3] + (z - 1)*((o[Nx][3] - o[1][3]) / (Nx - 1));
		o[z][4] = o[1][4] + (z - 1)*((o[Nx][4] - o[1][4]) / (Nx - 1));
		o[z][5] = o[1][5] + (z - 1)*((o[Nx][5] - o[1][5]) / (Nx - 1));
		o[z][6] = o[1][6] + (z - 1)*((o[Nx][6] - o[1][6]) / (Nx - 1));
		CS_Left[0] = o[z][1];
		CS_Left[1] = o[z][2] + 90;
		CS_Left[2] = o[z][3];
		CS_Left[3] = o[z][4];
		CS_Left[4] = o[z][5] + 90;
		CS_Left[5] = o[z][6];
		m_pDisplay->OnPaint2();
		fstream myfile;
		myfile.open("\Solutions.txt", ios::app);
		myfile << "#0P" << int(1500 + o[z][1] * 1000 / 135) << "T" << int(time) << "!" << "#1P" << int(1500 - o[z][2] * 1000 / 135) << "T" << int(time) << "!" << "#2P" << int(1500 - o[z][3] * 1000 / 135) << "T" << int(time) << "!" << "#3P" << int(1500 - o[z][4] * 1000 / 135) << "T" << int(time) << "!" << "#4P" << int(1500 + o[z][5] * 1000 / 135) << "T" << int(time) << "!" << "#5P" << int(1500 + o[z][6] * 1000 / 135) << "T" << int(time) << "!" << endl;
		myfile.close();
	}
	
		m_OutO11 = oL;
		m_OutO12 = od;
		m_OutO13 = dif[3];
		m_OutO14 = dif[4];
		m_OutO15 = dif[5];
		m_OutO16 = dif[6];
		m_OutO21 = o[1][1];
		m_OutO22 = o[1][2];
		m_OutO23 = o[1][3];
		m_OutO24 = o[1][4];
		m_OutO25 = o[1][5];
		m_OutO26 = o[1][6];
		m_OutO31 = o[2][1];
		m_OutO32 = o[2][2];
		m_OutO33 = o[2][3];
		m_OutO34 = o[2][4];
		m_OutO35 = o[2][5];
		m_OutO36 = o[2][6];
		m_OutO41 = o[3][1];
		m_OutO42 = o[3][2];
		m_OutO43 = o[3][3];
		m_OutO44 = o[3][4];
		m_OutO45 = o[3][5];
		m_OutO46 = o[3][6];
		m_OutO51 = o[4][1];
		m_OutO52 = o[4][2];
		m_OutO53 = o[4][3];
		m_OutO54 = o[4][4];
		m_OutO55 = o[4][5];
		m_OutO56 = o[4][6];
		m_OutO61 = px;
		m_OutO62 = py;
		m_OutO63 = pz;
		m_OutO64 = m_px2;
		m_OutO65 = m_py2;
		m_OutO66 = m_pz2;
		m_OutO71 = m_px;
		m_OutO72 = m_py;
		m_OutO73 = m_pz;
		m_OutO74 = z;
		m_OutO75 = Nx;
	UpdateData(false);


}

void CSimulator_XiaotianDlg::OnBnClickedOpen()
{
	ShellExecute(NULL, "open","\Solutions.txt", NULL, NULL, SW_SHOWNORMAL);
}



void CSimulator_XiaotianDlg::OnBnClickedStepchange()
{
	UpdateData(true);
	d = m_step;
	od = m_step2;
	UpdateData(false);
}


void CSimulator_XiaotianDlg::OnBnClickedCont()
{
	// TODO: Add your control notification handler code here
}



