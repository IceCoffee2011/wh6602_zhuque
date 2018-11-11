
//
// 2009
// DhLj
//

#include "Roles.h"

//
// *
//
Roles::Roles( void ) 
{
}

//
// *
//
Roles::~Roles( void )
{
}

//
// ����
//
void Roles::SetCards( IVector& ivIn )
{
	int iTemp;

	// ���m_Cards
	if ( !m_Cards.empty() )
	{
		m_Cards.erase( m_Cards.begin(), m_Cards.end() );
	}

	// ѡ������
	for ( int j = 0; j < ivIn.size(); ++j )
	{
		for ( int i = 0; i < ivIn.size(); ++i )
		{
			if ( ivIn[j] > ivIn[i] )
			{
				iTemp = ivIn[j];
				ivIn[j] = ivIn[i];
				ivIn[i] = iTemp;
			}
		}
	}

	CARD cTemp;
	for ( int k = 0; k < ivIn.size(); ++k )
	{	
		cTemp.iCard = ivIn[k];
		cTemp.cs = no;

		m_Cards.push_back( cTemp );
	}
}


//
// ת����,���浽m_RCards
//
void Roles::ChangeCard( void )
{
	RCARD rcTemp;
	int iTemp = 0;

	// ������
	if ( m_ivTemp.empty() )
	{
		return;
	}

	// ���m_RCards
	if ( !m_RCards.empty() )
	{
		m_RCards.erase( m_RCards.begin(), m_RCards.end() );
	}

	for ( int j = 0; j < m_ivTemp.size(); ++j )
	{
		if ( m_ivTemp[j] > 0 && m_ivTemp[j] <= 52 )			// �ų���С��
		{
			iTemp = ( m_ivTemp[j] - 1 ) / 4 + 3;
		}
		else
		{
			iTemp = m_ivTemp[j];
		}
		
		switch ( iTemp )
		{
		case 3:
			rcTemp.rc = R3;
			break;
		case 4:
			rcTemp.rc = R4;
			break;
		case 5:
			rcTemp.rc = R5;
			break;
		case 6:
			rcTemp.rc = R6;
			break;
		case 7:
			rcTemp.rc = R7;
			break;
		case 8:
			rcTemp.rc = R8;
			break;
		case 9:
			rcTemp.rc = R9;
			break;
		case 10:
			rcTemp.rc = RO;
			break;
		case 11:
			rcTemp.rc = RJ;
			break;
		case 12:
			rcTemp.rc = RQ;
			break;
		case 13:
			rcTemp.rc = RK;
			break;
		case 14:
			rcTemp.rc = RA;
			break;
		case 15:
			rcTemp.rc = RT;
			break;
		case 53:
			rcTemp.rc = RWS;
			break;
		case 54:
			rcTemp.rc = RWB;
			break;
		default:
			break;
		}

		// ����Ҫ��֤VECTOR����������
		if ( !m_RCards.empty() && m_RCards[m_RCards.size() - 1].rc == rcTemp.rc )
		{
			++m_RCards[m_RCards.size() - 1].iNum;
		}
		else
		{
			rcTemp.iNum = 1;
			m_RCards.push_back( rcTemp );
		}

	}
}

//����
bool Roles::ChangeCardToIvVertor(const IVector& ivIn, RCVector& rcVec)
{
	RCARD rcTemp;
	int iTemp = 0;

	// ������
	if ( ivIn.empty() )
	{
		return false;
	}

	// ���m_RCards
	if ( !rcVec.empty() )
	{
		rcVec.erase( rcVec.begin(), rcVec.end() );
	}

	for ( int j = 0; j < ivIn.size(); ++j )
	{
		if ( ivIn[j] > 0 && ivIn[j] <= 52 )			// �ų���С��
		{
			iTemp = ( ivIn[j] - 1 ) / 4 + 3;
		}
		else
		{
			iTemp = ivIn[j];
		}

		switch ( iTemp )
		{
		case 3:
			rcTemp.rc = R3;
			break;
		case 4:
			rcTemp.rc = R4;
			break;
		case 5:
			rcTemp.rc = R5;
			break;
		case 6:
			rcTemp.rc = R6;
			break;
		case 7:
			rcTemp.rc = R7;
			break;
		case 8:
			rcTemp.rc = R8;
			break;
		case 9:
			rcTemp.rc = R9;
			break;
		case 10:
			rcTemp.rc = RO;
			break;
		case 11:
			rcTemp.rc = RJ;
			break;
		case 12:
			rcTemp.rc = RQ;
			break;
		case 13:
			rcTemp.rc = RK;
			break;
		case 14:
			rcTemp.rc = RA;
			break;
		case 15:
			rcTemp.rc = RT;
			break;
		case 53:
			rcTemp.rc = RWS;
			break;
		case 54:
			rcTemp.rc = RWB;
			break;
		default:
			break;
		}

		// ����Ҫ��֤VECTOR����������
		if ( !rcVec.empty() && rcVec[rcVec.size() - 1].rc == rcTemp.rc )
		{
			++rcVec[rcVec.size() - 1].iNum;
		}
		else
		{
			rcTemp.iNum = 1;
			rcVec.push_back( rcTemp );
		}

	}

	return true;
}


CARDCLASS Roles::GetCardRCVector(const RCVector& rcVec)
{
	CARDCLASS ccTemp = cerror;

	switch ( rcVec.size() )
	{
	case 1:												// һ����
		{
			switch ( rcVec[0].iNum )
			{
			case 1:										// ����
				{
					ccTemp = cone;
				}
				break;
			case 2:										// ����
				{
					ccTemp = ctwo;
				}
				break;
			case 3:										// ����
				{
					ccTemp = cthree;
				}
				break;
			case 4:										// ը��
				{
					ccTemp = cfour;
				}
				break;
			default:
				break;
			}
		}
		break;
	case 2:
		{
			// ��˳
			if ( rcVec[0].rc <= RA && 
				FindLinkTemp(rcVec,  3, 0, 1 ) )
			{
				ccTemp = ctth;
			}

			// ����һ��
			else if ( rcVec[0].iNum == 3 && rcVec[1].iNum == 1 ||
				rcVec[1].iNum == 3 && rcVec[0].iNum == 1 )
			{
				ccTemp = cthone;
			}

			// ����һ��
			else if ( rcVec[0].iNum == 3 && rcVec[1].iNum == 2 ||
				rcVec[1].iNum == 3 && rcVec[0].iNum == 2 )
			{
				ccTemp = cthtwo;
			}

			//			// �Ĵ�һ��
			//			else if ( rcVec[0].iNum == 4 && rcVec[1].iNum == 1 ||
			//					  rcVec[1].iNum == 4 && rcVec[0].iNum == 1 )
			//			{
			//				ccTemp = cfone;
			//			}
			//
			//			// �Ĵ�һ��
			//			else if ( rcVec[0].iNum == 4 && rcVec[1].iNum == 2 ||
			//					  rcVec[1].iNum == 4 && rcVec[0].iNum == 2 )
			//			{
			//				ccTemp = cftwo;
			//			}

			// ˫��
			else if ( ( rcVec[0].rc == RWS && rcVec[1].rc == RWB ) ||
				( rcVec[1].rc == RWS && rcVec[0].rc == RWB ) )
			{
				ccTemp = chigh;
			}
		}
		break;
	case 3:
		{
			// ˫˳(3)	
			if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 2, 0, 2 ) )
			{
				ccTemp = cttth;
			}

			// ��˳(3)	
			else if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 3, 0, 2 ) )
			{
				ccTemp = cthth;
			}

			// �Ĵ�����
			else if ( rcVec[0].iNum == 4 && rcVec[1].iNum == 1 &&
				rcVec[2].iNum == 1 ||
				rcVec[1].iNum == 4 && rcVec[0].iNum == 1 &&
				rcVec[2].iNum == 1 ||
				rcVec[2].iNum == 4 && rcVec[0].iNum == 1 &&
				rcVec[1].iNum == 1 )
			{
				ccTemp = cftone;
			}

			// �Ĵ�����
			else if ( rcVec[0].iNum == 4 && rcVec[1].iNum == 2 &&
				rcVec[2].iNum == 2 ||
				rcVec[1].iNum == 4 && rcVec[0].iNum == 2 &&
				rcVec[2].iNum == 2 ||
				rcVec[2].iNum == 4 && rcVec[0].iNum == 2 &&
				rcVec[1].iNum == 2 )
			{
				ccTemp = cfttwo;
			}
		}
		break;
	case 4:
		{
			// ˫˳(4)
			if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 2, 0, 3 ) )
			{
				ccTemp = ctfour;
			}

			// ��˳(4)
			else if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 3, 0, 3 ) )
			{
				ccTemp = cthfour;
			}

			// ��˳������
			else if ( rcVec[0].rc <= RA &&
				( FindLinkTemp(rcVec, 3, 0, 1 ) && 
				rcVec[2].iNum == 1 && rcVec[3].iNum == 1 ) ||
				( FindLinkTemp(rcVec, 3, 1, 2 ) && 
				rcVec[0].iNum == 1 && rcVec[3].iNum == 1 ) ||
				( FindLinkTemp(rcVec, 3, 2, 3 ) && 
				rcVec[0].iNum == 1 && rcVec[1].iNum == 1 ) )
			{
				ccTemp = ctthone;
			}

			// ��˳������
			else if ( rcVec[0].rc <= RA &&
				( FindLinkTemp(rcVec, 3, 0, 1 ) && 
				rcVec[2].iNum == 2 && rcVec[3].iNum == 2 ) ||
				( FindLinkTemp(rcVec, 3, 1, 2 ) && 
				rcVec[0].iNum == 2 && rcVec[3].iNum == 2 ) ||
				( FindLinkTemp(rcVec, 3, 2, 3 ) && 
				rcVec[0].iNum == 2 && rcVec[1].iNum == 2 ) )
			{
				ccTemp = ctthtwo;
			}

		}
		break;
	case 5:
		{
			// ��˳(5)
			if ( rcVec[0].rc <= RA &&
				FindLinkTemp(rcVec, 1, 0, 4 ) )
			{
				ccTemp = clink5;
			}

			// ˫˳(5)
			else if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 2, 0, 4 ) )
			{
				ccTemp = ctfive;
			}

			// ��˳(5)
			else if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 3, 0, 4 ) )
			{
				ccTemp = cthfive;
			}
		}
		break;
	case 6:
		{
			// ��˳(6)
			if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 1, 0, 5 ) )
			{
				ccTemp = clink6;
			}

			// ˫˳(6)
			else if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 2, 0, 5 ) )
			{
				ccTemp = ctsix;
			}

			// ��˳(6)
			else if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 3, 0, 5 ) )
			{
				ccTemp = cthsix;
			}

			// ��˳������
			else if ( rcVec[0].rc <= RA &&
				( FindLinkTemp(rcVec, 3, 0, 2 ) && 
				rcVec[3].iNum == 1 && rcVec[4].iNum == 1 &&
				rcVec[5].iNum == 1 ) ||
				( FindLinkTemp(rcVec, 3, 1, 3 ) && 
				rcVec[0].iNum == 1 && rcVec[4].iNum == 1 &&
				rcVec[5].iNum == 1 ) ||
				( FindLinkTemp(rcVec, 3, 2, 4 ) && 
				rcVec[0].iNum == 1 && rcVec[1].iNum == 1 &&
				rcVec[5].iNum == 1 ) ||
				( FindLinkTemp(rcVec, 3, 3, 5 ) && 
				rcVec[0].iNum == 1 && rcVec[1].iNum == 1 &&
				rcVec[2].iNum == 1 ) )
			{
				ccTemp = cththone;
			}

			// ��˳������
			else if ( rcVec[0].rc <= RA &&
				( FindLinkTemp(rcVec, 3, 0, 2 ) && 
				rcVec[3].iNum == 2 && rcVec[4].iNum == 2 &&
				rcVec[5].iNum == 2 ) ||
				( FindLinkTemp(rcVec, 3, 1, 3 ) && 
				rcVec[0].iNum == 2 && rcVec[4].iNum == 2 &&
				rcVec[5].iNum == 2 ) ||
				( FindLinkTemp(rcVec, 3, 2, 4 ) && 
				rcVec[0].iNum == 2 && rcVec[1].iNum == 2 &&
				rcVec[5].iNum == 2 ) ||
				( FindLinkTemp(rcVec, 3, 3, 5 ) && 
				rcVec[0].iNum == 2 && rcVec[1].iNum == 2 &&
				rcVec[2].iNum == 2 ) )
			{
				ccTemp = cththtwo;
			}
		}
		break;
	case 7:
		{
			// ��˳(7)
			if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 1, 0, 6 ) )
			{
				ccTemp = clink7;
			}

			// ˫˳(7)
			else if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 2, 0, 6 ) )
			{
				ccTemp = ctseven;
			}
		}
		break;
	case 8:
		{
			// ��˳(8)
			if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 1, 0, 7 ) )
			{
				ccTemp = clink8;
			}

			// ˫˳(8)
			else if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 2, 0, 7 ) )
			{
				ccTemp = ctevent;
			}

			// ��˳���ĵ�
			else if ( rcVec[0].rc <= RA &&
				( FindLinkTemp(rcVec, 3, 0, 3 ) && 
				rcVec[4].iNum == 1 && rcVec[5].iNum == 1 &&
				rcVec[6].iNum == 1 && rcVec[7].iNum == 1 ) ||
				( FindLinkTemp(rcVec, 3, 1, 4 ) && 
				rcVec[0].iNum == 1 && rcVec[5].iNum == 1 &&
				rcVec[6].iNum == 1 && rcVec[7].iNum == 1 ) ||
				( FindLinkTemp(rcVec, 3, 2, 5 ) && 
				rcVec[0].iNum == 1 && rcVec[1].iNum == 1 &&
				rcVec[6].iNum == 1 && rcVec[7].iNum == 1 ) ||
				( FindLinkTemp(rcVec, 3, 3, 6 ) && 
				rcVec[0].iNum == 1 && rcVec[1].iNum == 1 &&
				rcVec[2].iNum == 1 && rcVec[7].iNum == 1 ) ||
				( FindLinkTemp(rcVec, 3, 4, 7 ) && 
				rcVec[0].iNum == 1 && rcVec[1].iNum == 1 &&
				rcVec[2].iNum == 1 && rcVec[3].iNum == 1 ) )
			{
				ccTemp = cthfone;
			}

			// ��˳���Ķ�
			else if ( rcVec[0].rc <= RA &&
				( FindLinkTemp(rcVec, 3, 0, 3 ) && 
				rcVec[4].iNum == 2 && rcVec[5].iNum == 2 &&
				rcVec[6].iNum == 2 && rcVec[7].iNum == 2 ) ||
				( FindLinkTemp(rcVec, 3, 1, 4 ) && 
				rcVec[0].iNum == 2 && rcVec[5].iNum == 2 &&
				rcVec[6].iNum == 2 && rcVec[7].iNum == 2 ) ||
				( FindLinkTemp(rcVec, 3, 2, 5 ) && 
				rcVec[0].iNum == 2 && rcVec[1].iNum == 2 &&
				rcVec[6].iNum == 2 && rcVec[7].iNum == 2 ) ||
				( FindLinkTemp(rcVec, 3, 3, 6 ) && 
				rcVec[0].iNum == 2 && rcVec[1].iNum == 2 &&
				rcVec[2].iNum == 2 && rcVec[7].iNum == 2 ) ||
				( FindLinkTemp(rcVec, 3, 4, 7 ) && 
				rcVec[0].iNum == 2 && rcVec[1].iNum == 2 &&
				rcVec[2].iNum == 2 && rcVec[3].iNum == 2 ) )
			{
				ccTemp = cthftwo;
			}
		}
		break;
	case 9:
		{
			// ��˳(9)
			if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 1, 0, 8 ) )
			{
				ccTemp = clink9;
			}

			// ˫˳(8)
			else if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 2, 0, 8 ) )
			{
				ccTemp = ctnine;
			}
		}
		break;
	case 10:
		{
			// ��˳(10)
			if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 1, 0, 9 ) )
			{
				ccTemp = clink10;
			}

			// ˫˳(10)
			else if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 2, 0, 9 ) )
			{
				ccTemp = ctten;
			}
		}
		break;
	case 11:
		{
			// ��˳(11)
			if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 1, 0, 10 ) )
			{
				ccTemp = clink11;
			}
		}
		break;
	case 12:
		{
			// ��˳(12)
			if ( rcVec[0].rc <= RA && FindLinkTemp(rcVec, 1, 0, 11 ) )
			{
				ccTemp = clink12;
			}
		}
		break;
	default:
		break;
	}

	return ccTemp;
}

//
// ��˳
//
bool Roles::FindLinkTemp(const RCVector& rcVec, const int iOr, const int iF, const int iL )
{
	bool bTemp = true;

	for ( int i = iF; bTemp && i < iL; ++i )
	{
		if ( rcVec[i].rc - rcVec[i + 1].rc != 1 ||
			rcVec[i].iNum != iOr || 
			rcVec[i + 1].iNum != iOr)
		{
			bTemp = false;
		}
	}

	return bTemp;
}



//
// �Ʒ���
//
CARDCLASS Roles::SelectClass( void )
{
	CARDCLASS ccTemp = cerror;

	switch ( m_RCards.size() )
	{
	case 1:												// һ����
		{
			switch ( m_RCards[0].iNum )
			{
			case 1:										// ����
				{
					ccTemp = cone;
				}
				break;
			case 2:										// ����
				{
					ccTemp = ctwo;
				}
				break;
			case 3:										// ����
				{
					ccTemp = cthree;
				}
				break;
			case 4:										// ը��
				{
					ccTemp = cfour;
				}
				break;
			default:
				break;
			}
		}
		break;
	case 2:
		{
			// ��˳
			if ( m_RCards[0].rc <= RA && 
				 FindLink( 3, 0, 1 ) )
			{
				ccTemp = ctth;
			}

			// ����һ��
			else if ( m_RCards[0].iNum == 3 && m_RCards[1].iNum == 1 ||
				 m_RCards[1].iNum == 3 && m_RCards[0].iNum == 1 )
			{
				ccTemp = cthone;
			}

			// ����һ��
			else if ( m_RCards[0].iNum == 3 && m_RCards[1].iNum == 2 ||
					  m_RCards[1].iNum == 3 && m_RCards[0].iNum == 2 )
			{
				ccTemp = cthtwo;
			}

//			// �Ĵ�һ��
//			else if ( m_RCards[0].iNum == 4 && m_RCards[1].iNum == 1 ||
//					  m_RCards[1].iNum == 4 && m_RCards[0].iNum == 1 )
//			{
//				ccTemp = cfone;
//			}
//
//			// �Ĵ�һ��
//			else if ( m_RCards[0].iNum == 4 && m_RCards[1].iNum == 2 ||
//					  m_RCards[1].iNum == 4 && m_RCards[0].iNum == 2 )
//			{
//				ccTemp = cftwo;
//			}
			
			// ˫��
			else if ( ( m_RCards[0].rc == RWS && m_RCards[1].rc == RWB ) ||
					  ( m_RCards[1].rc == RWS && m_RCards[0].rc == RWB ) )
			{
				ccTemp = chigh;
			}
		}
		break;
	case 3:
		{
			// ˫˳(3)	
			if ( m_RCards[0].rc <= RA && FindLink( 2, 0, 2 ) )
			{
				ccTemp = cttth;
			}

			// ��˳(3)	
			else if ( m_RCards[0].rc <= RA && FindLink( 3, 0, 2 ) )
			{
				ccTemp = cthth;
			}

			// �Ĵ�����
			else if ( m_RCards[0].iNum == 4 && m_RCards[1].iNum == 1 &&
					  m_RCards[2].iNum == 1 ||
				      m_RCards[1].iNum == 4 && m_RCards[0].iNum == 1 &&
					  m_RCards[2].iNum == 1 ||
					  m_RCards[2].iNum == 4 && m_RCards[0].iNum == 1 &&
					  m_RCards[1].iNum == 1 )
			{
				ccTemp = cftone;
			}

			// �Ĵ�����
			else if ( m_RCards[0].iNum == 4 && m_RCards[1].iNum == 2 &&
					  m_RCards[2].iNum == 2 ||
					  m_RCards[1].iNum == 4 && m_RCards[0].iNum == 2 &&
					  m_RCards[2].iNum == 2 ||
					  m_RCards[2].iNum == 4 && m_RCards[0].iNum == 2 &&
					  m_RCards[1].iNum == 2 )
			{
				ccTemp = cfttwo;
			}
		}
		break;
	case 4:
		{
			// ˫˳(4)
			if ( m_RCards[0].rc <= RA && FindLink( 2, 0, 3 ) )
			{
				ccTemp = ctfour;
			}

			// ��˳(4)
			else if ( m_RCards[0].rc <= RA && FindLink( 3, 0, 3 ) )
			{
				ccTemp = cthfour;
			}

			// ��˳������
			else if ( m_RCards[0].rc <= RA &&
				    ( FindLink( 3, 0, 1 ) && 
					  m_RCards[2].iNum == 1 && m_RCards[3].iNum == 1 ) ||
					( FindLink( 3, 1, 2 ) && 
					  m_RCards[0].iNum == 1 && m_RCards[3].iNum == 1 ) ||
					( FindLink( 3, 2, 3 ) && 
					  m_RCards[0].iNum == 1 && m_RCards[1].iNum == 1 ) )
			{
				ccTemp = ctthone;
			}

			// ��˳������
			else if ( m_RCards[0].rc <= RA &&
				    ( FindLink( 3, 0, 1 ) && 
					  m_RCards[2].iNum == 2 && m_RCards[3].iNum == 2 ) ||
					( FindLink( 3, 1, 2 ) && 
					  m_RCards[0].iNum == 2 && m_RCards[3].iNum == 2 ) ||
					( FindLink( 3, 2, 3 ) && 
					  m_RCards[0].iNum == 2 && m_RCards[1].iNum == 2 ) )
			{
				ccTemp = ctthtwo;
			}

		}
		break;
	case 5:
		{
			// ��˳(5)
			if ( m_RCards[0].rc <= RA &&
				 FindLink( 1, 0, 4 ) )
			{
				ccTemp = clink5;
			}

			// ˫˳(5)
			else if ( m_RCards[0].rc <= RA && FindLink( 2, 0, 4 ) )
			{
				ccTemp = ctfive;
			}

			// ��˳(5)
			else if ( m_RCards[0].rc <= RA && FindLink( 3, 0, 4 ) )
			{
				ccTemp = cthfive;
			}
		}
		break;
	case 6:
		{
			// ��˳(6)
			if ( m_RCards[0].rc <= RA && FindLink( 1, 0, 5 ) )
			{
				ccTemp = clink6;
			}

			// ˫˳(6)
			else if ( m_RCards[0].rc <= RA && FindLink( 2, 0, 5 ) )
			{
				ccTemp = ctsix;
			}

			// ��˳(6)
			else if ( m_RCards[0].rc <= RA && FindLink( 3, 0, 5 ) )
			{
				ccTemp = cthsix;
			}

			// ��˳������
			else if ( m_RCards[0].rc <= RA &&
				    ( FindLink( 3, 0, 2 ) && 
					  m_RCards[3].iNum == 1 && m_RCards[4].iNum == 1 &&
					  m_RCards[5].iNum == 1 ) ||
					( FindLink( 3, 1, 3 ) && 
					  m_RCards[0].iNum == 1 && m_RCards[4].iNum == 1 &&
					  m_RCards[5].iNum == 1 ) ||
					( FindLink( 3, 2, 4 ) && 
					  m_RCards[0].iNum == 1 && m_RCards[1].iNum == 1 &&
					  m_RCards[5].iNum == 1 ) ||
					( FindLink( 3, 3, 5 ) && 
					  m_RCards[0].iNum == 1 && m_RCards[1].iNum == 1 &&
					  m_RCards[2].iNum == 1 ) )
			{
				ccTemp = cththone;
			}

			// ��˳������
			else if ( m_RCards[0].rc <= RA &&
				    ( FindLink( 3, 0, 2 ) && 
					  m_RCards[3].iNum == 2 && m_RCards[4].iNum == 2 &&
					  m_RCards[5].iNum == 2 ) ||
					( FindLink( 3, 1, 3 ) && 
					  m_RCards[0].iNum == 2 && m_RCards[4].iNum == 2 &&
					  m_RCards[5].iNum == 2 ) ||
					( FindLink( 3, 2, 4 ) && 
					  m_RCards[0].iNum == 2 && m_RCards[1].iNum == 2 &&
					  m_RCards[5].iNum == 2 ) ||
					( FindLink( 3, 3, 5 ) && 
					  m_RCards[0].iNum == 2 && m_RCards[1].iNum == 2 &&
					  m_RCards[2].iNum == 2 ) )
			{
				ccTemp = cththtwo;
			}
		}
		break;
	case 7:
		{
			// ��˳(7)
			if ( m_RCards[0].rc <= RA && FindLink( 1, 0, 6 ) )
			{
				ccTemp = clink7;
			}

			// ˫˳(7)
			else if ( m_RCards[0].rc <= RA && FindLink( 2, 0, 6 ) )
			{
				ccTemp = ctseven;
			}
		}
		break;
	case 8:
		{
			// ��˳(8)
			if ( m_RCards[0].rc <= RA && FindLink( 1, 0, 7 ) )
			{
				ccTemp = clink8;
			}

			// ˫˳(8)
			else if ( m_RCards[0].rc <= RA && FindLink( 2, 0, 7 ) )
			{
				ccTemp = ctevent;
			}

			// ��˳���ĵ�
			else if ( m_RCards[0].rc <= RA &&
				    ( FindLink( 3, 0, 3 ) && 
					  m_RCards[4].iNum == 1 && m_RCards[5].iNum == 1 &&
					  m_RCards[6].iNum == 1 && m_RCards[7].iNum == 1 ) ||
					( FindLink( 3, 1, 4 ) && 
					  m_RCards[0].iNum == 1 && m_RCards[5].iNum == 1 &&
					  m_RCards[6].iNum == 1 && m_RCards[7].iNum == 1 ) ||
					( FindLink( 3, 2, 5 ) && 
					  m_RCards[0].iNum == 1 && m_RCards[1].iNum == 1 &&
					  m_RCards[6].iNum == 1 && m_RCards[7].iNum == 1 ) ||
					( FindLink( 3, 3, 6 ) && 
					  m_RCards[0].iNum == 1 && m_RCards[1].iNum == 1 &&
					  m_RCards[2].iNum == 1 && m_RCards[7].iNum == 1 ) ||
				    ( FindLink( 3, 4, 7 ) && 
					  m_RCards[0].iNum == 1 && m_RCards[1].iNum == 1 &&
					  m_RCards[2].iNum == 1 && m_RCards[3].iNum == 1 ) )
			{
				ccTemp = cthfone;
			}

			// ��˳���Ķ�
			else if ( m_RCards[0].rc <= RA &&
				    ( FindLink( 3, 0, 3 ) && 
					  m_RCards[4].iNum == 2 && m_RCards[5].iNum == 2 &&
					  m_RCards[6].iNum == 2 && m_RCards[7].iNum == 2 ) ||
					( FindLink( 3, 1, 4 ) && 
					  m_RCards[0].iNum == 2 && m_RCards[5].iNum == 2 &&
					  m_RCards[6].iNum == 2 && m_RCards[7].iNum == 2 ) ||
					( FindLink( 3, 2, 5 ) && 
					  m_RCards[0].iNum == 2 && m_RCards[1].iNum == 2 &&
					  m_RCards[6].iNum == 2 && m_RCards[7].iNum == 2 ) ||
					( FindLink( 3, 3, 6 ) && 
					  m_RCards[0].iNum == 2 && m_RCards[1].iNum == 2 &&
					  m_RCards[2].iNum == 2 && m_RCards[7].iNum == 2 ) ||
				    ( FindLink( 3, 4, 7 ) && 
					  m_RCards[0].iNum == 2 && m_RCards[1].iNum == 2 &&
					  m_RCards[2].iNum == 2 && m_RCards[3].iNum == 2 ) )
			{
				ccTemp = cthftwo;
			}
		}
		break;
	case 9:
		{
			// ��˳(9)
			if ( m_RCards[0].rc <= RA && FindLink( 1, 0, 8 ) )
			{
				ccTemp = clink9;
			}

			// ˫˳(8)
			else if ( m_RCards[0].rc <= RA && FindLink( 2, 0, 8 ) )
			{
				ccTemp = ctnine;
			}
		}
		break;
	case 10:
		{
			// ��˳(10)
			if ( m_RCards[0].rc <= RA && FindLink( 1, 0, 9 ) )
			{
				ccTemp = clink10;
			}

			// ˫˳(10)
			else if ( m_RCards[0].rc <= RA && FindLink( 2, 0, 9 ) )
			{
				ccTemp = ctten;
			}
		}
		break;
	case 11:
		{
			// ��˳(11)
			if ( m_RCards[0].rc <= RA && FindLink( 1, 0, 10 ) )
			{
				ccTemp = clink11;
			}
		}
		break;
	case 12:
		{
			// ��˳(12)
			if ( m_RCards[0].rc <= RA && FindLink( 1, 0, 11 ) )
			{
				ccTemp = clink12;
			}
		}
		break;
	default:
		break;
	}

	return ccTemp;
}

//
// ��˳
//
bool Roles::FindLink( const int iOr, const int iF, const int iL )
{
	bool bTemp = true;

	for ( int i = iF; bTemp && i < iL; ++i )
	{
		if ( m_RCards[i].rc - m_RCards[i + 1].rc != 1 ||
			m_RCards[i].iNum != iOr || 
			m_RCards[i + 1].iNum != iOr)
		{
			bTemp = false;
		}
	}
	
	return bTemp;
}


//
// ȡ��ѡ����,���浽m_ivTemp
//
bool Roles::GetChoose( void )
{
	// ���m_ivTemp
	if ( !m_ivTemp.empty() )
	{
		m_ivTemp.erase( m_ivTemp.begin(), m_ivTemp.end() );
	}

	for ( int i = 0; i < m_Cards.size(); ++i )
	{
		if ( m_Cards[i].cs == yes )						// ѡ����ȡ��
		{
			m_ivTemp.push_back( m_Cards[i].iCard );		// ����ԭʼ����
		}
	}
	
	if ( m_ivTemp.empty() )
	{
		return false;
	}

	return true;
}

//
// ���ѡ�е���
//
void Roles::DeleteChoose( void )
{
	// ���ѡ��״̬
	for ( int i = 0; i < m_Cards.size(); ++i )
	{
		m_Cards[i].cs = no;
	}
}



//
// ɾ��
//
void Roles::DeleteCard( void )
{
	for ( int i = 0; i < m_Cards.size(); ++i )
	{
		if ( m_Cards[i].cs == yes )							// ѡ�е�����ɾ��
		{
			m_Cards.erase( m_Cards.begin() + i-- );
		}
	}
}

//
// �Ƚ�
//
bool Roles::CompareCard( RCVector rcIn, CARDCLASS cc ) const 
{
	bool bTemp = false;

	if ( cc == ctrue )					// �׳�
	{
		return true;
	}

	if ( m_ccTemp == chigh )
	{
		return true;					// 2KING
	}

	if ( cc == m_ccTemp )				// �����������뵱ǰ�����
	{
		// �ж������ԱȽ������С
		switch ( cc )
		{
		case cone:									// ����
		case ctwo:									// ˫��	
		case cthree:								// ����	
		case cfour:									// ը��	
		case ctth:									// ��˳
		case cttth:									// ˫˳(3)
		case cthth:									// ��˳(3)
		case ctfour:								// ˫˳(4)
		case cthfour:								// ��˳(4)
		case clink5:								// ��˳(5)	
		case clink6:								// ��˳(6)	
		case clink7:								// ��˳(7)	
		case clink8:								// ��˳(8)	
		case clink9:								// ��˳(9)	
		case clink10:								// ��˳(10)	
		case clink11:								// ��˳(11)	
		case clink12:								// ��˳(12)	
		case ctfive:								// ˫˳(5)	
		case cthfive:								// ��˳(5)	
		case ctsix:									// ˫˳(6)	
		case cthsix:								// ��˳(6)
		case ctseven:								// ˫˳(7)	
		case ctevent:								// ˫˳(8)	
		case ctnine:								// ˫˳(9)	
		case ctten:									// ˫˳(10)	
			{
				if ( rcIn[0].rc < m_RCards[0].rc )	// �Ƚ���󼴿�
				{
					bTemp = true;
				}
			}
			break;
		case cthone:								// ����һ��
		case cthtwo:								// ����һ��
		case ctthone:								// ��˳������
		case ctthtwo:								// ��˳������
		case cththone:								// ��˳������
		case cththtwo:								// ��˳������
		case cthfone:								// ��˳���ĵ�
		case cthftwo:								// ��˳���Ķ�
			{
				int i = 0;
				while ( i < rcIn.size() )
				{
					if ( rcIn[i].iNum == 3 )		// ��������
					{
						break;
					}
					++i;
				}
				int j = 0;
				while ( j < m_RCards.size() )
				{
					if ( m_RCards[j].iNum == 3 )		// ��������
					{
						break;
					}
					++j;
				}
				if ( rcIn[i].rc < m_RCards[j].rc )
				{
					bTemp = true;
				}
			}
			break;
//		case cfone:									// �Ĵ�һ��	
//		case cftwo:									// �Ĵ�һ��
		case cftone:								// �Ĵ�����	
		case cfttwo:								// �Ĵ�����
			{
				int i = 0;
				while ( i < rcIn.size() )
				{
					if ( rcIn[i].iNum == 4 )		// ��������
					{
						break;
					}
					++i;
				}
				int j = 0;
				while ( j < m_RCards.size() )
				{
					if ( m_RCards[j].iNum == 4 )		// ��������
					{
						break;
					}
					++j;
				}
				if ( rcIn[i].rc < m_RCards[j].rc )
				{
					bTemp = true;
				}
			}
			break;
		default:
			break;
		}
	}
	else										// �����������뵱ǰ�Ʋ���
	{
		if ( m_ccTemp == cfour )		// ը��
		{
			if ( cc != chigh 					// ����2KING
//				 cc != cfone &&					// �Ĵ�һ��	
//				 cc != cftwo &&					// �Ĵ�һ��	
//				 cc != cftone &&				// �Ĵ�����	
//				 cc != cfttwo					// �Ĵ�����
				)
			{
				bTemp = true;
			}
		}
	}

	return bTemp;
}


//
// public
// ---------------------
// ��Ӧ������ı��Ƶ�ѡ��״̬
//
void Roles::ChooseCards( const int iNum )
{
	if ( !m_Cards.empty() && iNum >= 0 && iNum < m_Cards.size() )
	{
		if ( m_Cards[iNum].cs == yes )					// ����״̬Ϊѡ��
		{
			m_Cards[iNum].cs = no;
		}
		else
		{
			m_Cards[iNum].cs = yes;
		}
	}
}

//
// ����
//
bool Roles::PushCard( const RCVector rcIn, const CARDCLASS cc )
{
	if ( !GetChoose() )									// ȡѡ��������
	{
		return false;
	}

	ChangeCard();										// ת����JQK��ʽ

	// ����ѡ�������͵�m_ccTemp
	if ( ( m_ccTemp = SelectClass() ) == cerror )		// �Ƿ���
	{
		DeleteChoose();									// ���ѡ�������Լ�״̬
		return false;									// ������
	}

	if ( !CompareCard( rcIn, cc ) )						// �Ƚ�ѡ�������ϰ������С
	{
		DeleteChoose();
		return false;									// ������
	}

	DeleteCard();										// ���ƺ�ɾ���ѳ���
	return true;										// ��Ӧ����
}