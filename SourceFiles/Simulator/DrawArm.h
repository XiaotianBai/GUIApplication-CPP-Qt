// DrawArm.h: interface for the CDrawArm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWARM_H__7F3FC4AD_C5D5_4AC9_997F_7C39EA8105F9__INCLUDED_)
#define AFX_DRAWARM_H__7F3FC4AD_C5D5_4AC9_997F_7C39EA8105F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDrawArm  
{
public:
	void SolidXieCube(GLfloat Lenght,GLfloat Width,GLfloat Height);
	void SolidCube(GLfloat Length,GLfloat Width,GLfloat Height);
	void DrawArmBody(float OS,float OL,float OU,float OR,float OB,float OT);
	void CalcNormal(GLfloat *p0,GLfloat *p1,GLfloat *p2,GLfloat *normal);
	void SolidCylinder(GLdouble radius,GLdouble height,int n_div);
	void DrawDynamicGoods();
	void DrawFloor(double floorSize);
	void DrawStaticGoods();
	CDrawArm();
	virtual ~CDrawArm();

};

#endif // !defined(AFX_DRAWARM_H__7F3FC4AD_C5D5_4AC9_997F_7C39EA8105F9__INCLUDED_)
