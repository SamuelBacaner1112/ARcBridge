// RebarDanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "RebarDanDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRebarDanDlg dialog


CRebarDanDlg::CRebarDanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRebarDanDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRebarDanDlg)
	//}}AFX_DATA_INIT
	m_pBri		= NULL;
	m_pStd		= NULL;
	m_nCycle	= -1;
	m_nTab		= 0;
	m_nTypeTab	= 0;
	m_nTabInWall = -1;
	m_nTabLowerSlab = -1;
	m_nTabRib = -1;
	m_nTabFooting = -1;
	m_nTabOutWall = -1;
	m_nTabUpperSlab = -1;
}
CRebarDanDlg::CRebarDanDlg(CRcBridgeRebar *pBri, CARcBridgeDataStd *pStd, long nCycle, CWnd* pParent /*=NULL*/)
	: CDialog(CRebarDanDlg::IDD, pParent)
{
	m_pBri		= pBri;
	m_pStd		= pStd;
	m_nCycle	= nCycle;
	m_nTab		= 0;
	m_nTabOutWall = -1;
	m_nTabLowerSlab = -1;
	m_nTabRib = -1;
	m_nTabFooting = -1;
	m_nTabUpperSlab = -1;
	m_nTabInWall = -1;
	m_nTypeTab = -1;
}

void CRebarDanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRebarDanDlg)
	DDX_Control(pDX, IDC_CHECK_DETAILINPUT_23DAN, m_checkDetailInput23Dan);
	DDX_Control(pDX, IDC_BUTTON_RECOMMAND, m_btnRecommand);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDC_TAB, m_ctlTab);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRebarDanDlg, CDialog)
	//{{AFX_MSG_MAP(CRebarDanDlg)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMAND, OnButtonRecommand)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_BN_CLICKED(IDC_CHECK_DETAILINPUT_23DAN, OnCheckDetailinput23dan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRebarDanDlg message handlers

BOOL CRebarDanDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str = _T("");
	str.Format("��ġ�ܼ� ����");
	this->SetWindowText(str);

	m_ctlTab.RemoveAllTabs();
	m_ctlTab.SendInitialUpdate(TRUE);
	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);



	m_checkDetailInput23Dan.SetCheck(m_pBri->m_bDetailInput23DanMainRebar);
	m_Grid.InitGrid(m_pStd->m_Fy);
	InitTab();
	SetGridData();
	SetDataInit();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRebarDanDlg::OnOK() 
{
	// TODO: Add extra validation here
	SetDataSave();
	
	CDialog::OnOK();
}

void CRebarDanDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CRebarDanDlg::SetGridData()
{
	if(m_nTab == m_nTabUpperSlab) SetGridDataUpperSlab();
	else if(m_nTab == m_nTabLowerSlab) SetGridDataLowerSlab();
	else if(m_nTab == m_nTabOutWall) SetGridDataOutWall();
	else if(m_nTab == m_nTabInWall) SetGridDataInWall();
	else if(m_nTab == m_nTabFooting) SetGridDataFooting();
	else if(m_nTab == m_nTabRib) SetGridDataRib();
}

void CRebarDanDlg::SetDataInit()
{
	if(!m_pBri) return;
	m_Grid.UpdateData(FALSE);
	
	// ����
	long row(0), col(0);
	m_Grid.GetCellCount(row, col);
	m_Grid.MergeGrid(0, 1, 0, col);

	UpdateData(FALSE);
}

void CRebarDanDlg::SetDataSave()
{
	if(!m_pBri) return;
	
	UpdateData(TRUE);	
	m_Grid.UpdateData(TRUE);

	SetExistOutWall2Dan();
}

void CRebarDanDlg::InitTab()
{
	m_ctlTab.RemoveAllTabs();
	m_ctlTab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	
	
	long nTab		= 0;
	m_nTabUpperSlab	= -1;
	m_nTabLowerSlab = -1;
	m_nTabOutWall	= -1;
	m_nTabInWall	= -1;
	m_nTabFooting	= -1;
	m_nTabRib		= -1;
	m_sArrTabName.RemoveAll();

	if(!m_pBri) return;
	
	m_sArrTabName.Add("��ν�����"); m_nTabUpperSlab = nTab++;
	if(m_pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		m_sArrTabName.Add("�Ϻν�����"); m_nTabLowerSlab = nTab++;
	}
	m_sArrTabName.Add("������ü"); m_nTabOutWall = nTab++;
	if(m_pBri->GetCountInWall() > 0)
	{
		m_sArrTabName.Add("������ü"); m_nTabInWall	= nTab++;
	}
	if(m_pBri->IsBoxType() == FALSE)
	{
		m_sArrTabName.Add("����"); m_nTabFooting = nTab++;
	}

	if(m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		m_sArrTabName.Add("����"); m_nTabRib = nTab++;
	}


		
	long i = 0; for(i = 0; i < m_sArrTabName.GetSize(); i++)
		m_ctlTab.InsertItem(i, m_sArrTabName[i]);

	if(m_nTypeTab < m_ctlTab.GetItemCount())
		m_nTab	= m_nTypeTab;

	if(m_ctlTab.GetItemCount() <= m_nTab) m_nTab = 0;
	m_ctlTab.SetCurSel(m_nTab);
}

void CRebarDanDlg::SetGridDataUpperSlab()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;
	if(m_pBri->m_bDetailInput23DanMainRebar)
	{
		m_Grid.SetGridDefault(1, 1, 1, 1);
		m_Grid.AddCell(0, 0, "2, 3�� ö�� ���� �Է�");
		m_Grid.SetColumnWidth(0, 200);
		return;
	}

	long row = 0, col = 0;
	CString str = _T(""), sHelp = _T("");
	CStringArray sArr;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	// ���õ� ����Ŭ ��
	long nCountCycle = pBri->GetCountCycleMainRebar();
	
	m_Grid.AddCell(row, col, "ö����ġ\n");
	m_Grid.AddCell(row+1, col++, "ö����ġ\n");
	
	m_Grid.AddCell(row, col, "��ġ�ܼ�\n");
	m_Grid.AddCell(row+1, col++, "��ġ�ܼ�\n");

	if(!m_pBri->m_bDetailInput23DanMainRebar)
	{
		long dan = 0; for(dan = 1; dan < MAX_REBAR_DAN; dan++)
		{
			long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
			{
				str.Format("%d��ö��", dan+1);
				m_Grid.AddCell(row, col, str);

				str = pBri->m_pARcBridgeDataStd->GetStringCircleNum(cycle+1);
				m_Grid.AddCell(row+1, col++, str);
			}
		}

		m_Grid.AddCell(row, col, "2,3�� ö��\n�������");
		m_Grid.AddCell(row+1, col, "2,3�� ö��\n�������");
	}

	// setrebardan�� �ʿ��� ��ŭ �����Ѵ�.
	long nIdxSetRebarDan = 0;
	long nCountNeedSetRebarDanUpper = pBri->m_nQtyJigan+1;
	long nCountNeedSetRebarDanLower = pBri->m_nQtyJigan;

	AhTPAMake(nCountNeedSetRebarDanUpper, &pBri->m_pArrSetRebarDanUpperSlab[iUPPER], (CSetRebarDan*)0);
	AhTPAMake(nCountNeedSetRebarDanLower, &pBri->m_pArrSetRebarDanUpperSlab[iLOWER], (CSetRebarDan*)0);
	
	// �ܼ� �޺� ���� ����
	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		str.Format("%d��", dan+1);
		sArr.Add(str);
	}

	// ��� : ������ ��������
	row	= 2;
	col = 0;
	CSetRebarDan *pSetRebarDan = pBri->m_pArrSetRebarDanUpperSlab[iUPPER].GetAt(nIdxSetRebarDan++);
	m_Grid.AddCell(row, col++, "��� : ������ ��������");
	m_Grid.AddCell(row, col++, &pSetRebarDan->m_nCountDan, nFormat, TRUE, nLine, sHelp, CELL_TYPE_COMBO, sArr); m_Grid.SetCellBaseOfIndex(1);

	if(!m_pBri->m_bDetailInput23DanMainRebar)
	{
		for(dan = 1; dan < MAX_REBAR_DAN; dan++)
		{
			long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
			{
				m_Grid.AddCell(row, col++, &pSetRebarDan->m_dDia[cycle][dan], nFormat, nLine, sHelp, CELL_TYPE_DIA);
			}
		}
		m_Grid.AddCell(row, col++, &pSetRebarDan->m_dLengthExt23DanRebar);
	}

	// ��� : ��ü(���)
	long wall = 0; for(wall = 0; wall < pBri->m_nQtyJigan-1; wall++)
	{
		row++;
		col = 0;
		pSetRebarDan = pBri->m_pArrSetRebarDanUpperSlab[iUPPER].GetAt(nIdxSetRebarDan++);

		str.Format("��� : �߰�����%2d", wall+1);
		m_Grid.AddCell(row, col++, str);
		m_Grid.AddCell(row, col++, &pSetRebarDan->m_nCountDan, nFormat, TRUE, nLine, sHelp, CELL_TYPE_COMBO, sArr); m_Grid.SetCellBaseOfIndex(1);

		if(!m_pBri->m_bDetailInput23DanMainRebar)
		{
			for(dan = 1; dan < MAX_REBAR_DAN; dan++)
			{
				long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
				{
					m_Grid.AddCell(row, col++, &pSetRebarDan->m_dDia[cycle][dan], nFormat, nLine, sHelp, CELL_TYPE_DIA);
				}
			}

			m_Grid.AddCell(row, col++, &pSetRebarDan->m_dLengthExt23DanRebar);
		}
	}
	
	// ��� : ������ ��������
	row++;
	col = 0;
	pSetRebarDan = pBri->m_pArrSetRebarDanUpperSlab[iUPPER].GetAt(nIdxSetRebarDan++);
	m_Grid.AddCell(row, col++, "��� : ������ ��������");
	m_Grid.AddCell(row, col++, &pSetRebarDan->m_nCountDan, nFormat, TRUE, nLine, sHelp, CELL_TYPE_COMBO, sArr); m_Grid.SetCellBaseOfIndex(1);
	if(!m_pBri->m_bDetailInput23DanMainRebar)
	{
		for(dan = 1; dan < MAX_REBAR_DAN; dan++)
		{
			long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
			{
				m_Grid.AddCell(row, col++, &pSetRebarDan->m_dDia[cycle][dan], nFormat, nLine, sHelp, CELL_TYPE_DIA);
			}
		}
		m_Grid.AddCell(row, col++, &pSetRebarDan->m_dLengthExt23DanRebar);
	}

	// �ϸ� : ���� �߾Ӻ�
	nIdxSetRebarDan	= 0;
	for(long jigan = 0; jigan < pBri->m_nQtyJigan; jigan++)
	{
		row++;
		col = 0;
		pSetRebarDan = pBri->m_pArrSetRebarDanUpperSlab[iLOWER].GetAt(nIdxSetRebarDan++);

		str.Format("�ϸ� : �����߾Ӻ�%2d", jigan+1);
		m_Grid.AddCell(row, col++, str);
		m_Grid.AddCell(row, col++, &pSetRebarDan->m_nCountDan, nFormat, TRUE, nLine, sHelp, CELL_TYPE_COMBO, sArr); m_Grid.SetCellBaseOfIndex(1);

		if(!m_pBri->m_bDetailInput23DanMainRebar)
		{
			for(dan = 1; dan < MAX_REBAR_DAN; dan++)
			{
				long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
				{
					m_Grid.AddCell(row, col++, &pSetRebarDan->m_dDia[cycle][dan], nFormat, nLine, sHelp, CELL_TYPE_DIA);
				}
			}

			m_Grid.AddCell(row, col++, &pSetRebarDan->m_dLengthExt23DanRebar);
		}
	}


	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 150);
	m_Grid.SetColumnWidth(1, 80);
	if(col > 6)
		m_Grid.SetColumnWidth(6, 80);
}

void CRebarDanDlg::SetGridDataLowerSlab()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;
	if(m_pBri->m_bDetailInput23DanMainRebar)
	{
		m_Grid.SetGridDefault(1, 1, 1, 1);
		m_Grid.AddCell(0, 0, "2, 3�� ö�� ���� �Է�");
		m_Grid.SetColumnWidth(0, 200);
		return;
	}

	long row = 0, col = 0;
	CString str = _T(""), sHelp = _T("");
	CStringArray sArr;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	// ���õ� ����Ŭ ��
	long nCountCycle = pBri->GetCountCycleMainRebar();

	m_Grid.AddCell(row, col, "ö����ġ\n");
	m_Grid.AddCell(row+1, col++, "ö����ġ\n");

	m_Grid.AddCell(row, col, "��ġ�ܼ�\n");
	m_Grid.AddCell(row+1, col++, "��ġ�ܼ�\n");

	if(!m_pBri->m_bDetailInput23DanMainRebar)
	{
		long dan = 0; for(dan = 1; dan < MAX_REBAR_DAN; dan++)
		{
			long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
			{
				str.Format("%d��ö��", dan+1);
				m_Grid.AddCell(row, col, str);

				str = pBri->m_pARcBridgeDataStd->GetStringCircleNum(cycle+1);
				m_Grid.AddCell(row+1, col++, str);
			}
		}

		m_Grid.AddCell(row, col, "2,3�� ö��\n�������");
		m_Grid.AddCell(row+1, col, "2,3�� ö��\n�������");
	}

	// setrebardan�� �ʿ��� ��ŭ �����Ѵ�.
	long nIdxSetRebarDan = 0;
	long nCountNeedSetRebarDanUpper = pBri->m_nQtyJigan;
	long nCountNeedSetRebarDanLower = pBri->m_nQtyJigan+1;

	AhTPAMake(nCountNeedSetRebarDanUpper, &pBri->m_pArrSetRebarDanLowerSlab[iUPPER], (CSetRebarDan*)0);
	AhTPAMake(nCountNeedSetRebarDanLower, &pBri->m_pArrSetRebarDanLowerSlab[iLOWER], (CSetRebarDan*)0);

	// �ܼ� �޺� ���� ����
	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		str.Format("%d��", dan+1);
		sArr.Add(str);
	}

	// �ϸ� : ������ ��������
	row	= 2;
	col = 0;
	CSetRebarDan *pSetRebarDan = pBri->m_pArrSetRebarDanLowerSlab[iLOWER].GetAt(nIdxSetRebarDan++);
	m_Grid.AddCell(row, col++, "�ϸ� : ������ ��������");
	m_Grid.AddCell(row, col++, &pSetRebarDan->m_nCountDan, nFormat, TRUE, nLine, sHelp, CELL_TYPE_COMBO, sArr); m_Grid.SetCellBaseOfIndex(1);

	if(!m_pBri->m_bDetailInput23DanMainRebar)
	{
		for(dan = 1; dan < MAX_REBAR_DAN; dan++)
		{
			long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
			{
				m_Grid.AddCell(row, col++, &pSetRebarDan->m_dDia[cycle][dan], nFormat, nLine, sHelp, CELL_TYPE_DIA);
			}
		}
		m_Grid.AddCell(row, col++, &pSetRebarDan->m_dLengthExt23DanRebar);
	}

	// �ϸ� : ��ü(���)
	long wall = 0; for(wall = 0; wall < pBri->m_nQtyJigan-1; wall++)
	{
		row++;
		col = 0;
		pSetRebarDan = pBri->m_pArrSetRebarDanLowerSlab[iLOWER].GetAt(nIdxSetRebarDan++);

		str.Format("�ϸ� : �߰�����%2d", wall+1);
		m_Grid.AddCell(row, col++, str);
		m_Grid.AddCell(row, col++, &pSetRebarDan->m_nCountDan, nFormat, TRUE, nLine, sHelp, CELL_TYPE_COMBO, sArr); m_Grid.SetCellBaseOfIndex(1);

		if(!m_pBri->m_bDetailInput23DanMainRebar)
		{
			for(dan = 1; dan < MAX_REBAR_DAN; dan++)
			{
				long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
				{
					m_Grid.AddCell(row, col++, &pSetRebarDan->m_dDia[cycle][dan], nFormat, nLine, sHelp, CELL_TYPE_DIA);
				}
			}

			m_Grid.AddCell(row, col++, &pSetRebarDan->m_dLengthExt23DanRebar);
		}
	}

	// �ϸ� : ������ ��������
	row++;
	col = 0;
	pSetRebarDan = pBri->m_pArrSetRebarDanLowerSlab[iLOWER].GetAt(nIdxSetRebarDan++);
	m_Grid.AddCell(row, col++, "�ϸ� : ������ ��������");
	m_Grid.AddCell(row, col++, &pSetRebarDan->m_nCountDan, nFormat, TRUE, nLine, sHelp, CELL_TYPE_COMBO, sArr); m_Grid.SetCellBaseOfIndex(1);
	if(!m_pBri->m_bDetailInput23DanMainRebar)
	{
		for(dan = 1; dan < MAX_REBAR_DAN; dan++)
		{
			long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
			{
				m_Grid.AddCell(row, col++, &pSetRebarDan->m_dDia[cycle][dan], nFormat, nLine, sHelp, CELL_TYPE_DIA);
			}
		}
		m_Grid.AddCell(row, col++, &pSetRebarDan->m_dLengthExt23DanRebar);
	}

	// ��� : ���� �߾Ӻ�
	nIdxSetRebarDan	= 0;
	for(long jigan = 0; jigan < pBri->m_nQtyJigan; jigan++)
	{
		row++;
		col = 0;
		pSetRebarDan = pBri->m_pArrSetRebarDanLowerSlab[iUPPER].GetAt(nIdxSetRebarDan++);

		str.Format("��� : �����߾Ӻ�%2d", jigan+1);
		m_Grid.AddCell(row, col++, str);
		m_Grid.AddCell(row, col++, &pSetRebarDan->m_nCountDan, nFormat, TRUE, nLine, sHelp, CELL_TYPE_COMBO, sArr); m_Grid.SetCellBaseOfIndex(1);

		if(!m_pBri->m_bDetailInput23DanMainRebar)
		{
			for(dan = 1; dan < MAX_REBAR_DAN; dan++)
			{
				long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
				{
					m_Grid.AddCell(row, col++, &pSetRebarDan->m_dDia[cycle][dan], nFormat, nLine, sHelp, CELL_TYPE_DIA);
				}
			}

			m_Grid.AddCell(row, col++, &pSetRebarDan->m_dLengthExt23DanRebar);
		}
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 150);
	m_Grid.SetColumnWidth(1, 80);
	if(col > 6)
		m_Grid.SetColumnWidth(6, 80);
}
// ������ü ö�� �ܼ� ����
void CRebarDanDlg::SetGridDataOutWall()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;
	if(m_pBri->m_bDetailInput23DanMainRebar)
	{
		m_Grid.SetGridDefault(1, 1, 1, 1);
		m_Grid.AddCell(0, 0, "2, 3�� ö�� ���� �Է�");
		m_Grid.SetColumnWidth(0, 200);
		return;
	}

	long row = 0, col = 0;
	CString str = _T(""), sHelp = _T("");
	CStringArray sArr;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	// ���õ� ����Ŭ ��
	long nCountCycle = pBri->GetCountCycleMainRebar();
	
	m_Grid.AddCell(row, col, "ö����ġ\n");
	m_Grid.AddCell(row+1, col++, "ö����ġ\n");
	
	m_Grid.AddCell(row, col, "��ġ�ܼ�\n");
	m_Grid.AddCell(row+1, col++, "��ġ�ܼ�\n");

	if(!m_pBri->m_bDetailInput23DanMainRebar)
	{
		long dan = 0; for(dan = 1; dan < MAX_REBAR_DAN; dan++)
		{
			long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
			{
				str.Format("%d��ö��", dan+1);
				m_Grid.AddCell(row, col, str);

				str = pBri->m_pARcBridgeDataStd->GetStringCircleNum(cycle+1);
				m_Grid.AddCell(row+1, col++, str);
			}
		}
		m_Grid.AddCell(row, col, "2,3�� ö��\n�������");
		m_Grid.AddCell(row+1, col, "2,3�� ö��\n�������");
	}

	// �ܼ� �޺� ���� ����
	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		str.Format("%d��", dan+1);
		sArr.Add(str);
	}

	// ��� : ������ ��������
	CString sStt	= _T("");
	CString sInner	= _T("");
	row	= 1;
	for(long stt = 0; stt < 2; stt++)
	{
		sStt	= (stt == iSTT) ? "������" : "������";
		for(long inner = 0; inner < 2; inner++)
		{
			sInner	= (inner == iINNER) ? "������" : "������";

			if(pBri->IsOutWall(stt))
			{
				row++;
				col = 0;
				str.Format("%s : %s", sStt, sInner);
				
				CSetRebarDan *pSetRebarDan = &pBri->m_pArrSetRebarDanOutWall[stt][inner];
				m_Grid.AddCell(row, col++, str);
				m_Grid.AddCell(row, col++, &pSetRebarDan->m_nCountDan, nFormat, TRUE, nLine, sHelp, CELL_TYPE_COMBO, sArr); m_Grid.SetCellBaseOfIndex(1);

				if(!m_pBri->m_bDetailInput23DanMainRebar)
				{
					long dan = 0; for(dan = 1; dan < MAX_REBAR_DAN; dan++)
					{
						long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
						{
							m_Grid.AddCell(row, col++, &pSetRebarDan->m_dDia[cycle][dan], nFormat, nLine, sHelp, CELL_TYPE_DIA);
						}
					}

					m_Grid.AddCell(row, col++, &pSetRebarDan->m_dLengthExt23DanRebar);
				}
			}
		}
	}


	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 150);
	m_Grid.SetColumnWidth(1, 80);
	if(col > 6)
		m_Grid.SetColumnWidth(6, 80);
}


void CRebarDanDlg::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	
	return;
}

void CRebarDanDlg::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	m_nTab = m_ctlTab.GetCurSel();
	SetGridData();
	SetDataInit();

	return;
}

BOOL CRebarDanDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Grid.ViewGridCellForCombo(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}


// ������ü ö�� �ܼ� ����
void CRebarDanDlg::SetGridDataInWall()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;
	if(m_pBri->m_bDetailInput23DanMainRebar)
	{
		m_Grid.SetGridDefault(1, 1, 1, 1);
		m_Grid.AddCell(0, 0, "2, 3�� ö�� ���� �Է�");
		m_Grid.SetColumnWidth(0, 200);
		return;
	}
	
	long nCountInWall	= pBri->GetCountInWall();

	long row = 0, col = 0;
	CString str = _T(""), sHelp = _T("");
	CStringArray sArr;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	// ���õ� ����Ŭ ��
	long nCountCycle = pBri->GetCountCycleMainRebar();
	
	m_Grid.AddCell(row, col, "ö����ġ\n");
	m_Grid.AddCell(row+1, col++, "ö����ġ\n");
	
	m_Grid.AddCell(row, col, "��ġ�ܼ�\n");
	m_Grid.AddCell(row+1, col++, "��ġ�ܼ�\n");

	if(!m_pBri->m_bDetailInput23DanMainRebar)
	{
		long dan = 0; for(dan = 1; dan < MAX_REBAR_DAN; dan++)
		{
			long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
			{
				str.Format("%d��ö��", dan+1);
				m_Grid.AddCell(row, col, str);

				str = pBri->m_pARcBridgeDataStd->GetStringCircleNum(cycle+1);
				m_Grid.AddCell(row+1, col++, str);
			}
		}
		m_Grid.AddCell(row, col, "2,3�� ö��\n�������");
		m_Grid.AddCell(row+1, col, "2,3�� ö��\n�������");
	}

	row	= 1;
	long wall = 0; for(wall = 0; wall < nCountInWall; wall++)
	{
		CWallApp *pWall	= pBri->GetInWall(wall);
		if(!pWall) continue;
		if(!pWall->m_bIs) continue;


		// �ܼ� �޺� ���� ����
		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			str.Format("%d��", dan+1);
			sArr.Add(str);
		}

		// ��� : ������ ��������
		CString sLeft	= _T("");
		
		for(long left = 0; left < 2; left++)
		{
			sLeft	= (left == iLEFT) ? "����" : "����";
			
			row++;
			col = 0;
			str.Format("�߰�����%d(%s)", wall+1, sLeft);
			
			CSetRebarDan *pSetRebarDan = &pWall->m_setRebarDan[left];
			m_Grid.AddCell(row, col++, str);
			m_Grid.AddCell(row, col++, &pSetRebarDan->m_nCountDan, nFormat, TRUE, nLine, sHelp, CELL_TYPE_COMBO, sArr); m_Grid.SetCellBaseOfIndex(1);

			if(!m_pBri->m_bDetailInput23DanMainRebar)
			{
				long dan = 0; for(dan = 1; dan < MAX_REBAR_DAN; dan++)
				{
					long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
					{
						m_Grid.AddCell(row, col++, &pSetRebarDan->m_dDia[cycle][dan], nFormat, nLine, sHelp, CELL_TYPE_DIA);
					}
				}
				
				m_Grid.AddCell(row, col++, &pSetRebarDan->m_dLengthExt23DanRebar);
			}
		}

		col = 0;
	}


	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 150);
	m_Grid.SetColumnWidth(1, 80);
	if(col > 6)
		m_Grid.SetColumnWidth(6, 80);
}

// ���� �� ����
// ����������, ��������, ���������� - ���ϸ�
void CRebarDanDlg::SetGridDataFooting()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri	= m_pBri;
	if(!pBri) return;
	long nCountInWall		= pBri->GetCountInWall();
	long nCountFooting		= nCountInWall+2;
	

	long row = 0, col = 0;
	CString str = _T(""), sHelp = _T("");
	CStringArray sArr;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	// ���õ� ����Ŭ ��
	long nCountCycle = pBri->GetCountCycleMainRebar();
	
	m_Grid.AddCell(row, col, "ö����ġ\n");
	m_Grid.AddCell(row+1, col++, "ö����ġ\n");
	
	m_Grid.AddCell(row, col, "��ġ�ܼ�\n");
	m_Grid.AddCell(row+1, col++, "��ġ�ܼ�\n");

	long dan = 0; for(dan = 1; dan < MAX_REBAR_DAN; dan++)
	{
		long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
		{
			str.Format("%d��ö��", dan+1);
			m_Grid.AddCell(row, col, str);

			str = pBri->m_pARcBridgeDataStd->GetStringCircleNum(cycle+1);
			m_Grid.AddCell(row+1, col++, str);
		}
	}
	m_Grid.AddCell(row, col, "2,3�� ö��\n�������");
	m_Grid.AddCell(row+1, col, "2,3�� ö��\n�������");

	// �ܼ� �޺� ���� ����
	for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		str.Format("%d��", dan+1);
		sArr.Add(str);
	}

	// ���������� : ���ϸ�
	// ������ü : ���ϸ�(�������� �ִ��� �ϳ��� �Ѵ�..�ϴ�)
	// ��������ü : ���ϸ�
	CString sUpper		= _T("");
	CString sFooting	= _T("");
	long nIdxWall		= 0;
	row					= 1;
	long footing = 0; for(footing = 0; footing < nCountFooting; footing++)
	{
		// ���� ����
		CFootingApp *pFooting	= NULL;
		if(footing == 0)
		{
			if(pBri->IsFooting(TRUE)) pFooting	= &pBri->m_footingStt;
			sFooting	= _T("������");
		}
		else if(footing == nCountFooting-1)
		{
			if(pBri->IsFooting(FALSE)) pFooting	= &pBri->m_footingEnd;
			sFooting	= _T("������");
		}
		else 
		{
			CWallApp *pWall	= pBri->GetInWall(nIdxWall++);
			pFooting		= &pWall->m_footing;
			sFooting.Format("�߰�����%d", nIdxWall);
		}

		if(!pFooting) continue;

		for(long updn = 0; updn < 2; updn++)
		{
			sUpper	= (updn == iUPPER) ? "���" : "�ϸ�";
			
			row++;
			col = 0;
			
			CSetRebarDan *pSetRebarDan = &pFooting->m_setRebarDan[updn];
			m_Grid.AddCell(row, col++, sFooting+":"+sUpper);
			m_Grid.AddCell(row, col++, &pSetRebarDan->m_nCountDan, nFormat, TRUE, nLine, sHelp, CELL_TYPE_COMBO, sArr); m_Grid.SetCellBaseOfIndex(1);
			long dan = 0; for(dan = 1; dan < MAX_REBAR_DAN; dan++)
			{
				long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
				{
					m_Grid.AddCell(row, col++, &pSetRebarDan->m_dDia[cycle][dan], nFormat, nLine, sHelp, CELL_TYPE_DIA);
				}
			}

			m_Grid.AddCell(row, col++, &pSetRebarDan->m_dLengthExt23DanRebar);
		}
	}



	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 150);
	m_Grid.SetColumnWidth(1, 80);
	m_Grid.SetColumnWidth(6, 80);
}

void CRebarDanDlg::OnButtonRecommand() 
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox("���� �Է� ������ �����Ͱ� ��� �⺻������ �ٲ�ϴ�. ��� �Ͻðڽ��ϱ�?  ", MB_YESNO | MB_ICONQUESTION)==IDNO)
		return;

	m_pBri->SetDataDefaultSetRebarDan(-1);
	SetDataInit();
}

void CRebarDanDlg::SetGridDataRib()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;

	long row = 0, col = 0;
	CString str = _T(""), sHelp = _T("");
	CStringArray sArr;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	// ���õ� ����Ŭ ��
	long nCountCycle = pBri->GetCountCycleMainRebar();
	
	m_Grid.AddCell(row, col, "ö����ġ\n");
	m_Grid.AddCell(row+1, col++, "ö����ġ\n");
	
	CString sCycle	= _T("");
	long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
	{
		sCycle = pBri->m_pARcBridgeDataStd->GetStringCircleNum(cycle+1);
		m_Grid.AddCell(row, col, "��ġ�ܼ�");
		m_Grid.AddCell(row+1, col++, sCycle);
	}

	row	= 1;
	// �ܼ� �޺� ���� ����
	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		str.Format("%d��", dan+1);
		sArr.Add(str);
	}

	CString sUpper	= _T("");
	
	for(long upper = 0; upper < 2; upper++)
	{
		sUpper	= upper == iUPPER ? "���" : "�ϸ�";
		
		row++;
		col = 0;
		str	= sUpper;
		
		m_Grid.AddCell(row, col++, str);
		for(cycle = 0; cycle < nCountCycle; cycle++)
		{
			m_Grid.AddCell(row, col++, &pBri->m_nCountDanMainRebarRib[cycle][upper], nFormat, TRUE, nLine, sHelp, CELL_TYPE_COMBO, sArr); m_Grid.SetCellBaseOfIndex(1);
		}
	}


	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 150);
	for(cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
		m_Grid.SetColumnWidth(cycle+1, 80);
}

void CRebarDanDlg::OnCheckDetailinput23dan() 
{
	// TODO: Add your control notification handler code here
	if(!m_pBri) return;

	if(AfxMessageBox("2, 3�� ��ö�� �Է¹���� �����ϸ�, ������ 2, 3�� ��ö���� �ʱ�ȭ �˴ϴ�. ��� �Ͻðڽ��ϱ�?", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		m_checkDetailInput23Dan.SetCheck(!m_checkDetailInput23Dan.GetCheck());
		return;
	}
		
	m_pBri->Clear23DanMainRebar();
	m_pBri->m_bDetailInput23DanMainRebar	= m_checkDetailInput23Dan.GetCheck();
	long nCountInWall	= m_pBri->GetCountInWall();
	long wall = 0; for(wall = 0; wall < nCountInWall; wall++)
	{
		CWallApp *pWall	= m_pBri->GetInWall(wall);
		if(!pWall) continue;
		pWall->m_bDetailInput23DanMainRebar	= m_pBri->m_bDetailInput23DanMainRebar;
	}

	SetGridData();
	SetDataInit();
}

void CRebarDanDlg::SetExistOutWall2Dan()
{
	if(m_nTab != m_nTabOutWall && m_nTab != m_nTabInWall)
		return;
	
	for (long nStt=0; nStt < 2; nStt++)
	{
		for (long nInner =0; nInner < 2; nInner++)
		{
			CSetRebarDan *pSetRebarDan		= &m_pBri->m_pArrSetRebarDanOutWall[nStt][nInner];
			if(pSetRebarDan == NULL) continue;
			if(pSetRebarDan->m_nCountDan < 2)
			{
				m_pBri->m_pArrExistOutWall[m_nCycle][nStt][nInner][1].RemoveAll();
				m_pBri->m_pArrExistOutWall[m_nCycle][nStt][nInner][2].RemoveAll();
				continue;
			}

			for (long nDan =1; nDan < pSetRebarDan->m_nCountDan; nDan++)
			{
				for (long nIdx =0; nIdx < m_pBri->m_pArrExistOutWall[m_nCycle][nStt][nInner][nDan].GetSize(); nIdx++)
				{
					BOOL bDia =  pSetRebarDan->m_dDia[m_nCycle][nDan] > 0? TRUE : FALSE;

					m_pBri->m_pArrExistOutWall[m_nCycle][nStt][nInner][nDan].SetAt(nIdx, (double)bDia);
				}	
			}			
		}
	}
}