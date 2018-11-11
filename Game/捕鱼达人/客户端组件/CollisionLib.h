/*
**	CollisionLib.h
**	created by weni23
**	2011-10-16
**
**	����ײ���⣬������
*/

#ifndef __QC_COLLISIONLIB_H__
#define __QC_COLLISIONLIB_H__

#include "def.h"


//��һ���߶�תΪ����
qcRect Line_conv2_Rect(const qcLine& line);
qcRect Line_conv2_Rect(const qcPoint& p1, const qcPoint& p2);

//��һ�����νṹ��ת��Ϊ�ı��νṹ��
qcQuad Rect_conv2_Quad(const qcRect& rect);

//////////////////////////////////////////////////////////////////////////

bool Point_hit_Rect(const qcPoint& p, const qcRect& rect);

//����߶κ��߶��Ƿ��ཻ
bool Line_hit_Line(const qcLine& line1, const qcLine& line2);

bool Rect_hit_Rect(const qcRect& rect1, const qcRect& rect2);

// ���������Ҫ��Ϊ�ı�֤����εĵ����������͹�����
bool Quad_hit_Quad(const qcQuad& quad1, const qcQuad& quad2);

#endif//__QC_COLLISIONLIB_H__