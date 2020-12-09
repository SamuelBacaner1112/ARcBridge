// DialogExportStxOption.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARcBridge.h"
#include "MainFrm.h"
#include "ARcBridgeDoc.h"
#include "ExportToAStrutTie.h"
#include "DialogExportStxOption.h"
//#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CDialogExportStxOption 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogExportStxOption, CDialog)

CDialogExportStxOption::CDialogExportStxOption(CARcBridgeDataStd *pDataStd, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogExportStxOption::IDD, pParent)
{
	m_pDataStd = pDataStd;
}

CDialogExportStxOption::~CDialogExportStxOption()
{
}

void CDialogExportStxOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID_STM_EXPORT, m_Grid);
	DDX_Control(pDX, IDC_BTN_ARCCORNER, m_btnArcCorner);
	DDX_Control(pDX, IDC_BTN_BRACKET, m_btnBraket);
	DDX_Control(pDX, IDC_BTN_FOOTING, m_btnFooting);
	DDX_Control(pDX, IDC_BTN_FOOTING_MID, m_btnFootingMid);
}


BEGIN_MESSAGE_MAP(CDialogExportStxOption, CDialog)
	ON_BN_CLICKED(IDC_BTN_CHECK_ALL, &CDialogExportStxOption::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDC_BTN_UNCHECK_ALL, &CDialogExportStxOption::OnBnClickedUncheckAll)
	ON_BN_CLICKED(IDC_BTN_EXPORT, &CDialogExportStxOption::OnBnClickedExport)
	ON_BN_CLICKED(IDC_BTN_ARCCORNER, &CDialogExportStxOption::OnBnClickedBtnArccorner)
	ON_BN_CLICKED(IDC_BTN_BRACKET, &CDialogExportStxOption::OnBnClickedBtnBracket)
	ON_BN_CLICKED(IDC_BTN_FOOTING, &CDialogExportStxOption::OnBnClickedBtnFooting)
	ON_BN_CLICKED(IDC_BTN_FOOTING_MID, &CDialogExportStxOption::OnBnClickedBtnFootingMid)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_STM_EXPORT, OnCellChangedData)
END_MESSAGE_MAP()


// CDialogExportStxOption 메시지 처리기입니다.
BOOL CDialogExportStxOption::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDataDefault();	

 	SetDataSave();
 	SetGridData();
 	SetDataInit();

 	SetButtonImage();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
 
void CDialogExportStxOption::SetDataInit()
{
	m_Grid.UpdateData(FALSE);
}

void CDialogExportStxOption::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
}

void CDialogExportStxOption::SetGridData()
{
	long nRow=0, nCol=0;
	m_Grid.SetGridDefault(1,1,1,0);

	m_Grid.AddCell(0, 0, _T("출력항목"));
	HGINT32 icOption = m_pDataStd->m_vStxOption.size();
	for(HGINT32 ix = 0; ix < icOption; ++ix)
	{
		m_Grid.AddCell(ix+1, 0, &m_pDataStd->m_vStxOption[ix].check, DT_LEFT|DT_VCENTER, CELL_LINE_NO, m_pDataStd->m_vStxOption[ix].name, CELL_TYPE_CHECK);
	}

	m_Grid.GetCellCount(nRow, nCol);
	m_Grid.SetRows(nRow);
	m_Grid.SetCols(nCol);
	m_Grid.ExpandColumnsToFit();
	m_Grid.SetRowHeightAll(20);
}

void CDialogExportStxOption::SetDataDefault()
{
	std::vector<SStxOptionItem> vStxOption = m_pDataStd->m_vStxOption;
	m_pDataStd->m_vStxOption.clear();
	
	CString str;
	
	HGINT32 icCountBridge = m_pDataStd->GetBridgeSize();
	// 브라켓
	for (HGINT32 ix=0; ix<icCountBridge; ++ix)
	{
		str.Format(_T("브라켓%d"), ix+1);
		AddOptionItem(str, eStmBracketExport, ix);
	}

	// 우각부
	for (HGINT32 ix=0; ix<icCountBridge; ++ix)
	{
		str.Format(_T("우각부%d"), ix+1);
		AddOptionItem(str, eStmCornerExport, ix);
	}

	// 기초 - 시종점
	for (HGINT32 ixStt=0; ixStt<2; ++ixStt)
	{
		CString strTitle = ixStt==0 ? _T("시점 기초") : _T("종점 기초");
		for (HGINT32 ix=0; ix<icCountBridge; ++ix)
		{
			str.Format(_T("%s%d"), strTitle, ix+1);
			AddOptionItem(str, ixStt==0 ? eStmFootSttExport : eStmFootEndExport, ix);
		}
	}
	
	// 중간 기초
	
	for (HGINT32 ix=0; ix<icCountBridge; ++ix)
	{
		CRcBridgeApp *pBri = m_pDataStd->GetBridge(ix);
		if( pBri == NULL ) continue;

		for(long nJijum=0; nJijum<MAX_JIJUM_COUNT_RC; nJijum++)
		{
			if(pBri->IsFootingJijum(nJijum+1)==FALSE)
				continue;
			
			str.Format(_T("중간 기초%d 지점%d"), ix+1, nJijum+1 );
			AddOptionItem(str, eStmMidFootExport + nJijum, ix);
		}
	}
}

void CDialogExportStxOption::OnBnClickedCheckAll()
{
	HGINT32 icOption = m_pDataStd->m_vStxOption.size();
	for(HGINT32 ix = 0; ix < icOption; ++ix)
	{
		m_pDataStd->m_vStxOption[ix].check = TRUE;
	}

	SetGridData();
	SetDataInit();
}

void CDialogExportStxOption::OnBnClickedUncheckAll()
{
	HGINT32 icOption = m_pDataStd->m_vStxOption.size();
	for(HGINT32 ix = 0; ix < icOption; ++ix)
	{
		m_pDataStd->m_vStxOption[ix].check = FALSE;
	}

	SetGridData();
	SetDataInit();
}


void CDialogExportStxOption::OnBnClickedExport()
{
	SetDataSave();

	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if(pFrm == NULL) return;

	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)pFrm->GetActiveDocument();
	if(pDoc == NULL) return;

	CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;
	if(pIn == NULL) return;

	CString strName = pDoc->GetTitle();
	if(strName.Find(_T('.'))>=0)
	{
		int pos = strName.Find(_T('.'));
		strName = strName.Left(pos);
	}

	HGINT32 nIdxProgress = 1;

	CString str = _T("");
	str.Format(_T("%s (*.stx)|*.stx||"),hggettext("AStrutTie 파일"));
	CFileDialog dlg(FALSE, _T("*.stx"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);
	dlg.m_ofn.lpstrTitle = _T("AStrutTie 파일 내보내기");

	CStringArray sArrError;
	HGBOOL bSuccess = TRUE;
	if(dlg.DoModal()==IDOK)
	{
		//Progress
		HGDOUBLE dOffsetProgress = 100./(GetCountCheck()+1);
		CDlgProgress dlgProgress;
		dlgProgress.Create(IDD_DIALOG_HGPROGRESS, AfxGetMainWnd());
		dlgProgress.ShowWindow(SW_SHOW);
		dlgProgress.CenterWindow();
		dlgProgress.InitProgressData();

		HGINT32 icOption = m_pDataStd->m_vStxOption.size();
		for(HGINT32 ix = 0; ix < icOption; ++ix)
		{
			if(m_pDataStd->m_vStxOption[ix].check == FALSE) 
				continue;

			dlgProgress.SetProgressPos(m_pDataStd->m_vStxOption[ix].name + _T(" 출력중.."), (long)(nIdxProgress*dOffsetProgress));
			nIdxProgress++;

			HGINT32 iDesignType = m_pDataStd->m_vStxOption[ix].designType;
			HGINT32 idxRcBridge = m_pDataStd->m_vStxOption[ix].iRcBridge;

			CString strPathName = dlg.GetPathName();
			strPathName = strPathName.Left(strPathName.GetLength() - 4) +  _T("_") + m_pDataStd->m_vStxOption[ix].name + _T(".stx");

			CFile file;
			if(file.Open(strPathName, CFile::modeCreate|CFile::modeWrite)==FALSE) return;
			CArchive ar(&file, CArchive::store);

			CExportToAStrutTie pExport(pDoc->m_pARcBridgeDataStd);

			pExport.SetExportType(iDesignType, idxRcBridge);
			pExport.ExportStmData();

			pExport.m_pExSTM->Serialize(ar);

			ar.Close();
			file.Close();
		}

		if(bSuccess)
			AfxMessageBox(_T("완료되었습니다."));
		else
		{
			CString str;
			CString strErrorMes;
			HGINT32 icErrorPier = sArrError.GetSize();
			for(HGINT32 ix = 0; ix < icErrorPier; ++ix)
			{
				strErrorMes += sArrError.GetAt(ix);
			}
			str.Format(_T("완료되었습니다.\nFrame 해석이 되지 않은 경우 내보내기에서 제외됩니다.(교각%s)"), strErrorMes);

			AfxMessageBox(str);

		}
	}
}

// 
void CDialogExportStxOption::SetButtonImage()
{
// 	m_bmpAbutFooting.DeleteObject();
// 	m_bmpAbutFooting.LoadBitmap(IDB_EXPORT_ABUT_FOOTING);
// 	m_btnAbutFooting.SetBitmap(m_bmpAbutFooting);
// 
// 	m_bmpAbutBracket.DeleteObject();
// 	m_bmpAbutBracket.LoadBitmap(IDB_EXPORT_ABUT_BRACKET);
// 	m_btnAbutBracket.SetBitmap(m_bmpAbutBracket);
// 
// 	m_bmpPierCopingFront.DeleteObject();
// 	m_bmpPierCopingFront.LoadBitmap(IDB_EXPORT_PIER_COPING_FRONT);
// 	m_btnPierCopingFront.SetBitmap(m_bmpPierCopingFront);
// 
// 	m_bmpPierCopingSide.DeleteObject();
// 	m_bmpPierCopingSide.LoadBitmap(IDB_EXPORT_PIER_COPING_SIDE);
// 	m_btnPierCopingSide.SetBitmap(m_bmpPierCopingSide);
// 
// 	m_bmpPierFooingSide.DeleteObject();
// 	m_bmpPierFooingSide.LoadBitmap(IDB_EXPORT_PIER_FOOTING_SIDE);
// 	m_btnPierFooingSide.SetBitmap(m_bmpPierFooingSide);
// 
// 	m_bmpPierFooingFront.DeleteObject();
// 	m_bmpPierFooingFront.LoadBitmap(IDB_EXPORT_PIER_FOOTING_FRONT);
// 	m_btnPierFooingFront.SetBitmap(m_bmpPierFooingFront);
}
// 
HGINT32 CDialogExportStxOption::GetCountCheck()
{
	HGINT32 icCheck = 0;
	HGINT32 icOption = m_pDataStd->m_vStxOption.size();
	for(HGINT32 ix = 0; ix < icOption; ++ix)
	{
		if(m_pDataStd->m_vStxOption[ix].check)
			icCheck++;
	}

	return icCheck;
}


void CDialogExportStxOption::OnBnClickedBtnArccorner()
{
	HGINT32 icOption = m_pDataStd->m_vStxOption.size();
	for(HGINT32 ix = 0; ix < icOption; ++ix)
	{
		if((m_pDataStd->m_vStxOption[ix].designType == eStmCornerExport))
		{
			m_pDataStd->m_vStxOption[ix].check = TRUE;
		}
	}

	SetDataInit();
}

void CDialogExportStxOption::OnBnClickedBtnBracket()
{
	HGINT32 icOption = m_pDataStd->m_vStxOption.size();
	for(HGINT32 ix = 0; ix < icOption; ++ix)
	{
		if((m_pDataStd->m_vStxOption[ix].designType == eStmBracketExport))
		{
			m_pDataStd->m_vStxOption[ix].check = TRUE;
		}
	}

	SetDataInit();
}

void CDialogExportStxOption::OnBnClickedBtnFooting()
{
	HGINT32 icOption = m_pDataStd->m_vStxOption.size();
	for(HGINT32 ix = 0; ix < icOption; ++ix)
	{
		if( m_pDataStd->m_vStxOption[ix].designType == eStmFootSttExport ||
			m_pDataStd->m_vStxOption[ix].designType == eStmFootEndExport )
		{
			m_pDataStd->m_vStxOption[ix].check = TRUE;
		}
	}

	SetDataInit();
}


void CDialogExportStxOption::OnBnClickedBtnFootingMid()
{
	HGINT32 icOption = m_pDataStd->m_vStxOption.size();
	for(HGINT32 ix = 0; ix < icOption; ++ix)
	{
		if((m_pDataStd->m_vStxOption[ix].designType >= eStmMidFootExport))
		{
			m_pDataStd->m_vStxOption[ix].check = TRUE;
		}
	}

	SetDataInit();}

void CDialogExportStxOption::AddOptionItem( CString strName, HGINT32 iDesignType, HGINT32 iRcBridge )
{
	SStxOptionItem item;
	item.name = strName;
	item.check = TRUE;
	item.designType = iDesignType;
	item.iRcBridge = iRcBridge;

	m_pDataStd->m_vStxOption.push_back(item);
}

void CDialogExportStxOption::OnCellChangedData( NMHDR* pNotifyStruct, LRESULT* result )
{
	CCellID Cell = m_Grid.GetFocusCell();
	m_pDataStd->m_vStxOption[Cell.row - 1].check = !m_pDataStd->m_vStxOption[Cell.row - 1].check;

	SetDataInit();
}