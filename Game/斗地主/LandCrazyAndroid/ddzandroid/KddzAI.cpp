
//
// 2009
// DhLj
//

#include "KddzAI.h"

//
// *
//
KddzAI::KddzAI( Roles* rIn ) : m_RoleAI(NULL), m_iHands(15)
{
	m_RoleAI = rIn;	
}


//
// *
//
KddzAI::~KddzAI( void )
{
}


//
// ת�����Ե������ƴ���m_rcCards
//
void KddzAI::ChangeCard( void )
{
	CARDWORTH cwTemp;
	int iTemp = 0;

	// ������|�Ѿ�û����
	if ( m_RoleAI->GetCards().empty() )
	{
		return;
	}

	// ���m_cwCards
	if ( !m_cwCards.empty() )
	{
		m_cwCards.erase( m_cwCards.begin(), m_cwCards.end() );
	}

	// ������
	for ( int i = 0; i < m_RoleAI->GetCards().size(); ++i )
	{
		if ( m_RoleAI->GetCards()[i].iCard > 0 &&
			 m_RoleAI->GetCards()[i].iCard <= 52 )			// �ų���С��
		{
			iTemp = ( m_RoleAI->GetCards()[i].iCard - 1 ) / 4 + 3;
		}
		else
		{
			iTemp = m_RoleAI->GetCards()[i].iCard;
		}
		
		switch ( iTemp )
		{
		case 3:
			cwTemp.rcard.rc = R3;
			break;
		case 4:
			cwTemp.rcard.rc = R4;
			break;
		case 5:
			cwTemp.rcard.rc = R5;
			break;
		case 6:
			cwTemp.rcard.rc = R6;
			break;
		case 7:
			cwTemp.rcard.rc = R7;
			break;
		case 8:
			cwTemp.rcard.rc = R8;
			break;
		case 9:
			cwTemp.rcard.rc = R9;
			break;
		case 10:
			cwTemp.rcard.rc = RO;
			break;
		case 11:
			cwTemp.rcard.rc = RJ;
			break;
		case 12:
			cwTemp.rcard.rc = RQ;
			break;
		case 13:
			cwTemp.rcard.rc = RK;
			break;
		case 14:
			cwTemp.rcard.rc = RA;
			break;
		case 15:
			cwTemp.rcard.rc = RT;
			break;
		case 53:
			cwTemp.rcard.rc = RWS;
			break;
		case 54:
			cwTemp.rcard.rc = RWB;
			break;
		default:
			break;
		}
		
		// ����Ҫ��֤VECTOR����������(��->С)
		if ( !m_cwCards.empty() && m_cwCards[m_cwCards.size() - 1].rcard.rc == cwTemp.rcard.rc )
		{
			++m_cwCards[m_cwCards.size() - 1].rcard.iNum;
		}
		else
		{
			cwTemp.rcard.iNum = 1;
			cwTemp.csif = sde;
			m_cwCards.push_back( cwTemp );
		}
	}
}


//
// ����(ת��)����ȡm_RoleAI->GetCards()������ֵ
//
int KddzAI::GetCard( const CARDCHANGE& rIn )
{
	int ivTemp;

	switch ( rIn )
	{
	case R3:
	case R4:
	case R5:
	case R6:
	case R7:
	case R8:
	case R9:
	case RO:
	case RJ:
	case RQ:
	case RK:
	case RA:
	case RT:
		{
			ivTemp = 4 * rIn;
		}
		break;	
	case RWS:
		{
			ivTemp = 53;			// ����
		}
		break;
	case RWB:
		{
			ivTemp = 54;
		}
	default:
		break;
	}

	// �Ǵ�С��
	if ( ivTemp == 53 || ivTemp == 54 )
	{
		// Ѱ�Ҵ�С��������
		for ( int i = 0; i < m_RoleAI->GetCards().size(); ++i )
		{		
			if ( m_RoleAI->GetCards()[i].iCard == ivTemp )
			{
				// ��������
				return i;
			}
		}
	}

	//	���Ǵ�С��		
	else  
	{
		// Ѱ�ҷ����������С����
		for ( int j = m_RoleAI->GetCards().size() - 1; j >= 0; --j )
		{
			if ( m_RoleAI->GetCards()[j].iCard > ivTemp &&
				m_RoleAI->GetCards()[j].iCard <= ivTemp + 4 )
			{
				return j;
			}
		}
	}

	// ���س���
	return -1;
}

//
// ��������
//
int KddzAI::MathHands( void )
{
	int iHandTemp = 0;
	for ( int m = 0; m < m_cwOrderCards.size(); ++m )
	{					
		if ( m_cwOrderCards[m].csif == sde || m_cwOrderCards[m].csif == end )
		{
			++iHandTemp;
		}	
		if ( m_cwOrderCards[m].rcard.iNum == 3 )
		{
			--iHandTemp;
		}
		if ( m_cwOrderCards[m].rcard.iNum == 4 )
		{
			iHandTemp -= 2;
		}
	}

	return iHandTemp;
}



//
// ��ȡ���������
//
int KddzAI::GetNum( const int iBegin, const int iEnd )
{
	int iTemp = 0;

	while ( 1 )
	{
		if ( ( iTemp = rand() % iEnd ) > iBegin	)		// �������
		{
			break;
		}
	}

	return iTemp;
}


//
// �ж��Ƿ�е���
//
bool KddzAI::GetLard( void )
{
	ChangeCard();
	CutCards();	

	AIRUN aiRun;
	MakeCardsTable( aiRun );
	MathInOrOut();

	// ��������
	int iHandTemp = MathHands();
	int iHand = 0;

	if ( m_cwOrderCards.size()>=1 &&
		 m_cwOrderCards[0].rcard.rc == RWB && 
		 m_cwOrderCards.size()>=2 &&
		 m_cwOrderCards[1].rcard.rc == RWS )
	{
		iHandTemp -= 1;
	}

	// �˴����Ե��ڵ��Խе���(�Ѷ�)
	// 8-10�ֿ��Գ���
	if ( iHandTemp <= GetNum( 8, 10 ) )
	{
		for ( int i = 0; i < m_cwIncome.size(); ++i )
		{
			if ( m_cwIncome[i].csif == sde ||
				 m_cwIncome[i].csif == beg )
			{
				++iHand;
			}
		}

		// ����2�ִ��ƲŽ�
		if ( iHand > 1 )
		{
			return true;
		}
	}

	return false;
}


//
// �ж��������Ƿ��ڵ�ǰ������
//
bool KddzAI::GetIsIn( const CWVector& cwCut, const CWVector& cwCards )
{
	if ( cwCards.empty() || cwCut.empty() )
	{
		return false;
	}

	for ( int i = 0; i < cwCut.size(); ++i )
	{
		int j = 0;
		for (j = 0; j < cwCards.size(); ++j )
		{
			if ( cwCut[i].rcard.rc == cwCards[j].rcard.rc &&
				 cwCut[i].rcard.iNum <= cwCards[j].rcard.iNum )
			{
				// �����ڲ�ѭ��
				break;
			}
		}
		// ѭ�����е���û�ҵ�ƥ�����
		if ( j >= cwCards.size() )
		{
			return false;
		}
	}

	return true;
}


//
// ɾ���������е����
//
void KddzAI::DeleteCards( const CWVector& cwCut, CWVector& cwCards )
{
	if ( cwCards.empty() || cwCut.empty() )
	{
		return;
	}

	for ( int i = 0; i < cwCut.size(); ++i )
	{
		for ( int j = 0; j < cwCards.size(); ++j )
		{
			if ( cwCut[i].rcard.rc == cwCards[j].rcard.rc )
			{
				cwCards[j].rcard.iNum -=  cwCut[i].rcard.iNum;
				if ( cwCards[j].rcard.iNum <= 0 )
				{
					cwCards.erase( cwCards.begin() + j );
				}
				
				// �����ڲ�ѭ��
				break;
			}
		}
	}
}



//
// �ݹ�
//
bool KddzAI::WeaveCards( const CWVector& cwIn, const CWVector& cwOut, const CWVector& cwKate )
{			
	// cwKate:һ���ڵ����м����Ƶļ���
	int i = 0;
	CWVector cwCut;
	while ( i < cwKate.size() )
	{
		// cwIn:��ǰ������
		CWVector cwTemp = cwIn;

		// cwOut:��Ϻ����
		CWVector cwOutTemp = cwOut;

		// ȡ����������Ƭ��		
		while ( i < cwKate.size() )
		{
			cwCut.push_back( cwKate[i] );
			if ( cwKate[i].csif == sde || cwKate[i].csif == end )
			{
				++i;
				break;
			}
			++i;
		}

		// ����������Ƿ��ڵ�ǰ������
		if ( GetIsIn( cwCut, cwTemp ) )
		{
			// �ڵ�ǰ������ɾ��������
			DeleteCards( cwCut, cwTemp );

			// ��������Ƶ�cwOutTemp
			for ( int k = 0; k < cwCut.size(); ++k )
			{
				cwOutTemp.push_back( cwCut[k] );
			}

			// �ݹ�
			if ( WeaveCards( cwTemp, cwOutTemp, cwKate ) )
			{
				// ���ʣ����
				for ( int j = 0; j < cwTemp.size(); ++j )
				{
					cwOutTemp.push_back( cwTemp[j] );
				}

				// ��������
				int iHandTemp = 0;
				for ( int m = 0; m < cwOutTemp.size(); ++m )
				{					
					if ( cwOutTemp[m].csif == sde || cwOutTemp[m].csif == end )
					{
						++iHandTemp;
					}	
					if ( cwOutTemp[m].rcard.iNum == 3 )
					{
						--iHandTemp;
					}
					if ( cwOutTemp[m].rcard.iNum == 4 )
					{
						iHandTemp -= 2;
					}
				}
				if ( iHandTemp < m_iHands )
				{
					m_iHands = iHandTemp;	
					m_cwCutCards = cwOutTemp;
				}							
			}
		}

		if ( !cwCut.empty() )
		{
			cwCut.erase( cwCut.begin(), cwCut.end() );
		}
	}

	return true;
}



//
// ������,����WeaveCards()ѡ����������
//
void KddzAI::CutCards( void )
{
	CWVector cwTemp;
	CWVector cwLinkTemp;
	CWVector cwKate;
	CARDWORTH cwKTemp;
	CWVector cwOut;

	// ���Ʒ���
	if ( m_cwCards.empty() )
	{
		return;
	}

	// ���
	if ( !m_cwOrderCards.empty() )
	{
		m_cwOrderCards.erase( m_cwOrderCards.begin(), m_cwOrderCards.end() );
	}

	cwTemp = m_cwCards;

	// ȥ������2,��������
	if ( cwTemp.size() >= 2 &&
		 cwTemp[0].rcard.rc == RWB &&
		 cwTemp[1].rcard.rc == RWS )
	{
		cwTemp[0].csif = beg;
		cwTemp[1].csif = end;
		m_cwOrderCards.push_back( cwTemp[0] );
		m_cwOrderCards.push_back( cwTemp[1] );
		cwTemp.erase( cwTemp.begin(), cwTemp.begin() + 2 );
	}

	for ( int i = 0; i < cwTemp.size(); ++i )
	{	
		if ( cwTemp[i].rcard.rc > RA )
		{
			// ������Ʊ�
			m_cwOrderCards.push_back( cwTemp[i] );
			cwTemp.erase( cwTemp.begin() + i-- );
		}
		else
		{
			break;
		}
	}

	// ��,�Զ�Ϊ��
	while ( !cwTemp.empty() )
	{
		// ��һλ�϶�Ҫ��
		cwLinkTemp.push_back( cwTemp[0] );

		// �Ҷϵ�
		for ( int j = 1; j < cwTemp.size(); ++j )
		{
			// ����
			if ( cwTemp[0].rcard.rc - cwTemp[j].rcard.rc == j )
			{
				cwLinkTemp.push_back( cwTemp[j] );			
			}
			else
			{
				break;
			}
		}

		// ɾ��ѡ��
		cwTemp.erase( cwTemp.begin(), cwTemp.begin() + cwLinkTemp.size() );

		//
		// loop1:Ѱ�����п��ܵ�˳,˫˳,��˳,��˳��,����,��,�Ĵ�
		//

		// ��˳
		for ( int ka = 0; ka + 5 <= cwLinkTemp.size(); ++ka )
		{
			for ( int ma = 5; ma + ka <= cwLinkTemp.size(); ++ma )
			{
				int kaTemp = ka;
				while ( kaTemp < ma + ka )
				{		
					cwKTemp.rcard.rc = cwLinkTemp[kaTemp].rcard.rc;
					cwKTemp.rcard.iNum = 1;								// ��
					if ( kaTemp == ka )	
					{
						cwKTemp.csif = beg;
					}
					else if ( kaTemp == ma + ka - 1 )
					{
						cwKTemp.csif = end;
					}
					else
					{
						cwKTemp.csif = mid;
					}
					cwKate.push_back( cwKTemp );
					++kaTemp;
				}				
			}
		}	

		// ˫˳
		for ( int kb = 0; kb + 3 <= cwLinkTemp.size(); ++kb )
		{
			for ( int mb = 3; mb + kb <= cwLinkTemp.size(); ++mb )
			{
				int kbTemp = kb;
				while ( kbTemp < mb + kb )
				{
					// ������������
					if ( cwLinkTemp[kbTemp].rcard.iNum < 2 )
					{
						break;
					}
					++kbTemp;
				}	
				if ( kbTemp >= mb + kb )
				{
					// �������˫˳
					kbTemp = kb;
					while ( kbTemp < mb + kb )
					{
						cwKTemp.rcard.rc = cwLinkTemp[kbTemp].rcard.rc;
						cwKTemp.rcard.iNum = 2;								// ˫
						if ( kbTemp == kb )	
						{
							cwKTemp.csif = beg;
						}
						else if ( kbTemp == mb + kb - 1 )
						{
							cwKTemp.csif = end;
						}
						else
						{
							cwKTemp.csif = mid;
						}
						cwKate.push_back( cwKTemp );
						++kbTemp;
					}	
				}
				else
				{
					// ����ѭ��,cwLinkTemp����λ�ƶ�
					break;
				}
			}
		}

		// ��˳��(���Ⱑ)
		for ( int kc = 0; kc + 2 <= cwLinkTemp.size(); ++kc )
		{
			for ( int mc = 2; mc + kc <= cwLinkTemp.size(); ++mc )
			{
				int kcTemp = kc;
				while ( kcTemp < mc + kc )
				{
					if ( cwLinkTemp[kcTemp].rcard.iNum < 3 )
					{
						break;
					}
					++kcTemp;
				}	
				if ( kcTemp >= mc + kc )
				{
					kcTemp = kc;
					while ( kcTemp < mc + kc )
					{
						cwKTemp.rcard.rc = cwLinkTemp[kcTemp].rcard.rc;
						cwKTemp.rcard.iNum = 3;								// ��
						if ( kcTemp == kc )	
						{
							cwKTemp.csif = beg;
						}
						else if ( kcTemp == mc + kc - 1 )
						{
							cwKTemp.csif = end;
						}
						else
						{
							cwKTemp.csif = mid;
						}
						cwKate.push_back( cwKTemp );
						++kcTemp;
					}	
				}
				else
				{
					// ����ѭ��,cwLinkTemp����λ�ƶ�
					break;
				}
			}
		}

		// ����,�Ĵ�
		for ( int kd = 0; kd < cwLinkTemp.size(); ++kd )
		{
			if ( cwLinkTemp[kd].rcard.iNum >= 3 )
			{			
				cwKTemp.rcard.rc = cwLinkTemp[kd].rcard.rc;
				cwKTemp.rcard.iNum = cwLinkTemp[kd].rcard.iNum;
				cwKTemp.csif = sde;
				cwKate.push_back( cwKTemp );
			}		
		}	

		//
		// end loop1
		//

		// �к�������,m_cwCutCards�϶��ǿ�
		if ( !cwKate.empty() )
		{
			WeaveCards( cwLinkTemp, cwOut, cwKate );
			
			for ( int nb = 0; nb < m_cwCutCards.size(); ++nb )
			{
				m_cwOrderCards.push_back( m_cwCutCards[nb] );
			}

			m_cwCutCards.erase( m_cwCutCards.begin(), m_cwCutCards.end() );
		}
		
		//û����������ֱ����ϵ�ǰ���浽m_cwOrderCards
		else		
		{
			for ( int na = 0; na < cwLinkTemp.size(); ++na )
			{
				m_cwOrderCards.push_back( cwLinkTemp[na] );
			}
		}

		// ��ԭ���
		m_iHands = 15;
		if ( !cwLinkTemp.empty() )				// �쳣��������������
		{
			cwLinkTemp.erase( cwLinkTemp.begin(), cwLinkTemp.end() );
		}
		if ( !cwKate.empty() )
		{
			cwKate.erase( cwKate.begin(), cwKate.end() );
		}
		if ( !cwOut.empty() )
		{
			cwOut.erase( cwOut.begin(), cwOut.end() );
		}
	}
}


//
// ����
//
void KddzAI::SetOrder( void )
{
	CWVector cw;
	CWVector cwTemp = m_cwOrderCards;

	// �ȶ���С��
	while ( !cwTemp.empty() )
	{
		// �ҵ���һ��
		int iTemp = 0;
		while ( iTemp < cwTemp.size() )
		{
			if ( cwTemp[iTemp].csif == sde ||
				 cwTemp[iTemp].csif == end )
			{
				break;
			}
			++iTemp;
		}

		// �ȶ�
		for ( int i = iTemp + 1; i < cwTemp.size(); ++i )
		{
			if ( cwTemp[i].csif == sde ||
				 cwTemp[i].csif == end )
			{
				// ѡ��
				if ( cwTemp[iTemp].rcard.rc < cwTemp[i].rcard.rc )
				{
					iTemp = i;
				}
			}
		}

		// ��BEG
		while ( iTemp >= 0 )
		{
			if ( cwTemp[iTemp].csif == sde ||
				 cwTemp[iTemp].csif == beg )
			{
				break;
			}
			--iTemp;
		}

		// ���������

		while ( iTemp < cwTemp.size() )
		{
			CARDWORTH cwcw = cwTemp[iTemp];

			cw.push_back( cwTemp[iTemp] );
			cwTemp.erase( cwTemp.begin() + iTemp );

			if ( cwcw.csif == sde ||
				 cwcw.csif == end )
			{
				break;
			}
		}
		
	}

	m_cwOrderCards = cw;
}

//
// ���������֧����
//
void KddzAI::MathInOrOut( void )
{
	CWVector m_cwTemp = m_cwOrderCards;
	
	// ���Ʒ���
	if ( m_cwTemp.empty() )
	{
		return;
	}

	if ( !m_cwIncome.empty() )
	{
		m_cwIncome.erase( m_cwIncome.begin(), m_cwIncome.end() );
	}

	if ( !m_cwOutgo.empty() )
	{
		m_cwOutgo.erase( m_cwOutgo.begin(), m_cwOutgo.end() );
	}

	int iCut = 0;
	while ( iCut < m_cwTemp.size() )
	{
		CWVector m_cwCut;

		// ����
		int iTempWhile = iCut;
		while ( iTempWhile < m_cwTemp.size() )
		{
			if ( m_cwTemp[iTempWhile].csif == sde || 
				 m_cwTemp[iTempWhile].csif == end )
			{
				break;
			}
			++iTempWhile;
		}

		// ѡ��
		while ( iCut <= iTempWhile )
		{
			m_cwCut.push_back( m_cwTemp[iCut++] );
		}

		// ������
		if ( GetBig( m_cwCut ) )
		{
			for ( int i = 0; i < m_cwCut.size(); ++i )
			{
				m_cwIncome.push_back( m_cwCut[i] );
			}
		}

		// ��֧��
		else
		{
			for ( int j = 0; j < m_cwCut.size(); ++j )
			{
				m_cwOutgo.push_back( m_cwCut[j] );
			}
		}
	}
}


//
// �ж��Ƿ�Ϊ������
//
bool KddzAI::GetBig( const CWVector& cwIn )
{	
	switch ( cwIn[0].rcard.iNum )
	{
	case 1:							
		{
			if ( cwIn.size() == 1 )				// ��
			{
				for ( int i = 0; i < m_rcAll.size(); ++i )
				{
					if ( m_rcAll[i].rc > cwIn[0].rcard.rc &&
						 m_rcAll[i].iNum >= 1 )
					{
						return false;
					}
				}
				return true;
			}
			else if ( cwIn.size() == 2 )		// ����
			{
				return true;
			}
			else								// ��˳
			{
				for ( int i = 0; i < m_rcAll.size(); ++i )
				{
					if ( m_rcAll[i].rc <= RA &&
						 m_rcAll[i].rc > cwIn[0].rcard.rc &&
						 m_rcAll[i].iNum >= 1 )
					{
						int iAdd = 1;
						for ( int j = i + 1; j < m_rcAll.size(); ++j )
						{
							if ( m_rcAll[j].iNum >= 1 &&
								 m_rcAll[j - 1].rc - m_rcAll[j].rc == 1 )
							{
								++iAdd;
							}
							else
							{
								break;
							}
						}
						if ( iAdd >= cwIn.size() )
						{
							return false;
						}
					}
				}
				return true;
			}
		}
		break;
	case 2:							
		{
			if ( cwIn.size() == 1 )				// ��
			{
				for ( int i = 0; i < m_rcAll.size(); ++i )
				{
					if ( m_rcAll[i].rc > cwIn[0].rcard.rc &&
						 m_rcAll[i].iNum >= 2 )
					{
						return false;
					}
				}
				return true;
			}
			else								// ˫˳
			{
				for ( int i = 0; i < m_rcAll.size(); ++i )
				{
					if ( m_rcAll[i].rc <= RA &&
						 m_rcAll[i].rc > cwIn[0].rcard.rc &&
						 m_rcAll[i].iNum >= 2 )
					{
						int iAdd = 1;
						for ( int j = i + 1; j < m_rcAll.size(); ++j )
						{
							if ( m_rcAll[j].iNum >= 2 &&
								 m_rcAll[j - 1].rc - m_rcAll[j].rc == 1 )
							{
								++iAdd;
							}
							else
							{
								break;
							}
						}
						if ( iAdd >= cwIn.size() )
						{
							return false;
						}
					}
				}
				return true;
			}
		}
		break;
	case 3:							
		{
			if ( cwIn.size() == 1 )				// ��
			{
				for ( int i = 0; i < m_rcAll.size(); ++i )
				{
					if ( m_rcAll[i].rc > cwIn[0].rcard.rc &&
						 m_rcAll[i].iNum >= 3 )
					{
						return false;
					}
				}
				return true;
			}
			else								// ��˳
			{
				for ( int i = 0; i < m_rcAll.size(); ++i )
				{
					if ( m_rcAll[i].rc <= RA &&
						 m_rcAll[i].rc > cwIn[0].rcard.rc &&
						 m_rcAll[i].iNum >= 3 )
					{
						int iAdd = 1;
						for ( int j = i + 1; j < m_rcAll.size(); ++j )
						{
							if ( m_rcAll[j].iNum >= 3 &&
								 m_rcAll[j - 1].rc - m_rcAll[j].rc == 1 )
							{
								++iAdd;
							}
							else
							{
								break;
							}
						}
						if ( iAdd >= cwIn.size() )
						{
							return false;
						}
					}
				}
				return true;
			}
		}
		break;
	case 4:	
		{
			// ը�������ൽ������
			return true;
		}
	default:
		break;
	}

	return false;
}



//
// �ж�ͬ��������ǲ������
//
bool KddzAI::GetBigOther( const AIRUN& aiRun )
{
	CWVector cwTemp;
	CARDWORTH cw;

	cw.csif = sde;
	cw.rcard.iNum = aiRun.rcIn[0].iNum;
	cw.rcard.rc = aiRun.rcIn[0].rc;
	cwTemp.push_back( cw );

	if ( GetBig( cwTemp ) )
	{
		return true;
	}

	return false;
}


//
// ��ϳ��Ʊ�,ȡ����
//
void KddzAI::MakeCardsTable( const AIRUN& aiRun )
{
	RCARD rTemp;
	RCVector rcTemp;
	RCVector rcAll;

	if ( !aiRun.rcAll.empty() )
	{
		rcAll = aiRun.rcAll;
	}

	// ����54���Ʊ�
	for ( int iEnum = 14; iEnum >= 0; --iEnum )
	{
		if ( iEnum == 14 || iEnum == 13 )
		{
			rTemp.iNum = 1;
		}
		else
		{
			rTemp.iNum = 4;
		}
		rTemp.rc = (CARDCHANGE)iEnum;
		rcTemp.push_back( rTemp );
	}

	// ��ϵ�ǰ���ƺ��Լ����е���
	for ( int i = 0; i < m_cwCards.size(); ++i )
	{
		if ( rcAll.empty() )
		{
			rcAll.push_back( m_cwCards[i].rcard );
		}
		else
		{
			int j = 0;
			while ( j < rcAll.size() )
			{
				if ( rcAll[j].rc == m_cwCards[i].rcard.rc )
				{
					rcAll[j].iNum += m_cwCards[i].rcard.iNum;
					break;
				}
				++j;
			}

			// ������������
			if ( j >= rcAll.size() )
			{
				int k = 0;
				while ( k < rcAll.size() )
				{
					if ( m_cwCards[i].rcard.rc > rcAll[k].rc )
					{
						rcAll.insert( rcAll.begin() + k, m_cwCards[i].rcard );
						break;
					}
					++k;
				}

				// ��С
				if ( k >= rcAll.size() )
				{
					rcAll.push_back( m_cwCards[i].rcard );
				}
			}
		}
	}

	// ȡ����
	for ( int m = 0; m < rcTemp.size(); ++m )
	{
		for ( int n = 0; n < rcAll.size(); ++n )
		{
			if ( rcTemp[m].rc == rcAll[n].rc )
			{
				rcTemp[m].iNum -= rcAll[n].iNum;
				if ( rcTemp[m].iNum <= 0 )
				{
					rcTemp.erase( rcTemp.begin() + m-- );
				}
				break;
			}
		}
	}

	m_rcAll = rcTemp;
}



//
// ѡ��,��С
//
void KddzAI::ChooseCards( const CARDCHANGE rcIn, const int iNum )
{
	// ���Ʒ���
	if ( m_cwCards.empty() )
	{
		return;
	}

	for ( int i = 0; i < m_cwCards.size(); ++i )
	{
		if ( m_cwCards[i].rcard.rc == rcIn )
		{		
			// �Ӵ�С����
			for ( int j = 0; j < iNum; ++j )
			{
				m_RoleAI->ChooseCards( GetCard( m_cwCards[i].rcard.rc ) - j );
			}
			break;
		}
	}
}



//
// ����
//
bool KddzAI::PushCards( const AIRUN& aiRun )
{
	bool bTemp = false;
	
	ChangeCard();
	CutCards();	

	if ( m_cwOrderCards.empty() )
	{
		return false;
	}

	MakeCardsTable( aiRun );
	MathInOrOut();
	SetOrder();

	switch ( m_RoleAI->m_pos )
	{
	case p_mid:
		{
			bTemp = PushCardsMid( aiRun );
		}
		break;
	case p_up:
		{
			bTemp = PushCardsUp( aiRun );
		}
		break;
	case p_down:
		{
			bTemp = PushCardsDown( aiRun );
		}
		break;
	default:
		break;
	}

	return bTemp;
}



//
// ����
//
int KddzAI::FindOrderIncome( const CARDIF ci, const unsigned int iIn )
{
	CARDIF ciTemp = ( ci == sde ) ? sde : beg;

	for ( int i = m_cwIncome.size() - 1; i >= 0; --i )
	{
		// ����(ci:sde|end)
		if ( m_cwIncome[i].csif == ci &&
			 ( ( iIn == 0 && m_cwIncome[i].rcard.iNum <= 3 ) || 
			 ( iIn != 0 && ( m_cwIncome[i].rcard.iNum == iIn || iIn == 5 ) ) ) )
		{
			// ���������õ�˳��
			if ( ci == end && 
				 iIn == 0 &&
				 m_cwIncome[i].rcard.rc == RWS )
			{
				continue;
			}

			int iTempWhile = i;
			while ( iTempWhile >= 0 )
			{
				if ( m_cwIncome[iTempWhile].csif == ciTemp )
				{
					break;
				}
				--iTempWhile;
			}

			int iRe = i - iTempWhile + 1;
			// ѡ��
			while ( i >= iTempWhile )
			{
				ChooseCards( m_cwIncome[i].rcard.rc, 
							 m_cwIncome[i].rcard.iNum );
				--i;
			}

			if ( m_cwIncome[iTempWhile].rcard.iNum == 3 )
			{
				// ����3˳�ĳ���
				return iRe;
			}

			// ��������1��ʾ�ɹ�ѡ��(ע:��3��Ҳ�Ƿ���1��)
			return 1;
		}
	}

	return 0;
}


//
// ����
//
int KddzAI::FindOrderOutgo( const CARDIF ci, const unsigned int iIn )
{
	CARDIF ciTemp = ( ci == sde ) ? sde : beg;

	for ( int i = m_cwOutgo.size() - 1; i >= 0; --i )
	{
		// ����(ci:sde|end)
		if ( m_cwOutgo[i].csif == ci &&
			 ( ( iIn == 0 && m_cwOutgo[i].rcard.iNum <= 3 ) || 
			 ( iIn != 0 && m_cwOutgo[i].rcard.iNum == iIn ) ) )
		{
			int iTempWhile = i;
			while ( iTempWhile >= 0 )
			{
				if ( m_cwOutgo[iTempWhile].csif == ciTemp )
				{
					break;
				}
				--iTempWhile;
			}

			int iRe = i - iTempWhile + 1;
			// ѡ��
			while ( i >= iTempWhile )
			{
				ChooseCards( m_cwOutgo[i].rcard.rc, 
							 m_cwOutgo[i].rcard.iNum );
				--i;
			}

			if ( m_cwOutgo[iTempWhile].rcard.iNum == 3 )
			{
				// ����3˳�ĳ���
				return iRe;
			}

			// ��������1��ʾ�ɹ�ѡ��(ע:��3��Ҳ�Ƿ���1��)
			return 1;
		}
	}

	return 0;
}




//
// �Ҵ�
//
bool KddzAI::FindBigOne( const CWVector& cwIn )
{
	for ( int i = 0; i < cwIn.size(); ++i )
	{
		if ( cwIn[i].csif == sde &&
			 cwIn[i].rcard.iNum == 1 )
		{
			ChooseCards( cwIn[i].rcard.rc, 
						 cwIn[i].rcard.iNum );
			return true;
		}
	}

	return false;
}



//
// ����С����(���Ƴ�)
//
bool KddzAI::FindSmall( const unsigned int iIn )
{
	for ( int i = m_cwCards.size() - 1; i >= 0 ; --i )
	{
		if ( m_cwCards[i].rcard.iNum >= iIn )
		{
			ChooseCards( m_cwCards[i].rcard.rc, iIn );
			return true;
		}
	}

	return false;
}



//
// ��3�����
//
bool KddzAI::FindTowOne( const CWVector& cwIn, const unsigned int iIn )
{
	unsigned int iAddOne = 0;
	unsigned int iAddTwo = 0;
	int iRe = 0;

	for ( int i = cwIn.size() - 1; i >= 0; --i )
	{
		if ( cwIn[i].csif == sde )
		{
			// �ȵ��ȵ�(AA4433,KQJ����Ӧ�ô�ǰ�ߣ��������������BUG������)
			if ( cwIn[i].rcard.iNum == 1 )
			{
				++iAddOne;
				if ( iAddOne >= iIn )
				{
					// ������������
					iRe = 1;
					break;
				}
			}
			else if ( cwIn[i].rcard.iNum == 2 )
			{
				++iAddTwo;
				if ( iAddTwo >= iIn )
				{
					// ������������
					iRe = 2;
					break;
				}
			}
		}
	}

	unsigned int iPlas = iIn;
	if ( iRe != 0 )
	{
		for ( int j = cwIn.size() - 1; j >= 0 && iPlas > 0; --j )
		{
			if ( cwIn[j].csif == sde &&
				 cwIn[j].rcard.iNum == iRe )
			{
				ChooseCards( cwIn[j].rcard.rc, 
							 cwIn[j].rcard.iNum );
				--iPlas;
			}
		}

		return true;
	}

	return false;
}


//
// ��3˳���
//
bool KddzAI::FindTowOne( const CWVector& cwIn, const unsigned int iNum, const unsigned int iIn )
{
	unsigned int iAdd = 0;
	bool bRe = false;

	for ( int i = cwIn.size() - 1; i >= 0; --i )
	{
		if ( cwIn[i].csif == sde )
		{
			// �ȵ��ȵ�(AA4433,KQJ����Ӧ�ô�ǰ�ߣ��������������BUG������)
			if ( cwIn[i].rcard.iNum == iNum )
			{
				++iAdd;
				if ( iAdd >= iIn )
				{
					bRe = true;
					break;
				}
			}
		}
	}

	unsigned int iPlas = iIn;
	if ( bRe )
	{
		for ( int j = cwIn.size() - 1; j >= 0 && iPlas > 0; --j )
		{
			if ( cwIn[j].csif == sde &&
				 cwIn[j].rcard.iNum == iNum )
			{
				ChooseCards( cwIn[j].rcard.rc, 
							 cwIn[j].rcard.iNum );
				--iPlas;
			}
		}

		return true;
	}

	return false;
}




//
// �ҵ�(������)
//
bool KddzAI::FindOne( const AIRUN& aiRun, const unsigned int iIn )
{
	int iFour = ( iIn == 4 ) ? 4 : 3;
	CARDCHANGE rcTemp = R3;				// ��С

	// �ҳ����
	for ( int rc = aiRun.rcIn.size() - 1; rc >= 0; --rc )
	{	
		if ( aiRun.rcIn[rc].iNum == iIn &&
			 aiRun.rcIn[rc].rc > rcTemp )
		{
			rcTemp = aiRun.rcIn[rc].rc;
		}
	}

	if ( !m_cwOrderCards.empty() )
	{
		// �ҵ���
		for ( int i = m_cwOrderCards.size() - 1; i >= 0; --i )
		{
			if ( m_cwOrderCards[i].csif == sde &&
				 m_cwOrderCards[i].rcard.iNum == iIn &&
				 m_cwOrderCards[i].rcard.rc > rcTemp )
			{
				CWVector cwTemp;
				CARDWORTH cw;

				cw.csif = sde;
				cw.rcard.iNum = iIn;
				cw.rcard.rc = m_cwOrderCards[i].rcard.rc;
				cwTemp.push_back( cw );

				if ( GetBig( cwTemp ) )
				{
					ChooseCards( m_cwOrderCards[i].rcard.rc, iIn );
					return true;
				}
			}
		}

		// ��SDE	
		for ( int j = m_cwOrderCards.size() - 1; j >= 0; --j )
		{
			// ��2222
			if ( m_cwOrderCards[j].csif == sde &&
				 m_cwOrderCards[j].rcard.iNum == 4 &&
				 m_cwOrderCards[j].rcard.rc == RT &&
				 m_cwOrderCards[j].rcard.rc > rcTemp )
			{
				ChooseCards( m_cwOrderCards[j].rcard.rc, iIn );
				return true;
			}
			// ����
			else if ( m_cwOrderCards[j].csif == end &&
				 m_cwOrderCards[j].rcard.rc == RWS &&
				 iIn == 1 )
			{
				ChooseCards( m_cwOrderCards[j].rcard.rc, iIn );
				return true;
			}
			else if ( m_cwOrderCards[j].csif == sde &&
					  m_cwOrderCards[j].rcard.iNum >= iIn &&
					  m_cwOrderCards[j].rcard.iNum <= iFour &&		// ը������
					  m_cwOrderCards[j].rcard.rc > rcTemp )
			{
				CWVector cwTemp;
				CARDWORTH cw;

				cw.csif = sde;
				cw.rcard.iNum = iIn;
				cw.rcard.rc = m_cwOrderCards[j].rcard.rc;
				cwTemp.push_back( cw );

				if ( m_cwOrderCards[j].rcard.rc == RT || GetBig( cwTemp ) )
				{
					ChooseCards( m_cwOrderCards[j].rcard.rc, iIn );
					return true;
				}
			}				
		}

		// ��BEG-END	
		for ( int k = m_cwOrderCards.size() - 1; k >= 0; --k )
		{
			// ˳������
			if ( m_cwOrderCards[k].csif == beg &&
				 m_cwOrderCards[k].rcard.iNum >= iIn &&
				 m_cwOrderCards[k].rcard.iNum <= iFour &&
				 m_cwOrderCards[k].rcard.rc > rcTemp )
			{
				CWVector cwTemp;
				CARDWORTH cw;

				cw.csif = sde;
				cw.rcard.iNum = iIn;
				cw.rcard.rc = m_cwOrderCards[k].rcard.rc;
				cwTemp.push_back( cw );

				if ( GetBig( cwTemp ) )
				{
					ChooseCards( m_cwOrderCards[k].rcard.rc, iIn );
					return true;
				}
			}				
		}


		// ��󵥻��
		for ( int m = 0; m < m_cwCards.size(); ++m )
		{
			if ( m_cwCards[m].rcard.iNum >= iIn &&
				 m_cwCards[m].rcard.iNum <= iFour &&
				 m_cwCards[m].rcard.rc > rcTemp )
			{
				ChooseCards( m_cwCards[m].rcard.rc, iIn );
				return true;
			}
		}

	}

	return false;
}



//
// ��֧����
//
bool KddzAI::FindOneOutgo( const AIRUN& aiRun, const unsigned int iIn, const bool bOneSide )
{
	CARDCHANGE rcTemp = R3;				// ��С

	// �ҳ����
	for ( int rc = aiRun.rcIn.size() - 1; rc >= 0; --rc )
	{	
		if ( aiRun.rcIn[rc].iNum == iIn &&
			 aiRun.rcIn[rc].rc > rcTemp )
		{
			rcTemp = aiRun.rcIn[rc].rc;
		}
	}

	if ( !m_cwOutgo.empty() )
	{
		// �ҵ���
		for ( int i = m_cwOutgo.size() - 1; i >= 0; --i )
		{
			if ( m_cwOutgo[i].csif == sde &&
				 m_cwOutgo[i].rcard.iNum == iIn &&
				 m_cwOutgo[i].rcard.rc > rcTemp )
			{
				//
				// patch:2��С���������Ƶ�BUG
				//
				// ��ͬ�������,2,С������
				if ( bOneSide &&
					 m_cwOutgo[i].rcard.rc >= RT )
				{
					continue;
				}

				ChooseCards( m_cwOutgo[i].rcard.rc, iIn );
				return true;
			}
		}
	}

	return false;
}


//
// �����뵥
//
bool KddzAI::FindOneIncome( const AIRUN& aiRun, const unsigned int iIn )
{
	int iFour = ( iIn == 4 ) ? 4 : 3;
	CARDCHANGE rcTemp = R3;				// ��С

	// �ҳ����
	for ( int rc = aiRun.rcIn.size() - 1; rc >= 0; --rc )
	{	
		if ( aiRun.rcIn[rc].iNum == iIn &&
			 aiRun.rcIn[rc].rc > rcTemp )
		{
			rcTemp = aiRun.rcIn[rc].rc;
		}
	}

	if ( !m_cwIncome.empty() )
	{
		// �ҵ���
		for ( int i = m_cwIncome.size() - 1; i >= 0; --i )
		{
			if ( m_cwIncome[i].csif == sde &&
				 m_cwIncome[i].rcard.iNum == iIn &&
				 m_cwIncome[i].rcard.rc > rcTemp )
			{
				CWVector cwTemp;
				CARDWORTH cw;

				cw.csif = sde;
				cw.rcard.iNum = iIn;
				cw.rcard.rc = m_cwIncome[i].rcard.rc;
				cwTemp.push_back( cw );

				if ( GetBig( cwTemp ) )
				{
					ChooseCards( m_cwIncome[i].rcard.rc, iIn );
					return true;
				}
			}
		}

		// ��SDE	
		for ( int j = m_cwIncome.size() - 1; j >= 0; --j )
		{
			// ��2222
			if ( m_cwIncome[j].csif == sde &&
				 m_cwIncome[j].rcard.iNum == 4 &&
				 m_cwIncome[j].rcard.rc == RT &&
				 m_cwIncome[j].rcard.rc > rcTemp )
			{
				ChooseCards( m_cwIncome[j].rcard.rc, iIn );
				return true;
			}
			// ����
			else if ( m_cwIncome[j].csif == end &&
				 m_cwIncome[j].rcard.rc == RWS &&
				 iIn == 1 )
			{
				ChooseCards( m_cwIncome[j].rcard.rc, iIn );
				return true;
			}
			else if ( m_cwIncome[j].csif == sde &&
					  m_cwIncome[j].rcard.iNum >= iIn &&
					  m_cwIncome[j].rcard.iNum <= iFour &&
					  m_cwIncome[j].rcard.rc > rcTemp )
			{
				CWVector cwTemp;
				CARDWORTH cw;

				cw.csif = sde;
				cw.rcard.iNum = iIn;
				cw.rcard.rc = m_cwIncome[j].rcard.rc;
				cwTemp.push_back( cw );

				// ��2
				if ( m_cwIncome[j].rcard.rc == RT || GetBig( cwTemp ) )
				{
					ChooseCards( m_cwIncome[j].rcard.rc, iIn );
					return true;
				}
			}				
		}

		// û��������˳
	}

	return false;
}



//
// ��˳(������)
//
bool KddzAI::FindLink( const AIRUN& aiRun, const unsigned int iNum, const unsigned int iWeidth )
{
	CARDCHANGE rcTemp = RWB;				// ���

	// �ҳ���С
	for ( int rc = aiRun.rcIn.size() - 1; rc >= 0; --rc )
	{	
		if ( aiRun.rcIn[rc].iNum == iNum &&
			 aiRun.rcIn[rc].rc < rcTemp )
		{
			rcTemp = aiRun.rcIn[rc].rc;
		}
	}

	if ( !m_cwOrderCards.empty() )
	{
		// ����Ե�
		for ( int i = m_cwOrderCards.size() - 1; i >= 0; --i )
		{
			if ( m_cwOrderCards[i].csif == end &&
				 m_cwOrderCards[i].rcard.iNum == iNum &&
				 m_cwOrderCards[i].rcard.rc > rcTemp )
			{
				int iTempWhile = i;
				while ( iTempWhile >= 0 )
				{
					if ( m_cwOrderCards[iTempWhile].csif == beg )
					{
						break;
					}
					--iTempWhile;
				}

				if ( iTempWhile - i + 1 == iWeidth )
				{
					while ( i >= iTempWhile )
					{
						ChooseCards( m_cwOrderCards[i].rcard.rc, 
									 m_cwOrderCards[i].rcard.iNum );
						--i;
					}

					return true;
				}
			}
		}

		// ��˳
		for ( int j = m_cwOrderCards.size() - 1; j >= 0; --j )
		{
			if ( m_cwOrderCards[j].csif == end &&
				 m_cwOrderCards[j].rcard.iNum >= iNum &&
				 m_cwOrderCards[j].rcard.rc > rcTemp )
			{
				int iTempWhile = j;
				while ( iTempWhile >= 0 )
				{
					if ( m_cwOrderCards[iTempWhile].csif == beg )
					{
						break;
					}
					--iTempWhile;
				}

				if ( iTempWhile - j + 1 >= iWeidth )
				{
					while ( j >= iTempWhile )
					{
						ChooseCards( m_cwOrderCards[j].rcard.rc, 
									 m_cwOrderCards[j].rcard.iNum );
						--j;
					}

					return true;
				}
			}
		}
	}

	return false;
}



//
//
//
bool KddzAI::FindLinkOutgo( const AIRUN& aiRun, const unsigned int iNum, const unsigned int iWeidth )
{
	CARDCHANGE rcTemp = RWB;				// ���

	// �ҳ���С
	for ( int rc = aiRun.rcIn.size() - 1; rc >= 0; --rc )
	{	
		if ( aiRun.rcIn[rc].iNum == iNum &&
			 aiRun.rcIn[rc].rc < rcTemp )
		{
			rcTemp = aiRun.rcIn[rc].rc;
		}
	}

	if ( !m_cwOutgo.empty() )
	{
		// ����Ե�
		for ( int i = m_cwOutgo.size() - 1; i >= 0; --i )
		{
			if ( m_cwOutgo[i].csif == end &&
				 m_cwOutgo[i].rcard.iNum == iNum &&
				 m_cwOutgo[i].rcard.rc > rcTemp )
			{
				int iTempWhile = i;
				while ( iTempWhile >= 0 )
				{
					if ( m_cwOrderCards[iTempWhile].csif == beg )
					{
						break;
					}
					--iTempWhile;
				}

				if ( iTempWhile - i + 1 == iWeidth )
				{
					while ( i >= iTempWhile )
					{
						ChooseCards( m_cwOutgo[i].rcard.rc, 
									 m_cwOutgo[i].rcard.iNum );
						--i;
					}

					return true;
				}
			}
		}

		// ����
	}

	return false;
}


//
//
//
bool KddzAI::FindLinkIncome( const AIRUN& aiRun, const unsigned int iNum, const unsigned int iWeidth )
{
	CARDCHANGE rcTemp = RWB;				// ���

	// �ҳ���С
	for ( int rc = aiRun.rcIn.size() - 1; rc >= 0; --rc )
	{	
		if ( aiRun.rcIn[rc].iNum == iNum &&
			 aiRun.rcIn[rc].rc < rcTemp )
		{
			rcTemp = aiRun.rcIn[rc].rc;
		}
	}

	if ( !m_cwIncome.empty() )
	{
		// ����Ե�
		for ( int i = m_cwIncome.size() - 1; i >= 0; --i )
		{
			if ( m_cwIncome[i].csif == end &&
				 m_cwIncome[i].rcard.iNum == iNum &&
				 m_cwIncome[i].rcard.rc > rcTemp )
			{
				int iTempWhile = i;
				while ( iTempWhile >= 0 )
				{
					if ( m_cwIncome[iTempWhile].csif == beg )
					{
						break;
					}
					--iTempWhile;
				}

				if ( iTempWhile - i + 1 == iWeidth )
				{
					while ( i >= iTempWhile )
					{
						ChooseCards( m_cwIncome[i].rcard.rc, 
									 m_cwIncome[i].rcard.iNum );
						--i;
					}

					return true;
				}
			}
		}

		// ��˳
		for ( int j = m_cwIncome.size() - 1; j >= 0; --j )
		{
			if ( m_cwIncome[j].csif == end &&
				 m_cwIncome[j].rcard.iNum >= iNum &&
				 m_cwIncome[j].rcard.rc > rcTemp )
			{
				int iTempWhile = j;
				while ( iTempWhile >= 0 )
				{
					if ( m_cwIncome[iTempWhile].csif == beg )
					{
						break;
					}
					--iTempWhile;
				}

				if ( iTempWhile - j + 1 >= iWeidth )
				{
					while ( j >= iTempWhile )
					{
						ChooseCards( m_cwIncome[j].rcard.rc, 
									 m_cwIncome[j].rcard.iNum );
						--j;
					}

					return true;
				}
			}
		}
	}

	return false;
}



//
// ��������
//
bool KddzAI::PushCardsMid( const AIRUN& aiRun )
{
	bool bTemp = false;
	bool bOne = false;
	bool bTwo = false;
	bool bReOne = false;
	bool bReTwo = false;

	// ũ���е��ƾ���
	if ( aiRun.iSizeUp == 1 || 
		 aiRun.iSizeDown == 1 )
	{
		bOne = true;
	}

	// ũ���ж��ƾ���
	if ( aiRun.iSizeUp == 2 || 
		 aiRun.iSizeDown == 2 )
	{
		bTwo = true;
	}

	// ģ��ѡ��
	switch ( aiRun.cc )
	{		
		case ctrue:									// ���ȳ���
			{				
				int iRe = 0;
				bool bBreak = false;
				bool bEnough = true;
				int iReHandNum = GetHandNum( true );
				
				// ֧������
				// ������������û��������
				if ( iReHandNum > 1 || m_cwIncome.empty() )
				{
					// Ԥ����,��û������
					if ( iReHandNum == 2 && !bOne && !bTwo )
					{
						iRe = UpEnough();

						switch ( iRe )
						{
						case 0:					// ʧ��
							break;
						case 1:					// ����
						case 2:					// ��
							{
								bEnough = false;
							}
							break;
						case 3:					// 3��
							{
								// ����
								FindTowOne( m_cwOrderCards, 1 );
								bEnough = false;
							}
							break;
						default:
							break;
						}
					}

					if ( bEnough )
					{
						// ��˳��˫˳����˳
						iRe = FindOrderOutgo( end );

						switch ( iRe )
						{
						case 0:					// ʧ��
							{
								// 3��
								iRe = FindOrderOutgo( sde, 3 );				

								// ʧ��
								if ( iRe == 0 )
								{
									if ( iReHandNum > 1 )
									{
										// ����
										if ( bOne )
										{
											// �ҶԳ�
											iRe = FindOrderOutgo( sde, 2 );
											
											// ������ֱ�ӳ�
											if ( iRe == 0 )
											{
												bReOne = true;
//												FindBigOne( m_cwOutgo );
											}
										}

										else if ( bTwo )
										{
											// �ҵ���
											iRe = FindOrderOutgo( sde, 1 );
											
											if ( iRe == 0 )
											{
												// ���
												bReTwo = true;
//												FindSmall( 1 );
											}
										}

										else
										{
											FindOrderOutgo( sde );
										}
									}

									else if ( m_cwIncome.empty() )
									{
										FindOrderOutgo( sde );
									}
								}

								// �ɹ�
								else
								{
									// ȫ������
									FindTowOne( m_cwOrderCards, 1 );
								}
							}
							break;
						case 1:					// �ɹ�,������3˳
							break;
						default:				// 3˳
							{
								// �Ҷ�Ӧ�����ĵ����߶�
								FindTowOne( m_cwOrderCards, iRe );
							}
							break;
						}
					}
				}

				// ��������
				if ( iReHandNum <= 1 || bReOne || bReTwo )
				{	
					// ��˳��˫˳����˳
					iRe = FindOrderIncome( end );

					switch ( iRe )
					{
					case 0:					// ʧ��
						{
							// 3��
							iRe = FindOrderIncome( sde, 3 );				

							// ʧ��
							if ( iRe == 0 )
							{
								if ( iReHandNum <= 1 )
								{
									// 4��
									if ( FindOrderIncome( sde, 4 ) != 0 )
									{										
										// ��2����
										if ( !FindTowOne( m_cwOrderCards, 1, 2 ) )
										{
											// ��2����
											FindTowOne( m_cwOrderCards, 2, 2 );
										}

										bBreak = true;
									}
								}

								if ( !bBreak )
								{
									if ( bReOne )
									{
										if ( FindBigOne( m_cwOutgo ) )
										{
											break;
										}
									}
									else if ( bReTwo )
									{
										if ( FindSmall( 1 ) )
										{
											break;
										}
									}

									// ����+333
									if ( !Patch3And() )
									{
										// ����
										iRe = FindOrderIncome( sde );
										
										if ( iRe == 0 )
										{											
											// ����
											if ( !Patch2w( bOne || bTwo ) )
											{		
												// ��˫��
												FindOrderIncome( end, 5 );												
											}
										}										
									}
								}
							}

							// �ɹ�
							else
							{
								// ȫ������
								FindTowOne( m_cwOrderCards, 1 );
							}
						}
						break;
					case 1:					// �ɹ�,������3˳
						break;
					default:				// 3˳
						{
							// �Ҷ�Ӧ�����ĵ����߶�(�о������̫����)[AAAKKKQQQ]
							FindTowOne( m_cwOrderCards, iRe );
						}
						break;
					}
				}

				// ����
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cone:									// ����
			{
				ChoseOneMid( aiRun, 1, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case ctwo:									// ����
			{
				ChoseOneMid( aiRun, 2, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cthree:									// 3�Ų���
			{
				ChoseOneMid( aiRun, 3, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cfour:									// 4�Ų���
			{
				ChoseOneMid( aiRun, 4, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case ctth:										// ��˳(2)
			{
				ChoseLinkMid( aiRun, 3, 2, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cthone:										// 3��1
			{
				ChoseOneMid( aiRun, 3, bOne || bTwo );
				FindTowOne( m_cwOrderCards, 1, 1 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cthtwo:										// 3��2
			{
				ChoseOneMid( aiRun, 3, bOne || bTwo );
				FindTowOne( m_cwOrderCards, 2, 1 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
//		case cfone:											// 4��1							
//			{
//				ChoseOneMid( aiRun, 4, bOne || bTwo );
//				FindTowOne( m_cwOrderCards, 1, 1 );
//
//				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
//			}
//			break;
//		case cftwo:											// 4��2
//			{
//				ChoseOneMid( aiRun, 4, bOne || bTwo );
//				FindTowOne( m_cwOrderCards, 2, 1 );
//
//				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
//			}
//			break;
		case cttth:									// ˫˳(3)
			{
				ChoseLinkMid( aiRun, 2, 3, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cftone:								// �Ĵ�����
			{
				ChoseOneMid( aiRun, 4, bOne || bTwo );
				FindTowOne( m_cwOrderCards, 1, 2 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cfttwo:										// �Ĵ�����
			{
				ChoseOneMid( aiRun, 4, bOne || bTwo );
				FindTowOne( m_cwOrderCards, 2, 2 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case ctfour:									// ˫˳(4)
			{
				ChoseLinkMid( aiRun, 2, 4, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cthfour:													// ��˳(4)
			{
				ChoseLinkMid( aiRun, 3, 4, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case ctthone:											// ��˳������
			{
				ChoseLinkMid( aiRun, 3, 2, bOne || bTwo );
				FindTowOne( m_cwOrderCards, 1, 2 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case ctthtwo:										// ��˳������
			{
				ChoseLinkMid( aiRun, 3, 2, bOne || bTwo );
				FindTowOne( m_cwOrderCards, 2, 2 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case clink5:									// ��˳(5)
			{
				ChoseLinkMid( aiRun, 1, 5, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case clink6:								// ��˳(6)
			{
				ChoseLinkMid( aiRun, 1, 6, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case clink7:								// ��˳(7)
			{
				ChoseLinkMid( aiRun, 1, 7, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case clink8:									// ��˳(8)
			{
				ChoseLinkMid( aiRun, 1, 8, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case clink9:									// ��˳(9)
			{
				ChoseLinkMid( aiRun, 1, 9, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case clink10:							// ��˳(10)
			{
				ChoseLinkMid( aiRun, 1, 10, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case clink11:								// ��˳(11)
			{
				ChoseLinkMid( aiRun, 1, 11, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case ctfive:									// ˫˳(5)
			{
				ChoseLinkMid( aiRun, 2, 5, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cthfive:								// ��˳(5)
			{
				ChoseLinkMid( aiRun, 3, 5, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case ctsix:								// ˫˳(6)
			{
				ChoseLinkMid( aiRun, 2, 6, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cththone:							// ��˳������
			{
				ChoseLinkMid( aiRun, 3, 3, bOne || bTwo );
				FindTowOne( m_cwOrderCards, 1, 3 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cththtwo:							// ��˳������
			{
				ChoseLinkMid( aiRun, 3, 3, bOne || bTwo );
				FindTowOne( m_cwOrderCards, 2, 3 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case ctseven:							// ˫˳(7)
			{
				ChoseLinkMid( aiRun, 2, 7, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case ctevent:				     	// ˫˳(8)
			{
				ChoseLinkMid( aiRun, 2, 8, bOne || bTwo );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cthfone:					// ��˳���ĵ�
			{
				ChoseLinkMid( aiRun, 3, 4, bOne || bTwo );
				FindTowOne( m_cwOrderCards, 1, 4 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		default:
			break;
	} 

	return bTemp;
}



//
// �����ϼҳ���
//
bool KddzAI::PushCardsUp( const AIRUN& aiRun )
{
	bool bTemp = false;
	bool bOneMid = false;
	bool bTwoMid = false;
	bool bReOneMid = false;
	bool bReTwoMid = false;
	bool bOneDown = false;
	bool bTwoDown = false;
	static bool bTwoToOne = false;			// ��ԸĴ�

	// �޾���״̬
	if ( aiRun.iSizeDown > 2 )
	{
		bTwoToOne = false;
	}

	// ͬ���е��ƾ���
	if ( aiRun.iSizeDown == 1 ||
		 bTwoToOne )
	{
		bOneDown = true;
	}

	// ͬ���ж��ƾ���
	if ( aiRun.iSizeDown == 2 &&
		 !bTwoToOne )
	{
		bTwoDown = true;
	}

	// �����е��ƾ���
	if ( aiRun.iSizeMid == 1 )
	{
		bOneMid = true;
	}

	// �����ж��ƾ���
	if ( aiRun.iSizeMid == 2 )
	{
		bTwoMid = true;
	}

	// ģ��ѡ��
	switch ( aiRun.cc )
	{		
		case ctrue:									// ���ȳ���
			{
				int iRe = 0;
				bool bRe = false;
				bool bBreak = false;
				bool bEnough = true;
				int iReHandNum = GetHandNum( true );

				// �Լ�������
				if ( iReHandNum > 1  )
				{
					if ( bOneDown && !bOneMid )
					{
						// ��С��
						bRe = FindSmall( 1 );
					}

					else if ( bTwoDown && !bTwoMid )
					{
						// ��С��
						bRe = FindSmall( 2 );
					}

					if ( bRe )
					{
						bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );

						if ( bTemp && bTwoDown )
						{
							bTwoToOne = true;
						}

						break;
					}
				}

				// ֧������
				if ( iReHandNum > 1 || m_cwIncome.empty() )
				{			
					// Ԥ����
					if ( iReHandNum == 2 && !bOneMid && !bTwoMid )
					{
						iRe = UpEnough();

						switch ( iRe )
						{
						case 0:					// ʧ��
							break;
						case 1:					// ����
						case 2:					// ��
							{
								bEnough = false;
							}
							break;
						case 3:					// 3��
							{
								// ����
								FindTowOne( m_cwOrderCards, 1 );
								bEnough = false;
							}
							break;
						default:
							break;
						}
					}

					if ( bEnough )
					{
						// ��˳��˫˳����˳
						iRe = FindOrderOutgo( end );

						switch ( iRe )
						{
						case 0:					// ʧ��
							{
								// 3��
								iRe = FindOrderOutgo( sde, 3 );				

								// ʧ��
								if ( iRe == 0 )
								{
									if ( iReHandNum > 1 )
									{
										// ����
										if ( bOneMid )
										{
											// �ҶԳ�
											iRe = FindOrderOutgo( sde, 2 );
											
											if ( iRe == 0 )
											{
												bReOneMid = true;
//												FindBigOne( m_cwOutgo );
											}
										}

										else if ( bTwoMid )
										{
											// �ҵ���
											iRe = FindOrderOutgo( sde, 1 );
											
											if ( iRe == 0 )
											{
												bReTwoMid = true;
//												FindSmall( 1 );
											}
										}

										else
										{
											FindOrderOutgo( sde );
										}
									}

									else if ( m_cwIncome.empty() )
									{
										FindOrderOutgo( sde );
									}
								}

								// �ɹ�
								else
								{
									FindTowOne( m_cwOrderCards, 1 );
								}
							}
							break;
						case 1:					// �ɹ�,������3˳
							break;
						default:				// 3˳
							{
								// �Ҷ�Ӧ�����ĵ����߶�
								FindTowOne( m_cwOrderCards, iRe );
							}
							break;
						}
					}
				}

				// ��������
				if ( iReHandNum <= 1 || bReOneMid || bReTwoMid )
				{	
					// ��˳��˫˳����˳
					iRe = FindOrderIncome( end );

					switch ( iRe )
					{
					case 0:					// ʧ��
						{
							// 3��
							iRe = FindOrderIncome( sde, 3 );				

							// ʧ��
							if ( iRe == 0 )
							{
								if ( iReHandNum <= 1 )
								{
									// 4��
									if ( FindOrderIncome( sde, 4 ) != 0 )
									{
										// ��2����
										if ( !FindTowOne( m_cwOrderCards, 1, 2 ) )
										{
											// ��2����
											FindTowOne( m_cwOrderCards, 2, 2 );
										}

										bBreak = true;
									}
								}

								if ( !bBreak )
								{
									if ( bReOneMid )
									{
										if ( FindBigOne( m_cwOutgo ) )
										{
											break;
										}
									}
									else if ( bReTwoMid )
									{
										if ( FindSmall( 1 ) )
										{
											break;
										}
									}

									if ( !Patch3And() )
									{
										// ����
										iRe = FindOrderIncome( sde );
										
										if ( iRe == 0 )
										{
											if ( !Patch2w( bOneMid || bTwoMid ) )
											{
												// 2��
												FindOrderIncome( end, 5 );
											}
										}
									}
								}
							}

							// �ɹ�
							else
							{
								FindTowOne( m_cwOrderCards, 1 );
							}
						}
						break;
					case 1:					// �ɹ�,������3˳
						break;
					default:				// 3˳
						{
							// �Ҷ�Ӧ�����ĵ����߶�(�о������̫����)[AAAKKKQQQ]
							FindTowOne( m_cwOrderCards, iRe );
						}
						break;
					}
				}

				// ����
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cone:									// ����
			{
				bool bOneInCome = false;

				// ����
				if ( ChoseOneOk( aiRun, 1 ) )
				{
					bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
					break;
				}

				// ��������
				if ( bOneMid )
				{
					// �������ԣ�ͬ��ŵ��Ͳ�����
					// ͬ������ƣ��Ҳ������
					if ( aiRun.aipos == p_down &&
						 !GetBigOther( aiRun ) )						
					{
						// ����
						FindOne( aiRun, 1 );
					}
					else if ( aiRun.aipos != p_down )
					{
						// ����
						FindOne( aiRun, 1 );
					}
				}

				else if ( bTwoMid && aiRun.aipos != p_down )
				{
					// ����
					FindOne( aiRun, 1 );
				}

				// ͬ�鱨��,�ҵ���������
				else if ( bOneDown && 
						  aiRun.aipos != p_down )
				{
					// ����
					if ( !FindOne( aiRun, 1 ) )
					{
						PushZha( aiRun );
					}
				}

				// ͬ�鱨��
				else if ( bTwoDown && 
						  aiRun.aipos != p_down )
				{
					// ����
					if ( !FindOne( aiRun, 1 ) )
					{
						PushZha( aiRun );
					}
				}
						
				else
				{					
					// ˳��
					if ( !m_cwOutgo.empty() )
					{
						// ˳��
						if ( !FindOneOutgo( aiRun, 1, aiRun.aipos == p_down ) &&
							 aiRun.aipos != p_down )
						{
							bOneInCome = true;			
						}
					}

					if ( m_cwOutgo.empty() || bOneInCome )
					{
						// ����(����ͬ��Ŵ�)
						FindOneIncome( aiRun, 1 );
					}
				}
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case ctwo:									// ����
			{
				bool bOneInCome = false;

				// ����
				if ( ChoseOneOk( aiRun, 2 ) )
				{
					bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
					break;
				}

				// �����������
				if ( bTwoMid )
				{
					// ����������ͬ��ŶԾͲ�����
					// ͬ������ƣ��Ҳ������
					if ( aiRun.aipos == p_down &&
						 !GetBigOther( aiRun ) )						
					{
						// ����
						FindOne( aiRun, 2 );
					}
					else if ( aiRun.aipos != p_down )
					{
						// ����
						FindOne( aiRun, 2 );
					}
				}

				else if ( bOneMid && aiRun.aipos != p_down )
				{
					// ����
					FindOne( aiRun, 2 );
				}

				//
				// ע:
				// ��������ϼҸ����ܲ��ϵĻ��ͳ���һ��BUG
				// ����һֱ�ŵ��ͶԵ�ʱ��û�˹�
				//
				// ͬ�鱨��,�ҵ���������
				else if ( bOneDown && 
						  aiRun.aipos != p_down )
				{
					// ����
					if ( !FindOne( aiRun, 2 ) )
					{
						PushZha( aiRun );
					}
				}

				// ͬ�鱨��
				else if ( bTwoDown && 
						  aiRun.aipos != p_down )
				{
					// ����
					if ( !FindOne( aiRun, 2 ) )
					{
						PushZha( aiRun );
					}
				}
						
				else
				{
					// ˳��
					if ( !m_cwOutgo.empty() )
					{
						// ˳��
						if ( !FindOneOutgo( aiRun, 2 ) &&
							 aiRun.aipos != p_down )
						{
							bOneInCome = true;			
						}
					}

					if ( m_cwOutgo.empty() || bOneInCome )
					{
						// ����(����ͬ��Ŵ�)
						FindOneIncome( aiRun, 2 );
					}
				}
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case cthree:									// 3�Ų���
			{
				ChoseOneUp( aiRun, 3, bOneMid || bTwoMid, bOneDown, bTwoDown );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );		
			}
			break;
		case cfour:									// 4�Ų���
			{
				ChoseOneUp( aiRun, 4, bOneMid || bTwoMid, bOneDown, bTwoDown );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case ctth:										// ��˳(2)
			{
				ChoseLinkUp( aiRun, 3, 2, bOneMid || bTwoMid, bOneDown, bTwoDown );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case cthone:										// 3��1
			{
				ChoseOneUp( aiRun, 3, bOneMid || bTwoMid, bOneDown, bTwoDown );
				FindTowOne( m_cwOrderCards, 1, 1 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case cthtwo:										// 3��2
			{
				ChoseOneUp( aiRun, 3, bOneMid || bTwoMid, bOneDown, bTwoDown );
				FindTowOne( m_cwOrderCards, 2, 1 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
//		case cfone:											// 4��1							
//			{
//				ChoseOneUp( aiRun, 4, bOneMid || bTwoMid, bOneDown, bTwoDown );
//				FindTowOne( m_cwOrderCards, 1, 1 );
//
//				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
//			}
//			break;
//		case cftwo:											// 4��2
//			{
//				ChoseOneUp( aiRun, 3, bOneMid || bTwoMid, bOneDown, bTwoDown );
//				FindTowOne( m_cwOrderCards, 2, 1 );
//
//				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
//			}
//			break;
		case cttth:									// ˫˳(3)
			{
				ChoseLinkUp( aiRun, 2, 3, bOneMid || bTwoMid, bOneDown, bTwoDown );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cftone:								// �Ĵ�����
			{
				ChoseOneUp( aiRun, 4, bOneMid || bTwoMid, bOneDown, bTwoDown );
				FindTowOne( m_cwOrderCards, 1, 2 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case cfttwo:										// �Ĵ�����
			{
				ChoseOneUp( aiRun, 4, bOneMid || bTwoMid, bOneDown, bTwoDown );
				FindTowOne( m_cwOrderCards, 2, 2 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case ctfour:									// ˫˳(4)
			{
				ChoseLinkUp( aiRun, 2, 4, bOneMid || bTwoMid, bOneDown, bTwoDown );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cthfour:													// ��˳(4)
			{
				ChoseLinkUp( aiRun, 3, 4, bOneMid || bTwoMid, bOneDown, bTwoDown );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case ctthone:											// ��˳������
			{
				ChoseLinkUp( aiRun, 3, 2, bOneMid || bTwoMid, bOneDown, bTwoDown );
				FindTowOne( m_cwOrderCards, 1, 2 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case ctthtwo:										// ��˳������
			{
				ChoseLinkUp( aiRun, 3, 2, bOneMid || bTwoMid, bOneDown, bTwoDown );
				FindTowOne( m_cwOrderCards, 2, 2 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case clink5:									// ��˳(5)
			{
				ChoseLinkUp( aiRun, 1, 5, bOneMid || bTwoMid, bOneDown, bTwoDown );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case clink6:								// ��˳(6)
			{
				ChoseLinkUp( aiRun, 1, 6, bOneMid || bTwoMid, bOneDown, bTwoDown );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case clink7:								// ��˳(7)
			{
				ChoseLinkUp( aiRun, 1, 7, bOneMid || bTwoMid, bOneDown, bTwoDown );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case clink8:									// ��˳(8)
			{
				ChoseLinkUp( aiRun, 1, 8, bOneMid || bTwoMid, bOneDown, bTwoDown );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case clink9:									// ��˳(9)
			{
				ChoseLinkUp( aiRun, 1, 9, bOneMid || bTwoMid, bOneDown, bTwoDown );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case clink10:							// ��˳(10)
			{
				ChoseLinkUp( aiRun, 1, 10, bOneMid || bTwoMid, bOneDown, bTwoDown );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case clink11:								// ��˳(11)
			{
				ChoseLinkUp( aiRun, 1, 11, bOneMid || bTwoMid, bOneDown, bTwoDown );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case ctfive:									// ˫˳(5)
			{
				ChoseLinkUp( aiRun, 2, 5, bOneMid || bTwoMid, bOneDown, bTwoDown );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cthfive:								// ��˳(5)
			{
				ChoseLinkUp( aiRun, 3, 5, bOneMid || bTwoMid, bOneDown, bTwoDown );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case ctsix:								// ˫˳(6)
			{
				ChoseLinkUp( aiRun, 2, 6, bOneMid || bTwoMid, bOneDown, bTwoDown );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cththone:							// ��˳������
			{
				ChoseLinkUp( aiRun, 3, 3, bOneMid || bTwoMid, bOneDown, bTwoDown );
				FindTowOne( m_cwOrderCards, 1, 3 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case cththtwo:							// ��˳������
			{
				ChoseLinkUp( aiRun, 3, 3, bOneMid || bTwoMid, bOneDown, bTwoDown );
				FindTowOne( m_cwOrderCards, 2, 3 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case ctseven:							// ˫˳(7)
			{
				ChoseLinkUp( aiRun, 2, 7, bOneMid || bTwoMid, bOneDown, bTwoDown );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case ctevent:				     	// ˫˳(8)
			{
				ChoseLinkUp( aiRun, 2, 8, bOneMid || bTwoMid, bOneDown, bTwoDown );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );
			}
			break;
		case cthfone:					// ��˳���ĵ�
			{
				ChoseLinkUp( aiRun, 3, 4, bOneMid || bTwoMid, bOneDown, bTwoDown );
				FindTowOne( m_cwOrderCards, 1, 4 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		default:
			break;
	} 

	return bTemp;
}



//
// �����¼ҳ���
//
bool KddzAI::PushCardsDown( const AIRUN& aiRun )
{
	bool bTemp = false;
	bool bOneMid = false;
	bool bTwoMid = false;
	bool bReOneMid = false;
	bool bReTwoMid = false;
	bool bOneUp = false;
	bool bTwoUp = false;
	static bool bTwoToOne = false;				

	// �޾���״̬
	if ( aiRun.iSizeUp > 2 )
	{
		bTwoToOne = false;
	}

	// ͬ���е��ƾ���
	if ( aiRun.iSizeUp == 1 ||
		 bTwoToOne )
	{
		bOneUp = true;
	}

	// ͬ���ж��ƾ���
	if ( aiRun.iSizeUp == 2 &&
		 !bTwoToOne )				
	{
		bTwoUp = true;
	}

	// �����е��ƾ���
	if ( aiRun.iSizeMid == 1 )
	{
		bOneMid = true;
	}

	// �����ж��ƾ���
	if ( aiRun.iSizeMid == 2 )
	{
		bTwoMid = true;
	}

	// ģ��ѡ��
	switch ( aiRun.cc )
	{		
		case ctrue:									// ���ȳ���
			{
				int iRe = 0;
				bool bRe = false;
				bool bBreak = false;
				bool bEnough = true;
				int iReHandNum = GetHandNum( true );

				// ����
				if ( iReHandNum > 1 )
				{
					if ( bOneUp )
					{
						bRe = FindSmall( 1 );
					}

					else if ( bTwoUp )
					{
						bRe = FindSmall( 2 );
					}

					if ( bRe )
					{
						bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );

						if ( bTemp && bTwoUp )
						{
							bTwoToOne = true;
						}

						break;
					}
				}
				
				// ֧������
				if ( iReHandNum > 1 || m_cwIncome.empty() )
				{
					// Ԥ����
					if ( iReHandNum == 2 && !bOneMid && !bTwoMid )
					{
						iRe = UpEnough();

						switch ( iRe )
						{
						case 0:					// ʧ��
							break;
						case 1:					// ����
						case 2:					// ��
							{
								bEnough = false;
							}
							break;
						case 3:					// 3��
							{
								// ����
								FindTowOne( m_cwOrderCards, 1 );
								bEnough = false;
							}
							break;
						default:
							break;
						}
					}

					if ( bEnough )
					{
						// ��˳��˫˳����˳
						iRe = FindOrderOutgo( end );

						switch ( iRe )
						{
						case 0:					// ʧ��
							{
								// 3��
								iRe = FindOrderOutgo( sde, 3 );				

								// ʧ��
								if ( iRe == 0 )
								{				
									// ��������ʱ��
									if ( iReHandNum > 1 )
									{
										if ( bOneMid )
										{
											// �ҶԳ�
											iRe = FindOrderOutgo( sde, 2 );
											
											if ( iRe == 0 )
											{
												bReOneMid = true;
//												FindBigOne( m_cwOutgo );
											}
										}

										else if ( bTwoMid )
										{
											// �ҵ���
											iRe = FindOrderOutgo( sde, 1 );
											
											if ( iRe == 0 )
											{
												bReTwoMid = true;
//												FindSmall( 1 );
											}
										}

										// �޾����ճ���
										else 
										{
											FindOrderOutgo( sde );
										}
									}

									// ���һ����
									else if ( m_cwIncome.empty() )
									{
										FindOrderOutgo( sde );
									}
								}

								// �ɹ�
								else
								{
									FindTowOne( m_cwOrderCards, 1 );
								}
							}
							break;
						case 1:					// �ɹ�,������3˳
							break;
						default:				// 3˳
							{
								// �Ҷ�Ӧ�����ĵ����߶�
								FindTowOne( m_cwOrderCards, iRe );
							}
							break;
						}
					}
				}

				// ��������
				if ( iReHandNum <= 1 || bReOneMid || bReTwoMid )
				{	
					// ��˳��˫˳����˳
					iRe = FindOrderIncome( end );

					switch ( iRe )
					{
					case 0:					// ʧ��
						{
							// 3��
							iRe = FindOrderIncome( sde, 3 );				

							// ʧ��
							if ( iRe == 0 )
							{
								if ( iReHandNum <= 1 )
								{
									// 4��
									if ( FindOrderIncome( sde, 4 ) != 0 )
									{
										// ��2����
										if ( !FindTowOne( m_cwOrderCards, 1, 2 ) )
										{
											// ��2����
											FindTowOne( m_cwOrderCards, 2, 2 );
										}

										bBreak = true;
									}
								}

								if ( !bBreak )
								{
									if ( bReOneMid )
									{
										if ( FindBigOne( m_cwOutgo ) )
										{
											break;
										}
									}
									else if ( bReTwoMid )
									{
										if ( FindSmall( 1 ) )
										{
											break;
										}
									}

									if ( !Patch3And() )
									{									
										// ����
										iRe = FindOrderIncome( sde );
										
										if ( iRe == 0 )
										{
											if ( !Patch2w( bOneMid || bTwoMid ) )
											{
												// 2��
												FindOrderIncome( end, 5 );
											}
										}
									}
								}
							}

							// �ɹ�
							else
							{
								FindTowOne( m_cwOrderCards, 1 );
							}
						}
						break;
					case 1:					// �ɹ�,������3˳
						break;
					default:				// 3˳
						{
							// �Ҷ�Ӧ�����ĵ����߶�(�о������̫����)[AAAKKKQQQ]
							FindTowOne( m_cwOrderCards, iRe );
						}
						break;
					}
				}

				// ����
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case cone:									// ����
			{
				bool bOneInCome = false;

				// ����
				if ( ChoseOneOk( aiRun, 1 ) )
				{
					bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
					break;
				}

				// ͬ�������, �������ܣ���Ҳ����
				if ( aiRun.aipos == p_up )
				{
					break;
				}
				else
				{
					// �����������
					if ( bOneMid || bTwoMid )
					{
						// ����
						if ( !FindOne( aiRun, 1 ) && ( bOneUp || bTwoUp ) )
						{
							PushZha( aiRun );
						}
					}

					// ͬ�鱨��
					else if ( bOneUp )
					{
						// ���Լ���С���ƻ�С����	
						if ( aiRun.rcIn[0].rc < m_cwCards[m_cwCards.size() - 1].rcard.rc )
						{
							break;
						}
						else
						{
							// ����
							if ( !FindOne( aiRun, 1 ) )
							{
								PushZha( aiRun );
							}
						}
					}

					// ͬ�鱨��
					else if ( bTwoUp && 
							  aiRun.aipos != p_up )
					{
						// ����
						if ( !FindOne( aiRun, 1 ) )
						{
							PushZha( aiRun );
						}
					}

					else
					{
						// ˳��
						if ( !m_cwOutgo.empty() )
						{
							// ˳��
							if ( !FindOneOutgo( aiRun, 1, aiRun.aipos == p_up ) )
							{
								bOneInCome = true;							
							}
						}

						if ( m_cwOutgo.empty() || bOneInCome )
						{
							// ����(�϶��ǵ�������)
							FindOneIncome( aiRun, 1 );
						}
					}
					bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
				}								
			}
			break;
		case ctwo:									// ����
			{
				bool bTwoInCome = false;

				// ����
				if ( ChoseOneOk( aiRun, 1 ) )
				{
					bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
					break;
				}

				// ͬ�������, �������ܣ���Ҳ����
				if ( aiRun.aipos == p_up )
				{
					break;
				}
				else
				{
					// �����������
					if ( bOneMid || bTwoMid )
					{
						// ����
						if ( !FindOne( aiRun, 2 ) && ( bOneUp || bTwoUp ) )
						{
							PushZha( aiRun );
						}
					}

					// ͬ�鱨��
					else if ( bOneUp && 
							  aiRun.aipos != p_up )
					{
						// ����
						if ( !FindOne( aiRun, 2 ) )
						{
							PushZha( aiRun );
						}
					}

					// ͬ�鱨��
					else if ( bTwoUp )
					{
						// ���Լ���С���ƻ�С����
						CARDCHANGE rc;
						for ( int ic = m_cwCards.size() - 1; ic >= 0; --ic )
						{
							if ( m_cwCards[ic].rcard.iNum >= 2 )
							{
								rc = m_cwCards[ic].rcard.rc;
								break;
							}
						}
						if ( aiRun.rcIn[0].rc < rc )
						{
							break;
						}
						else
						{
							// ����
							if ( !FindOne( aiRun, 2 ) )
							{
								PushZha( aiRun );
							}
						}
					}

					else
					{
						// ˳��
						if ( !m_cwOutgo.empty() )
						{
							// ˳��
							if ( !FindOneOutgo( aiRun, 2 ) )
							{
								bTwoInCome = true;							
							}
						}

						if ( m_cwOutgo.empty() || bTwoInCome )
						{
							// ����(�϶��ǵ�������)
							FindOneIncome( aiRun, 2 );
						}
					}

					bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
				}
			}
			break;
		case cthree:									// 3�Ų���
			{
				ChoseOneDown( aiRun, 3, bOneMid || bTwoMid, bOneUp, bTwoUp );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case cfour:									// 4�Ų���
			{
				ChoseOneDown( aiRun, 4, bOneMid || bTwoMid, bOneUp, bTwoUp );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case ctth:										// ��˳(2)
			{
				ChoseLinkDown( aiRun, 3, 2, bOneMid || bTwoMid, bOneUp, bTwoUp );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case cthone:										// 3��1
			{
				ChoseOneDown( aiRun, 3, bOneMid || bTwoMid, bOneUp, bTwoUp );
				FindTowOne( m_cwOrderCards, 1, 1 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case cthtwo:										// 3��2
			{
				ChoseOneDown( aiRun, 3, bOneMid || bTwoMid, bOneUp, bTwoUp );
				FindTowOne( m_cwOrderCards, 2, 1 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
//		case cfone:											// 4��1							
//			{
//				ChoseOneDown( aiRun, 4, bOneMid || bTwoMid, bOneUp, bTwoUp );
//				FindTowOne( m_cwOrderCards, 1, 1 );

//				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
//			}
//			break;
//		case cftwo:											// 4��2
//			{
//				ChoseOneDown( aiRun, 4, bOneMid || bTwoMid, bOneUp, bTwoUp );
//				FindTowOne( m_cwOrderCards, 2, 1 );
//
//				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
//			}
//			break;
		case cttth:									// ˫˳(3)
			{
				ChoseLinkDown( aiRun, 2, 3, bOneMid || bTwoMid, bOneUp, bTwoUp );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case cftone:								// �Ĵ�����
			{
				ChoseOneDown( aiRun, 4, bOneMid || bTwoMid, bOneUp, bTwoUp );
				FindTowOne( m_cwOrderCards, 1, 2 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case cfttwo:										// �Ĵ�����
			{
				ChoseOneDown( aiRun, 4, bOneMid || bTwoMid, bOneUp, bTwoUp );
				FindTowOne( m_cwOrderCards, 2, 2 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case ctfour:									// ˫˳(4)
			{
				ChoseLinkDown( aiRun, 2, 4, bOneMid || bTwoMid, bOneUp, bTwoUp );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case cthfour:													// ��˳(4)
			{
				ChoseLinkDown( aiRun, 3, 4, bOneMid || bTwoMid, bOneUp, bTwoUp );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case ctthone:											// ��˳������
			{
				ChoseLinkDown( aiRun, 3, 2, bOneMid || bTwoMid, bOneUp, bTwoUp );
				FindTowOne( m_cwOrderCards, 1, 2 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case ctthtwo:										// ��˳������
			{
				ChoseLinkDown( aiRun, 3, 2, bOneMid || bTwoMid, bOneUp, bTwoUp );
				FindTowOne( m_cwOrderCards, 2, 2 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case clink5:									// ��˳(5)
			{
				ChoseLinkDown( aiRun, 1, 5, bOneMid || bTwoMid, bOneUp, bTwoUp );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case clink6:								// ��˳(6)
			{
				ChoseLinkDown( aiRun, 1, 6, bOneMid || bTwoMid, bOneUp, bTwoUp );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case clink7:								// ��˳(7)
			{
				ChoseLinkDown( aiRun, 1, 7, bOneMid || bTwoMid, bOneUp, bTwoUp );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case clink8:									// ��˳(8)
			{
				ChoseLinkDown( aiRun, 1, 8, bOneMid || bTwoMid, bOneUp, bTwoUp );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case clink9:									// ��˳(9)
			{
				ChoseLinkDown( aiRun, 1, 9, bOneMid || bTwoMid, bOneUp, bTwoUp );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case clink10:							// ��˳(10)
			{
				ChoseLinkDown( aiRun, 1, 10, bOneMid || bTwoMid, bOneUp, bTwoUp );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case clink11:								// ��˳(11)
			{
				ChoseLinkDown( aiRun, 1, 11, bOneMid || bTwoMid, bOneUp, bTwoUp );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case ctfive:									// ˫˳(5)
			{
				ChoseLinkDown( aiRun, 2, 5, bOneMid || bTwoMid, bOneUp, bTwoUp );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case cthfive:								// ��˳(5)
			{
				ChoseLinkDown( aiRun, 3, 5, bOneMid || bTwoMid, bOneUp, bTwoUp );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case ctsix:								// ˫˳(6)
			{
				ChoseLinkDown( aiRun, 2, 6, bOneMid || bTwoMid, bOneUp, bTwoUp );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case cththone:							// ��˳������
			{
				ChoseLinkDown( aiRun, 3, 3, bOneMid || bTwoMid, bOneUp, bTwoUp );
				FindTowOne( m_cwOrderCards, 1, 3 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case cththtwo:							// ��˳������
			{
				ChoseLinkDown( aiRun, 3, 3, bOneMid || bTwoMid, bOneUp, bTwoUp );
				FindTowOne( m_cwOrderCards, 2, 3 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case ctseven:							// ˫˳(7)
			{
				ChoseLinkDown( aiRun, 2, 7, bOneMid || bTwoMid, bOneUp, bTwoUp );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case ctevent:				     	// ˫˳(8)
			{
				ChoseLinkDown( aiRun, 2, 8, bOneMid || bTwoMid, bOneUp, bTwoUp );
				
				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		case cthfone:					// ��˳���ĵ�
			{
				ChoseLinkDown( aiRun, 3, 4, bOneMid || bTwoMid, bOneUp, bTwoUp );
				FindTowOne( m_cwOrderCards, 1, 4 );

				bTemp = m_RoleAI->PushCard( aiRun.rcIn, aiRun.cc );	
			}
			break;
		default:
			break;
	} 

	return bTemp;
}


//
//
//
void KddzAI::ChoseLinkMid( const AIRUN& aiRun, const unsigned int iNum, const unsigned int iWeidth, const bool bIn  )
{
	bool bLinkInCome = false;

	// ����
	if ( ChoseLinkOk( aiRun, iNum, iWeidth ) )
	{
		return;
	}

	// ũ�񱨵����
	if ( bIn )
	{
		// ����,���ܲ���, ����
		FindLink( aiRun, iNum, iWeidth );
	}
	else
	{
		// ˳��
		if ( !m_cwOutgo.empty() )
		{
			if ( !FindLinkOutgo( aiRun, iNum, iWeidth ) )
			{
				bLinkInCome = true;			
			}
		}

		// ����
		if ( m_cwOutgo.empty() || bLinkInCome )
		{
			// ����
			if ( !FindLinkIncome( aiRun, iNum, iWeidth ) )
			{
				// ���Ʊ�
				if ( aiRun.cc == ctth ||
					 aiRun.cc == cttth ||
					 aiRun.cc == cthth ||
					 aiRun.cc == ctfour ||
					 aiRun.cc == cthfour ||
					 aiRun.cc == ctthone ||
					 aiRun.cc == ctthtwo ||
					 aiRun.cc == clink8 ||
					 aiRun.cc == clink9 ||
					 aiRun.cc == clink10 ||
					 aiRun.cc == clink11 ||
					 aiRun.cc == clink12 ||
					 aiRun.cc == ctfive ||
					 aiRun.cc == cthfive ||
					 aiRun.cc == ctsix ||
					 aiRun.cc == cthsix ||
					 aiRun.cc == cththone ||
					 aiRun.cc == cththtwo ||
					 aiRun.cc == ctseven ||
					 aiRun.cc == ctevent ||
					 aiRun.cc == ctnine ||
					 aiRun.cc == cthfone ||
				  	 aiRun.cc == cfttwo ||
					 aiRun.cc == cftone )
				{
					PushZha( aiRun );
				}
			}
		}
	}
}


//
//
//
void KddzAI::ChoseLinkUp( const AIRUN& aiRun, const unsigned int iNum, 
						 const unsigned int iWeidth, const bool bIn,
						 const bool bOneDown, const bool bTwoDown )
{
	bool bLinkInCome = false;

	// ����
	if ( ChoseLinkOk( aiRun, iNum, iWeidth ) )
	{
		return;
	}

	// �����������
	if ( bIn )
	{
		// ����ͬ�������
		if ( aiRun.aipos != p_down )						
		{
			// ����
			FindLink( aiRun, iNum, iWeidth );
		}
	}

	// ͬ�鱨��,�ҵ���������
	else if ( bOneDown && 
			  aiRun.aipos != p_down )
	{
		// ����
		if ( !FindLink( aiRun, iNum, iWeidth ) )
		{
			PushZha( aiRun );
		}
	}

	// ͬ�鱨��
	else if ( bTwoDown && 
			  aiRun.aipos != p_down )
	{
		// ����
		if ( !FindLink( aiRun, iNum, iWeidth ) )
		{
			PushZha( aiRun );
		}
	}
				
	else
	{
		// ˳��
		if ( !m_cwOutgo.empty() )
		{
			// ˳��
			if ( !FindLinkOutgo( aiRun, iNum, iWeidth ) &&
				 aiRun.aipos != p_down )
			{
				bLinkInCome = true;						
			}
		}

		if ( m_cwOutgo.empty() || bLinkInCome )
		{
			// ����(����ͬ��Ŵ�)
			if ( !FindLinkIncome( aiRun, iNum, iWeidth ) )
			{
				// ���Ʊ�
				if ( aiRun.cc == ctth ||
					 aiRun.cc == cttth ||
					 aiRun.cc == cthth ||
					 aiRun.cc == ctfour ||
					 aiRun.cc == cthfour ||
					 aiRun.cc == ctthone ||
					 aiRun.cc == ctthtwo ||
					 aiRun.cc == clink8 ||
					 aiRun.cc == clink9 ||
					 aiRun.cc == clink10 ||
					 aiRun.cc == clink11 ||
					 aiRun.cc == clink12 ||
					 aiRun.cc == ctfive ||
					 aiRun.cc == cthfive ||
					 aiRun.cc == ctsix ||
					 aiRun.cc == cthsix ||
					 aiRun.cc == cththone ||
					 aiRun.cc == cththtwo ||
					 aiRun.cc == ctseven ||
					 aiRun.cc == ctevent ||
					 aiRun.cc == ctnine ||
					 aiRun.cc == cthfone ||
					 aiRun.cc == cfttwo ||
					 aiRun.cc == cftone )
				{
					PushZha( aiRun );
				}
			}
		}
	}
}


//
//
//
void KddzAI::ChoseLinkDown( const AIRUN& aiRun, const unsigned int iNum, 
						 const unsigned int iWeidth, const bool bIn,
						 const bool bOneUp, const bool bTwoUp )
{
	bool bLinkInCome = false;
	
	// ����
	if ( ChoseLinkOk( aiRun, iNum, iWeidth ) )
	{
		return;
	}

	// ͬ�������, �������ܣ���Ҳ����
	if ( aiRun.aipos == p_up )
	{
		return;
	}
	else
	{
		// �����������
		if ( bIn )
		{
			// ����
			if ( !FindLink( aiRun, iNum, iWeidth ) && ( bOneUp || bTwoUp ) )
			{
				PushZha( aiRun );
			}
		}

		// ͬ�鱨��
		else if ( bOneUp && 
				  aiRun.aipos != p_up )
		{
			// ����
			if ( !FindLink( aiRun, iNum, iWeidth ) )
			{
				PushZha( aiRun );
			}
		}

		// ͬ�鱨��
		else if ( bTwoUp && 
				 aiRun.aipos != p_up )
		{
			// ����
			if ( !FindLink( aiRun, 1, 5 ) )
			{
				PushZha( aiRun );
			}
		}

		else
		{
			// ˳��
			if ( !m_cwOutgo.empty() )
			{
				// ˳��
				if ( !FindLinkOutgo( aiRun, iNum, iWeidth ) )
				{
					bLinkInCome = true;							
				}
			}

			if ( m_cwOutgo.empty() || bLinkInCome )
			{
				// ����(�϶��ǵ�������)
				if ( !FindLinkIncome( aiRun, iNum, iWeidth ) )
				{
					// ���Ʊ�
					if ( aiRun.cc == ctth ||
						 aiRun.cc == cttth ||
						 aiRun.cc == cthth ||
						 aiRun.cc == ctfour ||
						 aiRun.cc == cthfour ||
						 aiRun.cc == ctthone ||
						 aiRun.cc == ctthtwo ||
						 aiRun.cc == clink8 ||
						 aiRun.cc == clink9 ||
						 aiRun.cc == clink10 ||
						 aiRun.cc == clink11 ||
						 aiRun.cc == clink12 ||
						 aiRun.cc == ctfive ||
						 aiRun.cc == cthfive ||
						 aiRun.cc == ctsix ||
						 aiRun.cc == cthsix ||
						 aiRun.cc == cththone ||
						 aiRun.cc == cththtwo ||
						 aiRun.cc == ctseven ||
						 aiRun.cc == ctevent ||
						 aiRun.cc == ctnine ||
						 aiRun.cc == cthfone ||
						 aiRun.cc == cfttwo ||
						 aiRun.cc == cftone )
					{
						PushZha( aiRun );
					}
				}
			}
		}
	}
}


//
//
//
void KddzAI::ChoseOneMid( const AIRUN& aiRun, const unsigned int iNum, const bool bIn )
{
	bool bOneInCome = false;

	// ����
	if ( ChoseOneOk( aiRun, iNum ) )
	{
		return;
	}

	// ũ�񱨵����
	if ( bIn )
	{
		// ����,���ܲ���, ����
		FindOne( aiRun, iNum );
	}
	else
	{
		// ˳��
		if ( !m_cwOutgo.empty() )
		{
			// ˳��
			if ( !FindOneOutgo( aiRun, iNum ) )
			{
				bOneInCome = true;			
			}
		}

		// ����
		if ( m_cwOutgo.empty() || bOneInCome )
		{
			// ����
			if ( !FindOneIncome( aiRun, iNum ) )
			{
				// ���Ʊ�
				if ( aiRun.cc == ctth ||
					 aiRun.cc == cttth ||
					 aiRun.cc == cthth ||
					 aiRun.cc == ctfour ||
					 aiRun.cc == cthfour ||
					 aiRun.cc == ctthone ||
					 aiRun.cc == ctthtwo ||
					 aiRun.cc == clink8 ||
					 aiRun.cc == clink9 ||
					 aiRun.cc == clink10 ||
					 aiRun.cc == clink11 ||
					 aiRun.cc == clink12 ||
					 aiRun.cc == ctfive ||
					 aiRun.cc == cthfive ||
					 aiRun.cc == ctsix ||
					 aiRun.cc == cthsix ||
					 aiRun.cc == cththone ||
					 aiRun.cc == cththtwo ||
					 aiRun.cc == ctseven ||
					 aiRun.cc == ctevent ||
					 aiRun.cc == ctnine ||
					 aiRun.cc == cthfone ||
					 aiRun.cc == cfttwo ||
					 aiRun.cc == cftone )
				{
					PushZha( aiRun );
				}
			}
		}
	}
}


//
// 3������
//
void KddzAI::ChoseOneUp( const AIRUN& aiRun, const unsigned int iNum, 
					  const bool bIn, const bool bOneDown, const bool bTwoDown )
{
	bool bOneInCome = false;

	// ����
	if ( ChoseOneOk( aiRun, iNum ) )
	{
		return;
	}

	// �����������
	if ( bIn &&
		 aiRun.aipos != p_down &&
		 !bOneDown && !bTwoDown )
	{
		// ����
		FindOne( aiRun, iNum );
	}

	// ͬ�鱨��,�ҵ���������
	else if ( bOneDown && 
			  aiRun.aipos != p_down )
	{
		// ����
		if ( !FindOne( aiRun, iNum ) )
		{
			PushZha( aiRun );
		}
	}

	// ͬ�鱨��
	else if ( bTwoDown && 
			  aiRun.aipos != p_down )
	{
		// ����
		if ( !FindOne( aiRun, iNum ) )
		{
			PushZha( aiRun );
		}
	}
			
	else
	{
		// ˳��
		if ( !m_cwOutgo.empty() )
		{
			// ˳��
			if ( !FindOneOutgo( aiRun, iNum, aiRun.aipos == p_down ) &&
				 aiRun.aipos != p_down )
			{
				bOneInCome = true;			
			}
		}

		if ( m_cwOutgo.empty() || bOneInCome )
		{
			// ����(����ͬ��Ŵ�)
			if ( !FindOneIncome( aiRun, iNum ) )
			{
				// ���Ʊ�
				if ( aiRun.cc == ctth ||
					 aiRun.cc == cttth ||
					 aiRun.cc == cthth ||
					 aiRun.cc == ctfour ||
					 aiRun.cc == cthfour ||
					 aiRun.cc == ctthone ||
					 aiRun.cc == ctthtwo ||
					 aiRun.cc == clink8 ||
					 aiRun.cc == clink9 ||
					 aiRun.cc == clink10 ||
					 aiRun.cc == clink11 ||
					 aiRun.cc == clink12 ||
					 aiRun.cc == ctfive ||
					 aiRun.cc == cthfive ||
					 aiRun.cc == ctsix ||
					 aiRun.cc == cthsix ||
					 aiRun.cc == cththone ||
					 aiRun.cc == cththtwo ||
					 aiRun.cc == ctseven ||
					 aiRun.cc == ctevent ||
					 aiRun.cc == ctnine ||
					 aiRun.cc == cthfone ||
					 aiRun.cc == cfttwo ||
					 aiRun.cc == cftone
					 )
				{
					PushZha( aiRun );
				}
			}
		}
	}
}


//
//
//
void KddzAI::ChoseOneDown( const AIRUN& aiRun, const unsigned int iNum, 
					  const bool bIn, const bool bOneUp, const bool bTwoUp )
{
	bool bThreeInCome = false;
	
	// ����
	if ( ChoseOneOk( aiRun, iNum ) )
	{
		return;
	}

	// ͬ�������, �������ܣ���Ҳ����
	if ( aiRun.aipos == p_up )
	{
		return;
	}
	else
	{
		// �����������
		if ( bIn && 
			 aiRun.aipos != p_up )
		{
			// ����(����Լ����Ҳ�����ˣ����ұ�)
			if ( !FindOne( aiRun, iNum ) && ( bOneUp || bTwoUp ) )
			{
				PushZha( aiRun );
			}
		}

		// ͬ�鱨��
		else if ( bOneUp && 
				  aiRun.aipos != p_up )
		{
			// ����
			if ( !FindOne( aiRun, iNum ) )
			{
				PushZha( aiRun );
			}
		}

		// ͬ�鱨��
		else if ( bTwoUp && 
				  aiRun.aipos != p_up )
		{
			// ����
			if ( !FindOne( aiRun, iNum ) )
			{
				PushZha( aiRun );
			}
		}

		else
		{
			// ˳��
			if ( !m_cwOutgo.empty() )
			{
				// ˳��
				if ( !FindOneOutgo( aiRun, iNum, aiRun.aipos == p_up ) )
				{
					bThreeInCome = true;							
				}
			}

			if ( m_cwOutgo.empty() || bThreeInCome )
			{
				// ����(�϶��ǵ�������)
				if ( !FindOneIncome( aiRun, iNum ) )
				{
					// ���Ʊ�
					if ( aiRun.cc == ctth ||
						 aiRun.cc == cttth ||
						 aiRun.cc == cthth ||
						 aiRun.cc == ctfour ||
						 aiRun.cc == cthfour ||
						 aiRun.cc == ctthone ||
						 aiRun.cc == ctthtwo ||
						 aiRun.cc == clink8 ||
						 aiRun.cc == clink9 ||
						 aiRun.cc == clink10 ||
						 aiRun.cc == clink11 ||
						 aiRun.cc == clink12 ||
						 aiRun.cc == ctfive ||
						 aiRun.cc == cthfive ||
						 aiRun.cc == ctsix ||
						 aiRun.cc == cthsix ||
						 aiRun.cc == cththone ||
						 aiRun.cc == cththtwo ||
						 aiRun.cc == ctseven ||
						 aiRun.cc == ctevent ||
						 aiRun.cc == ctnine ||
						 aiRun.cc == cthfone ||
						 aiRun.cc == cfttwo ||
						 aiRun.cc == cftone
						 )
					{
						PushZha( aiRun );
					}
				}
			}
		}
	}
}



//
// ȡ�����͵�����(���㹻��):bIn���������ը���Թ�����Ӱ��
//
int KddzAI::GetHandNum( const bool bIn )
{
	int iSdeOne = 0;
	int iSdeTwo = 0;
	int iSde = 0;
	int ibeg = 0;
	bool bTemp = bIn;

	if ( !m_cwOutgo.empty() )
	{
		for ( int i = 0; i < m_cwOutgo.size(); ++i )
		{
			if ( m_cwOutgo[i].csif == sde )
			{
				++iSde;
				if ( m_cwOutgo[i].rcard.iNum == 1 )
				{
					++iSdeOne;
				}
				else if ( m_cwOutgo[i].rcard.iNum == 2 )
				{
					++iSdeTwo;
				}
			}
			else if ( m_cwOutgo[i].csif == beg )
			{
				++ibeg;
			}

		}
	}

	if ( !m_cwIncome.empty() )
	{
		for ( int j = 0; j < m_cwIncome.size(); ++j )
		{
			if ( m_cwIncome[j].csif == sde )
			{
				// ��3����������ǲ������ܴ�
				if ( m_cwIncome[j].rcard.iNum == 3 &&
					 ( iSdeOne >= 1 || iSdeTwo >= 1 ) )
				{
					--iSde;
				}
				else if ( m_cwIncome[j].rcard.iNum == 4 )
				{	
					if ( bTemp )
					{						
						if ( iSdeOne >= 2 || iSdeTwo >= 2 )
						{
							iSde -= 2;
						}
					}
					else
					{
						// һ�����ϵ�ը��,BUG����
						bTemp = true;
					}
				}
			}
			
			// 3˳
			else if ( m_cwIncome[j].csif == beg &&
				      m_cwIncome[j].rcard.iNum == 3 )
			{
				int jTemp = j;
				while ( jTemp < m_cwIncome.size() )
				{
					if ( m_cwIncome[jTemp].csif == end )
					{
						break;
					}
					++jTemp;
				}

				int iTemp = jTemp - j + 1;

				if ( iSdeOne >= iTemp || iSdeTwo >= iTemp )
				{
					iSde -= iTemp;
				}
			}
		}
	}

	iSde = ( iSde <= 0 ) ? 0 : iSde;

	return ibeg + iSde;
}


//
// ��
//
bool KddzAI::ChoseOneOk( const AIRUN& aiRun, const unsigned int iIn )
{
	int iFour = ( iIn == 4 ) ? 4 : 3;
	CARDCHANGE rcTemp = R3;				// ��С

	// ����
	if ( GetHandNum( true ) <= 1 )	
	{
		// �ҳ����
		for ( int rc = aiRun.rcIn.size() - 1; rc >= 0; --rc )
		{	
			if ( aiRun.rcIn[rc].iNum == iIn &&
				 aiRun.rcIn[rc].rc > rcTemp )
			{
				rcTemp = aiRun.rcIn[rc].rc;
			}
		}

		if ( !m_cwIncome.empty() )
		{
			// ��SDE(ƥ���)
			for ( int i = m_cwIncome.size() - 1; i >= 0; --i )
			{
				if ( m_cwIncome[i].csif == sde &&
					 m_cwIncome[i].rcard.iNum == iIn &&
					 m_cwIncome[i].rcard.rc > rcTemp )
				{
					CWVector cwTemp;
					CARDWORTH cw;

					cw.csif = sde;
					cw.rcard.iNum = iIn;
					cw.rcard.rc = m_cwIncome[i].rcard.rc;
					cwTemp.push_back( cw );

					if ( GetBig( cwTemp ) )
					{
						ChooseCards( m_cwIncome[i].rcard.rc, iIn );
						return true;
					}
				}
			}
		

			if ( GetHandNum( false ) <= 1 )
			{
				// ��ը��
				for ( int n = m_cwIncome.size() - 1; n >= 0 ; --n )
				{
					if ( m_cwIncome[n].rcard.iNum == 4 )
					{
						ChooseCards( m_cwIncome[n].rcard.rc, 4 );
						return true;
					}
				}
			}

			// ��˫��
			if ( m_cwIncome.size() >= 2 &&
				 m_cwIncome[0].rcard.rc == RWB &&
				 m_cwIncome[1].rcard.rc == RWS )
			{
				ChooseCards( m_cwIncome[0].rcard.rc, 1 );
				ChooseCards( m_cwIncome[1].rcard.rc, 1 );
				return true;
			}
			
			// ���Ǵ�����,��
			if ( m_cwOutgo.empty() )
			{
				for ( int j = m_cwIncome.size() - 1; j >= 0; --j )
				{
					if ( m_cwIncome[j].csif == sde &&
						 m_cwIncome[j].rcard.iNum >= iIn &&
						 m_cwIncome[j].rcard.rc > rcTemp )
					{
						CWVector cwTemp;
						CARDWORTH cw;

						cw.csif = sde;
						cw.rcard.iNum = iIn;
						cw.rcard.rc = m_cwIncome[j].rcard.rc;
						cwTemp.push_back( cw );

						if ( m_cwIncome[j].rcard.rc == RT || GetBig( cwTemp ) )
						{
							ChooseCards( m_cwIncome[j].rcard.rc, iIn );
							return true;
						}
					}
				}
			}
		}

		

		// û������
		if ( !m_cwOutgo.empty() )
		{
			for ( int j = 0; j < m_cwOutgo.size(); ++j )
			{
				if ( m_cwOutgo[j].csif == sde &&
					 m_cwOutgo[j].rcard.iNum == iIn &&
					 m_cwOutgo[j].rcard.rc > rcTemp )
				{
					ChooseCards( m_cwOutgo[j].rcard.rc, 
								 m_cwOutgo[j].rcard.iNum );
					return true;
				}
			}
		}
	}

	return false;
}



//
// ��
//
bool KddzAI::ChoseLinkOk( const AIRUN& aiRun, const unsigned int iNum, const unsigned int iWeidth )
{
	CARDCHANGE rcTemp = RWB;				// ���

	// ����
	if ( GetHandNum( true ) <= 1 )	
	{
		// �ҳ���С
		for ( int rc = aiRun.rcIn.size() - 1; rc >= 0; --rc )
		{	
			if ( aiRun.rcIn[rc].iNum == iNum &&
				 aiRun.rcIn[rc].rc < rcTemp )
			{
				rcTemp = aiRun.rcIn[rc].rc;
			}
		}

		if ( !m_cwIncome.empty() )
		{
			// ����Ե�
			for ( int i = m_cwIncome.size() - 1; i >= 0; --i )
			{
				if ( m_cwIncome[i].csif == end &&
					 m_cwIncome[i].rcard.iNum == iNum &&
					 m_cwIncome[i].rcard.rc > rcTemp )
				{
					int iTempWhile = i;
					while ( iTempWhile >= 0 )
					{
						if ( m_cwIncome[iTempWhile].csif == beg )
						{
							break;
						}
						--iTempWhile;
					}

					if ( iTempWhile - i + 1 == iWeidth )
					{
						while ( i >= iTempWhile )
						{
							ChooseCards( m_cwIncome[i].rcard.rc, 
										 m_cwIncome[i].rcard.iNum );
							--i;
						}

						return true;
					}
				}
			}

			if ( GetHandNum( false ) <= 1 )
			{
				// ��ը��
				for ( int n = m_cwIncome.size() - 1; n >= 0 ; --n )
				{
					if ( m_cwIncome[n].rcard.iNum == 4 )
					{
						ChooseCards( m_cwIncome[n].rcard.rc, 4 );
						return true;
					}
				}
			}

			// ��˫��
			if ( m_cwIncome.size() >= 2 &&
				 m_cwIncome[0].rcard.rc == RWB &&
				 m_cwIncome[1].rcard.rc == RWS )
			{
				ChooseCards( m_cwIncome[0].rcard.rc, 1 );
				ChooseCards( m_cwIncome[1].rcard.rc, 1 );
				return true;
			}
		}

		// û������
		if ( !m_cwOutgo.empty() )
		{
			for ( int j = 0; j < m_cwOutgo.size(); ++j )
			{
				if ( m_cwOutgo[j].csif == end &&
					 m_cwOutgo[j].rcard.iNum == iNum &&
					 m_cwOutgo[j].rcard.rc > rcTemp )
				{
					int iTempWhile = j;
					while ( iTempWhile >= 0 )
					{
						if ( m_cwOutgo[iTempWhile].csif == beg )
						{
							break;
						}
						--iTempWhile;
					}

					if ( iTempWhile - j + 1 == iWeidth )
					{
						while ( j >= iTempWhile )
						{
							ChooseCards( m_cwOutgo[j].rcard.rc, 
										 m_cwOutgo[j].rcard.iNum );
							--j;
						}
						return true;
					}
				}
			}
		}
	}

	return false;
}


//
// ��ը��
//
void KddzAI::PushZha( const AIRUN& aiRun )
{
	if ( !m_cwIncome.empty() )
	{
		
		// ��ը��
		for ( int n = m_cwIncome.size() - 1; n >= 0 ; --n )
		{
			if ( m_cwIncome[n].rcard.iNum == 4 )
			{
				ChooseCards( m_cwIncome[n].rcard.rc, 4 );
				return;
			}
		}

		// ��˫��
		if ( m_cwIncome.size() >= 2 &&
			 m_cwIncome[0].rcard.rc == RWB &&
			 m_cwIncome[1].rcard.rc == RWS )
		{
			ChooseCards( m_cwIncome[0].rcard.rc, 1 );
			ChooseCards( m_cwIncome[1].rcard.rc, 1 );
		}
	}
}


//
// ����
//
bool KddzAI::Patch2w( const bool bIn )
{
	// �ԼҾ���,�����Լ�������
	if ( bIn && GetHandNum( true ) > 1 )
	{
		// ����
		if ( m_cwIncome.size() >= 2 &&
			 m_cwIncome[0].rcard.rc == RWB &&
			 m_cwIncome[1].rcard.rc == RWS )
		{
			// ѡС��
			ChooseCards( m_cwIncome[1].rcard.rc, 1 );
			return true;
		}
	}

	return false;
}



//
// 3��һ�Ĳ���
//
bool KddzAI::Patch3And( void )
{
	if ( !m_cwIncome.empty() && !m_cwOutgo.empty() )
	{
		if ( m_cwIncome.size() == 1 && 
			 m_cwIncome[0].csif == sde &&
			 ( m_cwIncome[0].rcard.iNum <= 2 ) )
		{
			if ( m_cwOutgo.size() == 1 && 
				 m_cwOutgo[0].csif == sde &&
				 m_cwOutgo[0].rcard.iNum == 3 )
			{
				ChooseCards( m_cwIncome[0].rcard.rc, m_cwIncome[0].rcard.iNum );
				ChooseCards( m_cwOutgo[0].rcard.rc, m_cwOutgo[0].rcard.iNum );
				return true;
			}	
		}
	}

	return false;
}


//
// Ԥ����, Outgo handnum == 2
//
int KddzAI::UpEnough( void )
{
	// iNum ������4�Ŀ���
	for ( int i = m_cwOutgo.size() - 1; i >= 0; --i )
	{
		// ��
		if ( m_cwOutgo[i].csif == sde )
		{
			for ( int j = m_cwIncome.size() - 1; j >= 0; --j )
			{
				// �������
				if ( m_cwOutgo[i].rcard.iNum == m_cwIncome[j].rcard.iNum )
				{
					ChooseCards( m_cwOutgo[i].rcard.rc, m_cwOutgo[i].rcard.iNum );
					return m_cwOutgo[i].rcard.iNum;
				}
			}
		}

		// ˳
		else if ( m_cwOutgo[i].csif == end )
		{
			// ������,��Ϊ���Ҳ����ܹ��ϵ��ƣ����ǻ����ȳ�˳��
		}
	}
	
	return 0;
}