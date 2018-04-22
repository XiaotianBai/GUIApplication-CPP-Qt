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
   	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );//清除颜色缓冲以及深度缓冲 

	
	glPushMatrix();//将当前矩阵压入矩阵堆栈

	glRotatef( rotX, 1.0f, 0.0f, 0.0f );	//视点移动
	glRotatef( rotZ, 0.0f, 0.0f, 1.0f );

	/*{
		::glPushMatrix();
		
		drawarm.DrawStaticGoods();//在这里绘制静止物体
		
		::glPopMatrix();//将当前矩阵弹出矩阵堆栈
	}*/
	{
		::glPushMatrix();
		
		drawarm.DrawDynamicGoods();//在这里绘制运动的物体
		
		::glPopMatrix();//将当前矩阵弹出矩阵堆栈
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
	aspect_ratio = (GLdouble)cx / (GLdouble)cy;//长宽比
	SetupViewPort(cx, cy);//好像是用来设置图框的大小的

	::glMatrixMode(GL_PROJECTION);//对投影矩阵应用随后的矩阵操作
	::glLoadIdentity();//重置当前指定的矩阵为单位矩阵
	

	SetupViewingFrustum( aspect_ratio );//设置观看范围的
	SetupViewingTransform();//设置观看角度
	
	::glMatrixMode(GL_MODELVIEW);//对模型视景矩阵堆栈应用随后的矩阵操作
	
	//Applies subsequent matrix operations to the modelview matrix stack
	::glLoadIdentity();//重置当前指定的矩阵为单位矩阵
	
	SetupLighting();//设置光照

}

BOOL COpenGLView::SetupViewPort(int cx, int cy)
{
	glViewport(0, 0, cx, cy);//根据窗口的实时变化重绘窗口
	
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
	
	GLfloat fovy = 30.0; //画角//没用上
	GLfloat eye[3];
	GLfloat center[3] = { 0.0f, 0.0f, 1.0f };//三个值依此为看图左右、远近、上下
	GLfloat eye_dir[3];
	GLfloat up[3];
	GLfloat norm, dist;
	
	eye[0] = 0.0;//改变看的左右值
	eye[1] = 5.0;//改变看的远近
	eye[2] = 2.0;//改变看的上下
	
	for(i=0; i<3; i++)
		eye_dir[i] = center[i] - eye[i];
	dist = (GLfloat)sqrt( eye_dir[0]*eye_dir[0] + eye_dir[1]*eye_dir[1] + eye_dir[2]*eye_dir[2] );
	for(i=0; i<3; i++)
		eye_dir[i] /= (GLfloat)dist;//从站的位置向对像重心看的向量的单位化
	
	//向上的方向向量的设定
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

	//设置光照
	GLfloat model_ambient[] = { 2.0f, 2.0f, 2.0f, 1.0f };//后面的值是默认值
	GLfloat light_position0[]= { 1.0f, 0.0f, 5.0f, 0.0f };
	GLfloat light_color0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	
	//环境光
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, model_ambient );
	
	
	//光源位置
	glLightfv( GL_LIGHT0, GL_POSITION, light_position0 );//零号光
	

	//光源颜色
	glLightfv( GL_LIGHT0, GL_DIFFUSE, light_color0 );

	//打开光源
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
		CSize rotate = pointcurrent - point;//获得鼠标移动的距离
		pointcurrent = point;//把当前点再赋给变量
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
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );//清除颜色缓冲以及深度缓冲 
	
	glPushMatrix();//将当前矩阵压入矩阵堆栈
	
	glRotatef( rotX, 1.0f, 0.0f, 0.0f );	//视点移动
	glRotatef( rotZ, 0.0f, 0.0f, 1.0f );
/*	
	{
		::glPushMatrix();
		
		drawarm.DrawStaticGoods();//在这里绘制静止物体
		
		::glPopMatrix();//将当前矩阵弹出矩阵堆栈
	}
	*/
	{
		::glPushMatrix();
		
		drawarm.DrawDynamicGoods();//在这里绘制运动的物体
		
		::glPopMatrix();//将当前矩阵弹出矩阵堆栈
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
		scaleview+=0.0005;			//让画的图比例放大
	}
	else
	  {
		scaleview-=0.0005;
	  }
	OnPaint2();

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
