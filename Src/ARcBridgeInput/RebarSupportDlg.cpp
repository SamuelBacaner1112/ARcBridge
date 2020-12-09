// RebarSupportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "RebarSupportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRebarSupportDlg dialog


CRebarSupportDlg::CRebarSupportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRebarSupportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRebarSupportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pStd = NULL;
	m_pBri = NULL;
}

CRebarSupportDlg::CRebarSupportDlg(CRcBridgeRebar *pBri, CARcBridgeDataStd *pStd, CWnd* pParent /*=NULL*/)
	: CDialog(CRebarSupportDlg::IDD, pParent)
{
	m_pStd	= pStd;
	m_pBri	= pBri;
}

void CRebarSupportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRebarSupportDlg)
	DDX_Control(pDX, IDC_RECOMMOND, m_btnRecommond);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_CHECK_HUNCH, m_checkHunch);
	DDX_Control(pDX, IDC_CHECK_DAN, m_checkDan);
	DDX_Control(pDX, IDC_CHECK_23, m_check23);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRebarSupportDlg, CDialog)
	//{{AFX_MSG_MAP(CRebarSupportDlg)
	ON_BN_CLICKED(IDC_RECOMMOND, OnRecommond)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRebarSupportDlg message handlers

void CRebarSupportDlg::SetGridData()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 3, 1, 1);
	m_Grid.SetCellSize(20, -1);

	if(!m_pBri) return;

	long row = 0, col = 0;
	CString str = _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row, col+1, "����");
	m_Grid.AddCell(row++, col+2, "CTC");

	CPlaceRebarByBlock *pPlace	= &m_pBri->m_placeByBlock_SupportRebar_UpperSlab[iUPPER][0];
	m_Grid.AddCell(row, col, "��ν�����");
	m_Grid.AddCell(row++, col+1, &pPlace->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartUpperSlab));

	if(m_pBri->IsBoxType())
	{
		pPlace	= &m_pBri->m_placeByBlock_SupportRebar_LowerSlab[iUPPER][0];
		m_Grid.AddCell(row, col, "�Ϻν�����");
		m_Grid.AddCell(row++, col+1, &pPlace->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
		m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartLowerSlab));
	}

	pPlace	= &m_pBri->m_placeByBlock_SupportRebar_OutWall[iSTT][iINNER][0];
	m_Grid.AddCell(row, col, "������ü");
	m_Grid.AddCell(row++, col+1, &pPlace->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartOutWallStt));

	long nCountInWall	= m_pBri->GetCountInWall();
	for (HGINT32 ix = 0; ix < nCountInWall; ++ix)
	{
		CWallApp *pWall	= m_pBri->GetInWall(ix);
		if(pWall == NULL)
			continue;

		pPlace	= &pWall->m_placeByBlock_SupportRebar[iLEFT][0];

		CString str;
		str.Format("�߰���ü%d", ix+1);
		m_Grid.AddCell(row, col, str);
		m_Grid.AddCell(row++, col+1, &pPlace->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
		m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartInWall));
	}

	pPlace	= &m_pBri->m_placeByBlock_SupportRebar_OutWall[iEND][iINNER][0];
	m_Grid.AddCell(row, col, "������ü");
	m_Grid.AddCell(row++, col+1, &pPlace->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartOutWallEnd));

	long nCountFooting	= m_pBri->GetCountFooting();
	if(nCountFooting && !m_pBri->IsBoxType())
	{
		CFootingApp *pFootingStt	= m_pBri->GetFooting(0);
		if(pFootingStt)
		{
			pPlace	= &pFootingStt->m_placeByBlock_Support_Front[iUPPER][0];
			m_Grid.AddCell(row, col, "����������");
			m_Grid.AddCell(row++, col+1, &pPlace->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
			m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartFootStt));
		}

		for (HGINT32 ix = 1; ix < nCountFooting - 1; ++ix)
		{
			CFootingApp *pFooting	= m_pBri->GetFooting(ix);
			if (pFooting == NULL)
				continue;

			pPlace	= &pFooting->m_placeByBlock_Support_Front[iUPPER][0];
			
			CString str;
			str.Format("�߰���������%d", ix);
			m_Grid.AddCell(row, col, str);
			m_Grid.AddCell(row++, col+1, &pPlace->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
			m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartInFoot));
		}
		
		CFootingApp *pFootingEnd	= m_pBri->GetFooting(nCountFooting - 1);
		if (pFootingEnd && pFootingEnd != pFootingStt)
		{
			pPlace	= &pFootingEnd->m_placeByBlock_Support_Front[iUPPER][0];

			m_Grid.AddCell(row, col, "����������");
			m_Grid.AddCell(row++, col+1, &pPlace->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
			m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartFootEnd));
		}

		if (pFootingStt)
		{
			pPlace	= &pFootingStt->m_placeByBlock_Side_Out;
			m_Grid.AddCell(row, col, "����(����)");
			m_Grid.AddCell(row, col+1, &pPlace->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
			m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartFootStt));
			m_Grid.AddCell(row++, col+2, "", nFormat, CELL_READONLY);
		}
	}

	HGINT32 icRibAndBracket = 0;
	if(m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		pPlace	= &m_pBri->m_placeByBlock_SupportRebar_ArchRib[iUPPER][0][iLEFT];
		m_Grid.AddCell(row, col, "������");
		m_Grid.AddCell(row, col+1, &pPlace->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
		m_Grid.AddCell(row++, col+2, "", nFormat, CELL_READONLY);

		pPlace	= &m_pBri->m_placeByBlock_SupportRebar_ArchRib[iLOWER][0][iLEFT];
		m_Grid.AddCell(row, col, "�����ϸ�");
		m_Grid.AddCell(row, col+1, &pPlace->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
		m_Grid.AddCell(row++, col+2, "", nFormat, CELL_READONLY);

		icRibAndBracket += 2;
	}

	if(m_pBri->IsOutWall(TRUE))
	{
		m_Grid.AddCell(row, col, "���������");
		m_Grid.AddCell(row, col+1, &m_pBri->m_dDiaRebarSupport_Bracket[iSTT], nFormat, nLine, "", CELL_TYPE_DIA);
		m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartUpperSlab));
		m_Grid.AddCell(row++, col+2, "", nFormat, CELL_READONLY);

		++icRibAndBracket;
	}
	
	if(m_pBri->IsOutWall(FALSE))
	{
		m_Grid.AddCell(row, col, "���������");
		m_Grid.AddCell(row, col+1, &m_pBri->m_dDiaRebarSupport_Bracket[iEND], nFormat, nLine, "", CELL_TYPE_DIA);
		m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartUpperSlab));
		m_Grid.AddCell(row++, col+2, "", nFormat, CELL_READONLY);

		++icRibAndBracket;
	}	

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col+1);
	m_Grid.SetColumnWidth(0, 130);
	m_Grid.SetColumnWidth(1, 60);

	for (HGINT32 ix = 1; ix < row-icRibAndBracket; ++ix)
	{
		m_Grid.SetCellType(ix, 2, CT_COMBO);
	}
}

void CRebarSupportDlg::SetDataInit()
{
	if(!m_pBri) return;

	long row = 1, col = 2;

	CString str;
	str.Format("%.0f", m_pBri->m_dCTC_SupportRebar[iUPPER]);
	m_Grid.SetTextMatrix(row++, col, str);

	if(m_pBri->IsBoxType())
	{
		str.Format("%.0f", m_pBri->m_dCTC_SupportRebar[iLOWER]);
		m_Grid.SetTextMatrix(row++, col, str);
	}

	str.Format("%.0f", m_pBri->m_dCTC_SupportRebarOutWall[iSTT]);
	m_Grid.SetTextMatrix(row++, col, str);

	long nCountInWall	= m_pBri->GetCountInWall();
	for (HGINT32 ix = 0; ix < nCountInWall; ++ix)
	{
		CWallApp *pWall	= m_pBri->GetInWall(ix);
		if(pWall == NULL)
			continue;

		str.Format("%.0f", pWall->m_dCTC_SupportRebar);
		m_Grid.SetTextMatrix(row++, col, str);
	}

	str.Format("%.0f", m_pBri->m_dCTC_SupportRebarOutWall[iEND]);
	m_Grid.SetTextMatrix(row++, col, str);

	long nCountFooting	= m_pBri->GetCountFooting();
	for (HGINT32 ix = 0; ix < nCountFooting; ++ix)
	{
		CFootingApp *pFooting	= m_pBri->GetFooting(0);
		if (pFooting == NULL)
			continue;

		str.Format("%.0f", pFooting->m_dCTC_SupportRebar);
		m_Grid.SetTextMatrix(row++, col, str);
	}

	m_Grid.UpdateData(FALSE);

	m_checkDan.SetCheck(m_pBri->m_bPlacingSupportAtDanBuRebar);
	m_check23.SetCheck(m_pBri->m_bPlacing23DanByStandardOffset);
	m_checkHunch.SetCheck(m_pBri->m_bPlacingSupportAtHunchRebar);
}

void CRebarSupportDlg::SetDataSave()
{
	if(!m_pBri) return;

	m_Grid.UpdateData(TRUE);

	long row = 1, col = 2;
	m_pBri->m_dCTC_SupportRebar[iUPPER] = m_Grid.GetTextMatrixDouble(row++, col);
	if(m_pBri->IsBoxType())
	{
		m_pBri->m_dCTC_SupportRebar[iLOWER] = m_Grid.GetTextMatrixDouble(row++, col);
	}

	m_pBri->m_dCTC_SupportRebarOutWall[iSTT] = m_Grid.GetTextMatrixDouble(row++, col);

	long nCountInWall	= m_pBri->GetCountInWall();
	for (HGINT32 ix = 0; ix < nCountInWall; ++ix)
	{
		CWallApp *pWall	= m_pBri->GetInWall(ix);
		if(pWall == NULL)
			continue;

		pWall->m_dCTC_SupportRebar = m_Grid.GetTextMatrixDouble(row++, col);
	}

	m_pBri->m_dCTC_SupportRebarOutWall[iEND] = m_Grid.GetTextMatrixDouble(row++, col);

	long nCountFooting	= m_pBri->GetCountFooting();
	for (HGINT32 ix = 0; ix < nCountFooting; ++ix)
	{
		CFootingApp *pFooting	= m_pBri->GetFooting(0);
		if (pFooting == NULL)
			continue;

		pFooting->m_dCTC_SupportRebar = m_Grid.GetTextMatrixDouble(row++, col);
	}
	
	m_pBri->m_bPlacingSupportAtDanBuRebar	= m_checkDan.GetCheck();
	m_pBri->m_bPlacing23DanByStandardOffset	= m_check23.GetCheck();
	m_pBri->m_bPlacingSupportAtHunchRebar	= m_checkHunch.GetCheck();

	SyncData();
}

void CRebarSupportDlg::SetControl()
{

}

BOOL CRebarSupportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// ��ư ��Ÿ�� ����

	SetControl();
	InitGridManager();
	SetGridData();
	SetDataInit();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRebarSupportDlg::OnOK() 
{
	// TODO: Add extra validation here
	SetDataSave();
	
	CDialog::OnOK();
}

void CRebarSupportDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
		
	CDialog::OnCancel();
}

void CRebarSupportDlg::OnRecommond() 
{
	HGDOUBLE dDia = 19;
	HGINT32 dCtc = 300;

	m_pBri->m_dCTC_SupportRebar[iUPPER] = dCtc;
	m_pBri->m_dCTC_SupportRebar[iLOWER] = dCtc;

	m_pBri->m_dCTC_SupportRebarOutWall[iSTT] = dCtc;
	m_pBri->m_dCTC_SupportRebarOutWall[iEND] = dCtc;


	long dan(0), upper(0), stt(0), inner(0), left(0);
	for(upper = 0; upper < 2; upper++)
	{
		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			CPlaceRebarByBlock *pPlace	= &m_pBri->m_placeByBlock_SupportRebar_UpperSlab[upper][dan];
			pPlace->m_dDia				= dDia;
		}
	}

	// �Ϻν�����
	for(upper = 0; upper < 2; upper++)
	{
		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			CPlaceRebarByBlock *pPlace	= &m_pBri->m_placeByBlock_SupportRebar_LowerSlab[upper][dan];
			pPlace->m_dDia				= dDia;
		}
	}

	// ������ü
	for(stt = 0; stt < 2; stt++)
	{
		for(inner = 0; inner < 2; inner++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				CPlaceRebarByBlock *pPlace	= &m_pBri->m_placeByBlock_SupportRebar_OutWall[stt][inner][dan];
				pPlace->m_dDia				= dDia;
			}
		}
	}


	// ������ü��
	long nCountInWall	= m_pBri->GetCountInWall();
	for (HGINT32 ix = 0; ix < nCountInWall; ++ix)
	{
		CWallApp *pWall	= m_pBri->GetInWall(ix);
		if(pWall)
		{
			pWall->m_dCTC_SupportRebar = dCtc;

			for(left = 0; left < 2; left++)
			{
				for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					CPlaceRebarByBlock *pPlace	= &pWall->m_placeByBlock_SupportRebar[left][dan];
					pPlace->m_dDia				= dDia;
				}
			}
		}
	}

	// ���ʵ�
	long nCountFooting	= m_pBri->GetCountFooting();
	for (HGINT32 ix = 0; ix < nCountFooting; ++ix)
	{
		CFootingApp *pFooting	= m_pBri->GetFooting(ix);
		if (pFooting == NULL)
			continue;

		pFooting->m_dCTC_SupportRebar = dCtc;

		for(upper = 0; upper < 2; upper++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				CPlaceRebarByBlock *pPlace	= &pFooting->m_placeByBlock_Support_Front[upper][dan];
				pPlace->m_dDia				= dDia;
			}
		}
	}

	if(nCountFooting > 1)
	{
		CFootingApp *pFooting	= m_pBri->GetFooting(0);
		if(pFooting)
		{
			// ���� ö��
			for(long footing = 0; footing < nCountFooting; footing++)
			{
				CFootingApp *pFooting	= m_pBri->GetFooting(footing);
				CPlaceRebarByBlock *pPlace	= &pFooting->m_placeByBlock_Side_Out;
				pPlace->m_dDia				= dDia;
			}
		}
	}

	// ����
	for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		for(left = 0; left < 2; left++)
		{
			m_pBri->m_placeByBlock_SupportRebar_ArchRib[iUPPER][dan][left].m_dDia	= dDia;
			m_pBri->m_placeByBlock_SupportRebar_ArchRib[iLOWER][dan][left].m_dDia	= dDia;
		}
	}

	m_pBri->m_bPlacingSupportAtDanBuRebar	= TRUE;
	m_pBri->m_bPlacing23DanByStandardOffset	= FALSE;
	m_pBri->m_bPlacingSupportAtHunchRebar	= TRUE;

	SetGridData();
	SetDataInit();

	UpdateData(FALSE);
}

void CRebarSupportDlg::InitGridManager()
{
	m_Grid.InitGrid(m_pStd->m_Fy);
}

BOOL CRebarSupportDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Grid.ViewGridCellForCombo(pMsg);

	if(pMsg->hwnd==m_Grid.GetSafeHwnd() && pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		if(next.col==2)
		{
			CStringArray strArr;
			CString str	= _T("");
			for(long ctc = 0; ctc < MAX_REBAR_CTC; ctc++)
			{
				CHgBaseConcStd baseConcStd;
				str.Format("%.0f", baseConcStd.GetValueRebarCTC(ctc));
				strArr.Add(str);
			}

			m_Grid.SetComboString(strArr);
		}
	}

	
	return CDialog::PreTranslateMessage(pMsg);
}

// ��� ������ ���ؼ� ������ �Է¹��� �ʰ�, ��ν����� ���, ù��° ������ü, ������ü, ù��°���ʸ� �Է��� 
// �ޱ� ������ �����Ҷ� ���� �Է¹��� ���� �κп� ���ؼ� �Էµ� ����Ÿ�� ���� ������ ��� �ȴ�.
void CRebarSupportDlg::SyncData()
{
	if(!m_pBri) return;

	long dan(0), upper(0), stt(0), inner(0), left(0);
	CPlaceRebarByBlock *pPlaceSource	= NULL;

	// �Է��� �޴� �ֵ�.
	pPlaceSource	= &m_pBri->m_placeByBlock_SupportRebar_UpperSlab[iUPPER][0];

	// ��ν�����
	for(upper = 0; upper < 2; upper++)
	{
		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			if(upper == iUPPER && dan == 0) continue;

			CPlaceRebarByBlock *pPlace	= &m_pBri->m_placeByBlock_SupportRebar_UpperSlab[upper][dan];
			pPlace->m_dDia				= pPlaceSource->m_dDia;
		}
	}

	// �Ϻν�����
	pPlaceSource	= &m_pBri->m_placeByBlock_SupportRebar_LowerSlab[iUPPER][0];
	for(upper = 0; upper < 2; upper++)
	{
		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			if(upper == iUPPER && dan == 0) continue;

			CPlaceRebarByBlock *pPlace	= &m_pBri->m_placeByBlock_SupportRebar_LowerSlab[upper][dan];
			pPlace->m_dDia				= pPlaceSource->m_dDia;
		}
	}

	// ������ü
	for(stt = 0; stt < 2; stt++)
	{
		pPlaceSource	= &m_pBri->m_placeByBlock_SupportRebar_OutWall[stt][iINNER][0];
		for(inner = 0; inner < 2; inner++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				if(inner == iINNER && dan == 0) continue;

				CPlaceRebarByBlock *pPlace	= &m_pBri->m_placeByBlock_SupportRebar_OutWall[stt][inner][dan];
				pPlace->m_dDia				= pPlaceSource->m_dDia;
			}
		}
	}
	

	// ������ü��
	long nCountInWall	= m_pBri->GetCountInWall();
	for (HGINT32 ix = 0; ix < nCountInWall; ++ix)
	{
		CWallApp *pWall	= m_pBri->GetInWall(ix);
		if(pWall)
		{
			pPlaceSource	= &pWall->m_placeByBlock_SupportRebar[iLEFT][0];
			for(left = 0; left < 2; left++)
			{
				for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					if(left == iLEFT && dan == 0) continue;

					CPlaceRebarByBlock *pPlace	= &pWall->m_placeByBlock_SupportRebar[left][dan];
					pPlace->m_dDia				= pPlaceSource->m_dDia;
				}
			}
		}
	}

	// ���ʵ�
	long nCountFooting	= m_pBri->GetCountFooting();
	for (HGINT32 ix = 0; ix < nCountFooting; ++ix)
	{
		CFootingApp *pFooting	= m_pBri->GetFooting(ix);
		if (pFooting == NULL)
			continue;

		pPlaceSource	= &pFooting->m_placeByBlock_Support_Front[iUPPER][0];
		for(upper = 0; upper < 2; upper++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				if(upper == iUPPER && dan == 0) continue;
				CPlaceRebarByBlock *pPlace	= &pFooting->m_placeByBlock_Support_Front[upper][dan];
				pPlace->m_dDia				= pPlaceSource->m_dDia;
			}
		}
	}

	if(nCountFooting > 1)
	{
		CFootingApp *pFooting	= m_pBri->GetFooting(0);
		if(pFooting)
		{
			// ���� ö��
			pPlaceSource	= &pFooting->m_placeByBlock_Side_Out;
			for(long footing = 0; footing < nCountFooting; footing++)
			{
				CFootingApp *pFooting	= m_pBri->GetFooting(footing);
				CPlaceRebarByBlock *pPlace	= &pFooting->m_placeByBlock_Side_Out;
				pPlace->m_dDia				= pPlaceSource->m_dDia;
			}
		}
	}

	// ����
	m_pBri->m_placeByBlock_SupportRebar_ArchRib[iUPPER][0][iRIGHT].m_dDia	= m_pBri->m_placeByBlock_SupportRebar_ArchRib[iUPPER][0][iLEFT].m_dDia;
	m_pBri->m_placeByBlock_SupportRebar_ArchRib[iLOWER][0][iRIGHT].m_dDia	= m_pBri->m_placeByBlock_SupportRebar_ArchRib[iLOWER][0][iLEFT].m_dDia;
	for(dan = 1; dan < MAX_REBAR_DAN; dan++)
	{
		for(left = 0; left < 2; left++)
		{
			m_pBri->m_placeByBlock_SupportRebar_ArchRib[iUPPER][dan][left].m_dDia	= m_pBri->m_placeByBlock_SupportRebar_ArchRib[iUPPER][0][left].m_dDia;
			m_pBri->m_placeByBlock_SupportRebar_ArchRib[iLOWER][dan][left].m_dDia	= m_pBri->m_placeByBlock_SupportRebar_ArchRib[iLOWER][0][left].m_dDia;
		}
	}
}
