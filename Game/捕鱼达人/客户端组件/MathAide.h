#ifndef MATH_AIDE_HEAD_FILE
#define MATH_AIDE_HEAD_FILE
#pragma once

//��������
class CMathAide
{
	//��������
public:
	//���캯��
	CMathAide(void);
	//��������
	~CMathAide(void);

	//��������
public:
	//��������
	static void BuildBezier(int nInitX[], int nInitY[], int nInitCount, POINT ptBezier[], int nBuffertCount, WORD &nBezierCount, int nDistance);
	//����ֱ��
	static void BuildLinear(int nInitX[], int nInitY[], int nInitCount, POINT ptLinear[], int nBuffertCount, WORD &nLinearCount, int nDistance);
	//����ֱ��
	static void BuildLinear(int nInitX[], int nInitY[], int nInitCount, POINT ptLinear[], int nBuffertCount, WORD &nLinearCount, int nDistance, CRect &rcBound);
	//�������
	static LONG CalcDistance(int nX1, int nY1, int nX2, int nY2);
	//�������
	static LONG Combination(int nCount,int nR);
	//����׳�
	static LONG Factorial(int nNumber);
};

#endif