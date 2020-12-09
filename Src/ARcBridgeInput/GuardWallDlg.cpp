// GuardWallDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "GuardWallDlg.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuardWallDlg dialog


//<param name="bRoadNotSlab">TRUE=���߶���� ��� ���(���θ�, ö����)�̸� �������(����)�ƴ�
//							FALSE=�Ϲݱ���, ���߶���� �������(����)</param>
CGuardWallDlg::CGuardWallDlg(CARcBridgeInputStd* pIn, BOOL bRoadNotSlab, CWnd* pParent /*=NULL*/)
	: CDialog(CGuardWallDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGuardWallDlg)
	//}}AFX_DATA_INIT
	m_pIn = pIn;
	m_bRoadNotSlab = bRoadNotSlab;

	m_bRail = FALSE;
	m_nTab7 = -1;
	m_nTab6 = -1;
	m_nTab5 = -1;
	m_nTab2 = -1;
	m_nTab8 = -1;
	m_nTab1 = -1;
	m_nTab3 = -1;
	m_nTab4 = -1;

//	m_DomView.GetDomyun()->SetCWnd(pParent);
}


void CGuardWallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGuardWallDlg)
	DDX_Control(pDX, IDC_HYPER_HELP, m_hyperHelp);
	DDX_Control(pDX, IDC_BUTTON_RECOMMAND, m_btDefault);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_DomControl(pDX, IDC_DOM, m_DomView);
	DDX_Control(pDX, IDC_TAB, m_ctlTab);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGuardWallDlg, CDialog)
	//{{AFX_MSG_MAP(CGuardWallDlg)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMAND, OnButtonDefault)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuardWallDlg message handlers

BOOL CGuardWallDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_MOUSEWHEEL)
	{
		m_DomView.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	}

	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().row == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}

	if(pMsg->hwnd==m_Grid.GetSafeHwnd() && pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		CStringArray strArr;
		long nTab = m_ctlTab.GetCurSel();
		if(nTab==m_nTab3)	// ���� �� ���
		{
			if(next.col==1)
			{
				strArr.Add(_T("������"));
				strArr.Add(_T("��ũ��Ʈ"));
				strArr.Add(_T("�ƽ���"));
				m_Grid.SetComboString(strArr);
			}
			if(next.col==2)
			{
				strArr.Add(_T("��ũ��Ʈ"));
				strArr.Add("��Ż");
				strArr.Add(_T("��"));
				m_Grid.SetComboString(strArr);
			}
		}
	}

	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);	
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CGuardWallDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(m_bRoadNotSlab)
	{
		CString str = _T("���� �μ� ������ �Է�");
		CGuardWallDlg::SetWindowText(str);
	}
	
	m_DomView.SetOwnerMouseMode(FALSE);
	m_DomView.m_bEnablePopupMenu = TRUE;
	m_DomView.SetCrossLength(0);


	COLORREF clrLink = RGB(0x00, 0x00, 0xFF);
	m_hyperHelp.SetColors(clrLink, clrLink, clrLink);
	m_hyperHelp.SetURL(m_pIn->GetManualPath());
	m_hyperHelp.SetTipText(_T("���� �Է�â�� ���� ������ ǥ���մϴ�."));

	m_bRail = m_pIn->m_pARcBridgeDataStd->IsRailDesign();
	CRcBridgeApp* pBri = m_pIn->GetRcBridge();
	if(!pBri) return TRUE;

	InitTabCtrl();

	if(!pBri->m_bGuardWallDlgUsed)
		SetDataDefault();
	SetDataInit();

	SetFocusCellByType();

	DrawInputDomyunView();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGuardWallDlg::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	SetDataSave();
	DrawInputDomyunView(FALSE);
}

void CGuardWallDlg::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	
	long nRow = pGridView->iRow;
	long nCol = pGridView->iColumn;

	CString str = m_Grid.GetTextMatrix(0, nCol);
	if(str==_T("C"))
		str = _T("C X C");
	if(str==_T("���� ���� ����"))
		str = _T("����");
	if(str==_T("SF(%)"))
		str = m_Grid.GetTextMatrix(nRow, nCol) + _T("%");
	if(str=="P (kN)")
		str = "P";

	DrawInputDomyunView();
	m_DomView.STMakeCursor(str);
}

void CGuardWallDlg::InitTabCtrl()
{
	CRcBridgeRebar* pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	m_ctlTab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	m_nTab1	= -1;
	m_nTab2	= -1;
	m_nTab3	= -1;
	m_nTab4	= -1;
	m_nTab5	= -1;
	m_nTab6	= -1;
	m_nTab7	= -1;
	m_nTab8	= -1;
	
	long nTab = 0;
	if(m_bRail)
	{
		if(pBri->GetQtyHDanRailGuardFence(m_bRoadNotSlab)>0)
		{
			m_nTab1 = nTab++;
			m_ctlTab.InsertItem(m_nTab1, _T("����"));
		}
		if(pBri->GetQtyHDanRailDetectGravel(m_bRoadNotSlab)>0)
		{
			m_nTab2 = nTab++;
			m_ctlTab.InsertItem(m_nTab2, _T("�ڰ�����(ö��)"));
		}
		if(pBri->GetQtyHDanRailFootway(m_bRoadNotSlab)>0)
		{
			m_nTab3 = nTab++;
			m_ctlTab.InsertItem(m_nTab3, _T("��������"));
		}
		if(pBri->GetQtyHDanRailFootwayBlock(m_bRoadNotSlab)>0)
		{
			m_nTab4 = nTab++;
			m_ctlTab.InsertItem(m_nTab4, _T("����+����"));
		}
		if(pBri->GetQtyHDanRailHole(m_bRoadNotSlab)>0)
		{
			m_nTab5 = nTab++;
			m_ctlTab.InsertItem(m_nTab5, _T("��ű�"));
		}
		if(pBri->GetQtyHDanRailHoleFootway(m_bRoadNotSlab)>0)
		{
			m_nTab6 = nTab++;
			m_ctlTab.InsertItem(m_nTab6, _T("��ű�+����"));
		}
		if(pBri->GetQtyHDanRoadRailDike(m_bRoadNotSlab)>0)
		{
			m_nTab7 = nTab++;
			m_ctlTab.InsertItem(m_nTab7, _T("����ũ"));
		}
		if(pBri->GetQtyHDanRailDetectGravel2(m_bRoadNotSlab)>0)
		{
			m_nTab8 = nTab++;
			m_ctlTab.InsertItem(m_nTab8, _T("�ڰ�����(����)"));
		}
	}
	else
	{
		if(pBri->GetQtyHDanRoadGuardFence(m_bRoadNotSlab) + pBri->GetQtyHDanRoadCentralWallFull(m_bRoadNotSlab) + pBri->GetQtyHDanRoadCentralWallHalf(m_bRoadNotSlab)>0)
		{
			m_nTab1 = nTab++;
			m_ctlTab.InsertItem(m_nTab1, _T("��ȣ�� �� �ߺд�"));
		}
		if(pBri->GetQtyHDanRoadFootwayBlock(m_bRoadNotSlab)>0)
		{
			m_nTab2 = nTab++;
			m_ctlTab.InsertItem(m_nTab2, _T("������ ��輮"));
		}
		if(pBri->GetQtyHDanRoadFootway(m_bRoadNotSlab)>0)
		{
			m_nTab3 = nTab++;
			m_ctlTab.InsertItem(m_nTab3, _T("����"));
		}
		if(pBri->GetQtyHDanRoadSoundProof(m_bRoadNotSlab)>0)
		{
			m_nTab4 = nTab++;
			m_ctlTab.InsertItem(m_nTab4, _T("������ ����"));
		}
		if(pBri->GetQtyHDanRoadRetainingWall(m_bRoadNotSlab)>0)
		{
			m_nTab5 = nTab++;
			m_ctlTab.InsertItem(m_nTab5, _T("�븷�̺�"));
		}
		if(pBri->GetQtyHDanRoadHandRail(m_bRoadNotSlab)>0)
		{
			m_nTab6 = nTab++;
			m_ctlTab.InsertItem(m_nTab6, _T("��������"));
		}
		if(pBri->GetQtyHDanRoadRailDike(m_bRoadNotSlab)>0)
		{
			m_nTab7 = nTab++;
			m_ctlTab.InsertItem(m_nTab7, _T("����ũ"));
		}
	}
}

void CGuardWallDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long nJ = 0;
	double sta = pBri->GetStationOnJijum(nJ);
	CDPoint vAng = CDPoint(0,1);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pBri->GetJijum(0)->m_dLengthUgan[1];
	if(pBri->m_bIsExp || pBri->IsExtLine())
	{
		sta = pBri->GetStationBridgeMid();
	}

	CGuardWallRC* pGW = NULL;
	long nTab = m_ctlTab.GetCurSel();
	long nGW = m_Grid.GetFocusCell().row - 1;
	if(nGW < 0)	nGW = 0;

	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
	
	CDomyun *pDom = m_DomView.GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(20);
	if(m_bRail)
	{
		if(nTab == m_nTab1)				//����
			pGW = pBri->GetGuardWallRailGuardFence(nGW,m_bRoadNotSlab);
		else if(nTab == m_nTab2)		//�ڰ�����
			pGW = pBri->GetGuardWallRailDetectGravel(nGW,m_bRoadNotSlab);
		else if(nTab == m_nTab3)		//����
		{
			pDom->SetScaleDim(15);
			pGW = pBri->GetGuardWallRailFootway(nGW,m_bRoadNotSlab);
		}
		else if(nTab == m_nTab4)		//����+����
			pGW = pBri->GetGuardWallRailFootwayBlock(nGW,m_bRoadNotSlab);
		else if(nTab == m_nTab5)		//��ű�
			pGW = pBri->GetGuardWallRailHole(nGW,m_bRoadNotSlab);
		else if(nTab == m_nTab6)		//��ű�+����
			pGW = pBri->GetGuardWallRailHoleFootway(nGW,m_bRoadNotSlab);
		else if(nTab == m_nTab7)		//��ű�+����
			pGW = pBri->GetGuardWallRoadRailDike(nGW,m_bRoadNotSlab);
		else if(nTab == m_nTab8)		//�ڰ�����
			pGW = pBri->GetGuardWallRailDetectGravel2(nGW,m_bRoadNotSlab);
	}
	else
	{
		if(nTab == m_nTab1)				//��ȣ�� �� �ߺд�
			pGW = pBri->GetGuardWallRoadGuardFenceAndCentralWall(nGW,m_bRoadNotSlab);
		else if(nTab == m_nTab2)		//������ ��輮
			pGW = pBri->GetGuardWallRoadFootwayBlock(nGW,m_bRoadNotSlab);
		else if(nTab == m_nTab3)		//����
		{
			pDom->SetScaleDim(15);
			pGW = pBri->GetGuardWallRoadFootway(nGW,m_bRoadNotSlab);
		}
		else if(nTab == m_nTab4)		//������ ����
			pGW = pBri->GetGuardWallRoadSoundProof(nGW,m_bRoadNotSlab);
		else if(nTab == m_nTab5)		//�븷�̺�
			pGW = pBri->GetGuardWallRoadRetainingWall(nGW,m_bRoadNotSlab);
		else if(nTab == m_nTab6)		//��������
			pGW = pBri->GetGuardWallRoadHandRail(nGW,m_bRoadNotSlab);
		else if(nTab == m_nTab7)		//��ű�+����
			pGW = pBri->GetGuardWallRoadRailDike(nGW,m_bRoadNotSlab);
	}

	if(pGW == NULL)	return;
	long nHDan = pGW->m_nNumberHDan;
	long nHDanAct = 0;
	double dLenHDan = 0;
	nHDanAct = pBri->GetNumSlabNodeAct(sta, vAng, nHDan,m_bRoadNotSlab);
	dLenHDan = pBri->GetLengthHDanAct(sta, vAng, nHDan,m_bRoadNotSlab);
	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A,m_bRoadNotSlab);
	CTwinVectorArray tvArr;
	pBri->GetTvCrossSection(tvArr, sta, vAng,TRUE,TRUE,TRUE,0,FALSE,TRUE,m_bRoadNotSlab);
	CDRect rect = tvArr.GetRect();
	rect.left = A[nHDanAct].x - 800.0;
	rect.top += 100.0;
	rect.right = A[nHDanAct].x + dLenHDan + 800.0;
	rect.bottom -= 100.0;

	CDomyun Dom(pDom);
	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.DrawCuttingTvArray(&Dom, tvArr, rect);

	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	BOOL bLeft = pGW->IsLeftGuardFence();
	if(nTab==2)
	{
		if(pGW->m_dSlopeUpper > 0)		bLeft = TRUE;
		else							bLeft = FALSE;
	}
	DBStd.m_pDrawCrossSection->DimGuardWall(&Dom, pGW, sta, TRUE, bLeft, !bLeft,m_bRoadNotSlab);
	
	*pDom << Dom;

	pDom->SetCalcExtRect();
	CDRect bound = pDom->GetExtRect();
	pDom->SetLineColor(-1);
	pDom->Point(bound.left, bound.bottom+100.0);

	if(bZoomAll)
		m_DomView.ZoomAll();
	else
		m_DomView.RedrawAll();
}

void CGuardWallDlg::SetDataDefault()
{
	CRcBridgeApp* pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	long nTab = m_ctlTab.GetCurSel();
	long nQtyHDan = pBri->GetQtyHDan(m_bRoadNotSlab);

	for(long nHDan=0; nHDan<nQtyHDan; nHDan++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan, m_bRoadNotSlab);
		if(!pGW)	continue;
		if(m_bRail)
		{
			if(nTab==m_nTab1 && !pGW->IsTypeHDanRailGuardFence())				continue;	//����	
			else if(nTab==m_nTab2 && !pGW->IsTypeHDanRailDetectGravel())		continue;	//�ڰ�����(ö��)
			else if(nTab==m_nTab3 && !pGW->IsTypeHDanRailFootway())				continue;	//����
			else if(nTab==m_nTab4 && !pGW->IsTypeHDanRailFootwayBlock())		continue;	//����+����
			else if(nTab==m_nTab5 && !pGW->IsTypeHDanRailHole())				continue;	//��ű�
			else if(nTab==m_nTab6 && !pGW->IsTypeHDanRailHoleFootway())			continue;	//��ű�+����
			else if(nTab==m_nTab7 && !pGW->IsTypeHDanRoadRailDike())			continue;	//����ũ
			else if(nTab==m_nTab8 && !pGW->IsTypeHDanRailDetectGravel2())		continue;	//�ڰ�����(����)
		}
		else
		{
			if(nTab==m_nTab1 && !pGW->IsTypeHDanRoadTotalGuardFenceAndCentralWall())	continue;	//��ȣ��, �ߺд�
			else if(nTab==m_nTab2 && !pGW->IsTypeHDanRoadFootwayBlock())				continue;	//������ ��輮
			else if(nTab==m_nTab3 && !pGW->IsTypeHDanRoadFootway())						continue;	//����
			else if(nTab==m_nTab4 && !pGW->IsTypeHDanRoadSoundProof())					continue;	//������ ����
			else if(nTab==m_nTab5 && !pGW->IsTypeHDanRoadRetainingWall())				continue;	//�븷�̺�
			else if(nTab==m_nTab6 && !pGW->IsTypeHDanRoadHandRail())					continue;	//��������
			else if(nTab==m_nTab7 && !pGW->IsTypeHDanRoadRailDike())					continue;	//����ũ
		}

		BOOL bRoadNotSlab = FALSE;
		if(pBri->m_bUnderGroundRahmen && !m_bRoadNotSlab)	//���߶���̰� ������ �μӹ��� �ö���� ��쿡��, ��ȣ���� bRoadNotSlab=TRUE ����
			bRoadNotSlab = TRUE;
		pGW->SetDataDefaultGuardWall(pBri->GetThickPave(bRoadNotSlab), bRoadNotSlab);
	}
}
void CGuardWallDlg::SetGridTitle()
{
	long nTab = m_ctlTab.GetCurSel();
	if(nTab == m_nTab3)	// ���α� ����, ö���� ��������
		SetGridTitleFW();			
	else
		SetGridTitleGW();			// ��ȣ��, �ߺд�
}

// ��ȣ��, �ߺд�
void CGuardWallDlg::SetGridTitleGW()
{
	CRcBridgeApp* pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	long nTab = m_ctlTab.GetCurSel();
	long nRows = 0;
	long nCols = 15;

	if(m_bRail)
	{
		if(nTab == m_nTab1)			// ����
		{
			nRows = pBri->GetQtyHDanRailGuardFence(m_bRoadNotSlab) + 1;
		}
		else if(nTab == m_nTab2)	// �ڰ�����
		{
			nRows = pBri->GetQtyHDanRailDetectGravel(m_bRoadNotSlab) + 1;
		}
		else if(nTab == m_nTab4)	// ����+����
		{
			nRows = pBri->GetQtyHDanRailFootwayBlock(m_bRoadNotSlab) + 1;
			nCols = 19;
		}
		else if(nTab == m_nTab5)	// ��ű�
		{
			nRows = pBri->GetQtyHDanRailHole(m_bRoadNotSlab) + 1;
			nCols = 19;
		}
		else if(nTab == m_nTab6)	// ��ű�+����
		{
			nRows = pBri->GetQtyHDanRailHoleFootway(m_bRoadNotSlab) + 1;
			nCols = 19;
		}
		else if(nTab == m_nTab7)	// ����ũ
		{
			nRows = pBri->GetQtyHDanRoadRailDike(m_bRoadNotSlab) + 1;
		}
		else if(nTab == m_nTab8)	// �ڰ�����
		{
			nRows = pBri->GetQtyHDanRailDetectGravel2(m_bRoadNotSlab) + 1;
		}
	}
	else
	{
		if(nTab == m_nTab1)			// ��ȣ��, �ߺд�
		{
			nRows = pBri->GetQtyHDanRoadGuardFence(m_bRoadNotSlab) + pBri->GetQtyHDanRoadCentralWallFull(m_bRoadNotSlab) + pBri->GetQtyHDanRoadCentralWallHalf(m_bRoadNotSlab) + 1;
		}
		else if(nTab == m_nTab2)	// ������ ��輮
		{
			nRows = pBri->GetQtyHDanRoadFootwayBlock(m_bRoadNotSlab) + 1;
		}
		else if(nTab == m_nTab4)	// ������ ���� Sound Proof
		{
			nRows = pBri->GetQtyHDanRoadSoundProof(m_bRoadNotSlab) + 1;
		}
		else if(nTab == m_nTab5)	// �븷�̺�
		{
			nRows = pBri->GetQtyHDanRoadRetainingWall(m_bRoadNotSlab) + 1;
		}
		else if(nTab == m_nTab6)	// �������� Handrail
		{
			nRows = pBri->GetQtyHDanRoadHandRail(m_bRoadNotSlab) + 1;
		}
		else if(nTab == m_nTab7)	// ����ũ
		{
			nRows = pBri->GetQtyHDanRoadRailDike(m_bRoadNotSlab) + 1;
		}
	}
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(48);
	m_Grid.SetColumnWidth(0, 52);
	m_Grid.SetColumnWidth(14, 55);

	m_Grid.SetTextBkColor(CLR_BACK);

	long n=0; for(n=1; n<nRows; n++)
	{
		m_Grid.SetCellType(n, 1, CT_EDIT);
		m_Grid.SetCellType(n, 2, CT_EDIT);
	}

	long nRow = 0;
	CString str = _T("");
	m_Grid.SetTextMatrix(nRow++, 0, _T("����"));
	for(n=0; n<nRows; n++)
	{
		str.Format(_T("%d ����"), n+1);
		m_Grid.SetTextMatrix(nRow++, 0, str);
	}

	long nCol = 1;
	m_Grid.SetTextMatrix(0, nCol++, _T("D"));
	for(n=0; n<3; n++)
	{
		str.Format(_T("H%d"), n+1);
		m_Grid.SetTextMatrix(0, nCol++, str);
	}
	for(n=0; n<5; n++)
	{
		str.Format(_T("W%d"), n+1);
		m_Grid.SetTextMatrix(0, nCol++, str);
	}
	m_Grid.SetTextMatrix(0, nCol++, _T("R1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("R2"));
	m_Grid.SetTextMatrix(0, nCol++, _T("R"));
	m_Grid.SetTextMatrix(0, nCol++, _T("C"));

	long nColForce = nCol;
	str = "P (" + pBri->m_pARcBridgeDataStd->m_szTonf_M + ")";		m_Grid.SetTextMatrix(0, nCol++, str);

	if(m_bRail && (nTab == m_nTab4 || nTab == m_nTab5 || nTab == m_nTab6))	// ����+����, ��ű�, ��ű�+����
	{
		m_Grid.SetTextMatrix(0, nCol++, _T("W6"));
		m_Grid.SetTextMatrix(0, nCol++, _T("W7"));
		m_Grid.SetTextMatrix(0, nCol++, _T("H4"));
		m_Grid.SetTextMatrix(0, nCol++, _T("H5"));
	}

	m_Grid.SetColumnWidth(nColForce, 80);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ����
void CGuardWallDlg::SetGridTitleFW()
{
	CRcBridgeApp* pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	long nRows = 0;
	if(m_bRail) 
		nRows = pBri->GetQtyHDanRailFootway(m_bRoadNotSlab) + 1;
	else
		nRows = pBri->GetQtyHDanRoadFootway(m_bRoadNotSlab) + 1;
	long nCols = 8;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(48);
	m_Grid.SetColumnWidth(0, 52);
	m_Grid.SetColumnWidth(1, 100);
	m_Grid.SetColumnWidth(2, 100);
	m_Grid.SetColumnWidth(3, 110);
	m_Grid.SetColumnWidth(4, 110);

	m_Grid.SetTextBkColor(CLR_BACK);

	long n=0; for(n=1; n<nRows; n++)
	{
		m_Grid.SetCellType(n, 1, CT_COMBO);
		m_Grid.SetCellType(n, 2, CT_COMBO);
	}

	long nRow = 0;
	CString str = _T("");
	m_Grid.SetTextMatrix(nRow++, 0, _T("����"));
	for(n=0; n<nRows; n++)
	{
		str.Format(_T("%d ����"), n+1);
		m_Grid.SetTextMatrix(nRow++, 0, str);
	}

	long nCol = 1;
	m_Grid.SetTextMatrix(0, nCol++, _T("���� ����"));
	m_Grid.SetTextMatrix(0, nCol++, _T("ä�� ����"));
	str = "�����߷�(" + pBri->m_pARcBridgeDataStd->m_szKgf_M3 +")";
	m_Grid.SetTextMatrix(0, nCol++, str);
	str = "ä���߷�(" + pBri->m_pARcBridgeDataStd->m_szKgf_M3 +")";
	m_Grid.SetTextMatrix(0, nCol++, str);
	m_Grid.SetTextMatrix(0, nCol++, _T("H3"));
	m_Grid.SetTextMatrix(0, nCol++, _T("TP"));
	m_Grid.SetTextMatrix(0, nCol++, _T("S(%)"));

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CGuardWallDlg::SetDataInit()
{
	long nTab = m_ctlTab.GetCurSel();
	if(nTab == m_nTab3)	// ����
		SetDataInitFootway();			// ����
	else
		SetDataInitGW();			// ��ȣ��, �ߺд�

}

//��ȣ��, �ߺд�
void CGuardWallDlg::SetDataInitGW()
{
	CRcBridgeApp* pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	SetGridTitleGW();

	CGuardWallRC* pGW = NULL;
	CString str = _T("");
	long nRows = m_Grid.GetRows()-1;
	long nTab = m_ctlTab.GetCurSel();
	for(long nGW=0; nGW<nRows; nGW++)
	{
		if(m_bRail)
		{
			if(nTab == m_nTab1)				//����
				pGW = pBri->GetGuardWallRailGuardFence(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab2)		//�ڰ�����
				pGW = pBri->GetGuardWallRailDetectGravel(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab4)		//����+����
				pGW = pBri->GetGuardWallRailFootwayBlock(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab5)		//��ű�
				pGW = pBri->GetGuardWallRailHole(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab6)		//��ű�+����
				pGW = pBri->GetGuardWallRailHoleFootway(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab7)		//����ũ
				pGW = pBri->GetGuardWallRoadRailDike(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab8)		//�ڰ�����
				pGW = pBri->GetGuardWallRailDetectGravel2(nGW,m_bRoadNotSlab);
		}
		else
		{
			if(nTab == m_nTab1)				//��ȣ�� �� �ߺд�
				pGW = pBri->GetGuardWallRoadGuardFenceAndCentralWall(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab2)		//������ ��輮
				pGW = pBri->GetGuardWallRoadFootwayBlock(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab4)		//������ ����
				pGW = pBri->GetGuardWallRoadSoundProof(nGW);
			else if(nTab == m_nTab5)		//�븷�̺�
				pGW = pBri->GetGuardWallRoadRetainingWall(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab6)		//��������
				pGW = pBri->GetGuardWallRoadHandRail(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab7)		//����ũ
				pGW = pBri->GetGuardWallRoadRailDike(nGW,m_bRoadNotSlab);
		}

		if(pGW == NULL)		continue;

		long nCol = 1;
		m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dDistFrLeft);
		// H1
		if(pGW->m_dH1==0)
		{
			m_Grid.SetItemBkColour(nGW+1, nCol, CLR_READONLY);
			m_Grid.SetItemState(nGW+1, nCol, GVIS_READONLY);
		}
		m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dH1);
		// H2
		if(pGW->m_dH2==0)
		{
			m_Grid.SetItemBkColour(nGW+1, nCol, CLR_READONLY);
			m_Grid.SetItemState(nGW+1, nCol, GVIS_READONLY);
		}
		m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dH2);
		// H3
		m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dH3);
		// W1
		if(pGW->m_dW1==0)
		{
			m_Grid.SetItemBkColour(nGW+1, nCol, CLR_READONLY);
			m_Grid.SetItemState(nGW+1, nCol, GVIS_READONLY);
		}
		m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dW1);
		// W2
		if(pGW->m_dW2==0)
		{
			m_Grid.SetItemBkColour(nGW+1, nCol, CLR_READONLY);
			m_Grid.SetItemState(nGW+1, nCol, GVIS_READONLY);			
		}
		m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dW2);
		// W3
		m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dW3);
		// W4
		if(pGW->m_dW4==0)
		{
			m_Grid.SetItemBkColour(nGW+1, nCol, CLR_READONLY);
			m_Grid.SetItemState(nGW+1, nCol, GVIS_READONLY);
		}
		m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dW4);
		// W5
		if(pGW->m_dW5==0)
		{
			m_Grid.SetItemBkColour(nGW+1, nCol, CLR_READONLY);
			m_Grid.SetItemState(nGW+1, nCol, GVIS_READONLY);			
		}
		m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dW5);
		// R1, R2
		if(nTab==m_nTab2 || nTab==m_nTab4 || nTab==m_nTab5 || nTab==m_nTab7)
		{
			if(pGW->m_dR1==0)
			{
				m_Grid.SetItemBkColour(nGW+1, nCol, CLR_READONLY);
				m_Grid.SetItemState(nGW+1, nCol, GVIS_READONLY);
			}
		}
		m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dR1);
		// R2
		if(nTab==m_nTab2 || nTab==m_nTab4 || nTab==m_nTab5 || nTab==m_nTab7)
		{
			if(pGW->m_dR2==0)
			{
				m_Grid.SetItemBkColour(nGW+1, nCol, CLR_READONLY);
				m_Grid.SetItemState(nGW+1, nCol, GVIS_READONLY);
			}
		}
		m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dR2);
		//R3
		m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dR3);
		// C
		m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dC);
		// P
		m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.3f"), pBri->m_pARcBridgeDataStd->GetValueUnitType(pGW->m_dForce, UNIT_CGS_TONF_M,pBri->m_pARcBridgeDataStd->m_szTonf_M));

		if(pGW->IsTypeHDanRailFootwayBlock() || pGW->IsTypeHDanRailHole() || pGW->IsTypeHDanRailHoleFootway())	// ����+����, ��ű�, ��ű�+����
		{
			m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dW6);
			m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dW7);
			m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dH4);
			m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dH5);
		}
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}
// ����
void CGuardWallDlg::SetDataInitFootway()
{
	CRcBridgeApp* pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	SetGridTitleFW();

	CGuardWallRC* pGW = NULL;

	BOOL bLSD = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	CString str = _T("");
	long nRows = m_Grid.GetRows()-1;

	UpdateData(FALSE);

	for(long nGW=0; nGW<nRows; nGW++)
	{
		long nCol = 1;
		if(m_bRail)
			pGW = pBri->GetGuardWallRailFootway(nGW,m_bRoadNotSlab);
		else
			pGW = pBri->GetGuardWallRoadFootway(nGW,m_bRoadNotSlab);

		if(pGW == NULL)		continue;

		m_Grid.SetTextMatrix(nGW+1, nCol++, pGW->GetStringPave(pGW->m_nTypePave));
		m_Grid.SetTextMatrix(nGW+1, nCol++, pGW->GetStringInnerFill(pGW->m_nTypeInnerFill));

		m_Grid.SetTextMatrix(nGW+1, nCol++, bLSD ? _T("%.7f") : _T("%.1f"), pBri->m_pARcBridgeDataStd->GetValueUnitType(pGW->m_dUnitWeightPave, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3));
		m_Grid.SetTextMatrix(nGW+1, nCol++, bLSD ? _T("%.7f") : _T("%.1f"), pBri->m_pARcBridgeDataStd->GetValueUnitType(pGW->m_dUnitWeightInnerFill, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3));

		m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dH3);
		m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%.0f"), pGW->m_dThickPaveFootway);
		m_Grid.SetTextMatrix(nGW+1, nCol++, _T("%+.2f"), pGW->m_dSlopeUpper*100);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CGuardWallDlg::SetDataSave()
{
	long nTab = m_ctlTab.GetCurSel();
	if(nTab == m_nTab3)
		SetDataSaveFootway();	// ����
	else
		SetDataSaveGW();		// ��ȣ��, �ߺд�, ������ ��輮, ������ ����, �븷�̺�, ��������		
}

void CGuardWallDlg::SetDataSaveGW()
{
	CGuardWallRC* pGW = NULL;
	CRcBridgeApp* pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	long nTab = m_ctlTab.GetCurSel();
	long nRows = m_Grid.GetRows()-1;
	for(long nGW=0; nGW<nRows; nGW++)
	{
		if(m_bRail)
		{
			if(nTab == m_nTab1)				//����
				pGW = pBri->GetGuardWallRailGuardFence(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab2)		//�ڰ�����
				pGW = pBri->GetGuardWallRailDetectGravel(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab4)		//����+����
				pGW = pBri->GetGuardWallRailFootwayBlock(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab5)		//��ű�
				pGW = pBri->GetGuardWallRailHole(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab6)		//��ű�+����
				pGW = pBri->GetGuardWallRailHoleFootway(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab7)		//����ũ
				pGW = pBri->GetGuardWallRoadRailDike(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab8)		//�ڰ�����
				pGW = pBri->GetGuardWallRailDetectGravel2(nGW,m_bRoadNotSlab);
		}
		else
		{
			if(nTab == m_nTab1)				//��ȣ�� �� �ߺд�
				pGW = pBri->GetGuardWallRoadGuardFenceAndCentralWall(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab2)		//������ ��輮
				pGW = pBri->GetGuardWallRoadFootwayBlock(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab4)		//������ ����
				pGW = pBri->GetGuardWallRoadSoundProof(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab5)		//�븷�̺�
				pGW = pBri->GetGuardWallRoadRetainingWall(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab6)		//��������
				pGW = pBri->GetGuardWallRoadHandRail(nGW,m_bRoadNotSlab);
			else if(nTab == m_nTab7)		//����ũ
				pGW = pBri->GetGuardWallRoadRailDike(nGW,m_bRoadNotSlab);
		}
		if(pGW == NULL)		continue;

		long nCol = 1;
		pGW->m_dDistFrLeft = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
		pGW->m_dH1 = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
		pGW->m_dH2 = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
		pGW->m_dH3 = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
		pGW->m_dW1 = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
		pGW->m_dW2 = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
		pGW->m_dW3 = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
		pGW->m_dW4 = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
		pGW->m_dW5 = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
		pGW->m_dR1 = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
		pGW->m_dR2 = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
		pGW->m_dR3 = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
		pGW->m_dC = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);

		pGW->m_dForce = pBri->m_pARcBridgeDataStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nGW+1, nCol++),pBri->m_pARcBridgeDataStd->m_szTonf_M,UNIT_CGS_TONF_M);

		if(pGW->IsTypeHDanRailFootwayBlock() || pGW->IsTypeHDanRailHole() || pGW->IsTypeHDanRailHoleFootway())	// ����+����, ��ű�, ��ű�+����
		{
			pGW->m_dW6 = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
			pGW->m_dW7 = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
			pGW->m_dH4 = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
			pGW->m_dH5 = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
		}
	}
}
void CGuardWallDlg::SetDataSaveFootway()
{
	CGuardWallRC* pGW = NULL;
	CRcBridgeApp* pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	CString str = _T("");
	long nRows = m_Grid.GetRows()-1;

	for(long nGW=0; nGW<nRows; nGW++)
	{
		long nCol = 1;
		if(m_bRail)
			pGW = pBri->GetGuardWallRailFootway(nGW,m_bRoadNotSlab);
		else
			pGW = pBri->GetGuardWallRoadFootway(nGW,m_bRoadNotSlab);

		pGW->m_nTypePave = pGW->GetNumPave(m_Grid.GetTextMatrix(nGW+1, nCol++));
		pGW->m_nTypeInnerFill = pGW->GetNumInnerFill(m_Grid.GetTextMatrix(nGW+1, nCol++));
		pGW->m_dUnitWeightPave = pBri->m_pARcBridgeDataStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nGW+1, nCol++),pBri->m_pARcBridgeDataStd->m_szKgf_M3,UNIT_CGS_KGF_M3);
		pGW->m_dUnitWeightInnerFill = pBri->m_pARcBridgeDataStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nGW+1, nCol++),pBri->m_pARcBridgeDataStd->m_szKgf_M3,UNIT_CGS_KGF_M3);
		pGW->m_dH3 = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
		pGW->m_dThickPaveFootway = m_Grid.GetTextMatrixDouble(nGW+1, nCol++);
		pGW->m_dSlopeUpper = m_Grid.GetTextMatrixDouble(nGW+1, nCol++)/100;
		pGW->m_dW3 = pBri->GetLengthHDan(pGW->m_nNumberHDan, m_bRoadNotSlab);
	}
}

void CGuardWallDlg::OnOK() 
{
	SetDataSave();
	
	CDialog::OnOK();
}

void CGuardWallDlg::OnButtonDefault() 
{
	if(AfxMessageBox("���� �Է� ������ �����Ͱ� ��� �⺻������ �ٲ�ϴ�. ��� �Ͻðڽ��ϱ�?  ", MB_YESNO|MB_ICONQUESTION)==IDNO)
		return;

	SetDataDefault();
	SetDataInit();

	DrawInputDomyunView();
}

void CGuardWallDlg::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetFocus();
	SetFocusCellByType();
	DrawInputDomyunView();
	
	return;
}

void CGuardWallDlg::SetFocusCellByType() 
{
	CRcBridgeApp* pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	m_Grid.DeleteAllItems();
	SetDataInit();

	long nRow = 1;
	m_Grid.SetFocusCell(nRow, 1);
}

void CGuardWallDlg::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataSave();
	
	return;
}

