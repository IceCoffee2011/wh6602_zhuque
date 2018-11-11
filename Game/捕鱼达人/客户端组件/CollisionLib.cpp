/*
**	CollisionLib.cpp
**	created by weni23
**	2011-10-16
**
**	����ײ���⣬������
*/

#include "Stdafx.h"

#include <vector>

#include "CollisionLib.h"

qcRect Line_conv2_Rect(const qcLine& line)
{
	qcRect _rect;
	if(line.point[0].x < line.point[1].x)
		_rect.x = line.point[0].x;
	else
		_rect.x = line.point[1].x;

	if(line.point[0].y < line.point[1].y)
		_rect.y = line.point[0].y;
	else
		_rect.y = line.point[1].y;

	_rect.w = abs(line.point[0].x - line.point[1].x);
	_rect.h = abs(line.point[0].y - line.point[1].y);

	return _rect;
}

qcRect Line_conv2_Rect(const qcPoint& p1, const qcPoint& p2)
{
	qcLine _line(p1, p2);
	return Line_conv2_Rect(_line);
}

qcQuad Rect_conv2_Quad(const qcRect& rect)
{
	qcQuad _quad(qcPoint(rect.x,		rect.y),
		qcPoint(rect.x+rect.w, rect.y),
		qcPoint(rect.x+rect.w, rect.y+rect.h),
		qcPoint(rect.x,		rect.y+rect.h));
	return _quad;
}

//////////////////////////////////////////////////////////////////////////

bool Point_hit_Rect(const qcPoint& p, const qcRect& rect)
{
	return (p.x > rect.x && p.x < rect.x+rect.w && p.y > rect.y && p.y < rect.y+rect.h);
}

bool Line_hit_Line(const qcLine& line1, const qcLine& line2)
{
	qcRect _rect1 = Line_conv2_Rect(line1);
	qcRect _rect2 = Line_conv2_Rect(line2);

	//����ֱ��������߶�Ϊ�Խ��߹��ɵľ��β��ཻ��˵���������߶β������ཻ������
	if(Rect_hit_Rect(_rect1, _rect2) == false)
		return false;

	int value1 = ((line1.point[0].x - line2.point[0].x)*(line2.point[1].y - line2.point[0].y)
		-(line2.point[1].x - line2.point[0].x)*(line1.point[0].y - line2.point[0].y))
		*((line2.point[1].x - line2.point[0].x)*(line1.point[0].y - line2.point[0].y)
		-(line1.point[1].x - line2.point[0].x)*(line2.point[1].y - line2.point[0].y));

	int value2 =((line2.point[0].x - line1.point[0].x)*(line1.point[1].y - line1.point[0].y)
		-(line1.point[1].x - line1.point[0].x)*(line2.point[0].y - line1.point[0].y))
		*((line1.point[1].x - line1.point[0].x)*(line2.point[1].y - line1.point[0].y)
		-(line2.point[1].x - line1.point[0].x)*(line1.point[1].y - line1.point[0].y));

	return (value1>= 0 && value2 >= 0 );
}

bool Rect_hit_Rect(const qcRect& rect1, const qcRect& rect2)
{
	if(rect1.x+rect1.w < rect2.x) return false;		//����ұ��ҵ����󣬲��ཻ
	if(rect1.x > rect2.x + rect2.w) return false;	//�ҵ��������һ��ң����ཻ
	if(rect1.y+rect1.h < rect2.y) return false;		//�ϵ��±��µ��ϻ��ϣ����ཻ
	if(rect1.y > rect2.y + rect2.h) return false;	//�µ��ϱ��ϵ��»��£����ཻ

	return true;	//�������Ҷ��ų��ˣ����ཻ��
}

bool Quad_hit_Quad(const qcQuad& quad1, const qcQuad& quad2)
{
	for(int i=0; i<4; ++i)
	{
		qcLine _line1(quad1.point[i], quad1.point[(i+1)%4]);
		for (int j=0; j<4; ++j)
		{
			qcLine _line2(quad2.point[j], quad2.point[(j+1)%4]);

			if(Line_hit_Line(_line1, _line2))
				return true;
		}
	}

	return false;
}

