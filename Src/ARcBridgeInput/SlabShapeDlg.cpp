// SlabShapeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "SlabShapeDlg.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSlabShapeDlg dialog


CSlabShapeDlg::CSlabShapeDlg(CARcBridgeInputStd* pIn, CWnd* pParent /*=NULL*/)
	: CDialog(CSlabShapeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSlabShapeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pIn = pIn;
}


void CSlabShapeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSlabShapeDlg)
	DDX_Control(pDX, IDC_HYPER_HELP, m_hyperHelp);
	DDX_Control(pDX, IDC_BUTTON_RECOMMAND, m_btDefault);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_DomControl(pDX, IDC_DOM, m_DomView);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSlabShapeDlg, CDialog)
	//{{AFX_MSG_MAP(CSlabShapeDlg)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMAND, OnButtonDefault)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSlabShapeDlg message handlers

BOOL CSlabShapeDlg::PreTranslateMessage(MSG* pMsg) 
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
		if(next.col==1)
		{
			strArr.Add(_T("일반"));
			strArr.Add(_T("켄틸레버"));
			m_Grid.SetComboString(strArr);
		}
		if(next.col==2)
		{
			strArr.Add(_T("없음"));
			strArr.Add(_T("직선"));
			strArr.Add(_T("곡선"));
			m_Grid.SetComboString(strArr);
		}
	}

	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);	
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSlabShapeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_DomView.SetOwnerMouseMode(FALSE);
	m_DomView.m_bEnablePopupMenu = TRUE;
	m_DomView.SetCrossLength(0);


	COLORREF clrLink = RGB(0x00, 0x00, 0xFF);
	m_hyperHelp.SetColors(clrLink, clrLink, clrLink);
	m_hyperHelp.SetURL(m_pIn->GetManualPath());
	m_hyperHelp.SetTipText(_T("현재 입력창에 대한 도움말을 표시합니다."));

	CRcBridgeApp* pBri = m_pIn->GetRcBridge();
	if(!pBri->m_bSSDlgUsed)
		SetDataDefault();
	SetDataInit();

	m_Grid.SetFocusCell(1,1);

	DrawInputDomyunView();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSlabShapeDlg::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	
	int nRow = pGridView->iRow;
	int nCol = pGridView->iColumn;

	SetDataSave();

	CRcBridgeApp* pBri = m_pIn->GetRcBridge();
	CString str1 = m_Grid.GetTextMatrix(nRow, 1);
	CString str2 = m_Grid.GetTextMatrix(nRow, 2);
	if(nCol==1)
	{
		if(str1==_T("일반"))
		{
			pBri->m_dCantilH1[nRow-1] = 0.0;
			pBri->m_dCantilH2[nRow-1] = 0.0;
			pBri->m_dCantilB[nRow-1] = 0.0;
			pBri->m_dRE2[nRow-1] = 0.0;
			pBri->m_dTCW2[nRow-1] = 0.0;
			pBri->m_dTCH2[nRow-1] = 0.0;
		}
		else
		{
			pBri->m_dCantilH1[nRow-1] = 320.0;
			pBri->m_dCantilH2[nRow-1] = 160.0;
			pBri->m_dCantilB[nRow-1] = 960.0;
			if(str2==_T("직선"))
			{
				pBri->m_dTCW2[nRow-1] = 160.0;
				pBri->m_dTCH2[nRow-1] = 160.0;
			}
			if(str2==_T("곡선"))
				pBri->m_dRE2[nRow-1] = 160.0;
		}
	}
	if(nCol==2)
	{
		if(str2==_T("직선"))
		{
			pBri->m_dRE1[nRow-1] = 0.0;
			pBri->m_dRE2[nRow-1] = 0.0;
			pBri->m_dTCW1[nRow-1] = 160.0;
			pBri->m_dTCH1[nRow-1] = 160.0;
			pBri->m_dTCW2[nRow-1] = str1==_T("일반") ? 0.0 : 160.0;
			pBri->m_dTCH2[nRow-1] = str1==_T("일반") ? 0.0 : 160.0;
		}
		if(str2==_T("곡선"))
		{
			pBri->m_dRE1[nRow-1] = 160.0;
			pBri->m_dRE2[nRow-1] = str1==_T("일반") ? 0.0 : 160.0;
			pBri->m_dTCW1[nRow-1] = 0.0;
			pBri->m_dTCH1[nRow-1] = 0.0;
			pBri->m_dTCW2[nRow-1] = 0.0;
			pBri->m_dTCH2[nRow-1] = 0.0;
		}
	}

	if(nCol==1 || nCol==2)
	{
		if(str2==_T("없음"))
		{
			pBri->m_dDH[nRow-1] = 0.0;
			pBri->m_dRE1[nRow-1] = 0.0;
			pBri->m_dRE2[nRow-1] = 0.0;
			pBri->m_dTCW1[nRow-1] = 0.0;
			pBri->m_dTCH1[nRow-1] = 0.0;
			pBri->m_dTCW2[nRow-1] = 0.0;
			pBri->m_dTCH2[nRow-1] = 0.0;
		}
		else
			pBri->m_dDH[nRow-1] = 165.0;
	}

	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CSlabShapeDlg::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;

	int nRow = pGridView->iRow;
	int nCol = pGridView->iColumn;

	CString str = m_Grid.GetTextMatrix(0, nCol);
	if(str.Left(1)==_T("R"))
		str += (_T(" = ") + m_Grid.GetTextMatrix(nRow, nCol));
	if(str.Left(2)==_T("CW") || str.Left(2)==_T("CH"))
		str.Format(_T("CW%s X CH%s"), str.Right(1), str.Right(1));

//	DrawInputDomyunView(FALSE);
	m_DomView.STMakeCursor(str);
}

void CSlabShapeDlg::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
	DrawInputDomyunView(FALSE);
}

void CSlabShapeDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_DomView.GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(25);

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = m_pIn->GetRcBridgeRebar();
	CJijum* pJ = pBri->GetJijum(0);
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	CCellID cID = m_Grid.GetFocusCell();
	int nRow = cID.row;
	BOOL bLeft = nRow!=2;
	double dSta = pBri->GetStationOnJijum(0, 0);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		dSta += pJ->m_dLengthUgan[1];
	CDPoint vAng = CDPoint(0, 1);

	CTwinVectorArray tvArr, tvArrHu;
	CDPoint A[30];
	pBri->GetXySlabUpperAct(dSta, vAng, A);
	pBri->GetTvCrossSection(tvArr, dSta, vAng, FALSE, TRUE, FALSE);
	pBri->GetTvCrossHunch(tvArrHu, dSta, vAng, TRUE, 0);
	tvArr.AddFromTvArray(tvArrHu);

	CDRect rect = tvArr.GetRect();
	if(bLeft)
	{
		rect.left -= 100.0;
		rect.right = rect.left + pBri->m_dCantilB[0] + 1500.0;
	}
	else
	{
		rect.right += 100.0;
		rect.left = rect.right - (pBri->m_dCantilB[0] + 1500.0);
	}
	rect.top += 100.0;
	rect.bottom -= 100.0;

	CDomyun Dom(pDom);

	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.DrawCuttingTvArray(&Dom, tvArr, rect);

	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawCrossSection->DimCrossSectionSlabEnd(&Dom, bLeft, dSta, vAng, TRUE);
	
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

void CSlabShapeDlg::SetDataDefault()
{
	CRcBridgeApp* pBri = m_pIn->GetRcBridge();

	int i=0; for(i=0; i<2; i++)
	{
		if(i==0)
		{
			pBri->m_nSlabChemfer[i] = pBri->m_nSeparBri==2 ? 0 : 2;	// 라운드 처리
			pBri->m_dDH[i] = pBri->m_nSeparBri==2 ? 0.0 : 165.0;
		}
		else
		{
			pBri->m_nSlabChemfer[i] = pBri->m_nSeparBri==1 ? 0 : 2;	// 라운드 처리
			pBri->m_dDH[i] = pBri->m_nSeparBri==1 ? 0.0 : 165.0;
		}
		pBri->m_bCantilSlab[i] = FALSE;
		pBri->m_dCantilH1[i] = 0.0;
		pBri->m_dCantilH2[i] = 0.0;
		pBri->m_dCantilB[i] = 0.0;
		pBri->m_dRE1[i] = 160.0;	// 슬래브 하부 끝단 라운드 처리시 반지름, [0]:좌, [1]:우
		pBri->m_dRE2[i] = 0.0;		// 슬래브 상부 끝단 라운드 처리시 반지름, [0]:좌, [1]:우
		pBri->m_dTCW1[i] = 0.0;		// 슬래브 하부 끝단 헌치 처리시 헌치 폭
		pBri->m_dTCH1[i] = 0.0;		// 슬래브 하부 끝단 헌치 처리시 헌치 높이
		pBri->m_dTCW2[i] = 0.0;		// 슬래브 상부 끝단 헌치 처리시 헌치 폭
		pBri->m_dTCH2[i] = 0.0;		// 슬래브 상부 끝단 헌치 처리시 헌치 높이
	}
}

void CSlabShapeDlg::SetGridTitle()
{
	int nRows = 3;
	int nCols = 13;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetColumnResize(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(44);
	m_Grid.SetColumnWidth(1, 80);
	m_Grid.SetColumnWidth(2, 80);
	m_Grid.SetTextBkColor(CLR_BACK);

	int i=0; for(i=1; i<3; i++)
	{
		m_Grid.SetCellType(1, i, CT_COMBO);
		m_Grid.SetCellType(2, i, CT_COMBO);
	}

	int nRow = 0;
	CString str = _T("");
	m_Grid.SetTextMatrix(nRow++, 0, _T("구분"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("좌측"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("우측"));

	int nCol = 1;
	m_Grid.SetTextMatrix(0, nCol++, _T("끝단 형태"));
	m_Grid.SetTextMatrix(0, nCol++, _T("모따기 형태"));
	m_Grid.SetTextMatrix(0, nCol++, _T("H1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("H2"));
	m_Grid.SetTextMatrix(0, nCol++, _T("B"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DH"));
	m_Grid.SetTextMatrix(0, nCol++, _T("R1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("R2"));
	m_Grid.SetTextMatrix(0, nCol++, _T("CW1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("CH1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("CW2"));
	m_Grid.SetTextMatrix(0, nCol++, _T("CH2"));

	m_Grid.SetRedraw(TRUE,TRUE);
	m_Grid.ExpandColumnsToFit();
}

void CSlabShapeDlg::SetDataInit()
{
	SetGridTitle();

	CRcBridgeApp* pBri = m_pIn->GetRcBridge();
	CString str = _T("");

	int i=0; for(i=0; i<2; i++)
	{
		int nCol = 1;
		str = pBri->m_bCantilSlab[i] ? _T("켄틸레버") : _T("일반");
		m_Grid.SetTextMatrix(i+1, nCol++, str);
		if(pBri->m_nSlabChemfer[i]==0)
			str = _T("없음");
		if(pBri->m_nSlabChemfer[i]==1)
			str = _T("직선");
		if(pBri->m_nSlabChemfer[i]==2)
			str = _T("곡선");
		m_Grid.SetTextMatrix(i+1, nCol++, str);
		m_Grid.SetTextMatrix(i+1, nCol++, _T("%.f"), pBri->m_dCantilH1[i]);
		m_Grid.SetTextMatrix(i+1, nCol++, _T("%.f"), pBri->m_dCantilH2[i]);
		m_Grid.SetTextMatrix(i+1, nCol++, _T("%.f"), pBri->m_dCantilB[i]);
		m_Grid.SetTextMatrix(i+1, nCol++, _T("%.f"), pBri->m_dDH[i]);
		m_Grid.SetTextMatrix(i+1, nCol++, _T("%.f"), pBri->m_dRE1[i]);
		m_Grid.SetTextMatrix(i+1, nCol++, _T("%.f"), pBri->m_dRE2[i]);
		m_Grid.SetTextMatrix(i+1, nCol++, _T("%.f"), pBri->m_dTCW1[i]);
		m_Grid.SetTextMatrix(i+1, nCol++, _T("%.f"), pBri->m_dTCH1[i]);
		m_Grid.SetTextMatrix(i+1, nCol++, _T("%.f"), pBri->m_dTCW2[i]);
		m_Grid.SetTextMatrix(i+1, nCol++, _T("%.f"), pBri->m_dTCH2[i]);

		if(pBri->m_bCantilSlab[i])
		{
			int j=0; for(j=0; j<10; j++)
			{
				if(j==3 || j==4 || j==6 || j==7) continue;
				m_Grid.SetItemState(i+1, j+3, m_Grid.GetItemState(i+1, j+3) & ~GVIS_READONLY);
				m_Grid.SetItemBkColour(i+1, j+3);
				m_Grid.SetItemFgColour(i+1, j+3);
			}
		}
		else
		{
			int j=0; for(j=0; j<10; j++)
			{
				if(j==3 || j==4 || j==6 || j==7) continue;
				m_Grid.SetItemState(i+1, j+3, m_Grid.GetItemState(i+1, j+3) | GVIS_READONLY);
				m_Grid.SetItemBkColour(i+1, j+3, CLR_READONLY);
				m_Grid.SetItemFgColour(i+1, j+3, RGB(128, 128, 128));
			}
		}

		if(pBri->m_nSlabChemfer[i]==0)
		{
			int j=0; for(j=0; j<6; j++)
			{
				m_Grid.SetItemState(i+1, j+7, m_Grid.GetItemState(i+1, j+7) | GVIS_READONLY);
				m_Grid.SetItemBkColour(i+1, j+7, CLR_READONLY);
				m_Grid.SetItemFgColour(i+1, j+7, RGB(128, 128, 128));
			}
		}
		if(pBri->m_nSlabChemfer[i]==1)
		{
			int j=0; for(j=0; j<2; j++)
			{
				m_Grid.SetItemState(i+1, j+7, m_Grid.GetItemState(i+1, j+7) | GVIS_READONLY);
				m_Grid.SetItemBkColour(i+1, j+7, CLR_READONLY);
				m_Grid.SetItemFgColour(i+1, j+7, RGB(128, 128, 128));
			}
			for(j=0; j<4; j++)
			{
				if(!pBri->m_bCantilSlab[i] && j>=2) continue;
				m_Grid.SetItemState(i+1, j+9, m_Grid.GetItemState(i+1, j+9) & ~GVIS_READONLY);
				m_Grid.SetItemBkColour(i+1, j+9);
				m_Grid.SetItemFgColour(i+1, j+9);
			}
		}
		if(pBri->m_nSlabChemfer[i]==2)
		{
			int j=0; for(j=0; j<2; j++)
			{
				if(!pBri->m_bCantilSlab[i] && j==1) continue;
				m_Grid.SetItemState(i+1, j+7, m_Grid.GetItemState(i+1, j+7) & ~GVIS_READONLY);
				m_Grid.SetItemBkColour(i+1, j+7);
				m_Grid.SetItemFgColour(i+1, j+7);
			}
			for(j=0; j<4; j++)
			{
				m_Grid.SetItemState(i+1, j+9, m_Grid.GetItemState(i+1, j+9) | GVIS_READONLY);
				m_Grid.SetItemBkColour(i+1, j+9, CLR_READONLY);
				m_Grid.SetItemFgColour(i+1, j+9, RGB(128, 128, 128));
			}
		}
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CSlabShapeDlg::SetDataSave()
{
	CRcBridgeApp* pBri = m_pIn->GetRcBridge();
	CString str = _T("");

	int i=0; for(i=0; i<2; i++)
	{
		int nCol = 1;
		str = m_Grid.GetTextMatrix(i+1, nCol++);
		pBri->m_bCantilSlab[i] = str==_T("켄틸레버");
		str = m_Grid.GetTextMatrix(i+1, nCol++);
		if(str==_T("없음"))
			pBri->m_nSlabChemfer[i] = 0;
		if(str==_T("직선"))
			pBri->m_nSlabChemfer[i] = 1;
		if(str==_T("곡선"))
			pBri->m_nSlabChemfer[i] = 2;
		pBri->m_dCantilH1[i] = m_Grid.GetTextMatrixDouble(i+1, nCol++);
		pBri->m_dCantilH2[i] = m_Grid.GetTextMatrixDouble(i+1, nCol++);
		pBri->m_dCantilB[i] = m_Grid.GetTextMatrixDouble(i+1, nCol++);
		pBri->m_dDH[i] = m_Grid.GetTextMatrixDouble(i+1, nCol++);
		pBri->m_dRE1[i] = m_Grid.GetTextMatrixDouble(i+1, nCol++);
		pBri->m_dRE2[i] = m_Grid.GetTextMatrixDouble(i+1, nCol++);
		pBri->m_dTCW1[i] = m_Grid.GetTextMatrixDouble(i+1, nCol++);
		pBri->m_dTCH1[i] = m_Grid.GetTextMatrixDouble(i+1, nCol++);
		pBri->m_dTCW2[i] = m_Grid.GetTextMatrixDouble(i+1, nCol++);
		pBri->m_dTCH2[i] = m_Grid.GetTextMatrixDouble(i+1, nCol++);
	}
}

void CSlabShapeDlg::OnOK() 
{
	SetDataSave();
	
	CDialog::OnOK();
}

void CSlabShapeDlg::OnButtonDefault() 
{
	if(AfxMessageBox("현재 입력 상자의 데이터가 모두 기본값으로 바뀝니다. 계속 하시겠습니까?  ", MB_YESNO|MB_ICONQUESTION)==IDNO)
		return;

	SetDataDefault();
	SetDataInit();

	DrawInputDomyunView();
}
