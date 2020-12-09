// DialogSTMSetting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "DialogSTMSetting.h"
#include "afxdialogex.h"

// CDialogSTMSetting 대화 상자입니다.

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

// CDialogSTMSetting 메시지 처리기입니다.

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
		sArr.Add(hggettext("Frame모델링위치"));
		sArr.Add(hggettext("전단면"));
		m_Grid.AddCell(nRow, nCol++, hggettext("응력흐름도 하중재하법"));
		m_Grid.AddCell(nRow, nCol++, hggettext("산정방식"));
		m_Grid.AddCell(nRow, nCol, (long*)&pModel->m_nTypeLoadStressFlow, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		nRow++;
		nCol	= 0;

		// 배근된 철근을 변경할수 없으므로 Type 1만 지원하기로 결정.
// 		sArr.RemoveAll();
// 		sArr.Add(hggettext("Type1"));
// 		sArr.Add(hggettext("Type2"));
// 		m_Grid.AddCell(nRow, nCol++, hggettext("모델링Type"));
// 		m_Grid.AddCell(nRow, nCol++, hggettext("산정방식"));
// 		m_Grid.AddCell(nRow, nCol++, (long*)&pBri->m_nTypeStmModeling[m_nIndexCorner], nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
// 		nRow++;
// 		nCol	= 0;
// 
// 		m_Grid.AddCell(nRow, nCol++, hggettext("모델링Type"));
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
		m_Grid.AddCell(nRow, nCol++, hggettext("기둥 최소지지점"));
		m_Grid.AddCell(nRow, nCol++, hggettext("기둥 최소지지점"));
		m_Grid.AddCell(nRow, nCol, (long*)&pDataModeling->m_ixMinSupportNode, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		nRow++;
		nCol	= 0;

		sArr.RemoveAll();
		sArr.Add(hggettext("기둥철근도심"));
		sArr.Add(hggettext("기둥두께등분"));
		sArr.Add(hggettext("(분담하중/작용하중) x 기둥두께"));
		sArr.Add(hggettext("분담하중/(fce x 2)"));
		sArr.Add(hggettext("사용자 입력"));
		m_Grid.AddCell(nRow, nCol++, hggettext("기둥 유효폭 산정"));
		m_Grid.AddCell(nRow, nCol++, hggettext("산정방식"));
		m_Grid.AddCell(nRow, nCol, (long*)&pDataModeling->m_ixEffectiveWidthColumn, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		nRow++;
		nCol	= 0;

		m_Grid.AddCell(nRow, nCol++, hggettext("기둥 유효폭 산정"));
		m_Grid.AddCell(nRow, nCol++, hggettext("사용자 입력"));
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
			sArr.Add(hggettext("기둥 및 벽체만 작용"));
			sArr.Add(hggettext("기초하단 반력 작용"));
			m_Grid.AddCell(nRow, nCol++, hggettext("기초모델링시 하중재하"));
			m_Grid.AddCell(nRow, nCol++, hggettext(" 산정방식 "));
			m_Grid.AddCell(nRow, nCol, (long*)&pDataModeling->m_ixTypeFootingLoad, nFormat, TRUE, bPileBase ? nLine : CELL_READONLY, _T(""), CELL_TYPE_COMBO, sArr);
			nRow++;
			nCol	= 0;
		}

		sArr.RemoveAll();
		sArr.Add(hggettext("높이비설정값기준"));
		sArr.Add(hggettext("Spring산정의기본간격기준"));
		m_Grid.AddCell(nRow, nCol++, hggettext("기초 Node기본간격"));
		m_Grid.AddCell(nRow, nCol++, hggettext("산정방식"));
		m_Grid.AddCell(nRow, nCol, (long*)&pDataModeling->m_ixTypeDivideSpringSpace, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		nRow++;
		nCol	= 0;

		m_Grid.AddCell(nRow, nCol++, hggettext("기초 Node기본간격"));
		m_Grid.AddCell(nRow, nCol++, hggettext("기준값"));
		if(pDataModeling->m_ixTypeDivideSpringSpace == 0)
			m_Grid.AddCell(nRow, nCol, &pDataModeling->m_dRateHeightSpringSpace, nFormat, nLine);
		else
			m_Grid.AddCell(nRow, nCol, &pDataModeling->m_dDivideSpringSpace, nFormat, nLine);
		nRow++;
		nCol	= 0;
	}

	sArr.RemoveAll();
	sArr.Add(hggettext("1단철근위치(피복)"));
	sArr.Add(hggettext("철근의도심"));
	sArr.Add(hggettext("사용자 입력"));
	m_Grid.AddCell(nRow, nCol++, hggettext("철근타이 위치"));
	m_Grid.AddCell(nRow, nCol++, hggettext("산정방식"));
	m_Grid.AddCell(nRow, nCol, (long*)&pModel->m_nTypePosRebarTie, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
	nRow++;
	nCol	= 0;

	m_Grid.AddCell(nRow, nCol++, hggettext("철근타이 위치"));
	m_Grid.AddCell(nRow, nCol++, hggettext("설정값"));
	m_Grid.AddCell(nRow, nCol, &pModel->m_dUserPosRebarTie, nFormat,pModel->m_nTypePosRebarTie == 2 ? nLine : CELL_READONLY);
	nRow++;
	nCol	= 0;

	CString strRatio = m_pIn->m_pARcBridgeDataStd->IsLsdDesign() ? hggettext("재료저항계수비") : hggettext("강도감소계수비");

	sArr.RemoveAll();
	sArr.Add("등가직사각형 응력블럭 깊이/2 x " + strRatio);
	sArr.Add(hggettext("1단철근위치(피복)"));
	sArr.Add(hggettext("철근의도심"));
	sArr.Add(hggettext("1단철근위치(피복) x ") + strRatio);
	sArr.Add(hggettext("철근의도심 x ")  + strRatio);
	sArr.Add(hggettext("사용자 입력"));
	m_Grid.AddCell(nRow, nCol++, hggettext("압축스트럿 위치"));
	m_Grid.AddCell(nRow, nCol++, hggettext("산정방식"));
	m_Grid.AddCell(nRow, nCol, (long*)&pModel->m_nTypePosCompressStrut, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
	nRow++;
	nCol	= 0;
	
	m_Grid.AddCell(nRow, nCol++, hggettext("압축스트럿 위치"));
	m_Grid.AddCell(nRow, nCol++, hggettext("설정값"));
	m_Grid.AddCell(nRow, nCol, &pModel->m_dUserPosCompressStrut, nFormat,pModel->m_nTypePosCompressStrut == 5 ? nLine : CELL_READONLY);
	nRow++;
	nCol	= 0;
	
	if (IsFooting())
	{
		sArr.RemoveAll();
		sArr.Add(hggettext("수직력"));
		sArr.Add(hggettext("수직력+수평력"));
		m_Grid.AddCell(nRow, nCol++, hggettext("Spring Type"));
		m_Grid.AddCell(nRow, nCol++, hggettext("Spring Type"));
		m_Grid.AddCell(nRow, nCol++, (long*)&pDataModeling->m_ixSpringType, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);

		if(bLsdDesign) //한계상태에서는 하중조합에 있던 자중설정이 모델링으로 이동됨
		{
			nRow++;
			nCol	= 0;

			sArr.RemoveAll();
			sArr.Add(hggettext("도심위치 집중하중"));
			sArr.Add(hggettext("수직하중 재하위치 집중하중"));
			sArr.Add(hggettext("적용안함"));
			m_Grid.AddCell(nRow, nCol++, hggettext("자중 설정"));
			m_Grid.AddCell(nRow, nCol++, hggettext("자중 설정"));
			m_Grid.AddCell(nRow, nCol++, (long*)&pDataModeling->m_ixSelfWeight, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		}
	}

	if (IsCornerCloseType())
	{
		sArr.RemoveAll();
		sArr.Add("R기준 사용자 입력");
		sArr.Add(hggettext("보강철근 도심"));
		sArr.Add(hggettext("사용자 입력"));
		m_Grid.AddCell(nRow, nCol++, hggettext("외측경사부의 위치"));
		m_Grid.AddCell(nRow, nCol++, hggettext("산정방식"));
		m_Grid.AddCell(nRow, nCol, (long*)&pModel->m_nTypeDistCornerRebar, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		nRow++;
		nCol	= 0;

		m_Grid.AddCell(nRow, nCol++, hggettext("외측경사부의 위치"));
		m_Grid.AddCell(nRow, nCol++, hggettext("설정값"));
		m_Grid.AddCell(nRow, nCol, &pModel->m_dUserDistCornerRebar, nFormat,pModel->m_nTypeDistCornerRebar == 1 ? CELL_READONLY : nLine);
		nRow++;
		nCol	= 0;

		sArr.RemoveAll();
		sArr.Add("R기준 사용자 입력");
		sArr.Add(hggettext("헌치철근위치(피복)"));
		sArr.Add(hggettext("사용자 입력"));
		m_Grid.AddCell(nRow, nCol++, hggettext("내측경사부의 위치"));
		m_Grid.AddCell(nRow, nCol++, hggettext("산정방식"));
		m_Grid.AddCell(nRow, nCol, (long*)&pModel->m_nTypeDistHunchRebar, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		nRow++;
		nCol	= 0;

		m_Grid.AddCell(nRow, nCol++, hggettext("내측경사부의 위치"));
		m_Grid.AddCell(nRow, nCol++, hggettext("설정값"));
		m_Grid.AddCell(nRow, nCol, &pModel->m_dUserDistHunchRebar, nFormat,pModel->m_nTypeDistHunchRebar == 1 ? CELL_READONLY : nLine);
		nRow++;
		nCol	= 0;

		sArr.RemoveAll();
		sArr.Add(_T("45˚"));
		sArr.Add(hggettext("Hunch"));
		m_Grid.AddCell(nRow, nCol++, hggettext("내측경사부의 각도"));
		m_Grid.AddCell(nRow, nCol++, hggettext("설정값"));
		m_Grid.AddCell(nRow, nCol, (long*)&pModel->m_ixInnerAngle, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		nRow++;
		nCol	= 0;
	}
	
	if (IsCornerOpenType())
	{
		sArr.RemoveAll();
		sArr.Add(hggettext("45도 기준 모델링"));
		sArr.Add(hggettext("부재의 1/2기준 모델링"));
		m_Grid.AddCell(nRow, nCol++, hggettext("내측인장 스트럽부재"));
		m_Grid.AddCell(nRow, nCol++, hggettext("산정방식"));
		m_Grid.AddCell(nRow, nCol, (long*)&pModel->m_nTypeStirrupInnerTension, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		nRow++;
		nCol	= 0;

		m_Grid.AddCell(nRow, nCol++, hggettext("내측인장 스트럽부재"));
		m_Grid.AddCell(nRow, nCol++, hggettext("설정값"));
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

	// (ARCBRIDGE-3236) 우각부는 제외하고 모든 모델 적용기능 추가.
	if(IsChangedData() && !bCorner)
	{
		if(AfxMessageBox(_T("모든하중조합에 대해 적용하시겠습니까?"), MB_YESNO | MB_ICONQUESTION) == IDYES)
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
		// 우각부 모델의 Key값은 연속되지 않는다.
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

		// 우각부 모델의 Key값은 연속되지 않는다.
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
