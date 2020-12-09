// InputBaseLineInfo.cpp : implementation file
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "../ARcBridgeData/ARcBridgeData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	MAXIPCOUNT 100
#define VERTICAL_RATIO 5

/////////////////////////////////////////////////////////////////////////////
// CInputBaseLineInfo dialog


CInputBaseLineInfo::CInputBaseLineInfo(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputBaseLineInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputBaseLineInfo)
	m_nLineImport = 1;
	m_strFilePath = _T("");
	m_strLineName = _T("");
	m_strStationStt = _T("");
	m_strHeightIp = _T("");
	//}}AFX_DATA_INIT
//	m_pLineOutDlg = new CFTextOutDlg;

	m_dVScaleDraw = 5.00;
}


CInputBaseLineInfo::~CInputBaseLineInfo()
{
//	delete m_pLineOutDlg;
}


void CInputBaseLineInfo::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputBaseLineInfo)
	DDX_Control(pDX, IDC_BUTTON_INSERT_ROW, m_btInsert);
	DDX_Control(pDX, IDC_STATIC_NAME, m_stcLineName);
	DDX_Control(pDX, IDC_EDIT_LINENAME, m_edtLineName);
	DDX_Control(pDX, IDC_BUTTON_PLANE, m_btPlane);
	DDX_Control(pDX, IDC_BUTTON_SLOPE, m_btSlope);
	DDX_Control(pDX, IDC_BUTTON_VERTICAL, m_btVertical);
	DDX_Control(pDX, IDC_BUTTON_SYMLR, m_btSymLR);
	DDX_Control(pDX, IDC_BUTTON_EQLR, m_btEQLR);
	DDX_Control(pDX, IDC_BUTTON_ADD_ROW, m_btAdd);
	DDX_Control(pDX, IDC_BUTTON_DELETE_ROW, m_btDelete);
	DDX_Control(pDX, IDC_BUTTON_LINEOPEN, m_btLineOpen);
	DDX_Control(pDX, IDC_EDIT_FILEPATH, m_edtFilePath);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Radio(pDX, IDC_RADIO_LINEIMPORT, m_nLineImport);
	DDX_Text(pDX, IDC_EDIT_FILEPATH, m_strFilePath);
	DDX_Text(pDX, IDC_EDIT_LINENAME, m_strLineName);
	DDX_Text(pDX, IDC_EDIT_STATION_STT, m_strStationStt);
	DDX_Text(pDX, IDC_EDIT_IP_HEIGHT, m_strHeightIp);
	DDX_Control(pDX, IDC_COMBO_LINESELECT, m_ctlLineSelect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputBaseLineInfo, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputBaseLineInfo)
	ON_BN_CLICKED(IDC_RADIO_LINEIMPORT, OnRadioLineImport)
	ON_BN_CLICKED(IDC_RADIO_LINEINPUT, OnRadioLineInput)
	ON_BN_CLICKED(IDC_BUTTON_LINEOPEN, OnButtonLineOpen)
	ON_BN_CLICKED(IDC_BUTTON_LINESAVE, OnButtonLineSave)
	ON_BN_CLICKED(IDC_BUTTON_ADD_ROW, OnButtonAddRow)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_ROW, OnButtonDeleteRow)
	ON_BN_CLICKED(IDC_BUTTON_EQLR, OnButtonEqLR)
	ON_BN_CLICKED(IDC_BUTTON_SYMLR, OnButtonSymLR)
	ON_BN_CLICKED(IDC_BUTTON_PLANE, OnButtonPlane)
	ON_BN_CLICKED(IDC_BUTTON_VERTICAL, OnButtonVertical)
	ON_BN_CLICKED(IDC_BUTTON_SLOPE, OnButtonSlope)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_EN_KILLFOCUS(IDC_EDIT_LINENAME, OnKillfocusEditLinename)
	ON_EN_KILLFOCUS(IDC_EDIT_STATION_STT, OnKillfocusEditStationStt)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_BN_CLICKED(IDC_BUTTON_ADD_LINE, OnButtonAddLine)
	ON_BN_CLICKED(IDC_BUTTON_DEL_LINE, OnButtonDelLine)
	ON_WM_PAINT()
	ON_EN_KILLFOCUS(IDC_EDIT_IP_HEIGHT, OnKillfocusEditIpHeight)
	ON_CBN_SELCHANGE(IDC_COMBO_LINESELECT, OnSelchangeComboLineselect)	
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
	ON_NOTIFY(GVN_CELL_INSERTED_ROW, IDC_GRID, OnCellInsertedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputBaseLineInfo message handlers

void CInputBaseLineInfo::OnPreInitDialog() 
{

	InitTabCtrl();
	SetEnableWindow();
	
	m_btInsert.ShowWindow(SW_HIDE); // 2004. 7. 5. 수정

	SetResize(IDC_GRID,				 SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB,				 SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_RADIO_LINEIMPORT,	 SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_LINEINPUT,	 SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_FILEPATH,	 SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_LINEOPEN,	 SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_LINESAVE,	 SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_ADD_ROW,	 SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DELETE_ROW, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_INSERT_ROW, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_BUTTON_EQLR,		 SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SYMLR,		 SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC_GROUP2,	 SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_CALC,		 SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_PLANE,		 SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_VERTICAL,	 SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_SLOPE,		 SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC_IP_HEIGHT,		 SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_IP_HEIGHT,		 SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_BUTTON_ADD_LINE,		 SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DEL_LINE,		 SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetHyperHelp(m_pIn->GetManualPath());

	SetLineName(0);

	// 최초 실행시 권고안
	if(m_ctlLineSelect.GetCount() == 1 && m_strLineName==_T(""))
	{
		SetDataDefault();
		SetLineName(0);
	}	
}

void CInputBaseLineInfo::DrawInputDomyunView(BOOL bZoomAll)
{
	// 기본입력시는 평면을 매번 구해주도록 셋팅해놓자.
	m_pIn->SettingNeedPlane();

	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		m_pIn->m_pARcBridgeDataStd->GetBridge(bri)->InitSaveCrossSection();
		m_pIn->m_pARcBridgeDataStd->GetBridge(bri)->InitSaveXySlabUpperByLenFrSttStation();
		m_pIn->m_pARcBridgeDataStd->GetBridge(bri)->InitSaveXyMatchLineAndSlabUpper();
	}

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(1);

	UpdateData();

	CDomyun Dom(pDom);

	int nTab = m_Tab.GetCurSel();
	switch(nTab)
	{
		case 0:
			DrawLinePlane(&Dom);
			break;
		case 1:
			DrawVertPlanEL(&Dom);
			break;
		case 2:
			DrawVertGroundEL(&Dom);
			break;
		case 3:
			DrawSlope(&Dom, FALSE);
			DrawSlope(&Dom, TRUE);
		case 4:
			DrawSlope(&Dom, TRUE);
			DrawSlope(&Dom, FALSE);
			break;
	}
	Dom.SetCalcExtRect(); //CDRect rect	= Dom.GetExtRect();
	Dom.SetCalcExtRect(); //rect	= Dom.GetExtRect();

	*pDom << Dom;

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputBaseLineInfo::DrawLinePlane(CDomyun* pDom)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	CLineInfoApp* pLineApp = pStd->GetLineApp();

	CString strErrorMsg = _T("선형 데이터가 올바르지 않습니다.  ");

	CDomyun Dom(pDom);

	pLineApp->SetDataIp(strErrorMsg);
	if(pLineApp->IsValidLineInuptData())
	{
		pLineApp->DrawLineAll(&Dom, 0, 600, TRUE, TRUE, TRUE);
	}

	*pDom << Dom;
}

void CInputBaseLineInfo::DrawVertGroundEL(CDomyun* pDom)
{
	if(m_Grid.GetRowCount()<=2)
		return;

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	CDomyun Dom(pDom);

	Dom.SetLineColor(1);
	Dom.SetTextColor(2);
	Dom.SetTextHeight(pLine->GetTextHeightIp());

   	pLine->DrawJongEarthLine(&Dom,m_dVScaleDraw); // 종단 곡선모양
	
	DrawCursor(&Dom, m_Grid.GetFocusCell().row);


	*pDom << Dom;
}

void CInputBaseLineInfo::DrawCursor(CDomyun* pDom, int nRow)
{
	if(nRow<0)
		return;

	if(nRow>2 && m_Grid.GetTextMatrixDouble(nRow,1) < m_Grid.GetTextMatrixDouble(nRow-1,1))
		return;

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfoApp* pLineApp = pStd->GetLineApp();

	double JCURVERATE = 5;
	double dSta = frM(m_Grid.GetTextMatrixDouble(nRow,1));
	double dEle = frM(m_Grid.GetTextMatrixDouble(nRow,2));

	double dStaSttJongDan = pLineApp->m_VIPData[0][0];
	double dStaEndJongDan = pLineApp->m_VIPData[pLineApp->m_VIPsu - 1][0];
	double dElevBase = pLineApp->GetMinSectionEL(dStaSttJongDan, dStaEndJongDan);

	CDomyun Dom(pDom);

	Dom.SetLineColor(4);
	Dom.SetPLineThick(180);
	Dom.PMoveTo(dSta,(dEle)*JCURVERATE-15000);
	Dom.PLineTo(dSta,(dEle)*JCURVERATE+15000);
	Dom.PLineEnd();

	*pDom << Dom;
}

void CInputBaseLineInfo::DrawVertPlanEL(CDomyun* pDom)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	CDomyun Dom(pDom);

	Dom.SetLineColor(1);
	Dom.SetTextColor(2);
	Dom.SetTextHeight(pLine->GetTextHeightIp());

	m_dVScaleDraw	= 5;
   	pLine->DrawJongDan(&Dom, m_dVScaleDraw); // 종단 곡선모양

	Dom.ZoomAll();

	*pDom << Dom;
}

void CInputBaseLineInfo::DrawSlope(CDomyun* pDom, BOOL bLeft)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	// 함수 그대로 가져옴.
	// DrawPyungubae(CDomyun* pDomP, double dStaStt, double dStaEnd, BOOL bLeft, double dRatio)
	CDomyun Dom(pDom);

	const CTypedPtrArray <CObArray, CSlop*>& rSlopArr = bLeft ? pLine->m_pSlopLeft : pLine->m_pSlopRight; // 편경사

	int nTotalIP = rSlopArr.GetSize();

	// 2개이상 입력되어야 표시 가능
	if(nTotalIP < 2) return;

	double dStaStt = 0.0, dStaEnd = 0.0;
	if(dStaStt == 0 && dStaEnd == 0)
	{
		dStaStt = rSlopArr.GetAt(0)->m_dStationPyungubae;            // 입력된 첫     IP의 Station
		dStaEnd = rSlopArr.GetAt(nTotalIP - 1)->m_dStationPyungubae; // 입력된 마지막 IP의 Station
	}

	long nCase = -1;

	if(bLeft) nCase = 1;
	else      nCase = 2;

	BOOL bTotal =  TRUE;
	BOOL bUser  = FALSE;

	double dRatio = 1000000.0;
	Dom.SetTextColor(4);
	Dom.SetTextHeight(pLine->GetTextHeightIp());

	Dom.SetTextAlignHorz(TA_CENTER);
	Dom.SetTextAngle(0);
	Dom.SetLineColor(1);
	pLine->DrawPyungubaeRatioBar(&Dom, dStaStt, dStaEnd, dRatio, nCase, bTotal, bUser);
	pLine->DrawPyungubaeCenterLine(&Dom, dStaStt, dStaEnd, nCase, bUser);

	long nLineColor = bLeft ? 3 : 5;
	Dom.SetLineColor(nLineColor);
	pLine->DrawPyungubaePlan(&Dom, dStaStt, dStaEnd, dRatio, nCase, bTotal);

	Dom.SetTextAlignHorz(TA_LEFT);
	Dom.SetTextAngle(90);
	pLine->DrawPyungubaeStationDim(&Dom, dStaStt, dStaEnd, dRatio, nCase, bTotal, bUser);

	CString str = bLeft ? _T("좌측") : _T("우측");
	Dom.SetTextColor(nLineColor);
	
	double TextHeight = pLine->GetTextHeightIp()*1.5;
	Dom.SetTextHeight(TextHeight);
	Dom.SetTextAlignHorz(TA_CENTER);
	double dSlop = rSlopArr.GetAt(0)->m_dValuePyungubae;
	double dMoveX = bLeft ? 8000.0 + TextHeight*1.5 : 8000.0;
	double dMoveY = pLine->GetPyungubae(dStaStt,0)*10000.0;
	Dom.TextOut(dStaStt-dMoveX, dSlop*dRatio+dMoveY, str);

	Dom.Invalidate();

	*pDom << Dom;
}


void CInputBaseLineInfo::SetLineInfoFile(CString strFilePath)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfoApp* pLineApp = pStd->GetLineApp();

	CFile file;
	if(file.Open(strFilePath, CFile::modeRead)==FALSE) return;

	CArchive ar(&file,CArchive::load);
	pLineApp->Serialize(ar);
	ar.Close();
	file.Close();					

	CString csErrorMsg(_T(""));
	if(!pLineApp->SetDataIp(csErrorMsg))
	{
		AfxMessageBox(csErrorMsg,MB_OK | MB_ICONEXCLAMATION);
	}
	SetDataInit();
}

void CInputBaseLineInfo::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetFocus();
	m_Grid.SetFocusCell(1, 1);
	SetDataInit();

	UpdateData();

	int nTab = m_Tab.GetCurSel();
// 	if(nTab==2)
// 		m_btInsert.ShowWindow(SW_HIDE);
// 	else
		m_btInsert.ShowWindow(SW_HIDE);
	if(nTab>=3)
	{
		if(nTab==3)
		{
			m_btEQLR.SetWindowText(_T("우측과 동일"));
			m_btSymLR.SetWindowText(_T("우측과 대칭"));
		}
		else
		{
			m_btEQLR.SetWindowText(_T("좌측과 동일"));
			m_btSymLR.SetWindowText(_T("좌측과 대칭"));
		}
		m_btEQLR.ShowWindow(SW_SHOW);
		m_btSymLR.ShowWindow(SW_SHOW);
	}
	else
	{
		m_btEQLR.ShowWindow(SW_HIDE);
		m_btSymLR.ShowWindow(SW_HIDE);
	}

	DrawInputDomyunView();

	return;
}

void CInputBaseLineInfo::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataSave();

	return;
}

void CInputBaseLineInfo::OnRadioLineImport() 
{
	SetEnableWindow();

	if(m_strFilePath.IsEmpty())
	{
		OnButtonLineOpen();
	}
}

void CInputBaseLineInfo::OnRadioLineInput() 
{
	SetEnableWindow();
	SetDataSave();

	DrawInputDomyunView();
}

void CInputBaseLineInfo::OnButtonLineOpen() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	if(!m_strFilePath.IsEmpty())
	{
		CString sMsg = "선형 정보는 이미 로드 되어있습니다.\r\n";		
		sMsg += "선형 파일을 다시 로드 하면 기본 자료 데이터가 맞지 않을수 있습니다.\n계속하시겠습니까?";
		if(AfxMessageBox(sMsg,MB_YESNO|MB_DEFBUTTON2|MB_ICONEXCLAMATION|MB_ICONWARNING)==IDNO)
		{
			return;
		}
	}

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("ALine 파일 (*.lin)|*.lin||"));

	if(dlg.DoModal()==IDOK)
	{
		m_strFilePath = dlg.GetPathName();

		pStd->m_strLineFilePath = m_strFilePath;
		SetLineInfoFile(m_strFilePath);
		m_nLineImport = 0;
		UpdateData(FALSE);
		DrawInputDomyunView();
		SetEnableWindow();
	}

	SetDataSave();

	AfxGetMainWnd()->SendMessage(WM_USER+1234, (WPARAM)pStd->GetLineApp());
}

void CInputBaseLineInfo::SetEnableWindow()
{
	UpdateData();
	BOOL bEnable = (BOOL)m_nLineImport;

	m_edtFilePath.EnableWindow(!bEnable);
	m_btLineOpen.EnableWindow(!bEnable);
	GetDlgItem(IDC_BUTTON_LINESAVE)->EnableWindow(!bEnable);

	m_stcLineName.EnableWindow(bEnable);
	m_edtLineName.EnableWindow(bEnable);

	GetDlgItem(IDC_STATIC_STATION_STT)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_STATION_STT)->EnableWindow(bEnable);

	GetDlgItem(IDC_BUTTON_ADD_LINE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_DEL_LINE)->EnableWindow(bEnable);
	
	m_btAdd.EnableWindow(bEnable);
	m_btDelete.EnableWindow(bEnable);
	m_btInsert.EnableWindow(bEnable);

	m_btEQLR.EnableWindow(bEnable);
	m_btSymLR.EnableWindow(bEnable);

	long i=0; for(i=0; i<m_Grid.GetRowCount(); i++)
	{
		long j=0; for(j=0; j<m_Grid.GetColumnCount(); j++)
		{
			if(bEnable)
				m_Grid.SetItemState(i,j,m_Grid.GetItemState(i,j)& ~ GVIS_READONLY);
			else
				m_Grid.SetItemState(i,j,m_Grid.GetItemState(i,j)| GVIS_READONLY);
		}
	}
	COLORREF clr = bEnable ? RGB(0, 0, 0) : RGB(128, 128, 128);
	m_Grid.SetTextColor(clr);
	m_Grid.SetFixedTextColor(clr);
	m_Grid.SetRedraw(TRUE, TRUE);
}

void CInputBaseLineInfo::InitTabCtrl()
{
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	m_Tab.InsertItem(0, _T("평면 선형"));
	m_Tab.InsertItem(1, _T("종단 계획고"));
	m_Tab.InsertItem(2, _T("종단 지반고"));
	m_Tab.InsertItem(3, _T("좌측 편경사"));
	m_Tab.InsertItem(4, _T("우측 편경사"));
}

void CInputBaseLineInfo::SetGridTitle()
{
	int nRows = m_Grid.GetRowCount();
	int nCols = m_Grid.GetColumnCount();
	for(int nRow=1; nRow<nRows; nRow++)
	{
		for(long nCol=1; nCol<nCols; nCol++)
		{
			m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol) & ~GVIS_READONLY);
			m_Grid.SetItemBkColour(nRow, nCol, CLR_BACK);
			m_Grid.SetItemFgColour(nRow, nCol);
		}
	}
	m_Grid.SetSelectedRange(0, 0, 0, 0);

	int nTab = m_Tab.GetCurSel();
	switch(nTab)
	{
	case 0:
		SetGridTitleLinePlane();
		break;
	case 1:
		SetGridTitleVertPlanEL();
		break;
	case 2:
		SetGridTitleVertGroundEL();
		break;
	case 3:
	case 4:
		SetGridTitleSlope();
		break;
	}
}

void CInputBaseLineInfo::SetGridTitleLinePlane() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	long nIPs = pLine->GetQtyIp();
	long nRows = nIPs + 3;
	long nCols = 6;

	m_Grid.EnableInsertRow(TRUE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 70);
	int nCol=0; for(nCol=1; nCol<nCols; nCol++)
	{
		m_Grid.SetColumnWidth(nCol, 120);
	}

	COLORREF clrBack = RGB(253, 250, 255);
	m_Grid.SetTextBkColor(clrBack);

	nCol = 0;
	m_Grid.SetTextMatrix(0, nCol++, _T("IP"));
	m_Grid.SetTextMatrix(0, nCol++, _T("X 좌표 (m)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("Y 좌표 (m)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("반경 R (m)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("매개변수 A1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("매개변수 A2"));

	m_Grid.SetTextMatrix(1, 0, _T("BP"));
	CString str = _T("");
	for(long nIP=0; nIP<nIPs; nIP++)
	{
		str.Format(_T("IP %d"), nIP+1L);
		m_Grid.SetTextMatrix(nIP+2, 0, str);
	}
	m_Grid.SetTextMatrix(nIPs+2, 0, _T("EP"));

	int i=0; for(i=3; i<6; i++)
	{
		m_Grid.SetTextMatrix(1, i, _T("-"));
		m_Grid.SetItemState(1, i, m_Grid.GetItemState(1, i) | GVIS_READONLY);
		m_Grid.SetItemBkColour(1, i, CLR_READONLY);
		m_Grid.SetItemFgColour(1, i, RGB(128, 128, 128));
		m_Grid.SetTextMatrix(nRows-1, i, _T("-"));
		m_Grid.SetItemState(nRows-1, i, m_Grid.GetItemState(nRows-1, i) | GVIS_READONLY);
		m_Grid.SetItemBkColour(nRows-1, i, CLR_READONLY);
		m_Grid.SetItemFgColour(nRows-1, i, RGB(128, 128, 128));
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputBaseLineInfo::SetGridTitleVertGroundEL() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	long nStas = pLine->m_VEarthLine.GetSize();
	long nRows = nStas + 1;
	long nCols = 3;

	BOOL bFirst = nRows==1 ? TRUE : FALSE;
	if(bFirst)
		nRows += 1;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 70);
	int nCol=0; for(nCol=1; nCol<nCols; nCol++)
	{
		m_Grid.SetColumnWidth(nCol, 120);
	}

	COLORREF clrBack = RGB(253, 250, 255);
	m_Grid.SetTextBkColor(clrBack);

	nCol = 0;
	m_Grid.SetTextMatrix(0, nCol++, _T("순번 (No.)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("측점 (m)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("지반고 (m)"));
	CString str = _T("");
	for(long nRow=1; nRow<nRows; nRow++)
	{
		str.Format(_T("%d"), nRow);
		m_Grid.SetTextMatrix(nRow, 0, str);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputBaseLineInfo::SetGridTitleVertPlanEL() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	long nRows = pLine->m_VIPsu + 1;
	long nCols = 4;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 70);
	int nCol=0; for(nCol=1; nCol<nCols; nCol++)
	{
		m_Grid.SetColumnWidth(nCol, 120);
	}

	COLORREF clrBack = RGB(253, 250, 255);
	m_Grid.SetTextBkColor(clrBack);

	nCol = 0;
	m_Grid.SetTextMatrix(0, nCol++, _T("VIP"));
	m_Grid.SetTextMatrix(0, nCol++, _T("Station (m)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("Elevation (m)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("곡선 길이 (m)"));
	CString str = _T("");
	for(long nRow=1; nRow<nRows; nRow++)
	{
		str.Format(_T("%d"), nRow);
		m_Grid.SetTextMatrix(nRow, 0, str);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputBaseLineInfo::SetGridTitleSlope() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	int nTab = m_Tab.GetCurSel();

	long nRows = 0;
	if(nTab==3)
		nRows = pLine->m_pSlopLeft.GetSize() + 1;
	if(nTab==4)
		nRows = pLine->m_pSlopRight.GetSize() + 1;
	long nCols = 3;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 70);
	int nCol=0; for(nCol=1; nCol<nCols; nCol++)
	{
		m_Grid.SetColumnWidth(nCol, 120);
	}

	COLORREF clrBack = RGB(253, 250, 255);
	m_Grid.SetTextBkColor(clrBack);

	nCol = 0;
	m_Grid.SetTextMatrix(0, nCol++, _T("순번 (No.)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("Station (m)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("편경사 (%)"));
	CString str = _T("");
	for(long nRow=1; nRow<nRows; nRow++)
	{
		str.Format(_T("%d"), nRow);
		m_Grid.SetTextMatrix(nRow, 0, str);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputBaseLineInfo::SetDataDefault() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfoApp* pLineApp  = pStd->GetLineApp();

	BOOL bFirst = !IsUsedFlag();
	if(!bFirst && !m_bClickDefaultButton) return;
	long nTab = m_Tab.GetCurSel();

	if(pStd->m_nHowLineInput==0) return;

	pStd->m_nHowLineInput = 1;


	if(bFirst)
	{
		double SttSta = 0;

		pLineApp->MakeLineInfo(0);
		pLineApp->NewDocument();
		pLineApp->SetValidLineInuptData(TRUE);
		pLineApp->m_sNameProject  = pStd->m_strProjectName;	// 사업명
		pLineApp->SetNameLineEach("선형1");					// 선형명
		pLineApp->m_dStationBp = SttSta;
		pLineApp->SetQtyIp(0);
		pLineApp->m_dZfactor    = 11.8;
//		pLineApp->m_dCantMax	= 160;
		pLineApp->m_dRadiusMax	= 2000;
		pLineApp->m_dValueC	= 0;
		pLineApp->m_dUnitCoordinate	= frM(100.0);
		pLineApp->SetTextHeightIp(frM(2.0));	// 2.5하니까 너무 커서 영역이 엉뚱하게 잡힘. 2로 줄임.(사이즈는 입력받도록 함)
		pLineApp->m_dTextHeightChain = frM(1.5);
		m_strStationStt = "0.000";
		m_strHeightIp.Format("%.1f", toM(pLineApp->GetTextHeightIp()));
	}
	

	long nSizeIP = 2;
	//////////////////////////////////////////////
	// 선형
	if(bFirst || nTab==0)
	{
		double Length = 200000;

		pLineApp->m_dDisChainUnit = frM(20);
		pLineApp->m_xyBp = CDPoint(0,0);
		pLineApp->m_xyEp = CDPoint(Length,0);
	}

	//////////////////////////////////////////////
	// 종단
	if(bFirst || nTab==1)
	{
		pLineApp->m_VIPsu = nSizeIP;
		pLineApp->m_VIPData[0][0] = pLineApp->m_dStationBp;
		pLineApp->m_VIPData[0][1] = 10000;
		pLineApp->m_VIPData[1][0] = pLineApp->GetStationEpLine(); //Station
		pLineApp->m_VIPData[1][1] = 14000;		 //Elevation
	}

	//////////////////////////////////////////////////////////////////////////
	// 지반고
	if(bFirst || nTab==2)
	{
		CLineInfo* pLineBase = GetCurLineInfo();
		AhTPAMake(nSizeIP, &pLineBase->m_VEarthLine, (CVEarthLine*)0);
		pLineBase->m_VEarthLine.GetAt(0)->dSta = pLineApp->m_dStationBp;
		pLineBase->m_VEarthLine.GetAt(0)->dEle = 2000;
		pLineBase->m_VEarthLine.GetAt(1)->dSta = pLineApp->GetStationEpLine();
		pLineBase->m_VEarthLine.GetAt(1)->dEle = 2000;
	}

	//////////////////////////////////////////////
	// 편경사
	if(bFirst || nTab==3)
	{
		double dPyungubeLeft  = -2.0;

		AhTPAMake(nSizeIP, &pLineApp->m_pSlopLeft,  (CSlop*)0);
		pLineApp->m_pSlopLeft[0]->m_dStationPyungubae = pLineApp->m_dStationBp;          // 선형시점
		pLineApp->m_pSlopLeft[0]->m_dValuePyungubae   = dPyungubeLeft/100;
		pLineApp->m_pSlopLeft[1]->m_dStationPyungubae = pLineApp->GetStationEpLine();		// 선형종점
		pLineApp->m_pSlopLeft[1]->m_dValuePyungubae   = dPyungubeLeft/100;
	}
	if(bFirst || nTab==4)
	{
		double dPyungubeRight = -2.0;

		AhTPAMake(nSizeIP, &pLineApp->m_pSlopRight, (CSlop*)0);
		pLineApp->m_pSlopRight[0]->m_dStationPyungubae = pLineApp->m_dStationBp;         // 선형시점
		pLineApp->m_pSlopRight[0]->m_dValuePyungubae   = dPyungubeRight/100;
		pLineApp->m_pSlopRight[1]->m_dStationPyungubae = pLineApp->GetStationEpLine();	// 선형종점
		pLineApp->m_pSlopRight[1]->m_dValuePyungubae   = dPyungubeRight/100;
	}	

	m_Grid.SetFocusCell(1, 1);

	m_pIn->m_pDoc->SetModifiedFlag();
}


void CInputBaseLineInfo::SetDataInit() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	m_nLineImport = pStd->m_nHowLineInput;
	UpdateData(FALSE);

	SetEnableWindow();

	SetGridTitle();
	m_strLineName = pStd->m_pLineApp->GetNameLineEach();
	m_strStationStt.Format("%.3f",toM(pStd->m_pLineApp->m_dStationBp));
	m_strHeightIp.Format("%.1f", toM(pStd->m_pLineApp->GetTextHeightIp()));

	int nTab = m_Tab.GetCurSel();
	switch(nTab)
	{
	case 0:
		SetDataInitLinePlane();
		break;
	case 1:
		SetDataInitVertPlanEL();
		break;
	case 2:
		SetDataInitVertGroundEL();
		break;
	case 3:
	case 4:
		SetDataInitSlope();
		break;
	}

	UpdateData(FALSE);
}


void CInputBaseLineInfo::SetDataInitLinePlane() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	CString str = _T("");

	m_strFilePath = pStd->m_strLineFilePath;
	if(m_strFilePath == "")	//Import 한 선형이 없으면
		m_nLineImport = 1;

	m_strLineName = pLine->GetNameLineEach();
	UpdateData(FALSE);

	str.Format("%.6f", toM(pLine->m_xyBp.x));
	m_Grid.SetTextMatrix(1, 2, str);
	str.Format("%.6f", toM(pLine->m_xyBp.y));
	m_Grid.SetTextMatrix(1, 1, str);

	int nIPs = pLine->GetQtyIp();
	for(int nRow=0; nRow<nIPs; nRow++)
	{
		CIPRecord* pIpRec = &pLine->m_pIpRec[nRow];

		str.Format("%.6f", toM(pIpRec->m_xy_IpRecord.x));
		m_Grid.SetTextMatrix(nRow+2, 2, str);
		str.Format("%.6f", toM(pIpRec->m_xy_IpRecord.y));
		m_Grid.SetTextMatrix(nRow+2, 1, str);
		str.Format("%.6f", toM(pIpRec->m_dR_IpRecord));
		m_Grid.SetTextMatrix(nRow+2, 3, str);
		str.Format("%.6f", toM(pIpRec->m_dA1_IpRecord));
		m_Grid.SetTextMatrix(nRow+2, 4, str);
		str.Format("%.6f", toM(pIpRec->m_dA2_IpRecord));
		m_Grid.SetTextMatrix(nRow+2, 5, str);
	}

	str.Format("%.6f", toM(pLine->m_xyEp.x));
	m_Grid.SetTextMatrix(nIPs+2, 2, str);
	str.Format("%.6f", toM(pLine->m_xyEp.y));
	m_Grid.SetTextMatrix(nIPs+2, 1, str);

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputBaseLineInfo::SetDataInitVertGroundEL() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();


	int nRows = pLine->m_VEarthLine.GetSize();
	for(int nRow=0; nRow<nRows; nRow++)
	{
		double dSta = 0;
		double dElev = 0;

		dSta = toM(pLine->m_VEarthLine.GetAt(nRow)->dSta);
		dElev = toM(pLine->m_VEarthLine.GetAt(nRow)->dEle);

		m_Grid.SetTextMatrix(nRow+1, 1, "%.6f", dSta);
		m_Grid.SetTextMatrix(nRow+1, 2, "%.6f", dElev);

		m_Grid.SetItemFormat(nRow+1, 1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		m_Grid.SetItemFormat(nRow+1, 2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputBaseLineInfo::SetDataInitVertPlanEL() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	if(pLine->m_VIPsu<=1)
		return;

	int nRows = pLine->m_VIPsu + 1;

	CString str;
	for(int nRow=1; nRow<nRows; nRow++)
	{
		int nCol=0; for(nCol=1; nCol<4; nCol++)
		{
			str.Format("%.6f",toM(pLine->m_VIPData[nRow-1][nCol-1]));
			m_Grid.SetTextMatrix(nRow, nCol, str);
		}
	}

	// 종단계획고 처음과 마지막 row 에는 반경을 못 넣도록 하자 ///////////////
	if(nRows > 1)
	{
		m_Grid.SetTextMatrix(1, 3, "-");
		m_Grid.SetItemState(1, 3, m_Grid.GetItemState(1, 3) | GVIS_READONLY);
		m_Grid.SetItemBkColour(1, 3, CLR_READONLY);
		m_Grid.SetItemFgColour(1, 3, RGB(128, 128, 128));

		m_Grid.SetTextMatrix(nRows-1, 3, "-");
		m_Grid.SetItemState(nRows-1, 3, m_Grid.GetItemState(nRows-1, 3) | GVIS_READONLY);
		m_Grid.SetItemBkColour(nRows-1, 3, CLR_READONLY);
		m_Grid.SetItemFgColour(nRows-1, 3, RGB(128, 128, 128));
	}


	m_Grid.SetRedraw(TRUE, TRUE);
}

void CInputBaseLineInfo::SetDataInitSlope() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	CSlop* pSlop;

	int nTab = m_Tab.GetCurSel();
	if(nTab==3)
	{
		long nRows = pLine->m_pSlopLeft.GetSize() + 1;

		CString str;

		for(long nRow=1; nRow<nRows; nRow++)
		{
			pSlop = pLine->m_pSlopLeft[nRow-1];

			str.Format("%.6f", toM(pSlop->m_dStationPyungubae));
			m_Grid.SetTextMatrix(nRow,1,str);
			str.Format("%.6f", pSlop->m_dValuePyungubae*100);
			m_Grid.SetTextMatrix(nRow,2,str);
		}
	}

	if(nTab==4)
	{
		long nRows = pLine->m_pSlopRight.GetSize() + 1;

		CString str;

		for(long nRow=1; nRow<nRows; nRow++)
		{
			pSlop = pLine->m_pSlopRight[nRow-1];

			str.Format("%.6f", toM(pSlop->m_dStationPyungubae));
			m_Grid.SetTextMatrix(nRow,1,str);
			str.Format("%.6f", pSlop->m_dValuePyungubae*100);
			m_Grid.SetTextMatrix(nRow,2,str);
		}
	}

	m_Grid.SetRedraw();
}

void CInputBaseLineInfo::SetDataSave() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;


	// 지중라멘은 선형을 새로 만들어서 붙여 줘야 한다.
	long nQtyBri = pStd->GetBridgeSize();
	long i=0; for(i=0; i<nQtyBri; i++)
	{
		pStd->MakeLinefoBridge(i);
	}

	UpdateData();
	pStd->m_nHowLineInput = m_nLineImport;

	if(pStd->m_nHowLineInput==0) return;

	// 현재 선택된 선형의 이름을 주선형에 적용해버린다. 막아도 되나?
	//pStd->m_pLineApp->SetNameLineEach(m_strLineName);

	int nTab = m_Tab.GetCurSel();
	switch(nTab)
	{
		case 0:
			SetDataSaveLinePlane();
			break;
		case 1:
			SetDataSaveVertPlanEL();
			break;
		case 2:
			SetDataSaveVertGroundEL();
			break;
		case 3:
		case 4:
			SetDataSaveSlope();
		break;
	}

	ResetLine();

	// 지중라멘이 아닌 경우 각 교량의 선형을 CARcBridgeDataStd의 선형으로 설정..
	nQtyBri = pStd->GetBridgeSize();
	CString strErrorMsg = _T("선형 데이터가 올바르지 않습니다.  ");
	for(i=0; i<nQtyBri; i++)
	{
		CRcBridgeApp *pBri = pStd->GetBridge(i);
		if(pBri->m_bUnderGroundRahmen) continue;

		*pBri->m_pLineApp = *pStd->m_pLineApp;
		pBri->m_pLineApp->SetDataIp(strErrorMsg);
		pBri->m_bNeedSettingVertSection	= TRUE;

		pStd->MakeLinefoBridge(i);
	}

	CheckInput();

	
	m_pIn->m_pDoc->SetModifiedFlag();
}


void CInputBaseLineInfo::SetDataSaveLinePlane()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	UpdateData();
	pLine->SetNameLineEach(m_strLineName);

	long nIPs = m_Grid.GetRowCount() - 3;
	pLine->SetQtyIp(nIPs);

	pLine->m_xyBp.x = frM(atof(m_Grid.GetTextMatrix(1, 2)));
	pLine->m_xyBp.y = frM(atof(m_Grid.GetTextMatrix(1, 1)));
	for(int nRow=0; nRow<nIPs; nRow++)
	{
		CIPRecord* pIpRec = &pLine->m_pIpRec[nRow];

		pIpRec->m_xy_IpRecord.x = frM(atof(m_Grid.GetTextMatrix(nRow + 2, 2)));
		pIpRec->m_xy_IpRecord.y = frM(atof(m_Grid.GetTextMatrix(nRow + 2, 1)));
		pIpRec->m_dR_IpRecord     = frM(atof(m_Grid.GetTextMatrix(nRow + 2, 3)));
		pIpRec->m_dA1_IpRecord	  = frM(atof(m_Grid.GetTextMatrix(nRow + 2, 4)));
		pIpRec->m_dA2_IpRecord	  = frM(atof(m_Grid.GetTextMatrix(nRow + 2, 5)));
	}
	pLine->m_xyEp.x = frM(atof(m_Grid.GetTextMatrix(nIPs+2, 2)));
	pLine->m_xyEp.y = frM(atof(m_Grid.GetTextMatrix(nIPs+2, 1)));
}

void CInputBaseLineInfo::SetDataSaveVertGroundEL()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	int nSize = m_Grid.GetRowCount() - 1;

	AhTPAMake(nSize, &pLine->m_VEarthLine, (CVEarthLine*)0);
	
	for(long nRow=0; nRow<nSize; nRow++)
	{
		pLine->m_VEarthLine.GetAt(nRow)->dSta = frM(m_Grid.GetTextMatrixDouble(nRow+1, 1));
		pLine->m_VEarthLine.GetAt(nRow)->dEle = frM(m_Grid.GetTextMatrixDouble(nRow+1, 2));
	}
}

void CInputBaseLineInfo::SetDataSaveVertPlanEL()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	int nRows = pLine->m_VIPsu;
	for(int nRow=0; nRow<nRows; nRow++)
	{
		int nCol=0; for(nCol=0; nCol<3; nCol++)
		{
			pLine->m_VIPData[nRow][nCol] = frM(m_Grid.GetTextMatrixDouble(nRow+1,nCol+1));
		}
	}
}

void CInputBaseLineInfo::SetDataSaveSlope()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	int nTab = m_Tab.GetCurSel();

	int nSIPs = m_Grid.GetRowCount() - 1;
	if(nTab==3)
		AhTPAMake(nSIPs, &pLine->m_pSlopLeft, (CSlop*)0);
	if(nTab==4)
		AhTPAMake(nSIPs, &pLine->m_pSlopRight, (CSlop*)0);

	CSlop *pSlop;

	for(long nRow=1; nRow<=nSIPs; nRow++)
	{
		double sta = 0, val = 0;

		sta = atof(m_Grid.GetTextMatrix(nRow, 1));
		val = atof(m_Grid.GetTextMatrix(nRow, 2));

		if(nTab==3)
			pSlop = pLine->m_pSlopLeft[nRow-1];
		if(nTab==4)
			pSlop = pLine->m_pSlopRight[nRow-1];

		pSlop->m_dStationPyungubae = frM(sta);
		pSlop->m_dValuePyungubae   = val*0.01;
	}
}

void CInputBaseLineInfo::OnButtonAddRow() 
{
	int nTab = m_Tab.GetCurSel();
	switch(nTab)
	{
	case 0:
		AddRowLinePlane();
		break;
	case 1:
		AddRowVertPlanEL();
		break;
	case 2:
		AddRowVertGroundEL();
		break;
	case 3:
	case 4:
		AddRowSlope();
		break;
	}

	// 선형개수가 변경되면 선형을 재설정
	ResetLine();
}

void CInputBaseLineInfo::AddRowLinePlane()
{
	if(m_Grid.GetRows()>=MAXIPCOUNT)
	{
		CString csMessage;
		csMessage.Format("현재 IP수가 최대 IP수(%ld개))에 해당하므로 더 이상 추가할 수 없습니다.  ", MAXIPCOUNT);
		MessageBox(csMessage, "평면 선형 입력", MB_ICONINFORMATION);
		m_Grid.SetFocus();
		return;
	}

	CCellID cID = m_Grid.GetFocusCell();
	if(cID.row==m_Grid.GetRows()-1 && m_Grid.GetRowCount()>2)
		return;

	int nRow = cID.row+1;
	m_Grid.InsertRow(_T(""), nRow);

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	long nIPs = pLine->GetQtyIp();
	nIPs++;
	pLine->SetQtyIp(nIPs);

	SetGridTitleLinePlane();

	for(int nCol = 1; nCol<6; nCol++)
	{
		m_Grid.SetTextMatrix(nRow, nCol, "%.6f", m_Grid.GetTextMatrixDouble(nRow, nCol));
	}

	m_Grid.SetFocusCell(nRow, cID.col);
}

void CInputBaseLineInfo::AddRowVertGroundEL()
{
	CCellID cID = m_Grid.GetFocusCell();
	if(cID.row==m_Grid.GetRows()-1 && m_Grid.GetRowCount()>2)
		return;

	if(cID.row<=0)
	{
		cID.row = 1;
	}
	int nRow = cID.row+1;

	m_Grid.InsertRow(_T(""), nRow);

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	int nStas = pLine->m_VEarthLine.GetSize();
	nStas++;
	AhTPAMake(nStas, &pLine->m_VEarthLine, (CVEarthLine*)0);

	SetGridTitleVertGroundEL();

	double dPrevX = m_Grid.GetTextMatrixDouble(nRow-1, 1);
	double dNextX = m_Grid.GetTextMatrixDouble(nRow+1, 1);
	m_Grid.SetTextMatrix(nRow, 1, "%.6f", (dPrevX+dNextX)/2);
	m_Grid.SetTextMatrix(nRow, 2, "%.6f", m_Grid.GetTextMatrixDouble(nRow-1, 2));
	m_Grid.SetTextMatrix(nRow, 3, "%.6f", 0.0);

//	m_Grid.SetItemState(nRow, 2, m_Grid.GetItemState(nRow,2) | GVIS_READONLY);
//	m_Grid.SetItemBkColour(nRow, 2, CLR_READONLY);
//	m_Grid.SetItemFgColour(nRow, 2, RGB(128, 128, 128));

	m_Grid.SetFocusCell(nRow, cID.col);

	SetDataSave();
	DrawInputDomyunView();
}

void CInputBaseLineInfo::AddRowVertPlanEL()
{
	CCellID cID = m_Grid.GetFocusCell();	
	if(m_Grid.GetRowCount()>100 || (cID.row==m_Grid.GetRows()-1 && m_Grid.GetRowCount()>2))
	{
		m_Grid.SetFocus();
		return;
	}

	int nRow = cID.row+1;
	m_Grid.InsertRow(_T(""), nRow);

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	pLine->m_VIPsu++;

	SetGridTitleVertPlanEL();

	double dPrevX = m_Grid.GetTextMatrixDouble(nRow-1, 1);
	double dNextX = m_Grid.GetTextMatrixDouble(nRow+1, 1);
	m_Grid.SetTextMatrix(nRow, 1, "%.6f", (dPrevX+dNextX)/2);
	m_Grid.SetTextMatrix(nRow, 2, "%.6f", 0.0);
	m_Grid.SetTextMatrix(nRow, 3, "%.6f", 0.0);

	m_Grid.SetFocusCell(nRow, cID.col);

	SetDataSave();
	DrawInputDomyunView();
}

void CInputBaseLineInfo::AddRowSlope()
{
	CCellID cID = m_Grid.GetFocusCell();
	if( m_Grid.GetRows()>100 || (cID.row==m_Grid.GetRows()-1 && m_Grid.GetRowCount()>2))
	{
		m_Grid.SetFocus();
		return;
	}

	int nRow = cID.row+1;
	m_Grid.InsertRow("", nRow);

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	int nTab = m_Tab.GetCurSel();
	if(nTab==3)
	{
		int nSIPs = pLine->m_pSlopLeft.GetSize();
		nSIPs++;
		AhTPAMake(nSIPs, &pLine->m_pSlopLeft, (CSlop*)0);
	}

	if(nTab==4)
	{
		int nSIPs = pLine->m_pSlopRight.GetSize();
		nSIPs++;
		AhTPAMake(nSIPs, &pLine->m_pSlopRight, (CSlop*)0);
	}

	SetGridTitleSlope();

	double dPrevX = m_Grid.GetTextMatrixDouble(nRow-1, 1);
	double dNextX = m_Grid.GetTextMatrixDouble(nRow+1, 1);
	m_Grid.SetTextMatrix(nRow, 1, "%.6f", (dPrevX+dNextX)/2);
	m_Grid.SetTextMatrix(nRow, 2, "%.6f", 0.0);
	m_Grid.SetTextMatrix(nRow, 3, "%.6f", 0.0);

	m_Grid.SetFocusCell(cID.row, cID.col);

	SetDataSave();
	DrawInputDomyunView();
}

void CInputBaseLineInfo::OnButtonDeleteRow() 
{
	int nTab = m_Tab.GetCurSel();
	switch(nTab)
	{
	case 0:
		DeleteRowLinePlane();
		break;
	case 1:
		DeleteRowVertPlanEL();
		break;
	case 2:
		DeleteRowVertGroundEL();
		break;
	case 3:
	case 4:
		DeleteRowSlope();
		break;
	}
}

void CInputBaseLineInfo::DeleteRowLinePlane()
{
	CCellID cID = m_Grid.GetFocusCell();
	int nRow = cID.row;
	if(nRow==1 || nRow==m_Grid.GetRows()-1)
	{
		AfxMessageBox(_T("첫째 행과 마지막 행은 삭제할 수 없습니다.  "));
		m_Grid.SetFocus();
		return;
	}

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	long nIPs = pLine->GetQtyIp();
	nIPs--;
	pLine->SetQtyIp(nIPs);

	m_Grid.DeleteRow(nRow);

	SetGridTitleLinePlane();

	m_Grid.SetFocusCell(nRow, cID.col);
}

void CInputBaseLineInfo::DeleteRowVertGroundEL()
{
	CCellID cID = m_Grid.GetFocusCell();
	int nRow = cID.row;
	if(nRow==1 || nRow==m_Grid.GetRows()-1)
	{
		AfxMessageBox(_T("첫째 행과 마지막 행은 삭제할 수 없습니다.  "));
		m_Grid.SetFocus();
		return;
	}

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	int nStas = pLine->m_VEarthLine.GetSize();
	nStas--;
	AhTPAMake(nStas, &pLine->m_VEarthLine, (CVEarthLine*)0);

	m_Grid.DeleteRow(nRow);

	SetGridTitleVertGroundEL();

	m_Grid.SetFocusCell(nRow, cID.col);

	SetDataSave();
	DrawInputDomyunView();
}

void CInputBaseLineInfo::DeleteRowVertPlanEL()
{
	CCellID cID = m_Grid.GetFocusCell();
	int nRow = cID.row;
	if(nRow==1 || nRow==m_Grid.GetRows()-1)
	{
		AfxMessageBox(_T("첫째 행과 마지막 행은 삭제할 수 없습니다.  "));
		m_Grid.SetFocus();
		return;
	}

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	pLine->m_VIPsu--;

	m_Grid.DeleteRow(nRow);

	SetGridTitleVertPlanEL();

	m_Grid.SetFocusCell(nRow, cID.col);

	SetDataSave();
	DrawInputDomyunView();
}

void CInputBaseLineInfo::DeleteRowSlope()
{
	CCellID cID = m_Grid.GetFocusCell();
	int nRow = cID.row;
	if(nRow==1 || nRow==m_Grid.GetRows()-1)
	{
		AfxMessageBox(_T("첫째 행과 마지막 행은 삭제할 수 없습니다.  "));
		m_Grid.SetFocus();
		return;
	}

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	int nTab = m_Tab.GetCurSel();
	if(nTab==3)
	{
		int nSIPs = pLine->m_pSlopLeft.GetSize();
		nSIPs--;
		AhTPAMake(nSIPs, &pLine->m_pSlopLeft, (CSlop*)0);
	}

	if(nTab==4)
	{
		int nSIPs = pLine->m_pSlopRight.GetSize();
		nSIPs--;
		AhTPAMake(nSIPs, &pLine->m_pSlopRight, (CSlop*)0);
	}

	m_Grid.DeleteRow(nRow);

	SetGridTitleSlope();

	m_Grid.SetFocusCell(nRow, cID.col);

	SetDataSave();
	DrawInputDomyunView();
}

void CInputBaseLineInfo::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	
	int nTab = m_Tab.GetCurSel();
	int nRow = pGridView->iRow;

	switch(nTab)
	{
	case 0:
		m_Grid.SetTextMatrix(nRow, 1, "%.6f", m_Grid.GetTextMatrixDouble(nRow, 1));
		m_Grid.SetTextMatrix(nRow, 2, "%.6f", m_Grid.GetTextMatrixDouble(nRow, 2));
		m_Grid.SetTextMatrix(nRow, 3, "%.6f", m_Grid.GetTextMatrixDouble(nRow, 3));
		m_Grid.SetTextMatrix(nRow, 4, "%.6f", m_Grid.GetTextMatrixDouble(nRow, 4));
		m_Grid.SetTextMatrix(nRow, 5, "%.6f", m_Grid.GetTextMatrixDouble(nRow, 5));
		break;
	case 1:
		m_Grid.SetTextMatrix(nRow, 1, "%.6f", m_Grid.GetTextMatrixDouble(nRow, 1));
		m_Grid.SetTextMatrix(nRow, 2, "%.6f", m_Grid.GetTextMatrixDouble(nRow, 2));
		m_Grid.SetTextMatrix(nRow, 3, "%.6f", m_Grid.GetTextMatrixDouble(nRow, 3));
		break;
	case 2:
	case 3:
	case 4:
		m_Grid.SetTextMatrix(nRow, 1, "%.6f", m_Grid.GetTextMatrixDouble(nRow, 1));
		m_Grid.SetTextMatrix(nRow, 2, "%.6f", m_Grid.GetTextMatrixDouble(nRow, 2));
		break;
	}

	if(nTab!=0)
	{
		SetDataSave();
	//	DrawInputDomyunView(FALSE);
	}
	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputBaseLineInfo::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
	int nTab = m_Tab.GetCurSel();
	if(nTab!=0)
	{
		DrawInputDomyunView();
	}
}

void CInputBaseLineInfo::OnCellInsertedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;	
	
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();
		
	long nIPs = pLine->GetQtyIp();
	pLine->SetQtyIp(nIPs+1);

	SetGridTitle();
		
	int nRow = pGridView->iRow;
	for(int nCol = 1; nCol < m_Grid.GetColumnCount(); nCol++)
	{
// 		if(nCol > 2)
// 			m_Grid.SetTextMatrix(nRow, nCol, "%.6f", m_Grid.GetTextMatrixDouble(nRow, nCol));
// 		else
			m_Grid.SetTextMatrix(nRow, nCol, "%.6f", m_Grid.GetTextMatrixDouble(nRow, nCol));
	}
}

BOOL CInputBaseLineInfo::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();

		int nTab = m_Tab.GetCurSel();
		CCellID cID = m_Grid.GetFocusCell();
		if(cID.row == m_Grid.GetRowCount() - 2 && cID.col == m_Grid.GetColumnCount() - 1)
		{
			m_Grid.SetFocusCell(cID.row+1, 0);
			if(nTab==0) OnLineinfoAdd();			
		}
		if(cID.col == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}

	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputBaseLineInfo::OnButtonEqLR() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	int nTab = m_Tab.GetCurSel();
	if(nTab==3)
	{
		AhTPACopy(&pLine->m_pSlopLeft, &pLine->m_pSlopRight, (CSlop*)0);
		SetDataInitSlope();
	}
	
	if(nTab==4)
	{
		AhTPACopy(&pLine->m_pSlopRight, &pLine->m_pSlopLeft, (CSlop*)0);
		SetDataInitSlope();
	}

	DrawInputDomyunView();
}

void CInputBaseLineInfo::OnButtonSymLR() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	int nTab = m_Tab.GetCurSel();
	if(nTab==3)
	{
		AhTPACopy(&pLine->m_pSlopLeft, &pLine->m_pSlopRight, (CSlop*)0);
		int i=0; for(i=0; i<pLine->GetRSlopsu(); i++)
		{
			pLine->m_pSlopLeft[i]->m_dValuePyungubae *= -1;
		}
		SetDataInitSlope();
	}
	
	if(nTab==4)
	{
		AhTPACopy(&pLine->m_pSlopRight, &pLine->m_pSlopLeft, (CSlop*)0);
		int i=0; for(i=0; i<pLine->GetLSlopsu(); i++)
		{
			pLine->m_pSlopRight[i]->m_dValuePyungubae *= -1;
		}
		SetDataInitSlope();
	}

	DrawInputDomyunView();
}

void CInputBaseLineInfo::OnButtonPlane() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	CStringArray SA;
	pLine->WriteLinePlane(&SA);
	OutputViewOnAView(SA, "평면 선형 정보");

/*	CString szTest = m_pView->GetDocument()->GetTitle();
	szTest.Replace(".arb", "");
	m_pLineOutDlg->SetSaveFileName(szTest + "_평면선형");

	m_pLineOutDlg->SetChild(m_pView);
	m_pLineOutDlg->CreateEx();
	m_pLineOutDlg->SetWindowText("평면 선형 정보");
	m_pLineOutDlg->Clear();
	m_pLineOutDlg->AddText(SA);

	m_pLineOutDlg->m_FOutRichText.SetSelStart(0);
*/
}

void CInputBaseLineInfo::OnButtonVertical() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	CStringArray SA;
	pLine->WriteJongDan(&SA);
	OutputViewOnAView(SA, "종단 선형 정보");

/*	CString szTest = m_pView->GetDocument()->GetTitle();
	szTest.Replace(".arb", "");
	m_pLineOutDlg->SetSaveFileName(szTest + "_종단선형");

	m_pLineOutDlg->SetChild(m_pView);
	m_pLineOutDlg->CreateEx();
	m_pLineOutDlg->SetWindowText("종단 선형 정보");
	m_pLineOutDlg->Clear();
	m_pLineOutDlg->AddText(SA);

	m_pLineOutDlg->m_FOutRichText.SetSelStart(1);
*/
}

void CInputBaseLineInfo::OnButtonSlope() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	CStringArray SA;
	pLine->WritePyungubae(&SA);
	OutputViewOnAView(SA, "편경사 정보");

/*	CString szTest = m_pView->GetDocument()->GetTitle();
	szTest.Replace(".arb", "");
	m_pLineOutDlg->SetSaveFileName(szTest + "_편경사");

	m_pLineOutDlg->SetChild(m_pView);
	m_pLineOutDlg->CreateEx();
	m_pLineOutDlg->SetWindowText("편경사 정보");
	m_pLineOutDlg->Clear();
	m_pLineOutDlg->AddText(SA);

	m_pLineOutDlg->m_FOutRichText.SetSelStart(2);
*/
}

void CInputBaseLineInfo::OnButtonNext() 
{
	if(m_nLineImport==0 && m_strFilePath==_T(""))
	{
		AfxMessageBox(_T("선형 파일을 선택하여 주십시오.  "));
		return;
	}

	CInputDialogBase::OnButtonNext();
}

void CInputBaseLineInfo::OnKillfocusEditLinename() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	UpdateData();
	pLine->SetNameLineEach(m_strLineName);
	SetLineName(m_ctlLineSelect.GetCurSel());

	DrawInputDomyunView(FALSE);
}

void CInputBaseLineInfo::OnKillfocusEditStationStt() 
{
	// TODO: Add your control notification handler code here
	ResetLine();

	DrawInputDomyunView(FALSE);	
}

// 추가
void CInputBaseLineInfo::OnLineinfoAdd()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();
	
	if(m_Grid.GetRows() >= MAXIPSU)
	{
		CString csMessage;
		csMessage.Format("현재 IP 수량이 최대 IP수(%ld개))에 해당하므로 더이상 추가할수 없습니다", MAXIPSU);
		MessageBox(csMessage, "평면 선형 정보", MB_ICONINFORMATION);
		m_Grid.SetFocus();
		return;
	}

	long nIPs = pLine->GetQtyIp();
	pLine->SetQtyIp(nIPs+1);

	// Insert
	CCellID cID = m_Grid.GetFocusCell();
	m_Grid.InsertRow("", cID.row);
	SetGridTitle();

	int nRow = cID.row;
	for(int nCol = 1; nCol < m_Grid.GetColumnCount(); nCol++)
	{
// 		if(nCol > 2)
// 			m_Grid.SetTextMatrix(nRow, nCol, "%.6f", m_Grid.GetTextMatrixDouble(nRow, nCol));
// 		else
			m_Grid.SetTextMatrix(nRow, nCol, "%.6f", m_Grid.GetTextMatrixDouble(nRow, nCol));
	}

	m_Grid.SetFocusCell(cID.row, cID.col);
}

// 입력데이타에 경고 상황 판단.
void CInputBaseLineInfo::CheckInput()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	// 1. BP, IP, EP에 중복된 위치가 있으면 평면 구조도가 정상적으로 그려지지 않을 수 있음
	BOOL bDuplicationIP	= FALSE;
	long nCountIP = pLine->GetQtyIp();	
	CDPointArray xyArr;
	xyArr.Add(pLine->m_xyBp);
	for(long ip = 0; ip < nCountIP; ip++)
		xyArr.Add(pLine->m_pIpRec[ip].m_xy_IpRecord);
	xyArr.Add(pLine->m_xyEp);

	CDPoint xy1(0, 0), xy2(0, 0);
	long nSize	= xyArr.GetSize();
	long i = 0; for(i = 0; i < nSize - 1; i++)
	{
		xy1	= xyArr.GetAt(i);
		for(long j = i + 1; j < nSize; j++)
		{
			xy2	= xyArr.GetAt(j);
			if(ComparePt(xy1, xy2, "="))
			{
				bDuplicationIP	= TRUE;
				break;
			}
		}
	}

	if(bDuplicationIP)
	{
		m_pIn->SetComment("BP, IP, EP에 중복된 위치가 있으면 평면 구조도가 정상적으로 그려지지 않을 수 있습니다.", TRUE);
		return;
	}


	m_pIn->SetComment("", FALSE);
}

BOOL CInputBaseLineInfo::IsCheckMoveOtherDlg()
{
	return FALSE;
}

void CInputBaseLineInfo::OnKillfocusEditIpHeight() 
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfoApp* pLineApp = pStd->GetLineApp();

	UpdateData();
	pLineApp->SetTextHeightIp(frM(atof(m_strHeightIp)));

	DrawInputDomyunView(FALSE);		
}


void CInputBaseLineInfo::OutputViewOnAView(CStringArray &Arr, CString szTitle)
{	
	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	CString szOutput = szPath + (szTitle.IsEmpty() ? "\\Result.Out" : "\\" + szTitle + ".Out");
	CString szAEdit = szPath + "\\AEdit.exe";

	CFile file;
	if(!file.Open(szOutput, CFile::modeCreate | CFile::modeWrite))
	{
		AfxMessageBox("파일을 생성할 수 없습니다.",MB_ICONSTOP | MB_OK);
		return;
	}
	CArchive ar(&file, CArchive::store);
	long n=0; for(n=0; n<Arr.GetSize(); n++) ar.WriteString(Arr.GetAt(n)+"\r\n");
	ar.Close();
	file.Close();

	ShellExecute(NULL,NULL,szAEdit,"\"\tNEW" + szOutput + "\"",NULL,SW_SHOWNORMAL);	
}

void CInputBaseLineInfo::OnButtonLineSave()
{
	AfxGetMainWnd()->SendMessage(206/*ID_EXPORT_LINE*/);
	SetLineName(m_ctlLineSelect.GetCurSel());
	CLineInfo* pLine = GetCurLineInfo();
	m_strStationStt.Format("%.3f",toM(pLine->m_dStationBp));
	SetDataInit();
	DrawInputDomyunView(TRUE);
	UpdateData(FALSE);
}

void CInputBaseLineInfo::OnButtonAddLine()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfoApp* pLineApp = pStd->GetLineApp();

	long nSize = pLineApp->GetLinesu();
	CDialogMakeLineInfo dlg(pLineApp);

	if(dlg.DoModal() == IDOK)
	{
		SetLineName(nSize);

		SetDataInit();
		DrawInputDomyunView(TRUE);
		UpdateData(FALSE);
	}
}

void CInputBaseLineInfo::OnButtonDelLine()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfoApp* pLineApp = pStd->GetLineApp();

	long nSel = m_ctlLineSelect.GetCurSel();
	if(nSel == 0)
	{
		AfxMessageBox(hggettext("주선형은 삭제할 수 없습니다."), MB_OK | MB_ICONINFORMATION);
		return;
	}
	else if(nSel > 0)
	{
		pLineApp->DeleteLineInfo(nSel);

		SetLineName(nSel-1);
		SetDataInit();
		DrawInputDomyunView(TRUE);
		UpdateData(FALSE);
	}
}

void CInputBaseLineInfo::OnSelchangeComboLineselect()
{
	UpdateData();
	SetDataInit();
	DrawInputDomyunView(TRUE);
}

void CInputBaseLineInfo::SetLineName( long nCur )
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfoApp* pLineApp = pStd->GetLineApp();

	m_ctlLineSelect.ResetContent();
	for(long nLine=0; nLine<pLineApp->GetLinesu(); nLine++)
	{
		m_ctlLineSelect.AddString(pLineApp->GetLine(nLine)->GetNameLineEach());
	}
	m_ctlLineSelect.SetCurSel(nCur);
	m_strLineName = GetCurLineInfo()->GetNameLineEach();
}

CLineInfo* CInputBaseLineInfo::GetCurLineInfo()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfoApp* pLineApp = pStd->GetLineApp();

	return pLineApp->GetLine(m_ctlLineSelect.GetCurSel());
}

void CInputBaseLineInfo::KillFocusDlg()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	SetDataSave();
	AfxGetMainWnd()->SendMessage(WM_USER+1234, (WPARAM)pStd->GetLineApp());
}

void CInputBaseLineInfo::ResetLine()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfo* pLine = GetCurLineInfo();

	CString strErrorMsg = _T("선형 데이터가 올바르지 않습니다.  ");

	UpdateData();
	pLine->m_dStationBp = frM(atof(m_strStationStt));
	pLine->SetValidLineInuptData(TRUE);	// 이미 잘못된 선형이 있을 수 있으므로 SetDataIPlineOne전에 정상으로 설정
	pLine->SetDataIpLineOne(strErrorMsg);
}
