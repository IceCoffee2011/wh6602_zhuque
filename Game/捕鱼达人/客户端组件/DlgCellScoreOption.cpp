#include "stdafx.h"
#include "resource.h"
#include "DlgCellScoreOption.h"
#include ".\dlgcellscoreoption.h"

IMPLEMENT_DYNAMIC(CDlgCellScoreOption, CSkinDialogEx)
BEGIN_MESSAGE_MAP(CDlgCellScoreOption, CSkinDialogEx)
END_MESSAGE_MAP()

//���캯��
CDlgCellScoreOption::CDlgCellScoreOption(CWnd* pParent): CSkinDialogEx(IDD_CELL_SCORE_OPTION, pParent)
{
	m_lCellScoreSelect=0L;
	m_pCenterView=NULL;
}

//��������
CDlgCellScoreOption::~CDlgCellScoreOption()
{
}

//�ؼ���
void CDlgCellScoreOption::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);	
	DDX_Control(pDX, IDC_CELL_SCORE, m_ComBoxCellScore);
}

//ȷ������
void CDlgCellScoreOption::OnOK()
{
	//���±���
	UpdateData(TRUE);

	//��ȡѡ��
	int nItem=m_ComBoxCellScore.GetCurSel();
	m_lCellScoreSelect=LONG(m_ComBoxCellScore.GetItemData(nItem));
	
	//���ø���
	__super::OnOK();
}

//��ʼ����
BOOL CDlgCellScoreOption::OnInitDialog()
{
	//���ø���
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("���ñ���"));

	//��Ԫ����
	TCHAR szCellScore[128];
	for (int i=0; i<CountArray(m_lCellScore); ++i)
	{
		int nItem=m_ComBoxCellScore.AddString(_itoa(m_lCellScore[i],szCellScore,10));
		if (CB_ERR!=nItem) m_ComBoxCellScore.SetItemData(nItem,m_lCellScore[i]);
	}
	m_ComBoxCellScore.SetCurSel(0);

	//���ñ���
	m_lCellScoreSelect=m_lCellScore[0];

	//���д���
	if (NULL!=m_pCenterView) CenterWindow(m_pCenterView);

	return TRUE;
}
