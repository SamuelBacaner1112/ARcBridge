// InputBaseBridgeFormFactor.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../../ARcBridge/ARcBridgeDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputBaseBridgeFormFactor dialog


CInputBaseBridgeFormFactor::CInputBaseBridgeFormFactor(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputBaseBridgeFormFactor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputBaseBridgeFormFactor)
	m_nPlane = 0;
	//}}AFX_DATA_INIT
	m_nFlag	= 0;
	
	m_pProgress	= new CDlgProgress;
}

CInputBaseBridgeFormFactor::~CInputBaseBridgeFormFactor()
{
	delete m_pProgress;
}


void CInputBaseBridgeFormFactor::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputBaseBridgeFormFactor)
	DDX_Control(pDX, IDC_BUTTON_SAVE_INDIVISUAL, m_btSaveIndivisual);
	DDX_Control(pDX, IDC_BUTTON_INSERT_INDIVISUAL, m_btInsertIndivisual);
	DDX_Control(pDX, IDC_BUTTON_NEW_ALL, m_btNewAll);
	DDX_Control(pDX, IDC_BUTTON_COPY, m_btCopy);
	DDX_Control(pDX, IDC_BUTTON_MOVE_DOWN, m_btMoveDown);
	DDX_Control(pDX, IDC_BUTTON_MOVE_UP, m_btMoveUp);
	DDX_Control(pDX, IDC_BUTTON_DELETE_ROW, m_btDelete);
	DDX_Control(pDX, IDC_BUTTON_ADD_ROW, m_btAdd);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Radio(pDX, IDC_RADIO_PLANE, m_nPlane);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputBaseBridgeFormFactor, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputBaseBridgeFormFactor)
	ON_BN_CLICKED(IDC_BUTTON_ADD_ROW, OnButtonAddRow)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_ROW, OnButtonDeleteRow)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_UP, OnButtonMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_DOWN, OnButtonMoveDown)
	ON_BN_CLICKED(IDC_BUTTON_COPY, OnButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_NEW_ALL, OnButtonNewAll)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_INDIVISUAL, OnButtonSaveIndivisual)
	ON_BN_CLICKED(IDC_RADIO_PLANE, OnRadioPlane)
	ON_BN_CLICKED(IDC_RADIO_VERTICAL, OnRadioVertical)
	ON_BN_CLICKED(IDC_BUTTON_INSERT_INDIVISUAL, OnButtonInsetIndivisual)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputBaseBridgeFormFactor message handlers

BOOL CInputBaseBridgeFormFactor::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().col == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}

	if(pMsg->hwnd==m_Grid.GetSafeHwnd() && pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		if(next.col==5)
		{
			CString str;
			CStringArray strArr;
			int i=0; for(i=1; i<=MAX_JIJUM_COUNT_RC-1; i++)
			{
				str.Format(_T("%d ��"), i);
				strArr.Add(str);
			}
			m_Grid.SetComboString(strArr);
		}

		if(next.col==6)
		{
			CString str;
			CStringArray strArr;
			strArr.Add(_T("������ ��ü"));
			strArr.Add(_T("������ �и�"));
			m_Grid.SetComboString(strArr);
		}

		if(next.col==8)
		{
			CString str;
			CStringArray strArr;
			int i=0; for(i=2; i<=MAX_JIJUM_COUNT_RC-2; i+=2)
			{
				str.Format(_T("%d ��"), i);
				strArr.Add(str);
			}
			m_Grid.SetComboString(strArr);
		}
	}
	
	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputBaseBridgeFormFactor::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	
	int nRow = pGridView->iRow;
	int nCol = pGridView->iColumn;

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBriCur = pStd->GetBridge(nRow-1);

	long nTypeBridge_Old = pBriCur->m_nTypeBridge;

	CString str	= m_Grid.GetTextMatrix(0, nCol);
	if(str == "���� ����")
	{
		// (37990) ���� ������ �ٲ� ��쿡�� ��� ���� �ʱ�ȭ �Ѵ�.
		if(AfxMessageBox(_T("���� ������ �����ϴ� ��� �ܸ����� �� ������꼭, ����Էµ���\n�Է°��� �缳���Ͽ��� �մϴ�.\n�ش� �޴����� �Է°��� �ٽ� Ȯ���Ͽ� �ֽñ� �ٶ��ϴ�.\n\n�Է°��� �ʱ�ȭ �Ͻðڽ��ϱ�?"), MB_YESNO | MB_ICONQUESTION)==IDNO)
		{
			m_Grid.UpdateData(FALSE);
			SetDataInit();
			return;
		}
	}
	
	if(nCol==1)
		SetDataSave();
	if(nCol>=2 && nCol<=5 || nCol==8)
	{
		if(nCol==3 || nCol==4)
			m_Grid.SetTextMatrix(nRow, nCol, "%.3f", m_Grid.GetTextMatrixDouble(nRow, nCol));
		if(pBriCur->m_nSeparBri==1)
			m_Grid.SetTextMatrix(nRow+1, nCol, m_Grid.GetTextMatrix(nRow, nCol));

		SetDataSave();
	}
	if(nCol==6)
	{ 
		CString str = m_Grid.GetTextMatrix(nRow, nCol);
		if(str==_T("������ ��ü"))
		{
			pBriCur->m_nSeparBri = 0;		// ��ü

			CRcBridgeApp* pBriNext = pStd->GetBridge(nRow);
			delete pBriNext;

			pStd->m_pRcBridgeArray.RemoveAt(nRow);
		}
		else
		{
			pBriCur->m_nSeparBri = 1;		// ����
			pStd->m_pRcBridgeArray.InsertAt(nRow, new CRcBridgeRebar);

			CRcBridgeApp* pBriNext = pStd->GetBridge(nRow);
			*pBriNext = *pBriCur;

			pBriNext->m_pARcBridgeDataStd = pBriCur->m_pARcBridgeDataStd;
			(dynamic_cast<CRcBridgeRebar*>(pBriNext))->m_Util.m_pARcBridgeDataStd	= pBriNext->m_pARcBridgeDataStd;
			pBriNext->m_nSeparBri = 2;		// ����
			pBriNext->SetDataInitJijum();
			pBriNext->SetDataDefaultShapeStd(0, FALSE);
			pBriNext->SetDataDefaultCrossPosition();
			pBriNext->SetDataDefaultJijumLength();
			pBriNext->SetDataDefaultJijumAngle();
			pBriNext->SetDataDefaultHDanSlab();		// Ⱦ�� ���� ���� �� ���� ����
			pBriNext->m_bUnderGroundRahmen = pBriCur->m_bUnderGroundRahmen;

			// #ARCBRIDGE-2469
			pStd->MakeLinefoBridge(nRow);
			SetMatchBridge(pBriCur, nRow-1);
		}

		pStd->m_bMakeDrawingOptionUsed = FALSE;
	}
	
	if (nCol == 7)
	{
		SetDataSave();

		// ���߶���϶��� ��ũ���� ���� �ȵ� //
		if(pBriCur->m_bUnderGroundRahmen)
		{
			pBriCur->m_bApplyLoad[BRIDGE_LOAD_TANK]	= FALSE;
		}

		///////////////////////////////////////
		pBriCur->SetDataDefaultHDanUnderMember();
		pBriCur->SetDataDefaultHDanRoad();
		pStd->MakeLinefoBridge(nRow-1);

		SetMatchBridge(pBriCur, nRow-1);

		pBriCur->MakeGuardWall(pBriCur->GetQtyHDan(TRUE),TRUE);

		if(!pBriCur->m_bUnderGroundRahmen && pBriCur->m_bPrevApplyCalcOpen)
		{
			pBriCur->m_bPrevApplyCalcOpen = FALSE;
			pBriCur->m_bCalcOpen = FALSE;
		}
	}

	if(str == "���� ����")
	{
		pBriCur->SetDataInitForChangeType();
		// ��� �⺻�� �Ҽ� �ֵ��� �ʱ�ȭ����.
		for(long ix=IDD_BASE6_CROSS_POSITION; ix<=IDD_REBAR_GAGAK; ix++)
		{
			pBriCur->SetDefaultedState(ix, 0x00000000);
			//			CInputDialogBase *pDlg = m_pIn->GetDialogByIDD(ix);
			//			if(pDlg)
			//				pDlg->SetUsedFlag(FALSE);
			//			else
			//				continue;
		}

		// (ARCBRIDGE-3593) ���ĵ��� ���� �Է´��̾�α� ID�� �ִ�. ���� ����� �Ѵ�.
		pBriCur->SetDefaultedState(IDD_GEN_LOAD, 0x00000000);

		SetMatchBridge(pBriCur, nRow-1);

		// Ÿ�Ժ��濡 ���� �⺻���� ���׵��� �̸� ��������� �Ѵ�.
		pBriCur->KeepInWallCount();			// ������ ���濡 ���� ��ü���� ����
		pBriCur->SetDataDefaultOutWallType();	// ������ü Ÿ�� ����
		pBriCur->SyncBridge(0, FALSE, FALSE);	// ������ ������ �°� ��ü�� ������ ���� ��� ���� path�� ����

		SetGridData();
	}
	SetDataInit();

	//DrawInputDomyunView(FALSE);

	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputBaseBridgeFormFactor::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	
	int nRow = pGridView->iRow;

	CString str = m_Grid.GetTextMatrix(nRow, 1);
	m_pView->GetDomyun()->STMakeCursor(str);
}

void CInputBaseBridgeFormFactor::OnPreInitDialog()
{

	SetResize(IDC_GRID,					  SZ_TOP_LEFT,	  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_ADD_ROW,		  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DELETE_ROW,	  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_MOVE_UP,		  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_MOVE_DOWN,		  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_COPY,			  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_NEW_ALL,		  SZ_TOP_RIGHT,   SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_SAVE_INDIVISUAL, SZ_TOP_RIGHT,   SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_INSERT_INDIVISUAL, SZ_TOP_RIGHT,   SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_VIEW,			  SZ_TOP_RIGHT,	  SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_PLANE,			  SZ_TOP_RIGHT,	  SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_VERTICAL,		  SZ_TOP_RIGHT,	  SZ_TOP_RIGHT);

	SetHyperHelp(m_pIn->GetManualPath());

	m_dScaleOffset	= 0;
	m_dScaleDefault	= 1;
}

void CInputBaseBridgeFormFactor::DrawInputDomyunView(BOOL bZoomAll)
{
	// �⺻�Է½ô� ����� �Ź� ��������.
	m_pIn->SettingNeedPlane();

	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		m_pIn->m_pARcBridgeDataStd->GetBridge(bri)->InitSaveCrossSection();
		m_pIn->m_pARcBridgeDataStd->GetBridge(bri)->InitSaveXySlabUpperByLenFrSttStation();
		m_pIn->m_pARcBridgeDataStd->GetBridge(bri)->InitSaveXyMatchLineAndSlabUpper();
	}

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	UpdateData();

	CDomyun Dom(pDom);

	if(m_nPlane==0)
		DrawLinePlane(&Dom);
	else
		DrawLineVertical(&Dom);

	*pDom << Dom;

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputBaseBridgeFormFactor::DrawLinePlane(CDomyun* pDom)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfoApp* pLineApp = pStd->GetLineApp();
	CRcBridgeApp* pBri = pStd->GetBridge(0);

	CDomyun Dom(pDom);

	// ��� ���� ǥ��
	CString strErrorMsg = _T("���� �����Ͱ� �ùٸ��� �ʽ��ϴ�.  ");
	pLineApp->SetDataIp(strErrorMsg);

	if(pLineApp->IsValidLineInuptData())
	{
		pLineApp->DrawLineAll(&Dom, 0);
	}

	*pDom << Dom;

	// ���� ǥ��
	CLineInfo* pLine = pBri->GetLineBase();

	CDPoint xyS1(0,0), xyS2(0,0), xyE1(0,0), xyE2(0,0);
	CDPoint vAng(0,0), xyM(0,0);
	double dBriW = 19500;
	double dTextH = pLine->GetTextHeightIp();

	Dom.SetLineColor(3);
	Dom.SetTextColor(2);
	Dom.SetTextHeight(dTextH*1.5);
	Dom.SetTextAlignHorz(TA_CENTER);

	int nBridgeCount = pStd->GetBridgeSize();
	for(int nBr=0; nBr<nBridgeCount; nBr++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(nBr);

// 		if(pBri->m_nSeparBri==2) continue; // ������ �и� ����(0:������ ��ü, 1:���༱, 2:���༱)

		double dSttBri = pBri->m_dStationBridgeStt;
		double dEndBri = dSttBri + pBri->m_dLengthBridge;
		double dMidBri = (dSttBri+dEndBri)/2;
		CString strBri = pBri->m_strBridgeName;

		double dDisLeft = -dBriW/2;
		double dDisRight = dBriW/2;
		if(pBri->m_nSeparBri != 0)
		{
			double dSW = (pBri->m_nSeparBri == 1) ? +1.0 : -1.0;
			dDisLeft = (pBri->m_nSeparBri == 1) ? 10 : -dBriW/2;
			dDisRight = (pBri->m_nSeparBri == 1) ? dBriW/2 : -10; 
		}

		pLine->DrawLS(&Dom, dSttBri, dEndBri, dDisLeft);
		pLine->DrawLS(&Dom, dSttBri, dEndBri, dDisRight);

		xyS1 = pLine->GetXyLineDis(dSttBri, dDisLeft);
		xyS2 = pLine->GetXyLineDis(dEndBri, dDisLeft);
		xyE1 = pLine->GetXyLineDis(dSttBri, dDisRight);
		xyE2 = pLine->GetXyLineDis(dEndBri, dDisRight);
		Dom.LineTo(xyS1, xyE1);
		Dom.LineTo(xyS2, xyE2);

		vAng = pLine->GetAngleAzimuth(dMidBri);
		if(pBri->m_nSeparBri!=1)
		{
			xyM = pLine->GetXyLineDis(dMidBri, dDisLeft - Dom.GetTextHeight()*0.5);
		}
		else
		{
			xyM = pLine->GetXyLineDis(dMidBri, dDisRight + Dom.GetTextHeight()*1.5);
		}
		Dom.SetTextAngle(vAng.GetAngleDegree());
		Dom.TextOut(xyM, strBri);
	}

	*pDom << Dom;
}


void CInputBaseBridgeFormFactor::SetGridTitle() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
		
	m_dlgBridgeType.m_pARcBridgeDataStd	= pStd;
	BOOL bIncludeArch	= IsIncludeArch();

	int nRows = pStd->GetBridgeSize() + 1;
	int nCols = bIncludeArch ? 9 : 8;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 70);
	m_Grid.SetColumnWidth(1, 250);
	m_Grid.SetColumnWidth(2, 150);
	m_Grid.SetColumnWidth(3, 120);
	m_Grid.SetColumnWidth(4, 100);
	m_Grid.SetColumnWidth(5, 60);
	m_Grid.SetColumnWidth(6, 90);
	m_Grid.SetColumnWidth(7, 100);
	if(bIncludeArch)
		m_Grid.SetColumnWidth(8, 100);


	COLORREF clrBack = RGB(253, 250, 255);
	m_Grid.SetTextBkColor(clrBack);

	for(int nRow=1; nRow<nRows; nRow++)
	{
		m_Grid.SetTextMatrix(nRow, 0, pStd->GetStringBridgeOrder(nRow-1));

		m_Grid.SetUserComboDialog(nRow, 2, &m_dlgBridgeType);
		m_Grid.SetCellType(nRow, 5, CT_COMBO);
		m_Grid.SetCellType(nRow, 6, CT_COMBO);
		m_Grid.SetCellType(nRow, 7, CT_CHECKBOX);

		if(bIncludeArch)
			m_Grid.SetCellType(nRow, 8, CT_COMBO);
	}

	int nCol = 0;
	m_Grid.SetTextMatrix(0, nCol++, _T("����"));
	m_Grid.SetTextMatrix(0, nCol++, _T("������"));
	m_Grid.SetTextMatrix(0, nCol++, _T("���� ����"));
	m_Grid.SetTextMatrix(0, nCol++, _T("���� Station (m)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("���� ���� (m)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("������"));
	m_Grid.SetTextMatrix(0, nCol++, _T("�и� ����"));
	m_Grid.SetTextMatrix(0, nCol++, _T("���߶��"));
	if(bIncludeArch)
		m_Grid.SetTextMatrix(0, nCol++, _T("���ҵ已��"));

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputBaseBridgeFormFactor::SetDataDefault() 
{
	if(!IsUsedFlag() || m_bClickDefaultButton)
	{
		CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

		int nBridgeCount = 1;
		pStd->MakeRcBridge(nBridgeCount);		// �⺻�� 1��

		CRcBridgeApp* pBri = pStd->GetBridge(nBridgeCount-1);
		CLineInfo* pLine = pBri->GetLineBase();

		pBri->m_strBridgeName = _T("���� 1"); 
		pBri->SetDataDefaultBridgeType();
		pBri->m_dStationBridgeStt = pLine->m_dStationBp + 50000;
		pBri->m_dLengthBridge = 25000.0;
		pBri->m_nQtyJigan = 2;
		pBri->m_nSeparBri = 0;
		pBri->m_bUnderGroundRahmen = FALSE;
		
		pBri->SetSearchIP();
		pBri->SetDataInitJijum();			// ���� ����
		pBri->SetDataDefaultJijumAngle();	// ���� ���Ŀ� ���� ���� ���� �� ���� ���� ����
		pBri->SetDataDefaultJijumLength();	// ���� �Ÿ� ����
		
		pBri->KeepInWallCount();			// ������ ���濡 ���� ��ü���� ����
		pBri->SetDataDefaultOutWallType();	// ������ü Ÿ�� ����
		pBri->SetDataDefaultShapeStd(0, FALSE);
		pBri->SyncBridge(0, FALSE, FALSE);	// ������ ������ �°� ��ü�� ������ ���� ��� ���� path�� ����

		pBri->SetDataDefaultWingWallEl();	// ������ ���� Elevation ���
		pBri->m_bNeedSettingVertSection	= TRUE;
	}
}

void CInputBaseBridgeFormFactor::SetDataInit() 
{
	SetGridTitle();
	BOOL bIncludeArch	= IsIncludeArch();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	CString str = _T("");
	int nBridgeCount = pStd->GetBridgeSize();
	for(int nBr=0; nBr<nBridgeCount; nBr++)
	{
		int nCol = 1;
		CRcBridgeApp* pBri = pStd->GetBridge(nBr);
		BOOL bArch	= pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH;

		str = pBri->m_strBridgeName;
		m_Grid.SetTextMatrix(nBr+1, nCol++, str);

		str = pBri->GetStringBridgeType();
		if(pStd->m_nTypeProcess != TYPE_PROCESS_NORMAL)
		{
			m_Grid.SetItemState(nBr+1, nCol, m_Grid.GetItemState(nBr+1, nCol) | GVIS_READONLY);
			m_Grid.SetItemBkColour(nBr+1, nCol, CLR_READONLY);
		}
		m_Grid.SetTextMatrix(nBr+1, nCol++, str);

		str.Format("%.3f", toM(pBri->m_dStationBridgeStt));
		m_Grid.SetTextMatrix(nBr+1, nCol++, str);

		str.Format("%.3f", toM(pBri->m_dLengthBridge));
		m_Grid.SetTextMatrix(nBr+1, nCol++, str);

		if(bArch)
		{
			m_Grid.SetItemState(nBr+1, nCol, m_Grid.GetItemState(nBr+1, nCol) | GVIS_READONLY);
			m_Grid.SetItemBkColour(nBr+1, nCol, CLR_READONLY);
			m_Grid.SetItemFgColour(nBr+1, nCol, RGB(128, 128, 128));

			str.Format("1 ��");
			m_Grid.SetTextMatrix(nBr+1, nCol++, str);
		}
		else
		{
			str.Format("%d ��", pBri->m_nQtyJigan);
			m_Grid.SetTextMatrix(nBr+1, nCol++, str);
		}

		if(pBri->m_nSeparBri==0) str = _T("������ ��ü");
		else					 str = _T("������ �и�");
		m_Grid.SetTextMatrix(nBr+1, nCol++, str);

		if (IsEnableUnderGroundRahmen(pBri))
		{
			m_Grid.SetItemState(nBr+1, nCol, m_Grid.GetItemState(nBr+1, nCol) & ~GVIS_READONLY);
			m_Grid.SetItemBkColour(nBr+1, nCol);
			m_Grid.SetItemFgColour(nBr+1, nCol);
		}
		else
		{
			m_Grid.SetItemState(nBr+1, nCol, m_Grid.GetItemState(nBr+1, nCol) | GVIS_READONLY);
			m_Grid.SetItemBkColour(nBr+1, nCol, CLR_READONLY);
			m_Grid.SetItemFgColour(nBr+1, nCol, RGB(128, 128, 128));
		}

		m_Grid.SetTextMatrix(nBr+1, nCol, pBri->m_bUnderGroundRahmen? _T("����") : _T("�������"));
		m_Grid.SetCheck(nBr+1, nCol++, pBri->m_bUnderGroundRahmen);

		if(bIncludeArch)
		{
			if(!bArch)
			{
				m_Grid.SetItemState(nBr+1, nCol, m_Grid.GetItemState(nBr+1, nCol) | GVIS_READONLY);
				m_Grid.SetItemBkColour(nBr+1, nCol, CLR_READONLY);
				m_Grid.SetItemFgColour(nBr+1, nCol, RGB(128, 128, 128));
				m_Grid.SetTextMatrix(nBr+1, nCol++, "����");
			}
			else
			{
				str.Format("%d ��", pBri->m_nQtyJigan);
				m_Grid.SetTextMatrix(nBr+1, nCol++, str);
			}
		}
		
		long nCols	= 7;
		int i=0; for(i=2; i<nCols; i++)
		{
			if(bIncludeArch && i == 5) continue;
			if(pStd->m_nTypeProcess != TYPE_PROCESS_NORMAL && i == 2) continue;


			if(pBri->m_nSeparBri==2)
			{
				m_Grid.SetItemState(nBr+1, i, m_Grid.GetItemState(nBr+1, i) | GVIS_READONLY);
				m_Grid.SetItemBkColour(nBr+1, i, CLR_READONLY);
				m_Grid.SetItemFgColour(nBr+1, i, RGB(128, 128, 128));
			}
			else
			{
				m_Grid.SetItemState(nBr+1, i, m_Grid.GetItemState(nBr+1, i) & ~GVIS_READONLY);
				m_Grid.SetItemBkColour(nBr+1, i);
				m_Grid.SetItemFgColour(nBr+1, i);
			}
		}


	}
}

void CInputBaseBridgeFormFactor::SetDataSave() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	pStd->SetLineAppToBridge();

	BOOL bIncludeArch	= IsIncludeArch();

	CString str = _T("");
	int nCountBri1 = 0;
	int nCountBri2 = 0;
	int nCountBri3 = 0;

	int nSizeBri = pStd->GetBridgeSize();
	for(int nBr=0; nBr<nSizeBri; nBr++)
	{
		int nCol = 1;
		CRcBridgeRebar* pBri = pStd->GetBridgeRebar(nBr);
		BOOL bArch	= pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH;

		int nOldBriType = pBri->m_nTypeBridge;
		double dOldLengthBri = pBri->m_dLengthBridge;
		double dOldStaStt = pBri->m_dStationBridgeStt;
		int nOldSpanCount = pBri->m_nQtyJigan;
		pBri->GetLineBase()->ReSetSearchIp();

		pBri->m_nIdxBridge    = nBr;
		pBri->m_strBridgeName = m_Grid.GetItemText(nBr+1, nCol++);

		str = m_Grid.GetItemText(nBr+1, nCol++);
		if(str==_T("���� ��౳"))			pBri->m_nTypeBridge = BRIDGE_TYPE_PORTAL;
		if(str==_T("���� ��౳"))			pBri->m_nTypeBridge = BRIDGE_TYPE_PI;
		if(str==_T("V�� ��౳"))			pBri->m_nTypeBridge = BRIDGE_TYPE_V;
		if(str==_T("��� ���� ��౳"))		pBri->m_nTypeBridge = BRIDGE_TYPE_TRAP;
		if(str==_T("��� ���� ��౳2"))	pBri->m_nTypeBridge = BRIDGE_TYPE_TRAP2;
		if(str==_T("�����걳"))				pBri->m_nTypeBridge = BRIDGE_TYPE_SLAB;
		if(str==_T("BOX�� ��౳"))				pBri->m_nTypeBridge = BRIDGE_TYPE_BOX;
		if(str==_T("��ġ�� ��౳"))		pBri->m_nTypeBridge = BRIDGE_TYPE_ARCH;

		pBri->m_dStationBridgeStt = frM(atof(m_Grid.GetItemText(nBr+1, nCol++)));
		pBri->m_dLengthBridge = frM(atof(m_Grid.GetItemText(nBr+1, nCol++)));

		str = m_Grid.GetItemText(nBr+1, nCol++);
		if(!bArch)
			pBri->m_nQtyJigan = atol(str.Left(2));

		nCol++;

		if (IsEnableUnderGroundRahmen(pBri) == FALSE)
		{
			pBri->m_bUnderGroundRahmen = FALSE;
			nCol++;
		}
		else
		{
			pBri->m_bUnderGroundRahmen = m_Grid.IsCheck(nBr+1, nCol++);
		}

		if(bIncludeArch)
		{
			if(bArch)
			{
				str	= m_Grid.GetItemText(nBr+1, nCol++);
				pBri->m_nQtyJigan	= atol(str.Left(2));
			}
		}

		if(nOldBriType!=pBri->m_nTypeBridge || dOldLengthBri!=pBri->m_dLengthBridge
			|| nOldSpanCount!=pBri->m_nQtyJigan || dOldStaStt != pBri->m_dStationBridgeStt)
		{
			pBri->SetDataInitJijum();				// ���� ����
			pBri->SetDataDefaultJijumAngle();		// ���� ���Ŀ� ���� ���� ���� �� ���� ���� ����
			pBri->SetDataDefaultJijumLength();		// ���� �Ÿ� ����
		
			pBri->KeepInWallCount();				// ������ ���濡 ���� ��ü���� ����
			pBri->SetDataDefaultOutWallType();		// ������ü Ÿ�� ����
			pBri->SetDataDefaultShapeStd(0, FALSE);
			//pBri->SyncBridge();					// ������ ������ �°� ��ü�� ������ ���� ��� ���� path�� ����
			pBri->SetSearchIP();

			// ���� ���� �ٲ�� ö�� �ǰ�� ��� ���� ��
			if(nOldSpanCount != pBri->m_nQtyJigan)
			{
				pBri->SyncBridge(0, !pBri->IsVertDir());
				pBri->SetDataDefaultRebarInfoCycle_Std(-1, -1);
				pBri->SetDataDefaultRebarSupport_VertSection_Std(-1);
				pBri->SetDataDefaultRebarShear_VertSection_Std(-1);
				pBri->SyncBridgeRebar(TRUE);
			}			
		}

		pBri->m_bNeedSettingVertSection	= TRUE;

		if(pBri->IsOutsideWall())
			nCountBri1++;
		else
			nCountBri2++;
		if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB)
			nCountBri3++;


		// ������ �Է��Ǿ� �ִٰ� ������ �ٲ�鼭 ���� �ʱ�ȭ ���� ���� ��찡 ���� �� ����
		// ���� ����Ÿ ����� �׻� üũ ����
		BOOL bNoneShoe	= FALSE;
		for(long j = 0; j < pBri->GetCountJijum(); j++)
		{
			CJijum *pJ	= pBri->GetJijum(j);
			if(!pJ) continue;
			
			if(j == 0)
			{
				bNoneShoe	= pBri->IsOutWall(TRUE);
			}
			else if(j == pBri->GetCountJijum()-1)
			{
				bNoneShoe	= pBri->IsOutWall(FALSE);
			}
			else
			{
				bNoneShoe	= FALSE;
				CWallApp *pWall	= pBri->GetInWall(j-1);
				if(pWall)
				{
					if(pWall->m_bIs)
						bNoneShoe	= TRUE;
				}
			}
			
			if(bNoneShoe)
			{
				pJ->m_nQtyShoe[0]	= 0;
				pJ->m_nQtyShoe[1]	= 0;
			}
		}

		// �������� 5������ ������ ����ħ�ϴ� ������ Env��������
		if(pBri->GetCountJijum() > 5) pBri->m_bSelectEnvSubsid = TRUE;

		// ARCBRIDGE-3363
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
		{
			pBri->m_bApplyGroundWaterLevel = FALSE;
			pBri->m_dGroundWaterLevel = 0;
		}

		pBri->SyncJijumStation();
	}

	m_pIn->SetDialogEnable();
	/*
	// ������ ������ ��� ���� ��౳�̰ų� �ڽ����� ��� ���� ��ħ ��ġ �Է� �޴��� ǥ������ ����
	if(nSizeBri==nCountBri1)
		m_pIn->GetDialogByIDD(IDD_BASE8_SHOE_POSITION)->SetUsedFlag(FALSE);
	*/

	/*
	// ������ ���� �߿� ���� ��౳�� �ڽ����� ���� ��� ������ ���� �Է� �޴��� ǥ������ ����
	// ���ӽ�����, �Ҵ� �� ���ش�.
	if(nSizeBri==nCountBri2)
	{
		m_pIn->GetDialogByIDD(IDD_GEN_WINGWALL)->SetUsedFlag(FALSE);
		m_pIn->GetDialogByIDD(IDD_GEN_APS)->SetUsedFlag(FALSE);
		m_pIn->GetDialogByIDD(IDD_GEN_SODAN)->SetUsedFlag(FALSE);
	}
	*/

	/*
	// ������ ������ ��� �����걳�� ��� ��� ��ġ �Է� ������ �޴��� ǥ������ ����
	if(nSizeBri==nCountBri3)
	{
		m_pIn->GetDialogByIDD(IDD_GEN_COLUMN)->SetUsedFlag(FALSE);
		m_pIn->GetDialogByIDD(IDD_GEN_WINGWALL)->SetUsedFlag(FALSE);
		m_pIn->GetDialogByIDD(IDD_GEN_PLANE)->SetUsedFlag(FALSE);
		m_pIn->GetDialogByIDD(IDD_GEN_FOOTING)->SetUsedFlag(FALSE);
		m_pIn->GetDialogByIDD(IDD_GEN_APS)->SetUsedFlag(FALSE);
	}

	AfxGetMainWnd()->SendMessage(WM_USER+8, -1);
	AfxGetMainWnd()->SendMessage(WM_USER+13);
	*/

	m_pIn->CheckStation();
}

void CInputBaseBridgeFormFactor::OnButtonAddRow() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	int nBridgeCount = pStd->GetBridgeSize();

	nBridgeCount++;

	pStd->MakeRcBridge(nBridgeCount);
	pStd->m_bMakeDrawingOptionUsed = FALSE;

	// �ϳ� ����� �ٷ� �ǰ�� ����
	CRcBridgeApp *pBri = pStd->GetBridge(nBridgeCount-1);
	CLineInfo* pLine = pBri->GetLineBase();

	pBri->m_strBridgeName.Format(_T("���� %d"), nBridgeCount);
	pBri->m_dStationBridgeStt = pLine->m_dStationBp + 10000;
	pBri->SetDataInitJijum();
	pBri->SetDataDefaultShapeStd(0, FALSE);
	pBri->SetDataDefaultCrossPosition();
	pBri->SetDataDefaultJijumLength();
	pBri->SetDataDefaultJijumAngle();
	pBri->SetDataDefaultHDanSlab();		// Ⱦ�� ���� ���� �� ���� ����

	SetDataInit();

	DrawInputDomyunView();
}

void CInputBaseBridgeFormFactor::OnButtonDeleteRow() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	
	long nRow = m_Grid.GetFocusCell().row;
	if(nRow < 1) return;
	CRcBridgeApp* pBri = pStd->m_pRcBridgeArray.GetAt(nRow-1);

	int nSeparBri = pBri->m_nSeparBri;
	int nBridgeCount = pStd->GetBridgeSize();

	if((nSeparBri==0 && nBridgeCount<=1) || (nSeparBri>0 && nBridgeCount<=2))
		return;	

	nBridgeCount--;

	if(nRow<0)
	{
		AfxMessageBox("������ ������ �����Ͻʽÿ�.  ", MB_ICONINFORMATION);
		return;
	}

	if(nSeparBri==0)
	{
		delete pBri;
		pStd->m_pRcBridgeArray.RemoveAt(nRow-1);
	}
	if(nSeparBri==1)
	{
		delete pBri;
		delete pStd->m_pRcBridgeArray.GetAt(nRow);
		pStd->m_pRcBridgeArray.RemoveAt(nRow-1);
		pStd->m_pRcBridgeArray.RemoveAt(nRow-1);
	}
	if(nSeparBri==2)
	{
		delete pStd->m_pRcBridgeArray.GetAt(nRow-2);
		delete pBri;
		pStd->m_pRcBridgeArray.RemoveAt(nRow-2);
		pStd->m_pRcBridgeArray.RemoveAt(nRow-2);
	}

	pStd->m_bMakeDrawingOptionUsed = FALSE;
	
	SetDataInit();

	DrawInputDomyunView();
}

void CInputBaseBridgeFormFactor::OnButtonMoveUp() 
{
	long nRow = m_Grid.GetFocusCell().row;
	long nCol = m_Grid.GetFocusCell().col;

	if(nRow<0)
	{
		AfxMessageBox("�̵���ų ������ �����Ͻʽÿ�.  ", MB_ICONINFORMATION);
		return;
	}

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->m_pRcBridgeArray.GetAt(nRow-1);
	int nSeparBri = pBri->m_nSeparBri;

	if((nSeparBri<=1 && nRow==1) || (nSeparBri>1 && nRow<=2))
		return;

	m_Grid.ResetSelection();

	CRcBridgeRebar* pBriPrev = pStd->m_pRcBridgeArray.GetAt(nRow-2);
	if(nSeparBri==0)
	{
		if(pBriPrev->m_nSeparBri==0)
		{
			pStd->m_pRcBridgeArray.SetAt(nRow-2, pBri);
			pStd->m_pRcBridgeArray.SetAt(nRow-1, pBriPrev);
			m_Grid.SetFocusCell(nRow-1, nCol);
		}
		else
		{
			CRcBridgeRebar* pBriPrev2 = pStd->m_pRcBridgeArray.GetAt(nRow-3);
			pStd->m_pRcBridgeArray.SetAt(nRow-3, pBri);
			pStd->m_pRcBridgeArray.SetAt(nRow-2, pBriPrev2);
			pStd->m_pRcBridgeArray.SetAt(nRow-1, pBriPrev);
			m_Grid.SetFocusCell(nRow-2, nCol);
		}
	}
	if(nSeparBri==1)
	{
		CRcBridgeRebar* pBriNext = pStd->m_pRcBridgeArray.GetAt(nRow);
		if(pBriPrev->m_nSeparBri==0)
		{
			pStd->m_pRcBridgeArray.SetAt(nRow-2, pBri);
			pStd->m_pRcBridgeArray.SetAt(nRow-1, pBriNext);
			pStd->m_pRcBridgeArray.SetAt(nRow, pBriPrev);
		}
		else
		{
			CRcBridgeRebar* pBriPrev2 = pStd->m_pRcBridgeArray.GetAt(nRow-3);
			pStd->m_pRcBridgeArray.SetAt(nRow-3, pBri);
			pStd->m_pRcBridgeArray.SetAt(nRow-2, pBriNext);
			pStd->m_pRcBridgeArray.SetAt(nRow-1, pBriPrev2);
			pStd->m_pRcBridgeArray.SetAt(nRow, pBriPrev);
		}
		m_Grid.SetFocusCell(nRow-1, nCol);
	}
	if(nSeparBri==2)
	{
		CRcBridgeRebar* pBriPrev2 = pStd->m_pRcBridgeArray.GetAt(nRow-3);
		if(pBriPrev2->m_nSeparBri==0)
		{
			pStd->m_pRcBridgeArray.SetAt(nRow-3, pBriPrev);
			pStd->m_pRcBridgeArray.SetAt(nRow-2, pBri);
			pStd->m_pRcBridgeArray.SetAt(nRow-1, pBriPrev2);
		}
		else
		{
			CRcBridgeRebar* pBriPrev3 = pStd->m_pRcBridgeArray.GetAt(nRow-4);
			pStd->m_pRcBridgeArray.SetAt(nRow-4, pBriPrev);
			pStd->m_pRcBridgeArray.SetAt(nRow-3, pBri);
			pStd->m_pRcBridgeArray.SetAt(nRow-2, pBriPrev3);
			pStd->m_pRcBridgeArray.SetAt(nRow-1, pBriPrev2);
		}
		m_Grid.SetFocusCell(nRow-1, nCol);
	}

	SetDataInit();
}

void CInputBaseBridgeFormFactor::OnButtonMoveDown() 
{
	long nRow = m_Grid.GetFocusCell().row;
	long nCol = m_Grid.GetFocusCell().col;
	int nRows = m_Grid.GetRowCount();

	if(nRow<0)
	{
		AfxMessageBox("�̵���ų ������ �����Ͻʽÿ�.  ", MB_ICONINFORMATION);
		return;
	}

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->m_pRcBridgeArray.GetAt(nRow-1);
	int nSeparBri = pBri->m_nSeparBri;

	if((nSeparBri!=1 && nRow==nRows-1) || (nSeparBri==1 && nRow==nRows-2))
		return;

	m_Grid.ResetSelection();

	CRcBridgeRebar* pBriNext = pStd->m_pRcBridgeArray.GetAt(nRow);
	if(nSeparBri==0)
	{
		if(pBriNext->m_nSeparBri==0)
		{
			pStd->m_pRcBridgeArray.SetAt(nRow, pBri);
			pStd->m_pRcBridgeArray.SetAt(nRow-1, pBriNext);
			m_Grid.SetFocusCell(nRow+1, nCol);
		}
		else
		{
			CRcBridgeRebar* pBriNext2 = pStd->m_pRcBridgeArray.GetAt(nRow+1);
			pStd->m_pRcBridgeArray.SetAt(nRow+1, pBri);
			pStd->m_pRcBridgeArray.SetAt(nRow, pBriNext2);
			pStd->m_pRcBridgeArray.SetAt(nRow-1, pBriNext);
			m_Grid.SetFocusCell(nRow+2, nCol);
		}
	}
	if(nSeparBri==1)
	{
		CRcBridgeRebar* pBriNext2 = pStd->m_pRcBridgeArray.GetAt(nRow+1);
		if(pBriNext2->m_nSeparBri==0)
		{
			pStd->m_pRcBridgeArray.SetAt(nRow+1, pBriNext);
			pStd->m_pRcBridgeArray.SetAt(nRow, pBri);
			pStd->m_pRcBridgeArray.SetAt(nRow-1, pBriNext2);
		}
		else
		{
			CRcBridgeRebar* pBriNext3 = pStd->m_pRcBridgeArray.GetAt(nRow+2);
			pStd->m_pRcBridgeArray.SetAt(nRow+2, pBriNext);
			pStd->m_pRcBridgeArray.SetAt(nRow+1, pBri);
			pStd->m_pRcBridgeArray.SetAt(nRow, pBriNext3);
			pStd->m_pRcBridgeArray.SetAt(nRow-1, pBriNext2);
		}
		m_Grid.SetFocusCell(nRow+1, nCol);
	}
	if(nSeparBri==2)
	{
		CRcBridgeRebar* pBriPrev = pStd->m_pRcBridgeArray.GetAt(nRow-2);
		if(pBriNext->m_nSeparBri==0)
		{
			pStd->m_pRcBridgeArray.SetAt(nRow, pBri);
			pStd->m_pRcBridgeArray.SetAt(nRow-1, pBriPrev);
			pStd->m_pRcBridgeArray.SetAt(nRow-2, pBriNext);
		}
		else
		{
			CRcBridgeRebar* pBriNext2 = pStd->m_pRcBridgeArray.GetAt(nRow+1);
			pStd->m_pRcBridgeArray.SetAt(nRow+1, pBri);
			pStd->m_pRcBridgeArray.SetAt(nRow, pBriPrev);
			pStd->m_pRcBridgeArray.SetAt(nRow-1, pBriNext2);
			pStd->m_pRcBridgeArray.SetAt(nRow-2, pBriNext);
		}
		m_Grid.SetFocusCell(nRow+1, nCol);
	}

	SetDataInit();
}

void CInputBaseBridgeFormFactor::OnButtonCopy() 
{
	long nRow = m_Grid.GetFocusCell().row;

	if(nRow<0)
	{
		AfxMessageBox("������ ������ �����Ͻʽÿ�.  ", MB_ICONINFORMATION);
		return;
	}

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(nRow-1);

	int nBridgeCount = pStd->GetBridgeSize();
	int nSeparBri = pBri->m_nSeparBri;

	if(nSeparBri==0)
		nBridgeCount++;
	else
		nBridgeCount += 2;

	pStd->MakeRcBridge(nBridgeCount);
	pStd->m_bMakeDrawingOptionUsed = FALSE;

	if(nSeparBri==0)
		*pStd->GetBridgeRebar(pStd->GetBridgeSize()-1) = *pBri;
	if(nSeparBri==1)
	{
		*pStd->GetBridgeRebar(pStd->GetBridgeSize()-2) = *pBri;
		*pStd->GetBridgeRebar(pStd->GetBridgeSize()-1) = *pStd->GetBridgeRebar(nRow);
	}
	if(nSeparBri==2)
	{
		*pStd->GetBridgeRebar(pStd->GetBridgeSize()-2) = *pStd->GetBridgeRebar(nRow-2);
		*pStd->GetBridgeRebar(pStd->GetBridgeSize()-1) = *pBri;
	}

	SetDataInit();

	DrawInputDomyunView();
}

void CInputBaseBridgeFormFactor::OnButtonNewAll() 
{
	if(AfxMessageBox("[�Ϲ� �Է�]�� [��� �Է�]�� �����Ͱ� �Է� ������ ���·� �ٲ�ϴ�. ��� �Ͻðڽ��ϱ�?  ",
		MB_YESNO|MB_ICONQUESTION)==IDNO) return;

	long nID=0; for(nID=0; nID<m_pIn->m_IDDArray[MENU_GENERAL].GetSize(); nID++)
	{
		m_pIn->GetDialogByIDD(m_pIn->m_IDDArray[MENU_GENERAL].GetAt(nID))->SetUsedFlag(FALSE);	
	}

	for(nID=0; nID<m_pIn->m_IDDArray[MENU_REBAR].GetSize(); nID++)
	{
		m_pIn->GetDialogByIDD(m_pIn->m_IDDArray[MENU_REBAR].GetAt(nID))->SetUsedFlag(FALSE);	
	}

	AfxGetMainWnd()->SendMessage(WM_USER+8, -1);
	AfxGetMainWnd()->SendMessage(WM_USER+13);
}

void CInputBaseBridgeFormFactor::OnRadioPlane() 
{
	DrawInputDomyunView();
}

void CInputBaseBridgeFormFactor::OnRadioVertical() 
{
	DrawInputDomyunView();
}

void CInputBaseBridgeFormFactor::DrawLineVertical(CDomyun *pDomP)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(0);
	CLineInfo* pLineBase = pBri->GetLineBase();
	CString sNameLine	= pLineBase->GetNameLineEach();
	CLineInfo *pLine	= pStd->m_pLineApp->GetLine(sNameLine);
	if(!pLine) return;

	int nVIPCount = pLine->m_VIPsu;
	double dVBPSta = pLine->m_VIPData[0][0];
	double dVEPSta = pLine->m_VIPData[nVIPCount-1][0];
	double dTextH = pLine->GetTextHeightIp();

	CDomyun Dom(pDomP);

	// ���� ���� ǥ��
	Dom.SetLineColor(1);
	Dom.SetTextHeight(dTextH);

   	pLine->DrawJongDan(&Dom);

	*pDomP << Dom;

	// ���� ������ ǥ��
	int nBridgeCount = pStd->GetBridgeSize();
	for(int nBr=0; nBr<nBridgeCount; nBr++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(nBr);

		if(pBri->m_nSeparBri==2) continue;

		double dSttBri = pBri->m_dStationBridgeStt;
		double dEndBri = dSttBri + pBri->m_dLengthBridge;
		CString strBri = pBri->m_strBridgeName;

		Dom.SetLineColor(3);
		Dom.SetTextColor(2);
		Dom.SetTextHeight(dTextH);
		pLine->DrawJongDanBridgeTextOut(&Dom, dVBPSta, dVEPSta, dSttBri, dEndBri, 0, TRUE);

		CDRect rect;
		Dom.SetCalcExtRect();
		rect = Dom.GetExtRect();

		Dom.SetTextHeight(dTextH*1.5);
		Dom.SetTextAngle(0);
		Dom.SetTextAlignHorz(TA_CENTER);
		Dom.TextOut((dSttBri+dEndBri)*0.5, rect.bottom, strBri);

		*pDomP << Dom;
	}
}

// ���� ����
void CInputBaseBridgeFormFactor::OnButtonInsetIndivisual() 
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBriOrg	= pStd->GetBridgeRebar(0);
	if(!pBriOrg) return;


	CFileDialog dlg(TRUE, _T("abi"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("ARcBridge ���� (.abi)|*.abi||"), GetParent());	
	if(dlg.DoModal()==IDOK)
	{
		long nCountSaveBri	= 0;
		CFile file;
		if(file.Open(dlg.GetPathName(),CFile::modeRead)==FALSE)
			return;

		// ������������ ������� �¿��߽�,���������⿡ ���� ���� ��ǥ�� ��� ���� ���´�.
		// �ٸ� ������ ���� ������ �������� ������� ���� �̿��ϱ� ������ �ӵ� ������ ���ؼ� ���س��°���
		if(!m_pProgress->GetSafeHwnd())
		{
			m_pProgress->Create(IDD_DIALOG_HGPROGRESS, this);
		}
		m_pProgress->ShowWindow(SW_SHOW);
		m_pProgress->CenterWindow();
		m_pProgress->InitProgressData();


		CArchive ar(&file, CArchive::load);
		ar >> m_nFlag;
		ar >> nCountSaveBri;	// ���� ����

		long i = 0; for(i = 0; i < nCountSaveBri; i++)
		{
			int nBridgeCount = pStd->GetBridgeSize();
			nBridgeCount++;
			pStd->MakeRcBridge(nBridgeCount);
			pStd->m_bMakeDrawingOptionUsed	= FALSE;
			
			CRcBridgeRebar *pBri	= pStd->GetBridgeRebar(nBridgeCount-1);

			
			m_pProgress->SetProgressPos("������ �ҷ����� ��....", 20);
			pBri->Serialize(ar);

			m_pProgress->SetProgressPos("�ҷ��� ���� ��ü ����ȭ ��....", 40);
			long nOldTypeJongDanRebar	= pBri->m_nTypeJongdanForRebar;
			pBri->m_nTypeJongdanForRebar	= 2;
			pBri->SyncBridge(0, FALSE);
			pBri->m_nTypeJongdanForRebar	= nOldTypeJongDanRebar;

			m_pProgress->SetProgressPos("�ҷ��� ���� ���ܸ� ���� ����ȭ ��....", 60);
			pBri->SettingVertSection();

			m_pProgress->SetProgressPos("�ҷ��� ���� Stationüũ �� ....", 80);
			// ���� staüũ
			if(!pBri->IsValidStationOfBri())
			{
				CString str = _T("");
				str.Format("%s ������ Station�� ������ ���� �� �ֽ��ϴ�. \n������ ������ �����Ͻʽÿ�.", pBri->m_strBridgeName);
				AfxMessageBox(str);
			}

			m_pProgress->SetProgressPos("���� �ҷ����� �Ϸ�.", 100);
			m_pProgress->ShowWindow(SW_HIDE);
		}
		
		ar.Close();
		file.Close();

		SetDataInit();
		DrawInputDomyunView(FALSE);
		m_pIn->CheckStation();
	}

	if(!pStd->IsEnableSlabStandardOutput() && pStd->m_nTypeProcess == TYPE_PROCESS_SLAB_STANDARD_OUTPUT)
	{
		AfxMessageBox("�ҷ������� �����߿� ������ ǥ�ص� �۾��� �������� ���� �������� ���� ������ ǥ�ص� �۾��� ��ҵ˴ϴ�.");
		pStd->m_nTypeProcess	= TYPE_PROCESS_NORMAL;
	}
}

// ��������
void CInputBaseBridgeFormFactor::OnButtonSaveIndivisual() 
{
	long nRow = m_Grid.GetFocusCell().row;
	if(nRow<0)
	{
		AfxMessageBox("������ ������ �����Ͻʽÿ�.", MB_ICONINFORMATION);
		return;
	}	

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(nRow-1);
	int nSeparBri = pBri->m_nSeparBri;

	// ������ ����  ////////////////////////////////////////
	// �ִ� �� �� ���� �����(������ �и��� ��� 2��)
	long nCountSaveBri	= 0;
	long nBri[2]	= {-1, -1};
	if(nSeparBri == 0)
	{
		nBri[0]	= nRow - 1;
		if(pStd->GetBridgeRebar(nBri[0]))
			nCountSaveBri	= 1;
	}
	else if(nSeparBri == 1)
	{
		nBri[0]	= nRow - 1;
		nBri[1]	= nRow;
		if(pStd->GetBridgeRebar(nBri[0]) && pStd->GetBridgeRebar(nBri[1]))
			nCountSaveBri	= 2;
	}
	else if(nSeparBri == 2)
	{
		nBri[0]	= nRow - 2;
		nBri[1]	= nRow - 1;
		if(pStd->GetBridgeRebar(nBri[0]) && pStd->GetBridgeRebar(nBri[1]))
			nCountSaveBri	= 2;
	}
	/////////////////////////////////////////////////////////

	if(nCountSaveBri == 0)
	{
		AfxMessageBox("������ ������ ��ȿ���� �ʽ��ϴ�.", MB_ICONINFORMATION);
		return;
	}


	CFileDialog dlg(FALSE, _T("abi"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("ARcBridge ���� (.abi)|*.abi||"), GetParent());	
	if(dlg.DoModal()==IDOK)
	{
		CFile file;
		if(file.Open(dlg.GetPathName(),CFile::modeCreate|CFile::modeWrite)==FALSE)
			return;

		CArchive ar(&file, CArchive::store);
		ar << m_nFlag;
		ar << nCountSaveBri;	// ���� ����

		long i = 0; for(i = 0; i < nCountSaveBri; i++)
		{
			CRcBridgeRebar *pBri	= pStd->GetBridgeRebar(nBri[i]);
			pBri->Serialize(ar);
		}
		
		ar.Close();
		file.Close();
	}
}

BOOL CInputBaseBridgeFormFactor::IsOKMoveOtherDlg()
{
	if(!m_pIn->CheckStation()) return FALSE;

	return TRUE;
}


BOOL CInputBaseBridgeFormFactor::IsCheckMoveOtherDlg()
{
	return FALSE;
}

BOOL CInputBaseBridgeFormFactor::IsIncludeArch()
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	if(!pStd) return FALSE;

	BOOL bArch	= FALSE;
	for(long bri = 0; bri < pStd->GetBridgeSize(); bri++)
	{
		CRcBridgeApp *pBri	= pStd->GetBridge(bri);
		if(!pBri) continue;
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			bArch	= TRUE;
			break;
		}
	}

	return bArch;
}

HGBOOL CInputBaseBridgeFormFactor::IsEnableUnderGroundRahmen(CRcBridgeApp* pBri) const
{
	if (pBri->m_nTypeBridge == BRIDGE_TYPE_PORTAL || pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2)
	{
		return TRUE;
	}

	return FALSE;
}

void CInputBaseBridgeFormFactor::SetMatchBridge(CRcBridgeApp* pBriCur, HGINT32 iBridge)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	if(pBriCur->m_nSeparBri!=0)
	{
		long nIdxMatch = pStd->GetIdxMatchBridge(iBridge);
		CRcBridgeApp* pBriMatch = pStd->GetBridge(nIdxMatch);

		pBriMatch->m_bUnderGroundRahmen = pBriCur->m_bUnderGroundRahmen;
		// ���߶���϶��� ��ũ���� ���� �ȵ� //
		if(pBriMatch->m_bUnderGroundRahmen)
		{
			pBriMatch->m_bApplyLoad[BRIDGE_LOAD_TANK]	= FALSE;
		}
		///////////////////////////////////////
		pBriMatch->SetDataDefaultHDanUnderMember();
		pBriMatch->SetDataDefaultHDanRoad();
		// #ARCBRIDGE-2469
		if(pBriMatch->m_bUnderGroundRahmen)
		{
			pBriMatch->SetDataDefaultHDanNori();
			pBriMatch->SetDataDefaultHDanPave();
		}
		pStd->MakeLinefoBridge(nIdxMatch);
	}
}
