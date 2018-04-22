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

	//��ɫ
	GLfloat grey_ambient[] = { 0.2f, 0.2f, 0.2f }; //����ɫ
	GLfloat grey_diffuse[] = { 0.0f, 0.0f, 0.0f }; //��ɢɫ
	GLfloat grey_specular[] = { 0.1f, 0.1f, 0.1f }; //����ɫ
	GLfloat grey_shininess[] = { 20.0f };			 //����ǿ��

													 //�׻�ɫ
	GLfloat riceyellow_ambient[] = { 0.3f, 0.3f, 0.2f }; //����ɫ
	GLfloat riceyellow_diffuse[] = { 0.0f, 0.0f, 0.0f };   //��ɢɫ
	GLfloat riceyellow_specular[] = { 0.1f, 0.1f, 0.1f };    //����ɫ
	GLfloat riceyellow_shininess[] = { 1.0f };			   //����ǿ��

	glPushMatrix();

	//����Ĳ��ʶ��壺��
	glMaterialfv(GL_FRONT, GL_AMBIENT, grey_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grey_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, grey_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, grey_shininess);

	//���ϻ���
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

	//������ʵĶ��壺�׻�ɫ
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
	glTranslated(0, 0, 0);//ȷ����������
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


	//���������������n_div�����
	for (i = 0; i<n_div; i++)
	{
		angle = (2.0 * PI * i) / n_div;
		x = (GLfloat)cos(angle);
		y = (GLfloat)sin(angle);


		//�ϲ�������ζ������ʱ�붨��
		top_poly[i][0] = (GLfloat)radius * x;
		top_poly[i][1] = (GLfloat)radius * y;
		top_poly[i][2] = (GLfloat)height;


		//�²�������ζ������ʱ�붨��
		bottom_poly[i][0] = (GLfloat)radius * x;
		bottom_poly[i][1] = (GLfloat)radius * y;
		bottom_poly[i][2] = 0.0;
	}


	//�ϲ�������ε�����
	glBegin(GL_POLYGON);


	//�����ñ���ķ�������
	CalcNormal(top_poly[0], top_poly[1], top_poly[2], nv);//����ȷ��һƽ�棬Ȼ��ȷ��������
	glNormal3fv(nv);//ָ��������


					//����ʱ�뷽���ʾ������������
	for (i = 0; i<n_div; i++)
		glVertex3fv(top_poly[i]);
	glEnd();


	//�������棬�ı��ε���������
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= n_div; i++)
	{
		nv[0] = top_poly[i % n_div][0] / (GLfloat)radius;//ע�������%�����ú�����
		nv[1] = top_poly[i % n_div][1] / (GLfloat)radius;
		nv[2] = 0.0;
		glNormal3fv(nv);
		glVertex3fv(bottom_poly[i % n_div]);
		glVertex3fv(top_poly[i % n_div]);
	}
	glEnd();


	//�²�������������
	glBegin(GL_POLYGON);


	//�����ñ���ķ�������
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
		v0[i] = p2[i] - p1[i];//v0����
		v1[i] = p0[i] - p1[i];//v1����
	}

	// �ò��������
	normal[0] = v0[1] * v1[2] - v0[2] * v1[1];//x���ֵ
	normal[1] = v0[2] * v1[0] - v0[0] * v1[2];//y���ֵ
	normal[2] = v0[0] * v1[1] - v0[1] * v1[0];//z���ֵ

											  //���ߵ����滯��Ҳ���ǵ�λ��
	norm = (GLfloat)sqrt((double)(normal[0] * normal[0]
		+ normal[1] * normal[1]
		+ normal[2] * normal[2]));
	for (i = 0; i<3; i++)
		normal[i] /= norm;
}

void CDrawArm::DrawArmBody(float OS, float OL, float OU, float OR, float OB, float OT)
{




	//GLdouble height,radius;
	float a;   //�Ŵ���С����ϵ��
	a = scaleview;
	//������ʵĶ���

	//��ɫ��
	GLfloat red_ambient[] = { 0.2f, 0.0f, 0.0f }; //������
	GLfloat red_diffuse[] = { 0.5f, 0.0f, 0.0f }; //��ɢ��
	GLfloat red_specular[] = { 0.7f, 0.6f, 0.6f }; //�����
	GLfloat red_shininess[] = { 32.0f };			//  ����ǿ��
													//��ɫ
	GLfloat pink_ambient[] = { 0.2f, 0.0f, 0.2f }; //������
	GLfloat pink_diffuse[] = { 0.5f, 0.0f, 0.0f }; //��ɢ��
	GLfloat pink_specular[] = { 0.7f, 0.6f, 0.6f }; //�����
	GLfloat pink_shininess[] = { 32.0f };			//  ����ǿ��
													//��ɫ
	GLfloat green_ambient[] = { 0.0f, 0.2f, 0.0f };
	GLfloat green_diffuse[] = { 0.0f, 0.5f, 0.0f };
	GLfloat green_specular[] = { 0.6f, 0.7f, 0.6f };
	GLfloat green_shininess[] = { 32.0f };

	//��ɫ
	GLfloat blue_ambient[] = { 0.0f, 0.0f, 0.2f };
	GLfloat blue_diffuse[] = { 0.0f, 0.0f, 0.5f };
	GLfloat blue_specular[] = { 0.6f, 0.6f, 0.7f };
	GLfloat blue_shininess[] = { 32.0f };

	//��ɫ
	GLfloat yellow_ambient[] = { 0.3f, 0.3f, 0.0f };
	GLfloat yellow_diffuse[] = { 0.3f, 0.3f, 0.0f };
	GLfloat yellow_specular[] = { 0.7f, 0.7f, 0.5f };
	GLfloat yellow_shininess[] = { 32.0f };

	//��ɫ
	GLfloat black_ambient[] = { 0.0f, 0.0f, 0.0f };
	GLfloat black_diffuse[] = { 0.0f, 0.0f, 0.0f };
	GLfloat black_specular[] = { 0.7f, 0.7f, 0.7f };
	GLfloat black_shininess[] = { 32.0f };

	//��ɫ
	GLfloat grey_ambient[] = { 0.3f, 0.3f, 0.3f };
	GLfloat grey_diffuse[] = { 0.3f, 0.3f, 0.3f };
	GLfloat grey_specular[] = { 0.7f, 0.7f, 0.5f };
	GLfloat grey_shininess[] = { 32.0f };
	//���ɫ
	GLfloat darkgrey_ambient[] = { 0.2f, 0.2f, 0.2f };
	GLfloat darkgrey_diffuse[] = { 0.2f, 0.2f, 0.2f };
	GLfloat darkgrey_specular[] = { 0.7f, 0.7f, 0.5f };
	GLfloat darkgrey_shininess[] = { 32.0f };

	//��ɫ�̶���̨��
	/////////////////////////////////////////////////////////////
	//������ʵĶ��壺��
	glMaterialfv(GL_FRONT, GL_AMBIENT, black_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, black_shininess);

	glPushMatrix();
	SolidCube(32 * a, 32 * a, 10 * a);//�󷽺���
	glPopMatrix();

	//���ĸ�Сб��
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


	//���ɫ���ӿ��Ʋ�
	/////////////////////////////////////////////////////////////
	//������ʵĶ��壺���ɫ
	glTranslated(0.0, 0.0, 10 * a);  //��ԭ������Ų!!!!!!!!!!!!!!!!ûpush

	glMaterialfv(GL_FRONT, GL_AMBIENT, darkgrey_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, darkgrey_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, darkgrey_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, darkgrey_shininess);

	glPushMatrix();
	SolidCube(30 * a, 30 * a, 2 * a);
	glPopMatrix();

	glTranslated(0.0, 0.0, 2 * a);  //��ԭ������Ų!!!!!!!!!!!!!!!!ûpush

	glPushMatrix();
	SolidCylinder(15 * a, 15 * a, 16);//���������Ӳ��Ĵ�Բ��


	glTranslated(0.0, 0.0, 15 * a);  //��ԭ������Ų!!!!!!!!!!!!!!!!ûpush

									 //����ɫ���
	glMaterialfv(GL_FRONT, GL_AMBIENT, black_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, black_shininess);

	glPushMatrix();
	SolidCylinder(6 * a, 12 * a, 8);  //������󲿷�
	glTranslated(0.0, 0.0, 12 * a);
	SolidCylinder(4 * a, 5 * a, 8);   //�����С����
	glPopMatrix();

	//S����ת����
	/////////////////////////////////////////////////////////////
	//������ʵĶ��壺��ɫ
	glRotated(OS, 0, 0, 1);		//�˴��в�������һ����������S����ת�ĽǶ�ֵ��������������

	glMaterialfv(GL_FRONT, GL_AMBIENT, blue_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, blue_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, blue_shininess);

	glPushMatrix();
	SolidCylinder(17 * a, 8 * a, 30);//S������µĴ�Բ��
	glPopMatrix();


	glTranslated(0.0, 0.0, 8 * a);  //��ԭ������Ų!!!!!!!!!!!!!!!!ûpush
									
	glPushMatrix();
	glTranslated(0, 0, 0);
	SolidCube(9.2*a, 5 * a, 20 * a);//�����ŵĴ󳤷�����
	glPopMatrix();

	glPushMatrix();
	glTranslated(-12 * a, 3 * a, -3 * a);
	glRotated(-90, 0, 1, 0);
	glRotated(90, 0, 0, 1);
	SolidXieCube(5 * a, 6 * a, 3.2*a);    //��һ��С������б���ǣ�������
	glPopMatrix();

	glTranslated(0, 0.0, 20 * a); //��ԭ������Ų!!!!!!!!!!!!!!!!ûpush

	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	SolidCylinder(9 * a, 5 * a, 30); //�����ŵ��Ǹ���Բ
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, black_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, black_shininess);

	glPushMatrix();
	glTranslated(0, -14 * a, 0.0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(6 * a, 14.1*a, 8); //�����ŵ���Ĵ���ǲ���
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -18 * a, 0.0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(4.5*a, 4.1*a, 8); //�����ŵ����С���ǲ���	
	glPopMatrix();

	//L����ת����
	/////////////////////////////////////////////////////////////
	//������ʵĶ��壺��ɫ
	glRotated(OL, 0, -1, 0);		//�˴��в�������һ����������L����ת�ĽǶ�ֵ��������������

	glMaterialfv(GL_FRONT, GL_AMBIENT, pink_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, pink_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, pink_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, pink_shininess);

	glTranslated(0.0, 5 * a, 0.0);//��ԭ����ǰŲ!!!!!!!!!!!!!!!!ûpush

	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	SolidCylinder(8 * a, 4 * a, 30); //���������ӵ�С���ǲ���	
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 4 * a, 0.0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(9 * a, 3.2*a, 30); //���������ӵĴ���ǲ���	
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 7.2*a, 4.5*a);
	glRotated(-90, 1, 0, 0);
	SolidXieCube(12.2*a, 9 * a, 6.6*a);//����������µ�����
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 10.5*a, 9 * a);
	SolidCube(12.2*a, 6.6*a, 43.4*a);//��L��
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 7.2*a, 56.9*a);
	glRotated(-90, 1, 0, 0);
	glRotated(180, 0, 0, 1);
	SolidXieCube(12.2*a, 9 * a, 6.6*a);//���������ϵ�����
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 4.2*a, 61.4*a);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(7 * a, 3 * a, 30);   //�������ԭƬ
	glPopMatrix();

	//U����ת����
	/////////////////////////////////////////////////////////////
	//������ʵĶ��壺��ɫ
	glTranslated(0.0, 4.2*a, 61.4*a);//��ԭ����ǰŲ!!!!!!!!!!!!!!!!ûpush

	glMaterialfv(GL_FRONT, GL_AMBIENT, yellow_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellow_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, yellow_shininess);

	glRotated(-OU , 0, 1, 0);		//�˴��в�������һ����������U����ת�ĽǶ�ֵ��������������

	glPushMatrix();
	glTranslated(0.0, -2.5*a, 0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(6 * a, 2.5*a, 30);   //��ת�ᴦ��СԭƬ
	glPopMatrix();



	glMaterialfv(GL_FRONT, GL_AMBIENT, black_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, black_shininess);

	glPushMatrix();
	glTranslated(0, -19.5*a, 0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(6.5*a, 11 * a, 8);//���������ǲ���
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -24 * a, 0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(3.75*a, 4.5*a, 8);//�����С���ǲ���
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, yellow_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellow_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, yellow_shininess);

	glPushMatrix();
	//glTranslated(7.5*a, -9.2*a, 15.5*a);
	glTranslated(7.5*a, -9.2*a, 0);
	glRotated(-90, 0, 1, 0);
	SolidCylinder(6.5*a, 28.4*a, 30);//���ص���Ĵ�Բ��
	glPopMatrix();


	glMaterialfv(GL_FRONT, GL_AMBIENT, yellow_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellow_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, yellow_shininess);


	//R����ת����
	/////////////////////////////////////////////////////////////
	//������ʵĶ��壺��ɫ
	glTranslated(-20.9*a, -9.2*a, 0);//��ԭ��������Ų!!!!!!!!!!!!!!!!ûpush

	glRotated(OR, 1, 0, 0);		//�˴��в�������һ����������R����ת�ĽǶ�ֵ��������������

	glMaterialfv(GL_FRONT, GL_AMBIENT, green_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, green_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, green_shininess);

	glPushMatrix();
	glRotated(-90, 0, 1, 0);
	SolidCylinder(6.5*a, 2 * a, 30);//�������ŵ��Ǹ�СԭƬ
	glPopMatrix();

	glPushMatrix();
	glTranslated(-2 * a, 0, 0);
	glRotated(-90, 0, 1, 0);
	SolidCylinder(2.5*a, 14 * a, 30);//���������м��Բ��
	glPopMatrix();

	glPushMatrix();
	glTranslated(-9 * a, 0, -4.5*a);
	SolidCube(14 * a, 1.5*a, 9 * a);//�����ŵĿ�Ƭ
	glPopMatrix();

	glPushMatrix();
	glTranslated(-9 * a, 0, -0.75*a);
	SolidCube(14 * a, 9 * a, 1.5*a);//�����ŵĿ�Ƭ
	glPopMatrix();

	glPushMatrix();
	glTranslated(-15 * a, -6.5*a, 0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(4.8*a, 13 * a, 30);//��������Բ��
	glPopMatrix();

	glPushMatrix();
	glTranslated(-23.5*a, 0, -4.8*a);
	SolidCube(17 * a, 8.5*a, 9.6*a);//���м�ķ���
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, blue_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, blue_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, blue_shininess);

	glPushMatrix();
	glTranslated(-28.95*a, 5.25*a, -4.8*a);
	SolidCube(27.9*a, 2.5*a, 9.6*a);//��ǰ��ĵ���
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, yellow_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellow_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, yellow_shininess);

	glPushMatrix();
	glTranslated(-28.95*a, -5.25*a, -4.8*a);
	SolidCube(27.9*a, 2.5*a, 9.6*a);//�����ĵ���
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, green_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, green_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, green_shininess);
	glPushMatrix();
	glTranslated(-42.9*a, 4 * a, 0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(4.8*a, 2.5*a, 30);//���Ҳ�ǰ��ԭƬ
	glPopMatrix();

	glPushMatrix();
	glTranslated(-42.9*a, -6.5*a, 0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(4.8*a, 2.5*a, 30);//���Ҳ�ǰ��ԭƬ
	glPopMatrix();

	//B����ת����
	/////////////////////////////////////////////////////////////
	//������ʵĶ��壺��ɫ
	glTranslated(-42.9*a, 0, 0);//��ԭ��������Ų!!!!!!!!!!!!!!!!ûpush

	glRotated(OB, 0, -1, 0);		//�˴��в�������һ����������B����ת�ĽǶ�ֵ��������������
	glRotated(180, 0, -1, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, red_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, red_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, red_shininess);

	glPushMatrix();
	glTranslated(0, -4 * a, 0);
	glRotated(-90, 1, 0, 0);
	SolidCylinder(4.5*a, 8 * a, 30);//������ת��
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -5.5*a);
	SolidCube(6 * a, 6 * a, 4 * a);//С��������
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -10 * a);
	SolidCube(7 * a, 7 * a, 4.5*a);//��һ���������
	glPopMatrix();

	//B����ת����
	/////////////////////////////////////////////////////////////
	//������ʵĶ��壺��ɫ
	glTranslated(0, 0, -10 * a);//��ԭ������Ų!!!!!!!!!!!!!!!!ûpush

	glRotated(OT , 0, 0, 1);		//�˴��в�������һ����������T����ת�ĽǶ�ֵ������������

	glMaterialfv(GL_FRONT, GL_AMBIENT, yellow_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellow_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, yellow_shininess);

	glPushMatrix();
	glTranslated(0, 0, -0.6*a);
	SolidCylinder(2.5*a, 0.6*a, 30);//���ӵĻ�ɫ��
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, black_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, black_shininess);

	glPushMatrix();
	glTranslated(0, 0, -1.6*a);
	SolidCylinder(3.5*a, a, 30);//��ɫ������ĺ�ɫԲ����
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, grey_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grey_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, grey_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, grey_shininess);

	glPushMatrix();
	glTranslated(0, 0, -2.9*a);
	SolidCylinder(3.5*a, 1.3*a, 30);//��ɫ������Ļ�ɫԲ����
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -8.1*a);
	SolidCube(2.2*a, 4.1*a, 5.2*a);//��ץ��������
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, black_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, black_shininess);

	glPushMatrix();
	glTranslated(0, 0, -9 * a);
	SolidCube(2.2*a, 5 * a, 0.9*a);//��ץ��������
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.1*a, 0, -5.5*a);
	glRotated(90, 0, 1, 0);
	SolidCylinder(2 * a, 4.8*a, 30);//��ץ��Բ��
	glPopMatrix();

	//���������ǻ�����С��ץ
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
	//����ץǰ���
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

	//����ץ����
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

	glBegin(GL_QUADS);//���ƶ���ı���

					  //�Ҳ�	
	glNormal3fv(m2);  //�ĸ���һ��Ҫ����ʱ��˳����
	glVertex3fv(p1);
	glVertex3fv(p2);
	glVertex3fv(p3);
	glVertex3fv(p4);

	//���	
	glNormal3fv(m1);
	glVertex3fv(p5);
	glVertex3fv(p6);
	glVertex3fv(p7);
	glVertex3fv(p8);

	//ǰ��	
	glNormal3fv(m3);
	glVertex3fv(p1);
	glVertex3fv(p2);
	glVertex3fv(p6);
	glVertex3fv(p5);

	//����	
	glNormal3fv(m4);
	glVertex3fv(p4);
	glVertex3fv(p3);
	glVertex3fv(p7);
	glVertex3fv(p8);

	//����	
	glNormal3fv(m5);
	glVertex3fv(p2);
	glVertex3fv(p3);
	glVertex3fv(p7);
	glVertex3fv(p6);

	//����	
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

	glBegin(GL_TRIANGLES);//���ƶ���ı���

						  //�Ҳ�	
	glNormal3fv(m2);  //�ĸ���һ��Ҫ����ʱ��˳����
	glVertex3fv(p1);
	glVertex3fv(p2);
	glVertex3fv(p3);

	//���	
	glNormal3fv(m1);  //�ĸ���һ��Ҫ����ʱ��˳����
	glVertex3fv(p4);
	glVertex3fv(p5);
	glVertex3fv(p6);

	glEnd();

	glBegin(GL_QUADS);

	//����
	glNormal3fv(m4);  //�ĸ���һ��Ҫ����ʱ��˳����
	glVertex3fv(p2);
	glVertex3fv(p3);
	glVertex3fv(p6);
	glVertex3fv(p5);

	//����
	glNormal3fv(m6);  //�ĸ���һ��Ҫ����ʱ��˳����
	glVertex3fv(p1);
	glVertex3fv(p2);
	glVertex3fv(p5);
	glVertex3fv(p4);

	CalcNormal(p1, p3, p4, nv);

	//б��
	glNormal3fv(nv);  //�ĸ���һ��Ҫ����ʱ��˳����
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

	GLfloat red_ambient[] = { 0.5f, 0.0f, 0.0f }; //����ɫ
	GLfloat red_diffuse[] = { 0.0f, 0.0f, 0.0f }; //��ɢɫ
	GLfloat red_specular[] = { 0.1f, 0.1f, 0.1f }; //����ɫ
	GLfloat red_shininess[] = { 20.0f };			 //����ǿ��

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

	GLfloat red_ambient[] = { 0.5f, 0.5f, 0.0f }; //����ɫ
	GLfloat red_diffuse[] = { 0.5f, 0.5f, 0.0f }; //��ɢɫ
	GLfloat red_specular[] = { 0.1f, 0.1f, 0.1f }; //����ɫ
	GLfloat red_shininess[] = { 20.0f };			 //����ǿ��

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
	float a;   //�Ŵ���С����ϵ��
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

