// InputDialogBase.cpp : implementation file
//

#include "stdafx.h"
#include "InputDialogBase.h"

#include "ARcBridgeInputStd.h"
#include "..\..\ARcBridge\MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputDialogBase dialog


CInputDialogBase::CInputDialogBase(UINT nID, CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(nID, pParent)
{
	//{{AFX_DATA_INIT(CInputDialogBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nIDD					= nID;
	m_nMenuID				= -1;
	m_pIn					= NULL;
	m_bUsedFlag				= FALSE;
	m_bGL					= FALSE;
	m_bUbicon				= FALSE;
	m_bClickDefaultButton	= FALSE;
	m_dScaleOffset			= GAP_SCALE;
	m_dScaleDefault			= RCBRI_DEFAULT_SCALE;
	m_nIDGrid				= IDC_GRID;				// 포커스 아이템 체크시 사용 될 그리드 
	m_nRowFocusItemTitle	= 0;	// 포커스 아이템 제목이 있는 열
	m_bDesignLsd			= FALSE;
	m_nTab			= 0;
}


void CInputDialogBase::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputDialogBase)
	DDX_Control(pDX, IDC_BUTTON_PREV, m_btnPrev);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_BUTTON_RECOMMAND, m_btnDefault);
	DDX_Control(pDX, IDC_HYPER_HELP, m_hyperHelp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputDialogBase, CXTResizeDialog)
	//{{AFX_MSG_MAP(CInputDialogBase)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_PREV, OnButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMAND, OnButtonDefault)
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputDialogBase message handlers

BOOL CInputDialogBase::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	if(IsPrintPreview()) return TRUE;

	CMainFrame *pFrm = dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	m_pView = (CDomyunGLView *)pFrm->m_pView;
	
	if(m_bUbicon)
	{
		pFrm->SendMessage(WM_SWITCH_VIEW, VIEWID_UBICON, m_nMenuID);
	}
	else
	{
		pFrm->SendMessage(WM_SWITCH_VIEW, VIEWID_DEFAULT, m_nMenuID);
		if(m_pView)
			m_pView->SetGL(m_bGL);
	}
	

	SetResize(IDC_BUTTON_APPLY,	   SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_NEXT,	   SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_PREV,	   SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_RECOMMAND,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BITMAP_HELPICON,	 SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_HYPER_HELP,	   SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	m_pIn->SetComment(_T(""));

	// 빠른 선형 검색 ////////////////////
	CRcBridgeApp *pBri	= m_pIn->GetRcBridge();
	if(pBri) 
	{
		pBri->SetSearchIP();
		// 빠른 선형 검색 ////////////////////

		pBri->ResetFlagForRecoverVertSection();
		//m_bDesignLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	}

	m_bDesignLsd = m_pIn->m_pARcBridgeDataStd->IsLsdDesign();

	OnPreInitDialog();

	//if(!IsUsedFlag())
	SetDataDefault();
	SetDataInit();
	DrawInputDomyunView(TRUE);

	if(GetDlgItem(IDC_GRID)) 
	{
		CGridCtrlMng *pGrid = (CGridCtrlMng *)GetDlgItem(IDC_GRID);
		if(pGrid->GetSafeHwnd())
		{
			pGrid->ResetSelection();
			pGrid->SetFocusCell(pGrid->GetFixedRowCount(),pGrid->GetFixedColumnCount());			
		}
	}

	SetUsedFlag(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputDialogBase::OnPaint() 
{
	CPaintDC dcPaint(this); // device context for painting
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint, rc);

	dc.FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
	dc.Draw3dRect(rc, GetXtremeColor(XPCOLOR_FRAME), GetXtremeColor(XPCOLOR_FRAME));	
}

void CInputDialogBase::DrawInputDomyunView(BOOL bZoomAll)
{
	m_pIn->m_pDoc->SetModifiedFlag();

	if(m_bUbicon)
	{
		if(bZoomAll)
		{
			m_pIn->GetUbRenderer()->ZoomAll();
		}
		else
		{
			m_pIn->GetUbRenderer()->InvalidateWnd();
		}
	}
	else
	{
		if(!m_pView) return;
		if(bZoomAll)
		{
			if(m_pView->IsGL()) 
			{
				m_pView->ZoomPlusNegaExt(FALSE,0.3);
			}
			else
			{
				m_pIn->DrawMarginPoint(m_pView->GetDomyun());
				m_pView->ZoomAll();
			}
		}
		else
		{
			m_pView->RedrawAll();
		}
	}
}

void CInputDialogBase::OnButtonNext()
{	
	if(IsPrintPreview()) return;

	UpdateData();
	SetDataSave();

	if(!m_bUbicon)
	{
		if(m_pView)
		{
			m_pView->GetDomyun()->ClearEtt(TRUE, FALSE);
			m_pView->RedrawAll();
		}
	}

	m_pIn->MoveNextDialog();
}

void CInputDialogBase::OnButtonPrev()
{
	if(IsPrintPreview()) return;

	UpdateData();
	SetDataSave();

	if(!m_bUbicon)
	{
		if(m_pView)
		{
			m_pView->GetDomyun()->ClearEtt(TRUE, FALSE);
			m_pView->RedrawAll();
		}
	}
	
	m_pIn->MovePrevDialog();
}

void CInputDialogBase::OnButtonApply()
{
	m_bClickApplyButton	= TRUE;
	UpdateData();
	SetDataSave();
	SetDataInit();

	DrawInputDomyunView(FALSE);
	m_bClickApplyButton	= FALSE;
}

void CInputDialogBase::OnButtonDefault()
{
	if(AfxMessageBox("현재 입력 상자의 데이터가 모두 기본값으로 바뀝니다. 계속 하시겠습니까?  ", MB_YESNO | MB_ICONQUESTION)==IDNO)
		return;

	m_bClickDefaultButton	= TRUE;
	SetDataDefault();
	SetDataInit();

	DrawInputDomyunView(FALSE);

	m_bClickDefaultButton	= FALSE;
}

void CInputDialogBase::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	SetDataSave();
	if(IsRedrawInputApply())
		DrawInputDomyunView(FALSE);
}

void CInputDialogBase::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	if(GetDlgItem(IDC_GRID)) 
	{
		NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)pNotifyStruct;	
		int nRow = pGridView->iRow;
		int nCol = pGridView->iColumn;
		OnCursorChange(nRow, nCol);		
	}
}

void CInputDialogBase::OnSize(UINT nType, int cx, int cy) 
{
	long i = 0; for(i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTResizeItem *pItem	= m_arrItems.GetAt(i);
		if(!IsWindow(pItem->m_pWnd->m_hWnd)) return;
	}
	CXTResizeDialog::OnSize(nType, cx, cy);

}

BOOL CInputDialogBase::PreTranslateMessage(MSG* pMsg) 
{
	if(GetDlgItem(IDC_GRID)) 
	{
		if(((CGridCtrlMng *)GetDlgItem(IDC_GRID))->TranslateMsg(pMsg))
			return ((CGridCtrlMng *)GetDlgItem(IDC_GRID))->PreTranslateMessage(pMsg);		
	}

	//자동으로 focus되는 소스임 by 신종화-------------------------------------------------------------//
	CWnd *pWnd = FromHandle(pMsg->hwnd);
	TCHAR szClass [256];
	::GetClassName (pMsg->hwnd, szClass, 255);
	CString strClass = szClass;
	if(pWnd && strClass=="Edit" && pWnd->GetFocus() != pWnd && pMsg->message == WM_LBUTTONDOWN)
	{	
		CXTResizeDialog::PreTranslateMessage(pMsg);
		((CEdit *)pWnd)->SetSel(0,-1);
		return TRUE;
	}
	//-------------------------------------------------------------------------------------------------//

	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

void CInputDialogBase::SetPaneTitle()
{
	UINT nMenuID = GetMenuID();
	AfxGetMainWnd()->SendMessage(WM_USER+10, nMenuID);
}

// 컨트롤들을 xyMove만큼 이동시킴
// nspeed 이동 속도
// nBountMount 이동시 바운드 되는 비율.
// void CInputDialogBase::MoveControlGroup(CDoubleArray &arrID, CPoint xyMove, long nSpeed, long nDelay, double dBoundRate)
// {
// 	CPoint xyTotalVector(0, 0);
// 	CPoint xyVector(0, 0);
// 	
// 	CRect rect;
// 	long nX = xyMove.x > 0 ? 1 : -1;
// 	long nY = xyMove.y > 0 ? 1 : -1;
// 
// 	// 바운드 위치 까지 옮김.
// 	CPoint xyBound((int)(xyMove.x + ((double)xyMove.x * dBoundRate)), (int)(xyMove.y + ((double)xyMove.y * dBoundRate)));
// 	BOOL bArriveBoundX = FALSE;
// 	BOOL bArriveBoundY  = FALSE;
// 
// 	while(!bArriveBoundX || !bArriveBoundY)
// 	{
// 		xyTotalVector.x += !bArriveBoundX ? nSpeed*nX : 0;
// 		xyTotalVector.y += !bArriveBoundY ? nSpeed*nY : 0;
// 		xyVector.x = !bArriveBoundX ? nSpeed*nX : 0;
// 		xyVector.y = !bArriveBoundY ? nSpeed*nY : 0;
// 
// 		if(abs(xyBound.x) <= abs(xyTotalVector.x)) bArriveBoundX = TRUE;
// 		if(abs(xyBound.y) <= abs(xyTotalVector.y)) bArriveBoundY = TRUE;
// 		Sleep(nDelay);
// 		
// 
// 		long i = 0; for(i = 0; i < arrID.GetSize(); i++)
// 		{
// 			GetDlgItem((long)arrID[i])->GetWindowRect(&rect);
// 			ScreenToClient(rect);
// 			GetDlgItem((long)arrID[i])->SetWindowPos(NULL, rect.left+xyVector.x, rect.top+xyVector.y, rect.Width(), rect.Height(), NULL);
// 		}
// 	}
// 
// 	// 원래 위치 까지 옮김
// 	nX = -nX;
// 	nY = -nY;
// 	BOOL bFinishMoveX = FALSE;
// 	BOOL bFinishMoveY = FALSE;
// 
// 	while(!bFinishMoveX || !bFinishMoveY)
// 	{
// 		xyTotalVector.x += !bFinishMoveX ? nSpeed*nX : 0;
// 		xyTotalVector.y += !bFinishMoveY ? nSpeed*nY : 0;
// 		xyVector.x = !bFinishMoveX ? nSpeed*nX : 0;
// 		xyVector.y = !bFinishMoveY ? nSpeed*nY : 0;
// 
// 
// 		if(abs(xyMove.x) >= abs(xyTotalVector.x)) bFinishMoveX = TRUE;
// 		if(abs(xyMove.y) >= abs(xyTotalVector.y)) bFinishMoveY = TRUE;
// 		Sleep(nDelay);
// 
// 		long i = 0; for(i = 0; i < arrID.GetSize(); i++)
// 		{
// 			GetDlgItem((long)arrID[i])->GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			GetDlgItem((long)arrID[i])->SetWindowPos(NULL, rect.left+xyVector.x, rect.top+xyVector.y, rect.Width(), rect.Height(), NULL);
// 		}
// 	}
// 
// 	// 조금 벗어난 위치를 보정
// 	CDPoint xyOver(CDPoint(abs(xyTotalVector.x-xyBound.x), abs(xyTotalVector.y-xyBound.y)));
// 	nX = -nX;
// 	nY = -nY;
// 	xyOver.x *= nX;
// 	xyOver.y *= nY;
// 	long i = 0; for(i = 0; i < arrID.GetSize(); i++)
// 	{
// 		GetDlgItem((long)arrID[i])->GetWindowRect(rect);
// 		ScreenToClient(rect);
// 		GetDlgItem((long)arrID[i])->SetWindowPos(NULL, (int)(rect.left+xyOver.x), (int)(rect.top+xyOver.y), rect.Width(), rect.Height(), NULL);
// 
// 		// 보정하고 나서 혹시나 컨트롤들이 지저분하게 그려져있는 경우을 생각해서 다시 그려줌
// 		GetDlgItem((long)arrID[i])->RedrawWindow();
// 	}
// }

void CInputDialogBase::OnCursorChange(long nRow, long nCol)
{
	if(!m_bUbicon)
	{
		if(m_pView)
			m_pView->GetDomyun()->DeleteCursor();
	}
}

void CInputDialogBase::SetHyperHelp(CString strPath)
{
	COLORREF clrLink = RGB(0x00, 0x00, 0xFF);
//	m_hyperHelp.SetInitialize();
	m_hyperHelp.SetColors(clrLink, clrLink, clrLink);
	m_hyperHelp.EnableToolTips(FALSE);
	m_hyperHelp.SetBackColor(GetXtremeColor(COLOR_3DFACE));
	m_hyperHelp.SetURL(strPath);
}

// 현재 다이얼로그에서 dwDefault의 권고안이 진행 되었는지 체크함.
BOOL CInputDialogBase::IsDefaulted(long nBri, DWORD dwDefault)
{
	CRcBridgeData *pBri	= m_pIn->m_pARcBridgeDataStd->GetBridge(nBri);
	if(!pBri) return FALSE;
	
	BOOL bClickDefaultButton	= IsClickDefaultButton(nBri);
	BOOL bIsDefaulted			= pBri->IsDefaulted(GetIDD(), dwDefault);

	if(bClickDefaultButton)
	{
		
	}
	else
	{
		if(IsFirst(nBri)) bIsDefaulted	= FALSE;
	}

	return bIsDefaulted;
}

// 완천히 초기화 상태인지?
// 단지 플래그가 0x00000000인지 검사한다. (편의를 위해 필요할 때만 사용함)
BOOL CInputDialogBase::IsFirst(long nBri)
{
	CRcBridgeData *pBri	= m_pIn->m_pARcBridgeDataStd->GetBridge(nBri);
	if(!pBri) return FALSE;
	
	BOOL bClickDefaultButton	= IsClickDefaultButton(nBri);
	BOOL bFirst	= pBri->IsFirstDefault(GetIDD());
	if(bClickDefaultButton)
		bFirst	= FALSE;

	return bFirst;
}

BOOL CInputDialogBase::IsClickDefaultButton(long nBri)
{
	return m_bClickDefaultButton && (nBri == m_pIn->m_nCurrentBridge);
}

BOOL CInputDialogBase::IsFocusItem(CString &szTitle)
{
	if(GetDlgItem(m_nIDGrid)) 
	{
		CGridCtrlMng *pGrid = (CGridCtrlMng *)GetDlgItem(m_nIDGrid);
		if(pGrid->GetSafeHwnd())
		{
			for(long nCol=0; nCol<pGrid->GetColumnCount(); nCol++)
			{
				if(pGrid->GetTextMatrix(m_nRowFocusItemTitle,nCol)==szTitle)
				{
					return TRUE;					
				}
			}
		}
	}
	return FALSE;
}

void CInputDialogBase::SetFocusItem(CString szTitle)
{
	if(GetDlgItem(m_nIDGrid)) 
	{
		CGridCtrlMng *pGrid = (CGridCtrlMng *)GetDlgItem(m_nIDGrid);
		if(pGrid->GetSafeHwnd())
		{
			long nRow = pGrid->GetFocusCell().row;
			if(nRow < 0) nRow=1;
			for(long nCol=0; nCol<pGrid->GetColumnCount(); nCol++)
			{
				if(pGrid->GetTextMatrix(m_nRowFocusItemTitle,nCol)==szTitle)
				{
					GetFocus();
					//pGrid->ResetSelection();
					pGrid->SetSelectedRange(-1,-1,-1,-1);
					pGrid->SetFocusCell(nRow,nCol);					
					pGrid->SendMessage(WM_KEYDOWN,VK_F2);
					break;
				}
			}
		}
	}
}

// 미리보기 중인지?
BOOL CInputDialogBase::IsPrintPreview()
{
	CMainFrame *pFrm = dynamic_cast<CMainFrame *>(AfxGetMainWnd());

	return pFrm->m_bPrintPreview;
}

// 도면 그리기 해도 되는지?
// 보통때는 그냥 그려도 되지만, 옵션에 따라서 적용을 누를 때만 그려야 되는 경우가 있다.
BOOL CInputDialogBase::IsRedrawInputApply()
{
	CEnvRegist env;		
	return env.LoadEnvInt(CEnvRegist::REDRAW_INPUTAPPLY);
}


void CInputDialogBase::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	if(GetDlgItem(IDC_TAB))
	{
		m_nTab	= ((CTabCtrl*)GetDlgItem(IDC_TAB))->GetCurSel();
		SetGridData();
		SetDataInit();
		SetControl();
		DrawInputDomyunView(TRUE);
	}

	return ;
}


void CInputDialogBase::SetTabCurSel(CTabCtrl *pTab)
{
	if(!pTab) return;
	if(m_nTab <= -1) m_nTab	= 0;

	if(pTab->GetItemCount() <= m_nTab)
		m_nTab	= pTab->GetItemCount()-1;

	pTab->SetCurSel(m_nTab);
}