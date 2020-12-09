// DialogSTMSetting.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "DialogSTMSetting.h"
#include "afxdialogex.h"

// CDialogSTMSetting ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogSTMSetting, CDialog)

CDialogSTMSetting::CDialogSTMSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSTMSetting::IDD, pParent)
{
	m_pIn = NULL;
	m_ixStmKey = -1;
}

CDialogSTMSetting::~CDialogSTMSetting()
{
}

void CDialogSTMSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDialogSTMSetting, CDialog)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_BN_CLICKED(IDOK, &CDialogSTMSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogSTMSetting::OnBnClickedCancel)
END_MESSAGE_MAP()

// CDialogSTMSetting �޽��� ó�����Դϴ�.

BOOL CDialogSTMSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(!m_pIn) return FALSE;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if (!pBri) return FALSE;

	CStmModelForArcbridge *pModel = static_cast<CStmModelForArcbridge*>(pBri->GetStmModelByKey(m_ixStmKey));
	if(!pModel) return FALSE;

	CStmDataConfig *pDataModeling = pModel->GetStmDataConfig();
	m_DataModeling_Ori = *pDataModeling;

	if (IsCornerCloseType() || IsCornerOpenType())
	{
		SetOriStmCornerData();
	}

	SetGridData();
 	SetDataInit();

	return TRUE;
}

void CDialogSTMSetting::SetDataInit()
{
	m_Grid.UpdateData(FALSE);

	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols);

	m_Grid.MergeGridEx(0, nRows-1, 0, 0);
}

void CDialogSTMSetting::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
}

void CDialogSTMSetting::SetGridData()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if (!pBri) return;

	CStmModelForArcbridge *pModel = static_cast<CStmModelForArcbridge*>(pBri->GetStmModelByKey(m_ixStmKey));
	if(!pModel) return;

	CStmDataConfig* pDataModeling = pModel->GetStmDataConfig();
	if(!pDataModeling) return;

	BOOL bLsdDesign = m_pIn->m_pARcBridgeDataStd->IsLsdDesign();
	CStringArray sArr;
	HGINT32 nRow		= 0;
	HGINT32 nCol		= 0;
	UINT nFormat	= DT_RIGHT | DT_VCENTER;
	UINT nLine	= CELL_LINE_NO;

	m_Grid.SetGridDefault(2, 2, 0, 2);

	if (IsCornerCloseType() || IsCornerOpenType())
	{
		sArr.RemoveAll();
		sArr.Add(hggettext("Frame�𵨸���ġ"));
		sArr.Add(hggettext("���ܸ�"));
		m_Grid.AddCell(nRow, nCol++, hggettext("�����帧�� �������Ϲ�"));
		m_Grid.AddCell(nRow, nCol++, hggettext("�������"));
		m_Grid.AddCell(nRow, nCol, (long*)&pModel->m_nTypeLoadStressFlow, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		nRow++;
		nCol	= 0;

		// ��ٵ� ö���� �����Ҽ� �����Ƿ� Type 1�� �����ϱ�� ����.
// 		sArr.RemoveAll();
// 		sArr.Add(hggettext("Type1"));
// 		sArr.Add(hggettext("Type2"));
// 		m_Grid.AddCell(nRow, nCol++, hggettext("�𵨸�Type"));
// 		m_Grid.AddCell(nRow, nCol++, hggettext("�������"));
// 		m_Grid.AddCell(nRow, nCol++, (long*)&pBri->m_nTypeStmModeling[m_nIndexCorner], nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
// 		nRow++;
// 		nCol	= 0;
// 
// 		m_Grid.AddCell(nRow, nCol++, hggettext("�𵨸�Type"));
// 		m_Grid.AddCell(nRow, nCol++, hggettext(""));
// 		m_Grid.AddCell(nRow, nCol++, hggettext(""));
// 		nRow++;
// 		nCol	= 0;
	}

	if(IsFooting())
	{
		sArr.RemoveAll();
		sArr.Add(hggettext("1"));
		sArr.Add(hggettext("2"));
		sArr.Add(hggettext("3"));
		m_Grid.AddCell(nRow, nCol++, hggettext("��� �ּ�������"));
		m_Grid.AddCell(nRow, nCol++, hggettext("��� �ּ�������"));
		m_Grid.AddCell(nRow, nCol, (long*)&pDataModeling->m_ixMinSupportNode, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		nRow++;
		nCol	= 0;

		sArr.RemoveAll();
		sArr.Add(hggettext("���ö�ٵ���"));
		sArr.Add(hggettext("��յβ����"));
		sArr.Add(hggettext("(�д�����/�ۿ�����) x ��յβ�"));
		sArr.Add(hggettext("�д�����/(fce x 2)"));
		sArr.Add(hggettext("����� �Է�"));
		m_Grid.AddCell(nRow, nCol++, hggettext("��� ��ȿ�� ����"));
		m_Grid.AddCell(nRow, nCol++, hggettext("�������"));
		m_Grid.AddCell(nRow, nCol, (long*)&pDataModeling->m_ixEffectiveWidthColumn, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		nRow++;
		nCol	= 0;

		m_Grid.AddCell(nRow, nCol++, hggettext("��� ��ȿ�� ����"));
		m_Grid.AddCell(nRow, nCol++, hggettext("����� �Է�"));
		m_Grid.AddCell(nRow, nCol, &pDataModeling->m_dUserEffectiveWidthColumn, nFormat, pDataModeling->m_ixEffectiveWidthColumn == 4 ? nLine : CELL_READONLY);
		nRow++;
		nCol	= 0;

		if(bLsdDesign)
		{
			long nJijum = 0;
			if(m_ixStmKey < STM_MODEL_FOOT_END_MAX_AXIAL)
				nJijum = 0;
			else if(m_ixStmKey < STM_MODEL_FOOT_MID_MAX_AXIAL)
				nJijum = pBri->m_nQtyJigan;
			else
			{
				long iInex = (m_ixStmKey - STM_MODEL_FOOT_MID_MAX_AXIAL);				
				nJijum = (iInex / 15 + 1);
			}

			BOOL bPileBase = pBri->IsPileBase(nJijum);
			
			sArr.RemoveAll();
			sArr.Add(hggettext("��� �� ��ü�� �ۿ�"));
			sArr.Add(hggettext("�����ϴ� �ݷ� �ۿ�"));
			m_Grid.AddCell(nRow, nCol++, hggettext("���ʸ𵨸��� ��������"));
			m_Grid.AddCell(nRow, nCol++, hggettext(" ������� "));
			m_Grid.AddCell(nRow, nCol, (long*)&pDataModeling->m_ixTypeFootingLoad, nFormat, TRUE, bPileBase ? nLine : CELL_READONLY, _T(""), CELL_TYPE_COMBO, sArr);
			nRow++;
			nCol	= 0;
		}

		sArr.RemoveAll();
		sArr.Add(hggettext("���̺���������"));
		sArr.Add(hggettext("Spring�����Ǳ⺻���ݱ���"));
		m_Grid.AddCell(nRow, nCol++, hggettext("���� Node�⺻����"));
		m_Grid.AddCell(nRow, nCol++, hggettext("�������"));
		m_Grid.AddCell(nRow, nCol, (long*)&pDataModeling->m_ixTypeDivideSpringSpace, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		nRow++;
		nCol	= 0;

		m_Grid.AddCell(nRow, nCol++, hggettext("���� Node�⺻����"));
		m_Grid.AddCell(nRow, nCol++, hggettext("���ذ�"));
		if(pDataModeling->m_ixTypeDivideSpringSpace == 0)
			m_Grid.AddCell(nRow, nCol, &pDataModeling->m_dRateHeightSpringSpace, nFormat, nLine);
		else
			m_Grid.AddCell(nRow, nCol, &pDataModeling->m_dDivideSpringSpace, nFormat, nLine);
		nRow++;
		nCol	= 0;
	}

	sArr.RemoveAll();
	sArr.Add(hggettext("1��ö����ġ(�Ǻ�)"));
	sArr.Add(hggettext("ö���ǵ���"));
	sArr.Add(hggettext("����� �Է�"));
	m_Grid.AddCell(nRow, nCol++, hggettext("ö��Ÿ�� ��ġ"));
	m_Grid.AddCell(nRow, nCol++, hggettext("�������"));
	m_Grid.AddCell(nRow, nCol, (long*)&pModel->m_nTypePosRebarTie, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
	nRow++;
	nCol	= 0;

	m_Grid.AddCell(nRow, nCol++, hggettext("ö��Ÿ�� ��ġ"));
	m_Grid.AddCell(nRow, nCol++, hggettext("������"));
	m_Grid.AddCell(nRow, nCol, &pModel->m_dUserPosRebarTie, nFormat,pModel->m_nTypePosRebarTie == 2 ? nLine : CELL_READONLY);
	nRow++;
	nCol	= 0;

	CString strRatio = m_pIn->m_pARcBridgeDataStd->IsLsdDesign() ? hggettext("������װ����") : hggettext("�������Ұ����");

	sArr.RemoveAll();
	sArr.Add("����簢�� ���º� ����/2 x " + strRatio);
	sArr.Add(hggettext("1��ö����ġ(�Ǻ�)"));
	sArr.Add(hggettext("ö���ǵ���"));
	sArr.Add(hggettext("1��ö����ġ(�Ǻ�) x ") + strRatio);
	sArr.Add(hggettext("ö���ǵ��� x ")  + strRatio);
	sArr.Add(hggettext("����� �Է�"));
	m_Grid.AddCell(nRow, nCol++, hggettext("���ེƮ�� ��ġ"));
	m_Grid.AddCell(nRow, nCol++, hggettext("�������"));
	m_Grid.AddCell(nRow, nCol, (long*)&pModel->m_nTypePosCompressStrut, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
	nRow++;
	nCol	= 0;
	
	m_Grid.AddCell(nRow, nCol++, hggettext("���ེƮ�� ��ġ"));
	m_Grid.AddCell(nRow, nCol++, hggettext("������"));
	m_Grid.AddCell(nRow, nCol, &pModel->m_dUserPosCompressStrut, nFormat,pModel->m_nTypePosCompressStrut == 5 ? nLine : CELL_READONLY);
	nRow++;
	nCol	= 0;
	
	if (IsFooting())
	{
		sArr.RemoveAll();
		sArr.Add(hggettext("������"));
		sArr.Add(hggettext("������+�����"));
		m_Grid.AddCell(nRow, nCol++, hggettext("Spring Type"));
		m_Grid.AddCell(nRow, nCol++, hggettext("Spring Type"));
		m_Grid.AddCell(nRow, nCol++, (long*)&pDataModeling->m_ixSpringType, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);

		if(bLsdDesign) //�Ѱ���¿����� �������տ� �ִ� ���߼����� �𵨸����� �̵���
		{
			nRow++;
			nCol	= 0;

			sArr.RemoveAll();
			sArr.Add(hggettext("������ġ ��������"));
			sArr.Add(hggettext("�������� ������ġ ��������"));
			sArr.Add(hggettext("�������"));
			m_Grid.AddCell(nRow, nCol++, hggettext("���� ����"));
			m_Grid.AddCell(nRow, nCol++, hggettext("���� ����"));
			m_Grid.AddCell(nRow, nCol++, (long*)&pDataModeling->m_ixSelfWeight, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		}
	}

	if (IsCornerCloseType())
	{
		sArr.RemoveAll();
		sArr.Add("R���� ����� �Է�");
		sArr.Add(hggettext("����ö�� ����"));
		sArr.Add(hggettext("����� �Է�"));
		m_Grid.AddCell(nRow, nCol++, hggettext("���������� ��ġ"));
		m_Grid.AddCell(nRow, nCol++, hggettext("�������"));
		m_Grid.AddCell(nRow, nCol, (long*)&pModel->m_nTypeDistCornerRebar, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		nRow++;
		nCol	= 0;

		m_Grid.AddCell(nRow, nCol++, hggettext("���������� ��ġ"));
		m_Grid.AddCell(nRow, nCol++, hggettext("������"));
		m_Grid.AddCell(nRow, nCol, &pModel->m_dUserDistCornerRebar, nFormat,pModel->m_nTypeDistCornerRebar == 1 ? CELL_READONLY : nLine);
		nRow++;
		nCol	= 0;

		sArr.RemoveAll();
		sArr.Add("R���� ����� �Է�");
		sArr.Add(hggettext("��ġö����ġ(�Ǻ�)"));
		sArr.Add(hggettext("����� �Է�"));
		m_Grid.AddCell(nRow, nCol++, hggettext("���������� ��ġ"));
		m_Grid.AddCell(nRow, nCol++, hggettext("�������"));
		m_Grid.AddCell(nRow, nCol, (long*)&pModel->m_nTypeDistHunchRebar, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		nRow++;
		nCol	= 0;

		m_Grid.AddCell(nRow, nCol++, hggettext("���������� ��ġ"));
		m_Grid.AddCell(nRow, nCol++, hggettext("������"));
		m_Grid.AddCell(nRow, nCol, &pModel->m_dUserDistHunchRebar, nFormat,pModel->m_nTypeDistHunchRebar == 1 ? CELL_READONLY : nLine);
		nRow++;
		nCol	= 0;

		sArr.RemoveAll();
		sArr.Add(_T("45��"));
		sArr.Add(hggettext("Hunch"));
		m_Grid.AddCell(nRow, nCol++, hggettext("���������� ����"));
		m_Grid.AddCell(nRow, nCol++, hggettext("������"));
		m_Grid.AddCell(nRow, nCol, (long*)&pModel->m_ixInnerAngle, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		nRow++;
		nCol	= 0;
	}
	
	if (IsCornerOpenType())
	{
		sArr.RemoveAll();
		sArr.Add(hggettext("45�� ���� �𵨸�"));
		sArr.Add(hggettext("������ 1/2���� �𵨸�"));
		m_Grid.AddCell(nRow, nCol++, hggettext("�������� ��Ʈ������"));
		m_Grid.AddCell(nRow, nCol++, hggettext("�������"));
		m_Grid.AddCell(nRow, nCol, (long*)&pModel->m_nTypeStirrupInnerTension, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		nRow++;
		nCol	= 0;

		m_Grid.AddCell(nRow, nCol++, hggettext("�������� ��Ʈ������"));
		m_Grid.AddCell(nRow, nCol++, hggettext("������"));
		m_Grid.AddCell(nRow, nCol, &pModel->m_dUserStirrupInnerTension, nFormat,pModel->m_nTypeStirrupInnerTension == 0 ? nLine : CELL_READONLY);
	}

	long row = 0;
	long col = 0;
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);

	m_Grid.SetRowHeightAll(20);
	
	m_Grid.SetColumnWidth(0, 230);
	m_Grid.SetColumnWidth(1, 80);
	m_Grid.SetColumnWidth(2, 300);

	if(IsFooting())
	{
		m_Grid.SetMergeCol(0, 0, 1, TRUE);
		m_Grid.SetMergeCol(10, 0, 1, TRUE);
	}
}

void CDialogSTMSetting::OnBnClickedOk()
{
	SetDataSave();
	BOOL bCorner = (m_ixStmKey >= STM_MODEL_CORNER_STT_UPPER_MINUS && m_ixStmKey <= STM_MODEL_CORNER_END_LOWER_PLUS) || (m_ixStmKey >= STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1 && m_ixStmKey <= STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2);

	// (ARCBRIDGE-3236) �찢�δ� �����ϰ� ��� �� ������ �߰�.
	if(IsChangedData() && !bCorner)
	{
		if(AfxMessageBox(_T("����������տ� ���� �����Ͻðڽ��ϱ�?"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			SameAllConfigData();
		}
	}


	CDialog::OnOK();
}


void CDialogSTMSetting::OnBnClickedCancel()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if (!pBri) return;

	CStmModelForArcbridge *pModel = static_cast<CStmModelForArcbridge*>(pBri->GetStmModelByKey(m_ixStmKey));
	if(!pModel) return;

	CStmDataConfig *pDataModeling = pModel->GetStmDataConfig();
	*pDataModeling = m_DataModeling_Ori;

	if (IsCornerCloseType() || IsCornerOpenType())
	{
		pModel->m_nTypePosRebarTie	= m_nOriTypePosRebarTie;
		pModel->m_dUserPosRebarTie	= m_dOriUserPosRebarTie;
		pModel->m_nTypePosCompressStrut	= m_nTOriypePosCompressStrut;
		pModel->m_dUserPosCompressStrut	= m_dOriUserPosCompressStrut;

		pModel->m_nTypeLoadStressFlow = m_nOriTypeLoadStressFlow;
		pModel->m_nTypeStmModeling = m_nOriTypeStmModeling;			
		pModel->m_nTypeDistCornerRebar = m_nOriTypeDistCornerRebar;		
		pModel->m_dUserDistCornerRebar = m_dOriUserDistCornerRebar;	
		pModel->m_nTypeDistHunchRebar = m_nOriTypeDistHunchRebar;		
		pModel->m_dUserDistHunchRebar = m_dOriUserDistHunchRebar;		
		pModel->m_ixInnerAngle		= m_nOriInnerAngle;
		pModel->m_nTypeStirrupInnerTension = m_nOriTypeStirrupInnerTension;	
		pModel->m_dUserStirrupInnerTension = m_dOriUserStirrupInnerTension;
	}
		
	CDialog::OnCancel();
}

void CDialogSTMSetting::OnCellChangedData( NMHDR* pNotifyStruct, LRESULT* result )
{
	structGridCell *pCell = m_Grid.GetCurrentCell();

	if (pCell)
	{
		SetDataSave();
		SetGridData();
		SetDataInit();
	}
}

BOOL CDialogSTMSetting::IsChangedData()
{
	CStmDataConfig* pDataModeling = m_pIn->GetStmDataConfig(m_ixStmKey);
	if(!pDataModeling) return FALSE;

	if(*pDataModeling == m_DataModeling_Ori) 
		return FALSE;
	else
		return TRUE;
}

void CDialogSTMSetting::SameAllConfigData()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if (!pBri) return;

	long ixKeyStt(-1), ixKeyEnd(-1);
	if(m_ixStmKey >= STM_MODEL_BRACKET_STT && m_ixStmKey <= STM_MODEL_BRACKET_END)
	{
		ixKeyStt = STM_MODEL_BRACKET_STT;
		ixKeyEnd = STM_MODEL_BRACKET_END;
	}
	else if((m_ixStmKey >= STM_MODEL_CORNER_STT_UPPER_MINUS && m_ixStmKey <= STM_MODEL_CORNER_END_LOWER_PLUS) || (m_ixStmKey >= STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1 && m_ixStmKey <= STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2))
	{
		// �찢�� ���� Key���� ���ӵ��� �ʴ´�.
		ixKeyStt = STM_MODEL_CORNER_STT_UPPER_MINUS;
		ixKeyEnd = STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2;
	}
	else if(m_ixStmKey >= STM_MODEL_FOOT_STT_MAXP && m_ixStmKey < STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1)
	{
		ixKeyStt = STM_MODEL_FOOT_STT_MAXP;
		ixKeyEnd = STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1-1;
	}
	else if(m_ixStmKey >= STM_MODEL_FOOT_STT_MAX_AXIAL && m_ixStmKey <= STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_2)
	{
		ixKeyStt = STM_MODEL_FOOT_STT_MAX_AXIAL;
		ixKeyEnd = STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_2;
	}
	else if(m_ixStmKey >= STM_MODEL_FOOT_END_MAX_AXIAL && m_ixStmKey <= STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2)
	{
		ixKeyStt = STM_MODEL_FOOT_END_MAX_AXIAL;
		ixKeyEnd = STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2;
	}
	else if(m_ixStmKey >= STM_MODEL_FOOT_MID_MAX_AXIAL)
	{
		ixKeyStt = STM_MODEL_FOOT_MID_MAX_AXIAL;
		ixKeyEnd = ixKeyStt + pBri->GetCountInWall() * 15;
	}

	CStmModelForArcbridge *pDataSou = static_cast<CStmModelForArcbridge*>(pBri->GetStmModelByKey(m_ixStmKey));
	if(!pDataSou) return;
	CStmDataConfig* pConfigSou = pDataSou->GetStmDataConfig();
	if(!pConfigSou) return;

	for(long ix=ixKeyStt; ix<=ixKeyEnd; ++ix)
	{
		if(ix == m_ixStmKey)
			continue;

		// �찢�� ���� Key���� ���ӵ��� �ʴ´�.
		if(ixKeyStt == STM_MODEL_CORNER_STT_UPPER_MINUS && ix > STM_MODEL_CORNER_END_LOWER_PLUS && ix < STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1)
			continue;

		CStmModelForArcbridge *pDes = static_cast<CStmModelForArcbridge*>(pBri->GetStmModelByKey(ix));
		if(!pDes) continue;
		CStmDataConfig* pConfigDes = pDes->GetStmDataConfig();
		if(!pConfigDes) continue;

		pConfigDes->m_ixMinSupportNode			= pConfigSou->m_ixMinSupportNode;
		pConfigDes->m_ixEffectiveWidthColumn	= pConfigSou->m_ixEffectiveWidthColumn;
		pConfigDes->m_dUserEffectiveWidthColumn	= pConfigSou->m_dUserEffectiveWidthColumn;
		pConfigDes->m_ixTypeFootingLoad			= pConfigSou->m_ixTypeFootingLoad;
		pConfigDes->m_ixTypeDivideSpringSpace	= pConfigSou->m_ixTypeDivideSpringSpace;
		pConfigDes->m_dRateHeightSpringSpace	= pConfigSou->m_dRateHeightSpringSpace;
		pConfigDes->m_dDivideSpringSpace		= pConfigSou->m_dDivideSpringSpace;
		pDes->m_nTypePosRebarTie				= pDataSou->m_nTypePosRebarTie;
		pDes->m_dUserPosRebarTie				= pDataSou->m_dUserPosRebarTie;
		pDes->m_nTypePosCompressStrut			= pDataSou->m_nTypePosCompressStrut;
		pConfigDes->m_ixSpringType				= pConfigSou->m_ixSpringType;
		pConfigDes->m_ixSelfWeight				= pConfigSou->m_ixSelfWeight;
	}
}

BOOL CDialogSTMSetting::IsFooting()
{
	return ((m_ixStmKey >= STM_MODEL_FOOT_STT_MAXP && m_ixStmKey < STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1) || m_ixStmKey >= STM_MODEL_FOOT_STT_MAX_AXIAL);
}

BOOL CDialogSTMSetting::IsCornerCloseType()
{
	return ((m_ixStmKey >= STM_MODEL_CORNER_STT_UPPER_MINUS && m_ixStmKey <= STM_MODEL_CORNER_END_LOWER_MINUS) || 
		(m_ixStmKey >= STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1 && m_ixStmKey <= STM_MODEL_CORNER_END_LOWER_MINUS_EXT_1) ||
		(m_ixStmKey >= STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2 && m_ixStmKey <= STM_MODEL_CORNER_END_LOWER_MINUS_EXT_2));
}

BOOL CDialogSTMSetting::IsCornerOpenType()
{
	return ((m_ixStmKey >= STM_MODEL_CORNER_STT_UPPER_PLUS && m_ixStmKey <= STM_MODEL_CORNER_END_LOWER_PLUS) || 
		(m_ixStmKey >= STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_1 && m_ixStmKey <= STM_MODEL_CORNER_END_LOWER_PLUS_EXT_1) ||
		(m_ixStmKey >= STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_2 && m_ixStmKey <= STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2));
}

void CDialogSTMSetting::SetOriStmCornerData()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if (!pBri) return;

	CStmModelForArcbridge *pModel = static_cast<CStmModelForArcbridge*>(pBri->GetStmModelByKey(m_ixStmKey));
	if(!pModel) return;

	m_nOriTypePosRebarTie	= pModel->m_nTypePosRebarTie;
	m_dOriUserPosRebarTie	= pModel->m_dUserPosRebarTie;
	m_nTOriypePosCompressStrut	= pModel->m_nTypePosCompressStrut;
	m_dOriUserPosCompressStrut	= pModel->m_dUserPosCompressStrut;

	m_nOriTypeLoadStressFlow = pModel->m_nTypeLoadStressFlow;
	m_nOriTypeStmModeling = pModel->m_nTypeStmModeling;			
	m_nOriTypeDistCornerRebar = pModel->m_nTypeDistCornerRebar;		
	m_dOriUserDistCornerRebar = pModel->m_dUserDistCornerRebar;	
	m_nOriTypeDistHunchRebar = pModel->m_nTypeDistHunchRebar;		
	m_dOriUserDistHunchRebar = pModel->m_dUserDistHunchRebar;		
	m_nOriInnerAngle		= pModel->m_ixInnerAngle;
	m_nOriTypeStirrupInnerTension = pModel->m_nTypeStirrupInnerTension;	
	m_dOriUserStirrupInnerTension = pModel->m_dUserStirrupInnerTension;	
}
