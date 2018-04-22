// OpenGLView.cpp : implementation file
//

#include "stdafx.h"
#include "Simulator_Xiaotian.h"
#include "OpenGLView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

float rotX;
float rotZ;
extern float scaleview;
/////////////////////////////////////////////////////////////////////////////
// COpenGLView

COpenGLView::COpenGLView()
{
	rotX=0.0;
	rotZ=0.0;
	scaleview=0.015;
	lbuttondown=FALSE;
}

COpenGLView::~COpenGLView()
{
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(hglrc);
	::ReleaseDC(m_hWnd,hdc);
}


BEGIN_MESSAGE_MAP(COpenGLView, CWnd)
	//{{AFX_MSG_MAP(COpenGLView)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COpenGLView message handlers

int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	hdc=::GetDC(m_hWnd);
	SetupPixelFormat(hdc);
	CPaintDC dc(this);
	
	hglrc= wglCreateContext(hdc);
	wglMakeCurrent(hdc,hglrc);
	
	::glClearColor(0.5f,0.5f,0.5f,1.0f);
	::glClearDepth(1.0f);
	::glEnable(GL_DEPTH_TEST);


	return 0;
}

void COpenGLView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
   	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );//�����ɫ�����Լ���Ȼ��� 

	
	glPushMatrix();//����ǰ����ѹ������ջ

	glRotatef( rotX, 1.0f, 0.0f, 0.0f );	//�ӵ��ƶ�
	glRotatef( rotZ, 0.0f, 0.0f, 1.0f );

	/*{
		::glPushMatrix();
		
		drawarm.DrawStaticGoods();//��������ƾ�ֹ����
		
		::glPopMatrix();//����ǰ���󵯳������ջ
	}*/
	{
		::glPushMatrix();
		
		drawarm.DrawDynamicGoods();//����������˶�������
		
		::glPopMatrix();//����ǰ���󵯳������ջ
	}
	
	
	glPopMatrix();
	
	::glFinish();		
	
	SwapBuffers( hdc);
	
	// Do not call CWnd::OnPaint() for painting messages
}

BOOL COpenGLView::SetupPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | //The buffer can draw to a window or device surface
			PFD_SUPPORT_OPENGL |//The buffer supports OpenGL drawing.
			PFD_DOUBLEBUFFER,//The buffer is double-buffered. This flag and PFD_SUPPORT_GDI are mutually exclusive in the current generic implementation.
			PFD_TYPE_RGBA,//RGBA pixels. Each pixel has four components in this order: red, green, blue, and alpha.
			24,
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			16,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
		0, 0, 0
	};
	int pixelformat;
	
	if( 0 == (pixelformat =
		::ChoosePixelFormat( hdc, &pfd))  )
	{
		
		return FALSE;
	}
	
	if( FALSE == ::SetPixelFormat( hdc,
		pixelformat, &pfd) )
	{
		
		return FALSE;
	}
	
	return TRUE;

}

void COpenGLView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	GLdouble aspect_ratio;

	if( 0 >= cx || 0 >= cy )
		return;
	
	
	
	// compute the aspect ratio
	aspect_ratio = (GLdouble)cx / (GLdouble)cy;//�����
	SetupViewPort(cx, cy);//��������������ͼ��Ĵ�С��

	::glMatrixMode(GL_PROJECTION);//��ͶӰ����Ӧ�����ľ������
	::glLoadIdentity();//���õ�ǰָ���ľ���Ϊ��λ����
	

	SetupViewingFrustum( aspect_ratio );//���ùۿ���Χ��
	SetupViewingTransform();//���ùۿ��Ƕ�
	
	::glMatrixMode(GL_MODELVIEW);//��ģ���Ӿ������ջӦ�����ľ������
	
	//Applies subsequent matrix operations to the modelview matrix stack
	::glLoadIdentity();//���õ�ǰָ���ľ���Ϊ��λ����
	
	SetupLighting();//���ù���

}

BOOL COpenGLView::SetupViewPort(int cx, int cy)
{
	glViewport(0, 0, cx, cy);//���ݴ��ڵ�ʵʱ�仯�ػ洰��
	
	return TRUE;
}

BOOL COpenGLView::SetupViewingFrustum(GLdouble aspect_ratio)
{
	gluPerspective( 40.0f, aspect_ratio, 0.1f, 20.0f );
	return TRUE;
}

BOOL COpenGLView::SetupViewingTransform()
{
	int i;
	
	GLfloat fovy = 30.0; //����//û����
	GLfloat eye[3];
	GLfloat center[3] = { 0.0f, 0.0f, 1.0f };//����ֵ����Ϊ��ͼ���ҡ�Զ��������
	GLfloat eye_dir[3];
	GLfloat up[3];
	GLfloat norm, dist;
	
	eye[0] = 0.0;//�ı俴������ֵ
	eye[1] = 5.0;//�ı俴��Զ��
	eye[2] = 2.0;//�ı俴������
	
	for(i=0; i<3; i++)
		eye_dir[i] = center[i] - eye[i];
	dist = (GLfloat)sqrt( eye_dir[0]*eye_dir[0] + eye_dir[1]*eye_dir[1] + eye_dir[2]*eye_dir[2] );
	for(i=0; i<3; i++)
		eye_dir[i] /= (GLfloat)dist;//��վ��λ����������Ŀ��������ĵ�λ��
	
	//���ϵķ����������趨
	up[0] = -eye_dir[0] * eye_dir[2];
	up[1] = -eye_dir[1] * eye_dir[2];
	up[2] = eye_dir[0] * eye_dir[0] + eye_dir[1] * eye_dir[1];
	norm = up[1]*up[1] + up[1]*up[1] + up[2]*up[2];
	norm = (GLfloat)sqrt(norm);
	for(i=0; i<3; i++)
		up[i] /= norm;
	gluLookAt( eye[0], eye[1],    eye[2],
		center[0], center[1], center[2],
		up[0],    up[1],     up[2]);
	return TRUE;
}

BOOL COpenGLView::SetupLighting()
{

	//���ù���
	GLfloat model_ambient[] = { 2.0f, 2.0f, 2.0f, 1.0f };//�����ֵ��Ĭ��ֵ
	GLfloat light_position0[]= { 1.0f, 0.0f, 5.0f, 0.0f };
	GLfloat light_color0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	
	//������
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, model_ambient );
	
	
	//��Դλ��
	glLightfv( GL_LIGHT0, GL_POSITION, light_position0 );//��Ź�
	

	//��Դ��ɫ
	glLightfv( GL_LIGHT0, GL_DIFFUSE, light_color0 );

	//�򿪹�Դ
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	
	return TRUE;
}

void COpenGLView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	lbuttondown=TRUE;
	pointcurrent=point;
	SetFocus();
	CWnd::OnLButtonDown(nFlags, point);
}

void COpenGLView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	lbuttondown=FALSE;
	
	CWnd::OnLButtonUp(nFlags, point);
}

void COpenGLView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(lbuttondown)
	{
		CSize rotate = pointcurrent - point;//�������ƶ��ľ���
		pointcurrent = point;//�ѵ�ǰ���ٸ�������
		rotZ += rotate.cx*0.3;
		rotX += rotate.cy*0.3;

		OnPaint2();
	}

	CWnd::OnMouseMove(nFlags, point);
}


void COpenGLView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	rotX=0.0;
	rotZ=0.0;
	scaleview=0.015;
	OnPaint2();
	CWnd::OnLButtonDblClk(nFlags, point);
}

void COpenGLView::OnPaint2()
{
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );//�����ɫ�����Լ���Ȼ��� 
	
	glPushMatrix();//����ǰ����ѹ������ջ
	
	glRotatef( rotX, 1.0f, 0.0f, 0.0f );	//�ӵ��ƶ�
	glRotatef( rotZ, 0.0f, 0.0f, 1.0f );
/*	
	{
		::glPushMatrix();
		
		drawarm.DrawStaticGoods();//��������ƾ�ֹ����
		
		::glPopMatrix();//����ǰ���󵯳������ջ
	}
	*/
	{
		::glPushMatrix();
		
		drawarm.DrawDynamicGoods();//����������˶�������
		
		::glPopMatrix();//����ǰ���󵯳������ջ
	}
	
	
	glPopMatrix();
	
	::glFinish();	
	
	SwapBuffers( hdc);
	
	
}

BOOL COpenGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
  if(zDelta>0)
	{
		scaleview+=0.0005;			//�û���ͼ�����Ŵ�
	}
	else
	  {
		scaleview-=0.0005;
	  }
	OnPaint2();

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
