// DialogSetExpJointSpace.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DialogSetExpJointSpace.h"


// CDialogSetExpJointSpace ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogSetExpJointSpace, CDialog)

CDialogSetExpJointSpace::CDialogSetExpJointSpace(CRcBridgeApp* pBri, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetExpJointSpace::IDD, pParent)
{
	m_pBri = pBri;
	m_nSelRadio = 0;
}

CDialogSetExpJointSpace::~CDialogSetExpJointSpace()
{
}

void CDialogSetExpJointSpace::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Radio(pDX, IDC_RADIO1, m_nSelRadio);
}


BEGIN_MESSAGE_MAP(CDialogSetExpJointSpace, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_INIT, &CDialogSetExpJointSpace::OnBnClickedButtonInit)
	ON_BN_CLICKED(IDOK, &CDialogSetExpJointSpace::OnBnClickedOk)
	ON_COMMAND_RANGE(IDC_RADIO1, IDC_RADIO3, &CDialogSetExpJointSpace::OnRadioSelect)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()


// CDialogSetExpJointSpace �޽��� ó�����Դϴ�.

BOOL CDialogSetExpJointSpace::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(hggettext("�������� ���� ���"));
	GetDlgItem(IDOK)->SetWindowText(hggettext("�������� ���� ����"));
	GetDlgItem(IDCANCEL)->SetWindowText(hggettext("���"));
	GetDlgItem(IDC_BUTTON_INIT)->SetWindowText(hggettext("�����ʱ�ȭ"));
	GetDlgItem(IDC_STATIC_GROUP_BOX)->SetWindowText(hggettext("���� ������ �ø�ó��"));
	GetDlgItem(IDC_RADIO1)->SetWindowText(hggettext("10mm ���� �ø�"));
	GetDlgItem(IDC_RADIO2)->SetWindowText(hggettext("  5mm ���� �ø�"));
	GetDlgItem(IDC_RADIO3)->SetWindowText(hggettext("  1mm ���� �ø�"));

	m_nSelRadio = m_pBri->m_pDesignExpanJoint->GetJointLenRoundLevel();
	UpdateData(FALSE);

	SetGridData();
	SetDataInit();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDialogSetExpJointSpace::OnBnClickedButtonInit()
{
	if(m_pBri == NULL)
		return;

	long nCountJijum = m_pBri->GetCountJijum();
	for(long nSe=0; nSe < 2; ++nSe)
	{
		BOOL bStt = (nSe == 0);
		CJijum *pJ = m_pBri->GetJijum(bStt? 0 : nCountJijum-1);
		if(pJ == NULL)
			continue;

		CExpansionJointData* pEx = m_pBri->m_pDesignExpanJoint->GetExpansionJointData(bStt ? 0 : 1);
		if(pEx == NULL)
			continue;

		pEx->m_dAngleJijum_For_ExpansionJoint = pJ->m_vAngle.GetAngleDegree();
		pEx->m_dExpansionJoint_width = pEx->GetAveLayingGap();
	}

	SetGridData();
	SetDataInit();
}

void CDialogSetExpJointSpace::OnBnClickedOk()
{
	if(m_pBri == NULL)
		return;

	long nCountJijum = m_pBri->GetCountJijum();
	for(long nSe=0; nSe < 2; ++nSe)
	{
		BOOL bStt = (nSe == 0);
		CJijum *pJ = m_pBri->GetJijum(bStt? 0 : nCountJijum-1);
		if(pJ == NULL)
			continue;

		double dSpace = m_pBri->m_pDesignExpanJoint->GetCalcExpansionSapce(bStt? 0 : 1);
		
		double dExt = pJ->m_dLengthUgan[bStt? JIJUM_VPOS_END : JIJUM_VPOS_STT] - dSpace;
		pJ->m_dLengthUgan[bStt? JIJUM_VPOS_END : JIJUM_VPOS_STT] = dSpace;
		m_pBri->m_dLengthJigan[bStt? 0 : nCountJijum-2] += dExt;
	}

	CDialog::OnOK();
}

void CDialogSetExpJointSpace::SetDataInit()
{
	m_Grid.UpdateData(FALSE);
	UpdateData(FALSE);
}

void CDialogSetExpJointSpace::SetGridData()
{
	if(m_pBri == NULL)
		return;

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2,2,2,1);

	long nCol = 0;
	UINT nFormat = DT_CENTER|DT_VCENTER;
	m_Grid.AddCell(0, nCol, hggettext("�� ��"));
	m_Grid.AddCell(1, nCol, hggettext("�� ��"));
	m_Grid.AddCell(2, nCol, hggettext("�������� ����"));
	m_Grid.AddCell(3, nCol, hggettext("�������� ����(mm)"));

	CString str = _T("");

	for(long nSe=0; nSe < 2; ++nSe)
	{
		BOOL bStt = (nSe == 0);
		CExpansionJointData* pEx = m_pBri->m_pDesignExpanJoint->GetExpansionJointData(bStt ? 0 : 1);
		if(pEx == NULL)
			continue;

		++nCol;
		str = bStt? _T("����") : _T("����");
		
		m_Grid.AddCell(0, nCol, str);
		if(m_pBri->IsOutWall(bStt) == FALSE)
		{
			m_Grid.AddCell(1, nCol, &pEx->m_dAngleJijum_For_ExpansionJoint); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(2, nCol, &pEx->m_dExpansionJoint_width); m_Grid.SetCellDec(CELL_DEC_0);

			str.Format(_T("%.0f"), m_pBri->m_pDesignExpanJoint->GetCalcExpansionSapce(bStt? 0 : 1));
			m_Grid.AddCell(3, nCol, str, nFormat, CELL_READONLY);
		}
		else
		{
			m_Grid.AddCell(2, nCol, _T("-"), nFormat, CELL_READONLY);
			m_Grid.AddCell(3, nCol, _T("-"), nFormat, CELL_READONLY);
			m_Grid.AddCell(4, nCol, _T("-"), nFormat, CELL_READONLY);
		}
	}
	long nSizeRow = 0;
	long nSizeCol = 0;
	m_Grid.GetCellCount(nSizeRow, nSizeCol);
	m_Grid.SetRows(nSizeRow);
	m_Grid.SetCols(nSizeCol);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(60);
	m_Grid.SetColumnWidth(0,140);
	m_Grid.MergeGrid(0, 1, 0, 0, FALSE, TRUE);
}

void CDialogSetExpJointSpace::SetDataSave()
{
	UpdateData(TRUE);
	m_Grid.UpdateData(TRUE);
}

void CDialogSetExpJointSpace::OnCellChangedData( NMHDR* pNotifyStruct, LRESULT* result )
{
	SetDataSave();
	SetGridData();
	SetDataInit();
}

void CDialogSetExpJointSpace::OnRadioSelect( WPARAM nID )
{
	long nSelID = nID - IDC_RADIO1;

	UpdateData(TRUE);
	m_pBri->m_pDesignExpanJoint->SetJointLenRoundLevel(nSelID);
	SetGridData();
	SetDataInit();
}