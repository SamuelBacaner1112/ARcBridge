// RebarShearDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "RebarShearDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRebarShearDlg dialog


CRebarShearDlg::CRebarShearDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRebarShearDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRebarShearDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_CheckUpperHunch.SetCheck(FALSE);
	m_CheckLowerHunch.SetCheck(FALSE);

	m_pBri = NULL;
	m_pStd = NULL;
}

CRebarShearDlg::CRebarShearDlg(CRcBridgeRebar *pBri, CARcBridgeDataStd *pStd, CWnd* pParent /*=NULL*/)
	: CDialog(CRebarShearDlg::IDD, pParent)
{
	m_pStd	= pStd;
	m_pBri	= pBri;
}

void CRebarShearDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRebarShearDlg)
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_RECOMMOND, m_btnRecommand);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHK_UPPERSLAB, m_CheckUpperHunch);
	DDX_Control(pDX, IDC_CHK_LOWERSLAB, m_CheckLowerHunch);
}


BEGIN_MESSAGE_MAP(CRebarShearDlg, CDialog)
	//{{AFX_MSG_MAP(CRebarShearDlg)
	ON_BN_CLICKED(IDC_RECOMMOND, OnRecommond)
	ON_BN_CLICKED(IDC_CHK_UPPERSLAB,  OnCheckButtonUpperSlab)
	ON_BN_CLICKED(IDC_CHK_LOWERSLAB,  OnCheckButtonLowerSlab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRebarShearDlg message handlers

void CRebarShearDlg::SetGridData()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 1);
	m_Grid.SetCellSize(20, -1);

	if(!m_pBri) return;

	long row = 0, col = 0;
	CString str = _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
	
	CRebarShearDetail *pDetail	= &m_pBri->m_RebarShearDetail_UpperSlab;
	CStringArray sArr;
	long i = 0; for(i = 0; i < TYPE_SHEAR_SU; i++)
		sArr.Add(pDetail->GetStringType(i));

	CStringArray sArrWrap;
	for(long wrap = 0; wrap < MAX_REBAR_WRAP; wrap++)
	{
		str.Format("%d", wrap+1);
		sArrWrap.Add(str);
	}

	m_Grid.AddCell(row, col, "구분");
	m_Grid.AddCell(row, col+1, "직경");
	m_Grid.AddCell(row, col+2, "타입");
	m_Grid.AddCell(row++, col+3, "주철근감는구간수");

	m_Grid.AddCell(row, col, "상부슬래브");
	m_Grid.AddCell(row, col+1, &pDetail->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartUpperSlab, TRUE));
	m_Grid.AddCell(row, col+2, &pDetail->m_nType, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+3, &m_pBri->m_nCountWrapMainRebarUpperSlab, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArrWrap); m_Grid.SetCellBaseOfIndex(1);

	if(m_pBri->IsBoxType())
	{
		pDetail	= &m_pBri->m_RebarShearDetail_LowerSlab;
		m_Grid.AddCell(row, col, "하부슬래브");
		m_Grid.AddCell(row, col+1, &pDetail->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
		m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartLowerSlab, TRUE));
		m_Grid.AddCell(row, col+2, &pDetail->m_nType, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+3, &m_pBri->m_nCountWrapMainRebarLowerSlab, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArrWrap); m_Grid.SetCellBaseOfIndex(1);
	}

	pDetail	= &m_pBri->m_RebarShearDetail_OutWall[iSTT];
	m_Grid.AddCell(row, col, "시점벽체");
	m_Grid.AddCell(row, col+1, &pDetail->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartOutWallStt, TRUE));
	m_Grid.AddCell(row, col+2, &pDetail->m_nType, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+3, &m_pBri->m_nCountWrapMainRebarForOutWall[iSTT], nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArrWrap); m_Grid.SetCellBaseOfIndex(1);
	
	long nCountInWall	= m_pBri->GetCountInWall();
	for (HGINT32 ix = 0; ix < nCountInWall; ++ix)
	{
		CWallApp *pWall	= m_pBri->GetInWall(ix);
		if(pWall == NULL)
			continue;

		pDetail			= &pWall->m_rebarShearDetail;
		
		CString str;
		str.Format("중간벽체%d", ix+1);

		m_Grid.AddCell(row, col, str);
		m_Grid.AddCell(row, col+1, &pDetail->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
		m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartInWall, TRUE));
		m_Grid.AddCell(row, col+2, &pDetail->m_nType, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);	
		m_Grid.AddCell(row++, col+3, &pWall->m_nCountWrapMainRebar, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArrWrap); m_Grid.SetCellBaseOfIndex(1);
	}

	pDetail	= &m_pBri->m_RebarShearDetail_OutWall[iEND];
	m_Grid.AddCell(row, col, "종점벽체");
	m_Grid.AddCell(row, col+1, &pDetail->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartOutWallEnd, TRUE));
	m_Grid.AddCell(row, col+2, &pDetail->m_nType, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+3, &m_pBri->m_nCountWrapMainRebarForOutWall[iEND], nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArrWrap); m_Grid.SetCellBaseOfIndex(1);

	long nCountFooting	= m_pBri->GetCountFooting();
	if(nCountFooting > 1)
	{
		CFootingApp *pFootingStt	= m_pBri->GetFooting(0);
		if(pFootingStt)
		{
			pDetail	= &pFootingStt->m_rebarShearDetail;
			
			m_Grid.AddCell(row, col, "시점측기초");
			m_Grid.AddCell(row, col+1, &pDetail->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
			m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartFootStt, TRUE));
			m_Grid.AddCell(row, col+2, &pDetail->m_nType, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row++, col+3, &pFootingStt->m_nCountWrapMainRebar, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArrWrap); m_Grid.SetCellBaseOfIndex(1);
		}

		for (HGINT32 ix = 1; ix < nCountFooting - 1; ++ix)
		{
			CFootingApp *pFooting	= m_pBri->GetFooting(ix);
			if (pFooting == NULL)
				continue;

			pDetail	= &pFooting->m_rebarShearDetail;

			CString str;
			str.Format("중간지점기초%d", ix);
			m_Grid.AddCell(row, col, str);
			m_Grid.AddCell(row, col+1, &pDetail->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
			m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartInFoot, TRUE));
			m_Grid.AddCell(row, col+2, &pDetail->m_nType, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row++, col+3, &pFooting->m_nCountWrapMainRebar, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArrWrap); m_Grid.SetCellBaseOfIndex(1);
		}

		CFootingApp *pFootingEnd	= m_pBri->GetFooting(nCountFooting - 1);
		if (pFootingEnd && pFootingEnd != pFootingStt)
		{
			pDetail	= &pFootingEnd->m_rebarShearDetail;

			m_Grid.AddCell(row, col, "종점측기초");
			m_Grid.AddCell(row, col+1, &pDetail->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
			m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartFootEnd, TRUE));
			m_Grid.AddCell(row, col+2, &pDetail->m_nType, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row++, col+3, &pFootingEnd->m_nCountWrapMainRebar, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArrWrap); m_Grid.SetCellBaseOfIndex(1);
		}
	}

	pDetail	= &m_pBri->m_RebarShearDetail_Bracket[iSTT];
	m_Grid.AddCell(row, col, "시점브라켓");
	m_Grid.AddCell(row, col+1, &pDetail->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartUpperSlab, TRUE));
	m_Grid.AddCell(row, col+2, &pDetail->m_nType, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+3, &m_pBri->m_nCountWrapMainRebarForBracket[iSTT], nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArrWrap); m_Grid.SetCellBaseOfIndex(1);

	pDetail	= &m_pBri->m_RebarShearDetail_Bracket[iEND];
	m_Grid.AddCell(row, col, "종점브라켓");
	m_Grid.AddCell(row, col+1, &pDetail->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.SetLastCellFy(m_pBri->GetValueFy(ePartUpperSlab, TRUE));
	m_Grid.AddCell(row, col+2, &pDetail->m_nType, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+3, &m_pBri->m_nCountWrapMainRebarForBracket[iEND], nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArrWrap); m_Grid.SetCellBaseOfIndex(1);

	if(m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		pDetail	= &m_pBri->m_RebarShearDetail_ArchRib;
		m_Grid.AddCell(row, col, "리브");
		m_Grid.AddCell(row, col+1, &pDetail->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
		m_Grid.AddCell(row, col+2, &pDetail->m_nType, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+3, &m_pBri->m_nCountWrapMainRebarForArchRib, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArrWrap); m_Grid.SetCellBaseOfIndex(1);
	}
	
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidth(0, 110);
	m_Grid.SetColumnWidth(1, 50);
	m_Grid.SetColumnWidth(2, 75);
	m_Grid.SetColumnWidth(3, 120);
}

void CRebarShearDlg::SetDataInit()
{
	if(!m_pBri) return;

	m_Grid.UpdateData(FALSE);	
}

void CRebarShearDlg::SetDataSave()
{
	if(!m_pBri) return;

	m_Grid.UpdateData(TRUE);

	m_pBri->m_bExtendShearRebarToHunchRebar = m_CheckUpperHunch.GetCheck();
	m_pBri->m_bExtendShearRebarBoHunchRebar = m_CheckLowerHunch.GetCheck();
	
	SyncData();
}

void CRebarShearDlg::SetControl()
{
	m_CheckUpperHunch.SetCheck(m_pBri->m_bExtendShearRebarToHunchRebar);
	if(m_pBri->IsBoxType())
	{
		m_CheckLowerHunch.SetCheck(m_pBri->m_bExtendShearRebarBoHunchRebar);
	}
	else
	{
		m_CheckLowerHunch.SetCheck(FALSE);
		m_pBri->m_bExtendShearRebarBoHunchRebar = FALSE;
		GetDlgItem(IDC_CHK_LOWERSLAB)->EnableWindow(FALSE);		
	}
	
	UpdateData(FALSE);
}

void CRebarShearDlg::OnRecommond() 
{
	// TODO: Add your control notification handler code here
	if(!m_pBri) return;

	double dDia	= 19;
	long nType	= 0;

	m_pBri->m_RebarShearDetail_UpperSlab.m_dDia		= dDia;
	m_pBri->m_RebarShearDetail_LowerSlab.m_dDia		= dDia;
	m_pBri->m_RebarShearDetail_OutWall[iSTT].m_dDia	= dDia;
	m_pBri->m_RebarShearDetail_OutWall[iEND].m_dDia	= dDia;
	m_pBri->m_placeByBlock_ShearRebar_Bracket[iSTT].m_dDia	= dDia;
	m_pBri->m_placeByBlock_ShearRebar_Bracket[iEND].m_dDia	= dDia;
	m_pBri->m_RebarShearDetail_Bracket[iSTT].m_dDia	= dDia;
	m_pBri->m_RebarShearDetail_Bracket[iEND].m_dDia	= dDia;

	m_pBri->m_RebarShearDetail_UpperSlab.m_nType		= nType;
	m_pBri->m_RebarShearDetail_LowerSlab.m_nType		= nType;
	m_pBri->m_RebarShearDetail_OutWall[iSTT].m_nType	= nType;
	m_pBri->m_RebarShearDetail_OutWall[iEND].m_nType	= nType;
	m_pBri->m_RebarShearDetail_Bracket[iSTT].m_nType	= nType;
	m_pBri->m_RebarShearDetail_Bracket[iEND].m_nType	= nType;

	long nCountInWall	= m_pBri->GetCountInWall();
	for (HGINT32 ix = 0; ix < nCountInWall; ++ix)
	{
		CWallApp *pWall	= m_pBri->GetInWall(ix);
		if(pWall == NULL)
			continue;

		pWall->m_rebarShearDetail.m_dDia = dDia;
		pWall->m_rebarShearDetail.m_nType = nType;
	}

	long nCountFooting	= m_pBri->GetCountFooting();
	for (HGINT32 ix = 0; ix < nCountFooting; ++ix)
	{
		CFootingApp *pFooting	= m_pBri->GetFooting(ix);
		if (pFooting == NULL)
			continue;

		pFooting->m_rebarShearDetail.m_dDia = dDia;
		pFooting->m_rebarShearDetail.m_nType = nType;
	}

	SetGridData();
	SetDataInit();
	
	m_CheckUpperHunch.SetCheck(FALSE);
	m_CheckLowerHunch.SetCheck(FALSE);
	m_pBri->m_bExtendShearRebarToHunchRebar = FALSE;
	m_pBri->m_bExtendShearRebarBoHunchRebar = FALSE;
}

void CRebarShearDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CRebarShearDlg::OnOK() 
{
	// TODO: Add extra validation here
	SetDataSave();
	
	if(IsSingleHook())
	{
		AfxMessageBox(hggettext("한가닥스트럽 선택시 하면피복이 150mm 이상 적용되어야 합니다."), MB_ICONINFORMATION);
	}

	CDialog::OnOK();
}

HGBOOL CRebarShearDlg::IsSingleHook()
{
	if(m_pBri->m_RebarShearDetail_UpperSlab.m_nType==TYPE_SHEAR_SINGLE_HOOK
		|| m_pBri->m_RebarShearDetail_LowerSlab.m_nType==TYPE_SHEAR_SINGLE_HOOK
		|| m_pBri->m_RebarShearDetail_OutWall[iSTT].m_nType==TYPE_SHEAR_SINGLE_HOOK
		|| m_pBri->m_RebarShearDetail_OutWall[iEND].m_nType==TYPE_SHEAR_SINGLE_HOOK
		|| m_pBri->m_RebarShearDetail_ArchRib.m_nType==TYPE_SHEAR_SINGLE_HOOK)
		return TRUE;

	long nCountInWall	= m_pBri->GetCountInWall();
	for (HGINT32 ix = 0; ix < nCountInWall; ++ix)
	{
		CWallApp *pWall	= m_pBri->GetInWall(ix);
		if(pWall == NULL)
			continue;

		if (pWall->m_rebarShearDetail.m_nType == TYPE_SHEAR_SINGLE_HOOK)
		{
			return TRUE;
		}
	}

	long nCountFooting	= m_pBri->GetCountFooting();
	for (HGINT32 ix = 0; ix < nCountFooting; ++ix)
	{
		CFootingApp *pFooting	= m_pBri->GetFooting(ix);
		if (pFooting == NULL)
			continue;

		if (pFooting->m_rebarShearDetail.m_nType == TYPE_SHEAR_SINGLE_HOOK)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CRebarShearDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 버튼 스타일 설정

	SetControl();
	InitGridManager();
	SetGridData();
	SetDataInit();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRebarShearDlg::InitGridManager()
{
	m_Grid.InitGrid(m_pStd->m_Fy);
}

BOOL CRebarShearDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Grid.ViewGridCellForCombo(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

// 모든 부위에 대해서 지름을 입력받지 않고, 상부슬래브 상면, 첫번째 내측벽체, 외측벽체, 첫번째기초만 입력을 
// 받기 때문에 저장할때 마다 입력받지 못한 부분에 대해서 입력된 데이타를 따로 저장해 줘야 된다.
void CRebarShearDlg::SyncData()
{
	
}


void CRebarShearDlg::OnCheckButtonUpperSlab()
{

}

void CRebarShearDlg::OnCheckButtonLowerSlab()
{

}