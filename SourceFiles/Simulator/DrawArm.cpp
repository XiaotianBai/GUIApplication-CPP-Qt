// DrawArm.cpp: implementation of the CDrawArm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Simulator_Xiaotian.h"
#include "DrawArm.h"
#include "math.h"

#include<iostream>
using namespace std;
#include <fstream>



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


#define NDIV_MAX 30

extern double *posx, *posy, *posz;
float CS_Left[6] = { 0.0 };
//extern int pointnum, DrawFlag[2];


float scaleview;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawArm::CDrawArm()
{

}

CDrawArm::~CDrawArm()
{

}
/*
void CDrawArm::DrawStaticGoods()
{
	//DrawFloor(2.5);
	DrawObstacle();

	if (1 == DrawFlag[0])
	{
		DrawScaleOutt();
	}
	if (1 == DrawFlag[1])
	{
		DrawScaleIn();
	}
}
*/
void CDrawArm::DrawFloor(double floorSize)
{
	int i;

	//灰色
	GLfloat grey_ambient[] = { 0.2f, 0.2f, 0.2f }; //环境色
	GLfloat grey_diffuse[] = { 0.0f, 0.0f, 0.0f }; //扩散色
	GLfloat grey_specular[] = { 0.1f, 0.1f, 0.1f }; //境面色
	GLfloat grey_shininess[] = { 20.0f };			 //反射强度

													 //米黄色
	GLfloat riceyellow_ambient[] = { 0.3f, 0.3f, 0.2f }; //环境色
	GLfloat riceyellow_diffuse[] = { 0.0f, 0.0f, 0.0f };   //扩散色
	GLfloat riceyellow_specular[] = { 0.1f, 0.1f, 0.1f };    //境面色
	GLfloat riceyellow_shininess[] = { 1.0f };			   //反射强度

	glPushMatrix();

	//立体的材质定义：白
	glMaterialfv(GL_FRONT, GL_AMBIENT, grey_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grey_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, grey_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, grey_shininess);

	//床上画线
	glBegin(GL_LINES);

	for (i = -4; i <= 4; i++) {
		glVertex3d(-floorSize, floorSize / 4.0*(double)i, 0.02);
		glVertex3d(floorSize, floorSize / 4.0*(double)i, 0.02);
	}

	for (i = -4; i <= 4; i++) {
		glVertex3d(floorSize / 4.0*(double)i, -floorSize, 0.02);
		glVertex3d(floorSize / 4.0*(double)i, floorSize, 0.02);
	}
	glEnd();

	//立体材质的定义：米黄色
	glMaterialfv(GL_FRONT, GL_AMBIENT, riceyellow_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, riceyellow_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, riceyellow_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, riceyellow_shininess);

	glBegin(GL_QUADS);
	glVertex3d(floorSize, floorSize, 0.0);
	glVertex3d(-floorSize, floorSize, 0.0);
	glVertex3d(-floorSize, -floorSize, 0.0);
	glVertex3d(floorSize, -floorSize, 0.0);
	glEnd();

	glPopMatrix();
}

void CDrawArm::DrawDynamicGoods()
{
	glPushMatrix();
	glTranslated(0, 0, 0);//确定基座在哪
	DrawArmBody(CS_Left[0], CS_Left[1], CS_Left[2], CS_Left[3], CS_Left[4], CS_Left[5]);
	glPopMatrix();

}

void CDrawArm::SolidCylinder(GLdouble radius, GLdouble height, int n_div)
{
	int i;
	double angle;

	GLfloat x, y;
	GLfloat top_poly[NDIV_MAX][3], bottom_poly[NDIV_MAX][3];
	GLfloat nv[3];

#if !defined PI
#define PI 3.14159
#endif


	//定义上下两面的正n_div多边形
	for (i = 0; i<n_div; i++)
	{
		angle = (2.0 * PI * i) / n_div;
		x = (GLfloat)cos(angle);
		y = (GLfloat)sin(angle);


		//上部正多边形定点的逆时针定义
		top_poly[i][0] = (GLfloat)radius * x;
		top_poly[i][1] = (GLfloat)radius * y;
		top_poly[i][2] = (GLfloat)height;


		//下部正多边形定点的逆时针定义
		bottom_poly[i][0] = (GLfloat)radius * x;
		bottom_poly[i][1] = (GLfloat)radius * y;
		bottom_poly[i][2] = 0.0;
	}


	//上部正多边形的描述
	glBegin(GL_POLYGON);


	//给出该表面的法向向量
	CalcNormal(top_poly[0], top_poly[1], top_poly[2], nv);//三点确定一平面，然后确定法向量
	glNormal3fv(nv);//指定法向量


					//以逆时针方向表示出各定点坐标
	for (i = 0; i<n_div; i++)
		glVertex3fv(top_poly[i]);
	glEnd();


	//侧面的描绘，四边形的连续处理
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= n_div; i++)
	{
		nv[0] = top_poly[i % n_div][0] / (GLfloat)radius;//注意这里的%的作用很巧妙
		nv[1] = top_poly[i % n_div][1] / (GLfloat)radius;
		nv[2] = 0.0;
		glNormal3fv(nv);
		glVertex3fv(bottom_poly[i % n_div]);
		glVertex3fv(top_poly[i % n_div]);
	}
	glEnd();


	//下部的正多边形描绘
	glBegin(GL_POLYGON);


	//给出该表面的法线向量
	CalcNormal(bottom_poly[0], bottom_poly[1], bottom_poly[2], nv);
	glNormal3fv(nv);

	for (i = (n_div - 1); i >= 0; i--)
		glVertex3fv(bottom_poly[i]);
	glEnd();
}

void CDrawArm::CalcNormal(GLfloat *p0, GLfloat *p1, GLfloat *p2, GLfloat *normal)
{
	int i;
	GLfloat norm;
	GLfloat v0[3], v1[3];


	for (i = 0; i<3; i++) {
		v0[i] = p2[i] - p1[i];//v0向量
		v1[i] = p0[i] - p1[i];//v1向量
	}

	// 用叉乘求法向量
	normal[0] = v0[1] * v1[2] - v0[2] * v1[1];//x轴的值
	normal[1] = v0[2] * v1[0] - v0[0] * v1[2];//y轴的值
	normal[2] = v0[0] * v1[1] - v0[1] * v1[0];//z轴的值

											  //法线的正规化，也就是单位化
	norm = (GLfloat)sqrt((double)(normal[0] * normal[0]
		+ normal[1] * normal[1]
		+ normal[2] * normal[2]));
	for (i = 0; i<3; i++)
		normal[i] /= norm;
}

void CDrawArm::DrawArmBody(float OS, float OL, float OU, float OR, float OB, float OT)
{




	//GLdouble height,radius;
	float a;   //放大缩小比例系数
	a = scaleview;
	//立体材质的定义

	//红色的
	GLfloat red_ambient[] = { 0.2f, 0.0f, 0.0f }; //环境光
	GLfloat red_diffuse[] = { 0.5f, 0.0f, 0.0f }; //扩散光
	GLfloat red_specular[] = { 0.7f, 0.6f, 0.6f }; //境面光
	GLfloat red_shininess[] = { 32.0f };			//  反射强度
													//粉色
	GLfloat pink_ambient[] = { 0.2f, 0.0f, 0.2f }; //环境光
	GLfloat pink_diffuse[] = { 0.5f, 0.0f, 0.0f }; //扩散光
	GLfloat pink_specular[] = { 0.7f, 0.6f, 0.6f }; //境面光
	GLfloat pink_shininess[] = { 32.0f };			//  反射强度
													//绿色
	GLfloat green_ambient[] = { 0.0f, 0.2f, 0.0f };
	GLfloat green_diffuse[] = { 0.0f, 0.5f, 0.0f };
	GLfloat green_specular[] = { 0.6f, 0.7f, 0.6f };
	GLfloat green_shininess[] = { 32.0f };

	//青色
	GLfloat blue_ambient[] = { 0.0f, 0.0f, 0.2f };
	GLfloat blue_diffuse[] = { 0.0f, 0.0f, 0.5f };
	GLfloat blue_specular[] = { 0.6f, 0.6f, 0.7f };
	GLfloat blue_shininess[] = { 32.0f };

	//黄色
	GLfloat yellow_ambient[] = { 0.3f, 0.3f, 0.0f };
	GLfloat yellow_diffuse[] = { 0.3f, 0.3f, 0.0f };
	GLfloat yellow_specular[] = { 0.7f, 0.7f, 0.5f };
	GLfloat yellow_shininess[] = { 32.0f };

	//黑色
	GLfloat black_ambient[] = { 0.0f, 0.0f, 0.0f };
	GLfloat black_diffuse[] = { 0.0f, 0.0f, 0.0f };
	GLfloat black_specular[] = { 0.7f, 0.7f, 0.7f };
	GLfloat black_shininess[] = { 32.0f };

	//灰色
	GLfloat grey_ambient[] = { 0.3f, 0.3f, 0.3f };
	GLfloat grey_diffuse[] = { 0.3f, 0.3f, 0.3f };
	GLfloat grey_specular[] = { 0.7f, 0.7f, 0.5f };
	GLfloat grey_shininess[] = { 32.0f };
	//深灰色
	GLfloat darkgrey_ambient[] = { 0.2f, 0.2f, 0.2f };
	GLfloat darkgrey_diffuse[] = { 0.2f, 0.2f, 0.2f };
	GLfloat darkgrey_specular[] = { 0.7f, 0.7f, 0.5f };
	GLfloat darkgrey_shininess[] = { 32.0f };

	//黑色固定基台部
	/////////////////////////////////////////////////////////////
	//立体材质的定义：黑
	glMaterialfv(GL_FRONT, GL_AMBIENT, black_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, black_shininess);

	glPushMatrix();
	SolidCube(32 * a, 32 * a, 10 * a);//大方黑座
	glPopMatrix();

	//画四个小斜面
	glPushMatrix();
	glTranslated(11 * a, 21 * a, 0);
	SolidXieCube(10 * a, 10 * a, 10 * a);
	glPopMatrix();

	glPushMatrix();
	glTranslated(11 * a, -21 * a, 0);
	glRotatef(180, 0.0, 0.0, 1.0);
	SolidXieCube(10 * a, 10 * a, 10 * a);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-11 * a, 21 * a, 0);
	SolidXieCube(10 * a, 10 * a, 10 * a);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-11 * a, -21 * a, 0);
	glRotatef(180, 0.0, 0.0, 1.0);
	SolidXieCube(10 * a, 10 * a, 10 * a);
	glPopMatrix();


	//深灰色连接控制部
	/////////////////////////////////////////////////////////////
	//立体材质的定义：深灰色
	glTranslated(0.0, 0.0, 10 * a);  //把原点往上挪!!!!!!!!!!!!!!!!没push

	glMaterialfv(GL_FRONT, GL_AMBIENT, darkgrey_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, darkgrey_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, darkgrey_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, darkgrey_shininess);

	glPushMatrix();
	SolidCube(30 * a, 30 * a, 2 * a);
	glPopMatrix();

	glTranslated(0.0, 0.0, 2 * a);  //把原点往上挪!!!!!!!!!!!!!!!!没push

	glPushMatrix();
	SolidCylinder(15 * a, 15 * a, 16);//画的是连接部的大圆柱


	glTranslated(0.0, 0.0, 15 * a);  //把原点往上挪!!!!!!!!!!!!!!!!没push

									 //画黑色电机
	glMaterialfv(GL_FRONT, GL_AMBIENT, black_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, black_shininess);

	glPushMatrix();
	SolidCylinder(6 * a, 12 * a, 8);  //画电机大部分
	glTranslated(0.0, 0.0, 12 * a);
	SolidCylinder(4 * a, 5 * a, 8);   //画电机小部分
	glPopMatrix();

	//S轴旋转部分
	/////////////////////////////////////////////////////////////
	//立体材质的定义：灰色
	glRotated(OS, 0, 0, 1);		//此处有参数，第一个变量，是S轴旋转的角度值！！！！！！！

	glMaterialfv(GL_FRONT, GL_AMBIENT, blue_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, blue_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, blue_shininess);

	glPushMatrix();
	SolidCylinder(17 * a, 8 * a, 30);//S轴最低下的大圆盘
	glPopMatrix();


	glTranslated(0.0, 0.0, 8 * a);  //把原点往上挪!!!!!!!!!!!!!!!!没push
									
	glPushMatrix();
	glTranslated(0, 0, 0);
	SolidCube(9.2*a, 5 * a, 20 * a);//画竖着的大长方体托
	glPopMatrix();

	glPushMatrix();
	glTranslated(-12 * a, 3 * a, -3 * a);
	glRotated(-90, 0, 1, 0);
	glRotated(90, 0, 0, 1);
	SolidXieCube(5 * a, 6 * a, 3.2*a);    //画一个小的链接斜三角，链接用
	glPopMatrix();

	glTranslated(0, 0.0, 20 * a); //把原点往上挪!!!!!!!!!!!!!!!!没push

	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	SolidCylinder(9 * a, 5 * a, 30); //画横着的那个扁圆
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, black_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, black_shininess);

	glPushMatrix();
	glTranslated(0, -14 * a, 0.0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(6 * a, 14.1*a, 8); //画横着电机的大的那部分
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -18 * a, 0.0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(4.5*a, 4.1*a, 8); //画横着电机的小的那部分	
	glPopMatrix();

	//L轴旋转部分
	/////////////////////////////////////////////////////////////
	//立体材质的定义：灰色
	glRotated(OL, 0, -1, 0);		//此处有参数，第一个变量，是L轴旋转的角度值！！！！！！！

	glMaterialfv(GL_FRONT, GL_AMBIENT, pink_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, pink_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, pink_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, pink_shininess);

	glTranslated(0.0, 5 * a, 0.0);//把原点往前挪!!!!!!!!!!!!!!!!没push

	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	SolidCylinder(8 * a, 4 * a, 30); //画横着链接的小的那部分	
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 4 * a, 0.0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(9 * a, 3.2*a, 30); //画横着链接的大的那部分	
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 7.2*a, 4.5*a);
	glRotated(-90, 1, 0, 0);
	SolidXieCube(12.2*a, 9 * a, 6.6*a);//画下面的向下的三角
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 10.5*a, 9 * a);
	SolidCube(12.2*a, 6.6*a, 43.4*a);//画L臂
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 7.2*a, 56.9*a);
	glRotated(-90, 1, 0, 0);
	glRotated(180, 0, 0, 1);
	SolidXieCube(12.2*a, 9 * a, 6.6*a);//画上面向上的三角
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 4.2*a, 61.4*a);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(7 * a, 3 * a, 30);   //画上面的原片
	glPopMatrix();

	//U轴旋转部分
	/////////////////////////////////////////////////////////////
	//立体材质的定义：灰色
	glTranslated(0.0, 4.2*a, 61.4*a);//把原点往前挪!!!!!!!!!!!!!!!!没push

	glMaterialfv(GL_FRONT, GL_AMBIENT, yellow_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellow_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, yellow_shininess);

	glRotated(-OU , 0, 1, 0);		//此处有参数，第一个变量，是U轴旋转的角度值！！！！！！！

	glPushMatrix();
	glTranslated(0.0, -2.5*a, 0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(6 * a, 2.5*a, 30);   //画转轴处的小原片
	glPopMatrix();



	glMaterialfv(GL_FRONT, GL_AMBIENT, black_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, black_shininess);

	glPushMatrix();
	glTranslated(0, -19.5*a, 0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(6.5*a, 11 * a, 8);//画电机大的那部分
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -24 * a, 0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(3.75*a, 4.5*a, 8);//画电机小的那部分
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, yellow_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellow_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, yellow_shininess);

	glPushMatrix();
	//glTranslated(7.5*a, -9.2*a, 15.5*a);
	glTranslated(7.5*a, -9.2*a, 0);
	glRotated(-90, 0, 1, 0);
	SolidCylinder(6.5*a, 28.4*a, 30);//画藏电机的大圆柱
	glPopMatrix();


	glMaterialfv(GL_FRONT, GL_AMBIENT, yellow_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellow_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, yellow_shininess);


	//R轴旋转部分
	/////////////////////////////////////////////////////////////
	//立体材质的定义：灰色
	glTranslated(-20.9*a, -9.2*a, 0);//把原点往右上挪!!!!!!!!!!!!!!!!没push

	glRotated(OR, 1, 0, 0);		//此处有参数，第一个变量，是R轴旋转的角度值！！！！！！！

	glMaterialfv(GL_FRONT, GL_AMBIENT, green_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, green_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, green_shininess);

	glPushMatrix();
	glRotated(-90, 0, 1, 0);
	SolidCylinder(6.5*a, 2 * a, 30);//画紧贴着的那个小原片
	glPopMatrix();

	glPushMatrix();
	glTranslated(-2 * a, 0, 0);
	glRotated(-90, 0, 1, 0);
	SolidCylinder(2.5*a, 14 * a, 30);//画被卡在中间的圆柱
	glPopMatrix();

	glPushMatrix();
	glTranslated(-9 * a, 0, -4.5*a);
	SolidCube(14 * a, 1.5*a, 9 * a);//画竖着的卡片
	glPopMatrix();

	glPushMatrix();
	glTranslated(-9 * a, 0, -0.75*a);
	SolidCube(14 * a, 9 * a, 1.5*a);//画横着的卡片
	glPopMatrix();

	glPushMatrix();
	glTranslated(-15 * a, -6.5*a, 0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(4.8*a, 13 * a, 30);//画左侧的卧圆柱
	glPopMatrix();

	glPushMatrix();
	glTranslated(-23.5*a, 0, -4.8*a);
	SolidCube(17 * a, 8.5*a, 9.6*a);//画中间的方块
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, blue_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, blue_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, blue_shininess);

	glPushMatrix();
	glTranslated(-28.95*a, 5.25*a, -4.8*a);
	SolidCube(27.9*a, 2.5*a, 9.6*a);//画前侧的挡板
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, yellow_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellow_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, yellow_shininess);

	glPushMatrix();
	glTranslated(-28.95*a, -5.25*a, -4.8*a);
	SolidCube(27.9*a, 2.5*a, 9.6*a);//画后侧的挡板
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, green_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, green_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, green_shininess);
	glPushMatrix();
	glTranslated(-42.9*a, 4 * a, 0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(4.8*a, 2.5*a, 30);//画右侧前侧原片
	glPopMatrix();

	glPushMatrix();
	glTranslated(-42.9*a, -6.5*a, 0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(4.8*a, 2.5*a, 30);//画右侧前侧原片
	glPopMatrix();

	//B轴旋转部分
	/////////////////////////////////////////////////////////////
	//立体材质的定义：灰色
	glTranslated(-42.9*a, 0, 0);//把原点往右上挪!!!!!!!!!!!!!!!!没push

	glRotated(OB, 0, -1, 0);		//此处有参数，第一个变量，是B轴旋转的角度值！！！！！！！
	glRotated(180, 0, -1, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, red_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, red_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, red_shininess);

	glPushMatrix();
	glTranslated(0, -4 * a, 0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(4.5*a, 8 * a, 30);//画的旋转轴
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -5.5*a);
	SolidCube(6 * a, 6 * a, 4 * a);//小的正方体
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -10 * a);
	SolidCube(7 * a, 7 * a, 4.5*a);//大一点的正方体
	glPopMatrix();

	//B轴旋转部分
	/////////////////////////////////////////////////////////////
	//立体材质的定义：灰色
	glTranslated(0, 0, -10 * a);//把原点往下挪!!!!!!!!!!!!!!!!没push

	glRotated(OT , 0, 0, 1);		//此处有参数，第一个变量，是T轴旋转的角度值！！！！！！

	glMaterialfv(GL_FRONT, GL_AMBIENT, yellow_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellow_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, yellow_shininess);

	glPushMatrix();
	glTranslated(0, 0, -0.6*a);
	SolidCylinder(2.5*a, 0.6*a, 30);//链接的黄色轴
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, black_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, black_shininess);

	glPushMatrix();
	glTranslated(0, 0, -1.6*a);
	SolidCylinder(3.5*a, a, 30);//黄色轴下面的黑色圆柱体
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, grey_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grey_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, grey_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, grey_shininess);

	glPushMatrix();
	glTranslated(0, 0, -2.9*a);
	SolidCylinder(3.5*a, 1.3*a, 30);//黑色轴下面的灰色圆柱体
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -8.1*a);
	SolidCube(2.2*a, 4.1*a, 5.2*a);//气抓长立方体
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, black_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, black_shininess);

	glPushMatrix();
	glTranslated(0, 0, -9 * a);
	SolidCube(2.2*a, 5 * a, 0.9*a);//气抓矮立方体
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.1*a, 0, -5.5*a);
	glRotated(90, 0, 1, 0);
	SolidCylinder(2 * a, 4.8*a, 30);//横抓的圆柱
	glPopMatrix();

	//下面三个是画三个小手抓
	glPushMatrix();
	glTranslated(6.5*a, 0, -5.5*a);
	glTranslated(0, 0, -2.9*a);
	SolidCube(1.2*a, 1.2*a, 2.4*a);
	glPopMatrix();

	glPushMatrix();
	glTranslated(6.5*a, 0, -5.5*a);
	glRotated(120, 1, 0, 0);
	glTranslated(0, 0, -2.9*a);
	SolidCube(1.2*a, 1.2*a, 2.4*a);
	glPopMatrix();

	glPushMatrix();
	glTranslated(6.5*a, 0, -5.5*a);
	glRotated(-120, 1, 0, 0);
	glTranslated(0, 0, -2.9*a);
	SolidCube(1.2*a, 1.2*a, 2.4*a);
	glPopMatrix();
	//画大抓前侧的
	glMaterialfv(GL_FRONT, GL_AMBIENT, red_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, red_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, red_shininess);

	glPushMatrix();
	glTranslated(0, 0, -9 * a);
	glTranslated(0, 1.8*a, -3 * a);
	//SolidCube(2 * a, 1.4*a, 3 * a);

	glTranslated(0, 1.5*a, -1.4*a);

	glPushMatrix();
	glRotated(48, 1, 0, 0);
//	SolidCube(2 * a, 0.9*a, 2.7*a);
	glPopMatrix();

	glTranslated(0, 0, -1.6*a);
//	SolidCube(2 * a, 0.9*a, 1.9*a);

	glTranslated(0, -1.6*a, -1.5*a);
	glRotated(-45, 1, 0, 0);
//	SolidCube(2 * a, 0.9*a, 2.5*a);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, yellow_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellow_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, yellow_shininess);

	//画大抓后侧的
	glPushMatrix();
	glTranslated(0, 0, -9 * a);
	glTranslated(0, -1.8*a, -3 * a);
//	SolidCube(2 * a, 1.4*a, 3 * a);

	glTranslated(0, -1.5*a, -1.4*a);

	glPushMatrix();
	glRotated(-48, 1, 0, 0);
//	SolidCube(2 * a, 0.9*a, 2.7*a);
	glPopMatrix();
	glTranslated(0, 0, -1.6*a);
//	SolidCube(2 * a, 0.9*a, 1.9*a);

	glTranslated(0, 1.6*a, -1.5*a);
	glRotated(45, 1, 0, 0);
//	SolidCube(2 * a, 0.9*a, 2.5*a);
	glPopMatrix();
	
}

void CDrawArm::SolidCube(GLfloat Length, GLfloat Width, GLfloat Height)
{
	GLfloat L, W, H;
	L = Length;
	W = Width;
	H = Height;

	GLfloat
		p1[] = { -0.5*L,0.5*W,0 },
		p2[] = { -0.5*L,0.5*W,H },
		p3[] = { -0.5*L,-0.5*W,H },
		p4[] = { -0.5*L,-0.5*W,0 },
		p5[] = { 0.5*L,0.5*W,0 },
		p6[] = { 0.5*L,0.5*W,H },
		p7[] = { 0.5*L,-0.5*W,H },
		p8[] = { 0.5*L,-0.5*W,0 };

	GLfloat  m1[] = { 1.0,0.0,0.0 },
		m2[] = { -1.0,0.0,0.0 },
		m3[] = { 0.0,1.0,0.0 },
		m4[] = { 0.0,-1.0,0.0 },
		m5[] = { 0.0,0.0,1.0 },
		m6[] = { 0.0,0.0,-1.0 };

	glBegin(GL_QUADS);//绘制多个四边形

					  //右侧	
	glNormal3fv(m2);  //四个点一定要按照时针顺序来
	glVertex3fv(p1);
	glVertex3fv(p2);
	glVertex3fv(p3);
	glVertex3fv(p4);

	//左侧	
	glNormal3fv(m1);
	glVertex3fv(p5);
	glVertex3fv(p6);
	glVertex3fv(p7);
	glVertex3fv(p8);

	//前面	
	glNormal3fv(m3);
	glVertex3fv(p1);
	glVertex3fv(p2);
	glVertex3fv(p6);
	glVertex3fv(p5);

	//后面	
	glNormal3fv(m4);
	glVertex3fv(p4);
	glVertex3fv(p3);
	glVertex3fv(p7);
	glVertex3fv(p8);

	//上面	
	glNormal3fv(m5);
	glVertex3fv(p2);
	glVertex3fv(p3);
	glVertex3fv(p7);
	glVertex3fv(p6);

	//下面	
	glNormal3fv(m6);
	glVertex3fv(p1);
	glVertex3fv(p4);
	glVertex3fv(p8);
	glVertex3fv(p5);

	glEnd();




}

void CDrawArm::SolidXieCube(GLfloat Lenght, GLfloat Width, GLfloat Height)
{
	GLfloat L, W, H;
	L = Lenght;
	W = Width;
	H = Height;
	GLfloat nv[3];

	GLfloat
		p1[] = { -0.5*L,0.5*W,0 },
		p2[] = { -0.5*L,-0.5*W,0 },
		p3[] = { -0.5*L,-0.5*W,H },
		p4[] = { 0.5*L,0.5*W,0 },
		p5[] = { 0.5*L,-0.5*W,0 },
		p6[] = { 0.5*L,-0.5*W,H };


	GLfloat
		m1[] = { 1.0,0.0,0.0 },
		m2[] = { -1.0,0.0,0.0 },

		m4[] = { 0.0,-1.0,0.0 },

		m6[] = { 0.0,0.0,-1.0 };

	glBegin(GL_TRIANGLES);//绘制多个四边形

						  //右侧	
	glNormal3fv(m2);  //四个点一定要按照时针顺序来
	glVertex3fv(p1);
	glVertex3fv(p2);
	glVertex3fv(p3);

	//左侧	
	glNormal3fv(m1);  //四个点一定要按照时针顺序来
	glVertex3fv(p4);
	glVertex3fv(p5);
	glVertex3fv(p6);

	glEnd();

	glBegin(GL_QUADS);

	//后面
	glNormal3fv(m4);  //四个点一定要按照时针顺序来
	glVertex3fv(p2);
	glVertex3fv(p3);
	glVertex3fv(p6);
	glVertex3fv(p5);

	//下面
	glNormal3fv(m6);  //四个点一定要按照时针顺序来
	glVertex3fv(p1);
	glVertex3fv(p2);
	glVertex3fv(p5);
	glVertex3fv(p4);

	CalcNormal(p1, p3, p4, nv);

	//斜面
	glNormal3fv(nv);  //四个点一定要按照时针顺序来
	glVertex3fv(p1);
	glVertex3fv(p3);
	glVertex3fv(p6);
	glVertex3fv(p4);

	glEnd();
}

/*void CDrawArm::DrawScaleOutt()
{
	float a;
	a = scaleview*0.1;
	double i1, i2, i3;
	ifstream infile;
	infile.open("exworkspace.txt", ios::in);
	pointnum = 32400;

	GLfloat red_ambient[] = { 0.5f, 0.0f, 0.0f }; //环境色
	GLfloat red_diffuse[] = { 0.0f, 0.0f, 0.0f }; //扩散色
	GLfloat red_specular[] = { 0.1f, 0.1f, 0.1f }; //境面色
	GLfloat red_shininess[] = { 20.0f };			 //反射强度

	glMaterialfv(GL_FRONT, GL_AMBIENT, red_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, red_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, red_shininess);


	for (int j = 0; j <= pointnum; j++)
	{
		infile >> i1;
		infile >> i2;
		infile >> i3;
		//	if (j>32220||j<181)
		{
			glPushMatrix();
			//	glTranslated(*(posx+j)*a,*(posy+j)*a,*(posz+j)*a);
			glTranslated(-i1*a, -i2*a, (i3 + 550)*a);
			glutSolidSphere(0.01, 10, 10);
			glPopMatrix();
		}
	}

	infile.close();
}

void CDrawArm::DrawScaleIn()
{
	float a;
	a = scaleview*0.1;
	double i1, i2, i3;
	ifstream infile;
	infile.open("inworkspace.txt", ios::in);
	pointnum = 7381;

	GLfloat red_ambient[] = { 0.5f, 0.5f, 0.0f }; //环境色
	GLfloat red_diffuse[] = { 0.5f, 0.5f, 0.0f }; //扩散色
	GLfloat red_specular[] = { 0.1f, 0.1f, 0.1f }; //境面色
	GLfloat red_shininess[] = { 20.0f };			 //反射强度

	glMaterialfv(GL_FRONT, GL_AMBIENT, red_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, red_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, red_shininess);


	for (int j = 0; j <= pointnum; j++)
	{
		infile >> i1;
		infile >> i2;
		infile >> i3;
		//	if (j<180||j>4920)
		{
			glPushMatrix();
			//	glTranslated(*(posx+j)*a,*(posy+j)*a,*(posz+j)*a);
			glTranslated(-i1*a, -i2*a, (i3 + 550)*a);
			glutSolidSphere(0.01, 10, 10);
			glPopMatrix();
		}
	}

	infile.close();
}

void CDrawArm::DrawObstacle()
{
	float a;   //放大缩小比例系数
	a = scaleview;

	GLfloat green_ambient[] = { 0.0f, 0.2f, 0.0f };
	GLfloat green_diffuse[] = { 0.0f, 0.5f, 0.0f };
	GLfloat green_specular[] = { 0.6f, 0.7f, 0.6f };
	GLfloat green_shininess[] = { 32.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, green_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, green_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, green_shininess);

	glPushMatrix();
	glTranslated(35 * a, 35 * a, 0);
	SolidCylinder(5 * a, 250 * a, 30);
	glPopMatrix();
}
*/

