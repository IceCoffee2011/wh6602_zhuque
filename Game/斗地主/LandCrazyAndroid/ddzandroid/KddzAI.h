
//
// 2009
// DhLj
//

#ifndef _KDDZAI_H_
#define _KDDZAI_H_

// log
//#include <fstream>
//using namespace std;
// end log

#include "Roles.h"
//#include "RandNum.h"


// ������AI�㷨��
class KddzAI
{
public:
	KddzAI( Roles* rIn );
	virtual ~KddzAI( void );

	bool PushCards( const AIRUN& aiRun );
	bool GetLard( void );
	
private:
	Roles* m_RoleAI;

	int m_iHands;
	CWVector m_cwCards;															// ������������
	CWVector m_cwCutCards;														// WeaveCardsʹ�ñ���κ�������
	CWVector m_cwOrderCards;													// ��ϱ�
	CWVector m_cwIncome;														// ������Ȩ��
	CWVector m_cwOutgo;															// ֧����Ȩ��
	RCVector m_rcAll;															// δ֪��

	int GetNum( const int iBegin, const int iEnd );
	void ChangeCard( void );
	void CutCards( void );
	void SetOrder( void );
	int GetCard( const CARDCHANGE& rIn );
	bool WeaveCards( const CWVector& cwIn, const CWVector& cwOut, const CWVector& cwKate );
	bool GetIsIn( const CWVector& cwCut, const CWVector& cwCards );
	void DeleteCards( const CWVector& cwCut, CWVector& cwCards );
	void ChooseCards( const CARDCHANGE rcIn, const int iNum );
	int MathHands( void );
	void MathInOrOut( void );
	void MakeCardsTable( const AIRUN& aiRun );
	int FindOrderIncome( const CARDIF ci, const unsigned int iIn = 0 );
	int FindOrderOutgo( const CARDIF ci, const unsigned int iIn = 0 );
	bool FindBigOne( const CWVector& cwIn );
	bool FindSmall( const unsigned int iIn );
	bool FindTowOne( const CWVector& cwIn, const unsigned int iIn );
	bool FindTowOne( const CWVector& cwIn, const unsigned int iNum, const unsigned int iIn );
	bool GetBig( const CWVector& cwIn );
	bool GetBigOther( const AIRUN& aiRun );
	bool PushCardsMid( const AIRUN& aiRun );
	bool PushCardsUp( const AIRUN& aiRun );
	bool PushCardsDown( const AIRUN& aiRun );
	bool FindOne( const AIRUN& aiRun, const unsigned int iIn );
	bool FindOneOutgo( const AIRUN& aiRun, const unsigned int iIn, const bool bOneSide = false );
	bool FindOneIncome( const AIRUN& aiRun, const unsigned int iIn );
	void ChoseOneMid( const AIRUN& aiRun, const unsigned int iNum, const bool bIn );
	void ChoseOneUp( const AIRUN& aiRun, const unsigned int iNum, 
					  const bool bIn, const bool bOneDown, const bool bTwoDown );
	void ChoseOneDown( const AIRUN& aiRun, const unsigned int iNum, 
					  const bool bIn, const bool bOneUp, const bool bTwoUp );
	bool FindLink( const AIRUN& aiRun, const unsigned int iNum, const unsigned int iWeidth );
	bool FindLinkOutgo( const AIRUN& aiRun, const unsigned int iNum, const unsigned int iWeidth );
	bool FindLinkIncome( const AIRUN& aiRun, const unsigned int iNum, const unsigned int iWeidth );
	void ChoseLinkMid( const AIRUN& aiRun, const unsigned int iNum, const unsigned int iWeidth, const bool bIn );
	void ChoseLinkUp( const AIRUN& aiRun, const unsigned int iNum, const unsigned int iWeidth, 
					  const bool bIn, const bool bOneDown, const bool bTwoDown );
	void ChoseLinkDown( const AIRUN& aiRun, const unsigned int iNum, const unsigned int iWeidth, 
					    const bool bIn, const bool bOneUp, const bool bTwoUp );
	int GetHandNum( const bool bIn );
	bool ChoseOneOk( const AIRUN& aiRun, const unsigned int iIn );
	bool ChoseLinkOk( const AIRUN& aiRun, const unsigned int iNum, const unsigned int iWeidth );
	void PushZha( const AIRUN& aiRun );
	int UpEnough( void );



	// ����
	bool Patch2w( const bool bIn );
	bool Patch3And( void );
};



#endif