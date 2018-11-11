
//
// 2009
// DhLj
//

#ifndef _ROLES_H_
#define _ROLES_H_

#include <afxwin.h> 
#include <afxext.h> 
#include <afxdisp.h>
#include "TagRoles.h"


//ת������
class Roles
{
public:
	Roles( void );
	virtual ~Roles( void );

	CARDSTATE m_Lard;												// dz״̬,�ǲ��ǵ���
	CARDSTATE m_Push;												// ����״̬,�ǲ��Ǹó���
	AIPOS m_pos;													//�Լ���λ��

	void SetCards( IVector& ivIn );							// ����			
	bool PushCard( const RCVector rcIn, const CARDCLASS cc );		// ����
	void ChooseCards( const int iNum );								// ���ȡ��
	void DeleteChoose( void );										// ���ѡ��
	inline CVector GetCards( void ) { return m_Cards; }				// ȡ����
	inline RCVector GetRCards( void ) { return m_RCards; }			// ȡѡ��(ת��)
	inline IVector GetChCards( void ) { return m_ivTemp; }			// ȡѡ��
	inline CARDCLASS GetCardClass( void ) { return m_ccTemp; }		// ȡѡ�Ƶ�����

	//����
	CARDCLASS GetCardRCVector(const RCVector& rcVec);
	bool ChangeCardToIvVertor(const IVector& ivIn, RCVector& rcVec);
	bool FindLinkTemp(const RCVector& rcVec, const int iOr, const int iF, const int iL );

private:		
	CVector m_Cards;												// ����.size()�Ƶ�����
	IVector m_ivTemp;												// ѡ����
	RCVector m_RCards;												// ѡ����ת��
	CARDCLASS m_ccTemp;												// ѡ������

	void ChangeCard( void );										// ת����
	CARDCLASS SelectClass( void );									// �������
	bool FindLink( const int iOr, const int iF, const int iL );		// ��˳��
	bool GetChoose( void );											// ȡѡ����
	void DeleteCard( void );										// ɾ��
	bool CompareCard( RCVector rcIn, CARDCLASS cc ) const ;			// �Ƚ�����
};


#endif