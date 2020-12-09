// InputStmCondition.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "InputStmCondition.h"


// CInputStmCondition ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CInputStmCondition, CInputDialogBase)

CInputStmCondition::CInputStmCondition(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputStmCondition::IDD, pParent)
{

}

CInputStmCondition::~CInputStmCondition()
{
}

void CInputStmCondition::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_ctlTab);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_cbBridgeNo);
}


BEGIN_MESSAGE_MAP(CInputStmCondition, CInputDialogBase)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_BN_CLICKED(IDC_BUTTON_SAME_ALL, OnButtonSameAll)
END_MESSAGE_MAP()


// CInputStmCondition �޽��� ó�����Դϴ�.

void CInputStmCondition::OnPreInitDialog()
{
	GetDlgItem(IDC_BUTTON_RECOMMAND)->SetWindowText(hggettext("�⺻��(&D)"));
	GetDlgItem(IDC_BUTTON_NEXT)->SetWindowText(hggettext("����(&N)"));
	GetDlgItem(IDC_BUTTON_APPLY)->SetWindowText(hggettext("����(&A)"));
	GetDlgItem(IDC_BUTTON_PREV)->SetWindowText(hggettext("����(&P)"));

	SetResize(IDC_GRID,					SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB,					SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_SAME_ALL,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	InitTabCtrl();
	InitComboCtrl();
	SetGridData();
	SetDataInit();
}

void CInputStmCondition::OnSelchangeComboBridgeNo()
{
	SetDataSave();
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();
	SetGridData();
	SetDataInit();
	SetPaneTitle();
	m_cbBridgeNo.SetFocus();
}

void CInputStmCondition::InitTabCtrl()
{
	m_ctlTab.DeleteAllItems();

	// Clip children of the tab control from paint routines to reduce flicker.
	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	m_ctlTab.DeleteAllItems();

	m_nTab = 0;
	m_nTabCondition = -1;
	m_nTabStmCheck = -1;

	HGINT32 nIdx = 0;

	m_ctlTab.InsertItem(nIdx, _T("��������"));	m_nTabCondition = nIdx++;
	m_ctlTab.InsertItem(nIdx, _T("STM����"));	m_nTabStmCheck = nIdx++;
}

void CInputStmCondition::SetDataDefault()
{
	if(IsUsedFlag() && m_bClickDefaultButton==FALSE) return;
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	
	HGBOOL bFirst = !IsUsedFlag();
	if(bFirst)
	{
		for (HGINT32 ix = 0; ix < pStd->GetBridgeSize(); ++ix)
		{
			CRcBridgeApp *pBridge = pStd->GetBridge(ix);
			if(!pBridge) return;

			SetDataDefaultCondition(ix);
			SetDataDefaultStmCheck(ix);
		}
	}
	else
	{
		if(m_nTab == m_nTabCondition)
		{
			SetDataDefaultCondition(m_pIn->m_nCurrentBridge);
		}
		else if(m_nTab == m_nTabStmCheck)
		{
			SetDataDefaultStmCheck(m_pIn->m_nCurrentBridge);
		}
	}

	SetGridData();
	SetDataInit();
}

void CInputStmCondition::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
	UpdateData(TRUE);

	CInputDialogBase::SetDataSave();
}

void CInputStmCondition::SetDataInit()
{
	UpdateData(FALSE);
	m_Grid.UpdateData(FALSE);

	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols);
	if(m_nTab == m_nTabCondition)
	{
		m_Grid.MergeGridEx(0, 0, 0, nCols-1);

//		long nColInput = 3;
		m_Grid.MergeGridEx(0, nRows - 1, 0, 1);
		if( m_bDesignLsd == TRUE )
			m_Grid.MergeGrid(25, 25, 1, 2);
	}
	if(m_nTab == m_nTabStmCheck)
	{
		m_Grid.MergeGridEx(0, 0, 0, nCols-1);
		m_Grid.MergeGridEx(0, nRows - 1, 0, m_bDesignLsd ? 2 : 1);

		if( m_bDesignLsd )
		{
			for (HGINT32 ix=11; ix < nRows; ++ix)
			{
				m_Grid.MergeGrid(ix, ix, 0, 1);
			}
		}
	}
}

void CInputStmCondition::SetGridData()
{
	if(m_nTab == m_nTabCondition)
	{
		SynMaterialUsd(FALSE);
		SetGridDataCondition();
	}
	else if(m_nTab == m_nTabStmCheck)
	{
		if( m_bDesignLsd == TRUE )
			SetGridDataLsdStmCheck();
		else
			SetGridDataStmCheck();
	}
}

void CInputStmCondition::SetGridDataCondition()
{
// 	if( m_bDesignLsd == TRUE )
// 	{
// 		SetGridDataConditionLsd();
// 		return;
// 	}

	m_Grid.SetGridDefault(3, 3, 1, m_bDesignLsd==TRUE ? 3 : 2, TRUE, FALSE);
	
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	CRcBridgeApp *pBridge = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBridge) return;

	CStmDataConfig *pStmData = pBridge->m_pStmSolverInput->GetStmDataConfig();
	if(!pStmData) return;

	long row		= 0;
	long col		= 0;
	UINT nFormatRight	= DT_RIGHT | DT_VCENTER;
	UINT nFormatLeft = DT_LEFT | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
//	UINT nLineRefLine	= CELL_LINE_ON;

	CString sHelp	= _T("");
	CStringArray sArr;
	CString szUnit_CGS   = UNIT_CGS_KGF_CM2;
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

	m_Grid.AddCell(row, col++, hggettext("��        ��"));
	m_Grid.AddCell(row, col++, hggettext("��        ��"));
	if( m_bDesignLsd )
		m_Grid.AddCell(row, col++, hggettext("��        ��"));
	m_Grid.AddCell(row, col++, hggettext("�� �� ��"));
	m_Grid.AddCell(row, col, hggettext("����/���"));
	row++;
	col	= 0;

	if( m_bDesignLsd == TRUE )
	{
		AddCellTension(eLsdStmCornerUpper, row, col);			// �찢��
		if( pBridge->IsBoxType() )
			AddCellTension(eLsdStmCornerLower, row, col);		
		AddCellTension(eLsdStmBracketStt, row, col);			// ����� - ����
		AddCellTension(eLsdStmBracketEnd, row, col);			// ����� - ����
		AddCellTension(eLsdStmFootStt, row, col);			// ���� - ����
		AddCellTension(eLsdStmFootEnd, row, col);			// ���� - ����
		AddCellTension(eLsdStmFootMid, row, col);			// ���� - �߰�����
	}
	else
		AddCellTension(-1, row, col);

// 	m_Grid.AddCell(row, col++, hggettext("������ ����"));
// 	m_Grid.AddCell(row, col++, hggettext(" Fck : ��ũ��Ʈ ����"), nFormatLeft);
// 	m_Grid.AddCell(row, col++, &pStd->m_Fck, nFormatRight, CELL_READONLY);	m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
// 	m_Grid.AddCell(row, col, szUnit_Apply, nFormatLeft, CELL_READONLY);
// 	row++;
// 	col	= 0;
// 
// 	m_Grid.AddCell(row, col++, hggettext("������ ����"));
// 	m_Grid.AddCell(row, col++, hggettext(" ft : ��Ʈ��Ʈ ���� ����"), nFormatLeft);
// 	m_Grid.AddCell(row, col++, &pStmData->m_dFt, nFormatRight);	m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
// 	m_Grid.AddCell(row, col, szUnit_Apply, nFormatLeft, CELL_READONLY);
// 	row++;
// 	col	= 0;
// 
// 	m_Grid.AddCell(row, col++, hggettext("������ ����"));
// 	m_Grid.AddCell(row, col++, hggettext(" Ec : ��ũ��Ʈ ź�����"), nFormatLeft);
// 	m_Grid.AddCell(row, col++, &pStd->m_Ec, nFormatRight, CELL_READONLY);	m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
// 	m_Grid.AddCell(row, col, szUnit_Apply, nFormatLeft, CELL_READONLY);
// 	row++;
// 	col	= 0;
// 
// 	m_Grid.AddCell(row, col++, hggettext("������ ����"));
// 	m_Grid.AddCell(row, col++, hggettext(" Fy : ö�� �׺�����"), nFormatLeft);
// 	m_Grid.AddCell(row, col++, &pStd->m_Fy, nFormatRight, CELL_READONLY);	m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
// 	m_Grid.AddCell(row, col, szUnit_Apply, nFormatLeft, CELL_READONLY);
// 	row++;
// 	col	= 0;
 
//  	////���� ö�� �׺� ���� ã��
//  	m_Grid.AddCell(row, col++, hggettext("������ ����"));
//  	m_Grid.AddCell(row, col++, hggettext(" Fy : ���� ö�� �׺�����"), nFormatLeft);
//  	m_Grid.AddCell(row, col++, &pStd->m_Fy, nFormatRight, CELL_READONLY);	m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
//  	m_Grid.AddCell(row, col++, szUnit_Apply, nFormatLeft, CELL_READONLY);
//  	row++;
//  	col	= 0;
 
	m_Grid.AddCell(row, col++, hggettext("������ ����"));
	if( m_bDesignLsd == TRUE )
		m_Grid.AddCell(row, col++, hggettext(" Es : ö�� ź�����"), nFormatLeft);
	m_Grid.AddCell(row, col++, hggettext(" Es : ö�� ź�����"), nFormatLeft);
	m_Grid.AddCell(row, col++, &pStd->m_Es, nFormatRight, CELL_READONLY);	m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); m_Grid.SetCellDec(CELL_DEC_2);
	m_Grid.AddCell(row, col, szUnit_Apply, nFormatLeft, CELL_READONLY);
	row++;
	col	= 0;
 
	if( m_bDesignLsd == FALSE )
	{
		m_Grid.AddCell(row, col++, hggettext("���� ���� ���(��)"));
		m_Grid.AddCell(row, col++, hggettext(" Strut(��s)"), nFormatLeft);
		m_Grid.AddCell(row, col++, &pStmData->m_dPiStrut, nFormatRight);	m_Grid.SetCellDec(CELL_DEC_2);
		m_Grid.AddCell(row, col, _T(""), nFormatLeft, CELL_READONLY);
		row++;
		col	= 0;

		m_Grid.AddCell(row, col++, hggettext("���� ���� ���(��)"));
		m_Grid.AddCell(row, col++, hggettext(" Tie(��t)"), nFormatLeft);
		m_Grid.AddCell(row, col++, &pStmData->m_dPiTie, nFormatRight);	m_Grid.SetCellDec(CELL_DEC_2);
		m_Grid.AddCell(row, col, _T(""), nFormatLeft, CELL_READONLY);
		row++;
		col	= 0;

		m_Grid.AddCell(row, col++, hggettext("���� ���� ���(��)"));
		m_Grid.AddCell(row, col++, hggettext(" Node(��n)"), nFormatLeft);
		m_Grid.AddCell(row, col++, &pStmData->m_dPiNode, nFormatRight);	m_Grid.SetCellDec(CELL_DEC_2);
		m_Grid.AddCell(row, col, _T(""), nFormatLeft, CELL_READONLY);
		row++;
		col	= 0;
	}
 
	CString strRatio = m_bDesignLsd ? hggettext("������װ����") : hggettext("�������Ұ����");

	sArr.RemoveAll();
	sArr.Add(hggettext(m_bDesignLsd ? hggettext("��s/ ��c") : hggettext("��t/ ��s")));
 	sArr.Add(hggettext("����� �Է�"));
 	m_Grid.AddCell(row, col++, strRatio);
	if( m_bDesignLsd == TRUE )
		m_Grid.AddCell(row, col++, strRatio, nFormatLeft);
 	m_Grid.AddCell(row, col++, hggettext(" ������� "), nFormatLeft);
 	m_Grid.AddCell(row, col++, (long*)&pStmData->m_ixRatioPiT_PiS, nFormatRight, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
 	m_Grid.AddCell(row, col, _T(""), nFormatLeft, CELL_READONLY);
 	row++;
 	col	= 0;
 
	m_Grid.AddCell(row, col++, m_bDesignLsd ? hggettext("������װ����") : hggettext("�������Ұ����"));
	if(m_bDesignLsd)
		m_Grid.AddCell(row, col++, hggettext("������װ����"));
	m_Grid.AddCell(row, col++, hggettext(" ������Է�"), nFormatLeft);
	m_Grid.AddCell(row, col++, &pStmData->m_dUserRatioPiT_PiS, nFormatRight, pStmData->m_ixRatioPiT_PiS == 1 ? nLine : CELL_READONLY);
	m_Grid.AddCell(row, col, _T(""), nFormatLeft, CELL_READONLY);
	row++;
 	col	= 0;
 
	if(m_bDesignLsd == FALSE)
	{
		sArr.RemoveAll();
		sArr.Add(hggettext("��Ʈ�� ��ȿ����"));
		sArr.Add(hggettext("Min(��Ʈ����ȿ����, ������ȿ����)"));
		m_Grid.AddCell(row, col++, hggettext("��Ʈ���� ��ȿ���భ��"));
		m_Grid.AddCell(row, col++, hggettext("��Ʈ���� ��ȿ���భ��"));
		m_Grid.AddCell(row, col++, (long*)&pStmData->m_ixEFactorStrut, nFormatRight, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col, _T(""), nFormatLeft, CELL_READONLY);
		row++;
		col	= 0;
	}

	m_Grid.AddCell(row, col++, hggettext("Mesh Size")); 
	if( m_bDesignLsd == TRUE )
		m_Grid.AddCell(row, col++, hggettext("Mesh Size"));
	m_Grid.AddCell(row, col++, hggettext("�찢��"), nFormatLeft); 
	m_Grid.AddCell(row, col++, &pBridge->m_dMeshSizeArcCorner, nFormatRight, nLine);
	m_Grid.SetCellUnit(UNIT_CGS_M, pStd->m_szM); m_Grid.SetCellDec(m_bDesignLsd ? CELL_DEC_0 : CELL_DEC_3);
	m_Grid.AddCell(row, col, _T(""), nFormatLeft, CELL_READONLY);
	row++;
	col	= 0;

	m_Grid.AddCell(row, col++, hggettext("Mesh Size")); 
	if( m_bDesignLsd == TRUE )
		m_Grid.AddCell(row, col++, hggettext("Mesh Size"));
	m_Grid.AddCell(row, col++, hggettext("�����"), nFormatLeft); 
	m_Grid.AddCell(row, col++, &pBridge->m_dMeshSizeBracket, nFormatRight, nLine);
	m_Grid.SetCellUnit(UNIT_CGS_M, pStd->m_szM); m_Grid.SetCellDec(m_bDesignLsd ? CELL_DEC_0 : CELL_DEC_3);
	m_Grid.AddCell(row, col, _T(""), nFormatLeft, CELL_READONLY);
	row++;
	col	= 0;

	m_Grid.AddCell(row, col++, hggettext("Mesh Size")); 
	if( m_bDesignLsd == TRUE )
		m_Grid.AddCell(row, col++, hggettext("Mesh Size"));
	m_Grid.AddCell(row, col++, hggettext("����"), nFormatLeft); 
	m_Grid.AddCell(row, col++, &pBridge->m_dMeshSizeBase, nFormatRight, nLine);
	m_Grid.SetCellUnit(UNIT_CGS_M, pStd->m_szM); m_Grid.SetCellDec(m_bDesignLsd ? CELL_DEC_0 : CELL_DEC_3);
	m_Grid.AddCell(row, col, _T(""), nFormatLeft, CELL_READONLY);
	row++;
	col	= 0;

 	sArr.RemoveAll();
 	sArr.Add("Terzaghi ������");
 	sArr.Add("Terzaghi ������ ����� �Է�");
 	sArr.Add("�����������");
 	m_Grid.AddCell(row, col++, "���ݹݷ°�� ����", GVHT_LEFT); 
	if( m_bDesignLsd == TRUE )
		m_Grid.AddCell(row, col++, hggettext("���ݹݷ°�� ����"), GVHT_LEFT);
 	m_Grid.AddCell(row, col++, "���ݹݷ°��(Kv) ������ Terzaghi ����", GVHT_LEFT); 
 	m_Grid.AddCell(row, col++, &pBridge->m_nGroundFactorKV, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
 	m_Grid.AddCell(row, col, _T(""), nFormatLeft, CELL_READONLY);
 	row++;
 	col	= 0;
 	//
 	m_Grid.AddCell(row, col++, "���ݹݷ°�� ����", GVHT_LEFT); 
	if( m_bDesignLsd == TRUE )
		m_Grid.AddCell(row, col++, hggettext("���ݹݷ°�� ����"), GVHT_LEFT);
 	m_Grid.AddCell(row, col++, "���ݹݷ°��(Kv) ������ Terzaghi ����", GVHT_LEFT); 
 	m_Grid.AddCell(row, col++, &pBridge->m_dGroundFactorKV, nFormatRight, pBridge->m_nGroundFactorKV == 1 ? CELL_LINE_NO : CELL_READONLY);
 	m_Grid.AddCell(row, col, _T(""), nFormatLeft, CELL_READONLY);
 	row++;
 	col	= 0;
 	//
 	sArr.RemoveAll();
	sArr.Add("�������");
	sArr.Add("�ܸ��������");
 	sArr.Add("Ⱦ����(B:��������)");
 	sArr.Add("������(B:������)");
 	m_Grid.AddCell(row, col++, "���ݹݷ°�� ����", GVHT_LEFT); 
	if( m_bDesignLsd == TRUE )
		m_Grid.AddCell(row, col++, hggettext("���ݹݷ°�� ����"), GVHT_LEFT);
 	m_Grid.AddCell(row, col++, "����ȯ����(Bv)���� ��(B) ������", GVHT_LEFT); 
 	m_Grid.AddCell(row, col++, &pBridge->m_nGroundFactorBv, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
 	m_Grid.AddCell(row, col, _T(""), nFormatLeft, CELL_READONLY);
 	row++;
 	col	= 0;
 	//
 	sArr.RemoveAll();
 	sArr.Add("���������� �켼�� ���");
 	sArr.Add("���������� �켼�� ���");
 	sArr.Add("���������� �������� ȥ������ ���");
 	m_Grid.AddCell(row, col++, "���ݹݷ°�� ����", GVHT_LEFT); 
	if( m_bDesignLsd == TRUE )
		m_Grid.AddCell(row, col++, hggettext("���ݹݷ°�� ����"), GVHT_LEFT);
 	m_Grid.AddCell(row, col++, "�������� ���� �ݷ°��", GVHT_LEFT); 
 	m_Grid.AddCell(row, col++, &pBridge->m_nVerticalFactor, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
 	m_Grid.AddCell(row, col, _T(""), nFormatLeft, CELL_READONLY);
 	row++;

	if( m_bDesignLsd == TRUE )
	{
		// (ARCBRIDGE-2282) �ֹ� ������Ʈ �ɶ����� ��Ȱ�� ��û
		col = 0;
		m_Grid.AddCell(row, col++, "�찢�� ����", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, "�찢�� ����", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, "�ִ���", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pBridge->m_bCheckCorner, nFormatRight,  CELL_READONLY, pBridge->m_bCheckCorner ? "����" : "�������", CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col, _T(""), nFormatLeft, CELL_READONLY);
	}

 	m_Grid.GetCellCount(row, col);
 	m_Grid.SetRows(row);
 	m_Grid.SetCols(col);
 	m_Grid.SetRowHeightAll(20);

	if( m_bDesignLsd == TRUE )
	{
		m_Grid.SetColumnWidth(0, 150);
		m_Grid.SetColumnWidth(1, 150);
		m_Grid.SetColumnWidth(2, 230);
		m_Grid.SetColumnWidth(3, 240);
		m_Grid.SetColumnWidth(4, 70);
	}
	else
	{
		m_Grid.SetColumnWidth(0, 150);
		m_Grid.SetColumnWidth(1, 230);
		m_Grid.SetColumnWidth(2, 240);
		m_Grid.SetColumnWidth(3, 70);
	}
}

void CInputStmCondition::SetGridDataStmCheck()
{
	CRcBridgeRebar *pBridge = static_cast<CRcBridgeRebar*>(m_pIn->GetRcBridge());
	if (!pBridge) return;

	CARcBridgeCalcStd CalcStd(pBridge);

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(4, 2, 1, 2); //

	long row		= 0;
	long col		= 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
//	UINT nFormatRight	= DT_RIGHT | DT_VCENTER;
//	UINT nFormatLeft = DT_LEFT | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
// 	UINT nLineRefLine	= CELL_LINE_ON;

	m_Grid.AddCell(row, col++, hggettext("����"));
	m_Grid.AddCell(row, col++, hggettext("����"));
	
	m_Grid.AddCell(row, col++, hggettext("���� �����"));
	m_Grid.AddCell(row, col++, hggettext("���� �����"));
	m_Grid.AddCell(row, col++, hggettext("���� ��ü"));
	m_Grid.AddCell(row, col++, hggettext("���� ��ü"));
	
	HGINT32 nCountInwall = pBridge->GetCountInWall();
	CString str = hggettext("");
	for (HGINT32 ix = 0; ix < nCountInwall; ++ix)
	{
		str.Format("�߰� ��ü%d", ix + 1);
		m_Grid.AddCell(row, col++, str);
		m_Grid.AddCell(row, col++, str);
	}
	m_Grid.AddCell(row, col++, hggettext("���� ��ü"));
	m_Grid.AddCell(row, col++, hggettext("���� ��ü"));
	m_Grid.AddCell(row, col++, hggettext("���� �����"));
	m_Grid.AddCell(row, col, hggettext("���� �����"));
	row++;
	col	= 0;

	CString szValue(_T(""));
	szValue = GetStringValueBracket(&CalcStd, TRUE);
	//��� close
 	m_Grid.AddCell(row, col++, hggettext("���"));
 	m_Grid.AddCell(row, col++, hggettext("Close"));
 	m_Grid.AddCell(row, col++, &pBridge->m_bApplyStmModelingBracket[0], nFormat, nLine, "", CELL_TYPE_CHECK);
 	m_Grid.AddCell(row, col++, szValue, nFormat, CELL_READONLY);

	szValue = GetStringValueCorner(TRUE, TRUE, TRUE);
	m_Grid.AddCell(row, col++, &pBridge->m_bApplyStmModelingCorner[0][0][0][0], nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, szValue, nFormat, CELL_READONLY);

	for (HGINT32 ix = 0; ix < nCountInwall; ++ix)
	{
		m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);
	}

	szValue = GetStringValueCorner(FALSE, TRUE, TRUE);
	m_Grid.AddCell(row, col++, &pBridge->m_bApplyStmModelingCorner[1][0][0][0], nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, szValue, nFormat, CELL_READONLY);

	szValue = GetStringValueBracket(&CalcStd, FALSE);
	m_Grid.AddCell(row, col++, &pBridge->m_bApplyStmModelingBracket[1], nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col, szValue, nFormat, CELL_READONLY);
	row++;
	col = 0;

	//��� open
	m_Grid.AddCell(row, col++, hggettext("���"));
	m_Grid.AddCell(row, col++, hggettext("Open"));
	m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);

	szValue = GetStringValueCorner(TRUE, FALSE, TRUE);
	m_Grid.AddCell(row, col++, &pBridge->m_bApplyStmModelingCorner[0][0][0][1], nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++,szValue, nFormat, CELL_READONLY);

	for (HGINT32 ix = 0; ix < nCountInwall; ++ix)
	{
		m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);
	}

	szValue = GetStringValueCorner(FALSE, FALSE, TRUE);
	m_Grid.AddCell(row, col++, &pBridge->m_bApplyStmModelingCorner[1][0][0][1], nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, szValue, nFormat, CELL_READONLY);

	m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col, hggettext(""), nFormat, CELL_READONLY);
	row++;
	col = 0;

	//�Ϻ� close
	m_Grid.AddCell(row, col++, hggettext("�Ϻ�"));
	m_Grid.AddCell(row, col++, hggettext("Close"));
	m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);

	szValue = GetStringValueCorner(TRUE, TRUE, FALSE);
	m_Grid.AddCell(row, col++, &pBridge->m_bApplyStmModelingCorner[0][0][1][0], nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, szValue, nFormat, CELL_READONLY);

	for (HGINT32 ix = 0; ix < nCountInwall; ++ix)
	{
		m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);
	}

	szValue = GetStringValueCorner(FALSE, TRUE, FALSE);
	m_Grid.AddCell(row, col++, &pBridge->m_bApplyStmModelingCorner[1][0][1][0], nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, szValue, nFormat, CELL_READONLY);

	m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col, hggettext(""), nFormat, CELL_READONLY);
	row++;
	col = 0;

	//�Ϻ� open
	m_Grid.AddCell(row, col++, hggettext("�Ϻ�"));
	m_Grid.AddCell(row, col++, hggettext("Open"));
	m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);

	szValue = GetStringValueCorner(TRUE, FALSE, FALSE);
	m_Grid.AddCell(row, col++, &pBridge->m_bApplyStmModelingCorner[0][0][1][1], nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, szValue, nFormat, CELL_READONLY);

	for (HGINT32 ix = 0; ix < nCountInwall; ++ix)
	{
		m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);
	}

	szValue = GetStringValueCorner(FALSE, FALSE, FALSE);
	m_Grid.AddCell(row, col++, &pBridge->m_bApplyStmModelingCorner[1][0][1][1], nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, szValue, nFormat, CELL_READONLY);

	m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col, hggettext(""), nFormat, CELL_READONLY);
	row++;
	col = 0;

	for(HGINT32 iCase=0; iCase<4; ++iCase)
	{
		CString szCase(_T(""));
		switch (iCase)
		{
		case 0:
			szCase = hggettext("P-max"); break;
		case 1:
			szCase = hggettext("M-max"); break;
		case 2:
			szCase = hggettext("M-min"); break;
		case 3:
			szCase = hggettext("EQ"); break;
		default:
			break;
		}

		m_Grid.AddCell(row, col++, hggettext("����"));
		m_Grid.AddCell(row, col++, szCase);
		m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);

		HGINT32 ixCombo(0);
		szValue = GetStringValueFoont(&CalcStd, 0, iCase, ixCombo);
		m_Grid.AddCell(row, col++, &pBridge->m_bApplyStmModelingFooting[0][0][iCase], nFormat, nLine, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col++, szValue, nFormat, CELL_READONLY);

		for (HGINT32 ix = 0; ix < nCountInwall; ++ix)
		{
			szValue = GetStringValueFoont(&CalcStd, ix+1, iCase, ixCombo);
			m_Grid.AddCell(row, col++, &pBridge->m_bApplyStmModelingMidFooting[ix][0][iCase], nFormat, nLine, "", CELL_TYPE_CHECK);
			m_Grid.AddCell(row, col++, szValue, nFormat, CELL_READONLY);
		}

		szValue = GetStringValueFoont(&CalcStd, pBridge->m_nQtyJigan, iCase, ixCombo);
		m_Grid.AddCell(row, col++, &pBridge->m_bApplyStmModelingFooting[1][0][iCase], nFormat, nLine, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col++, szValue, nFormat, CELL_READONLY);

		m_Grid.AddCell(row, col++, hggettext(""), nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col, hggettext(""), nFormat, CELL_READONLY);
		row++;
		col = 0;
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(80);

	m_Grid.SetColumnWidth(2, 30);
	m_Grid.SetColumnWidth(4, 30);
	m_Grid.SetColumnWidth(6, 30);
	m_Grid.SetColumnWidth(8, 30);
}

void CInputStmCondition::SetGridDataLsdStmCheck()
{
	CRcBridgeRebar *pBridge = static_cast<CRcBridgeRebar*>(m_pIn->GetRcBridge());
	if (!pBridge) return;

	CARcBridgeCalcStd CalcStd(pBridge);

	// ��� ���� ��౳
	if( pBridge->m_nTypeBridge == BRIDGE_TYPE_SLAB )
	{
		m_pIn->GridMessage(&m_Grid, hggettext("�ش� ������ STM ���並 �������� �ʽ��ϴ�."));
		return;
	}

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(4, 2, 1, 3); //

	long row		= 0;
	long col		= 0;

	m_Grid.AddCell(row, col++, hggettext("����"));
	m_Grid.AddCell(row, col++, hggettext("����"));
	m_Grid.AddCell(row, col++, hggettext("����"));
	m_Grid.AddCell(row, col++, hggettext("��������"));
	m_Grid.AddCell(row, col++, hggettext("��i"));
	m_Grid.AddCell(row, col++, hggettext("Value"));
	m_Grid.AddCell(row, col, hggettext("����"));
	row++;
	col = 0;

	CString szValue(_T(""));
	if( pBridge->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || pBridge->IsBoxType() || pBridge->m_nTypeBridge == BRIDGE_TYPE_PORTAL )
	{
		for (long ixCase=0; ixCase<3; ++ixCase)
		{
			CString str[3] = { _T("ST"), _T("EX1"), _T("EX2") };

			ETypeEnvComboLsd eType = eEnvLimit; 
			if( 0 < ixCase )
			{
				eType = ixCase==1 ? eEnvExtremeI : eEnvExtremeII;
			}
			// �찢�� - ���
			szValue = GetStringValueCorner(TRUE, TRUE, TRUE, eType);
			AddCellStmCheckCorner(row, col, ixCase, hggettext("����-Close"), str[ixCase], szValue, pBridge->m_bApplyStmModelingCorner[0][ixCase][0][0], TRUE);

			szValue = GetStringValueCorner(TRUE, FALSE, TRUE, eType);
			AddCellStmCheckCorner(row, col, ixCase, hggettext("����-Open"), str[ixCase], szValue, pBridge->m_bApplyStmModelingCorner[0][ixCase][0][1], TRUE);

			szValue = GetStringValueCorner(FALSE, TRUE, TRUE, eType);
			AddCellStmCheckCorner(row, col, ixCase, hggettext("����-Close"), str[ixCase], szValue , pBridge->m_bApplyStmModelingCorner[1][ixCase][0][0], TRUE);

			szValue = GetStringValueCorner(FALSE, FALSE, TRUE, eType);
			AddCellStmCheckCorner(row, col, ixCase, hggettext("����-Open"), str[ixCase], szValue , pBridge->m_bApplyStmModelingCorner[1][ixCase][0][1], TRUE);

			// �찢�� - �ϴ�
			if( pBridge->IsBoxType()==TRUE )
			{
				szValue = GetStringValueCorner(TRUE, TRUE, FALSE, eType);
				AddCellStmCheckCorner(row, col, ixCase, hggettext("����-Close"), str[ixCase], szValue , pBridge->m_bApplyStmModelingCorner[0][ixCase][1][0], FALSE);

				szValue = GetStringValueCorner(TRUE, FALSE, FALSE, eType);
				AddCellStmCheckCorner(row, col, ixCase, hggettext("����-Open"), str[ixCase], szValue , pBridge->m_bApplyStmModelingCorner[0][ixCase][1][1], FALSE);

				szValue = GetStringValueCorner(FALSE, TRUE, FALSE, eType);
				AddCellStmCheckCorner(row, col, ixCase, hggettext("����-Close"), str[ixCase], szValue , pBridge->m_bApplyStmModelingCorner[1][ixCase][1][0], FALSE);

				szValue = GetStringValueCorner(FALSE, FALSE, FALSE, eType);
				AddCellStmCheckCorner(row, col, ixCase, hggettext("����-Open"), str[ixCase], szValue , pBridge->m_bApplyStmModelingCorner[1][ixCase][1][1], FALSE);
			}
		}

		// �����
		szValue = GetStringValueBracket(&CalcStd, TRUE);
		AddCellStmCheckBracket(row, col, pBridge->m_strStmModelingBracket[0], szValue, pBridge->m_bApplyStmModelingBracket[0], TRUE);

		szValue = GetStringValueBracket(&CalcStd, FALSE);
		AddCellStmCheckBracket(row, col, pBridge->m_strStmModelingBracket[1], szValue, pBridge->m_bApplyStmModelingBracket[1], FALSE);
	}

	// ������ ����
	if( pBridge->m_nTypeBridge == BRIDGE_TYPE_PORTAL || pBridge->m_nTypeBridge == BRIDGE_TYPE_TRAP2 )
	{
		AddCellStmCheckFooting(row, col, hggettext("����-����"), TRUE);
		AddCellStmCheckFooting(row, col, hggettext("����-����"), FALSE);
	}

	// �߰�����
	if( pBridge->m_nTypeBridge == BRIDGE_TYPE_PORTAL || pBridge->m_nTypeBridge == BRIDGE_TYPE_TRAP || pBridge->m_nTypeBridge == BRIDGE_TYPE_PI )
		AddCellStmCheckFootingMid(row, col);

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(120);

	m_Grid.SetColumnWidth(4, 80);
}

void CInputStmCondition::AddCellStmCheckCorner(long &row, long &col, long nCase, CString strTitle, CString strLoad, CString szValue, HGBOOL &bApply, HGBOOL bUpper)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	CRcBridgeApp *pBridge = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBridge) return;

	CRcBridgeData *pBri	= m_pIn->m_pARcBridgeDataStd->GetBridge(m_pIn->m_nCurrentBridge);
	if( NULL == pBri ) return;

	if( pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(nCase)) == FALSE ) return;

	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	CString strCase[3] = {_T("�����Ѱ�"), _T("�شܻ�Ȳ I"), _T("�شܻ�Ȳ II")};
	CString str = pBridge->IsBoxType()==TRUE ? (bUpper==TRUE ? hggettext("�찢�� - ���") : hggettext("�찢�� - �ϴ�")) : hggettext("�찢��");

	m_Grid.AddCell(row, col++, str);
	m_Grid.AddCell(row, col++, strCase[nCase]);
	m_Grid.AddCell(row, col++, strTitle);
	m_Grid.AddCell(row, col++, strLoad, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, hggettext("-"), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, szValue, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, &bApply, nFormat, nLine, bApply==TRUE ? "����" : "�������", CELL_TYPE_CHECK);
	row++;
	col = 0;
}

void CInputStmCondition::AddCellStmCheckBracket(long &row, long &col, CString strLoad, CString szValue, HGBOOL &bApply, HGBOOL bStt)
{
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
	
	CString szRet(_T("-")), sEta("");
	sEta.Format(_T("%.3f"), m_dEtaBracket);

	m_Grid.AddCell(row, col++, hggettext("�����"));
	m_Grid.AddCell(row, col++, hggettext("�����"));
	m_Grid.AddCell(row, col++, bStt==TRUE ? hggettext("����") : hggettext("����"));
	m_Grid.AddCell(row, col++, strLoad, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, sEta, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, szValue, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, &bApply, nFormat, nLine, bApply==TRUE ? "����" : "�������", CELL_TYPE_CHECK);
	row++;
	col = 0;
}

void CInputStmCondition::AddCellStmCheckFooting(long &row, long &col, CString strTitle, HGBOOL bStt)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	CRcBridgeApp *pBridgeApp = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBridgeApp) return;

	CRcBridgeRebar *pBridge = static_cast<CRcBridgeRebar*>(m_pIn->GetRcBridge());
	if (!pBridge) return;

	CRcBridgeData *pBri	= m_pIn->m_pARcBridgeDataStd->GetBridge(m_pIn->m_nCurrentBridge);
	if( NULL == pBri ) return;

	CARcBridgeCalcStd CalcStd(pBridge);

	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	CString sStatus[3] = {NOTRANS("�����Ѱ�"), NOTRANS("�شܻ�Ȳ I"), NOTRANS("�شܻ�Ȳ ��")};

	for(HGINT32 iStatus = LSD_STM_FOOTING_ULT; iStatus < LSD_STM_FOOTING_MAX; iStatus++)
	{
		if( iStatus > 0 )
		{
//			eEnvExtremeI, eEnvExtremeII
			if( pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(iStatus)) == FALSE ) continue;
		}

		long nSize = 5;

		for(HGINT32 ix = 0; ix < nSize; ix++)
		{
			HGINT32 ixCombo(-1);
			CString szValue = GetStringValueFoont(&CalcStd, bStt==TRUE ? 0 : pBridge->m_nQtyJigan, ix, ixCombo, (ETypeEnvComboLsd)iStatus);
			CString szLoad = pBridgeApp->m_strStmModelingFooting[bStt==TRUE ? 0 : 1][iStatus][ix];
			CString szEta("");

			double dEta = pBri->m_plsdManager->GetEtaValue(bStt ? ePartFootStt : ePartFootEnd, szLoad );
			szEta.Format(_T("%.3f"), dEta);

			m_Grid.AddCell(row, col++, strTitle);
			m_Grid.AddCell(row, col++, strTitle);
			m_Grid.AddCell(row, col++, sStatus[iStatus]);
			m_Grid.AddCell(row, col++, szLoad, nFormat, CELL_READONLY);
			m_Grid.AddCell(row, col++, szEta, nFormat, CELL_READONLY);
			m_Grid.AddCell(row, col++, szValue, nFormat, CELL_READONLY);
			m_Grid.AddCell(row, col++, &pBridgeApp->m_bApplyStmModelingFooting[bStt==TRUE ? 0 : 1][iStatus][ix], nFormat, nLine, 
				pBridgeApp->m_bApplyStmModelingFooting[bStt==TRUE ? 0 : 1][iStatus][ix]==TRUE ? "����" : "�������", CELL_TYPE_CHECK);
			row++;
			col	= 0;
		}
	}
}

void CInputStmCondition::AddCellStmCheckFootingMid(long &row, long &col)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	CRcBridgeApp *pBridgeApp = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBridgeApp) return;

	CRcBridgeData *pBri	= m_pIn->m_pARcBridgeDataStd->GetBridge(m_pIn->m_nCurrentBridge);
	if( NULL == pBri ) return;

	CRcBridgeRebar *pBridge = static_cast<CRcBridgeRebar*>(m_pIn->GetRcBridge());
	if (!pBridge) return;

	HGINT32 nCountInwall = pBridgeApp->GetCountInWall();

	CARcBridgeCalcStd CalcStd(pBridge);

	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	CString sStatus[3] = {NOTRANS("�����Ѱ�"), NOTRANS("�شܻ�Ȳ I"), NOTRANS("�شܻ�Ȳ ��")};

	for (HGINT32 ixMid = 0; ixMid < nCountInwall; ++ixMid)
	{
		for(HGINT32 iStatus = LSD_STM_FOOTING_ULT; iStatus < LSD_STM_FOOTING_MAX; iStatus++)
		{
			long nSize = 5;

			if( iStatus > 0 )
			{
				//			eEnvExtremeI, eEnvExtremeII
				if( pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(iStatus)) == FALSE ) continue;
			}

			for(HGINT32 ix = 0; ix < nSize; ix++)
			{
				HGINT32 ixCombo(-1);
				CString szValue = GetStringValueFoont(&CalcStd, ixMid+1, ix, ixCombo, (ETypeEnvComboLsd)iStatus);
				CString szLoad = pBridgeApp->m_strStmModelingMidFooting[ixMid][iStatus][ix];
				CString szEta("");

				double dEta = pBri->m_plsdManager->GetEtaValue(ePartInFoot, szLoad );
				szEta.Format(_T("%.3f"), dEta);

				if( nCountInwall == 1 )
				{
					m_Grid.AddCell(row, col++, hggettext("����-�߰�����"));
					m_Grid.AddCell(row, col++, hggettext("����-�߰�����"));
				}
				else
				{
					CString strTitle;
					strTitle.Format(_T("����-�߰����� %d"), ixMid+1);
					m_Grid.AddCell(row, col++, strTitle);
					m_Grid.AddCell(row, col++, strTitle);

				}
				m_Grid.AddCell(row, col++, sStatus[iStatus]);
				m_Grid.AddCell(row, col++, szLoad, nFormat, CELL_READONLY);
				m_Grid.AddCell(row, col++, szEta, nFormat, CELL_READONLY);
				m_Grid.AddCell(row, col++, szValue, nFormat, CELL_READONLY);
				m_Grid.AddCell(row, col++, &pBridgeApp->m_bApplyStmModelingMidFooting[ixMid][iStatus][ix], nFormat, nLine, 
					pBridgeApp->m_bApplyStmModelingMidFooting[ixMid][iStatus][ix]==TRUE ? "����" : "�������", CELL_TYPE_CHECK);
				row++;
				col	= 0;
			}
		}
	}
}

void CInputStmCondition::OnTabSelchange( NMHDR *pNMHDR, LRESULT *pResult )
{
	if(GetDlgItem(IDC_TAB))
	{
		m_nTab	= ((CTabCtrl*)GetDlgItem(IDC_TAB))->GetCurSel();
		SetGridData();
		SetDataInit();
	}

	return;
}

void CInputStmCondition::InitComboCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	CString str = _T("");
	HGINT32 nSize = pStd->GetBridgeSize();
	for(HGINT32 ix = 0; ix < nSize; ix++)
	{
		CRcBridgeApp *pBri = pStd->GetBridge(ix);
		str = pStd->GetStringBridgeOrder(ix) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_cbBridgeNo.AddString(str);
	}
	m_cbBridgeNo.SetCurSel(m_pIn->m_nCurrentBridge);
}

void CInputStmCondition::OnButtonSameAll()
{
	CString sMsg = hggettext("��� �������� ����Ÿ�� ���� ���� ����Ÿ�� �����˴ϴ�.");
	sMsg +=_T(" \n");
	sMsg += hggettext("����Ͻðڽ��ϱ�?");
	if(AfxMessageBox(sMsg, MB_YESNO | MB_ICONQUESTION) == IDNO)
		return;

	CARcBridgeDataStd *pStdData = m_pIn->m_pARcBridgeDataStd;
	if (!pStdData) return;

	if (m_nTab == m_nTabCondition)
	{
		CRcBridgeApp *pBridgeSrc = m_pIn->GetRcBridge();
		if (!pBridgeSrc) return;

		CStmDataConfig *pStmSrcData = pBridgeSrc->m_pStmSolverInput->GetStmDataConfig();
		if(!pStmSrcData) return;

		HGINT32 icCountBridge = pStdData->GetBridgeSize();
		for (HGINT32 ix = 0; ix < icCountBridge; ++ix)
		{
			if(ix == m_pIn->m_nCurrentBridge) continue;

			CRcBridgeApp *pBridge = pStdData->GetBridge(ix);
			if (!pBridge) continue;

			CStmDataConfig *pStmData = pBridge->m_pStmSolverInput->GetStmDataConfig();
			*pStmData = *pStmSrcData;

			pBridge->m_nGroundFactorKV = pBridgeSrc->m_nGroundFactorKV;
			pBridge->m_dGroundFactorKV = pBridgeSrc->m_dGroundFactorKV;
			pBridge->m_nGroundFactorBv = pBridgeSrc->m_nGroundFactorBv;
			pBridge->m_nVerticalFactor = pBridgeSrc->m_nVerticalFactor;
		}
	}
	else if (m_nTab == m_nTabStmCheck)
	{
		CRcBridgeApp *pBridgeSrc = m_pIn->GetRcBridge();
		if (!pBridgeSrc) return;

		HGINT32 icCountBridge = pStdData->GetBridgeSize();
		for (HGINT32 ix = 0; ix < icCountBridge; ++ix)
		{
			if(ix == m_pIn->m_nCurrentBridge) continue;
			
			CRcBridgeApp *pBridge = pStdData->GetBridge(ix);
			if (!pBridge) return;

			for (HGINT32 ix = 0; ix < 2; ++ix)
				pBridge->m_bApplyStmModelingBracket[ix] = pBridgeSrc->m_bApplyStmModelingBracket[ix];

			for (HGINT32 ix = 0; ix < 2; ++ix)
				for (HGINT32 jx = 0; jx < 3; ++jx)
					for (HGINT32 kx = 0; kx < 2; ++kx)
						for (HGINT32 lx = 0; lx < 2; ++lx)
							pBridge->m_bApplyStmModelingCorner[ix][jx][kx][lx] = pBridgeSrc->m_bApplyStmModelingCorner[ix][jx][kx][lx];

			for (HGINT32 ix = 0; ix < 2; ++ix)
				for (HGINT32 jx = 0; jx < 3; ++jx)
					for (HGINT32 kx = 0; kx < 5; ++kx)
						pBridge->m_bApplyStmModelingFooting[ix][jx][kx] = pBridgeSrc->m_bApplyStmModelingFooting[ix][jx][kx];

			for (HGINT32 ix = 0; ix < MAX_JIJUM_COUNT_RC; ++ix)
				for (HGINT32 jx = 0; jx < 3; ++jx)
					for (HGINT32 kx = 0; kx < 5; ++kx)
						pBridge->m_bApplyStmModelingMidFooting[ix][jx][kx] = pBridgeSrc->m_bApplyStmModelingMidFooting[ix][jx][kx];
		}
	}
}

void CInputStmCondition::OnCellChangedData( NMHDR* pNotifyStruct, LRESULT* result )
{
	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);

	HGINT32 iScrollPosHor = m_Grid.GetScrollPos(SB_HORZ);
	HGINT32 iScrollPosVert = m_Grid.GetScrollPos(SB_VERT);
	m_Grid.LockWindowUpdate();
	SetGridData();
	SetDataInit();
	m_Grid.UnlockWindowUpdate();
	m_Grid.SetScrollPos(SB_HORZ, iScrollPosHor);
	m_Grid.SetScrollPos(SB_VERT, iScrollPosVert);
}

void CInputStmCondition::SetDataDefaultCondition(HGINT32 ix)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd) return;
	CRcBridgeApp *pBridge = pStd->GetBridge(ix);
	if(!pBridge) return;

	CStmDataConfig *pStmData = pBridge->m_pStmSolverInput->GetStmDataConfig();
	if(!pStmData) return;

	pBridge->m_nGroundFactorKV = 0;
	pBridge->m_dGroundFactorKV = 0.67;
	pBridge->m_nGroundFactorBv = 0;
	pBridge->m_nVerticalFactor = 0;
	pBridge->m_dMeshSizeArcCorner = 0.050;
	pBridge->m_dMeshSizeBracket = 0.100;
	pBridge->m_dMeshSizeBase = 0.200;

	// �����Է�m
	if(m_pIn->m_pARcBridgeDataStd->m_nTypeKciCode == 2)
		pStmData->m_dPiTie = 0.85;
	else
		pStmData->m_dPiTie = 0.75;

	pStmData->m_dPiStrut = 0.75;
	pStmData->m_dPiNode = 0.75;
	pStmData->m_ixDistA = 0;
	pStmData->m_ixRatioPiT_PiS = 0;
	pStmData->m_ixEFactorStrut = 0;

	if(m_bDesignLsd)
	{
		for (long nStmType = eLsdStmCornerUpper; nStmType < eLsdStmCount; ++nStmType)
		{
			double dFck = pBridge->GetValueStmFck(nStmType);
			pBridge->m_Fctk[nStmType] = GetValueFctk(GetValueUnitChange(dFck, UNIT_CGS_KGF_CM2, GetStringUnitByType(UNIT_CGS_KGF_CM2, 1)));
			pBridge->m_Fctk[nStmType] = GetValueUnitChange(pBridge->m_Fctk[nStmType], GetStringUnitByType(UNIT_CGS_KGF_CM2, 1), UNIT_CGS_KGF_CM2);
		}
	}
	else
	{
		SynMaterialUsd(TRUE);
	}
}

void CInputStmCondition::SetDataDefaultStmCheck(HGINT32 ix)
{
	CRcBridgeApp *pBridge = m_pIn->m_pARcBridgeDataStd->GetBridge(ix);
	if(!pBridge) return;

	for (HGINT32 ix = 0; ix < 2; ++ix)
		pBridge->m_bApplyStmModelingBracket[ix] = FALSE;

	for (HGINT32 ix = 0; ix < 2; ++ix)
		for (HGINT32 jx = 0; jx < 3; ++jx)
			for (HGINT32 kx = 0; kx < 2; ++kx)
				for (HGINT32 lx=0; lx<2; ++lx)
					pBridge->m_bApplyStmModelingCorner[ix][jx][kx][lx] = FALSE;

	for (HGINT32 ix = 0; ix < 2; ++ix)
		for (HGINT32 jx = 0; jx < 3; ++jx)
			for (HGINT32 kx = 0; kx < 5; ++kx)
				pBridge->m_bApplyStmModelingFooting[ix][jx][kx] = FALSE;

	for (HGINT32 ix = 0; ix < MAX_JIJUM_COUNT_RC; ++ix)
		for (HGINT32 jx = 0; jx < 3; ++jx)
			for (HGINT32 kx = 0; kx < 5; ++kx)
				pBridge->m_bApplyStmModelingMidFooting[ix][jx][kx] = FALSE;
}

void CInputStmCondition::KillFocusDlg()
{
	CRcBridgeApp *pBridge = m_pIn->GetRcBridge();
	if (!pBridge) return;
	pBridge->MakeStmModelSyncAll();

	return;
}

void CInputStmCondition::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
}

BOOL CInputStmCondition::IsValid()
{
	BOOL bValid = ((m_pIn->m_pARcBridgeDataStd->m_nTypeKciCode>=2 || m_pIn->m_pARcBridgeDataStd->IsLsdDesign()) && m_pIn->m_pARcBridgeDataStd->IsLockRcBridgeStmModule());
	return bValid;
}

CString CInputStmCondition::GetStringValueBracket( CARcBridgeCalcStd *pCalc, HGBOOL bStt )
{
	CString szRet(_T("-"));
	CRcBridgeRebar *pBridge = static_cast<CRcBridgeRebar*>(m_pIn->GetRcBridge());
	if (!pBridge) return szRet;

	CARcBridgeCalcAps *pAps = bStt? pCalc->m_pApsStt[0] : pCalc->m_pApsEnd[0];

	if(pBridge->IsBracket(TRUE) && pAps)
	{
		pAps->CalculateAll();

		m_dEtaBracket = pAps->m_dEta[WING_FACTOR_ULT];
		pBridge->m_strStmModelingBracket[bStt==TRUE ? 0 : 1] = pAps->m_szCombo[APS_CASE_ULT];

		HGDOUBLE dLoadValue = (pBridge->IsAps(bStt, TRUE)) ? (pAps->GetVForceCorbel(TRUE) +  pAps->GetVForceCorbel(FALSE)) : 0.0;
		szRet.Format(_T("%.2f"), dLoadValue);
	}

	return szRet;
}

CString CInputStmCondition::GetStringValueCorner( HGBOOL bStt, HGBOOL bClose, HGBOOL bUpper, ETypeEnvComboLsd eEnvTypeLsd)
{
	CString szRet(_T("-"));
	CRcBridgeRebar *pBridge = static_cast<CRcBridgeRebar*>(m_pIn->GetRcBridge());
	if (!pBridge) return szRet;

	if(!bUpper && !pBridge->IsBoxType())
		return szRet;

	if(pBridge->IsOutWall(bStt) && pBridge->GetFemManage(0)->IsOutputLoaded())
	{
		HGDOUBLE dLoadValue = pBridge->GetMomentCorner(bStt, FALSE, bClose, bUpper, eEnvTypeLsd);
		szRet.Format(_T("%.2f"), dLoadValue);
	}
	
	return szRet;
}

CString CInputStmCondition::GetStringValueFoont( CARcBridgeCalcStd *pCalc, HGINT32 nJijum, HGINT32 iCase, HGINT32 &ixCombo, ETypeEnvComboLsd eEnvTypeLsd )
{
	CString szRet(_T("-"));

	CRcBridgeRebar *pBridge = static_cast<CRcBridgeRebar*>(m_pIn->GetRcBridge());
	if (!pBridge) return szRet;

	CFootingApp* pFooting = pBridge->GetFootingByJijumNumber(nJijum);
	if(pFooting == NULL || pFooting->m_bIs == FALSE) 
		return szRet;

	HGDOUBLE dLoadValue(0), dTemp(0);
	if( m_bDesignLsd )
	{
		long nIdxModel(0);
		ixCombo = pBridge->GetOutValueForFootingDesign_Lsd(nJijum, eEnvTypeLsd, (EFootingCheckLsd)iCase, dLoadValue, dTemp, dTemp, nIdxModel);
		if(nJijum == 0 || nJijum == pBridge->m_nQtyJigan)
		{
			BOOL bStt = (nJijum == 0);
			ELSDCombKind eKindCombo = pBridge->GetComboKindByEnvType(eEnvTypeLsd, bStt ? ePartFootStt : ePartFootEnd, CHECK_POS_CENTER);
			pBridge->m_strStmModelingFooting[bStt? 0 : 1][eEnvTypeLsd][iCase] = pBridge->GetStringComboEta_Lsd(eKindCombo, bStt ? ePartFootStt : ePartFootEnd, ixCombo + 1, TRUE, nIdxModel);
		}
		else
		{
			ELSDCombKind eKindCombo = pBridge->GetComboKindByEnvType(eEnvTypeLsd, ePartInFoot, CHECK_POS_CENTER);
			pBridge->m_strStmModelingMidFooting[nJijum-1][eEnvTypeLsd][iCase] = pBridge->GetStringComboEta_Lsd(eKindCombo, ePartInFoot, ixCombo + 1, TRUE);
		}
	}
	else
		ixCombo = pBridge->GetOutValueForFootingDesign(nJijum, iCase, dLoadValue, dTemp, dTemp, FALSE);

	if(!Compare(dLoadValue, 0.0, _T("="), 0.01))
		szRet.Format(_T("%.2f"), dLoadValue);

	return szRet;
}

void CInputStmCondition::AddCellTension(long nStmType, long &row, long &col)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	CRcBridgeApp *pBridge = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBridge) return;

	CStmDataConfig *pStmData = pBridge->m_pStmSolverInput->GetStmDataConfig();
	if(!pStmData) return;

	UINT nFormatLeft = DT_LEFT | DT_VCENTER;
	UINT nFormatRight	= DT_RIGHT | DT_VCENTER;

	CString szUnit_CGS   = UNIT_CGS_KGF_CM2;
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	CString strTitle = pBridge->GetStrLsdStmTitle(nStmType);
	m_Grid.AddCell(row, col++, hggettext("������ ����"));
	if( m_bDesignLsd ) 
		m_Grid.AddCell(row, col++, strTitle);
	m_Grid.AddCell(row, col++, hggettext(" Fck : ��ũ��Ʈ ����"), nFormatLeft);
	m_Grid.AddCell(row, col++, m_bDesignLsd==TRUE ? &pBridge->GetValueStmFck(nStmType) : &pStmData->m_dFck, nFormatRight, CELL_READONLY);	
	if(m_bDesignLsd) 
		m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
	else
		m_Grid.SetCellUnit(szUnit_Apply, szUnit_Apply);
	// pStmData�� �ִ� Fck���� �����谡 ����Ǿ��� ���̴�.
	
	m_Grid.AddCell(row, col, szUnit_Apply, nFormatLeft, CELL_READONLY);
	row++;
	col	= 0;

	m_Grid.AddCell(row, col++, hggettext("������ ����"));
	if( m_bDesignLsd ) 
		m_Grid.AddCell(row, col++, strTitle);
	m_Grid.AddCell(row, col++, m_bDesignLsd==TRUE ? hggettext(" fctk : ��ũ��Ʈ ���� ����") : hggettext(" ft : ��ũ��Ʈ ���� ����"), nFormatLeft);
	m_Grid.AddCell(row, col++, m_bDesignLsd==TRUE ? &pBridge->GetValueStmFctk(nStmType) : &pStmData->m_dFt, nFormatRight);	
	if(m_bDesignLsd) 
		m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
	else
		m_Grid.SetCellUnit(szUnit_Apply, szUnit_Apply);

	m_Grid.AddCell(row, col, szUnit_Apply, nFormatLeft, CELL_READONLY);
	row++;
	col	= 0;

	m_Grid.AddCell(row, col++, hggettext("������ ����"));
	if( m_bDesignLsd ) 
		m_Grid.AddCell(row, col++, strTitle);
	m_Grid.AddCell(row, col++, hggettext(" Ec : ��ũ��Ʈ ź�����"), nFormatLeft);
	m_Grid.AddCell(row, col++, m_bDesignLsd==TRUE ? &pBridge->GetValueStmEc(nStmType) : &pStmData->m_dEc, nFormatRight, CELL_READONLY);	
	if(m_bDesignLsd) 
		m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
	else
		m_Grid.SetCellUnit(szUnit_Apply, szUnit_Apply);

	m_Grid.AddCell(row, col, szUnit_Apply, nFormatLeft, CELL_READONLY);
	row++;
	col	= 0;

	m_Grid.AddCell(row, col++, hggettext("������ ����"));
	if( m_bDesignLsd ) 
		m_Grid.AddCell(row, col++, strTitle);
	m_Grid.AddCell(row, col++, hggettext(" Fy : ö�� �׺�����"), nFormatLeft);
	m_Grid.AddCell(row, col++, m_bDesignLsd==TRUE ? &pBridge->GetValueStmFy(nStmType) : &pStmData->m_dFy, nFormatRight, CELL_READONLY);	
	if(m_bDesignLsd) 
		m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
	else
		m_Grid.SetCellUnit(szUnit_Apply, szUnit_Apply);
	m_Grid.AddCell(row, col, szUnit_Apply, nFormatLeft, CELL_READONLY);
	row++;
	col	= 0;
	//
	if(m_bDesignLsd) 
	{
		m_Grid.AddCell(row, col++, hggettext("������ ����"));
		m_Grid.AddCell(row, col++, strTitle);
		m_Grid.AddCell(row, col++, hggettext(" Fvy : ö�� �׺�����"), nFormatLeft);
		m_Grid.AddCell(row, col++, &pBridge->GetValueStmFvy(nStmType), nFormatRight, CELL_READONLY);	
		m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
		m_Grid.AddCell(row, col, szUnit_Apply, nFormatLeft, CELL_READONLY);
		row++;
		col	= 0;
	}
}

void CInputStmCondition::SynMaterialUsd( BOOL bDefault )
{
	if( m_bDesignLsd )
		return;

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd) return;
	CRcBridgeApp *pBridge = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBridge) return;

	CStmDataConfig *pStmData = pBridge->m_pStmSolverInput->GetStmDataConfig();
	if(!pStmData) return;

	// ������ ������ CStmDataConfigModel�� ����
	CString szSI = _T("MPa");

	pStmData->m_dFck = GetValueUnitChange(pStd->m_Fck, UNIT_CGS_KGF_CM2, szSI);
	pStmData->m_dFy  = GetValueUnitChange(pStd->m_Fy, UNIT_CGS_KGF_CM2, szSI);
	pStmData->m_dFvy  = pStmData->m_dFy;
	pStmData->m_dEc  = GetValueUnitChange(pStd->m_Ec, UNIT_CGS_KGF_CM2, szSI);
	pStmData->m_dEs  = GetValueUnitChange(pStd->m_Es, UNIT_CGS_KGF_CM2, szSI);

	if(bDefault)
		pStmData->m_dFt = pStmData->m_dFck * 0.1;
}
