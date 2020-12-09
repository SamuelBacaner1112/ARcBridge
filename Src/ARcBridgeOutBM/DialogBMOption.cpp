// DialogBMOption.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "resource.h"
#include "arcbridgeoutbm.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"


// CDialogBMOption ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogBMOption, CXTResizeDialog)

CDialogBMOption::CDialogBMOption(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CDialogBMOption::IDD, pParent)
{
 	m_nTabOut	 = -1;
 	m_nTabNormal = -1;
 	m_nTabForm	 = -1;
 	m_nTab		 = 0;
	m_nBri		 = 0;
 	m_pStd		 = NULL;

	m_nCurPier = -1;
	m_nCurAbut = -1;
}

CDialogBMOption::~CDialogBMOption()
{
}

void CDialogBMOption::DoDataExchange(CDataExchange* pDX)
{
 	CDialog::DoDataExchange(pDX);
 	DDX_Control(pDX, IDC_BUTTON_BM_SAME, m_btnSame);
 	DDX_Control(pDX, IDC_BUTTON_BM_RECOMMAND, m_btnRecommand);
 	DDX_Control(pDX, IDC_COMBO, m_comboBridge);
 	DDX_Control(pDX, IDOK, m_btnOK);
 	DDX_Control(pDX, IDCANCEL, m_btnCancel);
 	DDX_Control(pDX, IDC_TAB_OPT_OUTBM, m_ctlTab);
 	DDX_GridControl(pDX, IDC_GRID_ORG_OUTBM, m_Grid);
}


BEGIN_MESSAGE_MAP(CDialogBMOption, CXTResizeDialog)
 	ON_CBN_SELCHANGE(IDC_COMBO, OnSelchangeCombo)
 	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_OPT_OUTBM, OnTabSelchange)
 	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_ORG_OUTBM, OnCellChangedData)
 	ON_BN_CLICKED(IDC_BUTTON_BM_RECOMMAND, OnButtonRecommand)
 	ON_BN_CLICKED(IDC_BUTTON_BM_SAME, OnButtonSame)
END_MESSAGE_MAP()


void CDialogBMOption::OnCancel() 
{
	// TODO: Add extra cleanup here

	CXTResizeDialog::OnCancel();
}

void CDialogBMOption::OnOK() 
{
	// TODO: Add extra validation here
	m_Grid.UpdateData(TRUE);
	CXTResizeDialog::OnOK();
}

BOOL CDialogBMOption::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();

	// TODO: Add extra initialization here
	SetResize(IDC_GRID_ORG_OUTBM, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB_OPT_OUTBM, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK,  SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,  SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_BM_RECOMMAND,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_BM_SAME,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	GetDlgItem(IDOK)->SetWindowText(hggettext("Ȯ��"));
	GetDlgItem(IDCANCEL)->SetWindowText(hggettext("���"));
	GetDlgItem(IDC_BUTTON_BM_RECOMMAND)->SetWindowText(hggettext("�ǰ��"));
	GetDlgItem(IDC_BUTTON_BM_SAME)->SetWindowText(hggettext("��� ���� ��������"));

	InitComboCtrl();
	InitTabCtrl();
	EnableControl();
	SetGridData();
	SetDataInit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogBMOption::SetGridData()
{
 	if(m_nTab == m_nTabOut) SetGridDataOut();
 	else if(m_nTab == m_nTabNormal) SetGridDataNormal();
 	else if(m_nTab == m_nTabForm) SetGridDataForm();
}


void CDialogBMOption::SetDataInit()
{
 	m_Grid.UpdateData(FALSE);
 
 	long nRows(0), nCols(0);
 	m_Grid.GetCellCount(nRows, nCols);

	if(m_nTab == m_nTabOut)
	{
		m_Grid.MergeGrid(1, nRows-1, 1, 2, TRUE, FALSE);
		m_Grid.MergeGrid(0, nRows-1, 3, 4, TRUE, FALSE);
	}
	else if (m_nTab == m_nTabNormal)
	{
		m_Grid.MergeGrid(1, nRows-1, 0, 1, TRUE, FALSE);
	}
	
	m_Grid.MergeGrid(0,0,0,nCols-1);
	m_Grid.MergeGridEx(0, nRows - 1, 0, 0, TRUE, TRUE);
}

void CDialogBMOption::SetDataSave()
{
 	m_Grid.UpdateData(TRUE);
}

void CDialogBMOption::InitTabCtrl()
{
 	m_ctlTab.SendInitialUpdate(TRUE);
 
 	// Clip children of the tab control from paint routines to reduce flicker.
 	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
 
 	m_ctlTab.DeleteAllItems();
 
 	m_nTabOut		= -1;
 	m_nTabNormal	= -1;
 	m_nTabForm		= -1;
 	m_nTab			= 0;
 	long nIdx		= 0;
 
 	m_ctlTab.InsertItem(nIdx, hggettext("��¿ɼ�")); m_nTabOut	= nIdx++;
 	m_ctlTab.InsertItem(nIdx, hggettext("�Ϲݼ��� �ɼ�")); m_nTabNormal	= nIdx++;
 	m_ctlTab.InsertItem(nIdx, hggettext("��Ǫ������ �ɼ�")); m_nTabForm	= nIdx++;
 
 	m_ctlTab.SetCurSel(m_nTab);
}

//��� �ɼ�
void CDialogBMOption::SetGridDataOut()
{
	CARcBridgeDataStd *pDataStd	= m_pStd->m_pARcBridgeDataStd;

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 5, 1, 1);
	m_Grid.SetColumnWidth(0,250);
	m_Grid.SetColumnWidth(1,60);
	m_Grid.SetColumnWidth(2,200);

	UINT nFormat = DT_CENTER|DT_VCENTER;
	
	CStringArray sArr;
	long row = 0;
	long col = 0;
	m_Grid.AddCell(row, col,	  "��  ��");
	m_Grid.AddCell(row, col+1, "������");
	m_Grid.AddCell(row++, col+2, "������");
	
	m_Grid.AddCell(row, col, "  ��ü�� ������ ö�ټ��� �и�����", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, &m_pStd->m_bSeparateExcludeRebarMonument,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+2, m_pStd->m_bSeparateExcludeRebarMonument ? "������" : "�������", DT_RIGHT);

	m_Grid.AddCell(row, col, "  �𵵿� ������ �� ǥ��", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, &m_pStd->m_bDrawElEarth,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+2, m_pStd->m_bDrawElEarth ? "ǥ����" : "ǥ������", DT_RIGHT);

	m_Grid.AddCell(row, col, "  ���� ��½� ����ٰ� ���ĺ�ȯ", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, &m_pStd->m_bConvertToForm,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+2, m_pStd->m_bConvertToForm ? "��ȯ��" : "��ȯ����", DT_RIGHT);

	m_Grid.AddCell(row, col, "  ����ǥ�� �� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, &m_pStd->m_bRefCellBMSum,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+2, m_pStd->m_bRefCellBMSum ? "������" : "��������", DT_RIGHT);

	m_Grid.AddCell(row, col, "  ����� ���ʸ� �ٴ� �� ���� �и�", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, &pDataStd->m_bSepFootingForSiljuk,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+2, pDataStd->m_bSepFootingForSiljuk ? "�и���" : "�и�����", DT_RIGHT);

	sArr.RemoveAll(); 
	sArr.Add("�Ϲݼ����������");
	sArr.Add("���������������(��������и�)");
	sArr.Add("���������������(�����������)");
	m_Grid.AddCell(row, col, "  �������� �������", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, &pDataStd->m_nTypeBMApply, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+2, GetStringArray(sArr, pDataStd->m_nTypeBMApply), DT_RIGHT);
}

//�Ϲݼ���
void CDialogBMOption::SetGridDataNormal()
{
	CARcBridgeDataStd *pStd	= m_pStd->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_nBri);
	if(!pBri) return;

	const HGBOOL bLsd = pStd->IsLsdDesign();

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(3, 5, 1, 2);
	m_Grid.SetColumnWidth(0,150);
	m_Grid.SetColumnWidth(1,220);
	m_Grid.SetColumnWidth(2,240);
	m_Grid.SetColumnWidth(3,50);
	m_Grid.SetColumnWidth(4,40);

	CStringArray sArr;
	long row = 0;
	long col = 0;
	m_Grid.AddCell(row, col,	"��  ��");
	m_Grid.AddCell(row, col+1, "��  ��");
	m_Grid.AddCell(row, col+2, "������");
	m_Grid.AddCell(row, col+3, "��  ��");
	m_Grid.AddCell(row++, col+4, "��  ��");

	if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
	{
		m_Grid.AddCell(row, col, "  ��ũ��Ʈ ���", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  ��ü, ������", GVHT_LEFT);
	}
	else
	{
		m_Grid.AddCell(row, col, "  ��ũ��Ʈ ���", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  ��ü", GVHT_LEFT);
	}
	sArr.RemoveAll();
	sArr.Add("����");
	sArr.Add("�������");
	m_Grid.AddCell(row++, col+2, &pBri->m_bSelectBM_ConcCare_Main, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
	{
		m_Grid.AddCell(row, col, "  ��ũ��Ʈ ���", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  ���ӽ�����,���潽����", GVHT_LEFT);
		m_Grid.AddCell(row++, col+2, &pBri->m_bSelectBM_ConcCare_Aps, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		m_Grid.AddCell(row, col, "  ��ũ��Ʈ ���", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  ��ũ��Ʈ ��� ������", GVHT_LEFT);
		sArr.RemoveAll();
		sArr.Add("���ʸ� ������ ��Ǫ������+��μ����");
		sArr.Add("���ʸ� ������ �����");
		m_Grid.AddCell(row++, col+2, &pBri->m_nSelectBM_ConcCare_Cal, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		m_Grid.AddCell(row, col, "  ���뺸ȣ�� ��ġ", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  ���뺸ȣ�� ��ġ", GVHT_LEFT);
		sArr.RemoveAll();
		sArr.Add("����");
		sArr.Add("��õ��");
		sArr.Add("������");
		m_Grid.AddCell(row++, col+2, &pBri->m_nTypeRcBridgeBlock, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		m_Grid.AddCell(row, col, "  ���뺸ȣ�� ������ġ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  ���뺸ȣ�� ������ġ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pBri->m_dAngleRcBridgeBlock, DT_RIGHT);
		m_Grid.AddCell(row++, col+3, "��", DT_LEFT);

		m_Grid.AddCell(row, col, "  �������迡 ���ӽ�����,���潽���� ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  �������迡 ���ӽ�����,���潽���� ����", GVHT_LEFT);
		sArr.RemoveAll();
		sArr.Add("����");
		sArr.Add("�������");
		m_Grid.AddCell(row++, col+2, &pBri->m_bSelectBM_ApsCnsInclude, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	}

	sArr.RemoveAll();
	sArr.Add("�����߽� ���� ����");
	sArr.Add("��� ���� ����((�ִ�+�ּ�)/2)");
	sArr.Add("�ִ� ���� ����");
	sArr.Add("������ ��� ���� ����((�ִ�+�ּ�)/2)");
	m_Grid.AddCell(row, col, "  ��ü ���� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ��ü ���� ����", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nTypeHeightWall, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	sArr.RemoveAll();
	sArr.Add("���Ÿ�");
	sArr.Add("�����Ÿ�");
	m_Grid.AddCell(row, col, "  ������ ���� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ������ ���� ����", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nTypeSlabLength, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	
	sArr.RemoveAll();
	sArr.Add("ħ����");
	sArr.Add("��Ʈ���");
	m_Grid.AddCell(row, col, "  ������ Ÿ��", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ������ Ÿ��", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nTypeSurfaceProof, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	sArr.RemoveAll();
	sArr.Add("���ʻ�� ����");
	sArr.Add("���ݰ� ����");
	sArr.Add("������ġ �Է�");
	m_Grid.AddCell(row, col, "  ���ٸ� ���� ���� ������ġ", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ���ٸ� ���� ���� ������ġ", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_nTypePostBaseLine, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col+3, "EL.(m)", GVHT_LEFT);
	m_Grid.AddCell(row++, col+4, &pBri->m_dPostLineEL, DT_RIGHT, pBri->m_nTypePostBaseLine != 2 ? CELL_READONLY : CELL_LINE_NO);

	m_Grid.AddCell(row, col, "���\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ��� ���� ���� ������ġ", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_nTypeScaffoldBaseLine, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col+3, "EL.(m)", GVHT_LEFT);
	m_Grid.AddCell(row++, col+4, &pBri->m_dScaffoldLineEL, DT_RIGHT, pBri->m_nTypeScaffoldBaseLine != 2 ? CELL_READONLY : CELL_LINE_NO);

	m_Grid.AddCell(row, col, "���\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ��� �β�", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_dThickScaffold, DT_RIGHT, CELL_LINE_NO);
	m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);

	CStringArray sArrWing;
	sArrWing.Add(hggettext("���������� ����(����ǹ����.2016)"));
	sArrWing.Add(hggettext("���������� ����"));
	m_Grid.AddCell(row, col, "���\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ��� ���� ���� ��ġ", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_nTypeScaffoldSideLocation, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArrWing);
	m_Grid.AddCell(row++, col+4, "", DT_RIGHT, CELL_READONLY);
		
	sArr.RemoveAll();
	sArr.Add("������");
	sArr.Add("�Ϲݿ�");
	m_Grid.AddCell(row, col, "  ��ü ���ٸ� Ÿ��", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ��ü ���ٸ� Ÿ��", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nTypePost, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	sArr.RemoveAll();
	sArr.Add("��������");
	sArr.Add("��������");
	sArr.Add("��������");
	m_Grid.AddCell(row, col, "  ����������", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ���������� ��Ǫ�� ��������", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nDeductNearFormBM, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	m_Grid.AddCell(row, col, "  ����������", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ���������� ���ı� ��������", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nDeductNearTupagiBM, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	if (bLsd)
	{
		m_Grid.AddCell(row, col, "  ��� ����ġ �� �Է�", DT_LEFT);
		m_Grid.AddCell(row, col+1, "  ��� ����ġ �� �Է�", DT_LEFT);

		sArr.RemoveAll();
		sArr.Add("����");
		sArr.Add("������");
		m_Grid.AddCell(row++, col+2, &pBri->m_bAggregate_SlumpDetail, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		const HGINT32 iEnd = pBri->m_bAggregate_SlumpDetail ? ePartCount : ePartUpperSlab + 1;
		for (HGINT32 ix = ePartUpperSlab; ix < iEnd; ++ix)
		{
			CString strPart;

			switch (ix)
			{
			case ePartUpperSlab:
				strPart = "��ü/��ν�����";
				break;
			case ePartInWall:
				strPart = "�߰�������:��ü";
				break;
			case ePartInFoot:
				strPart = "�߰�������:����";
				break;
			default:
				strPart = pBri->GetStringPart(static_cast<EPartOfBridge>(ix));
			}

			m_Grid.AddCell(row, col, "  ��ũ��Ʈ�����ִ�ġ��", DT_LEFT);
			m_Grid.AddCell(row, col+1, "  " + strPart, DT_LEFT,CELL_LINE_NO);
			m_Grid.AddCell(row, col+2, &pBri->m_dAggregate[ix], DT_RIGHT);
			m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);
		}

		if (pBri->m_bAggregate_SlumpDetail == FALSE)
		{
			m_Grid.AddCell(row, col, "  ��ũ��Ʈ�����ִ�ġ��", DT_LEFT);
			m_Grid.AddCell(row, col+1, "  " + pBri->GetStringPart(ePartApsStt), DT_LEFT,CELL_LINE_NO);
			m_Grid.AddCell(row, col+2, &pBri->m_dAggregate[ePartApsStt], DT_RIGHT);
			m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);
		}
	}
	else
	{
		// �����ִ�ġ��
		m_Grid.AddCell(row, col, "  ��ũ��Ʈ�����ִ�ġ��", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pBri->m_dAggregate_Footing, DT_RIGHT, CELL_LINE_NO);
		m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);

		m_Grid.AddCell(row, col, "  ��ũ��Ʈ�����ִ�ġ��", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  ��ü", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pBri->m_dAggregate_Main, DT_RIGHT, CELL_LINE_NO);
		m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);

		m_Grid.AddCell(row, col, "  ��ũ��Ʈ�����ִ�ġ��", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  ���ӽ�����", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pBri->m_dAggregate_Aps, DT_RIGHT, CELL_LINE_NO);
		m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);
	}

	m_Grid.AddCell(row, col, "  ��ũ��Ʈ�����ִ�ġ��", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ������ũ��Ʈ", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, bLsd ? &pBri->m_dAggregate_LeanLSD : &pBri->m_dAggregate_Lean, DT_RIGHT, CELL_LINE_NO);
	m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  ��ũ��Ʈ�����ִ�ġ��", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  MASS", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, bLsd ? &pBri->m_dAggregate_MassLSD : &pBri->m_dAggregate_Mass, DT_RIGHT, CELL_LINE_NO);
	m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  ��ũ��Ʈ�����ִ�ġ��", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ����Ÿ������", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, bLsd ? &pBri->m_dAggregate_HyuntaLSD : &pBri->m_dAggregate_Hyunta, DT_RIGHT, CELL_LINE_NO);
	m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  ��ũ��Ʈ�����ִ�ġ��", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ��ȣ��", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, bLsd ? &pBri->m_dAggregate_EtcLSD : &pBri->m_dAggregate_Etc, DT_RIGHT, CELL_LINE_NO);
	m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);

	if (bLsd)
	{
		const HGINT32 iEnd = pBri->m_bAggregate_SlumpDetail ? ePartCount : ePartUpperSlab + 1;
		for (HGINT32 ix = ePartUpperSlab; ix < iEnd; ++ix)
		{
			CString strPart;

			switch (ix)
			{
			case ePartUpperSlab:
				strPart = "��ü/��ν�����";
				break;
			case ePartInWall:
				strPart = "�߰�������:��ü";
				break;
			case ePartInFoot:
				strPart = "�߰�������:����";
				break;
			default:
				strPart = pBri->GetStringPart(static_cast<EPartOfBridge>(ix));
			}

			m_Grid.AddCell(row, col, "  ��ũ��ƮSlump", DT_LEFT);
			m_Grid.AddCell(row, col+1, "  " + strPart, DT_LEFT,CELL_LINE_NO);
			m_Grid.AddCell(row, col+2, &pBri->m_dSlump[ix], DT_RIGHT);
			m_Grid.AddCell(row++, col+3, "cm", GVHT_LEFT);
		}

		if (pBri->m_bAggregate_SlumpDetail == FALSE)
		{
			m_Grid.AddCell(row, col, "  ��ũ��ƮSlump", DT_LEFT);
			m_Grid.AddCell(row, col+1, "  " + pBri->GetStringPart(ePartApsStt), DT_LEFT,CELL_LINE_NO);
			m_Grid.AddCell(row, col+2, &pBri->m_dSlump[ePartApsStt], DT_RIGHT);
			m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);
		}
	}
	else
	{
		// slump
		// slump�� ������ mm(������ ���� ����) 2007.11.26 ����
		CString szUnit_CGS   = "cm";
		CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

		m_Grid.AddCell(row, col, "  ��ũ��ƮSlump", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pBri->m_dSlump_Footing, DT_RIGHT, CELL_LINE_NO);// m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		m_Grid.AddCell(row++, col+3, "cm", GVHT_LEFT);

		m_Grid.AddCell(row, col, "  ��ũ��ƮSlump", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  ��ü", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pBri->m_dSlump_Main, DT_RIGHT, CELL_LINE_NO); // m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		m_Grid.AddCell(row++, col+3, "cm", GVHT_LEFT);

		m_Grid.AddCell(row, col, "  ��ũ��ƮSlump", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  ���ӽ�����", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pBri->m_dSlump_Aps, DT_RIGHT, CELL_LINE_NO); // m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		m_Grid.AddCell(row++, col+3, "cm", GVHT_LEFT);
	}

	m_Grid.AddCell(row, col, "  ��ũ��ƮSlump", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ������ũ��Ʈ", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, bLsd ? &pBri->m_dSlump_LeanLSD : &pBri->m_dSlump_Lean, DT_RIGHT, CELL_LINE_NO); // m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, "cm", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  ��ũ��ƮSlump", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  MASS", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, bLsd ? &pBri->m_dSlump_MassLSD : &pBri->m_dSlump_Mass, DT_RIGHT, CELL_LINE_NO); // m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, "cm", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  ��ũ��ƮSlump", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ����Ÿ������", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, bLsd ? &pBri->m_dSlump_HyuntaLSD : &pBri->m_dSlump_Hyunta, DT_RIGHT, CELL_LINE_NO); // m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, "cm", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  ��ũ��ƮSlump", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ��ȣ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, bLsd ? &pBri->m_dSlump_EtcLSD : &pBri->m_dSlump_Etc, DT_RIGHT, CELL_LINE_NO); // m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, "cm", GVHT_LEFT);

	sArr.RemoveAll();
	sArr.Add("�������");
	sArr.Add("1��");
	sArr.Add("2��");
	sArr.Add("3��");
	sArr.Add("4��");

// (ARCBRIDGE-3182) ������ ��ħ ���� �Է� ���� �̵�	
// 	m_Grid.AddCell(row, col, "  ���� �������� ������ ��ħ ��ġ", GVHT_LEFT);
// 	m_Grid.AddCell(row, col+1, "  ���� �������� ������ ��ħ ��ġ", GVHT_LEFT);
// 	m_Grid.AddCell(row++, col+2, &pBri->m_nMonumenu, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	m_Grid.AddCell(row, col, "  ������ ��ġ", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ������ ��ġ", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nDesPanel, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	m_Grid.AddCell(row, col, "  ������ ��ġ", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ������ ��ġ", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nNamePanel, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	sArr.RemoveAll();
	sArr.Add("������ ���� ���̿� ����");
	sArr.Add(" 0 ~ 7m");
	sArr.Add(" 7 ~ 10m");
	sArr.Add("10 ~ 13m");
	sArr.Add("13 ~ 16m");
	sArr.Add("16 ~ 19m");
	sArr.Add("19m �̻�");
	m_Grid.AddCell(row, col, "  ������ ��Ǫ�� ��������� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ������ ��Ǫ�� ��������� ����", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nHeightForSlabBM, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

// 	// �������� ���� #38979
// 	for(long stt = 0; stt < 2; stt++)
// 	{
// 		BOOL bStt	= stt == iSTT;
// 		CString sStt	= bStt ? "  ��������(����)" : "  ��������(����)";
// 		CFlexibleJoint *pFx	= bStt ? &pBri->m_fxJointStt : &pBri->m_fxJointEnd;
// 
// 		m_Grid.AddCell(row, col, sStt, GVHT_LEFT);
// 		m_Grid.AddCell(row, col+1, "  ��Ī", GVHT_LEFT);
// 		m_Grid.AddCell(row++, col+2, &pFx->m_sName);
// 
// 		m_Grid.AddCell(row, col, sStt, GVHT_LEFT);
// 		m_Grid.AddCell(row, col+1, "  ö�ٷ�1", GVHT_LEFT);
// 		m_Grid.AddCell(row++, col+2, &pFx->m_rb[0].m_dDia, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_DIA);
// 
// 		m_Grid.AddCell(row, col, sStt, GVHT_LEFT);
// 		m_Grid.AddCell(row, col+1, "  ö�ٷ�1", GVHT_LEFT);
// 		m_Grid.AddCell(row, col+2, &pFx->m_rb[0].m_dLengthTotal);
// 		m_Grid.AddCell(row++, col+3, "tonf");
// 
// 		m_Grid.AddCell(row, col, sStt, GVHT_LEFT);
// 		m_Grid.AddCell(row, col+1, "  ö�ٷ�2", GVHT_LEFT);
// 		m_Grid.AddCell(row++, col+2, &pFx->m_rb[1].m_dDia, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_DIA);
// 		
// 		m_Grid.AddCell(row, col, sStt, GVHT_LEFT);
// 		m_Grid.AddCell(row, col+1, "  ö�ٷ�2", GVHT_LEFT);
// 		m_Grid.AddCell(row, col+2, &pFx->m_rb[1].m_dLengthTotal);
// 		m_Grid.AddCell(row++, col+3, "tonf");
// 	}

	m_Grid.AddCell(row, col, "  �� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  �� ����", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_dTackCotting, DT_RIGHT, CELL_LINE_NO);

	sArr.RemoveAll();
	sArr.Add("��(D+0.2)��/4 x L");
	sArr.Add("(��(D+0.2)��/4 - ��D��/4) x L");
	m_Grid.AddCell(row, col, "  ���α������� �ø�Ʈ ��ũ�� ������", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ���α������� �ø�Ʈ ��ũ�� ������", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_nTypePileMilkBM, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	
	m_Grid.AddCell(row, col, "  ö�ٰ����������� ������ ���� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ö�ٰ����������� ������ ���� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_bExcludeApsDowelRebar, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_CHECK);	
	m_Grid.AddCell(row++, col+3, pBri->m_bExcludeApsDowelRebar ? "����" : "�������", DT_RIGHT);

	m_Grid.AddCell(row, col, "  ��Ǫ��", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ��ũ��Ʈ ���, ��Ǫ�� ��ü", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_dTimeConcForm, DT_RIGHT, CELL_LINE_NO);
	m_Grid.AddCell(row++, col+3, "hr", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  ������ı� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ������ı� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_bApplyYongSu, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_CHECK);	
	m_Grid.AddCell(row++, col+3, pBri->m_bApplyYongSu ? "����" : "�������", DT_RIGHT);

	m_Grid.AddCell(row, col, "  ����Ÿ������", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ����Ÿ������ ����ó��", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_dSurplusSoil_HyunTaPile, DT_RIGHT, CELL_LINE_NO);// m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, "%", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  ����Ÿ������", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ����Ÿ������ ��ũ��Ʈ Ÿ��(���)", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_dConcreteSoil_HyunTaPile, DT_RIGHT, CELL_LINE_NO); // m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, "����(%)", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  ����Ÿ������", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  ����Ÿ������ ��ũ��Ʈ Ÿ��(�Ϲ�)", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_dConcreteRock_HyunTaPile, DT_RIGHT, CELL_LINE_NO); // m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, "����(%)", GVHT_LEFT);
}

//��Ǫ��
void CDialogBMOption::SetGridDataForm()
{
	CARcBridgeDataStd *pStd	= m_pStd->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_nBri);
	if(!pBri) return;

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 5, 1, 2);
	m_Grid.SetColumnWidth(0,150);
	m_Grid.SetColumnWidth(1,200);
	m_Grid.SetColumnWidth(2,190);
	m_Grid.SetColumnWidth(3,40);
	m_Grid.SetColumnWidth(4,40);

	CStringArray sArr;
	long row = 0;
	long col = 0;
	m_Grid.AddCell(row, col,	"��  ��");
	m_Grid.AddCell(row, col+1, "��  ��");
	m_Grid.AddCell(row, col+2, "������");
	m_Grid.AddCell(row, col+3, "��  ��");
	m_Grid.AddCell(row++, col+4, "��  ��");

// 	if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
// 	{
// 		m_Grid.AddCell(row, col, "  ��Ǫ��", GVHT_LEFT);
// 	}
// 	else
// 	{
		m_Grid.AddCell(row, col, "  ��Ǫ��", GVHT_LEFT);
// 	}
	m_Grid.AddCell(row, col+1, "  �Ϲ���ũ��Ʈ", GVHT_LEFT);
	sArr.RemoveAll();
	if(pStd->m_nTypeBMApply == BMTYPE_NORMAL)
	{
		long i=0; for(i=0; i<MAX_BM_FORM_TYPE_NORMAL; i++)
			sArr.Add(pStd->GetStringBM_FormType(i));
	}
	else
	{
		long i=0; for(i=0; i<MAX_BM_FORM_TYPE; i++)
			sArr.Add(pStd->GetStringBM_FormType(i));
	}
	m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_ConcNormal, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	// �����걳�϶� �Ϻ��� ������ �Է¹��� ����
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
	{
		m_Grid.AddCell(row, col, "  ��Ǫ��", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  ���ʺ�", GVHT_LEFT);
		m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_Footing, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		m_Grid.AddCell(row, col, "  ��Ǫ��", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  ��ü����", GVHT_LEFT);
		m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_WallFront, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		m_Grid.AddCell(row, col, "  ��Ǫ��", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  ��ü���", GVHT_LEFT);
		m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_WallBack, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		for(long stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			for(long left = 0; left < 2; left++)
			{
				BOOL bLeft	= left == iLEFT;
				CString sWing	= _T("");
				sWing.Format("  %s ������(%s)", bStt ? "����" : "����", bLeft ? "��" : "��");

				m_Grid.AddCell(row, col, "  ��Ǫ��", GVHT_LEFT);
				m_Grid.AddCell(row, col+1, sWing+"����", GVHT_LEFT);
				m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_WingFront[stt][left], DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

				m_Grid.AddCell(row, col, "  ��Ǫ��", GVHT_LEFT);
				m_Grid.AddCell(row, col+1, sWing+"���", GVHT_LEFT);
				m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_WingBack[stt][left], DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
			}
		}

		m_Grid.AddCell(row, col, "  ��Ǫ��", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  ���ӽ�����", GVHT_LEFT);
		m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_Aps, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		m_Grid.AddCell(row, col, "  ��Ǫ��", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  MASS ��ũ��Ʈ", GVHT_LEFT);
		m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_MassConc, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		m_Grid.AddCell(row, col, "  ��Ǫ��", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  LEAN ��ũ��Ʈ", GVHT_LEFT);
		m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_LeanConc, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	}
}

 void CDialogBMOption::SetOutBMStd(CARcBridgeOutBMStd *pStd)
 {
	m_pStd = pStd;
 }

void CDialogBMOption::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
 	m_nTab	= m_ctlTab.GetCurSel();

	EnableControl();
 	SetGridData();
 	SetDataInit();
 
 	return;
}

void CDialogBMOption::OnSelchangeCombo() 
{
	m_nBri	= m_comboBridge.GetCurSel();
	SetGridData();
	SetDataInit();
}

void CDialogBMOption::InitComboCtrl()
{
	CARcBridgeDataStd *pStd	= m_pStd->m_pARcBridgeDataStd;
	if(!pStd) return;

	// �޺�����
	long i = 0; for(i = 0; i < m_comboBridge.GetCount(); i++)
		m_comboBridge.DeleteString(0);

	long nCountBridge	= pStd->GetBridgeSize();
	for(i = 0; i < nCountBridge; i++)
		m_comboBridge.AddString(pStd->GetBridgeTotalName(i));
	
	m_comboBridge.SetCurSel(m_nBri);

}

void CDialogBMOption::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{	
	CARcBridgeDataStd* pStd = m_pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_nBri);
	if (pBri == NULL)
		return;

	CCellID cell = m_Grid.GetFocusCell();
	CString str  = m_Grid.GetTextMatrix(cell.row, 0);
	CString str1  = m_Grid.GetTextMatrix(cell.row, 1);

	if (str == "  ��� ����ġ �� �Է�")
	{
		SyncAggregateAndSlump(pBri);
	}
	
	if (pBri->m_bAggregate_SlumpDetail == FALSE)
	{
		if (str1 == "  ��ü/��ν�����" || str1 == "  " + pBri->GetStringPart(ePartApsStt))
		{
			SyncAggregateAndSlump(pBri);
		}
		
	}
	
	SetDataSave();

	if(str ==  _T("  ��Ǫ��"))	pBri->m_dTimeConcForm = pBri->m_dTimeConcForm < 0? 0 : pBri->m_dTimeConcForm;
 	SetGridData();
 	SetDataInit();

}

BOOL CDialogBMOption::PreTranslateMessage(MSG* pMsg) 
{
	//TODO: Add your specialized code here and/or call the base class
 	m_Grid.ViewGridCellForCombo(pMsg);
 	
 	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

CString CDialogBMOption::GetStringArray(CStringArray &sArr, long nIdx)
{
 	if(nIdx < 0 || nIdx >= sArr.GetSize()) return _T("");
 	return sArr.GetAt(nIdx);
}

void CDialogBMOption::OnButtonRecommand() 
{
 	if(AfxMessageBox(hggettext("���� �Է� ������ �����Ͱ� ��� �⺻������ �ٲ�ϴ�. ��� �Ͻðڽ��ϱ�?  "), MB_YESNO | MB_ICONQUESTION)==IDNO)
 		return;	
 	
	CARcBridgeDataStd *pStd	= m_pStd->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_nBri);
	if(!pBri) return;

 	if(m_nTab == m_nTabOut)
 		SetDataDefaultOut();
 	else if(m_nTab == m_nTabNormal)
 		pBri->SetDataDefaultBM(1);
 	else if(m_nTab == m_nTabForm)
 		pBri->SetDataDefaultBM(0);
 
 	SetGridData();
 	SetDataInit();
}

void CDialogBMOption::OnButtonSame() 
{
	// TODO: Add your control notification handler code here
	// �Է»��� �߰��� �׸��� �߰����־����....
	CARcBridgeDataStd* pStd = m_pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_nBri);
	if(!pBri) return;

	CString strMsg = _T("");
	strMsg.Format("��� ������ ��ü �Է»����� %s��(��) �����ϰ� �����˴ϴ�.\n����Ͻðڽ��ϱ� ?",pBri->m_strBridgeName);
	if(AfxMessageBox(strMsg,MB_YESNO)==IDYES)
	{
		long nQtyBri = pStd->GetBridgeSize();

		long i=0; for(i=0; i<nQtyBri; i++)
		{
			if(i == m_nBri) continue;

			CRcBridgeRebar* pBri2 = pStd->GetBridgeRebar(i);

			MakeSameBM(pBri2,pBri);
		}
	}	
}

void CDialogBMOption::SetDataDefaultOut()
{
	CARcBridgeDataStd *pDataStd	= m_pStd->m_pARcBridgeDataStd;

	m_pStd->m_bSeparateExcludeRebarMonument = FALSE;
	m_pStd->m_bDrawElEarth = TRUE;
	m_pStd->m_bConvertToForm = TRUE;
	m_pStd->m_bRefCellBMSum = FALSE;
	pDataStd->m_bSepFootingForSiljuk = FALSE;
}

void CDialogBMOption::MakeSameBM(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2)
{
	if(pBri1==NULL || pBri2==NULL) return;

	// �������û���(�Ϲݼ���, ��Ǫ��)
	pBri1->m_nFormType_ConcNormal		= pBri2->m_nFormType_ConcNormal;
	pBri1->m_nFormType_Footing			= pBri2->m_nFormType_Footing;
	pBri1->m_nFormType_WallFront		= pBri2->m_nFormType_WallFront;
	pBri1->m_nFormType_WallBack			= pBri2->m_nFormType_WallBack;
	for(long stt = 0; stt < 2; stt++)
	{
		for(long left = 0; left < 2; left++)
		{
			pBri1->m_nFormType_WingFront[stt][left]	= pBri2->m_nFormType_WingFront[stt][left];
			pBri1->m_nFormType_WingBack[stt][left]	= pBri2->m_nFormType_WingBack[stt][left];
		}
	}
	pBri1->m_nFormType_Aps				= pBri2->m_nFormType_Aps;
	pBri1->m_nFormType_MassConc			= pBri2->m_nFormType_MassConc;
	pBri1->m_nFormType_LeanConc			= pBri2->m_nFormType_LeanConc;
	pBri1->m_bSelectBM_ConcCare_Main	= pBri2->m_bSelectBM_ConcCare_Main;
	pBri1->m_bSelectBM_ConcCare_Aps		= pBri2->m_bSelectBM_ConcCare_Aps;
	pBri1->m_nSelectBM_ConcCare_Cal		= pBri2->m_nSelectBM_ConcCare_Cal;
	pBri1->m_nTypeRcBridgeBlock			= pBri2->m_nTypeRcBridgeBlock;
	pBri1->m_dAngleRcBridgeBlock		= pBri2->m_dAngleRcBridgeBlock;
	pBri1->m_bSelectBM_ApsCnsInclude	= pBri2->m_bSelectBM_ApsCnsInclude;
	pBri1->m_dPavementThick				= pBri2->m_dPavementThick;
	pBri1->m_dNosangThick				= pBri2->m_dNosangThick;
	pBri1->m_nTypePost					= pBri2->m_nTypePost;
	pBri1->m_nTypeHeightWall			= pBri2->m_nTypeHeightWall;
	pBri1->m_nTypeSlabLength			= pBri2->m_nTypeSlabLength;
	pBri1->m_nTypeSurfaceProof			= pBri2->m_nTypeSurfaceProof;
	pBri1->m_nTypePostBaseLine			= pBri2->m_nTypePostBaseLine;
	pBri1->m_dThickPostUpper			= pBri2->m_dThickPostUpper;
	pBri1->m_dThickPostSide				= pBri2->m_dThickPostSide;
	pBri1->m_dThickScaffold				= pBri2->m_dThickScaffold;
	pBri1->m_nDeductNearFormBM			= pBri2->m_nDeductNearFormBM;
	pBri1->m_nDeductNearTupagiBM		= pBri2->m_nDeductNearTupagiBM;
	pBri1->m_dAggregate_Footing			= pBri2->m_dAggregate_Footing;
	pBri1->m_dAggregate_Main			= pBri2->m_dAggregate_Main;
	pBri1->m_dAggregate_Aps				= pBri2->m_dAggregate_Aps;
	pBri1->m_dAggregate_Lean			= pBri2->m_dAggregate_Lean;
	pBri1->m_dAggregate_Mass			= pBri2->m_dAggregate_Mass;
	pBri1->m_dAggregate_Hyunta			= pBri2->m_dAggregate_Hyunta;
	pBri1->m_dAggregate_Etc				= pBri2->m_dAggregate_Etc;
	pBri1->m_dSlump_Footing				= pBri2->m_dSlump_Footing;
	pBri1->m_dSlump_Main				= pBri2->m_dSlump_Main;
	pBri1->m_dSlump_Aps					= pBri2->m_dSlump_Aps;
	pBri1->m_dSlump_Lean				= pBri2->m_dSlump_Lean;
	pBri1->m_dSlump_Mass				= pBri2->m_dSlump_Mass;
	pBri1->m_dSlump_Hyunta				= pBri2->m_dSlump_Hyunta;
	pBri1->m_dSlump_Etc					= pBri2->m_dSlump_Etc;

	pBri1->m_bAggregate_SlumpDetail		= pBri2->m_bAggregate_SlumpDetail;
	for (HGINT32 ix = 0; ix < ePartCount; ++ix)
	{
		pBri1->m_dAggregate[ix]			= pBri2->m_dAggregate[ix];
		pBri1->m_dSlump[ix]				= pBri2->m_dSlump[ix];
	}
	
	pBri1->m_dAggregate_LeanLSD			= pBri2->m_dAggregate_LeanLSD;
	pBri1->m_dAggregate_MassLSD			= pBri2->m_dAggregate_MassLSD;
	pBri1->m_dAggregate_HyuntaLSD		= pBri2->m_dAggregate_HyuntaLSD;
	pBri1->m_dAggregate_EtcLSD			= pBri2->m_dAggregate_EtcLSD;
	pBri1->m_dSlump_LeanLSD				= pBri2->m_dSlump_LeanLSD;
	pBri1->m_dSlump_MassLSD				= pBri2->m_dSlump_MassLSD;
	pBri1->m_dSlump_HyuntaLSD			= pBri2->m_dSlump_HyuntaLSD;
	pBri1->m_dSlump_EtcLSD				= pBri2->m_dSlump_EtcLSD;
}

void CDialogBMOption::SyncAggregateAndSlump(CRcBridgeApp *pBri)
{
	if (pBri == NULL)
		return;

	for (HGINT32 ix = ePartUpperSlab + 1; ix < ePartCount; ++ix)
	{
		if (ix == ePartApsStt || ix == ePartApsEnd)
			continue;

		pBri->m_dAggregate[ix]	 = pBri->m_dAggregate[ePartUpperSlab];
		pBri->m_dSlump[ix]	 = pBri->m_dSlump[ePartUpperSlab];
	}

	pBri->m_dAggregate[ePartApsEnd] = pBri->m_dAggregate[ePartApsStt];
	pBri->m_dSlump[ePartApsEnd] = pBri->m_dSlump[ePartApsStt];
}

void CDialogBMOption::EnableControl()
{
	if (m_nTab == m_nTabOut)
	{
		m_comboBridge.EnableWindow(FALSE);
	}
	else
	{
		m_comboBridge.EnableWindow(TRUE);
	}
}
