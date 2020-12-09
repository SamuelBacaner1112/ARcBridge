// InputBaseProjectInfo.cpp : implementation file
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "../ARcBridgeData/ARcBridgeData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputBaseProjectInfo dialog


CInputBaseProjectInfo::CInputBaseProjectInfo(CWnd* pParent /*=NULL*/)
: CInputDialogBase(CInputBaseProjectInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputBaseProjectInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CInputBaseProjectInfo::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputBaseProjectInfo)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputBaseProjectInfo, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputBaseProjectInfo)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputBaseProjectInfo message handlers

void CInputBaseProjectInfo::OnPreInitDialog() 
{
	m_btnDefault.ShowWindow(SW_HIDE);
	m_btnPrev.ShowWindow(SW_HIDE);
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetHyperHelp(m_pIn->GetManualPath());
}

void CInputBaseProjectInfo::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(1);

//	CGridDomyunEx Dom(pDom);
//
//	CHgBaseDrawStd baseDraw;	
//	baseDraw.ChangeGridToDom(&Dom, &m_Grid, HCAD_TBTC, HCAD_TBL1, HCAD_TBL1);

//	*pDom << Dom;
	
	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputBaseProjectInfo::SetGridTitle() 
{
	int nRows = 7;
	int nCols = 2;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 130);
	m_Grid.SetColumnWidth(1, 640);

	COLORREF clrBack = RGB(253, 250, 255);
	m_Grid.SetTextBkColor(clrBack);

	int nRow = 0;
	m_Grid.SetTextMatrix(nRow++, 0, _T("�� �� ��"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("�� �� ó"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("�� �� ��"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("�� �� ��"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("�� �� ��"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("���� ���� ��Ī"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("���� �ݴ� ���� ��Ī"));
}

void CInputBaseProjectInfo::SetDataDefault() 
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	BOOL bFirst = !IsUsedFlag();
	if(!bFirst && !m_bClickDefaultButton) return;

	pStd->m_strProjectName		= _T("����� 1");
	pStd->m_strOwnerName		= _T("�ѱ� ���� ����");
	pStd->m_strConstructorName	= _T("(��)�ѱ����Ƽ");
	pStd->m_strSpotName			= _T("�� 1����");
	pStd->m_strEngName			= _T("(��)�ѱ����Ƽ");
	pStd->m_strLineDirName		= _T("��������");
	pStd->m_strLineRevDirName	= _T("�����ݴ����");
}

void CInputBaseProjectInfo::SetDataInit() 
{
	SetGridTitle();

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	long nRow = 0;
	CString	str;

	str = pStd->m_strProjectName;
	m_Grid.SetTextMatrix(nRow++,1,str,GVHT_LEFT,FALSE);
	str = pStd->m_strOwnerName;
	m_Grid.SetTextMatrix(nRow++,1,str,GVHT_LEFT,FALSE);
	str = pStd->m_strConstructorName;
	m_Grid.SetTextMatrix(nRow++,1,str,GVHT_LEFT,FALSE);
	str = pStd->m_strSpotName;
	m_Grid.SetTextMatrix(nRow++,1,str,GVHT_LEFT,FALSE);
	str = pStd->m_strEngName;
	m_Grid.SetTextMatrix(nRow++,1,str,GVHT_LEFT,FALSE);
	str = pStd->m_strLineDirName;
	m_Grid.SetTextMatrix(nRow++,1,str,GVHT_LEFT,FALSE);
	str = pStd->m_strLineRevDirName;
	m_Grid.SetTextMatrix(nRow++,1,str,GVHT_LEFT,FALSE);

	m_Grid.SetRedraw(TRUE,TRUE);
	
	AfxGetMainWnd()->SendMessage(WM_USER+12);
}

void CInputBaseProjectInfo::SetDataSave() 
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	long nRow = 0;

	pStd->m_strProjectName	   = m_Grid.GetTextMatrix(nRow++, 1);
	pStd->m_strOwnerName	   = m_Grid.GetTextMatrix(nRow++, 1);
	pStd->m_strConstructorName = m_Grid.GetTextMatrix(nRow++, 1);
	pStd->m_strSpotName		   = m_Grid.GetTextMatrix(nRow++, 1);
	pStd->m_strEngName		   = m_Grid.GetTextMatrix(nRow++, 1);
	pStd->m_strLineDirName	   = m_Grid.GetTextMatrix(nRow++, 1);
	pStd->m_strLineRevDirName  = m_Grid.GetTextMatrix(nRow++, 1);

	m_pIn->m_pDoc->SetModifiedFlag();
	AfxGetMainWnd()->SendMessage(WM_USER+12);

	CInputDialogBase::SetDataSave();
}

BOOL CInputBaseProjectInfo::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		if(m_Grid.GetFocusCell().row == m_Grid.GetRows()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}
	
	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputBaseProjectInfo::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	
	int nRow = pGridView->iRow;

	CString str;
	switch(nRow)
	{
	case 0:
		str = _T("�������� �Է��մϴ�.");
		break;
	case 1:
		str = _T("����ó�� �̸��� �Է��մϴ�.");
		break;
	case 2:
		str = _T("�ð� ��ü�� �̸��� �Է��մϴ�.");
		break;
	case 3:
		str = _T("������� �Է��մϴ�.");
		break;
	case 4:
		str = _T("���� ��ü�� �̸��� �Է��մϴ�.");
		break;
	case 5:
		str = _T("���� ���� ������ �̸��� �����մϴ�.");
		break;
	case 6:
		str = _T("���� ���� �ݴ� ������ �̸��� �����մϴ�.");
		break;
	default:
		str = _T("");
		break;
	}

	m_pIn->SetComment(str);
}

void CInputBaseProjectInfo::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	SetDataSave();
}
