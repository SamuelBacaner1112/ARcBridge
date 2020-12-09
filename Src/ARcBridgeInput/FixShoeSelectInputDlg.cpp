// FixShoeSelectInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "FixShoeSelectInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFixShoeSelectInputDlg dialog


CFixShoeSelectInputDlg::CFixShoeSelectInputDlg(CARcBridgeInputStd* pIn, CWnd* pParent /*=NULL*/)
	: CUserDialog(CFixShoeSelectInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFixShoeSelectInputDlg)
	m_pIn = pIn;
	m_StrShoe.Add(_T("고정단"));
	m_StrShoe.Add(_T("교축방향"));
	m_StrShoe.Add(_T("직각방향"));
	m_StrShoe.Add(_T("양방향"));	
	//}}AFX_DATA_INIT
}


void CFixShoeSelectInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFixShoeSelectInputDlg)
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, ID_ALL_FIXFREE, m_btAllFixFree);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFixShoeSelectInputDlg, CDialog)
	//{{AFX_MSG_MAP(CFixShoeSelectInputDlg)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_BN_CLICKED(ID_ALL_FIXFREE, OnAllFixFree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFixShoeSelectInputDlg message handlers
BOOL CFixShoeSelectInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();


	SetDataInit();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFixShoeSelectInputDlg::OnOK() 
{
	// TODO: Add extra validation here
	SetDataSave();
	CDialog::OnOK();
}

void CFixShoeSelectInputDlg::OnAllFixFree() 
{
	for(long nRow=1; nRow<m_Grid.GetRowCount(); nRow++)
	{
		for(long nCol=1; nCol<m_Grid.GetColumnCount(); nCol++)
		{
			if(!m_Grid.GetItemText(nRow,nCol).IsEmpty() && m_Grid.GetItemState(nRow,nCol)!=GVIS_READONLY)
				m_Grid.SetTextMatrix(nRow, nCol, "양방향");
		}
	}

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CFixShoeSelectInputDlg::SetGridTitle()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	CJijum* pJ = NULL;
	
	long nCountJijum = pBri->GetCountJijum();
	long nFixedCols = pBri->GetCountJijumShoe()+1;
	long nCountShoe = 0;
	long nMaxShoe = 0;
	
	long i=0; for(i=0; i<nCountJijum; i++)
	{
		pJ = pBri->GetJijum(i);
		nCountShoe = pJ->m_nQtyShoe[0];
		nMaxShoe = max(nMaxShoe, nCountShoe);
		nCountShoe = pJ->m_nQtyShoe[1];
		nMaxShoe = max(nMaxShoe, nCountShoe);
	}

	long nFixedRows = nMaxShoe + 1;

	m_Grid.DeleteAllItems();
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableTitleTips(FALSE);
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.SetFixedColumnCount(1);	
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetRowCount(nFixedRows);
	m_Grid.SetColumnCount(nFixedCols);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(80);		


	m_Grid.SetTextMatrix(0,0,"구 분");
	
	CString strShoe = _T("");
	CString str = _T("");
	long nGridCol = 1;

	//최대 행 수 를 알아내어 빈 곳은 read-only한다.
	long nMaxRow = 0;
	for(long nJijum=0; nJijum<nCountJijum; nJijum++)
	{
		pJ = pBri->GetJijum(nJijum);
		for(long nL=0; nL<2; nL++)
		{			
			if(nMaxRow<pJ->m_nQtyShoe[nL])
				nMaxRow = pJ->m_nQtyShoe[nL];
		}
	}

	for(long nJ=0; nJ<nCountJijum; nJ++)
	{
		pJ = pBri->GetJijum(nJ);
		for(long nLeft=0; nLeft<2; nLeft++)
		{			
			nCountShoe = pJ->m_nQtyShoe[nLeft];
			strShoe = pJ->m_strJijumShoe[nLeft];
			
			if(pJ->m_strJijumShoe[nLeft]!=_T(""))
			{
				for(long nShoe=0; nShoe<nMaxRow; nShoe++)
				{
					if (nShoe<nCountShoe)
					{
						str.Format("L%d",nShoe+1);
						m_Grid.SetTextMatrix(nShoe+1,0,str);
						m_Grid.SetCellType(nShoe+1,nGridCol,CT_COMBO);
						m_Grid.SetTextMatrix(0,nGridCol,strShoe);
					}
					else
					{
						m_Grid.SetItemState(nShoe+1, nGridCol, m_Grid.GetItemState(nShoe+1, nGridCol) | GVIS_READONLY);
						m_Grid.SetItemBkColour(nShoe+1, nGridCol, CLR_READONLY);
						m_Grid.SetItemFgColour(nShoe+1, nGridCol, RGB(128, 128, 128));
						m_Grid.SetTextMatrix(nShoe+1, nGridCol, _T("-"));
					}
				}
				nGridCol++;
			}
		}		
	}

	m_Grid.SetComboString(m_StrShoe);
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CFixShoeSelectInputDlg::SetDataInit()
{
	SetGridTitle();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	CJijum* pJ = NULL;
	
	long nGridCol   = 1;	
	long nCountShoe  = 0;
	long nCountJijum = pBri->GetCountJijum();
	CString szData  = _T("");

	for(long nJ=0; nJ<nCountJijum; nJ++)
	{
		pJ = pBri->GetJijum(nJ);
		for(long nLeft=0; nLeft<2; nLeft++)
		{			
			nCountShoe = pJ->m_nQtyShoe[nLeft];
	
			if(pJ->m_strJijumShoe[nLeft]!=_T(""))
			{	
				for(long nShoe=0; nShoe<nCountShoe; nShoe++)
				{
					szData = GetShoeDirection(pJ->m_nDirShoe[nLeft][nShoe]);
					m_Grid.SetTextMatrix(nShoe+1,nGridCol,szData);
				}
				nGridCol++;
			}
		}		
	}
}

void CFixShoeSelectInputDlg::SetDataSave()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	CJijum* pJ = NULL;
	
	long nGridCol = 1;	
	long nCountShoe = 0;
	long nCountJijum = pBri->GetCountJijum();
	CString szData = _T("");

	for(long nJ=0; nJ<nCountJijum; nJ++)
	{
		pJ = pBri->GetJijum(nJ);
		for(long nLeft=0; nLeft<2; nLeft++)
		{			
			nCountShoe = pJ->m_nQtyShoe[nLeft];
			if(pJ->m_strJijumShoe[nLeft]!=_T(""))
			{	
				for(long nShoe=0; nShoe<nCountShoe; nShoe++)
				{
					pJ->m_nDirShoe[nLeft][nShoe] = GetShoeDirection(m_Grid.GetTextMatrix(nShoe+1,nGridCol));
				}
				nGridCol++;
			}
		}		
		
	}
}

void CFixShoeSelectInputDlg::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)pNotifyStruct;	
	long nRow = GridView->iRow;
    long nCol = GridView->iColumn;
	if(m_Grid.GetItemText(nRow, nCol)=="고정단")
	{
		SetShoeDirection(nRow, nCol);
	}
}

long CFixShoeSelectInputDlg::GetShoeDirection(CString str)
{
	long Dir = -1;

	if( str == "고정단" )	  return 0;
	if( str == "교축방향" )   return 1;
	if( str == "직각방향" )	  return 2;
	if( str == "양방향" )	  return 3;	

	return Dir;
}

CString CFixShoeSelectInputDlg::GetShoeDirection(long Dir)
// Dir값에 따른 슈방향 텍스트 리턴
{
	CString str;
	switch(Dir)
	{
		case 0: str = "고정단";		break;
		case 1: str = "교축방향";	break;
		case 2: str = "직각방향";	break;
		case 3: str = "양방향";		break;		
	}

	return str;
}

void CFixShoeSelectInputDlg::SetShoeDirection(long nRow, long nCol)
{
	CString strJijum = _T("");

	for(long nR=1; nR<m_Grid.GetRowCount(); nR++)
	{
		if(nR==nRow) continue;
		if(!m_Grid.GetItemText(nR, nCol).IsEmpty())
		{
			if(m_Grid.GetItemText(nR, nCol)=="고정단") break;
			m_Grid.SetTextMatrix(nR, nCol, "직각방향");
		}
	}

	
	long nC=0;
	for(nC=nCol+1; nC<m_Grid.GetColumnCount(); nC++)
	{
		strJijum = m_Grid.GetTextMatrix(0,nC);
		
		long nPosLeft = strJijum.Find("L");			//nCol+1부터 신축이음 L이 있는지 확인
		long nPosRight = strJijum.Find("R");
		if(nPosRight!=-1) break;					//현재 고정단이 L의 위치에 있는지 확인
		if(!m_Grid.GetItemText(nRow,nC).IsEmpty() && m_Grid.GetItemState(nRow, nC)!=GVIS_READONLY)
		{
			if(m_Grid.GetItemText(nRow, nC)=="고정단") break;
			m_Grid.SetTextMatrix(nRow, nC, "교축방향");
		}
		else break;
		if(nPosLeft!=-1) break;
	}

	for(nC=nCol-1; nC>0; nC--)
	{
		strJijum = m_Grid.GetTextMatrix(0,nC);
		long nPosLeft = strJijum.Find("L");			
		long nPosRight = strJijum.Find("R");			//nCol-1부터 신축이음 R이 있는지 확인
		if(nPosLeft!=-1) break;						//현재 고정단이 R의 위치에 있는지 확인
		if(!m_Grid.GetItemText(nRow,nC).IsEmpty() && m_Grid.GetItemState(nRow, nC)!=GVIS_READONLY)
		{
			if(m_Grid.GetItemText(nRow, nC)=="고정단") break;
			m_Grid.SetTextMatrix(nRow, nC, "교축방향");
		}
		else break;
		if(nPosRight!=-1) break;
	}

	m_Grid.Invalidate();
}
