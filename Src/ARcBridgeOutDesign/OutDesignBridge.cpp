// OutDesignBridge.cpp: implementation of the COutDesignBridge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../../Src/ARcBridgeCalculate/ARcBridgeCalculate.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "ARcbridgeOutDesign.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutDesignBridge::COutDesignBridge()
{
	m_nIndexTitle = -1;
	m_nRowSheet = -1;
	m_pStd = NULL;
	m_pCalc = NULL;
}

COutDesignBridge::~COutDesignBridge()
{

}


void COutDesignBridge::MakeExcel(CXLControl *pXL, long nBri)
{
	CARcBridgeDataStd *pData = m_pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar  *pBri  = pData->GetBridgeRebar(nBri);
	CProgressCtrl *pProgress = &m_pStd->m_pDlg->m_ctlProgress;

	CXLFormatText::SetExcelFileName("HgExcelBase.dll");
	CXLFormatText::SetRowPerPage(pData->m_nPageRow);

	pBri->SetConcData(TRUE, TRUE, TRUE ,TRUE, TRUE);
	m_pCalc = new CARcBridgeCalcStd(pBri);
	m_pCalc->m_pARoadOptionStd = m_pStd->m_pARoadOptionStd;
	m_pCalc->m_pView		   = m_pStd->m_pView;
	m_pCalc->CalculateAll();

	COptionItem *pOpt = m_pStd->m_pXLOption;
	CString sSheetName = _T("");
	long nSheetSu	 = pXL->GetSheetsu();
	long nSizeCalcPile = m_pCalc->m_pArrPileCalc.GetSize();
	long nSheetTotal = pOpt->GetOptionSize() + 1;	//#ARCBRIDGE-3355
	if(nSheetTotal==0) return;
	if(nSizeCalcPile >1 && pOpt->IsCheck(XL_DESIGNOUT_PILE)) nSheetTotal += nSizeCalcPile-1;
	if(pOpt->IsCheck(XL_DESIGNOUT_STMARC) || pOpt->IsCheck(XL_DESIGNOUT_STMBRACKET) || pOpt->IsCheck(XL_DESIGNOUT_STMFOOT) || pOpt->IsCheck(XL_DESIGNOUT_STMFOOTMID))
		nSheetTotal += pBri->m_mStmSolver.size();

	long i=0; for(i=0; i<nSheetTotal-nSheetSu; i++)	pXL->AddSheet();

	BOOL bLsd	= pData->IsLsdDesign();
	BOOL bBoxType = pBri->IsBoxType();
	BOOL bRcSlab    = pBri->IsRcSlab();
	BOOL bPiRahmen  = pBri->IsPiRahmen();

	long nTotPos   = m_pStd->m_pDlg->m_nPos;
	long nCurStep  = 0;
	long nTotStep  = nSheetTotal;
	long nCurSheet = 2;	//#ARCBRIDGE-3355 �ܵ� ��½� �ӵ� ������ ���� �� ��Ʈ �߰�
	CString szMsg;	

	CString sNameSheet = _T("ǥ��");
	// ������꼭 ǥ��
	if(pOpt->IsCheck(XL_DESIGNOUT_COVER))
	{
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] ǥ�� �����...", pBri->m_strBridgeName);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL, XFL_ROWSTT, XL_ROWEND_DESIGN, (long)XFL_ROWHEIGHT, (long)(XFL_TEXTSIZE*1.5), TRUE);
		pXL->SetSheetName(sNameSheet);

		CString sStandardFont	= pXL->GetStandardFont();
		double dStandardFondSize	= pXL->GetStandardFontSize();
		pXL->SetStandardFont("����");
		pXL->SetStandardFontSize(11);
		
		MakeExcelTitle(pXL,pBri);

		pXL->SetStandardFont(sStandardFont);
		pXL->SetStandardFontSize(dStandardFondSize);
	}

	// �����帧��
	if(pOpt->IsCheck(XL_DESIGNOUT_FLOW))
	{
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] �����帧�� �����...", pBri->m_strBridgeName);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL, XFL_ROWSTT, XL_ROWEND_DESIGN, (long)XFL_ROWHEIGHT, (long)(XFL_TEXTSIZE*1.5), TRUE);
		sNameSheet.Format("�����帧��");
		pXL->SetSheetName(sNameSheet);
		MakeExcelCalcFlow(pXL,pBri);
	}

	// ����
	if(pOpt->IsCheck(XL_DESIGNOUT_INDEX))
	{
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] ���� �����...", pBri->m_strBridgeName);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL, XFL_ROWSTT, XL_ROWEND_DESIGN, (long)XFL_ROWHEIGHT, (long)(XFL_TEXTSIZE*1.5), TRUE);
		sNameSheet.Format("����");
		pXL->SetSheetName(sNameSheet);
		MakeExcelCalcIndex(pXL,pBri);
	}
	
	// �ؼ���� ���
	if(pOpt->IsCheck(XL_DESIGNOUT_RESULT_SUMMARY))
	{
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] �ؼ���� ��� �����...", pBri->m_strBridgeName);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("�ؼ���� ���");
		pXL->SetSheetName(sNameSheet);
		MakeExcelCalcResultSummary(pXL,pBri);
	}

	m_nIndexTitle = 0;
	// ��������
	if(pOpt->IsCheck(XL_DESIGNOUT_CONDITION))
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. �������� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("��������");
		pXL->SetSheetName(sNameSheet);
		MakeExcelCondition(pXL,pBri);
	}

	// ������
	if(bLsd && pOpt->IsCheck(XL_DESIGNOUT_EXPOSURERATING))
	{
 		m_nIndexTitle++;
 		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
 		szMsg.Format("[ %s ] %d. ������ �����...", pBri->m_strBridgeName,m_nIndexTitle);
 		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
 		pXL->SetActiveSheet(nCurSheet++);
 		m_pStd->SetExcelFormat(pXL);
 		sNameSheet.Format("������");
 		pXL->SetSheetName(sNameSheet);
 		MakeExcelLsdExposure(pXL,pBri);
	}

	// �ܸ鰡��
	if(pOpt->IsCheck(XL_DESIGNOUT_SECTION))
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. �ܸ鰡�� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("�ܸ鰡��");
		pXL->SetSheetName(sNameSheet);
		MakeExcelSection(pXL,pBri);
	}

	// �𵨸� �ؼ�
	if(pOpt->IsCheck(XL_DESIGNOUT_MODELING))
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. �𵨸� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("�𵨸�");
		pXL->SetSheetName(sNameSheet);
		MakeExcelModeling(pXL,pBri);
	}

	// ���߻���
	if(pOpt->IsCheck(XL_DESIGNOUT_CALCLOAD))
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. ���߻��� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("���߻���");
		pXL->SetSheetName(sNameSheet);
		
		if(pData->IsLsdDesign())
			MakeExcelCalcLoad_Lsd(pXL,pBri);
		else
			MakeExcelCalcLoad(pXL,pBri);
	}

	// ��������
	if(pOpt->IsCheck(XL_DESIGNOUT_LOADDIAGRAM))
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. �������� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("��������");
		pXL->SetSheetName(sNameSheet);
		MakeExcelLoadDiagram(pXL,pBri);
	}

	// ��������
	if(pOpt->IsCheck(XL_DESIGNOUT_COMBINE))
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. �������� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("��������");
		pXL->SetSheetName(sNameSheet);

		if(bLsd)
			MakeExcelCombine_Lsd(pXL,pBri);
		else
			MakeExcelCombine(pXL,pBri);
	}

	// �ܸ�� ����
	if(pOpt->IsCheck(XL_DESIGNOUT_SUMMARY))
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. �ܸ�� ���� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("�ܸ�� ����");
		pXL->SetSheetName(sNameSheet);

		if(bLsd)
			MakeExcelForceSummarySheet_Lsd(pXL,pBri);
		else
			MakeExcelForceSummarySheet(pXL,pBri);
	}

	// �ܸ����
	if(pOpt->IsCheck(XL_DESIGNOUT_SECTION_DESIGN))
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. �ܸ���� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("�ܸ����");
		pXL->SetSheetName(sNameSheet);
		MakeExcelSectionDesign(pXL,pBri);
	}

	// ��뼺����
	if(pOpt->IsCheck(XL_DESIGNOUT_USEABLE_DESIGN))
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. ��뼺���� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("��뼺����");
		pXL->SetSheetName(sNameSheet);
		MakeExcelUseableDesign(pXL,pBri);
	}

	// ó�� �� �Ƿ� ����
	if(pOpt->IsCheck(XL_DESIGNOUT_FATIGUE))
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. ó�� �� �Ƿ� ���� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("ó�� �� �Ƿ� ����");
		pXL->SetSheetName(sNameSheet);
		MakeExcelSluggishAndFatigue(pXL,pBri);
	}

	// �찢�� �� ���ö�� ����
	if(pOpt->IsCheck(XL_DESIGNOUT_CORNER))
	{
		m_nIndexTitle++;
		CString szCheck = pBri->IsOutsideWall() ? _T("�찢�� �� ���ö��") : _T("���ö��");
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. %s ���� �����...", pBri->m_strBridgeName,m_nIndexTitle,szCheck);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("%s ����",szCheck);
		pXL->SetSheetName(sNameSheet);
		MakeExcelCornerAndSupportBar(pXL,pBri);
	}

	// ������ħ �ݷ� ����
	if((bRcSlab || bPiRahmen) && pOpt->IsCheck(XL_DESIGNOUT_REACTION))
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. ������ħ �ݷ� ���� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("������ħ �ݷ� ����");
		pXL->SetSheetName(sNameSheet);
		MakeExcelCalcShoeReaction(pXL,pBri);
	}

	// �����,�߰���ü
	long nCntInWall = (pBri->GetJijumConnectRibAndSlab() > 0)? pBri->GetCountInWallReal()-1 : pBri->GetCountInWallReal();
	if(pBri->m_nSelectInWallSectionDesign == 0)	// ������� ����ÿ��� �߰���ü�� Ÿ���� üũ�Ѵ�.
		nCntInWall = (pBri->GetJijumConnectRibAndSlab() > 0)? pBri->GetCountInWallReal(TRUE)-1 : pBri->GetCountInWallReal(TRUE);
	if(pOpt->IsCheck(XL_DESIGNOUT_GIRDER) && ((nCntInWall>0 && pBri->m_nSelectInWallSectionDesign != 0) || pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB || m_pCalc->m_pColGir->IsExistJongGirder() || (bLsd && pBri->m_nSelectWallSectionDesign == 1)))
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. ��,���(�߰���ü) ���� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("��,��� ����");
		pXL->SetSheetName(sNameSheet);
		MakeExcelColumnAndGirder(pXL,pBri);
	}
	
	// ��ġ���� ����
	if(pOpt->IsCheck(XL_DESIGNOUT_ARCHRIB) && pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. ��ġ���� ���� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("��ġ���� ����");
		pXL->SetSheetName(sNameSheet);
		MakeExcelArchRib(pXL,pBri);
	}

	// ���ʾ��� ����
	// (ARCBRIDGE-3555) BOX���� �����϶��� ��¾���.
	if(!bRcSlab && !(bBoxType && pBri->IsPileBase(0)) &&pOpt->IsCheck(XL_DESIGNOUT_FOOTING_SAFE))
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. ���ʾ��� ���� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("���ʾ��� ����");
		pXL->SetSheetName(sNameSheet);
		MakeExcelFootingSafeCheck(pXL,pBri);
	}

	// ���ʼ���
	if(!bRcSlab && !bBoxType && pOpt->IsCheck(XL_DESIGNOUT_FOOTING_DESIGN))
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. ���ʼ��� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("���ʼ���");
		pXL->SetSheetName(sNameSheet);
		MakeExcelFootingDesign(pXL,pBri);
	}

	// ������ ����
	if(!bRcSlab && !bPiRahmen && pOpt->IsCheck(XL_DESIGNOUT_WINGWALL) && pBri->IsWingWall())
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. ������ ���� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("������ ����");
		pXL->SetSheetName(sNameSheet);
		MakeExcelWingWall(pXL,pBri);
	}

	// ���ӽ����� ����
	if(!bRcSlab && !bPiRahmen && pOpt->IsCheck(XL_DESIGNOUT_APS))
	{
		if(pBri->m_apsStt[iLEFT].m_bIs || pBri->m_apsEnd[iLEFT].m_bIs)
		{
			m_nIndexTitle++;
			pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
			szMsg.Format("[ %s ] %d. ���ӽ����� ���� �����...", pBri->m_strBridgeName,m_nIndexTitle);
			m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
			pXL->SetActiveSheet(nCurSheet++);
			m_pStd->SetExcelFormat(pXL);
			sNameSheet.Format("���ӽ����� ����");
			pXL->SetSheetName(sNameSheet);
			MakeExcelApproachSlab(pXL,pBri);
		}
	}

	// �������� ����
	if(pOpt->IsCheck(XL_DESIGNOUT_EXP_JOINT) && pBri->m_pDesignExpanJoint->GetSize() > 0)
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. �������� ���� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("�������� ����");
		pXL->SetSheetName(sNameSheet);
		MakeExcelExpJoint(pXL,pBri);
	}

	// ���� ĵƿ����
 	if(pOpt->IsCheck(XL_DESIGNOUT_CANTILEVER) && pBri->m_bCantilSlab[iLEFT] )
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. ���� ĵƿ���� ���� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("���� ĵƿ������");
		pXL->SetSheetName(sNameSheet);
		MakeExcelCantilever(pXL,pBri,TRUE);
	}

	// ���� ĵƿ����
	if(pOpt->IsCheck(XL_DESIGNOUT_CANTILEVER) && pBri->m_bCantilSlab[iRIGHT] )
	{
		m_nIndexTitle++;
		pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
		szMsg.Format("[ %s ] %d. ���� ĵƿ���� ���� �����...", pBri->m_strBridgeName,m_nIndexTitle);
		m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
		pXL->SetActiveSheet(nCurSheet++);
		m_pStd->SetExcelFormat(pXL);
		sNameSheet.Format("���� ĵƿ������");
		pXL->SetSheetName(sNameSheet);
		MakeExcelCantilever(pXL,pBri,FALSE);
	}

	// ���Ұ���
	if(!bRcSlab && pOpt->IsCheck(XL_DESIGNOUT_PILE))
	{
		CString str = _T("");
		if(nSizeCalcPile > 0)
			m_nIndexTitle++;

		for(long i=0; i<nSizeCalcPile; i++)
		{
			CDesignPile *pCalcPile = m_pCalc->m_pArrPileCalc.GetAt(i);

			if(pBri->IsBoxType())
			{
				str = "BOX�����";
			}
			else
			{
				if(pCalcPile->m_nIndexJijum==0) 
					str = "������ ����";
				else if(pCalcPile->m_nIndexJijum==pBri->m_nQtyJigan) 
					str = "������ ����";
				else
					str.Format("�߰������� %d",pCalcPile->m_nIndexJijum);
			}

			pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
			szMsg.Format("[ %s ] %d. %s ���� ���� �����...", pBri->m_strBridgeName,m_nIndexTitle, str);
			m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
			pXL->SetActiveSheet(nCurSheet++);
			m_pStd->SetExcelFormat(pXL);
			sNameSheet.Format("���Ұ���(%s)", str);
			pXL->SetSheetName(sNameSheet);
			MakeExcelPile(pXL,pBri, i);
		}
	}

	for (HGINT32 ix = XL_DESIGNOUT_STMARC; ix <= XL_DESIGNOUT_STMFOOTMID; ++ix)
	{
		HGINT32 nIndexMiddle = 1;
		HGINT32 iKeyStt = 0;
		HGINT32 iKeyEnd = 0;
		GetKeySttEnd( ix, iKeyStt, iKeyEnd);

		for (HGINT32 iKey = iKeyStt; iKey <= iKeyEnd; ++iKey)
		{
			CStmModel *pModel = pBri->GetStmModelByKey(iKey);
			if (pModel && pModel->IsLock() == TRUE)
			{
				pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));

				if(pOpt->IsCheck(ix))
				{
					if(nIndexMiddle == 1)
						m_nIndexTitle++;

					pProgress->SetPos(long(nTotPos*nCurStep++/nTotStep));
					szMsg.Format("[ %s ] %d. %s ���� �����...", pBri->m_strBridgeName, m_nIndexTitle, pOpt->GetItem(ix));
					m_pStd->m_pDlg->m_sProgText.SetWindowText(szMsg);
					pXL->SetActiveSheet(nCurSheet++);
					m_pStd->SetExcelFormat(pXL);

					sNameSheet.Format(hggettext("%s(%s)"), pOpt->GetItem(ix), pBri->GetStringStmModelNameByKey(iKey));
					pXL->SetSheetName(sNameSheet);
					MakeExcelSTM(pXL, pBri, ix, iKey, nIndexMiddle++);
				}
			}

			// Index�� ���ӵ��� �ʴ´�. ��¿�� ���� �̷��� ó�� �ѵ�...
			// �׷��� ������ contine�� ��Ű�� �ȵȴ�. if�� �ɾ ����ϵ���.
			if( ix == XL_DESIGNOUT_STMARC && iKey == iKeyEnd )
				iKeyEnd = STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2;
		}	
	}

	// ���������� ��Ʈ �����
	long nDelSheeSu = nSheetTotal - (nCurSheet - 1);
	for(i = 0; i < nDelSheeSu; i++)
	{
		pXL->SetActiveSheet(nSheetTotal - i);
		pXL->SheetDelete();
	}
	if(nCurSheet > 2)
	{
		pXL->SetActiveSheet(1);
		pXL->SheetDelete();
	}	

	// ������ �� ���� => �ݵ�� �־�� ��
	CXLFormatText::DeleteAddressMap();
	delete m_pCalc;
}

void COutDesignBridge::MakeExcelTitle(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CXLFormatText::SetExcelFileName("HgExcelBase.dll");
	CXLFormatText XLText(pXL);
	
	BOOL bLSD = pData->IsLsdDesign();
	m_nRowSheet = 4;
	CString str, strTemp;
	str.Format("%s ������꼭",pBri->m_strBridgeName);
	pXL->SetMergeCell(m_nRowSheet,XFL_COLSTT,m_nRowSheet+3,XFL_COLEND);
	// �簢��
	XLText.GoPosition(m_nRowSheet+4,3);
	XLText.AddFormatText("$h$p[Rectangle]$n");
	//
	pXL->SetFonts(m_nRowSheet,XFL_COLSTT,XFL_TEXTSIZE*3,XFL_SHEET_FONT);
	pXL->SetHoriAlign(m_nRowSheet,XFL_COLSTT,m_nRowSheet+3,XFL_COLEND,TA_CENTER);
	pXL->SetXL(m_nRowSheet,XFL_COLSTT,str);	

	BOOL bArch = (pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH);
	double dTemp1 = 0;
	double dTemp2 = 0;
	
	XLText.GoPosition(m_nRowSheet+10,XFL_COLSTT);
	dTemp1 = (long)toM(pBri->m_dStationBridgeStt)/1000;
	dTemp2 = toM(pBri->m_dStationBridgeStt)-dTemp1*1000;
	str.Format("STA. %d + %.3f", (long)dTemp1, dTemp2);
	XLText.AddFormatText("$h$t$tb13�� �� �� �� ġ$m+06$c$tb13:$r$tb13 %s$n", str);

	if(bArch) str = pBri->GetStringBridgeType();
	else str.Format("%d �氣 %s", pBri->m_nQtyJigan, pBri->GetStringBridgeType());
	XLText.AddFormatText("$h$t$tb13�� �� �� �� ��$m+06$c$tb13:$r$tb13 %s$n", str);

	str.Format("L = %s %s(%s)", pData->GetStrDouble(bLSD? pBri->m_dLengthBridge : toM(pBri->m_dLengthBridge), UNIT_CGS_M), pData->m_szM, pBri->GetStringBridgeSpan());
	XLText.AddFormatText("$h$t$tb13�� �� �� �� ��$m+06$c$tb13:$r$tb13 %s$n", str);

	long nLine = 0;
	long nSpan = 0;
	CDPoint vAng = CDPoint(0,1);
	double dStationBridge = pBri->GetStationOnJijum(nSpan,nLine);
	str.Format("%s %s", pData->GetStrDouble(bLSD? pBri->GetWidthSlabAct(dStationBridge,vAng) : toM(pBri->GetWidthSlabAct(dStationBridge,vAng)), UNIT_CGS_M), pData->m_szM);
	XLText.AddFormatText("$h$t$tb13�� Ⱦ �� �� ��$m+06$c$tb13:$r$tb13 %s$n", str);

	BOOL bAll = TRUE;
	long i=0; for(i=0; i<pBri->GetCountJijum()-1; i++)
	{
		vAng	= pBri->GetAngleJijum(i);
		dTemp1	= vAng.GetAngleDegree();
		vAng	= pBri->GetAngleJijum(i+1);
		dTemp2	= vAng.GetAngleDegree();

		if(dTemp1 != dTemp2) bAll = FALSE;
	}

	if(bAll)
	{
		vAng	= pBri->GetAngleJijum(i);
		str.Format("%.3f��", vAng.GetAngleDegree());
	}
	else
	{
		str = _T("");
		long i=0; for(i=0; i<pBri->GetCountJijum(); i++)
		{
			vAng	= pBri->GetAngleJijum(i);
			if(i==0) strTemp.Format("����:%.3f��", vAng.GetAngleDegree());
			else if(i==pBri->m_nQtyJigan) strTemp.Format(", ����:%.3f��", vAng.GetAngleDegree());
			else strTemp.Format(", ����%d:%.3f��", i+1, vAng.GetAngleDegree());

			str += strTemp;
		}
	}
	XLText.AddFormatText("$h$t$tb13�� SKEW  �� ��$m+06$c$tb13:$r$tb13 %s$n", str);

	str = m_pStd->m_pOutResister->GetStringFootTypeAll(pBri);
	XLText.AddFormatText("$h$t$tb13�� �� �� �� ��$m+06$c$tb13:$r$tb13 %s$n", str);

	if(pData->m_nTypeConditionApply < DESIGN_CONDITION_RAIL)
		str.Format("%d ���", pData->m_nDegreeBridge+1);
	else
		str.Format("%d �޼�", pData->m_nDegreeRail+1);
	XLText.AddFormatText("$h$t$tb13�� �� �� �� ��$m+06$c$tb13:$r$tb13 %s$n", str);

	str.Format("%s ���", (pBri->GetDegreeEarthQuake()==0)? "I":"II");
	XLText.AddFormatText("$h$t$tb13�� �� �� �� ��$m+06$c$tb13:$r$tb13 %s$n", str);
}

void COutDesignBridge::MakeExcelCalcFlow(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);
	if(pBri->IsRcSlab())
		XLText.AddFormatText("$n$n$h$m+07$p[DESIGN_FLOW_SLAB]");
	else if(pBri->IsBoxType())
		XLText.AddFormatText("$n$n$h$m+03$p[DESIGN_FLOW_BOX]");
	else 
		XLText.AddFormatText("$n$h$m+03$p[DESIGN_FLOWCHART]");
}

void COutDesignBridge::MakeExcelCalcIndex(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	COptionItem *pOpt = m_pStd->m_pXLOption;

	HGBOOL bLsd = pData->IsLsdDesign();

	BOOL bFirstLine		= TRUE;
	BOOL bFirstPage		= TRUE;
	BOOL bIsSlab = (pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)? TRUE:FALSE;
	BOOL bIsPiRahmen = (pBri->IsPiRahmen())? TRUE:FALSE;
	long nIndexTitle	= 1;
	long nIndexSub		= 1;
	long nPageCheck		= 0;
	long nRowTot		= 46;
	long nRowPos		= 6;

	CString szDist = _T("");
	CString strLsd = _T("");
	XLText.AddFormatText("$n$h$m+08$p[TITLE_MOKCHA]$n$n$n$n");
	if(pOpt->IsCheck(XL_DESIGNOUT_CONDITION))
	{
		nPageCheck = 6;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";
		
		XLText.AddFormatText("$h%s$tb13%d.  ��  ��  ��  ��$n", szDist, nIndexTitle);
		XLText.AddFormatText("$h%s$r$a%d.1   �Ϲݻ���$n", szDist, nIndexTitle);
		XLText.AddFormatText("$h%s$r$a%d.2   ����$n", szDist, nIndexTitle);
		XLText.AddFormatText("$h%s$r$a%d.3   ������$n", szDist, nIndexTitle);
		XLText.AddFormatText("$h%s$r$a%d.4   ������$n", szDist, nIndexTitle);
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}
	if(pOpt->IsCheck(XL_DESIGNOUT_EXPOSURERATING) && bLsd )
	{
		// ������ - �Ѱ���°� �ƴϸ� ������� �ʴ´�.
		nIndexSub = 1;
		nPageCheck = 2;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		XLText.AddFormatText("$h%s$tb13%d.  ��  ��  ��  ��$n", szDist, nIndexTitle);
		//++++++++++++++++++++++++++++++++++++++++++++
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}
	if(pOpt->IsCheck(XL_DESIGNOUT_SECTION))
	{
		nPageCheck = (!bIsSlab)? 5 : 4;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";
		
		XLText.AddFormatText("$h%s$tb13%d.  ��  ��  ��  ��$n", szDist, nIndexTitle);
		XLText.AddFormatText("$h%s$r$a%d.1   ����ܸ�$n", szDist, nIndexTitle);
		XLText.AddFormatText("$h%s$r$a%d.2   ��Ÿ�ܸ�$n", szDist, nIndexTitle);
		if(!bIsSlab) XLText.AddFormatText("$h%s$r$a%d.3   ��������$n", szDist, nIndexTitle);
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}
	if(pOpt->IsCheck(XL_DESIGNOUT_MODELING))
	{
		nPageCheck = (!bIsSlab)? 5 : 4;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		XLText.AddFormatText("$h%s$tb13%d.  ��  ��  ��$n", szDist, nIndexTitle);
		XLText.AddFormatText("$h%s$r$a%d.1   ����(2D Frame)$n", szDist, nIndexTitle);
		if(!bIsSlab) 
		{
			XLText.AddFormatText("$h%s$r$a%d.2   ������(2D Frame)$n", szDist, nIndexTitle);
		}
		if(pBri->IsBoxType())
		{
			XLText.AddFormatText("$h%s$r$a%d.3   ���ݹݷ°�� ����$n", szDist, nIndexTitle);
		}
		if(pBri->m_nSelect3DShellTool != 2)
		{
			XLText.AddFormatText("$h%s$r$a%d.3   3D Plate$n", szDist, nIndexTitle);
		}
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}
	if(pOpt->IsCheck(XL_DESIGNOUT_CALCLOAD))
	{
		nIndexSub = 1;
		nPageCheck = 14;
		if(bIsSlab) nPageCheck -= 4;
		if(!pData->IsRailDesign()) nPageCheck -= 3;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";


		XLText.AddFormatText("$h%s$tb13%d.  ��  ��  ��  ��$n", szDist, nIndexTitle);
		if( bLsd )
		{
			XLText.AddFormatText("$h%s$r$a%d.%d   ��������(DC)$n", szDist, nIndexTitle, nIndexSub++);
			XLText.AddFormatText("$h%s$r$a%d.%d   ��������(DW)$n", szDist, nIndexTitle, nIndexSub++);
		}
		else
		{
			XLText.AddFormatText("$h%s$r$a%d.%d   ��������$n", szDist, nIndexTitle, nIndexSub++);
		}
		if(pData->IsRailDesign())
		{
			XLText.AddFormatText("$h%s$r$a%d.%d   Ȱ����$n", szDist, nIndexTitle, nIndexSub++);
			XLText.AddFormatText("$h%s$r$a%d.%d   ��뷹�� ������$n", szDist, nIndexTitle, nIndexSub++);
			XLText.AddFormatText("$h%s$r$a%d.%d   �õ� �� ��������$n", szDist, nIndexTitle, nIndexSub++);
		}
		else if( bLsd )
		{
			XLText.AddFormatText("$h%s$r$a%d.%d   Ȱ����(LL)$n", szDist, nIndexTitle, nIndexSub++);
			if(pBri->IsApplyTankLoad())
			{
				XLText.AddFormatText("$h%s$r$a%d.%d   ��ũ �� ������ ����$n", szDist, nIndexTitle, nIndexSub++);
			}
			XLText.AddFormatText("$h%s$r$a%d.%d   ��������(PL)$n", szDist, nIndexTitle, nIndexSub++);
			strLsd = "(EH)";
		}
		else	
		{
			XLText.AddFormatText("$h%s$r$a%d.%d   Ȱ����$n", szDist, nIndexTitle, nIndexSub++);
		}

		if(!bIsSlab) 
		{
			XLText.AddFormatText("$h%s$r$a%d.%d   ���%s$n", szDist, nIndexTitle, nIndexSub++, strLsd);
		}

		// �Ѱ���� �߰�����
		if( bLsd )
		{
			XLText.AddFormatText("$h%s$r$a%d.%d   ��������(ES)$n", szDist, nIndexTitle, nIndexSub++);
			XLText.AddFormatText("$h%s$r$a%d.%d   ����Ȱ����(LS)$n", szDist, nIndexTitle, nIndexSub++);
			XLText.AddFormatText("$h%s$r$a%d.%d   �µ���ȭ����(TU)$n", szDist, nIndexTitle, nIndexSub++);
			XLText.AddFormatText("$h%s$r$a%d.%d   �µ��� ����(TG)$n", szDist, nIndexTitle, nIndexSub++);
			strLsd = "(SH)";
		}
		else
			XLText.AddFormatText("$h%s$r$a%d.%d   �µ�����$n", szDist, nIndexTitle, nIndexSub++);
		XLText.AddFormatText("$h%s$r$a%d.%d   ��������%s$n", szDist, nIndexTitle, nIndexSub++, strLsd);
		strLsd = bLsd ? "(SD)" : "";
		if(pBri->IsBoxType() == FALSE)
		{
			XLText.AddFormatText("$h%s$r$a%d.%d   ����ħ��%s$n", szDist, nIndexTitle, nIndexSub++, strLsd);
		}
		if(!bIsSlab) 
		{
			XLText.AddFormatText("$h%s$r$a%d.%d   ������ ��������$n", szDist, nIndexTitle, nIndexSub++);
			XLText.AddFormatText("$h%s$r$a%d.%d   Ⱦ���� ��������$n", szDist, nIndexTitle, nIndexSub++);
			XLText.AddFormatText("$h%s$r$a%d.%d   ������ ���$n", szDist, nIndexTitle, nIndexSub++);
		}
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		// nRowPos += nPageCheck;
		nRowPos += nIndexSub;
	}
	if(pOpt->IsCheck(XL_DESIGNOUT_LOADDIAGRAM))
	{
		nIndexSub = 1;
		nPageCheck = (!bIsSlab)? 4 : 2;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		XLText.AddFormatText("$h%s$tb13%d.  ��  ��  ��  ��$n", szDist, nIndexTitle);
		if(!bIsSlab)
		{
			XLText.AddFormatText("$h%s$r$a%d.%d   ����$n", szDist, nIndexTitle, nIndexSub++);
			XLText.AddFormatText("$h%s$r$a%d.%d   ������$n", szDist, nIndexTitle, nIndexSub++);
		}
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}
	if(pOpt->IsCheck(XL_DESIGNOUT_COMBINE))
	{
		nIndexSub = 1;
		nPageCheck = (!bIsSlab)? 6 : 4;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		XLText.AddFormatText("$h%s$tb13%d.  ��  ��  ��  ��$n", szDist, nIndexTitle);
		if( bLsd )
		{
			if(pData->m_bPrintConditionApply)
			{
				XLText.AddFormatText("$h%s$r$a%d.%d   ��������$n", szDist, nIndexTitle, nIndexSub++);
				XLText.AddFormatText("$h%s$r$a%d.%d   ��P�� ���� ���߰��$n", szDist, nIndexTitle, nIndexSub++);
				XLText.AddFormatText("$h%s$r$a%d.%d   ��EQ�� ���� ���߰��$n", szDist, nIndexTitle, nIndexSub++);
				XLText.AddFormatText("$h%s$r$a%d.%d   ��TG�� ���� ���߰��$n", szDist, nIndexTitle, nIndexSub++);
				XLText.AddFormatText("$h%s$r$a%d.%d   ��SD�� ���� ���߰��$n", szDist, nIndexTitle, nIndexSub++);
			}
			else
			{
				XLText.AddFormatText("$h%s$r$a%d.%d   �������� : �����Ѱ����$n", szDist, nIndexTitle, nIndexSub++);
				XLText.AddFormatText("$h%s$r$a%d.%d   �������� : �شܻ�Ȳ�Ѱ����$n", szDist, nIndexTitle, nIndexSub++);
				XLText.AddFormatText("$h%s$r$a%d.%d   �������� : ����Ѱ����$n", szDist, nIndexTitle, nIndexSub++);
			}
		}
		else
		{
			XLText.AddFormatText("$h%s$r$a%d.%d   ������� ����$n", szDist, nIndexTitle, nIndexSub++);
			XLText.AddFormatText("$h%s$r$a%d.%d   ������� ����$n", szDist, nIndexTitle, nIndexSub++);
			if(!bIsSlab)
			{
				if(pBri->m_MatComboEcc.GetRow() > 0)
				{
					XLText.AddFormatText("$h%s$r$a%d.%d   ������� ����(�ִ� ���)$n", szDist, nIndexTitle, nIndexSub++);
				}
				XLText.AddFormatText("$h%s$r$a%d.%d   �������� ����$n", szDist, nIndexTitle, nIndexSub++);
			}
		}
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}
	if(pOpt->IsCheck(XL_DESIGNOUT_SUMMARY))
	{
		long nCntInWall = pBri->GetCountInWallReal();

		nIndexSub = 1;
		nPageCheck = (pBri->IsOutsideWall())? 7 : 5;
		nPageCheck += nCntInWall;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		XLText.AddFormatText("$h%s$tb13%d.  ��  ��  ��  ��  ��$n", szDist, nIndexTitle);
		if( bLsd )
		{
			XLText.AddFormatText("$h%s$r$a%d.%d   ���߼������(��) ����ǥ$n", szDist, nIndexTitle, nIndexSub++);
			XLText.AddFormatText("$h%s$r$a%d.%d   �ڴܸ���� �����$n", szDist, nIndexTitle, nIndexSub++);
			if(pBri->IsBoxType() == FALSE)
				XLText.AddFormatText("$h%s$r$a%d.%d   ���ʰ��� �����$n", szDist, nIndexTitle, nIndexSub++);

			if((pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign == 1) || (pBri->GetCountInWall() > 0 && pBri->m_nSelectInWallSectionDesign == 1))
			{
				XLText.AddFormatText("$h%s$r$a%d.%d   ��ü���� �����$n", szDist, nIndexTitle, nIndexSub++);
			}
		}
		else
		{
			XLText.AddFormatText("$h%s$r$a%d.%d   ���� �ܸ��$n", szDist, nIndexTitle, nIndexSub++);
			if(!bIsSlab) XLText.AddFormatText("$h%s$r$a%d.%d   ������ �ܸ��$n", szDist, nIndexTitle, nIndexSub++);
		
			if(pBri->IsBoxType() == FALSE)
			{
				if(pBri->IsOutsideWall())
				{
					XLText.AddFormatText("$h%s$r$a%d.%d   ������ ��ü ���ʰ���� ����$n", szDist, nIndexTitle, nIndexSub++);
					XLText.AddFormatText("$h%s$r$a%d.%d   ������ ��ü ���ʰ���� ����$n", szDist, nIndexTitle, nIndexSub++);
				}

				long i=0; for(i=0; i<nCntInWall; i++)
				{
					XLText.AddFormatText("$h%s$r$a%d.%d   �߰���ü(�߰�����%d)���ʰ���� ����$n", szDist, nIndexTitle, nIndexSub++, i+1);
				}
			}
		}
		XLText.AddFormatText("$h%s$r$a%d.%d   �ܸ�µ�$n", szDist, nIndexTitle, nIndexSub++);
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}
	if(pOpt->IsCheck(XL_DESIGNOUT_SECTION_DESIGN))
	{
		nIndexSub = 1;
		nPageCheck = 5;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		XLText.AddFormatText("$h%s$tb13%d.  ��  ��  ��  ��$n", szDist, nIndexTitle);
		XLText.AddFormatText("$h%s$r$a%d.%d   ��ö�� ���$n", szDist, nIndexTitle, nIndexSub++);
		XLText.AddFormatText("$h%s$r$a%d.%d   ����ö�� ���$n", szDist, nIndexTitle, nIndexSub++);
		XLText.AddFormatText("$h%s$r$a%d.%d   �ܸ� ����$n", szDist, nIndexTitle, nIndexSub++);
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}
	if(pOpt->IsCheck(XL_DESIGNOUT_USEABLE_DESIGN))
	{
		nIndexSub = 1;
		nPageCheck = 4;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		XLText.AddFormatText("$h%s$tb13%d.  ��  ��  ��  ��  ��$n", szDist, nIndexTitle);
		XLText.AddFormatText("$h%s$r$a%d.%d   ���ö�� ���$n", szDist, nIndexTitle, nIndexSub++);
		XLText.AddFormatText("$h%s$r$a%d.%d   �տ� ����$n", szDist, nIndexTitle, nIndexSub++);
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}
	if(pOpt->IsCheck(XL_DESIGNOUT_FATIGUE))
	{
		nIndexSub = 1;
		nPageCheck = 4;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		XLText.AddFormatText("$h%s$tb13%d. ó �� �� �� �� �� ��$n", szDist, nIndexTitle);
		XLText.AddFormatText("$h%s$r$a%d.%d   ó�� ����$n", szDist, nIndexTitle, nIndexSub++);
		XLText.AddFormatText("$h%s$r$a%d.%d   �Ƿ� ����$n", szDist, nIndexTitle, nIndexSub++);
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}
	if(pOpt->IsCheck(XL_DESIGNOUT_CORNER))
	{
		nIndexSub = 1;
		nPageCheck = (!bIsSlab)? ((pBri->IsOutsideWall())? 4:3) : 2;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		if(pBri->IsOutsideWall())
		{
			XLText.AddFormatText("$h%s$tb13%d.  �찢�� �� ���ö�� ����$n", szDist, nIndexTitle);
			XLText.AddFormatText("$h%s$r$a%d.%d   �찢�� ����$n", szDist, nIndexTitle, nIndexSub++);
			XLText.AddFormatText("$h%s$r$a%d.%d   ���ö�� ����$n", szDist, nIndexTitle, nIndexSub++);
		}
		else 
			XLText.AddFormatText("$h%s$tb13%d.  ��  ��  ö  ��  ��  ��$n", szDist, nIndexTitle);
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}
	if((bIsSlab || bIsPiRahmen) && pOpt->IsCheck(XL_DESIGNOUT_REACTION))
	{
		nIndexSub = 1;
		nPageCheck = 2;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		XLText.AddFormatText("$h%s$tb13%d.  ������ħ �ݷ»���$n", szDist, nIndexTitle);
		//++++++++++++++++++++++++++++++++++++++++++++
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}

	long nCntInWall = (pBri->GetJijumConnectRibAndSlab() > 0)? pBri->GetCountInWallReal()-1 : pBri->GetCountInWallReal();
	if(pBri->m_nSelectInWallSectionDesign == 0)	// ������� ����ÿ��� �߰���ü�� Ÿ���� üũ�Ѵ�.
		nCntInWall = (pBri->GetJijumConnectRibAndSlab() > 0)? pBri->GetCountInWallReal(TRUE)-1 : pBri->GetCountInWallReal(TRUE);
	if(pOpt->IsCheck(XL_DESIGNOUT_GIRDER) && (nCntInWall>0 || pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB || m_pCalc->m_pColGir->IsExistJongGirder()))
	{
		nIndexSub = 1;
		nPageCheck = 2;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		XLText.AddFormatText("$h%s$tb13%d.  �� , ��  ��  ��  ��$n", szDist, nIndexTitle);
		XLText.AddFormatText("$h%s$r$a%d.%d   ���,�߰���ü ����$n", szDist, nIndexTitle, nIndexSub++);
		//++++++++++++++++++++++++++++++++++++++++++++
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}

	if(pOpt->IsCheck(XL_DESIGNOUT_ARCHRIB) && pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)
	{
		nIndexSub = 1;
		nPageCheck = 2;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";
		
		XLText.AddFormatText("$h%s$tb13%d.  �� ġ �� ��  �� ��$n", szDist, nIndexTitle);
		//++++++++++++++++++++++++++++++++++++++++++++
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}

	if(pOpt->IsCheck(XL_DESIGNOUT_FOOTING_SAFE) && !bIsSlab)
	{
		long nCntInWall = pBri->IsBoxType() ? 0 : pBri->GetCountInWallReal();
		nIndexSub = 1;
		nPageCheck = (pBri->IsOutsideWall() && pBri->IsBoxType() == FALSE) ? 4 : 2;
		nPageCheck += nCntInWall;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		XLText.AddFormatText("$h%s$tb13%d.  ��  ��  ��  ��  ��  ��$n", szDist, nIndexTitle);
		if(pBri->IsBoxType() == FALSE)
		{
			if(pBri->IsOutWall(TRUE)) 
			{
				XLText.AddFormatText("$h%s$r$a%d.%d   ������ ����$n", szDist, nIndexTitle, nIndexSub++);
			}
			for(long i=0; i<nCntInWall; i++)
			{
				XLText.AddFormatText("$h%s$r$a%d.%d   �߰�������%d ����$n", szDist, nIndexTitle, nIndexSub++, i+1);
			}
			if(pBri->IsOutWall(FALSE)) 
			{
				XLText.AddFormatText("$h%s$r$a%d.%d   ������ ����$n", szDist, nIndexTitle, nIndexSub++);
			}
		}
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}
	if(pOpt->IsCheck(XL_DESIGNOUT_FOOTING_DESIGN) && !bIsSlab && pBri->IsBoxType() == FALSE)
	{
		long nCntInWall = pBri->GetCountInWallReal();
		nIndexSub = 1;
		nPageCheck = (pBri->IsOutsideWall())? 4:2;
		nPageCheck += nCntInWall;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		XLText.AddFormatText("$h%s$tb13%d.  ��  ��  ��  ��$n", szDist, nIndexTitle);
		if(pBri->IsOutWall(TRUE)) XLText.AddFormatText("$h%s$r$a%d.%d   ������ ����$n", szDist, nIndexTitle, nIndexSub++);
		long i=0; for(i=0; i<nCntInWall; i++)
		{
			XLText.AddFormatText("$h%s$r$a%d.%d   �߰�������%d ����$n", szDist, nIndexTitle, nIndexSub++, i+1);
		}
		if(pBri->IsOutWall(FALSE)) XLText.AddFormatText("$h%s$r$a%d.%d   ������ ����$n", szDist, nIndexTitle, nIndexSub++);
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}
	if(pOpt->IsCheck(XL_DESIGNOUT_WINGWALL) && !bIsSlab && !bIsPiRahmen && pBri->IsWingWall())
	{
		nIndexSub = 1;
		nPageCheck = 6;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		XLText.AddFormatText("$h%s$tb13%d.  ��  ��  ��  ��  ��$n", szDist, nIndexTitle);
		XLText.AddFormatText("$h%s$r$a%d.%d   ������ ���� ������$n", szDist, nIndexTitle, nIndexSub++);
		XLText.AddFormatText("$h%s$r$a%d.%d   ������ ���� ������$n", szDist, nIndexTitle, nIndexSub++);

		XLText.AddFormatText("$h%s$r$a%d.%d   ������ ���� ������$n", szDist, nIndexTitle, nIndexSub++);
		XLText.AddFormatText("$h%s$r$a%d.%d   ������ ���� ������$n", szDist, nIndexTitle, nIndexSub++);
		//++++++++++++++++++++++++++++++++++++++++++++
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}
	if(pOpt->IsCheck(XL_DESIGNOUT_APS) && !bIsSlab && !bIsPiRahmen)
	{
		if(pBri->m_apsStt[iLEFT].m_bIs || pBri->m_apsEnd[iLEFT].m_bIs)
		{
			nIndexSub = 1;
			nPageCheck = 6;
			if(nRowPos+nPageCheck > nRowTot)
			{
				if(!bFirstLine)
				{
					XLText.AddFormatText("$s+99");
					bFirstPage = FALSE;
				}
				else 
				{
					long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
					for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
				}
				nRowPos = (bFirstPage)? 6 : 0;
				bFirstLine = !bFirstLine;
			}
			szDist = (bFirstLine)? "$m+02" : "$m+13";

			CApsApp *pAps;

			pAps = &pBri->m_apsStt[0];
			double dLenAspStt = max(pAps->GetWidth(iLEFT), pAps->GetWidth(iRIGHT));
			double dThickStt = pAps->m_dH1 + pAps->m_dH2;

			pAps = &pBri->m_apsEnd[0];
			double dLenAspEnd = max(pAps->GetWidth(iLEFT), pAps->GetWidth(iRIGHT));
			double dThickEnd = pAps->m_dH1 + pAps->m_dH2;

			XLText.AddFormatText("$h%s$tb13%d.  �� �� �� �� �� �� ��$n", szDist, nIndexTitle);
			if(Compare(dLenAspStt, dLenAspEnd, "=", 0.001) && Compare(dThickStt, dThickEnd, "=", 0.001))
			{
				XLText.AddFormatText("$h%s$r$a%d.%d   ���� �� ���� ����$n", szDist, nIndexTitle, nIndexSub++);
				XLText.AddFormatText("$h%s$r$a%d.%d   �ܸ�� ����$n", szDist, nIndexTitle, nIndexSub++);
				XLText.AddFormatText("$h%s$r$a%d.%d   ����ö�� ����$n", szDist, nIndexTitle, nIndexSub++);
				XLText.AddFormatText("$h%s$r$a%d.%d   ��뼺 ����$n", szDist, nIndexTitle, nIndexSub++);
				
				XLText.AddFormatText("$h%s$r$a%d.%d   ��ħ�� ����$n", szDist, nIndexTitle, nIndexSub++);
			}
			else
			{
				XLText.AddFormatText("$h%s$r$a%d.%d   ���ӽ����� ����(������)$n", szDist, nIndexTitle, nIndexSub++);
				XLText.AddFormatText("$h%s$r$a%d.%d   ���ӽ����� ����(������)$n", szDist, nIndexTitle, nIndexSub++);
			}
			XLText.AddFormatText("$h$n");
			nIndexTitle++;
			nRowPos += nPageCheck;
		}
	}
	if(pOpt->IsCheck(XL_DESIGNOUT_PILE) && !bIsSlab)
	{
		long nSizeCalcPile = m_pCalc->m_pArrPileCalc.GetSize();
		nIndexSub = 1;
		nPageCheck = (nSizeCalcPile == 1) ? 2 : nSizeCalcPile + 2;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		CString str =_T("");

		long j=0; for(j=0; j<nSizeCalcPile; j++)
		{
			CDesignPile *pCalcPile = m_pCalc->m_pArrPileCalc.GetAt(j);
			
			if(pCalcPile->m_nIndexJijum==0) 
				str = "������ ����";
			else if(pCalcPile->m_nIndexJijum==pBri->m_nQtyJigan) 
				str = "������ ����";
			else
				str.Format("�߰������� %d",pCalcPile->m_nIndexJijum);
			
			if(nSizeCalcPile <2)
			{
				if(pBri->IsBoxType())
					XLText.AddFormatText("$h%s$tb13%d.  ��  ��  ��  ��$n", szDist, nIndexTitle);
				else
					XLText.AddFormatText("$h%s$tb13%d.  ��  ��  ��  ��: %s$n", szDist, nIndexTitle, str);
			}
			else if(nSizeCalcPile > 1 && j == 0)
			{
				XLText.AddFormatText("$h%s$tb13%d.  ��  ��  ��  ��$n$n", szDist, nIndexTitle);
				XLText.AddFormatText("$h%s$r$a%d.%d   %s ���Ұ���$n", szDist, nIndexTitle, nIndexSub++, str);
			}
			else if(nSizeCalcPile > 1 && j != 0)
			{
				XLText.AddFormatText("$h%s$r$a%d.%d   %s ���Ұ���$n", szDist, nIndexTitle, nIndexSub++, str);
			}
		}
//		if(nSizeCalcPile > 1) XLText.AddFormatText("$h$n");

		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}
	if(pOpt->IsCheck(XL_DESIGNOUT_SUBSIDENCE) && !bIsSlab && pBri->IsBoxType() == FALSE && 0)	// ���� ���� ������ ��...
	{
		nIndexSub = 1;
		nPageCheck = 2;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		XLText.AddFormatText("$h%s$tb13%d.  �� �� ħ �� �� ��$n", szDist, nIndexTitle);
		//++++++++++++++++++++++++++++++++++++++++++++
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
		nRowPos += nPageCheck;
	}

	if(pOpt->IsCheck(XL_DESIGNOUT_EXP_JOINT) && pBri->m_pDesignExpanJoint->GetSize() > 0)
	{
		nIndexSub = 1;
		nPageCheck = 2;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
//			nRowPos = (bFirstPage)? 6 : 0;
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		XLText.AddFormatText("$h%s$tb13%d.  �������� ����$n", szDist, nIndexTitle);
		XLText.AddFormatText("$h%s$r$a%d.%d   �������� ���� (����)$n", szDist, nIndexTitle, nIndexSub++);
		XLText.AddFormatText("$h%s$r$a%d.%d   �������� ���� (����)$n", szDist, nIndexTitle, nIndexSub++);
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
 		nRowPos += nPageCheck;
	}
	if(pOpt->IsCheck(XL_DESIGNOUT_EXP_JOINT) && (pBri->m_bCantilSlab[0] || pBri->m_bCantilSlab[1]) && bLsd )
	{
		// ĵƿ���� ���� - �Ѱ���°� �ƴϸ� ������� �ʴ´�
		nPageCheck = 2;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		XLText.AddFormatText("$h%s$tb13%d.  ĵƿ���� ����$n", szDist, nIndexTitle);
		//++++++++++++++++++++++++++++++++++++++++++++
		XLText.AddFormatText("$h$n");
		nIndexTitle++;
// 		nRowPos += nPageCheck;
	}

	for (HGINT32 ix = XL_DESIGNOUT_STMARC; ix <= XL_DESIGNOUT_STMFOOTMID; ++ix)
	{
		if(pOpt->IsCheck(ix) == FALSE)
			continue;

		nIndexSub = 1;
		nPageCheck = 2;
		if(nRowPos+nPageCheck > nRowTot)
		{
			if(!bFirstLine)
			{
				XLText.AddFormatText("$s+99");
				bFirstPage = FALSE;
			}
			else 
			{
				long nUpRow = (bFirstPage)? nRowPos-6 : nRowPos;
				for(long n=0; n<nUpRow; n++) XLText.AddFormatText("$u");
			}
			bFirstLine = !bFirstLine;
		}
		szDist = (bFirstLine)? "$m+02" : "$m+13";

		HGINT32 iKeyStt = 0;
		HGINT32 iKeyEnd = 0;

		GetKeySttEnd( ix, iKeyStt, iKeyEnd);

		for (HGINT32 iKey = iKeyStt; iKey <= iKeyEnd; ++iKey)
		{
			CStmModel *pModel = pBri->GetStmModelByKey(iKey);
			if (pModel && pModel->IsLock() == TRUE)
			{
				if(nIndexSub == 1)
				{
					XLText.AddFormatText("$h%s$tb13%d.  %s$n", szDist, nIndexTitle, pOpt->GetItem(ix));
				}
				XLText.AddFormatText("$h%s$r$a%d.%d   %s$n", szDist, nIndexTitle, nIndexSub++,  pBri->GetStringStmModelNameByKey(iKey));
			}

			// Index�� ���ӵ��� �ʴ´�. ��¿�� ���� �̷��� ó�� �ѵ�...
			// �׷��� ������ contine�� ��Ű�� �ȵȴ�. if�� �ɾ ����ϵ���.
			if( ix == XL_DESIGNOUT_STMARC && iKey == iKeyEnd )
				iKeyEnd = STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2;
		}
		if(nIndexSub > 1)
		{
			XLText.AddFormatText("$h$n");
			++nIndexTitle;
		}
	}
}

void COutDesignBridge::MakeExcelCondition(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	HGBOOL bLSD = pData->IsLsdDesign();

	long nT_1 = 1;
	long nT_2 = 2;
	long nIndexMiddle = 1;
	long nIndexSmall  = 1;
	long nTypeNumber  = 0;

	//
	BOOL bUse_LL = FALSE, bUse_Earth = FALSE, bUse_TU = FALSE, bUse_TG = FALSE, bUse_SD = FALSE, bUse_SH = FALSE;
	if(bLSD)
	{
		for (long nIdx =0; nIdx < eLSD_LOADSELECT_COUNT; nIdx++)
		{
			ELSDLoadSelectKind eKind = static_cast<ELSDLoadSelectKind>(nIdx);

			CLsdDesignLoad *pLoad = pBri->m_plsdManager->GetDesignLoad(eKind);
			if(pLoad == NULL) continue;
			if(pLoad->GetUse() == FALSE) continue;

			if(eKind == eLSD_LOADSELECT_LL1 || eKind == eLSD_LOADSELECT_LL2 || eKind == eLSD_LOADSELECT_LL3)
				bUse_LL = TRUE; //Ȱ����
			else if(eKind == eLSD_LOADSELECT_EH || eKind == eLSD_LOADSELECT_ES)
				bUse_Earth = TRUE;  //���
			else if(eKind == eLSD_LOADSELECT_TU )
				bUse_TU = TRUE;  //�µ�����
			else if(eKind == eLSD_LOADSELECT_TG)
				bUse_TG = TRUE; //�µ���
			else if(eKind == eLSD_LOADSELECT_SD)
				bUse_SD = TRUE; //����ħ��
			else if(eKind == eLSD_LOADSELECT_SH)
				bUse_SH = TRUE; //��������		
		}
	}
	else
	{
		bUse_LL = TRUE;
		bUse_Earth = TRUE;
		bUse_TU = TRUE;
		bUse_TG = TRUE;
		bUse_SD = TRUE;
		bUse_SH = TRUE;
	}
	
	
	BOOL bIsSlab = (pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)? TRUE:FALSE;	
	XLText.AddFormatText("$tb11%d. �� �� �� ��$n$n",m_nIndexTitle);				
	//////////////////////////////////////////////////////////////////////////
	CString strBridge = _T("");
	CString strSpan   = _T("");
	CString strFoot   = _T("");
	strBridge.Format("%d �氣  %s",pBri->m_nQtyJigan,pBri->GetStringBridgeType());
	strSpan.Format("L = %s %s ( %s )", pData->GetStrDouble(bLSD? pBri->m_dLengthBridge : toM(pBri->m_dLengthBridge), UNIT_CGS_M), pData->m_szM,pBri->GetStringBridgeSpan());
	if(pBri->m_nQtyJigan==1)
	{
		strBridge.Format("�ܰ氣  %s",pBri->GetStringBridgeType());
		strSpan.Format("L = %s %s",pData->GetStrDouble(bLSD? pBri->m_dLengthBridge : toM(pBri->m_dLengthBridge), UNIT_CGS_M), pData->m_szM);
	}
	if(pBri->IsBoxType() == FALSE)
	{
		BOOL bDirect = FALSE;
		BOOL bPile	= pBri->IsPileBase(-1);
		BOOL bMass	= FALSE;
		for(long i=0; i<pBri->m_nQtyJigan+1; i++)
		{
			CFootingApp *pFoot = pBri->GetFootingByJijumNumber(i);
			if(pFoot==NULL) continue;
			CExFootingApp *pExFoot	= &pFoot->m_exFooting;

			if(pExFoot->m_nType==EXFOOTING_TYPE_LEAN) bDirect = TRUE;
			if(pExFoot->m_nType==EXFOOTING_TYPE_MASS) bMass   = TRUE;
		}
		if(bDirect) strFoot += ", ���� ����";
		if(bMass)	strFoot += ", MASS ����";
		if(bPile)	strFoot += ", ���� ����";
		strFoot = strFoot.Right(strFoot.GetLength()-2);
	}
	else
	{
		strFoot = _T("���� ����");
		if(pBri->m_nTypeLower==LOWERSLAB_TYPE_MASS) strFoot = _T("MASS ����");
		if(pBri->m_nTypeLower==LOWERSLAB_TYPE_PILE) strFoot = _T("���� ����");
		if(pBri->m_nTypeLower==LOWERSLAB_TYPE_COBBSTONE) strFoot = _T("�⼮ ����");
		if(pBri->m_nTypeLower==LOWERSLAB_TYPE_ANCHOR) strFoot = _T("Anchor ����");
	}

	long nLine = 0;
	long nSpan = 0;
	CDPoint vAng = CDPoint(0,1);
	double dStationBridge = pBri->GetStationOnJijum(nSpan,nLine);
	double dWidthSlab     = bLSD? pBri->GetWidthSlabAct(dStationBridge,vAng) : toM(pBri->GetWidthSlabAct(dStationBridge,vAng));
	double Ko = 1 - sin(ToRadian(pBri->m_dAngleFrictionBackFill));

	XLText(nT_1);
	XLText.AddFormatText("$h$a%d.%d �� �� �� ��$n",m_nIndexTitle,nIndexMiddle);
	XLText += 1;
	XLText.AddFormatText("$h %d)$r�� �� �� ��$m+04$c:$r%s$n"	  ,nIndexSmall++,strBridge);
	XLText.AddFormatText("$h %d)$r�� �� �� ��$m+04$c:$r%s$n"	  ,nIndexSmall++,strSpan);
	XLText.AddFormatText("$h %d)$rȾ �� �� ��$m+04$c:$r %s %s$n",nIndexSmall++, pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), pData->m_szM);
	XLText.AddFormatText("$h %d)$rSKEW  �� ��$m+04$c:$r %.3f��$n",nIndexSmall++,pBri->GetAngleJijum(0).GetAngleDegree());
	if(!bIsSlab) XLText.AddFormatText("$h %d)$r�� �� �� ��$m+04$c:$r %s$n",nIndexSmall++,strFoot);
	XLText.AddFormatText("$h %d)$r�� �� �� ��$m+04$c:$r %d ���$n",nIndexSmall++,pData->m_nDegreeBridge+1);
	if(pBri->IsApplyEarthQuakeDesign())
	{
		if(!bIsSlab) XLText.AddFormatText("$h %d)$r�� �� �� ��$m+04$c:$r %s$n",nIndexSmall++, pBri->GetStringDegreeEarthQuake());
	}

	//////////////////////////////////////////////////////////////////////////
	double dUW_BackFill  = GetValueUnitChange(pBri->m_dUnitWeigthBackFill,UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double dUW_Ascon  = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightAsphalt),UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double dWeightReinforcedConcrete = pData->GetUnitWeightReinforcedConcrete();

	nIndexMiddle++;
	nIndexSmall  = 1;
	long nIndexTiny = 1;
	XLText(nT_1);
	XLText += 1;
	XLText.AddFormatText("$h$a%d.%d �� ��$n",m_nIndexTitle,nIndexMiddle);
	XLText += 1;
	XLText.AddFormatText("$h %d) �� �� �� ��$n",nIndexSmall++);
	XLText(nT_2);

	int nMe = bLSD==TRUE ? 2 : 1;
	int nDec = bLSD==TRUE ? 6 : 2;

	if(pData->IsSI())
	{
		XLText.AddFormatText("$h%s$rö�� ��ũ��Ʈ$m+04$c:$r$me%d$f%d$i%s$m+%02d%s$n",GetNumberString(nIndexTiny++,nTypeNumber),
			nMe, nDec, pData->GetStrDouble(dWeightReinforcedConcrete,UNIT_CGS_KGF_M3,FALSE,2),nMe+1,pData->m_szKgf_M3);
		if(bIsSlab) XLText.AddFormatText("$h%s$r��  �� $m+04$c:$r$me%d$f%d$i%s$m+%02d%s$n",GetNumberString(nIndexTiny++,nTypeNumber),nMe,nDec,
			pData->GetStrDouble(dUW_Ascon,UNIT_CGS_TONF_M3,FALSE,2),nMe+1,pData->m_szTonf_M3);
		else XLText.AddFormatText("$h%s$r��ä�� ��� $m+04$c:$r$me%d$f%d$i%s$m+%02d%s$n",GetNumberString(nIndexTiny++,nTypeNumber),nMe,nDec,
			pData->GetStrDouble(dUW_BackFill,UNIT_CGS_TONF_M3,FALSE,2),nMe+1,pData->m_szTonf_M3);
	}
	else
	{
		dWeightReinforcedConcrete = GetValueUnitChange(toTon(pData->GetUnitWeightReinforcedConcrete()), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);

		XLText.AddFormatText("$h%s$rö�� ��ũ��Ʈ$m+04$c:$r$me%d$f%d$i%s$m+%02d%s$n",GetNumberString(nIndexTiny++,nTypeNumber),
			nMe, nDec, pData->GetStrDouble(dWeightReinforcedConcrete,UNIT_CGS_TONF_M3), nMe+1, pData->m_szTonf_M3);
		if(bIsSlab) XLText.AddFormatText("$h%s$r��  �� $m+04$c:$r$me%d$f%d$i%s$m+%02d%s$n",GetNumberString(nIndexTiny++,nTypeNumber), nMe, nDec, 
			pData->GetStrDouble(dUW_Ascon,UNIT_CGS_TONF_M3),nMe+1,pData->m_szTonf_M3);
		else XLText.AddFormatText("$h%s$r��ä�� ��� $m+04$c:$r$me%d$f%d$i%s$m+%02d%s$n",GetNumberString(nIndexTiny++,nTypeNumber), nMe, nDec, 
			pData->GetStrDouble(dUW_BackFill,UNIT_CGS_TONF_M3),nMe+1, pData->m_szTonf_M3);

// 		XLText.AddFormatText("$h%s$rö�� ��ũ��Ʈ$m+04$c:$r$me1$f3$i%s$r$r%s$n",GetNumberString(nIndexTiny++,nTypeNumber),pData->GetStrDouble(dWeightReinforcedConcrete,UNIT_CGS_TONF_M3),pData->m_szTonf_M3);
// 		if(bIsSlab) XLText.AddFormatText("$h%s$r��  �� $m+04$c:$r$me1$f3$i%s$r$r%s$n",GetNumberString(nIndexTiny++,nTypeNumber),pData->GetStrDouble(dUW_Ascon,UNIT_CGS_TONF_M3),pData->m_szTonf_M3);
// 		else XLText.AddFormatText("$h%s$r��ä�� ��� $m+04$c:$r$me1$f3$i%s$r$r%s$n",GetNumberString(nIndexTiny++,nTypeNumber),pData->GetStrDouble(dUW_BackFill,UNIT_CGS_TONF_M3),pData->m_szTonf_M3);
	}
	if(pData->IsRailDesign())
	{
		double dUW_Mortar   = GetValueUnitChange(2300.0,UNIT_CGS_KGF_M3,pData->m_szKgf_M3);
		double dUW_RailBeam = GetValueUnitChange( 300.0,UNIT_CGS_KGF_M3,pData->m_szKgf_M3);
		double dUW_Dosang   = GetValueUnitChange(1900.0,UNIT_CGS_KGF_M3,pData->m_szKgf_M3);
		XLText.AddFormatText("$h%s$r�� Ż $m+04$c:$r$me%d$f%d$i%s$m+%02d%s$n",GetNumberString(nIndexTiny++,nTypeNumber),nMe,nDec,pData->GetStrDouble(dUW_Mortar,UNIT_CGS_KGF_M3,FALSE,2),nMe+1,pData->m_szKgf_M3);
		XLText.AddFormatText("$h%s$r�� �� $m+04$c:$r$me%d$f%d$i%s$m+%02d%s$n",GetNumberString(nIndexTiny++,nTypeNumber),nMe,nDec,pData->GetStrDouble(dUW_RailBeam,UNIT_CGS_KGF_M3,FALSE,2),nMe+1,pData->m_szKgf_M3);
		XLText.AddFormatText("$h%s$r�� �� $m+04$c:$r$me%d$f%d$i%s$m+%02d%s$n",GetNumberString(nIndexTiny++,nTypeNumber),nMe,nDec,pData->GetStrDouble(dUW_Dosang,UNIT_CGS_KGF_M3,FALSE,2),nMe+1,pData->m_szKgf_M3);
	}
	if(pBri->IsBoxType() && pBri->m_bApplyGroundWaterLevel)
	{
		double dUW_BackFillWater = GetValueUnitChange(pBri->m_dUnitWeigthBackFillWater, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
		XLText.AddFormatText("$h%s$r��ä�����(����)$m+04$c:$r$me%d$f%d$i%s$m+%02d%s$n",GetNumberString(nIndexTiny++,nTypeNumber),nMe, nDec,
			pData->GetStrDouble(dUW_BackFillWater,UNIT_CGS_TONF_M3,FALSE,2),nMe+1,pData->m_szTonf_M3);
		XLText.AddFormatText("$h%s$r���ϼ�  $m+04$c:$r$me%d$f%d$i%s$m+%02d%s$n",GetNumberString(nIndexTiny++,nTypeNumber),nMe,nDec,
			pData->GetStrDouble(dUW_BackFillWater,UNIT_CGS_TONF_M3,FALSE,2),nMe+1,pData->m_szTonf_M3);
	}
	nIndexTiny = 1;
	XLText += 1;

	if(bUse_LL)
	{
		XLText(nT_1);
		XLText.AddFormatText("$h %d) Ȱ �� ��$n",nIndexSmall++);
		XLText(nT_2);
		if(pData->IsRailDesign())
		{
			XLText.AddFormatText("$h%s$r�� �� �� ��$m+04$c:$r %s$n",GetNumberString(nIndexTiny++,nTypeNumber),pData->GetStringRailLoad(pData->m_nTypeRailLoad));
		}
		else if(bIsSlab)
		{
			CString strDB = pData->GetStringLoadDB();
			CString strDL = pData->GetStringLoadDB(TRUE);

			XLText.AddFormatText("$h%s$r�� �� �� ��$m+04$c:$r %s$n",GetNumberString(nIndexTiny++,nTypeNumber),strDB);
			XLText.AddFormatText("$h%s$r�� �� �� ��$m+04$c:$r %s$n",GetNumberString(nIndexTiny++,nTypeNumber),strDL);
			XLText.AddFormatText("$h%s$r�� �� �� ��$m+04$c:$r %s$n",GetNumberString(nIndexTiny++,nTypeNumber),"i=15/(40+L) �� 0.3");
		}
		else
		{
			double dLiveAlwyas = pData->GetValueUnitType(pBri->m_dLiveLoadAlways,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
			CString strDB = pData->GetStringLoadDB();
			CString strTitle = bLSD==TRUE ? "������ ����" : "�� �� �� ��";
			CString strContext = bLSD==TRUE ? (pData->m_nLSDRoadType==0 ? "�Ϲݵ���" : "��ӵ���") : (pData->GetStrDouble(dLiveAlwyas,UNIT_CGS_TONF_M2) + "$m+02" + pData->m_szTonf_M2);

			XLText.AddFormatText("$h%s$r%d �� �� ��$m+04$c:$r %s$n",GetNumberString(nIndexTiny++,nTypeNumber),pData->m_nDegreeBridge+1,strDB);
			XLText.AddFormatText("$h%s$r%s$m+04$c:$m+%02d$me1$i$f2%s$n",GetNumberString(nIndexTiny++,nTypeNumber), strTitle, nMe, strContext);
		}
		nIndexTiny  = 1;
		XLText += 1;
	}
	
	if(!bIsSlab && bUse_Earth)
	{
		XLText(nT_1);
		XLText.AddFormatText("$h %d) �� ��$n",nIndexSmall++);
		XLText(nT_2);
		XLText.AddFormatText("$h%s$r���� ���θ�����$m+04$c:$r �� = %.2f��$n",GetNumberString(nIndexTiny++,nTypeNumber),pBri->m_dAngleFrictionBackFill);
		XLText.AddFormatText("$h%s$r������а��$m+04$c:$r Ko = 1 - Sin�� = %.3f$n",GetNumberString(nIndexTiny++,nTypeNumber),Ko);
		//
		double dCoDanger = (pBri->GetDegreeEarthQuake() == 0) ? 1.40 : 1.00;
		double dCoEarthq = (pBri->GetRegionEarthQuake() == 0) ? 0.11 : 0.070;

		if(pBri->IsApplyEarthQuakeDesign())
		{
			nIndexTiny  = 1;
			XLText += 1;
			XLText(nT_1);
			XLText.AddFormatText("$h %d) �� �� �� ��$n",nIndexSmall++);
			XLText(nT_2);
			XLText.AddFormatText("$h%s$r�������$m+06$c:$r %s$n",GetNumberString(nIndexTiny++,nTypeNumber),pBri->GetStringDegreeEarthQuake());
			XLText.AddFormatText("$h%s$r��������$m+06$c:$r %d ����$n",GetNumberString(nIndexTiny++,nTypeNumber),pBri->GetRegionEarthQuake()+1);
			if(pData->m_nTypeEarthQuakeApply == 0)
				XLText.AddFormatText("$h%s$r���ݰ��$m+06$c:$r S = %.2f$n",GetNumberString(nIndexTiny++,nTypeNumber),pBri->GetCoefficientGround());
			else if(pBri->IsBoxType() == FALSE)
				XLText.AddFormatText("$h%s$r��������$m+06$c:$r %s (%s)$n", GetNumberString(nIndexTiny++,nTypeNumber), pData->GetStringTypeCoefficientGround(pBri->GetTypeCoefficientGround()), pBri->GetTypeCoefficientGround() == 0 ? _T("�Ϲ�����") : _T("�������"));

			if(pData->m_nTypeEarthQuakeApply != 0)
			{
				BOOL bFirst(TRUE);
				for(long ix=0; ix<eGoalEarthQuakeCount; ++ix)
				{
					EGoalOfEarthQuake eGoalEq = static_cast<EGoalOfEarthQuake>(ix);
					if(pBri->IsBoxType())
					{
						if(pBri->m_bGoalEarthQuake[eGoalEq] == FALSE) continue;
					}
					else
					{
						if(ix > 0) break;
					}

					dCoDanger = pBri->GetFactorCoDanger(eGoalEq);

					CString szEqCase(_T(""));
					if(pBri->IsBoxType())
					{
						szEqCase.Format(_T("  (%s)"), pBri->GetStringGoalEarthQuake(eGoalEq, TRUE));
					}
					
					if(bFirst)
					{
						XLText.AddFormatText("$h%s$r%s$m+06$c:$r %s = %.3f x %.3f = %.3f%s$n",GetNumberString(nIndexTiny++,nTypeNumber), hggettext("��ȿ�������ݰ��ӵ�"), hggettext("S"),dCoDanger,dCoEarthq, pBri->GetCoefficientAccelerateEQ(eGoalEq), szEqCase);
						bFirst = FALSE;
					}
					else
					{
						XLText.AddFormatText("$h$m+08 %s = %.3f x %.3f = %.3f%s$n", hggettext("S"),dCoDanger,dCoEarthq, pBri->GetCoefficientAccelerateEQ(eGoalEq), szEqCase);
					}
				}
			}
			else
				XLText.AddFormatText("$h%s$r%s$m+06$c:$r %s = %.3f x %.3f = %.3f$n",GetNumberString(nIndexTiny++,nTypeNumber), hggettext("���ӵ����"), hggettext("A"), dCoDanger,dCoEarthq, pBri->GetCoefficientAccelerateEQ());

			XLText.AddFormatText("$h%s$r������ ��а��$m+06$c:$r Mononobe-Okabe ����$n",GetNumberString(nIndexTiny++,nTypeNumber));
		}
	}

	//
	double dLoadTempertureAxial  = (pBri->m_dTS>=700) ? 10 : 15;

	CString sLoadTempertureUpDown[2];
	sLoadTempertureUpDown[0] = bLSD ? (m_pCalc->m_pLoad->m_dLoadTgValueT[0] > 0 ? "+" : "-") : "";
	sLoadTempertureUpDown[1] = bLSD ? (m_pCalc->m_pLoad->m_dLoadTgValueT[1] > 0 ? "+" : "-") : "";

	long nMoveCol = bLSD ? 8 : 7;

	if(bUse_TU || bUse_TG)
	{
		nIndexTiny  = 1;
		XLText += 1;
		XLText(nT_1);
		XLText.AddFormatText("$h %d) �� �� �� ��$n",nIndexSmall++);
		XLText(nT_2);
		if(bUse_TU)
			XLText.AddFormatText("$h%s$r����â��� (��)$m+0%d$c:$r%.3f x$r$r$p[Object_10_5]$n",GetNumberString(nIndexTiny++,nTypeNumber), nMoveCol, pBri->GetCoefficientExpansion()*pow(10.0,5.0));
		if( bLSD)
		{
			if(bUse_TG)
				XLText.AddFormatText("$h%s$r�ٴ��� ���ϸ� �µ���%s$m+0%d$c:$r %s%.3f ��, %s%.3f��$n",GetNumberString(nIndexTiny++,nTypeNumber), bLSD ? " (TG)" : "", nMoveCol, sLoadTempertureUpDown[0], abs(m_pCalc->m_pLoad->m_dLoadTgValueT[0]), 
					sLoadTempertureUpDown[1], abs(m_pCalc->m_pLoad->m_dLoadTgValueT[1]));
		}
		else
		{
			double dLoadTempertureUpDown = 5;
			XLText.AddFormatText("$h%s$r�ٴ��� ���ϸ� �µ���$m+0%d$c:$r %.0f ��$n",GetNumberString(nIndexTiny++,nTypeNumber), nMoveCol, dLoadTempertureUpDown);
		}
		XLText.AddFormatText("$h%s$r��ũ��Ʈ���� �µ���ȭ ����%s$m+0%d$c:$r %.0f �� (�ܸ��ּ�ġ�� : %.0f mm)$n",GetNumberString(nIndexTiny++,nTypeNumber), bLSD ? " (TU)" : "", nMoveCol,
			bLSD==TRUE ? pBri->m_dTemperatureChangeLoadGap : dLoadTempertureAxial,pBri->m_dTS);
	}

	//////////////////////////////////////////////////////////////////////////
// 	XLText.AddFormatText("$h%s$r����â��� (��)$m+08$c:$r%.3f x$r$r$p[Object_10_5]$n",GetNumberString(nIndexTiny++,nTypeNumber),pBri->GetCoefficientExpansion()*pow(10.0,5.0));
// 	XLText.AddFormatText("$h%s$r�ٴ��� ���ϸ� �µ���%s$m+08$c:$r %s%.3f ��, %s%.3f��$n",GetNumberString(nIndexTiny++,nTypeNumber), bLSD ? " (TG)" : "", sLoadTempertureUpDown[0], abs(m_pCalc->m_pLoad->m_dLoadTgValueT[0]), 
// 		sLoadTempertureUpDown[1], abs(m_pCalc->m_pLoad->m_dLoadTgValueT[1]));
// 
// 	XLText.AddFormatText("$h%s$r��ũ��Ʈ���� �µ���ȭ ����%s$m+08$c:$r %.0f �� (�ܸ��ּ�ġ�� : %.0f mm)$n",GetNumberString(nIndexTiny++,nTypeNumber), bLSD ? " (TU)" : "",
// 		bLSD==TRUE ? pBri->m_dTemperatureChangeLoadGap : dLoadTempertureAxial,pBri->m_dTS);
	//////////////////////////////////////////////////////////////////////////

	if(pBri->IsBoxType() == FALSE)
	{
		if(bUse_SD)
		{
			CString str = bLSD==TRUE ? _T("") : _T("��ũ��Ʈ���̹Ƿ� ");
			double dLoadSubsidence = 10; //mm
			nIndexTiny  = 1;
			XLText += 1;
			XLText(nT_1);
			XLText.AddFormatText("$h %d) �� �� ħ ��$n",nIndexSmall++);
			XLText(nT_2);
			XLText.AddFormatText("$h" + str + "�� = %.2f mm�� ����Ѵ�.$n", bLSD==TRUE ? pBri->m_dDepthApplySubSid : dLoadSubsidence);
		}		
	}
	//
	nIndexTiny  = 1;
	XLText += 1;
	if(!bIsSlab)
	{
		if(bUse_SH)
		{
			XLText(nT_1);
			XLText.AddFormatText("$h %d) �� �� �� ��$n",nIndexSmall++);
			XLText(nT_2);
			XLText.AddFormatText("$h%s$r��������� (��)$m+06$c:$r%.2f x$r$r$p[Object_10_5]$n",GetNumberString(nIndexTiny++,nTypeNumber),pBri->GetRateDryAndShrinkage()*pow(10.0,5.0));
			XLText.AddFormatText("$h%s$r����â��� (��)$m+06$c:$r%.3f x$r$r$p[Object_10_5]$n",GetNumberString(nIndexTiny++,nTypeNumber),pBri->GetCoefficientExpansion()*pow(10.0,5.0));
		}		
	}

	nIndexMiddle++;
	nIndexSmall  = 1;
	nIndexTiny = 1;
	XLText(nT_1);
	XLText += 1;
	XLText.AddFormatText("$s+10$h$a%d.%d �� �� �� ��$n",m_nIndexTitle,nIndexMiddle);
	XLText += 1;

	if(bLSD == TRUE)
		MakeExcelConditionUseMaterialLsd(&XLText, pBri);
	else
		MakeExcelConditionUseMaterial(&XLText, nIndexSmall, nIndexTiny, nTypeNumber, nT_1, nT_2);
	//////////////////////////////////////////////////////////////////////////
	nIndexMiddle++;
	nIndexSmall  = 1;
	XLText(nT_1);
	XLText += 1;
	XLText.AddFormatText("$h$a%d.%d �� �� �� ��$n",m_nIndexTitle,nIndexMiddle);
	XLText(nT_2);
	XLText += 1;
	if( bLSD )
	{
		if(pData->IsDesignKDS2019())
			XLText.AddFormatText(_T("$h %d)$r%s$m+11$c:$r%s$n")	  ,nIndexSmall++,hggettext("���� ���� �Ϲݻ���(�Ѱ���¼����)"),hggettext("KDS 24 10 11 : 2019"));
		else
			XLText.AddFormatText(_T("$h %d)$r%s$m+11$c:$r%s$n")	  ,nIndexSmall++,hggettext("���α� �������(�Ѱ���¼����)"),hggettext("�����ؾ�� (2012)"));
		XLText.AddFormatText(_T("$h %d)$r%s$m+11$c:$r%s$n")	  ,nIndexSmall++,hggettext("���α� ���迹����(�Ѱ���¼����)"),hggettext("�ѱ����α�����ȸ (2014)"));
		XLText.AddFormatText(_T("$h %d)$r%s$m+11$c:$r%s$n")	  ,nIndexSmall++,hggettext("��ӵ��α��� ���迹����(�Ѱ���¼����)"),hggettext("�ѱ����ΰ��� (2012)"));
		XLText.AddFormatText(_T("$h %d)$r%s$m+11$c:$r%s$n")	  ,nIndexSmall++,hggettext("ASSHTO LRFD Bridge Design Specifications"),hggettext("AASHTO (2007)"));
		XLText.AddFormatText(_T("$h %d)$r%s$m+11$c:$r%s$n")	  ,nIndexSmall++,hggettext("Eurocode 2 : Design of concrete structures"),hggettext("BSI (2004)"));
		if(pBri->IsApplyEarthQuakeDesign() && pData->m_nTypeEarthQuakeApply == 1)
		{
			if(pData->IsDesignKDS2019())
				XLText.AddFormatText(_T("$h %d)$r%s$m+11$c:$r%s$n")	  ,nIndexSmall++,hggettext("�������� �Ϲ�"),hggettext("KDS 17 10 00 : 2018"));
			else
				XLText.AddFormatText(_T("$h %d)$r%s$m+11$c:$r%s$n")	  ,nIndexSmall++,hggettext("�������� �Ϲ�(KDS 17 10 00)"),hggettext("����������� (2018)"));
		}
	}
	else
	{
		if(pData->IsRailDesign())
		{
			if(pData->IsRailDesignKDS2018())
			{
				XLText.AddFormatText("$h%d)$rö���������                 - KDS 47 00 00 : 2019$n",nIndexSmall++);
			}
			else if(pData->IsRailDesign2017())
			{
				XLText.AddFormatText("$h%d)$rö���������(�����)         - ���䱳���(2017��)$n",nIndexSmall++);
			}
			else
			{
				if(m_pStd->m_pARcBridgeDataStd->IsSI())
				{
					XLText.AddFormatText("$h%d)$rö���� �������(ö������)    - ���������ȸ(2004��)$n",nIndexSmall++);
					XLText.AddFormatText("$h%d)$rö���� �������(�����)      - ���������ȸ(2004��)$n",nIndexSmall++);
				}
				else
				{
					XLText.AddFormatText("$h%d)$rö���� �������(ö������)    - ö��û(1999��)$n",nIndexSmall++);
					XLText.AddFormatText("$h%d)$rö���� �������(�����)      - ö��û(1999��)$n",nIndexSmall++);
				}
			}			
		}
		if(m_pStd->m_pARcBridgeDataStd->IsSI())
		{
			CString strDoro = pData->IsDoro2010() ? "(2010��)" : "(2005��)";
			XLText.AddFormatText("$h%d)$r���α� �������              - �Ǽ������%s$n",nIndexSmall++, strDoro);
			if (pData->m_nTypeKciCode == 2)
			{
				XLText.AddFormatText("$h%d)$r��ũ��Ʈ��������             - �Ǽ������(%d��)$n",nIndexSmall++, 2012);
			}
			else
			{
				XLText.AddFormatText("$h%d)$r��ũ��Ʈ�����������         - �Ǽ������(%d��)$n",nIndexSmall++, (pData->m_nTypeKciCode == 1)? 2007 : 2004);
			}

		}
		else
		{
			XLText.AddFormatText("$h%d)$r���α� �������              - �Ǽ������(2003��)$n",nIndexSmall++);
			if (pData->m_nTypeKciCode == 2)
			{
				XLText.AddFormatText("$h%d)$r��ũ��Ʈ��������             - �Ǽ������(%d��)$n",nIndexSmall++, 2012);
			}
			else
			{
				XLText.AddFormatText("$h%d)$r��ũ��Ʈ�����������         - �Ǽ������(%d��)$n",nIndexSmall++, (pData->m_nTypeKciCode == 1)? 2007 : 2004);
			}
		}
		XLText.AddFormatText("$h%d)$r���μ����� ��3�� ������    - �ѱ����ΰ���(2001��)$n",nIndexSmall++);

		if(pBri->IsApplyEarthQuakeDesign())
		{
			if(!bIsSlab) 
			{
				XLText.AddFormatText("$h%d)$r��ӵ��� ������ ����������� - �ѱ����ΰ���(2000��)$n",nIndexSmall++);
			}
			if(pData->m_nTypeEarthQuakeApply == 1)
			{
				XLText.AddFormatText("$h%d)$r�������� �Ϲ�(KDS 17 10 00)  - ����������� (2018)$n",nIndexSmall++);
			}
		}
	}
}

void COutDesignBridge::MakeExcelLsdExposure(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	HGBOOL bLsd = pData->IsLsdDesign();
	if( bLsd == FALSE ) return;

	XLText.AddFormatText("$tb11%d. �� �� �� ��$n$n",m_nIndexTitle);				

	XLText.AddText("$s+45");

	SExposureOutputParams stOutputParams; // ���� �����Ѵ�.
	pBri->SetExposureParameter(stOutputParams);

	/////// �� ��////////////////
	CLsdEnviromentOutCalc EnviromentOutCalc(pBri->m_plsdEnvRating, TRUE);
	EnviromentOutCalc.SetLsdManager(pBri->m_plsdManager);
	EnviromentOutCalc.MakeOutputExcelAll(&XLText, stOutputParams,1,0);
}

void COutDesignBridge::MakeExcelSection(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nIndexMiddle = 1;
	BOOL bIsSlab = (pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)? TRUE:FALSE;

	XLText.AddFormatText("$tb13%d. �� �� �� ��$n$n",m_nIndexTitle);				
	//////////////////////////////////////////////////////////////////////////

	XLText.GoPosition(NO_MOVE,COL_POS_1);
	XLText.AddFormatText("$s+07$h$a%d.%d �� �� �� ��$n", m_nIndexTitle, nIndexMiddle++);
	
	double dScaleDim = 150;
	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(dScaleDim);
	pDom->SetLTScale(dScaleDim*5);

	//long nOldTypeJongDanRebar	= pBri->m_nTypeJongdanForRebar;
	//pBri->m_nTypeJongdanForRebar	= 2;
	//pBri->SyncBridge(0, TRUE, TRUE, FALSE);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2);
	pBri->SyncBridgeAps(0, FALSE);
	
	long nSizeRow = 40;
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, pData);
	DBStd.m_pDrawVertSection->DrawVertSectionAll(pDom, TRUE, TRUE,TRUE, FALSE, TRUE);
	m_pCalc->SetDomyunScale(pDom, nSizeRow);
	DBStd.m_pDrawCrossSection->DimLongi(pDom);
	pDom->Rotate(CDPoint(0,0),CDPoint(0,-1));

	XLText.InsertDomyun(pDom,m_pStd->m_pView,nSizeRow);

	// ��Ÿ ������ �ܸ�
	m_pCalc->m_pLoad->WriteSectionDeadLoad(&XLText,pBri,m_nIndexTitle,nIndexMiddle++);

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
	//pBri->m_nTypeJongdanForRebar	= nOldTypeJongDanRebar;

	// ������������
	if(!bIsSlab)
	{
		XLText.GoPosition(NO_MOVE,COL_POS_1);
		XLText.AddFormatText("$s+30$h$a%d.%d �� �� �� ��$n$n", m_nIndexTitle, nIndexMiddle++);
		MakeTableBoringData(&XLText,pBri);
	}
}

void COutDesignBridge::MakeExcelCalcLoad(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nT_0 = 0;
	long nIndexMiddle = 1;

	BOOL bRcSlab   = (pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) ? TRUE : FALSE;
	BOOL bPiRahmen = (pBri->IsPiRahmen())   ? TRUE : FALSE;
	
	XLText(nT_0);
	XLText.AddFormatText("$tb13%d. �� �� �� ��$n",m_nIndexTitle);
	m_pCalc->m_pLoad->WriteCalcDeadLoad(&XLText,pBri,FALSE,m_nIndexTitle,nIndexMiddle++); 
	
	if(pData->IsRailDesign())
	{
		if(pBri->m_bUnderGroundRahmen  && !pBri->m_bCalcOpen)
			m_pCalc->m_pLoad->WriteCalcLiveLoadUnderGround(&XLText,pBri,m_nIndexTitle,nIndexMiddle++); 
		else
			m_pCalc->m_pLoad->WriteCalcRailLoadLive(&XLText,pBri,m_nIndexTitle,nIndexMiddle++); 
		m_pCalc->m_pLoad->WriteCalcRailLoadLR(&XLText,pBri,m_nIndexTitle,nIndexMiddle++);   
		m_pCalc->m_pLoad->WriteCalcRailLoadSB(&XLText,pBri,m_nIndexTitle,nIndexMiddle++);	  
	}
	else
	{
		if(pBri->m_bUnderGroundRahmen && !pBri->m_bCalcOpen)
			m_pCalc->m_pLoad->WriteCalcLiveLoadUnderGround(&XLText,pBri,m_nIndexTitle,nIndexMiddle++); 
		else
			m_pCalc->m_pLoad->WriteCalcLiveLoad(&XLText,pBri,m_nIndexTitle,nIndexMiddle++); 
		if(pBri->m_bApplyLoad[BRIDGE_LOAD_TANK]) m_pCalc->m_pLoad->WriteCalcLiveLoadTank(&XLText,pBri,m_nIndexTitle,nIndexMiddle++); 
	}
	if(!bRcSlab && !bPiRahmen && pBri->m_bApplyLoad[BRIDGE_LOAD_TOAP_BOTH])
	{
		m_pCalc->m_pLoad->WriteCalcSoilPressure(&XLText,pBri,m_nIndexTitle,nIndexMiddle++, FALSE, pBri->GetEarthQuakeGoalByIndex(0)); 
	}

	// ARCBRIDGE-3429
	if( pBri->IsBoxType() && ((pData->IsLsdDesign()==FALSE && pBri->m_dHeightInteriorWaterPress>0) || (pBri->m_bApplyGroundWaterLevel && pBri->m_bApplyLoad[BRIDGE_LOAD_WATERHORI])) )
	{
		m_pCalc->m_pLoad->WriteCalcWaterPressure(&XLText,pBri,m_nIndexTitle,nIndexMiddle++); 
	}
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_TEMP1])
	{
		m_pCalc->m_pLoad->WriteCalcTemperature(&XLText,pBri,m_nIndexTitle,nIndexMiddle++); 
	}
	if(!bRcSlab && pBri->m_bApplyLoad[BRIDGE_LOAD_SHRINK]) 
	{
		m_pCalc->m_pLoad->WriteCalcDryAndShrinkage(&XLText,pBri,m_nIndexTitle,nIndexMiddle++); 
	}
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_SUBSID])
	{
		m_pCalc->m_pLoad->WriteCalcSubsidence(&XLText,pBri,m_nIndexTitle,nIndexMiddle++); 
	}
	if(pBri->IsBoxType() && pBri->m_bApplyAnchorForce)
	{
		XLText.GoPosition(NO_MOVE,COL_POS_1);
		XLText.AddFormatText("$s+07$n$h$a%d.%d ��Ŀ�� ���� �ۿ��$n",m_nIndexTitle,nIndexMiddle++);
		XLText.GoPosition(NO_MOVE,COL_POS_2);
		XLText.AddFormatText("$h�ۿ�� = ��Ŀ�� x ��Ŀ���� / ��������$n");
		double dWidth = m_pCalc->GetSlabWidth();
		for(long ix = 0; ix < pBri->m_nCountAnchor; ix++)
		{
			double dLoadAnchor = 0, dLoad = 0, dEa = 0;

			dLoadAnchor = GetValueUnitChange(pBri->m_dArrLoadAnchor.GetAt(ix),UNIT_CGS_TONF,pData->m_szTonf);
			dEa = pBri->m_dArrEaAnchor.GetAt(ix);
			dLoad = dLoadAnchor * dEa / dWidth;
			XLText.AddFormatText("$h'- P%d  =  %7.3f x %.0f EA / %.3f   =  %8.3f %s$n",ix+1, dLoadAnchor, dEa, dWidth, dLoad, pData->m_szTonf_M);
		}
		XLText += 1;
	}
	// ��������
	WriteExcelEarthQuakeLoad(&XLText, pBri, nIndexMiddle);
}

void COutDesignBridge::MakeExcelCalcLoad_Lsd( CXLControl *pXL, CRcBridgeRebar *pBri )
{
	CXLFormatText XLText(pXL);

	long nT_0 = 0;
	long nIndexMiddle = 1;

	BOOL bRcSlab   = (pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) ? TRUE : FALSE;
	BOOL bPiRahmen = (pBri->IsPiRahmen())   ? TRUE : FALSE;
	BOOL bBoxType = pBri->IsBoxType();

	XLText(nT_0);
	XLText.AddFormatText("$tb13%d. �� �� �� ��$n",m_nIndexTitle);

	// �������� DC
	// 	if(bDW && m_DeadLoadDistribution[1] == 0) : DW���� FALSE�� �Ѱ��ְ� �����Ƿ� ������ �߰����� �ʴ´�.
	if((pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_DC)))
	{
		m_pCalc->m_pLoad->WriteCalcDeadLoad(&XLText,pBri,FALSE,m_nIndexTitle,nIndexMiddle++); 
	}
	
	// �������� DW
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_DW) && m_pCalc->m_pLoad->m_DeadLoadDistribution[1] != 0)
	{
		m_pCalc->m_pLoad->WriteCalcDeadLoad(&XLText,pBri,TRUE,m_nIndexTitle,nIndexMiddle++);
	}
	
	// Ȱ���� LL
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_LL1))
	{
		if ((pBri->m_bUnderGroundRahmen==FALSE || pBri->m_bCalcOpen == TRUE))
		{
			m_pCalc->m_pLoad->WriteCalcLiveLoadLsd(&XLText,pBri,m_nIndexTitle,nIndexMiddle++, FALSE); // �̵�����

			// (ARCBRIDGE-3232) �Ƿ� ���� ���� ���� -> �Ƿ�Ȱ������ �״�� ����. ������ �����Ƿ� ������ ���
			// if(pBri->m_plsdManager->IsUsedComboKind(eLSD_COMBINE_STRESS))
			{
				m_pCalc->m_pLoad->WriteCalcLiveLoadLsd(&XLText,pBri,m_nIndexTitle,nIndexMiddle++, TRUE); // �̵����� �Ƿ��Ѱ���� ���տ�
			}

			long nSeleOpt = pBri->m_plsdManager->GetDesignLoad(eLSD_LOADSELECT_LL1)->GetIndexSelectOfType(eLSD_LOADCASE_DIR_BRI);
			if(nSeleOpt != 0)
			{
				m_pCalc->m_pLoad->WriteCalcLiveLoadTank(&XLText,pBri,m_nIndexTitle,nIndexMiddle++); 
			}
		}
		
		if (pBri->m_bUnderGroundRahmen && pBri->m_bCalcOpen == FALSE)
		{
			m_pCalc->m_pLoad->WriteCalcLiveLoadUnderGround(&XLText, pBri, m_nIndexTitle, nIndexMiddle++); // ��������
		}
	}
	// �������� PL
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_PL) && 0 < m_pCalc->m_pLoad->m_dLiveLoadFootway)
	{
		m_pCalc->m_pLoad->WriteCalcLiveLoadFootwayLsd(&XLText,pBri,m_nIndexTitle,nIndexMiddle++);
	}
	
	// ��� EH
	// (bEarthquake && pBri->IsBoxType() == FALSE) : bEarthquake�� FALSE�� �Ѱ��ְ� �����Ƿ� ���� �߰����� �ʴ´�.
	if(!bRcSlab && !bPiRahmen && pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_EH))
	{
		m_pCalc->m_pLoad->WriteCalcSoilPressure(&XLText, pBri, m_nIndexTitle, nIndexMiddle++, FALSE, pBri->GetEarthQuakeGoalByIndex(0));
	}
	
	// �������� ES
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_ES))
	{
		m_pCalc->m_pLoad->WriteCalcAlwaysLoadLsd(&XLText,pBri,m_nIndexTitle,nIndexMiddle++);
	}
	
	// ����Ȱ���� LS
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_LS))
	{
		m_pCalc->m_pLoad->WriteCalcAlwaysLiveLoadForLsd(&XLText,pBri,m_nIndexTitle,nIndexMiddle++);
	}
	
	// ������� EV
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_EV) && m_pCalc->m_pLoad->IsExistEV(pBri))
	{
		m_pCalc->m_pLoad->WriteCalcVerticalSoilPressureForLsd(&XLText,pBri,m_nIndexTitle,nIndexMiddle++);
	}
	
	// �η� BP
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_BP) && bBoxType == TRUE && (pBri->m_bApplyGroundWaterLevel == TRUE || (pBri->m_nSelectInteriorWaterPress == 1 && pBri->m_dHeightInteriorWaterPress > 0)))
	{
		m_pCalc->m_pLoad->WriteCalcBouyancyLsd(&XLText,pBri,m_nIndexTitle,nIndexMiddle++);
	}
	
	// ������ WA
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_WA) && m_pCalc->m_pLoad->IsExistWA(pBri))
	{
		m_pCalc->m_pLoad->WriteCalcWaterPressure(&XLText,pBri,m_nIndexTitle,nIndexMiddle++); 
	}
	// �µ���ȭ���� TU
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_TU))
	{
		m_pCalc->m_pLoad->WriteCalcTemperature(&XLText, pBri, m_nIndexTitle, nIndexMiddle++);
	}
	
	// �µ��� ���� TG
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_TG))
	{
		m_pCalc->m_pLoad->WriteCalcTemperatureGapLsd(&XLText, pBri, m_nIndexTitle, nIndexMiddle++);
	}
	
	// �������� SH
	if(!bRcSlab && pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_SH))
	{
		m_pCalc->m_pLoad->WriteCalcDryAndShrinkage(&XLText, pBri, m_nIndexTitle, nIndexMiddle++);
	}
	
	// ����ħ�� SD
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_SD))
	{
		m_pCalc->m_pLoad->WriteCalcSubsidence(&XLText, pBri, m_nIndexTitle, nIndexMiddle++);
	}

	//���������
	long nType = 0;
	if(pBri->GetCountUserLoadSetLsd(nType) > 0)
	{
		m_pCalc->m_pLoad->WriteCalcUserLoad_Lsd(&XLText, pBri, m_nIndexTitle, nIndexMiddle++, FALSE);
	}	

	// ��������
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_EQ))
	{
		WriteExcelEarthQuakeLoad(&XLText, pBri, nIndexMiddle);

		for(long nTypeEQ=0; nTypeEQ<eGoalEarthQuakeCount; ++nTypeEQ)
		{
			if(pBri->IsBoxType() == FALSE && nTypeEQ > 0) break;
			if(pBri->IsBoxType())
			{
				if(pBri->m_bGoalEarthQuake[nTypeEQ] == FALSE) continue;
			}

			m_pCalc->m_pLoad->WriteCalcUserLoad_Lsd(&XLText, pBri, m_nIndexTitle, nIndexMiddle++, TRUE, nTypeEQ);
		}
	}
}

void COutDesignBridge::WriteExcelEarthQuakeLoad( CXLFormatText *XLText, CRcBridgeRebar *pBri, long &nIndexMiddle )
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
//	BOOL bLsd		= pData->IsLsdDesign();
	BOOL bRcSlab   = (pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) ? TRUE : FALSE;
	BOOL bPiRahmen = (pBri->IsPiRahmen())   ? TRUE : FALSE;

	if(pBri->IsApplyEarthQuakeDesign())
	{
		if(pBri->IsBoxType())
		{
			// ������ ���
			for(long nCase=0; nCase<eGoalEarthQuakeCount; ++nCase)
			{
				if(pBri->m_bGoalEarthQuake[nCase] == FALSE) continue;
				EGoalOfEarthQuake eGoalEq = static_cast<EGoalOfEarthQuake>(nCase);

				m_pCalc->m_pLoad->WriteCalcSoilPressure(XLText,pBri,m_nIndexTitle,nIndexMiddle++, TRUE, eGoalEq);
			}
			
			// ��ü������
			m_pCalc->m_pLoadEQ->WriteCalcLoadInertialForce(XLText,pBri,m_nIndexTitle,nIndexMiddle++); 

			// Ⱦ���� ��������
			m_pCalc->m_pLoadEQ->WriteCalcLoadDirY(XLText,pBri,m_nIndexTitle,nIndexMiddle++); 
		}
		else
		{
			if(!bRcSlab)
			{
				m_pCalc->m_pLoadEQ->WriteCalcLoadDirX(XLText,pBri,m_nIndexTitle,nIndexMiddle++); 
				m_pCalc->m_pLoadEQ->WriteCalcLoadDirY(XLText,pBri,m_nIndexTitle,nIndexMiddle++); 
			}
			if(!bRcSlab && !bPiRahmen)
			{
				m_pCalc->m_pLoadEQ->WriteCalcLoadSoilPressure(XLText,pBri,m_nIndexTitle,nIndexMiddle++); 
			}
		}
	}
}

void COutDesignBridge::MakeExcelLoadDiagram(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	long nT_0 = 0;
	long nIndexMiddle = 1;
	XLText(nT_0);
	XLText.AddFormatText("$tb13%d. �� �� �� ��$n$n",m_nIndexTitle);

	//////////////////////////////////////////////////////////////////////////
	// 1. ����(2D Frame)
	if(bLsd)
	{
		m_pCalc->m_pLoad->WriteLoadDiagram_Lsd(&XLText, pBri, m_nIndexTitle,nIndexMiddle++);
	}
	else
	{
		m_pCalc->m_pLoad->WriteLoadDiagram(&XLText, pBri, m_nIndexTitle,nIndexMiddle++);
	}
	XLText += 1;

	//////////////////////////////////////////////////////////////////////////
	// 2. ������(2D Frame)
	if(pBri->IsApplyEarthQuakeDesign())
	{
		if(pBri->m_nTypeBridge!=BRIDGE_TYPE_SLAB)
		{
			XLText.AddFormatText("$s+30");

			if(bLsd)
				m_pCalc->m_pLoadEQ->WriteLoadDiagram_Lsd(&XLText, pBri, m_nIndexTitle,nIndexMiddle++);
			else
				m_pCalc->m_pLoadEQ->WriteLoadDiagram(&XLText, pBri, m_nIndexTitle,nIndexMiddle++);
		}
	}
}

void COutDesignBridge::MakeExcelModeling(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);
	
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nT_0 = 0;
	long nIndexMiddle = 1;
	XLText(nT_0);
	XLText.AddFormatText("$tb13%d. �𵨸� �ؼ�$n",m_nIndexTitle);

	//////////////////////////////////////////////////////////////////////////
	// 1. ����(2D Frame)
	m_pCalc->m_pFrame->WriteModelingNormal(&XLText, pBri, m_nIndexTitle, nIndexMiddle++);

	//////////////////////////////////////////////////////////////////////////
	// 2. ������(2D Frame)
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB && pBri->IsApplyEarthQuakeDesign())
	{
		XLText.AddFormatText("$s+99");
		m_pCalc->m_pFrame->WriteModelingEQ(&XLText, pBri, m_nIndexTitle, nIndexMiddle++);
	}

	// ���ݹݷ°��
	if(pBri->IsBoxType())
	{
		XLText.AddFormatText("$s+99");
		if(pBri->IsPileBase(0))
		{
			XLText.AddFormatText("$h$e$a%d.%d  ������ ������ ����$n$n",m_nIndexTitle,nIndexMiddle++);
			m_pCalc->m_pFrame->WriteModelingCoefficienOfPile(&XLText, pBri);
			if(pBri->IsApplyEarthQuakeDesign())
			{
				XLText.AddFormatText("$h$e$a%d.%d ���� �ݷ� ��� : ������$n$n",m_nIndexTitle,nIndexMiddle++);
				m_pCalc->m_pFrame->WriteModelingCoefficienOfSubgradeReaction(&XLText, pBri, TRUE);
			}
		}
		else
		{
			XLText.AddFormatText("$h$e$a%d.%d ���� �ݷ� ��� : ���$n$n",m_nIndexTitle,nIndexMiddle++);
			m_pCalc->m_pFrame->WriteModelingCoefficienOfSubgradeReaction(&XLText, pBri, FALSE);

			if(pBri->IsApplyEarthQuakeDesign())
			{
				XLText.AddFormatText("$h$e$a%d.%d ���� �ݷ� ��� : ������$n$n",m_nIndexTitle,nIndexMiddle++);
				m_pCalc->m_pFrame->WriteModelingCoefficienOfSubgradeReaction(&XLText, pBri, TRUE);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 3. 3D Shell
	if(pBri->m_nSelect3DShellTool==1) // MIDAS�� ��쿡�� ����
	{
		XLText.AddFormatText("$s+99");
		m_pCalc->m_pFrame->WriteModeling3DShell(&XLText, pBri, m_nIndexTitle, nIndexMiddle++);
	}
}

void COutDesignBridge::MakeExcelCombine(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);
	
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nT_1 = 1;
	long nIndexMiddle = 1;
	
	XLText.AddFormatText("$e$tb11%d. ��������$n$n",m_nIndexTitle);			
	//////////////////////////////////////////////////////////////////////////
	// ���α��϶��� kci2003���� ���ǵ��� ��(10281)
	if(pData->m_nTypeConditionApply != DESIGN_CONDITION_ROAD && pData->m_nTypeKciCode == 1 && pBri->m_bUnderGroundRahmen)
	{
		double dH = toM(pBri->m_dUnderGroundDesignHeight);
		XLText.AddFormatText("$h$m+02��H$r$c:$r������ �β��� ���� �������� ����Hv�� ���� �������$n$h");
		XLText.AddFormatText("$h$m+03h��2m �� ���ؼ�, ��H = 1.0$n$h");
		XLText.AddFormatText("$h$m+03h��2m �� ���ؼ�, ��H = 1.05-0.025h��0.875, ��H = 1.05-0.025 x %.3f = %.3f��0.875$n$h",
			dH, pBri->m_dFactorComboVLoad);
	}
	//////////////////////////////////////////////////////////////////////////
	// 1. ������� ����
	XLText(nT_1);
	XLText.AddFormatText("$e$h$a%d.%d ������� ����$n",m_nIndexTitle,nIndexMiddle);
	XLText += 1;
	MakeTableLoadComBo(&XLText, pBri, COMBO_SERVICE, TRUE);
	nIndexMiddle++;

	//////////////////////////////////////////////////////////////////////////
	// 2. ������� ����
	XLText(nT_1);
	XLText.AddFormatText("$s+30$e$h$a%d.%d ������� ����$n",m_nIndexTitle,nIndexMiddle);
	XLText += 1;
	MakeTableLoadComBo(&XLText, pBri, COMBO_ULTIMATE, TRUE);
	nIndexMiddle++;

	//////////////////////////////////////////////////////////////////////////
	// 3. ������� ����(�ִ� ���)
	if(pBri->m_MatComboEcc.GetRow() > 0)
	{
		XLText(nT_1);
		XLText.AddFormatText("$s+30$e$h$a%d.%d ������� ����(�ִ� ���)$n",m_nIndexTitle,nIndexMiddle);
		XLText += 1;
		MakeTableLoadComBo(&XLText, pBri, COMBO_ECCENTRIC, TRUE);
		nIndexMiddle++;
	}

	//////////////////////////////////////////////////////////////////////////
	// 4. �������� ����
	if(pBri->IsApplyEarthQuakeDesign() && pBri->IsRcSlab() == FALSE)
	{
		XLText(nT_1);
		XLText.AddFormatText("$e$h$a%d.%d �������� ����$n",m_nIndexTitle,nIndexMiddle);
		XLText += 1;
		MakeTableLoadComBo(&XLText, pBri, COMBO_EARTHQUAKE);
		nIndexMiddle++;
	}
}

void COutDesignBridge::MakeExcelForceSummarySheet(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);
	
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CString str1, str2;

	long nT_1 = 1;
	long nIndexMiddle = 1;

	pBri->TrimRibByFootingUpper();
	
	XLText.AddFormatText("$tb11%d. �ܸ�� ����$n$n",m_nIndexTitle);				
	//////////////////////////////////////////////////////////////////////////
	// 1. ���� �ܸ��
	XLText(nT_1);
	XLText.AddFormatText("$h$a%d.%d ���� �ܸ��$n",m_nIndexTitle,nIndexMiddle++);
	XLText += 1;
	MakeTableSectionForce(&XLText, pBri);
	XLText += 2;
	
	//////////////////////////////////////////////////////////////////////////
	if(pBri->IsApplyEarthQuakeDesign())
	{
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
		{
			if(pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH || pBri->m_nQtyJigan>4) XLText.AddFormatText("$s+99");
			// 2. ������ �ܸ��
			XLText.AddFormatText("$h$a%d.%d ������ �ܸ��$n",m_nIndexTitle,nIndexMiddle++);
			XLText += 1;
			if(pBri->IsBoxType())
			{
				MakeTableSectionForceEQ_BOX(&XLText, pBri, TRUE);
				MakeTableSectionForceEQ_BOX(&XLText, pBri, FALSE);
			}
			else
				MakeTableSectionForceEQ(&XLText, pBri);
		}
		XLText += 2;
	}

	if(pBri->IsBoxType() == FALSE)
	{
		//////////////////////////////////////////////////////////////////////////
		// 3. ��������ü ���ʰ���� ����	4. ������ ��ü ���ʰ���� ����
		if(pBri->IsOutsideWall())
		{
			long i=0; for(i=0; i<2; i++)
			{
				str1 = (i==0)? "������":"������";
				XLText.AddFormatText("$s+09$h$a$e%d.%d %s��ü ���ʰ���� ����$n", m_nIndexTitle, nIndexMiddle++, str1);
				XLText += 1;

				MakeTableLoadForFooting(&XLText, pBri, (i==0)? 0 : pBri->m_nQtyJigan);
				XLText += 2;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		// 5. ����(n)���ʰ���� ���� ~
		long nCntInWall = pBri->GetCountInWallReal();
		long i=0; for(i=0; i<nCntInWall; i++)
		{
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH) break;
			str1.Format("�߰���ü(�߰�����%d)���ʰ���� ����", i+1);
			XLText.AddFormatText("$s+09$h$a$e%d.%d %s$n", m_nIndexTitle, nIndexMiddle++, str1);
			XLText += 1;

			MakeTableLoadForFooting(&XLText, pBri, i+1);
			XLText += 2;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// �ܸ�µ� (BMD,SFD,AFD)
	XLText.AddFormatText("$s+45$h$a$e%d.%d �ܸ�µ�$n", m_nIndexTitle, nIndexMiddle++);
	XLText += 1;
	MakeDiagramBMD_SFD(&XLText,pBri);
}

void COutDesignBridge::MakeExcelSectionDesign(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);
	
	long nT_1 = 1;
	long nT_2 = 2;
	long nIndexMiddle = 1;
	long nIndexSmall  = 1;
	
	const HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	BOOL bIsSlab = (pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)? TRUE:FALSE;

	XLText.AddFormatText("$tb11%d. �ܸ����$n$n",m_nIndexTitle);				
	//////////////////////////////////////////////////////////////////////////
	// 1. ��ö�� ���
	XLText(nT_1);
	XLText.AddFormatText("$h$a%d.%d ��ö�� ���$n",m_nIndexTitle,nIndexMiddle++);
	XLText += 1;
	MakeTableMainRebarSummary(&XLText, pBri);
	//////////////////////////////////////////////////////////////////////////
	// 2. ����ö�� ���
	XLText.AddFormatText("$s+45");
	XLText.AddFormatText("$h$e$a%d.%d ����ö�� ���$n",m_nIndexTitle,nIndexMiddle++);
	XLText += 1;
	if(bLsd)
	{
		MakeTableShearRebarSummaryLsd(&XLText, pBri);
	}
	else
	{
		MakeTableShearRebarSummary(&XLText, pBri);
	}
	
	// 3. �ܸ� ����
	XLText.AddFormatText("$s+45");
	XLText.AddFormatText("$h$e$a%d.%d �ܸ� ����$n",m_nIndexTitle,nIndexMiddle++);
	XLText += 1;

	CString strEnv;
	HGBOOL bPrintDryingShrinkage = FALSE;
	const HGINT32 icValidComboEnvType = pBri->GetCountValidComboEnvType();
	long nCntItemSlab = pBri->GetCountConcSlab();
	long i=0; for(i=0; i<nCntItemSlab; i++)
	{
		CConcBasic *pConc = pBri->GetConcDataSlab_Lsd(i, &strEnv);
		if(pConc->m_nTypeSection!=SECTION_SINGLE) continue;
		if(Compare(pConc->m_Moment, 0.0, _T("<="), 0.01)) 
		{
			if (bLsd && (bPrintDryingShrinkage && (i+1) % icValidComboEnvType == 0))
			{
				bPrintDryingShrinkage = FALSE;

				CLsdBeam Beam(pConc, LSD_ULS_DESIGN);
				Beam.CalculateDryingShrinkage();
				Beam.MakeOutputDryingShrinkageExcelFile(&XLText, nT_1, 0);
			}

			continue;
		}

		XLText(nT_1);
		if(bIsSlab || pBri->IsBoxType()) 
			XLText.AddFormatText("$s+40$h$c%d)$r%s$n",nIndexSmall++,pConc->m_sTitle + strEnv);
		else 
			XLText.AddFormatText("$s+40$h$c%d)$r������ : %s$n",nIndexSmall++,pConc->m_sTitle + strEnv);

		if (bLsd)
		{
			BOOL bDesignShear = (pBri->m_bCalcShearCenter == FALSE && pConc->m_sTitle.Find(_T("�߾�")) != -1)? FALSE : TRUE;
			CLsdBeam pBeam(pConc, strEnv == "����" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);
			pBeam.MakeOutputDesignExcelFile(&XLText,nT_1, bDesignShear,FALSE);
			bPrintDryingShrinkage = TRUE;

			if ((i+1) % icValidComboEnvType == 0)
			{
				bPrintDryingShrinkage = FALSE;
				CLsdBeam Beam(pConc, LSD_ULS_DESIGN);
				Beam.CalculateDryingShrinkage();
				Beam.MakeOutputDryingShrinkageExcelFile(&XLText, nT_1, 0);
			}
		}
		else
		{
			CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
			pBeam.MakeOutputDesignExcelFile(&XLText,nT_1, TRUE,FALSE);
		}
		XLText += 1;
	}

	bPrintDryingShrinkage = FALSE;
	long nCntItemWall = pBri->GetCountConcOutWall();
	for(i=0; i<nCntItemWall; i++)
	{
		CConcBasic *pConc = pBri->GetConcDataWall_Lsd(i, &strEnv);
		if(pBri->m_nSelectWallSectionDesign==0 && pConc->m_nTypeSection!=SECTION_SINGLE) continue;
		if(pBri->m_nSelectWallSectionDesign==1 && pConc->m_nTypeSection!=SECTION_WALL) continue;
		if(pConc->m_Moment <=0.0)
		{
			if (bLsd && (bPrintDryingShrinkage && (i+1) % icValidComboEnvType == 0))
			{
				bPrintDryingShrinkage = FALSE;
				CLsdBeam Beam(pConc, LSD_ULS_DESIGN);
				Beam.CalculateDryingShrinkage();
				Beam.MakeOutputDryingShrinkageExcelFile(&XLText, nT_1, 0);
			}

			continue;
		}

		XLText(nT_1);
		XLText.AddFormatText("$s+40$h$c%d)$r��ü : %s$n",nIndexSmall++,pConc->m_sTitle + strEnv);
		if(pBri->m_nSelectWallSectionDesign==0)
		{
			if (bLsd)
			{
				bPrintDryingShrinkage = TRUE;
				BOOL bDesignShear = (pBri->m_bCalcShearCenter == FALSE && pConc->m_sTitle.Find(_T("�߾�")) != -1)? FALSE : TRUE;
				CLsdBeam pBeam(pConc, strEnv == "����" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);
				pBeam.MakeOutputDesignExcelFile(&XLText,nT_1, bDesignShear,FALSE);

				if ((i+1) % icValidComboEnvType == 0)
				{
					bPrintDryingShrinkage = FALSE;

					CLsdBeam Beam(pConc, LSD_ULS_DESIGN);
					Beam.CalculateDryingShrinkage();
					Beam.MakeOutputDryingShrinkageExcelFile(&XLText, nT_1, 0);
				}
			}
			else
			{
				CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
				pBeam.MakeOutputDesignExcelFile(&XLText,nT_1, TRUE,FALSE);
			}
		}
		else
		{
			CColumn pWall(pConc);
			pWall.MakeOutputExcel(&XLText,nT_2, FALSE, TRUE, TRUE, TRUE);
		}
		XLText += 1;
	}

	bPrintDryingShrinkage = FALSE;
	nCntItemWall = pBri->GetCountConcInWall();
	for(i=0; i<nCntItemWall; i++)
	{
		CConcBasic *pConc = pBri->GetConcDataInWall_Lsd(i, &strEnv);
		if(!pConc)	continue;

		if(pBri->m_nSelectWallSectionDesign==0 && pConc->m_nTypeSection!=SECTION_SINGLE) continue;
		if(pBri->m_nSelectWallSectionDesign==1 && pConc->m_nTypeSection!=SECTION_WALL) continue;
		if(pConc->m_Moment <=0.0)
		{
			if (bLsd && (bPrintDryingShrinkage && (i+1) % icValidComboEnvType == 0))
			{
				CLsdBeam Beam(pConc, LSD_ULS_DESIGN);
				Beam.CalculateDryingShrinkage();
				Beam.MakeOutputDryingShrinkageExcelFile(&XLText, nT_1, 0);
			}

			continue;
		}

		XLText(nT_1);
		XLText.AddFormatText("$s+40$h$c%d)$r��ü : %s$n",nIndexSmall++,pConc->m_sTitle + strEnv);
		if(pBri->m_nSelectWallSectionDesign==0)
		{
			if (bLsd)
			{
				bPrintDryingShrinkage = TRUE;
				BOOL bDesignShear = (pBri->m_bCalcShearCenter == FALSE && pConc->m_sTitle.Find(_T("�߾�")) != -1)? FALSE : TRUE;

				CLsdBeam pBeam(pConc, strEnv == "����" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);
				pBeam.MakeOutputDesignExcelFile(&XLText,nT_1, bDesignShear,FALSE);

				if ((i+1) % icValidComboEnvType == 0)
				{
					bPrintDryingShrinkage = FALSE;
					CLsdBeam Beam(pConc, LSD_ULS_DESIGN);
					Beam.CalculateDryingShrinkage();
					Beam.MakeOutputDryingShrinkageExcelFile(&XLText, nT_1, 0);
				}
			}
			else
			{
				CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
				pBeam.MakeOutputDesignExcelFile(&XLText,nT_1, TRUE,FALSE);
			}
		}
		else
		{
			CColumn pWall(pConc);
			pWall.MakeOutputExcel(&XLText,nT_2);
		}
		XLText += 1;
	}

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		long nCntItemArchRib = pBri->m_pArrConcArchRib.GetSize();
		long i=0; for(i=0; i<nCntItemArchRib; i++)
		{
			CConcBasic *pConc = pBri->m_pArrConcArchRib.GetAt(i);
			if(pConc->m_nTypeSection!=SECTION_SINGLE) continue;
			if(pConc->m_Moment <=0.0) continue;
			
			XLText(nT_1);
			XLText.AddFormatText("$s+40$h$c%d)$r��ġ���� : %s$n",nIndexSmall++,pConc->m_sTitle);
			CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
			pBeam.MakeOutputDesignExcelFile(&XLText,nT_1, TRUE,FALSE);

			XLText += 1;
		}
	}

	if(pBri->m_bExistProtectionRiseFooting)
	{
		bPrintDryingShrinkage = FALSE;
		
		long nCntItemPRF = pBri->GetCountConcPRF();
		long i=0; for(i=0; i<nCntItemPRF; i++)
		{
			CConcBasic *pConc = pBri->GetConcDataPRF_Lsd(i, &strEnv);
			if(pConc->m_nTypeSection!=SECTION_SINGLE) continue;
			if(Compare(pConc->m_Moment, 0.0, _T("<="), 0.01)) 
			{
				if (bLsd && (bPrintDryingShrinkage && (i+1) % icValidComboEnvType == 0))
				{
					bPrintDryingShrinkage = FALSE;

					CLsdBeam Beam(pConc, LSD_ULS_DESIGN);
					Beam.CalculateDryingShrinkage();
					Beam.MakeOutputDryingShrinkageExcelFile(&XLText, nT_1, 0);
				}

				continue;
			}

			XLText(nT_1);
			XLText.AddFormatText("$s+40$h$c%d)$r%s : %s$n",nIndexSmall++, pConc->m_sTitle, strEnv);

			if(bLsd)
			{
				BOOL bDesignShear = FALSE;
				CLsdBeam pBeam(pConc, strEnv == "����" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);
				pBeam.MakeOutputDesignExcelFile(&XLText,nT_1, bDesignShear,FALSE);
				bPrintDryingShrinkage = TRUE;

				if ((i+1) % icValidComboEnvType == 0)
				{
					bPrintDryingShrinkage = FALSE;
					CLsdBeam Beam(pConc, LSD_ULS_DESIGN);
					Beam.CalculateDryingShrinkage();
					Beam.MakeOutputDryingShrinkageExcelFile(&XLText, nT_1, 0);
				}
			}
			else
			{
				CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
				pBeam.MakeOutputDesignExcelFile(&XLText,nT_1, TRUE,FALSE);
			}
			XLText += 1;
		}
	}
	
}

void COutDesignBridge::MakeExcelUseableDesign(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);
	
	long nT_1 = 1;
	long nIndexMiddle = 1;
	long nIndexSmall  = 1;
	
	const HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	BOOL bIsSlab = (pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)? TRUE:FALSE;
	
	XLText.AddFormatText("$tb11%d. ��뼺 ����$n$n",m_nIndexTitle);				
	//////////////////////////////////////////////////////////////////////////
	// 1. ���ö�� ���
	XLText(nT_1);
	XLText.AddFormatText("$h$a%d.%d ���ö�� ���$n",m_nIndexTitle,nIndexMiddle++);
	XLText += 1;
	if (bLsd)
	{
		MakeTableMainRebarSummaryCrackLsd(&XLText, pBri);
	}
	else
	{
		MakeTableMainRebarSummaryCrack(&XLText, pBri);
	}
	
	// 2. �տ�����
	XLText.AddFormatText("$s+45");
	XLText.AddFormatText("$h$e$a%d.%d �տ� ����$n",m_nIndexTitle,nIndexMiddle++);
	XLText += 1;

	const ETypeEnvComboLsd eTypeEnvComboLsd = bLsd ? eEnvService : eEnvLimit;
	long nCntItemSlab = pBri->m_pArrConcSlab[eTypeEnvComboLsd].GetSize();
	long i=0; for(i=0; i<nCntItemSlab; i++)
	{
		CConcBasic *pConc = pBri->m_pArrConcSlab[eTypeEnvComboLsd].GetAt(i);
		if(pConc->m_nTypeSection!=SECTION_SINGLE) continue;
		if(pConc->m_MomentUse <=0.0) continue;

		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		if (bLsd == FALSE)
		{
			if(pBeam.GetW() <= 0.0) continue;
		}

		XLText(nT_1);
		if(bIsSlab || pBri->IsBoxType()) 
			XLText.AddFormatText("$s+20$h$c%d)$r%s$n$n",nIndexSmall++,pConc->m_sTitle);
		else 
			XLText.AddFormatText("$s+20$h$c%d)$r������ : %s$n$n",nIndexSmall++,pConc->m_sTitle);

		if (bLsd)
		{
			CLsdBeam BeamLsd(pConc, LSD_ULS_DESIGN);
			long nMatType = pBri->GetLsdMatrialTypePos(ePartUpperSlab, FALSE);
			BeamLsd.SetMatrialTypePos(nMatType);
			BeamLsd.MakeOutputUsabilityExcelFile(&XLText, nT_1, FALSE);	
		}
		else
		{
			pBeam.MakeOutputCrackExcelFile(&XLText,nT_1,FALSE);
		}
		
		XLText += 1;
	}
	long nCntItemWall = pBri->m_pArrConcWall[eTypeEnvComboLsd].GetSize();
	// (ARCBRIDGE-3618) LSD���� �ھ������� ����� ��뼺����� Baem���� �����͸� �����ʰ� ��հ��� �����͸� ��޶�� ��û���� ���� ����Ѵ�.
	if(bLsd && pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign==1 && pBri->m_bOutWallUsebilityCheck)
		nCntItemWall = 2;

	for(i=0; i<nCntItemWall; i++)
	{
		CConcBasic *pConc(NULL);
		if(bLsd && pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign==1 && pBri->m_bOutWallUsebilityCheck)
		{
			pConc = m_pCalc->m_pColGir->GetConcWallForOutWallUsebilityCheck(i==0);
		}
		else
			pConc= pBri->GetConcDataWall(i,TRUE, eTypeEnvComboLsd);

		if(pConc == NULL) continue;
		if(pConc->m_nTypeSection!=SECTION_SINGLE && pBri->m_bOutWallUsebilityCheck == FALSE) continue;

		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		if (bLsd == FALSE)
		{
			if(pBeam.GetW() <= 0.0) continue;
			if(pConc->m_MomentUse <=0.0) continue;
		}
		else
		{
			if(pConc->m_MomentUse <=0.0 && pConc->m_MomentCrack <=0.0) continue;
		}
		

		XLText(nT_1);
		XLText.AddFormatText("$s+20$h$c%d)$r��ü : %s$n$n",nIndexSmall++,pConc->m_sTitle);

		if (bLsd)
		{
			CLsdBeam BeamLsd(pConc, LSD_ULS_DESIGN);
			long nMatType = pBri->GetLsdMatrialTypePos(ePartOutWallStt, FALSE);
			BeamLsd.SetMatrialTypePos(nMatType);
			BeamLsd.MakeOutputUsabilityExcelFile(&XLText, nT_1, FALSE);	
		}
		else
		{
			pBeam.MakeOutputCrackExcelFile(&XLText,nT_1,FALSE);
		}
		
		XLText += 1;
	}
	long nCntItemInWall = pBri->m_pArrConcInWall[eTypeEnvComboLsd].GetSize();
	for(i=0; i<nCntItemInWall; i++)
	{
		CConcBasic *pConc = pBri->m_pArrConcInWall[eTypeEnvComboLsd].GetAt(i);
		if(pConc->m_nTypeSection!=SECTION_SINGLE) continue;
		if(pConc->m_MomentUse <=0.0) continue;

		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		if(pBeam.GetW() <= 0.0) continue;

		XLText(nT_1);
		XLText.AddFormatText("$s+20$h$c%d)$r������ü : %s$n$n",nIndexSmall++,pConc->m_sTitle);
		if (bLsd)
		{
			CLsdBeam BeamLsd(pConc, LSD_ULS_DESIGN);
			long nMatType = pBri->GetLsdMatrialTypePos(ePartInWall, FALSE);
			BeamLsd.SetMatrialTypePos(nMatType);
			BeamLsd.MakeOutputUsabilityExcelFile(&XLText, nT_1, FALSE);	
		}
		else
		{
			pBeam.MakeOutputCrackExcelFile(&XLText,nT_1,FALSE);
		}
		
		XLText += 1;
	}
	long nCntItemArchRib = pBri->m_pArrConcArchRib.GetSize();
	for(i=0; i<nCntItemArchRib; i++)
	{
		CConcBasic *pConc = pBri->m_pArrConcArchRib.GetAt(i);
		if(pConc->m_nTypeSection!=SECTION_SINGLE) continue;
		if(pConc->m_MomentUse <=0.0) continue;

		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		if(pBeam.GetW() <= 0.0) continue;
		
		XLText(nT_1);
		XLText.AddFormatText("$s+20$h$c%d)$r��ġ���� : %s$n$n",nIndexSmall++,pConc->m_sTitle);
		pBeam.MakeOutputCrackExcelFile(&XLText,nT_1,FALSE);
		XLText += 1;
	}
	long nCntItemPRF = pBri->m_pArrConcPRF[eTypeEnvComboLsd].GetSize();
	for(i=0; i<nCntItemPRF; i++)
	{
		CConcBasic *pConc = pBri->m_pArrConcPRF[eTypeEnvComboLsd].GetAt(i);
		if(pConc->m_nTypeSection!=SECTION_SINGLE) continue;
		if(pConc->m_MomentUse <=0.0) continue;

		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		if (bLsd == FALSE)
		{
			if(pBeam.GetW() <= 0.0) continue;
		}

		XLText(nT_1);
		XLText.AddFormatText("$s+20$h$c%d)$r%s$n$n",nIndexSmall++,pConc->m_sTitle);

		if(bLsd)
		{
			CLsdBeam BeamLsd(pConc, LSD_ULS_DESIGN);
			long nMatType = pBri->GetLsdMatrialTypePos(ePartLowerSlab, FALSE);
			BeamLsd.SetMatrialTypePos(nMatType);
			BeamLsd.MakeOutputUsabilityExcelFile(&XLText, nT_1, FALSE);	
		}
		else
		{
			pBeam.MakeOutputCrackExcelFile(&XLText,nT_1,FALSE);
		}

		XLText += 1;
	}
}

void COutDesignBridge::MakeExcelSluggishAndFatigue(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);
	
	long nT_0 = 0;
	long nIndexMiddle = 1;
	XLText(nT_0);
	XLText.AddFormatText("$tb13%d. ó�� �� �Ƿ� ����$n",m_nIndexTitle);
	XLText += 1;
	
	m_pCalc->m_pFatigue->WriteCalcSluggishAndFatigue(&XLText, pBri, m_nIndexTitle, nIndexMiddle);
}

void COutDesignBridge::MakeExcelCornerAndSupportBar(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);
	
	CString szCheck = pBri->IsOutsideWall() ? _T("�찢�� �� ���ö��") : _T("���ö��");

	long nT_0 = 0;
	long nIndexMiddle = 1;
	XLText(nT_0);
	XLText.AddFormatText("$tb13%d. %s ����$n",m_nIndexTitle,szCheck);
	XLText += 1;
	
	if(pBri->IsOutsideWall() && pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB && pBri->m_nTypeBridge != BRIDGE_TYPE_PI) 
	{
		m_pCalc->m_pCorner[iUPPER]->WriteCalcCorner(&XLText, m_nIndexTitle,nIndexMiddle++);
		if(pBri->IsBoxType())
		{
			m_pCalc->m_pCorner[iLOWER]->WriteCalcCorner(&XLText, m_nIndexTitle,nIndexMiddle++);
		}
	}
	m_pCalc->m_pCorner[iUPPER]->WriteCalcSupport(&XLText, m_nIndexTitle,nIndexMiddle++);
}

void COutDesignBridge::MakeExcelColumnAndGirder(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);
	
	long nT_0 = 0;
	long nIndexMiddle = 1;
	XLText(nT_0);
	XLText.AddFormatText("$tb13%d. ��,���(�߰���ü) ����$n",m_nIndexTitle);
	XLText += 1;

	BOOL bExistColumn = FALSE;
	BOOL bExistShoe   = FALSE;
	for(long nJijum=0; nJijum<pBri->m_nQtyJigan+1; nJijum++)
	{
		if(pBri->GetTypeColumnByJijum(nJijum)!=-1) 
			bExistColumn = TRUE;
		
		CJijum *pJijum = pBri->GetJijum(nJijum);
		long nVerPos = (nJijum==0) ? 1 : 0;
		if(pJijum->m_nQtyShoe[nVerPos]>0)
			bExistShoe = TRUE;
	}
	
	if(bExistColumn || bExistShoe)
	{
		m_pCalc->m_pColGir->WriteCalcGirder(&XLText, pBri, m_nIndexTitle,nIndexMiddle++, TRUE);
		if(pBri->IsBoxType())
		{
			m_pCalc->m_pColGir->WriteCalcGirder(&XLText, pBri, m_nIndexTitle,nIndexMiddle++, FALSE);
		}
	}
	if(m_pCalc->m_pColGir->IsExistJongGirder()) 
		m_pCalc->m_pColGir->WriteCalcJongGirder(&XLText, pBri, m_nIndexTitle,nIndexMiddle++);

	long nCntInWall = (pBri->GetJijumConnectRibAndSlab() > 0)? pBri->GetCountInWallReal()-1 : pBri->GetCountInWallReal();
	if(pBri->m_nSelectInWallSectionDesign == 0)	// ������� ����ÿ��� �߰���ü�� Ÿ���� üũ�Ѵ�.
		nCntInWall = (pBri->GetJijumConnectRibAndSlab() > 0)? pBri->GetCountInWallReal(TRUE)-1 : pBri->GetCountInWallReal(TRUE);
	if((nCntInWall > 0 || (pBri->m_nSelectWallSectionDesign == 1 && pBri->m_pARcBridgeDataStd->IsLsdDesign())) && pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
	{
		m_pCalc->m_pColGir->WriteCalcColumn(&XLText, pBri, m_nIndexTitle,nIndexMiddle++);
	}
}

void COutDesignBridge::MakeExcelFootingSafeCheck(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);
	
	long nT_0 = 0;
	long nIndexMiddle = 1;
	XLText(nT_0);
	XLText.AddFormatText("$tb13%d. ���ʾ��� ����$n",m_nIndexTitle);
	XLText += 1;
	
	if(pBri->IsBoxType())
	{
		if(pBri->m_bApplyGroundWaterLevel)
		{
			m_pCalc->m_pFooting->WriteCalcFootingBuoyancyCheck(&XLText, m_nIndexTitle, nIndexMiddle++);
			XLText += 1;
		}
		m_pCalc->m_pFooting->WriteCalcFootingSupportCheck(&XLText, m_nIndexTitle, nIndexMiddle++, FALSE);
	}
	else
	{
		m_pCalc->m_pFooting->WriteCalcFootingSafeCheck(&XLText, pBri, m_nIndexTitle,nIndexMiddle++);
	}
}

void COutDesignBridge::MakeExcelFootingDesign(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);
	
	long nT_0 = 0;
	long nIndexMiddle = 1;
	XLText(nT_0);
	XLText.AddFormatText("$tb13%d. �� �� �� ��$n",m_nIndexTitle);
	XLText += 1;


	if( pBri->m_pARcBridgeDataStd->IsLsdDesign() )
		m_pCalc->m_pFooting->WriteCalcFootingDesignLsd(&XLText, pBri, m_nIndexTitle,nIndexMiddle++);
	else
		m_pCalc->m_pFooting->WriteCalcFootingDesign(&XLText, pBri, m_nIndexTitle,nIndexMiddle++);
}

void COutDesignBridge::MakeExcelWingWall(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);
	
	long nT_0 = 0;
	long nIndexMiddle = 1;
	XLText(nT_0);
	XLText.AddFormatText("$tb13%d. ������ ����$n",m_nIndexTitle);
	XLText +=1;

	if(pBri->IsWingWall(TRUE, TRUE)) m_pCalc->m_pWing[0]->WriteDesignWingWall(&XLText, m_nIndexTitle, nIndexMiddle++);
	if(pBri->IsWingWall(TRUE, FALSE)) m_pCalc->m_pWing[1]->WriteDesignWingWall(&XLText, m_nIndexTitle, nIndexMiddle++);
	if(pBri->IsWingWall(FALSE, TRUE)) m_pCalc->m_pWing[2]->WriteDesignWingWall(&XLText, m_nIndexTitle, nIndexMiddle++);
	if(pBri->IsWingWall(FALSE, FALSE)) m_pCalc->m_pWing[3]->WriteDesignWingWall(&XLText, m_nIndexTitle, nIndexMiddle++);
}

void COutDesignBridge::MakeExcelApproachSlab(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);
	
	CApsApp *pAps;
	
	long nT_0 = 0;
	long nIndexMiddle = 1;
	XLText(nT_0);
	XLText.AddFormatText("$tb13%d. ���ӽ����� ����$n",m_nIndexTitle);

	BOOL bCalcStt1 = TRUE;
	BOOL bCalcStt2 = FALSE;
	BOOL bCalcEnd1 = FALSE;
	BOOL bCalcEnd2 = FALSE;

	pAps = &pBri->m_apsStt[0];
	double dLenAspStt = max(pAps->GetWidth(iLEFT), pAps->GetWidth(iRIGHT));
	double dThickStt = pAps->m_dH1 + pAps->m_dH2;

	pAps = &pBri->m_apsStt[1];
// 	double dLenAspStt2 = 0;
	if(pAps->m_bIs)
	{
		double dLenAspStt2 = max(pAps->GetWidth(iLEFT), pAps->GetWidth(iRIGHT));
		bCalcStt2 = (Compare(dLenAspStt, dLenAspStt2, "=", 0.001))? FALSE : TRUE;
	}
	
	pAps = &pBri->m_apsEnd[0];
	double dLenAspEnd = max(pAps->GetWidth(iLEFT), pAps->GetWidth(iRIGHT));
	double dThickEnd = pAps->m_dH1 + pAps->m_dH2;
	bCalcEnd1 = (pAps->m_bIs == FALSE || (Compare(dLenAspStt, dLenAspEnd, "=", 0.001) && Compare(dThickStt, dThickEnd, "=", 0.001)))? FALSE : TRUE;

	pAps = &pBri->m_apsEnd[1];
// 	double dLenAspEnd2 = 0;
	if(pAps->m_bIs)
	{
		double dLenAspEnd2 = max(pAps->GetWidth(iLEFT), pAps->GetWidth(iRIGHT));
		bCalcEnd2 = (Compare(dLenAspEnd, dLenAspEnd2, "=", 0.001))? FALSE : TRUE;
	}

	BOOL bWriteOne = (bCalcStt1 && !bCalcStt2 && !bCalcEnd1 && !bCalcEnd2)? TRUE : FALSE;
	if(bCalcStt1)
	{
		if(!bWriteOne)
		{
			XLText.GoPosition(NO_MOVE,COL_POS_1);
			XLText.AddFormatText("$n$s+05$e$h$a%d.%d ���ӽ����� ����(������%s)$n",m_nIndexTitle,nIndexMiddle++, bCalcStt2? " ����" : "");
		}
		m_pCalc->m_pApsStt[0]->WriteCalcAps(&XLText,pBri,m_nIndexTitle,1, bWriteOne); 
		if(!bWriteOne) XLText.AddFormatText("$s+99");
	}
	if(bCalcStt2)
	{
		XLText.GoPosition(NO_MOVE,COL_POS_1);
		XLText.AddFormatText("$n$s+05$e$h$a%d.%d ���ӽ����� ����(������ ����)$n",m_nIndexTitle,nIndexMiddle++);
		m_pCalc->m_pApsStt[1]->WriteCalcAps(&XLText,pBri,m_nIndexTitle,1, FALSE); 
		XLText.AddFormatText("$s+99");
	}
	if(bCalcEnd1)
	{
		XLText.GoPosition(NO_MOVE,COL_POS_1);
		XLText.AddFormatText("$n$s+05$e$h$a%d.%d ���ӽ����� ����(������%s)$n",m_nIndexTitle,nIndexMiddle++, bCalcEnd2? " ����" : "");
		m_pCalc->m_pApsEnd[0]->WriteCalcAps(&XLText,pBri,m_nIndexTitle,1, FALSE); 
		XLText.AddFormatText("$s+99");
	}
	if(bCalcEnd2)
	{
		XLText.GoPosition(NO_MOVE,COL_POS_1);
		XLText.AddFormatText("$n$s+05$e$h$a%d.%d ���ӽ����� ����(������ ����)$n",m_nIndexTitle,nIndexMiddle++, bCalcEnd2? " ����" : "");
		m_pCalc->m_pApsEnd[1]->WriteCalcAps(&XLText,pBri,m_nIndexTitle,1, FALSE); 
		XLText.AddFormatText("$s+99");
	}
}

void COutDesignBridge::MakeExcelExpJoint(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);

	long nSizeExpJoint = pBri->m_pDesignExpanJoint->GetSize();
	if(nSizeExpJoint == 0) return;

	long nT_0 = 0;
	long nIndexMiddle = 1;
	XLText(nT_0);
	XLText.AddFormatText("$tb13%d. �������� ����$n",m_nIndexTitle);

	for(long nExp = 0; nExp < nSizeExpJoint; nExp++)
	{
		XLText.GoPosition(NO_MOVE,COL_POS_1);
		XLText.AddFormatText("$n$s+40$e$h$a%d.%d �������� ����(%s)$n",m_nIndexTitle,nIndexMiddle++, nExp == 0 ? _T("����") : _T("����") );

		CStringArray szArr;
		pBri->m_pDesignExpanJoint->SetExpansionJoint(nExp);
		pBri->m_pDesignExpanJoint->GetTextExpansionJoint(nExp, ACCURACY_METHOD, szArr);

		for(long nRow=0; nRow<szArr.GetSize(); nRow++)
		{
			XLText.AddFormatText("  %s$n",szArr.GetAt(nRow));
		}
	}
}

void COutDesignBridge::MakeExcelCantilever(CXLControl *pXL, CRcBridgeRebar *pBri, HGBOOL bLeft)
{
	CXLFormatText XLText(pXL);

	CString str = _T("");
	CString strPos = _T("");

//	long nIndexMiddle = 1;
	long nIndexSmall  = 1;

	CCalcCantilever *pCalcCantilever = m_pCalc->m_pCantilever;
	pCalcCantilever->WriteCantilever(&XLText, COL_POS_1, bLeft, m_nIndexTitle, nIndexSmall++);
}

void COutDesignBridge::MakeExcelPile(CXLControl *pXL, CRcBridgeRebar *pBri, long nIndex)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CXLFormatText XLText(pXL);

	long nRowStt = 0;
	CString str = _T("");
	CString strPos = _T("");

	CDesignPile *pCalcPile = m_pCalc->m_pArrPileCalc.GetAt(nIndex);
	
	double dScaleDim = 100;
	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(dScaleDim);
	pDom->SetLTScale(dScaleDim*5);
	if(pBri->IsBoxType())
	{
		CFootingApp *pFooting = pBri->GetFooting(0);
		CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, pData);

		long nTypeJongdanForRebarOrg = pBri->m_nTypeJongdanForRebar;
		pBri->m_nTypeJongdanForRebar = 2;
		pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE);
		DBStd.m_pDrawVertSection->DrawVertSection(pDom, TRUE, &pBri->m_tvArrVertSection);
		DBStd.m_pDrawExFooting->DrawFront(pDom, &pFooting->m_exFooting, FALSE);
		long nCount = pBri->GetCountInWall();
		for(long ix = 0; ix < nCount; ix++)
		{
			CWallApp *pWall = pBri->GetInWall(ix);
			if(pWall)
				DBStd.m_pDrawWall->DrawFront(pDom, pWall, TRUE);
		}
		DBStd.m_pDrawVertSection->DrawExFootingBoxType(pDom, FALSE);
		DBStd.m_pDrawVertSection->DimVerSectionBoxSideStt(pDom, 0, FALSE);
		DBStd.m_pDrawVertSection->DimVerSectionBoxSideEnd(pDom, 0, FALSE);
		DBStd.m_pDrawVertSection->DimVerSectionHunch(pDom, 0, TRUE, FALSE, FALSE);
		DBStd.m_pDrawVertSection->DimVerSectionHunch(pDom, 0, FALSE, FALSE, FALSE);
		DBStd.m_pDrawExFooting->DimFrontLowerBox(pDom, pBri, 0, FALSE);
		DBStd.m_pDrawVertSection->DimVertSectionLowerSlab(pDom, 0, TRUE, FALSE);

		pBri->m_nTypeJongdanForRebar = nTypeJongdanForRebarOrg;
	}
	else
	{
		m_pCalc->m_pLoadEQ->DrawFootingPileBase(pDom,pCalcPile->m_nIndexJijum);
	}
	*pCalcPile->m_pDomPlan << *pDom;
	
	if(pCalcPile->m_nIndexJijum==0) 
		strPos = "������ ����";
	else if(pCalcPile->m_nIndexJijum==pBri->m_nQtyJigan) 
		strPos = "������ ����";
	else
		strPos.Format("�߰������� %d",pCalcPile->m_nIndexJijum);
	
	if(m_pCalc->m_pArrPileCalc.GetSize() > 1)
	{
		if(nIndex == 0) XLText.AddFormatText("$tb13%d. �� �� �� ��$n$n",m_nIndexTitle);
		XLText.AddFormatText("$h$r$a%d.%d %s ���Ұ���$n",m_nIndexTitle, nIndex+1, strPos);
		nRowStt = 3;
	}
	else
	{
		if(pBri->IsBoxType())
			XLText.AddFormatText("$tb13%d. �� �� �� ��$n$n",m_nIndexTitle);
		else
			XLText.AddFormatText("$tb13%d. �� �� �� �� : %s$n$n",m_nIndexTitle, strPos);
		nRowStt = 1;
	}

	if(pBri->IsBoxType())
	{
		XLText.GoPosition(nRowStt, COL_POS_1);
		XLText.AddFormatText("$s+07$h$a1) ��������$n$n"); 
		nRowStt = pCalcPile->WritePileCondition(XLText.GetExcelControl(), nRowStt+1, 0, TRUE, NUM_CIRCLE);

		XLText.GoPosition(nRowStt+1, COL_POS_1);
		XLText.AddText("$s+20");
		XLText.OutXL();
		nRowStt = XLText.GetRowPostion();
		XLText.AddFormatText("$s+07$h$e$a2) ������ Ư��ġ ���$n$n");
		nRowStt = pCalcPile->WritePileAttribute(XLText.GetExcelControl(), nRowStt+1, 0, TRUE, NUM_CIRCLE, 1, TRUE);

		XLText.AddText("$s+12");
		XLText.GoPosition(nRowStt+1, COL_POS_1);
		XLText.OutXL();

		if(pData->IsLsdDesign())
		{
			long nIndexMiddle = 1;
			m_pCalc->m_pFooting->WriteCalcFootingPile(&XLText, m_nIndexTitle, nIndexMiddle);
		}
		else
		{
			long nIndexMiddle = 1;
			m_pCalc->m_pFooting->WriteCalcFootingPile(&XLText, m_nIndexTitle, nIndexMiddle);
		}

		nRowStt = XLText.GetRowPostion();
		nRowStt = (nRowStt%XFL_LINES_PER_PAGE==0) ? nRowStt : (nRowStt/XFL_LINES_PER_PAGE+1)*XFL_LINES_PER_PAGE;
		nRowStt = pCalcPile->WriteCheckConnectionExtendFooting(XLText.GetExcelControl(), nRowStt, 5, TRUE, 3, TRUE);	

		XLText.OutXL();

	}
	else
	{
		nRowStt = pCalcPile->WritePileOutput(pXL,nRowStt,FALSE);
	}
	
	delete pDom;
}

// void COutDesignBridge::MakeExcelSubsidence(CXLControl *pXL, CRcBridgeRebar *pBri)
// {
// }


// BOOL bIncreaseCombo : ����ħ�Ͽ� ����п� ���� �������������� ������ ���� �������..
void COutDesignBridge::MakeTableLoadComBo(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nType, BOOL bIncreaseCombo)			// 0: �������, 1: �������, 2: �������(�ִ����), 3: ��������
{
	CString strItem = _T("");
	CMatrix pDataMatrix;
	DWORD	dwCol = 0;

	switch(nType) {
	case 0:						// ������� ����
		strItem = "SV";
		pDataMatrix = pBri->m_MatComboUse;
		dwCol = pBri->m_dWLoadNormal;
		break;
	case 1:						// ������� ����
		strItem = "UL";
		pDataMatrix = pBri->m_MatComboUlt;
		dwCol = pBri->m_dWLoadNormal;
		break;
	case 2:						// ������� ����(�ִ� ���)
		strItem = "UE";
		pDataMatrix = pBri->m_MatComboEcc;
		dwCol = pBri->m_dWLoadNormal;
		break;
	case 3:						// �������� ����
		strItem = "EQ";
		pDataMatrix = pBri->m_MatComboEQ;
		dwCol = pBri->m_dWLoadEQ;
		break;
	default:
		return;
	}

	BOOL bEnvToapOne = pBri->m_bSelectEnvToapOne;
	BOOL bEnvSubsid = pBri->m_bSelectEnvSubsid? TRUE : FALSE;
	if(pBri->IsBoxType() || bEnvSubsid)
		bIncreaseCombo = FALSE;
	
	// (ARCBRIDGE-2953) �������� ��¿����� ������ �Է¿� �ִ� �⺻���ո� ��� �ϵ���.
	bIncreaseCombo = FALSE;

	// table�� ��ü row��
	CStringArray strAr;
	long nCntCol = 0;
	long nCntRow = pBri->GetSizeCombo(nType, bIncreaseCombo) + 1;
	long nCntComboOrg = pBri->GetSizeCombo(nType, FALSE) + 1;
	
	BOOL bEarthAnchor = (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR && pBri->m_bApplyAnchorForce);
	BOOL bApplyLoad[BRIDGE_LOAD_BASIC+1] = {FALSE,}; // +1 : ��Ŀ��
	
	strAr.RemoveAll();
	strAr.Add("�� ��");
	if(dwCol & LOAD_CASE_DEAD)		{ bApplyLoad[nCntCol]=TRUE; strAr.Add("����\n����"); }	  nCntCol++;
	if(dwCol & LOAD_CASE_SOIL)		{ bApplyLoad[nCntCol]=TRUE; strAr.Add("����\n���"); }	  nCntCol++;
	if(dwCol & LOAD_CASE_LIVE_FULL)	{ bApplyLoad[nCntCol]=TRUE; strAr.Add("����\nȰ����"); }  nCntCol++;
//  	if(dwCol & LOAD_CASE_LIVE_ECC)	// ���� Ȱ������ ������� �ʰ� ����.
// 	{ 
// 		bApplyLoad[nCntCol]=TRUE; 
// 		strAr.Add("����\nȰ����"); 
// 	}  
	nCntCol++;
	if(dwCol & LOAD_CASE_TANK)		{ bApplyLoad[nCntCol]=TRUE; strAr.Add("������\n����"); }  nCntCol++;
	if(dwCol & LOAD_CASE_TOAP_BOTH)	{ bApplyLoad[nCntCol]=TRUE; strAr.Add("�����"); }		  nCntCol++;
	if(dwCol & LOAD_CASE_TOAP_ONE)	{ bApplyLoad[nCntCol]=TRUE; strAr.Add("�����"); }		  nCntCol++;
	if(dwCol & LOAD_CASE_TEMP1)	{ bApplyLoad[nCntCol]=TRUE; strAr.Add("�µ�\n����(+)"); } nCntCol++;
	if(dwCol & LOAD_CASE_TEMP2)	{ bApplyLoad[nCntCol]=TRUE; strAr.Add("�µ�\n����(-)"); } nCntCol++;
	if(dwCol & LOAD_CASE_SHRINK)	{ bApplyLoad[nCntCol]=TRUE; strAr.Add("����\n����"); }	  nCntCol++;
	if(dwCol & LOAD_CASE_SUBSID)	{ bApplyLoad[nCntCol]=TRUE; strAr.Add("����\nħ��"); }	  nCntCol++;
	if(dwCol & LOAD_CASE_SB)		{ bApplyLoad[nCntCol]=TRUE; strAr.Add("$tx07�õ� ��\n��������"); } nCntCol++;
	if(dwCol & LOAD_CASE_LR)		{ bApplyLoad[nCntCol]=TRUE; strAr.Add("$tx07��뷹��\n������"); }  nCntCol++;
	if(dwCol & LOAD_CASE_EQ)		{ bApplyLoad[nCntCol]=TRUE; strAr.Add("����\n����"); }		  nCntCol++;
	if(dwCol & LOAD_CASE_WATERHORI)	{ bApplyLoad[nCntCol]=TRUE; strAr.Add("����\n����"); }		  nCntCol++;
	if(dwCol & LOAD_CASE_WATERVERT)	{ bApplyLoad[nCntCol]=TRUE; strAr.Add("����\n����"); }		  nCntCol++;
	if(bEarthAnchor)
	{
		bApplyLoad[nCntCol]=TRUE; 
		strAr.Add("��Ŀ��");
		nCntCol++;
	}


	long nCol = 0, nRow = 0;

	nCntCol = strAr.GetSize();
	if(nCntCol > 11) XLText->GoPosition(NO_MOVE, 0);
	else XLText->GoPosition(NO_MOVE,COL_POS_1);
	
	CString str = _T("");
	long nWidthTitle = (nCntCol < 11) ? 3 : 2; 
	XLText->AddTable(nCntRow, (nWidthTitle+2*(nCntCol-1))-1, 1, nWidthTitle);

	// Title
	long nWidthJoin = 0;
	for(nCol = 0; nCol<nCntCol; nCol++)
	{
		nWidthJoin = (nCntCol < 11 && nCol == 0)? 3:2;
		str = strAr.GetAt(nCol);
		XLText->AddMergeFormatText(0,nWidthJoin-1,"$c%s$md1", str);
		XLText->AddFormatText("$m+0%d",nWidthJoin);
	}
	XLText->AddText("$n$h");
	XLText->AddText("$n$h");

	// Data
	long nCountSubsid = bIncreaseCombo? MAX(1, pBri->GetCountSubsidence()) : 1;

	long nIndexCombo = 0;
	double dData = 0;
	for(long nComboSubsid = 0; nComboSubsid < nCountSubsid; nComboSubsid++)
	{
		if(bEnvSubsid && nComboSubsid == 1) break;

		for(long nComboToapOne = 0; nComboToapOne < (bIncreaseCombo? 2:1); nComboToapOne++)
		{
			if(bEnvToapOne && nComboToapOne == 1) break;

			for(nRow=1; nRow<nCntComboOrg; nRow++)
			{	
				long nIdxCol = 0;

				DWORD dWCombo = pBri->GetLoadCombo(nRow-1,nType);

				if(nComboSubsid > 0 && (dWCombo&LOAD_CASE_SUBSID)==FALSE) continue;
				if(nComboToapOne > 0 && (dWCombo&LOAD_CASE_TOAP_ONE)==FALSE) continue;

				for(nCol=0; nCol<BRIDGE_LOAD_BASIC+2; nCol++)
				{
					nWidthJoin = (nCntCol < 11 && nCol == 0)? 3:2;

					if(nCol == 0) str.Format("%s%02d", strItem, ++nIndexCombo);
					else if(bApplyLoad[nCol-1]==FALSE) continue;
					else
					{
						if(nCol == BRIDGE_LOAD_BASIC+1)
							dData = pBri->m_dFactorAnchor[nType][nRow-1];
						else
							dData = pDataMatrix(nRow-1, nCol-1);
						if(dData == 0) str = _T("");
						else str.Format("%.02f", dData);
					}

					XLText->AddMergeFormatText(0,nWidthJoin-1,"$f2$c%s", str);
					XLText->AddFormatText("$m+0%d",nWidthJoin);

					nIdxCol++;
				}
				XLText->AddText("$n$h");
			}
		}
	}

	*XLText += 2;
}

void COutDesignBridge::MakeTableMainRebarSummary(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	CARcBridgeDataStd *pBriStd = pBri->m_pARcBridgeDataStd;

	const HGBOOL bLsd = pBriStd->IsLsdDesign();
	BOOL bIsSlab = (pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)? TRUE:FALSE;
	long nDec = (pBriStd->IsSI() || bLsd) ? 1:3;
	CString str = _T("");
	long nRowSlab = 0;
	long nRowWallStt = 0;
	long nRowWallEnd = 0;
	long nRowArchRib = 0;
	long nRowPRF = 0;
	long nCntRow = 0;
	long nCntCol = 0;

//////////////////////////////////////////////////////////////////////////
//	��ü row�� �� Item�� row�� 
	CStringArray strRebarArr;
	CDoubleArray dItemRowArr;
	std::vector<HGINT32> vecRowInWall;
	long nCntItemSlab = pBri->GetCountConcSlab();
	long nCntItemWall = (pBri->IsOutsideWall())? pBri->GetCountConcOutWall():0;
	long nCntItemInWall = pBri->GetCountConcInWall();
	long nCntItemArchRib = pBri->m_pArrConcArchRib.GetSize();
	long nCntItemPRF = pBri->GetCountConcPRF();
	long nCntItemRow = 0;

	long nIdxSttArchRib = nCntItemSlab + nCntItemWall + nCntItemInWall;
	long nIdxSttPRF = nCntItemSlab + nCntItemWall + nCntItemInWall + nCntItemArchRib;

	CString strTemp = _T("");
	const HGINT32 icValidEnvType = pBri->GetCountValidComboEnvType();

	long i=0; for(i=0; i<nCntItemSlab+nCntItemWall+nCntItemArchRib+nCntItemPRF; i++)
	{
		BOOL bSlab = (i<nCntItemSlab)? TRUE:FALSE;

		CConcBasic *pData = NULL;
		if(i<nCntItemSlab)
			pData = pBri->GetConcDataSlab_Lsd(i);
		else if(i<(nCntItemSlab + nCntItemWall))
			pData = pBri->GetConcDataWall_Lsd(i-nCntItemSlab);
		else if(i< nIdxSttArchRib)
			pData = pBri->m_pArrConcArchRib.GetAt(i-nIdxSttArchRib);
		else
			pData = pBri->GetConcDataPRF_Lsd(i-nIdxSttPRF);
		
		strTemp = m_pCalc->m_pSummary->GetStringMainRebarUse(pData, nCntItemRow);

		if(nCntItemRow < 2) nCntItemRow = 2; // ö���� �ϳ��ۿ� ��� �ּ� ��ĭ�� ����Ѵ�.
		strRebarArr.Add(strTemp);
		dItemRowArr.Add(nCntItemRow);
		nCntRow += nCntItemRow;
		if(bSlab) 
			nRowSlab += nCntItemRow;
		else if(i < nCntItemSlab+nCntItemWall/2)
			nRowWallStt += nCntItemRow;
		else if(i < nCntItemSlab+nCntItemWall)
			nRowWallEnd += nCntItemRow;
		else if(i < nIdxSttArchRib)
			nRowArchRib += nCntItemRow;
		else
			nRowPRF += nCntItemRow;
	}

	for(i = 0; i < nCntItemInWall; i++)
	{
		CConcBasic *pData = pBri->GetConcDataInWall_Lsd(i);
		if(!pData)	continue;

		strTemp = m_pCalc->m_pSummary->GetStringMainRebarUse(pData, nCntItemRow);
		if(nCntItemRow < 2) nCntItemRow = 2; // ö���� �ϳ��ۿ� ��� �ּ� ��ĭ�� ����Ѵ�.
		vecRowInWall.push_back(nCntItemRow);
		nCntRow += nCntItemRow;
	}

//////////////////////////////////////////////////////////////////////////
	XLText->AddTable(nCntRow+2, 25-1, 3, bLsd ? 6 : 7);
	
	// Title
	CString strOpt = _T("");
	CString strUnit = (pBriStd->m_nTypeUnit == 0)? _T("��"):_T("��");
	int nWidthJoin1Temp1[] = { 7, 2, 2, 2, 2, 3, 5, 2 };
	int nWidthJoin1Temp2[] = { 6, 2, 4, 4, 3, 4, 2 };
	int nWidthJoin2Temp1[] = { 7, 2, 2, 2, 2, 3, 3, 2, 2 };
	int nWidthJoin2Temp2[] = { 6, 2, 4, 4, 3, 2, 2, 2 };
	char *Title1Temp1[] = { "�� ��", "b", "d", "Mu", "��Mn", "�ʿ�ö�ٷ�", "���ö�ٷ�", "������" };
	char *Title1Temp2[] = { "�� ��", "d", "Mu", "Mr", "As,req", "As,use", "������" };
	char *Title2Temp1[] = { "�� ��", "mm", "mm", pBriStd->m_szTonfM.GetBuffer(20), pBriStd->m_szTonfM.GetBuffer(20),
		strUnit.GetBuffer(20), "���ö��", strUnit.GetBuffer(20), "������" };
	char *Title2Temp2[] = { "�� ��", "mm", pBriStd->m_szTonfM.GetBuffer(20), pBriStd->m_szTonfM.GetBuffer(20),
		strUnit.GetBuffer(20), "���ö��", strUnit.GetBuffer(20), "������" };

	int *nWidthJoin1 = bLsd ? nWidthJoin1Temp2 : nWidthJoin1Temp1;
	int *nWidthJoin2 = bLsd ? nWidthJoin2Temp2 : nWidthJoin2Temp1;
	char **Title1 = bLsd ? Title1Temp2 : Title1Temp1;
	char **Title2 = bLsd ? Title2Temp2 : Title2Temp1;
	
	for(i=0; i< (bLsd ? 7 : 8); i++)
	{
		if(i==0 || i==(bLsd ? 6 : 7)) strOpt = "$md2";
		else strOpt = "";

		XLText->AddMergeFormatText(1,nWidthJoin1[i]-1,"$c%s%s",Title1[i], strOpt);
		XLText->AddFormatText("$m+0%d",nWidthJoin1[i]);
	}
	XLText->AddText("$n$n$h");
	for(i=0; i<(bLsd ? 8 : 9); i++)
	{
		XLText->AddMergeFormatText(0,nWidthJoin2[i]-1,"$c%s",Title2[i]);
		XLText->AddFormatText("$m+0%d",nWidthJoin2[i]);
	}
	XLText->AddText("$n$h");

	//////////////////////////////////////////////////////////////////////////
	// Data
	nCntCol = 10;
	BOOL bIsOutWall = (pBri->IsOutsideWall());

	int nWidthJoint1[] = { 3, 4, 2, 2, 2, 2, 3, 3, 2, 2 };
	int nWidthJoint2[] = { 1, 3, 2, 2, 4, 4, 3, 2, 2, 2 };
	int *nWidthJoint = bLsd ? nWidthJoint2 : nWidthJoint1;
	
	if(bIsSlab)
	{
		nWidthJoint[0] = 0;
		nWidthJoint[1] = 7;

		if (bLsd)
		{
			nWidthJoint[1] = 4;
		}
	}
	
	BOOL bExistCalcWall = FALSE;
	for(i=0; i<nCntItemSlab+nCntItemWall+nCntItemArchRib; i++)
	{
		BOOL bSlab = (i<nCntItemSlab) ? TRUE : FALSE;
		//BOOL bArchRib = (i >= (nCntItemSlab+nCntItemWall)) ? TRUE : FALSE;

		CString strEnv;
		CConcBasic *pData = NULL;
		if(bSlab)
		{
			pData = pBri->GetConcDataSlab_Lsd(i, &strEnv);
		}
		else if(i<(nCntItemSlab + nCntItemWall))
		{
			pData = pBri->GetConcDataWall_Lsd(i-nCntItemSlab, &strEnv);
		}
		else pData = pBri->m_pArrConcArchRib.GetAt(i-(nCntItemSlab + nCntItemWall));

		BOOL bCalcWall = (pData->m_nTypeSection==SECTION_WALL)? TRUE : FALSE;
		CRectBeam pBeam(pData, pBri->m_bCheckMainSlabAndFootTemper);
		CLsdBeam pBeamLsd(pData, strEnv == "����" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);
		CColumn pWall(pData);
		if(bCalcWall)
		{
			pWall.CalculateAll(FALSE);
			bExistCalcWall = TRUE;
		}

		long j=0; for(j=0; j<nCntCol; j++)
		{
			long nMerge = 0;
			if(j == 0)
			{
				if(bIsSlab) continue;
				else if(bSlab) 
				{
					str = "������";
					if(pBri->IsBoxType())
					{
						str = (i < nCntItemSlab/2 ) ? "��ν�����" : "�Ϻν�����";
					}
				}
				else if(i-nCntItemSlab<nCntItemWall / 2 && bIsOutWall) str = "������ü";
				else if(i-nCntItemSlab<nCntItemWall && bIsOutWall) str = "������ü";
				else str = "��ġ����";

				if(i==0) 
				{
					nMerge = pBri->IsBoxType() ? nRowSlab/2 - 1 : nRowSlab - 1;
				}
				else if(pBri->IsBoxType() && i == nCntItemSlab/2) nMerge = nRowSlab/2 - 1;
				else if(i==nCntItemSlab && bIsOutWall) nMerge = nRowWallStt-1;
				else if(i==nCntItemSlab+nCntItemWall/2 && bIsOutWall) nMerge = nRowWallEnd-1;
				else if((i==nCntItemSlab+nCntItemWall && bIsOutWall) || (i==nCntItemSlab && !bIsOutWall)) nMerge = nRowArchRib-1;
				else nMerge = 0;
			}
			else if(j == 1)
			{
				HGINT32 ix = pData->m_sTitle.Find(" ");
				if(ix > 0)
				{
					str = pData->m_sTitle.Mid(ix);
				}
				else
				{
					str = pData->m_sTitle;
				}

				if (bLsd)
				{
					ix = str.Find("(");
					if (ix > 0)
					{
						str.Insert(ix, "\n");
					}
				}

				strOpt = "";
			}
			else if (bLsd && j == 2)
			{
				str = strEnv;
			}
			else if(bLsd == FALSE && j == 2)
			{
				str.Format("$f1%.1f", pData->m_Bo);
				strOpt = "";
			}
			else if(j == 3)
			{
				str.Format("$f1%.1f", pData->m_Ho+pData->m_Hunch/3.-pData->GetRebarDistTension());
				strOpt = "";
			}
			else if(j == 4)
			{
				if (bLsd)
				{
					str = pBriStd->GetStrDouble(pBeamLsd.GetMu(), UNIT_CGS_TONFM);
				}
				else
				{
					if(bCalcWall) str.Format("$f2%.3f", GetValueUnitChange(pData->m_Axial,UNIT_CGS_TONF,pBriStd->m_szTonf));
					else str.Format("$f2%.3f", pBeam.GetMu());
				}
				
				strOpt = "";
			}
			else if(j == 5)
			{
				if (bLsd)
				{
					str = pBriStd->GetStrDouble(pBeamLsd.GetMn(), UNIT_CGS_TONFM);
				}
				else
				{
					if(bCalcWall) str.Format("$f2%.3f", GetValueUnitChange(pWall.GetPiPn(),UNIT_CGS_TONF,pBriStd->m_szTonf));
					else str.Format("$f2%.3f", pBeam.GetPiMn());
				}
				
				strOpt = "";
			}
			else if(j == 6)
			{
				double dArea = bLsd ? pBeamLsd.GetAsReq() : pBeam.GetRebarAreaRequired(); 
				
				if(bCalcWall) str = ("-");
				else str.Format("$f%d%.3f", nDec, dArea);
				strOpt = "";

				if (bLsd && pBeamLsd.IsSingleBeam() == FALSE)
				{
					str = "-";
				}
			}
			else if(j == 7)
			{
				str = strRebarArr.GetAt(i);
				strOpt = "";
			}
			else if(j == 8)
			{
				double dArea = pBriStd->IsSI() ? frCm2(pData->GetRebarAreaTension())
					                           : pData->GetRebarAreaTension(); 
				str.Format("$f%d%.3f", nDec, dArea);
				strOpt = "";
			}
			else
			{
				if(Compare(pData->m_Moment, 0.0, _T("="), 0.01))
				{
					str = "-";
				}
				else if (bLsd)
				{
					if (pBeamLsd.IsSingleBeam() == FALSE)
					{
						str = "-";
					}
					else
					{
						str.Format("$f3%.3f", pBeamLsd.GetMn() / pBeamLsd.GetMu());
					}
				}
				else
				{
					if(bCalcWall) str.Format("$f3%.3f", pWall.GetSafeFactorAxial());
					else str.Format("$f3%.3f", pBeam.GetSafeFactorMoment());
				}
				
				strOpt = "";
			}

			if(j > (bLsd ? 2 : 1) && (bLsd ? pBeamLsd.GetMu() : pBeam.GetMu()) <= 0.0) str = _T("-");
			if (bLsd && j == 1 && icValidEnvType > 1 && i % icValidEnvType == 0)
			{
				HGINT32 icMerge = 0;
				for (HGINT32 ix = 0; ix < icValidEnvType; ++ix)
				{
					icMerge += static_cast<HGINT32>(dItemRowArr.GetAt(i+ix));
				}

				XLText->AddMergeFormatText(icMerge - 1, nWidthJoint[j]-1, "$j$c%s", str);
			}
			else
			{
				XLText->AddMergeFormatText(static_cast<long>(dItemRowArr.GetAt(i)-1), nWidthJoint[j]-1, "$j$c%s", str);
			}

			XLText->Merge(nMerge, 0);
			XLText->AddFormatText("$m+0%d", nWidthJoint[j]);
		}
		*XLText += (long)(dItemRowArr.GetAt(i) - 1);
		XLText->AddText("$n$h");
	}
	
	for(i=0; i<nCntItemInWall; i++)
	{
		CString strEnv;
		CConcBasic *pData = pBri->GetConcDataInWall_Lsd(i, &strEnv);
		if(!pData)	continue;

		BOOL bCalcWall = (pData->m_nTypeSection==SECTION_WALL)? TRUE : FALSE;
		CRectBeam pBeam(pData, pBri->m_bCheckMainSlabAndFootTemper);
		CLsdBeam pBeamLsd(pData, strEnv == "����" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);
		CColumn pWall(pData);
		if(bCalcWall)
		{
			pWall.CalculateAll(FALSE);
			bExistCalcWall = TRUE;
		}

		strTemp = m_pCalc->m_pSummary->GetStringMainRebarUse(pData, nCntItemRow);
		if(nCntItemRow < 2) nCntItemRow = 2; // ö���� �ϳ��ۿ� ��� �ּ� ��ĭ�� ����Ѵ�.
		
		long j=0; for(j=0; j<nCntCol; j++)
		{
			if(j == 0)
			{
				long nIdx = pData->m_sTitle.Find(" ");
				if(nIdx > 0)
					str = pData->m_sTitle.Left(nIdx);
				else
					str = pData->m_sTitle.Left(9);
			}
			else if(j == 1)
			{
				str = pData->m_sTitle.Right(10);
			}
			else if (bLsd && j == 2)
			{
				str = strEnv;
			}
			else if(bLsd == FALSE && j == 2)
			{
				str.Format("$f1%.1f", pData->m_Bo);
			}
			else if(j == 3)
			{
				str.Format("$f1%.1f", pData->m_Ho+pData->m_Hunch/3.-pData->GetRebarDistTension());
			}
			else if(j == 4)
			{
				if (bLsd)
				{
					str = pBriStd->GetStrDouble(pBeamLsd.GetMu(), UNIT_CGS_TONFM);
				}
				else
				{
					if(bCalcWall) str.Format("$f2%.3f", GetValueUnitChange(pData->m_Axial,UNIT_CGS_TONF,pBriStd->m_szTonf));
					else str.Format("$f2%.3f", pBeam.GetMu());
				}
			}
			else if(j == 5)
			{
				if (bLsd)
				{
					str = pBriStd->GetStrDouble(pBeamLsd.GetMn(), UNIT_CGS_TONFM);
				}
				else
				{
					if(bCalcWall) str.Format("$f2%.3f", GetValueUnitChange(pWall.GetPiPn(),UNIT_CGS_TONF,pBriStd->m_szTonf));
					else str.Format("$f2%.3f", pBeam.GetPiMn());
				}
			}
			else if(j == 6)
			{
				double dArea = bLsd ? pBeamLsd.GetAsReq() :  pBeam.GetRebarAreaRequired(); 
				
				if(bCalcWall) str = ("-");
				else str.Format("$f%d%.3f", nDec, dArea);
			}
			else if(j == 7)
			{
				str = strTemp;
			}
			else if(j == 8)
			{
				double dArea = pBriStd->IsSI() ? frCm2(pData->GetRebarAreaTension())
					                           : pData->GetRebarAreaTension(); 
				str.Format("$f%d%.3f", nDec, dArea);
			}
			else
			{
				if(Compare(pData->m_Moment, 0.0, _T("="), 0.01))
				{
					str = "-";
				}
				else if (bLsd)
				{
					str.Format("$f3%.3f", pBeamLsd.GetMn() / pBeamLsd.GetMu());
				}
				else
				{
					if(bCalcWall) str.Format("$f3%.3f", pWall.GetSafeFactorAxial());
					else str.Format("$f3%.3f", pBeam.GetSafeFactorMoment());
				}
			}

			if(j > (bLsd ? 2 : 1) && (bLsd ? pBeamLsd.GetMu() : pBeam.GetMu()) <= 0.0) str = _T("-");
			if (bLsd && j == 1 && icValidEnvType > 1 && i % icValidEnvType == 0)
			{
				HGINT32 icMerge = accumulate(vecRowInWall.begin() + i, vecRowInWall.begin() + (i+icValidEnvType), 0);

				XLText->AddMergeFormatText(icMerge - 1, nWidthJoint[j]-1, "$j$c%s", str);
			}
			else
			{
				XLText->AddMergeFormatText(nCntItemRow-1, nWidthJoint[j]-1, "$j$c%s", str);
			}
			
			if(i % (6 * icValidEnvType) == 0 && j == 0)
				XLText->Merge(6 * icValidEnvType * nCntItemRow - 1, 0);
			XLText->AddFormatText("$m+0%d", nWidthJoint[j]);
		}
		*XLText += nCntItemRow-1;
		XLText->AddText("$n$h");
	}

	for(i=0; i<nCntItemPRF; i++)
	{
		CString strEnv;
		CConcBasic *pData = pBri->GetConcDataPRF_Lsd(i, &strEnv);
		if(pData == NULL) continue;

		CRectBeam pBeam(pData, pBri->m_bCheckMainSlabAndFootTemper);
		CLsdBeam pBeamLsd(pData, strEnv == "����" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);
		CColumn pWall(pData);
		
		for(long j=0; j<nCntCol; j++)
		{
			long nMerge = 1;
			if(j == 0)
			{
				str = _T("�λ��������");
				if(i == 0)
					nMerge = nRowPRF-1;
				else
					nMerge = 0;
			}
			else if(j == 1)
			{
				HGINT32 ix = pData->m_sTitle.Find(" ");
				str = pData->m_sTitle.Mid(ix);
			}
			else if(j == 2)
			{
				if(bLsd)
					str = strEnv;
				else
					str.Format("$f1%.1f", pData->m_Bo);
			}			
			else if(j == 3)
			{
				str.Format("$f1%.1f", pData->m_Ho+pData->m_Hunch/3.-pData->GetRebarDistTension());
			}
			else if(j == 4)
			{
				if (bLsd)
					str = pBriStd->GetStrDouble(pBeamLsd.GetMu(), UNIT_CGS_TONFM);
				else
					str.Format("$f2%.3f", pBeam.GetMu());
			}
			else if(j == 5)
			{
				if(bLsd)
					str = pBriStd->GetStrDouble(pBeamLsd.GetMn(), UNIT_CGS_TONFM);
				else
					str.Format("$f2%.3f", pBeam.GetPiMn());
			}
			else if(j == 6)
			{
				double dArea = bLsd ? pBeamLsd.GetAsReq() : pBeam.GetRebarAreaRequired(); 				
				str.Format("$f%d%.3f", nDec, dArea);

				if(bLsd && pBeamLsd.IsSingleBeam() == FALSE)
				{
					str = "-";
				}
			}
			else if(j == 7)
			{
				str = strRebarArr.GetAt(nCntItemSlab+nCntItemWall+nCntItemArchRib);
			}
			else if(j == 8)
			{
				double dArea = pBriStd->IsSI() ? frCm2(pData->GetRebarAreaTension())
					: pData->GetRebarAreaTension(); 
				str.Format("$f%d%.3f", nDec, dArea);
			}
			else
			{
				if(Compare(pData->m_Moment, 0.0, _T("="), 0.01))
					str = "-";
				else if(bLsd)
				{
					if(pBeamLsd.IsSingleBeam() == FALSE)
						str = "-";
					else
						str.Format("$f3%.3f", pBeamLsd.GetMn() / pBeamLsd.GetMu());
				}
				else
					str.Format("$f3%.3f", pBeam.GetSafeFactorMoment());
			}

			if(j > (bLsd ? 2 : 1) && (bLsd ? pBeamLsd.GetMu() : pBeam.GetMu()) <= 0.0) str = _T("-");
			if (bLsd && j == 1 && icValidEnvType > 1 && i % icValidEnvType == 0)
			{
				HGINT32 icMerge = 0;
				for(HGINT32 ix = 0; ix < icValidEnvType; ++ix)
				{
					icMerge += static_cast<HGINT32>(dItemRowArr.GetAt(i+ix));
				}

				XLText->AddMergeFormatText(icMerge - 1, nWidthJoint[j]-1, "$j$c%s", str);
			}
			else
				XLText->AddMergeFormatText(nMerge, nWidthJoint[j]-1, "$j$c%s", str);

			XLText->AddFormatText("$m+0%d", nWidthJoint[j]);
			XLText->OutXL();
		}
		*XLText += (long)(dItemRowArr.GetAt(i) - 1);
		XLText->AddText("$n$h");
	}

	if(bExistCalcWall)
	{
		XLText->AddText("$h* ��ü�δ� ��°��� ����մϴ�.$n");
	}
}

void COutDesignBridge::MakeTableShearRebarSummary(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	CARcBridgeDataStd *pBriStd = pBri->m_pARcBridgeDataStd;
	
	BOOL bIsSlab = (pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)? TRUE:FALSE;
	CString str = _T("");
	long nCntRow = 0;
	long nCntCol = 0;

	long nCntInWall = (pBri->m_nSelectInWallSectionDesign == 0)? pBri->GetCountInWall() : 0;
	long nCntItemSlab = pBri->m_pArrConcSlab[eEnvLimit].GetSize();
	long nCntItemWall = (pBri->IsOutsideWall())? MIN(6, pBri->m_pArrConcWall[eEnvLimit].GetSize()):0;
	long nCntItemInWall = MIN(nCntInWall*3, pBri->m_pArrConcInWall[eEnvLimit].GetSize());
	long nCntItemArchRib = pBri->m_pArrConcArchRib.GetSize();
	long nCntItemPRF = pBri->m_bExistProtectionRiseFooting ? 2 : 0;
	nCntRow = nCntItemSlab + nCntItemWall + nCntItemInWall + nCntItemArchRib + nCntItemPRF;

	XLText->AddTable(nCntRow*2+1*2, 25-1, 3, 7);
	
	long nSttIdxArchRib = nCntItemSlab + nCntItemWall + nCntItemInWall;
	long nSttIdxPRF = nCntItemSlab + nCntItemWall + nCntItemInWall + nCntItemArchRib;

	// Title
	CString strOpt = _T("");
	CString strUnit = (pBriStd->m_nTypeUnit == 0)? _T("��"):_T("��");
	int nWidthJoin1[] = { 7, 2, 2, 2, 2, 2, 6, 2 };
	int nWidthJoin2[] = { 7, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
	char *Title1[] = { "�� ��", "��Vc", "Vu", "��Vn", "�ٸ���", "�ʿ�\nö�ٷ�", "���ö�ٷ�", "�� ��" };
	char *Title2[] = { "�� ��", pBriStd->m_szTonf.GetBuffer(20), pBriStd->m_szTonf.GetBuffer(20), 
		pBriStd->m_szTonf.GetBuffer(20), "��",
		strUnit.GetBuffer(20), "����", "����", strUnit.GetBuffer(20), "�� ��" };
	
	int i=0; for(i=0; i<8; i++)
	{
		if(i==0 || i==7) strOpt = "$md2";
		else strOpt = "";
		
		XLText->AddMergeFormatText(1,nWidthJoin1[i]-1,"$c%s%s",Title1[i], strOpt);
		XLText->AddFormatText("$m+0%d",nWidthJoin1[i]);
	}
	XLText->AddText("$n$n$h");
	for(i=0; i<10; i++)
	{
		XLText->AddMergeFormatText(0,nWidthJoin2[i]-1,"$c%s",Title2[i]);
		XLText->AddFormatText("$m+0%d",nWidthJoin2[i]);
	}
	XLText->AddText("$n$h");

	//////////////////////////////////////////////////////////////////////////
	// Data
	BOOL bIsOutWall = (pBri->IsOutsideWall());
	nCntCol = 11;
	int nWidthJoint[] = { 3, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
	if(bIsSlab)
	{
		nWidthJoint[0] = 0;
		nWidthJoint[1] = 7;
	}
	for(i=0; i<nCntRow; i++)
	{
		BOOL bSlab = (i < nCntItemSlab) ? TRUE : FALSE;
		BOOL bArchRib = (nCntItemArchRib > 0 && i>=nSttIdxArchRib) ? TRUE : FALSE;
		BOOL bWall = FALSE;
		
		CConcBasic *pData = NULL;
		if(bSlab) 
			pData = pBri->m_pArrConcSlab[eEnvLimit].GetAt(i);
		else if(i < (nCntItemSlab + nCntItemWall))
		{
			pData = pBri->GetConcDataWall(i-nCntItemSlab, FALSE);
			bWall = TRUE;
		}
		else if(i < nSttIdxArchRib)
		{
			long nIndex = i-(nCntItemSlab + nCntItemWall);
			if(pBri->m_pArrConcInWall[eEnvLimit].GetSize() > nIndex)
				pData = pBri->m_pArrConcInWall[eEnvLimit].GetAt(nIndex*2);
			bWall = FALSE;
		}
		else if(i < nSttIdxPRF)
		{
			pData = pBri->m_pArrConcArchRib.GetAt(i-nSttIdxArchRib);
		}
		else
		{
			pData = pBri->GetConcDataPRF((i-nSttIdxPRF == 0) ? TRUE : FALSE);
		}
		
		if(bWall && pBri->m_nSelectWallSectionDesign==1)
		{
			CColumn pWall(pData);
			pWall.CalculateAll(FALSE);
			
			long nCheckShearForce =  pWall.GetCheckShearForce();

			long j=0; for(j=0; j<nCntCol; j++)
			{
				long nMerge = 1;
				if(j == 0)
				{
					if(i-nCntItemSlab<3 && bIsOutWall) 
						str = "������ü";
					else if(i-nCntItemSlab<6 && bIsOutWall) 
						str = "������ü";
					else if(i<nSttIdxArchRib)
					{
						long nCol = (i-(nCntItemSlab+nCntItemWall))/3;
						str.Format("�߰���ü%d", nCol+1);
					}
					else if(i<nSttIdxPRF)
						str = "��ġ����";
					
					if(i==0) nMerge = (nCntItemSlab)*2-1;
					else if(i==nCntItemSlab && bIsOutWall) nMerge = 5;
					else if(i==nCntItemSlab+3 && bIsOutWall) nMerge = 5;
					else if(i==nCntItemSlab+nCntItemWall && nCntItemInWall > 0) nMerge = nCntItemInWall * 2 - 1;
					else if((i==nSttIdxArchRib && bIsOutWall) || (i==nCntItemSlab && !bIsOutWall)) nMerge = nCntItemArchRib*2-1;
					else strOpt = "";
				}
				else if(j == 1)
				{
					if(bSlab || bArchRib) str = pData->m_sTitle;
					else
					{
//						CString strTemp = _T("");
						CString strTemp = pData->m_sTitle.Mid(9);
						if(strTemp.Right(1) == ")") str = strTemp.Left(4) + "��";
						else str = strTemp;
					}
					strOpt = "";
				}
				else if(j == 2)
				{
					str.Format("$f2%.3f", pWall.GetVc()*pData->m_Pi_V);
					strOpt = "";
				}
				else if(j == 3)
				{
					str.Format("$f2%.3f", pWall.GetVu());
					strOpt = "";
				}
				else if(j == 4)
				{
					str.Format("$f2%.3f", pWall.GetPiVn());
					strOpt = "";
				}
				else if(j == 5)
				{
					str.Format("$f2%.3f", pData->m_SvEa);
					strOpt = "";
				}
				else if(j == 6)
				{
					double AvReq = 0;
					long n = (pBriStd->IsSI())? 1:3;
					
					if(nCheckShearForce == 7) 
						AvReq = -1;
					else
						AvReq = pWall.GetMinAv();
					
					if(AvReq > 0) str.Format("$f%d%.3f",n, AvReq);
					else str = _T("-");
					strOpt = "";
				}
				else if(j == 7)
				{
					str = pBriStd->GetStringDia(pData->m_SvDia, pData->m_FyShear);
					strOpt = "";
				}
				else if(j == 8)
				{
					str.Format("$f0%.3f", pData->m_SvCtc);
					strOpt = "";
				}
				else if(j == 9)
				{
					double dArea = (pBriStd->IsSI()) ? frCm2(pData->GetAreaRebar(pData->m_SvDia, pData->m_SvEa)) 
						: pData->GetAreaRebar(pData->m_SvDia, pData->m_SvEa);
					long n = (pBriStd->IsSI())? 1 : 3;
					str.Format("$f%d%.3f", n, dArea);
					strOpt = "";
				}
				else if(j == 10)
				{
					str = (nCheckShearForce == 0 || nCheckShearForce == 4 || nCheckShearForce == 6)? _T("O.K"):_T("N.G");
					if(nCheckShearForce == 7) str = "-";
					strOpt = "";
				}
				else
				{
					str = "";
					strOpt = "";
				}
				XLText->AddMergeFormatText(nMerge, nWidthJoint[j]-1, "$c%s%s", str, strOpt);
				XLText->AddFormatText("$m+0%d", nWidthJoint[j]);
			}
		}
		else
		{
			CRectBeam pBeam(pData, pBri->m_bCheckMainSlabAndFootTemper);		
			
			long nCheckShearForce =  pBeam.GetCheckShearForce();
			for(long j=0; j<nCntCol; j++)
			{
				long nMerge = 1;
				if(j == 0)
				{
					if(bIsSlab) continue;
					else if(bSlab) 
					{
						str = "������";
						if(pBri->IsBoxType())
						{
							str = (i < nCntItemSlab/2 ) ? "��ν�����" : "�Ϻν�����";
						}
					}
					else if(i-nCntItemSlab<3 && bIsOutWall) 
						str = "������ü";
					else if(i-nCntItemSlab<6 && bIsOutWall) 
						str = "������ü";
					else if(i<nSttIdxArchRib)
					{
						long nCol = (i-(nCntItemSlab+nCntItemWall))/3;
						str.Format("�߰���ü%d", nCol+1);
					}
					else if(i<nSttIdxPRF)
						str = "��ġ����";
					else
						str = "�λ��������";

					if(i==0) 
					{
						nMerge = pBri->IsBoxType() ? nCntItemSlab - 1 : (nCntItemSlab)*2 - 1;
					}
					else if(pBri->IsBoxType() && i == nCntItemSlab/2) nMerge = nCntItemSlab - 1;
					else if(i==nCntItemSlab && bIsOutWall) nMerge = 5;
					else if(i==nCntItemSlab+3 && bIsOutWall) nMerge = 5;
					else if(nCntItemInWall > 0 && i>=(nCntItemSlab+nCntItemWall) && i<nSttIdxArchRib)
					{
						long ncol=0; for(ncol=0; ncol<pBri->GetCountInWall(); ncol++)
						{
							if(i==nCntItemSlab+nCntItemWall + (3 * ncol)) nMerge = 5;
						}
					}
					else if(i == nSttIdxPRF)
						nMerge = 3;
					else if((i==nSttIdxArchRib && bIsOutWall) || (i==nCntItemSlab && !bIsOutWall)) nMerge = nCntItemArchRib*2-1;
					else strOpt = "";
				}
				else if(j == 1)
				{
					if(bSlab || bArchRib) 
					{
						str = pData->m_sTitle;
						if(pBri->IsBoxType())
						{
							str = pData->m_sTitle.Right(pData->m_sTitle.GetLength() - 11);
						}
					}
					else
					{
						CString strTemp = _T("");
						if(bWall) 
							strTemp = pData->m_sTitle.Mid(9);
						else
						{
							if(i >= nCntItemPRF)
								strTemp = pData->m_sTitle.Mid(12);
							else
								strTemp = pData->m_sTitle.Mid(10);
						}
						
						if(strTemp.Right(1) == ")") str = strTemp.Left(4) + "��";
						else str = strTemp;
					}
					strOpt = "";
				}
				else if(j == 2)
				{
					str.Format("$f2%.3f", pBeam.GetVc()*pData->m_Pi_V);
					strOpt = "";
				}
				else if(j == 3)
				{
					str.Format("$f2%.3f", pBeam.GetVu());
					strOpt = "";
				}
				else if(j == 4)
				{
					str.Format("$f2%.3f", pBeam.GetPiVn());
					strOpt = "";
				}
				else if(j == 5)
				{
					str.Format("$f2%.3f", pData->m_SvEa);
					strOpt = "";
				}
				else if(j == 6)
				{
					double AvReq = 0;
					long n = (pBriStd->IsSI())? 1:3;
					
					if(nCheckShearForce == 6) 
						AvReq = -1;
					else if(nCheckShearForce == 1 || nCheckShearForce == 2)
						AvReq = pBeam.GetMinAv();
					else
						AvReq = pBeam.GetAvReq();
					
					if(AvReq > 0) str.Format("$f%d%.3f",n, AvReq);
					else str = _T("-");
					strOpt = "";
				}
				else if(j == 7)
				{
					str = pBriStd->GetStringDia(pData->m_SvDia, pData->m_FyShear);
					strOpt = "";
				}
				else if(j == 8)
				{
					str.Format("$f0%.3f", pData->m_SvCtc);
					strOpt = "";
				}
				else if(j == 9)
				{
					double dArea = (pBriStd->IsSI()) ? frCm2(pData->GetAreaRebar(pData->m_SvDia, pData->m_SvEa)) 
						: pData->GetAreaRebar(pData->m_SvDia, pData->m_SvEa);
					long n = (pBriStd->IsSI())? 1 : 3;
					str.Format("$f%d%.3f", n, dArea);
					strOpt = "";
				}
				else if(j == 10)
				{
					str = (nCheckShearForce == 0 || nCheckShearForce == 2 || nCheckShearForce == 6)? _T("O.K"):_T("N.G");
					strOpt = "";
				}
				else
				{
					str = "";
					strOpt = "";
				}
				XLText->AddMergeFormatText(nMerge, nWidthJoint[j]-1, "$c%s%s", str, strOpt);
				XLText->AddFormatText("$m+0%d", nWidthJoint[j]);

				XLText->OutXL();
			}
		}
		*XLText += 1;
		XLText->AddText("$n$h");
	}
}

void COutDesignBridge::MakeTableShearRebarSummaryLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	CARcBridgeDataStd *pBriStd = pBri->m_pARcBridgeDataStd;

	BOOL bIsSlab = (pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)? TRUE:FALSE;
	CString str = _T("");
	long nCntRow = 0;
	long nCntCol = 0;

	const HGINT32 icValidEnvType = pBri->GetCountValidComboEnvType();
	long nCntInWall = (pBri->m_nSelectInWallSectionDesign == 0)? pBri->GetCountInWall() : 0;
	long nCntItemSlab = pBri->GetCountConcSlab();
	long nCntItemWall = pBri->m_nSelectWallSectionDesign==1 ? 0 : ((pBri->IsOutsideWall())? 6 * icValidEnvType:0);
	long nCntItemInWall = nCntInWall * 3 * icValidEnvType;
	long nCntItemArchRib = pBri->m_pArrConcArchRib.GetSize();
	long nCntItemPRF = pBri->GetCountConcPRF();
	nCntRow = nCntItemSlab + nCntItemWall + nCntItemInWall + nCntItemArchRib + nCntItemPRF;

	XLText->AddTable(nCntRow*2+1*2, 25-1, 3, 6);

	// Title
	CString strOpt = _T("");
	CString strUnit = (pBriStd->m_nTypeUnit == 0)? _T("cm"):_T("mm");
	int nWidthJoin1[] = { 6, 3, 3, 3, 2, 2, 2, 2, 2 };
	int nWidthJoin2[] = { 6, 3, 3, 3, 2, 2, 2, 2, 2 };
	char *Title1[] = { "�� ��", "Vcd", "Vsd", "Vu", "pv.min", "pv.use", "S_max", "S", "�� ��" };
	char *Title2[] = { "�� ��", pBriStd->m_szTonf.GetBuffer(20), pBriStd->m_szTonf.GetBuffer(20), 
		pBriStd->m_szTonf.GetBuffer(20), "", "", strUnit.GetBuffer(20), strUnit.GetBuffer(20), "�� ��" };

	int i=0; for(i=0; i<9; i++)
	{
		if(i==0 || i==4 || i==5 || i==8) strOpt = "$md2";
		else strOpt = "";

		XLText->AddMergeFormatText(1,nWidthJoin1[i]-1,"$c%s%s",Title1[i], strOpt);
		XLText->AddFormatText("$m+0%d",nWidthJoin1[i]);
	}
	XLText->AddText("$n$n$h");
	for(i=0; i<9; i++)
	{
		XLText->AddMergeFormatText(0,nWidthJoin2[i]-1,"$c%s",Title2[i]);
		XLText->AddFormatText("$m+0%d",nWidthJoin2[i]);
	}
	XLText->AddText("$n$h");

	//////////////////////////////////////////////////////////////////////////
	// Data
	BOOL bIsOutWall = (pBri->IsOutsideWall());
	nCntCol = 11;
	int nWidthJoint[] = { 1, 3, 2, 3, 3, 3, 2, 2, 2, 2, 2 };
	if(bIsSlab)
	{
		nWidthJoint[0] = 0;
		nWidthJoint[1] = 4;
	}
	
	for(i=0; i<nCntRow; i++)
	{
		BOOL bSlab = (i<nCntItemSlab)? TRUE:FALSE;
		BOOL bArchRib = (nCntItemArchRib > 0 && i>=(nCntItemSlab + nCntItemWall + nCntItemInWall))? TRUE:FALSE;
		BOOL bWall = FALSE;
		BOOL bPRF = FALSE;

		CString strEnv;
		CConcBasic *pData = NULL;
		if(bSlab ) pData = pBri->GetConcDataSlab_Lsd(i, &strEnv);
		else if(i<(nCntItemSlab + nCntItemWall))
		{
			pData = pBri->GetConcDataWall_Lsd((i-nCntItemSlab), &strEnv, FALSE);
			bWall = TRUE;
		}
		else if(i<(nCntItemSlab + nCntItemWall + nCntItemInWall))
		{
			long nIndex = i-(nCntItemSlab + nCntItemWall);
			pData = pBri->GetConcDataInWall_Lsd(nIndex, &strEnv);
			bWall = FALSE;
		}
		else if(i<(nCntItemSlab + nCntItemWall + nCntItemInWall + nCntItemArchRib))
			pData = pBri->m_pArrConcArchRib.GetAt(i-(nCntItemSlab + nCntItemWall + nCntItemInWall));
		else
		{
			pData = pBri->GetConcDataPRF_Lsd(i-(nCntItemSlab + nCntItemWall + nCntItemInWall + nCntItemArchRib), &strEnv);
			bPRF = TRUE;
		}

		BOOL bDesignShear = (pBri->m_bCalcShearCenter == FALSE && pData->m_sTitle.Find(_T("�߾�")) != -1)? FALSE : TRUE;
		CLsdBeam pBeam(pData, strEnv == "����" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);

		const HGBOOL bNeedShear = pBeam.IsNeedShearRebar();
		HGBOOL bMinAv = (pBeam.m_pData->m_bMinAvCheck || bNeedShear);
		BOOL bVsd = (bNeedShear == FALSE || pBeam.IsOk_Vsd()) ? TRUE : FALSE;
		BOOL bPvMin = (bMinAv == FALSE || pBeam.IsOk_pvMin()) ? TRUE : FALSE ;
		BOOL bSvCtc = (bMinAv == FALSE || pBeam.IsOk_SvCtc()) ? TRUE : FALSE ;
		const HGBOOL bShearOk = (bVsd && bPvMin && bSvCtc) ? TRUE : FALSE;

		long j=0; for(j=0; j<nCntCol; j++)
		{
			long nMerge = 1;
			if(j == 0)
			{
				if(bIsSlab) continue;
				else if(bSlab) 
				{
					str = "������";
					if(pBri->IsBoxType())
					{
						str = (i < nCntItemSlab/2 ) ? "��ν�����" : "�Ϻν�����";
					}
				}
				else if(i-nCntItemSlab<3*icValidEnvType && bIsOutWall && bWall) str = "������ü";
				else if(i-nCntItemSlab<6*icValidEnvType && bIsOutWall && bWall) str = "������ü";
				else if(i<(nCntItemSlab+nCntItemWall+nCntItemInWall))
				{
					long nCol = (i-(nCntItemSlab+nCntItemWall))/(3*icValidEnvType);
					str.Format("�߰���ü%d", nCol+1);
				}
				else if(i<(nCntItemSlab+nCntItemWall+nCntItemInWall+nCntItemArchRib))
					str = "��ġ����";
				else
					str = "�λ��������";

				if(i==0) 
				{
					nMerge = pBri->IsBoxType() ? nCntItemSlab - 1 : (nCntItemSlab)*2 - 1;
				}
				else if(pBri->IsBoxType() && i == nCntItemSlab/2) 
					nMerge = nCntItemSlab - 1;
				else if(i==nCntItemSlab && bIsOutWall && bWall) 
					nMerge = 3*icValidEnvType * 2 - 1;
				else if(i==nCntItemSlab+(3*icValidEnvType) && bIsOutWall && bWall)
					nMerge = 3*icValidEnvType * 2 - 1;
				else if(nCntItemInWall > 0 && i>=(nCntItemSlab+nCntItemWall) && i<(nCntItemSlab+nCntItemWall+nCntItemInWall))
				{
					long ncol=0; for(ncol=0; ncol<pBri->GetCountInWall(); ncol++)
					{
						if(i==nCntItemSlab+nCntItemWall + (3 * icValidEnvType * ncol)) nMerge = 3*icValidEnvType * 2 - 1;
					}
				}
				else if(((i == nCntItemSlab+nCntItemWall+nCntItemInWall && bIsOutWall) || (i==nCntItemSlab && !bIsOutWall)) && nCntItemArchRib > 0) 
					nMerge = nCntItemArchRib*2-1;
				else 
				{
					if(i == nCntItemSlab+nCntItemWall+nCntItemInWall+nCntItemArchRib)
						nMerge = 2*icValidEnvType*2-1;
					else
						nMerge = 0;
				}
				strOpt = "";
			}
			else if(j == 1)
			{
				if(bSlab || bArchRib) 
				{
					str = pData->m_sTitle;
					if(pBri->IsBoxType())
					{
						str = pData->m_sTitle.Right(pData->m_sTitle.GetLength() - 11);
					}
				}
				else
				{
					CString strTemp = _T("");
					if(bWall) 
						strTemp = pData->m_sTitle.Mid(9);
					else if(bPRF)
						strTemp = pData->m_sTitle.Mid(12);
					else 
						strTemp = pData->m_sTitle.Mid(10);
					
					if(strTemp.Right(1) == ")") 
						str = strTemp.Left(4) + "��";
					else 
						str = strTemp;
				}
				
				HGINT32 ix = str.Find("(");
				if (ix > 0)
				{
					str.Insert(ix, "\n");
				}

				strOpt = "";
			}
			else if(j == 2)
			{
				str = strEnv;
				strOpt = "";
			}
			else if(bDesignShear == FALSE)
			{
				str = _T("-");
			}
			else if(j == 3)
			{
				str = pBriStd->GetStrDouble(pBeam.GetVcd(), UNIT_CGS_TONF);
				strOpt = "";
			}
			else if(j == 4)
			{
				if(pBeam.IsNeedShearRebar())
					str = pBriStd->GetStrDouble(pBeam.GetVd(), UNIT_CGS_TONF);
				else
					str = _T("-");
				strOpt = "";
			}
			else if(j == 5)
			{
				str = pBriStd->GetStrDouble(pBeam.GetVu(), UNIT_CGS_TONF);
				strOpt = "";
			}
			else if(j == 6)
			{
				if(bMinAv)
					str.Format("%.5f", pBeam.GetpvMin());
				else
					str = _T("-");
				strOpt = "";
			}
			else if(j == 7)
			{
				if(bMinAv)
					str.Format("%.5f", pBeam.GetpvUse());
				else
					str = _T("-");
				strOpt = "";
			}
			else if(j == 8)
			{
				if(bMinAv)
					str = pBriStd->GetStrDouble(pBeam.GetSMax(), UNIT_CGS_M);
				else
					str = _T("-");
				strOpt = "";
			}
			else if(j == 9)
			{
				if(bMinAv)
					str = pBriStd->GetStrDouble(pBeam.GetSvCtc(), UNIT_CGS_M);
				else
					str = _T("-");
				strOpt = "";
			}
			else if(j == 10)
			{
				str = bShearOk ? _T("O.K"):_T("N.G");
				strOpt = "";
			}
			else
			{
				str = "";
				strOpt = "";
			}

			if (j == 1 && icValidEnvType > 1 && i % icValidEnvType == 0)
			{
				XLText->AddMergeFormatText(2 * icValidEnvType - 1, nWidthJoint[j]-1, "$j$c%s", str);
			}
			else
			{
				if (str == "0")
				{
					str = "-";
				}

				XLText->AddMergeFormatText(nMerge, nWidthJoint[j]-1, "$j$c%s%s", str, strOpt);
			}

			XLText->AddFormatText("$m+0%d", nWidthJoint[j]);
			XLText->OutXL();
		}
		
		*XLText += 1;
		XLText->AddText("$n$h");
	}
}

void COutDesignBridge::MakeDiagramBMD_SFD(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	CFEMManage *pFemNormal = &pBri->m_FemNormal;
	CFEMManage *pFemEQ1    = pBri->GetFemManage(COMBO_EARTHQUAKE);
	CFEMManage *pFemEQ2    = pBri->GetFemManage(COMBO_EARTHQUAKE2);

	BOOL bIsSlab = pBri->IsRcSlab();
	double dScaleDim = 200;
	CDomyun *pDomMain = new CDomyun;
	pDomMain->SetCWnd(m_pStd->m_pView);
	pDomMain->SetScaleDim(dScaleDim);
	pDomMain->SetLTScale(dScaleDim*5);

	long nIndexMiddle = 1;
	long nIndexSmall  = 1;
	long nSizeRow = bIsSlab ? 35 : 15;

	CDRect rc;
	XLText->GoPosition(NO_MOVE,COL_POS_1);

	if(pFemNormal->IsOutputLoaded())
	{
		XLText->AddFormatText("$h$c%d)$r���� �������$n$n",nIndexMiddle++);
		if(bIsSlab) XLText->AddFormatText("$h$m+04$c%s$rB.M.D$n$n",GetNumberString(nIndexSmall++,NUM_CIRCLE));
		else XLText->AddFormatText("$h$r$c%s$rB.M.D$n$n",GetNumberString(nIndexSmall++,NUM_CIRCLE));

		CDomyun *pDom1 = new CDomyun(pDomMain);
		CForceDiagram ForceDiagram(pFemNormal);
		ForceDiagram.SetScaleDim(pDomMain->GetScaleDim()/1000.0);
		long nIdx = pFemNormal->GetLoadCaseByString("ENVULT MAX");	
		if(nIdx > -1)
		{
			ForceDiagram.DrawDiagramForce(nIdx, ELE_MOMENT2,FALSE,m_pStd->m_pARoadOptionStd);
			rc = ForceDiagram.GetExtRect();
			ForceDiagram.Point(rc.TopLeft().x-2, rc.TopLeft().y+2);
			ForceDiagram.Point(rc.BottomRight().x+2, rc.BottomRight().y+2);
			if(bIsSlab) ForceDiagram.Rotate(ConstPi/2*3,FALSE);
			*pDom1 << ForceDiagram;
			if(bIsSlab)
			{
				XLText->AddFormatText("$h$m+04");
				XLText->InsertDomyun(pDom1,m_pStd->m_pView,nSizeRow,CXLFormatText:: INSERT_DOM_CURPOS&~CXLFormatText::INSERT_DOM_MOVEROW);
			}
			else 
			{
				CDRect rc1 = pDom1->GetExtRect();
				if(nSizeRow * rc1.Width()/rc1.Height() > 32) nSizeRow = (long)(32*rc1.Height()/rc1.Width());
				XLText->InsertDomyun(pDom1,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_MOVEROW);
			}
			*XLText += 1;
			
			if(bIsSlab) XLText->AddFormatText("$u$u$u$h$m+15$c%s$rS.F.D$n$n",GetNumberString(nIndexSmall++,NUM_CIRCLE));
			else XLText->AddFormatText("$h$r$c%s$rS.F.D$n$n",GetNumberString(nIndexSmall++,NUM_CIRCLE));
			CDomyun *pDom2 = new CDomyun(pDomMain);
			ForceDiagram.DrawDiagramForce(nIdx, ELE_SHEAR_2,FALSE,m_pStd->m_pARoadOptionStd);	
			rc = ForceDiagram.GetExtRect();
			ForceDiagram.Point(rc.TopLeft().x-2, rc.TopLeft().y+2);
			ForceDiagram.Point(rc.BottomRight().x+2, rc.BottomRight().y+2);
			if(bIsSlab) ForceDiagram.Rotate(ConstPi/2*3,FALSE);
			*pDom2 << ForceDiagram;
			if(bIsSlab)
			{
				XLText->AddFormatText("$h$m+15");
				XLText->InsertDomyun(pDom2,m_pStd->m_pView,nSizeRow,CXLFormatText:: INSERT_DOM_CURPOS&~CXLFormatText::INSERT_DOM_MOVEROW);
				*XLText += nSizeRow;
			}
			else 
				XLText->InsertDomyun(pDom2,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_MOVEROW);
		}

		//
		nIdx = pFemNormal->GetLoadCaseByString("ENVUSE MAX");
		if(nIdx > -1)
		{
			nIndexSmall = 1;
			XLText->AddFormatText("$s+18$h$c%d)$r���� �������$n$n",nIndexMiddle++);
			if(bIsSlab) XLText->AddFormatText("$h$m+04$c%s$rB.M.D$n$n",GetNumberString(nIndexSmall++,NUM_CIRCLE));
			else XLText->AddFormatText("$h$r$c%s$rB.M.D$n$n",GetNumberString(nIndexSmall++,NUM_CIRCLE));
			CDomyun *pDom3 = new CDomyun(pDomMain);
			ForceDiagram.DrawDiagramForce(nIdx, ELE_MOMENT2,FALSE,m_pStd->m_pARoadOptionStd);	
			rc = ForceDiagram.GetExtRect();
			ForceDiagram.Point(rc.TopLeft().x-2, rc.TopLeft().y+2);
			ForceDiagram.Point(rc.BottomRight().x+2, rc.BottomRight().y+2);
			if(bIsSlab) ForceDiagram.Rotate(ConstPi/2*3,FALSE);
			*pDom3 << ForceDiagram;
			if(bIsSlab)
			{
				XLText->AddFormatText("$h$m+04");
				XLText->InsertDomyun(pDom3,m_pStd->m_pView,nSizeRow,CXLFormatText:: INSERT_DOM_CURPOS&~CXLFormatText::INSERT_DOM_MOVEROW);
			}
			else 
				XLText->InsertDomyun(pDom3,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_MOVEROW);
			*XLText += 1;
			
			if(bIsSlab) XLText->AddFormatText("$u$u$u$h$m+15$c%s$rS.F.D$n$n",GetNumberString(nIndexSmall++,NUM_CIRCLE));
			else XLText->AddFormatText("$s+18$h$r$c%s$rS.F.D$n$n",GetNumberString(nIndexSmall++,NUM_CIRCLE));
			CDomyun *pDom4 = new CDomyun(pDomMain);
			ForceDiagram.DrawDiagramForce(nIdx, ELE_SHEAR_2,FALSE,m_pStd->m_pARoadOptionStd);	
			rc = ForceDiagram.GetExtRect();
			ForceDiagram.Point(rc.TopLeft().x-2, rc.TopLeft().y+2);
			ForceDiagram.Point(rc.BottomRight().x+2, rc.BottomRight().y+2);
			if(bIsSlab) ForceDiagram.Rotate(ConstPi/2*3,FALSE);
			*pDom4 << ForceDiagram;
			if(bIsSlab)
			{
				XLText->AddFormatText("$h$m+15");
				XLText->InsertDomyun(pDom4,m_pStd->m_pView,nSizeRow,CXLFormatText:: INSERT_DOM_CURPOS&~CXLFormatText::INSERT_DOM_MOVEROW);
				*XLText += nSizeRow;
			}
			else 
				XLText->InsertDomyun(pDom4,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_MOVEROW);
		}
	}
	//
	if(pFemEQ1->IsOutputLoaded())
	{
		nIndexSmall = 1;
		if(pFemEQ2)
			XLText->AddFormatText("$s+18$h$c%d)$r������ : ������ ��ü ���$n",nIndexMiddle++);
		else
			XLText->AddFormatText("$s+18$h$c%d)$r������$n",nIndexMiddle++);
		XLText->AddFormatText("$h$r$c%s$rB.M.D$n",GetNumberString(nIndexSmall++,NUM_CIRCLE));
		
		CDomyun *pDom1 = new CDomyun(pDomMain);
		CForceDiagram ForceDiagram(pFemEQ1);
		ForceDiagram.SetScaleDim(pDomMain->GetScaleDim()/1000.0);
		long nIdx = pFemEQ1->GetLoadCaseByString("ENV MAX");
		if(nIdx > -1)
		{
			ForceDiagram.DrawDiagramForce(nIdx, ELE_MOMENT2,FALSE,m_pStd->m_pARoadOptionStd);	
			rc = ForceDiagram.GetExtRect();
			ForceDiagram.Point(rc.TopLeft().x-2, rc.TopLeft().y+2);
			ForceDiagram.Point(rc.BottomRight().x+2, rc.BottomRight().y+2);
			*pDom1 << ForceDiagram;
			XLText->InsertDomyun(pDom1,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_MOVEROW);
			*XLText += 1;
			
			XLText->AddFormatText("$s+18$h$r$c%s$rS.F.D$n",GetNumberString(nIndexSmall++,NUM_CIRCLE));
			CDomyun *pDom2 = new CDomyun(pDomMain);
			ForceDiagram.DrawDiagramForce(nIdx, ELE_SHEAR_2,FALSE,m_pStd->m_pARoadOptionStd);	
			rc = ForceDiagram.GetExtRect();
			ForceDiagram.Point(rc.TopLeft().x-2, rc.TopLeft().y+2);
			ForceDiagram.Point(rc.BottomRight().x+1, rc.BottomRight().y+2);
			*pDom2 << ForceDiagram;
			XLText->InsertDomyun(pDom2,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_MOVEROW);
			*XLText += 1;
		}
	}
	if(pFemEQ2 && pFemEQ2->IsOutputLoaded())
	{
		nIndexSmall = 1;
		XLText->AddFormatText("$s+18$h$c%d)$r������ : ������ü ���$n",nIndexMiddle++);
		XLText->AddFormatText("$h$r$c%s$rB.M.D$n",GetNumberString(nIndexSmall++,NUM_CIRCLE));
		
		CDomyun *pDom1 = new CDomyun(pDomMain);
		CForceDiagram ForceDiagram(pFemEQ2);
		ForceDiagram.SetScaleDim(pDomMain->GetScaleDim()/1000.0);
		long nIdx = pFemEQ2->GetLoadCaseByString("ENV MAX");
		if(nIdx > -1)
		{
			ForceDiagram.DrawDiagramForce(nIdx, ELE_MOMENT2,FALSE,m_pStd->m_pARoadOptionStd);	
			rc = ForceDiagram.GetExtRect();
			ForceDiagram.Point(rc.TopLeft().x-2, rc.TopLeft().y+2);
			ForceDiagram.Point(rc.BottomRight().x+1, rc.BottomRight().y+2);
			*pDom1 << ForceDiagram;
			XLText->InsertDomyun(pDom1,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_MOVEROW);
			*XLText += 1;
			
			XLText->AddFormatText("$s+18$h$r$c%s$rS.F.D$n",GetNumberString(nIndexSmall++,NUM_CIRCLE));
			CDomyun *pDom2 = new CDomyun(pDomMain);
			ForceDiagram.DrawDiagramForce(nIdx, ELE_SHEAR_2,FALSE,m_pStd->m_pARoadOptionStd);
			rc = ForceDiagram.GetExtRect();
			ForceDiagram.Point(rc.TopLeft().x-2, rc.TopLeft().y+2);
			ForceDiagram.Point(rc.BottomRight().x+1, rc.BottomRight().y+2);
			*pDom2 << ForceDiagram;
			XLText->InsertDomyun(pDom2,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_MOVEROW);
			*XLText += 1;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if(pBri->m_nSelect3DShellTool==1) // MIDAS
	{
		nIndexSmall = 1;
		XLText->AddFormatText("$s+18$h$c%d)$r3D SHELL : �������$n",nIndexMiddle++);
		XLText->AddFormatText("$h$r$c%s$rB.M.D$n",GetNumberString(nIndexSmall++,NUM_CIRCLE));
		*XLText += 1;
		XLText->AddFormatText("$s+18$h$r$c%s$rS.F.D$n",GetNumberString(nIndexSmall,NUM_CIRCLE));
		*XLText += 1;
		//
		nIndexSmall = 1;
		XLText->AddFormatText("$s+18$h$c%d)$r3D SHELL : �������$n",nIndexMiddle++);
		XLText->AddFormatText("$h$r$c%s$rB.M.D$n",GetNumberString(nIndexSmall++,NUM_CIRCLE));
		*XLText += 1;
		XLText->AddFormatText("$s+18$h$r$c%s$rS.F.D$n",GetNumberString(nIndexSmall++,NUM_CIRCLE));
		*XLText += 1;
	}

	delete pDomMain;
}

void COutDesignBridge::MakeTableMainRebarSummaryCrack(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	CARcBridgeDataStd *pBriStd = pBri->m_pARcBridgeDataStd;

	BOOL bIsSlab = (pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)? TRUE:FALSE;
	CString str = _T("");
	long nRowSlab = 0;
	long nRowWallStt = 0;
	long nRowWallEnd = 0;
	long nRowArchRib = 0;
	long nRowPRF = 0;
	CLongArray nArrRowInWall;
	long nCntRow = 0;
	long nCntCol = 0;
//////////////////////////////////////////////////////////////////////////
//	��ü row�� �� Item�� row�� 
	CStringArray strRebarArr;
	CDoubleArray dItemRowArr;
	long nCntInWall = (pBri->m_nSelectInWallSectionDesign == 0)? pBri->GetCountInWall() : 0;
	long nCntItemSlab = pBri->m_pArrConcSlab[eEnvLimit].GetSize();
	long nCntItemWall = (pBri->IsOutsideWall() && (pBri->m_nSelectWallSectionDesign==0 || pBri->m_bOutWallUsebilityCheck))? MIN(10, pBri->m_pArrConcWall[eEnvLimit].GetSize()):0;
	long nCntItemInWall = MIN(nCntInWall*6, pBri->m_pArrConcInWall[eEnvLimit].GetSize());
	long nCntItemArchRib = pBri->m_pArrConcArchRib.GetSize();
	long nCntItemPRF = pBri->m_pArrConcPRF[eEnvLimit].GetSize();
	long nCntItemTotal = nCntItemSlab+nCntItemWall+nCntItemInWall+nCntItemArchRib+nCntItemPRF;

	long nIdxSttArchRib = nCntItemSlab+nCntItemWall+nCntItemInWall;
	long nIdxSttPRF = nCntItemSlab+nCntItemWall+nCntItemInWall+nCntItemArchRib;

	int i=0; for(i = 0; i < nCntItemTotal; i++)
	{
		long nCntItemRow = 0;
		CString strTemp = _T("");
		BOOL bSlab = (i<nCntItemSlab)? TRUE:FALSE;

		CConcBasic *pData = NULL;
		if(i<nCntItemSlab)
			pData = pBri->m_pArrConcSlab[eEnvLimit].GetAt(i);
		else if(i<(nCntItemSlab + nCntItemWall))
			pData = pBri->GetConcDataWall(i-nCntItemSlab, TRUE);
		else if(i < nIdxSttArchRib) 
			pData = pBri->m_pArrConcInWall[eEnvLimit].GetAt(i-(nCntItemSlab + nCntItemWall));
		else if(i < nIdxSttPRF)
			pData = pBri->m_pArrConcArchRib.GetAt(i-nIdxSttArchRib);
		else
			pData = pBri->m_pArrConcPRF[eEnvLimit].GetAt(i-nIdxSttPRF);
		
		strTemp = m_pCalc->m_pSummary->GetStringMainRebarUse(pData, nCntItemRow);

//		if(nCntItemRow < 2) nCntItemRow = 2; // ö���� �ϳ��ۿ� ��� �ּ� ��ĭ�� ����Ѵ�.
		nCntItemRow = 2;	// ������ ��ĭ���� �����.
		strRebarArr.Add(strTemp);
		dItemRowArr.Add(nCntItemRow);
		nCntRow += nCntItemRow;
		if(bSlab) nRowSlab += nCntItemRow;
		else if(i<nCntItemSlab+5) nRowWallStt += nCntItemRow;
		else if(i<nCntItemSlab+10) nRowWallEnd += nCntItemRow;
		else if(i<nIdxSttArchRib)
		{
			long nIndex = (i-(nCntItemSlab+nCntItemWall)) / 6;
			if(nArrRowInWall.GetSize() > nIndex)
			{
				long nCur = nArrRowInWall.GetAt(nIndex);
				nCur += nCntItemRow;
				nArrRowInWall.SetAt(nIndex, nCur);
			}
			else
			{
				nArrRowInWall.Add(nCntItemRow);
			}
		}
		else if(i<nIdxSttPRF)
			nRowArchRib += nCntItemRow;
		else
			nRowPRF += nCntItemRow;
	}

//////////////////////////////////////////////////////////////////////////
	XLText->AddTable(nCntRow+2, 25-1, 3, 7);
	
	// Title
	CString strOpt = _T("");
	CString strUnit = (pBriStd->m_nTypeUnit == 0)? _T("��"):_T("��");
	int nWidthJoin1[] = { 7, 2, 2, 3, 3, 3, 3, 2 };
	int nWidthJoin2[] = { 7, 2, 2, 3, 3, 3, 3, 2 };
	char *Title1[] = { "�� ��", "b", "d", "Ms", "fs", "���տ���", "�տ���", "�� ��" };
	char *Title2[] = { "�� ��", "mm", "mm", pBriStd->m_szTonfM.GetBuffer(20), pBriStd->m_szKgf_CM2.GetBuffer(20),
		"mm", "mm", "�� ��" };

	if(pBriStd->m_nTypeKciCode!=0)
	{
		Title1[5] = _T("ö�ٰ���");
		Title1[6] = _T("�ִ밣��");
	}
	
	for(i=0; i<8; i++)
	{
		if(i==0 || i==7) strOpt = "$md2";
		else strOpt = "";

		XLText->AddMergeFormatText(1,nWidthJoin1[i]-1,"$c%s%s",Title1[i], strOpt);
		XLText->AddFormatText("$m+0%d",nWidthJoin1[i]);
	}
	XLText->AddText("$n$n$h");
	for(i=0; i<8; i++)
	{
		XLText->AddMergeFormatText(0,nWidthJoin2[i]-1,"$c%s",Title2[i]);
		XLText->AddFormatText("$m+0%d",nWidthJoin2[i]);
	}
	XLText->AddText("$n$h");

	//////////////////////////////////////////////////////////////////////////
	// Data
	nCntCol = 9;
	BOOL bIsOutWall = (pBri->IsOutsideWall());

	int nWidthJoint[] = { 3, 4, 2, 2, 3, 3, 3, 3, 2 };
	if(bIsSlab)
	{
		nWidthJoint[0] = 0;
		nWidthJoint[1] = 7;
	}

	for(i = 0; i< nCntItemTotal; i++)
	{
		BOOL bSlab = (i<nCntItemSlab) ? TRUE : FALSE;
		BOOL bArchRib = (nCntItemArchRib > 0 && i >= nIdxSttArchRib) ? TRUE : FALSE;

		CConcBasic *pData = NULL;
		if(bSlab) pData = pBri->m_pArrConcSlab[eEnvLimit].GetAt(i);
		else if(i<(nCntItemSlab + nCntItemWall)) pData = pBri->GetConcDataWall(i-nCntItemSlab, TRUE);
		else if(i < nIdxSttArchRib) 
			pData = pBri->m_pArrConcInWall[eEnvLimit].GetAt(i-(nCntItemSlab + nCntItemWall));
		else if(i < nIdxSttPRF)
			pData = pBri->m_pArrConcArchRib.GetAt(i-nIdxSttArchRib);
		else
			pData = pBri->GetConcDataPRF((i-nIdxSttPRF)==0 ? TRUE : FALSE);

		CRectBeam pBeam(pData, pBri->m_bCheckMainSlabAndFootTemper);

		long j=0; for(j=0; j<nCntCol; j++)
		{
			long nMerge = 0;
			if(j == 0)
			{
				if(bIsSlab) continue;
				else if(bSlab) 
				{
					str = "������";
					if(pBri->IsBoxType())
					{
						str = (i < nCntItemSlab/2 ) ? "��ν�����" : "�Ϻν�����";
					}
				}
				else if(i-nCntItemSlab<5 && bIsOutWall) str = "������ü";
				else if(i-nCntItemSlab<10 && bIsOutWall) str = "������ü";
				else if(i < nIdxSttArchRib)
				{
					long nCol = (i-(nCntItemSlab+nCntItemWall))/5;
					str.Format("�߰���ü%d", nCol+1);
				}
				else if(i < nIdxSttPRF)
					str = "��ġ����";
				else
					str = "�λ��������";

				if(i==0) 
				{
					nMerge = pBri->IsBoxType() ? nRowSlab/2 - 1 : nRowSlab - 1;
				}
				else if(pBri->IsBoxType() && i == nCntItemSlab/2) nMerge = nRowSlab/2 - 1;
				else if(i==nCntItemSlab && bIsOutWall) nMerge = nRowWallStt-1;
				else if(i==nCntItemSlab+5 && bIsOutWall) nMerge = nRowWallEnd-1;
				else if(nCntItemInWall > 0 && i>=(nCntItemSlab+nCntItemWall) && i< nIdxSttArchRib)
				{
					long ncol=0; for(ncol=0; ncol<pBri->GetCountInWall(); ncol++)
					{
						if(ncol >= nArrRowInWall.GetSize()) continue;
						if(i==nCntItemSlab+nCntItemWall + (6 * ncol)) nMerge = nArrRowInWall.GetAt(ncol) - 1;
					}
				}
				else if(nCntItemArchRib > 0 && (i==nIdxSttArchRib && bIsOutWall) || (i==nCntItemSlab && !bIsOutWall)) nMerge = nRowArchRib-1;
				else if(nCntItemPRF > 0 && i==nIdxSttPRF) nMerge = nRowPRF-1;
				else nMerge = 0;
			}
			else if(j == 1)
			{
				if(bSlab || bArchRib) 
				{
					str = pData->m_sTitle;
					if(pBri->IsBoxType())
					{
						str = pData->m_sTitle.Right(pData->m_sTitle.GetLength() - 11);
					}
				}
				else
				{
					if(pData->m_sTitle.Right(1) == ")") 
						str = pData->m_sTitle.Right(10);
					else if(i >= nIdxSttPRF)
						str = pData->m_sTitle.Mid(12);
					else 
						str = pData->m_sTitle.Right(6);
				}
				strOpt = "";
			}
			else if(j == 2)
			{
				str.Format("$f1%.1f", pData->m_Bo);
				strOpt = "";
			}
			else if(j == 3)
			{
				str.Format("$f1%.1f", pData->m_Ho+pData->m_Hunch/3.-pData->GetRebarDistTension());
				strOpt = "";
			}
			else if(j == 4)
			{
				str.Format("$f3%.3f", GetValueUnitChange(pData->m_MomentUse, UNIT_CGS_TONFM, pBriStd->m_szTonfM));
				strOpt = "";
			}
			else if(j == 5)
			{
				str.Format("$f3%.3f", pBeam.GetFs());
				strOpt = "";
			}
			else if(j == 6)
			{
				double dWa = pBeam.GetWa(); pBeam.GetRebarAreaRequired(); 
				str.Format("$f3%.3f", dWa);
				strOpt = "";
			}
			else if(j == 7)
			{
				double dW = pBeam.GetW();
				str.Format("$f3%.3f", dW);
				strOpt = "";
			}
			else if(j == 8)
			{
				BOOL bCheckCrack = (pBeam.GetWa() > pBeam.GetW())? TRUE:FALSE;
				
				str = (bCheckCrack)? "O.K":"$#03N.G";
				strOpt = "";
			}
			else
			{
				str = _T("");
				strOpt = "";
			}
			if(j > 4 && (pBeam.GetW() <= 0.0 || Compare(pData->m_MomentUse, 0.0, _T("="), 0.01)))
			{
				str = "-";
			}

			XLText->AddMergeFormatText(1, nWidthJoint[j]-1, "$c%s", str);
			XLText->Merge(nMerge, 0);
			XLText->AddFormatText("$m+0%d", nWidthJoint[j]);
		}
		*XLText += 1;
		XLText->AddText("$n$h");
	}
/*
	CARcBridgeDataStd *pBriStd = pBri->m_pARcBridgeDataStd;

	CString str = _T("");
	long nRowSlab = 0;
	long nRowWallStt = 0;
	long nRowWallEnd = 0;
	long nRowWallMid = 0;
	long nCntRow = 0;
	long nCntCol = 0;

	BOOL bIsSlab = (pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)? TRUE:FALSE;
//////////////////////////////////////////////////////////////////////////
//	��ü row�� �� Item�� row�� 
	CStringArray strRebarArr;
	CDoubleArray dItemRowArr;
	long nCntItemSlab = pBri->m_pArrConcSlab.GetSize();
	long nCntItemWall = (pBri->IsOutsideWall())? min(6, pBri->m_pArrConcWall.GetSize()):0;
	long nCntItemArchRib = pBri->m_pArrConcArchRib.GetSize();
	double dMainRebarCtc = pBri->m_dCTC_MainRebar;
	nCntRow = (nCntItemSlab+nCntItemWall+nCntItemArchRib)*2;

//////////////////////////////////////////////////////////////////////////
	long nCol = 0, nRow = 0;
	XLText->AddTable(nCntRow+2, 25-1, 3, 7);
	
	// Title
	CString strOpt = _T("");
	CString strUnit = (pBriStd->m_nTypeUnit == 0)? _T("��"):_T("��");
	int nWidthJoin1[] = { 7, 2, 2, 3, 3, 3, 3, 2 };
	int nWidthJoin2[] = { 7, 2, 2, 3, 3, 3, 3, 2 };
	char *Title1[] = { "�� ��", "b", "d", "Ms", "fs", "���տ���", "�տ���", "�� ��" };
	char *Title2[] = { "�� ��", "mm", "mm", pBriStd->m_szTonfM.GetBuffer(20), pBriStd->m_szKgf_CM2.GetBuffer(20),
		"mm", "mm", "�� ��" };
	
	long i=0; for(i=0; i<8; i++)
	{
		if(i==0 || i==7) strOpt = "$md2";
		else strOpt = "";

		XLText->AddMergeFormatText(1,nWidthJoin1[i]-1,"$c%s%s",Title1[i], strOpt);
		XLText->AddFormatText("$m+0%d",nWidthJoin1[i]);
	}
	XLText->AddText("$n$n$h");
	for(i=0; i<8; i++)
	{
		XLText->AddMergeFormatText(0,nWidthJoin2[i]-1,"$c%s",Title2[i]);
		XLText->AddFormatText("$m+0%d",nWidthJoin2[i]);
	}
	XLText->AddText("$n$h");

	//////////////////////////////////////////////////////////////////////////
	// Data
	nCntCol = 9;

	int nWidthJoint[] = { 3, 4, 2, 2, 3, 3, 3, 3, 2 };
	if(bIsSlab)
	{
		nWidthJoint[0] = 0;
		nWidthJoint[1] = 7;
	}

	for(i=0; i<nCntItemSlab+nCntItemWall+nCntItemArchRib; i++)
	{
		BOOL bSlab = (i<nCntItemSlab) ? TRUE : FALSE;
		BOOL bArchRib = (i >= (nCntItemSlab+nCntItemWall)) ? TRUE : FALSE;

		CConcBasic *pData = NULL;
		if(bSlab) pData = pBri->m_pArrConcSlab.GetAt(i);
		else if(i<(nCntItemSlab + nCntItemWall)) pData = pBri->GetConcDataWall(i-nCntItemSlab, FALSE);
		else pData = pBri->m_pArrConcArchRib.GetAt(i-(nCntItemSlab + nCntItemWall));

		CRectBeam pBeam(pData, pBri->m_bCheckMainSlabAndFootTemper);

		long j=0; for(j=0; j<nCntCol; j++)
		{
			long nMerge = 0;
			if(j == 0)
			{
				if(bIsSlab) continue;
				else if(bSlab) str = "������";
				else if(i-nCntItemSlab<3) str = "������ü";
				else if(i-nCntItemSlab<6) str = "������ü";
				else str = "��ġ����";

				if(i==0) nMerge = nCntItemSlab*2-1;
				else if(i==nCntItemSlab) nMerge = 6-1;
				else if(i==nCntItemSlab+3) nMerge = 6-1;
				else if(i==nCntItemSlab+6) nMerge = nCntItemArchRib*2-1;
				else nMerge = 0;
			}
			else if(j == 1)
			{
				if(bSlab || bArchRib)
				{
					str = pData->m_sTitle;
				}
				else
				{
					CString strTemp = _T("");
					strTemp = pData->m_sTitle.Mid(9);
					if(strTemp.Right(1) == ")") str = strTemp.Left(4) + "��";
					else str = strTemp;
				}
				strOpt = "";
			}
			else if(j == 2)
			{
				str.Format("$f1%.1f", pData->m_Bo);
				strOpt = "";
			}
			else if(j == 3)
			{
				str.Format("$f1%.1f", pData->m_Ho+pData->m_Hunch/3.-pData->GetRebarDistTension());
				strOpt = "";
			}
			else if(j == 4)
			{
				str.Format("$f3%.3f", GetValueUnitChange(pData->m_MomentUse, UNIT_CGS_TONFM, pBriStd->m_szTonfM));
				strOpt = "";
			}
			else if(j == 5)
			{
				str.Format("$f3%.3f", pBeam.GetFs());
				strOpt = "";
			}
			else if(j == 6)
			{
				double dWa = pBeam.GetWa(); pBeam.GetRebarAreaRequired(); 
				str.Format("$f3%.3f", dWa);
				strOpt = "";
			}
			else if(j == 7)
			{
				double dW = pBeam.GetW();
				str.Format("$f3%.3f", dW);
				strOpt = "";
			}
			else if(j == 8)
			{
				BOOL bCheckCrack = (pBeam.GetWa() > pBeam.GetW())? TRUE:FALSE;
				
				str = (bCheckCrack)? "O.K":"$#03N.G";
				strOpt = "";
			}
			else
			{
				str = _T("");
				strOpt = "";
			}

			if(j > 1 && pBeam.GetW() <= 0.0) str = _T("-");
			XLText->AddMergeFormatText(1, nWidthJoint[j]-1, "$c%s", str);
			XLText->Merge(nMerge, 0);
			XLText->AddFormatText("$m+0%d", nWidthJoint[j]);
		}
		*XLText += 1;
		XLText->AddText("$n$h");
	}
*/
}

void COutDesignBridge::MakeTableMainRebarSummaryCrackLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	CARcBridgeDataStd *pBriStd = pBri->m_pARcBridgeDataStd;

	BOOL bIsSlab = (pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)? TRUE:FALSE;
	CString str = _T("");
	long nRowSlab = 0;
	long nRowWallStt = 0;
	long nRowWallEnd = 0;
	long nRowArchRib = 0;
	long nRowPRF = 0;
	CLongArray nArrRowInWall;
	long nCntRow = 0;
	long nCntCol = 0;
	//////////////////////////////////////////////////////////////////////////
	//	��ü row�� �� Item�� row�� 
	CStringArray strRebarArr;
	CDoubleArray dItemRowArr;
	long nCntInWall = (pBri->m_nSelectInWallSectionDesign == 0)? pBri->GetCountInWall() : 0;
	long nCntItemSlab = pBri->m_pArrConcSlab[eEnvService].GetSize();
	long nCntItemWall = (pBri->IsOutsideWall() && (pBri->m_nSelectWallSectionDesign==0))? MIN(10, pBri->m_pArrConcWall[eEnvService].GetSize()):0;
	// (ARCBRIDGE-3618) LSD���� �ھ������� ����� ��뼺����� Baem���� �����͸� �����ʰ� ��հ��� �����͸� ��޶�� ��û���� ���� ����Ѵ�.
	if(pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign==1 && pBri->m_bOutWallUsebilityCheck)
		nCntItemWall = 2;
	long nCntItemInWall = MIN(nCntInWall*6, pBri->m_pArrConcInWall[eEnvService].GetSize());
	long nCntItemArchRib = pBri->m_pArrConcArchRib.GetSize();
	long nCntItemPRF = pBri->m_pArrConcPRF[eEnvService].GetSize();
	long nCntItemTotal = nCntItemSlab+nCntItemWall+nCntItemInWall+nCntItemArchRib+nCntItemPRF;

	long nIdxSttArchRib = nCntItemSlab+nCntItemWall+nCntItemInWall;
	long nIdxSttPRF = nCntItemSlab+nCntItemWall+nCntItemInWall+nCntItemArchRib;

	int i=0; for(i = 0; i < nCntItemTotal; i++)
	{
		long nCntItemRow = 0;
		CString strTemp = _T("");
		BOOL bSlab = (i<nCntItemSlab)? TRUE:FALSE;

		CConcBasic *pData = NULL;
		if(i<nCntItemSlab)
		{
			pData = pBri->m_pArrConcSlab[eEnvService].GetAt(i);
		}
		else if(i<(nCntItemSlab + nCntItemWall))
		{
			if(pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign==1 && pBri->m_bOutWallUsebilityCheck)
			{
				BOOL bSttWall = (i-nCntItemSlab) == 0 ? TRUE : FALSE;
				pData = m_pCalc->m_pColGir->GetConcWallForOutWallUsebilityCheck(bSttWall);
			}
			else
				pData = pBri->GetConcDataWall(i-nCntItemSlab, TRUE, eEnvService);
		}
		else if(i < nIdxSttArchRib) 
		{
			pData = pBri->m_pArrConcInWall[eEnvService].GetAt(i-(nCntItemSlab + nCntItemWall));
		}
		else if(i < nIdxSttPRF) 
		{
			pData = pBri->m_pArrConcArchRib.GetAt(i - nIdxSttArchRib);
		}
		else
		{
			pData = pBri->m_pArrConcPRF[eEnvService].GetAt(i- nIdxSttPRF);
		}

		strTemp = m_pCalc->m_pSummary->GetStringMainRebarUse(pData, nCntItemRow);

		//		if(nCntItemRow < 2) nCntItemRow = 2; // ö���� �ϳ��ۿ� ��� �ּ� ��ĭ�� ����Ѵ�.
		nCntItemRow = 2;	// ������ ��ĭ���� �����.
		strRebarArr.Add(strTemp);
		dItemRowArr.Add(nCntItemRow);
		nCntRow += nCntItemRow;
		if(bSlab) nRowSlab += nCntItemRow;
		else if(pBri->m_nSelectWallSectionDesign == 0 && i<nCntItemSlab+5) nRowWallStt += nCntItemRow;
		else if(pBri->m_nSelectWallSectionDesign == 0 && i<nCntItemSlab+10) nRowWallEnd += nCntItemRow;
		else if(pBri->m_nSelectWallSectionDesign == 1 && i<nCntItemSlab + nCntItemWall) nRowWallStt += nCntItemRow;
		else if(i < nIdxSttArchRib)
		{
			long nIndex = (i-(nCntItemSlab+nCntItemWall)) / 6;
			if(nArrRowInWall.GetSize() > nIndex)
			{
				long nCur = nArrRowInWall.GetAt(nIndex);
				nCur += nCntItemRow;
				nArrRowInWall.SetAt(nIndex, nCur);
			}
			else
			{
				nArrRowInWall.Add(nCntItemRow);
			}
		}
		else if(i < nIdxSttPRF) 
			nRowArchRib += nCntItemRow;
		else
			nRowPRF += nCntItemPRF;
	}

	//////////////////////////////////////////////////////////////////////////
	XLText->AddTable(nCntRow+2, 25-1, 3, 4);

	BOOL bCheckCrack = pBri->m_nSelectUsebilityCheck == 1 ? TRUE : FALSE;				// ��뼺 ����� �տ�����
	// Title
	CString strOpt = _T("");
	CString strUnit = (pBriStd->m_nTypeUnit == 0)? _T("��"):_T("��");
	int nWidthJoin1[] = { 4, 2, 2, 4, 6, 5, 2 };
	int nWidthJoin2[] = { 4, 2, 2, 4, 3, 3, 2, 3, 2};
	char *Title1[] = { "�� ��", "�տ�", "d", "M", "�ּ�ö�ٷ�", "�����տ�����", "�� ��" };
	char *Title2[] = { "�� ��", "�տ�", "mm", pBriStd->m_szTonfM.GetBuffer(20), "As,min", "As,use", "fs,max", "fs", "�� ��" };
	if(bCheckCrack == FALSE)
	{
		Title1[5] = "�տ��� ����";
		Title2[6] = "Wk";
		Title2[7] = "Wa";
	}

	for(i=0; i<7; i++)
	{
		if(i==0 || i==1 || i==6) strOpt = "$md2";
		else strOpt = "";

		XLText->AddMergeFormatText(1,nWidthJoin1[i]-1,"$c%s%s",Title1[i], strOpt);
		XLText->AddFormatText("$m+0%d",nWidthJoin1[i]);
	}
	XLText->AddText("$n$n$h");
	for(i=0; i<9; i++)
	{
		XLText->AddMergeFormatText(0,nWidthJoin2[i]-1,"$c%s",Title2[i]);
		XLText->AddFormatText("$m+0%d",nWidthJoin2[i]);
	}
	XLText->AddText("$n$h");

	//////////////////////////////////////////////////////////////////////////
	// Data
	nCntCol = 10;
	BOOL bIsOutWall = (pBri->IsOutsideWall());

	int nWidthJoint[] = { 1, 3, 2, 2, 4, 3, 3, 2, 3, 2 };
	if(bIsSlab)
	{
		nWidthJoint[0] = 0;
		nWidthJoint[1] = 4;
	}

	for(i = 0; i < nCntItemTotal; i++)
	{
		BOOL bSlab = (i<nCntItemSlab) ? TRUE : FALSE;
		BOOL bArchRib = (nCntItemArchRib > 0 && i >= nIdxSttArchRib) ? TRUE : FALSE;
		BOOL bWall = FALSE;

		CConcBasic *pData = NULL;
		if(bSlab) 
			pData = pBri->m_pArrConcSlab[eEnvService].GetAt(i);
		else if(i<(nCntItemSlab + nCntItemWall)) 
		{
			if(pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign==1 && pBri->m_bOutWallUsebilityCheck)
			{
				BOOL bSttWall = (i-nCntItemSlab) == 0 ? TRUE : FALSE;
				pData = m_pCalc->m_pColGir->GetConcWallForOutWallUsebilityCheck(bSttWall);
			}
			else
				pData = pBri->GetConcDataWall(i-nCntItemSlab, TRUE, eEnvService);

			bWall = TRUE;
		}
		else if(i < nIdxSttArchRib) 
			pData = pBri->m_pArrConcInWall[eEnvService].GetAt(i-(nCntItemSlab + nCntItemWall));
		else if(i < nIdxSttPRF)
			pData = pBri->m_pArrConcArchRib.GetAt(i- nIdxSttArchRib);
		else
			pData = pBri->m_pArrConcPRF[eEnvService].GetAt(i- nIdxSttPRF);

		CLsdBeam pBeam(pData, LSD_ULS_DESIGN);
		const HGBOOL bCrack = pBeam.IsCrackSection();

		long j=0; for(j=0; j<nCntCol; j++)
		{
			long nMerge = 0;
			if(j == 0)
			{
				if(bIsSlab) continue;
				else if(bSlab) 
				{
					str = "������";
					if(pBri->IsBoxType())
					{
						str = (i < nCntItemSlab/2 ) ? "��ν�����" : "�Ϻν�����";
					}
				}
				else if(i-nCntItemSlab<5 && bIsOutWall && bWall) str = pBri->m_nSelectWallSectionDesign==1 ? "��ü" : "������ü";
				else if(i-nCntItemSlab<10 && bIsOutWall && bWall) str = "������ü";
				else if(i < nIdxSttArchRib)
				{
					long nCol = (i-(nCntItemSlab+nCntItemWall))/5;
					str.Format("�߰���ü%d", nCol+1);
				}
				else if(i < nIdxSttPRF)
					str = "��ġ����";
				else
					str = "�λ��������";

				if(i==0) 
				{
					nMerge = pBri->IsBoxType() ? nRowSlab/2 - 1 : nRowSlab - 1;
				}
				else if(pBri->IsBoxType() && i == nCntItemSlab/2) nMerge = nRowSlab/2 - 1;
				else if(i==nCntItemSlab && bIsOutWall && bWall) nMerge = nRowWallStt-1;
				else if(i==nCntItemSlab+5 && bIsOutWall && bWall) nMerge = nRowWallEnd-1;
				else if(nCntItemInWall > 0 && i>=(nCntItemSlab+nCntItemWall) && i < nIdxSttArchRib)
				{
					long ncol=0; for(ncol=0; ncol<pBri->GetCountInWall(); ncol++)
					{
						if(ncol >= nArrRowInWall.GetSize()) continue;
						if(i==nCntItemSlab+nCntItemWall + (6 * ncol)) nMerge = nArrRowInWall.GetAt(ncol) - 1;
					}
				}
				else if(nCntItemArchRib > 0 && (i==nIdxSttArchRib && bIsOutWall) || (i==nCntItemSlab && !bIsOutWall)) nMerge = nRowArchRib-1;
				else if(nCntItemPRF > 0 && i == nIdxSttPRF) nMerge = nRowPRF-1;
				else nMerge = 0;
			}
			else if(j == 1)
			{
				if(bSlab || bArchRib) 
				{
					str = pData->m_sTitle;
					if(pBri->IsBoxType())
					{
						str = pData->m_sTitle.Right(pData->m_sTitle.GetLength() - 11);
					}
				}
				else if(i < (nCntItemSlab + nCntItemWall) && pBri->m_nSelectWallSectionDesign==1)
				{
					// (ARCBRIDGE-3618) 
					str = pData->m_sTitle;
				}
				else
				{
					if(pData->m_sTitle.Right(1) == ")") 
						str = pData->m_sTitle.Right(10);
					else if(i >= nIdxSttPRF)
						str = pData->m_sTitle.Mid(12);
					else 
						str = pData->m_sTitle.Right(6);
				}
				strOpt = "";
			}
			else if(j == 2)
			{
				
				str = bCrack ? "�տ�" : "��տ�";
				strOpt = "";
			}
			else if(j == 3)
			{
				str.Format("$f1%.1f", pData->m_Ho+pData->m_Hunch/3.-pData->GetRebarDistTension());
				strOpt = "";
			}
			else if(j == 4)
			{
				str = pBriStd->GetStrDoubleUnitChange(bCrack? pData->m_MomentUse : pData->m_MomentCrack, UNIT_CGS_TONFM, pBriStd->m_szTonfM);
				strOpt = "";
			}
			else if(j == 5)
			{
				str.Format("$f1%.1f", pBeam.GetAsMinCrack());
				strOpt = "";
			}
			else if(j == 6)
			{
				str.Format("$f1%.1f", pBeam.GetAsUse());
				strOpt = "";
			}
			else if(j == 7)
			{
				str.Format("$f1%.1f", bCheckCrack ? pBeam.GetMaxCrackMpa() : pBeam.GetWk());
				strOpt = "";
			}
			else if (j == 8)
			{
				str.Format("$f1%.1f", bCheckCrack ? pBeam.GetFsCrack() : pBeam.GetWa());
				strOpt = "";
			}
			else if(j == 9)
			{
				BOOL bCheckCrackResult = pBeam.IsOkCrackAsMin();
				if(pData->m_nSelectUsebilityCheck == 1)
					bCheckCrackResult = bCheckCrackResult && pBeam.IsOkCrackCtc();
				else
					bCheckCrackResult = bCheckCrackResult && pBeam.IsOkCrackCtc() && pBeam.IsOkCrackWidth();

				str = (bCheckCrackResult)? "O.K":"$#03N.G";
				strOpt = "";
			}
			else
			{
				str = _T("");
				strOpt = "";
			}

			if(j > 4 && bCrack == FALSE) str = _T("-");
			XLText->AddMergeFormatText(1, nWidthJoint[j]-1, "$j$c%s", str);
			XLText->Merge(nMerge, 0);
			XLText->AddFormatText("$m+0%d", nWidthJoint[j]);
		}
		*XLText += 1;
		XLText->AddText("$n$h");
	}
}

void COutDesignBridge::MakeTableBoringData(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	CString szJijum = _T(""), str = _T("");
	CConcSymbol sym;

	long nTitleSmall = 1;
	long nSizeJijum = pBri->IsBoxType() ? 1 : pBri->GetCountJijum();
	for(long nJijum=0; nJijum<nSizeJijum; nJijum++)
	{
		if(nJijum==0 && pBri->IsOutWall(TRUE)==FALSE) continue;
		if(nJijum==nSizeJijum-1 && pBri->IsOutWall(FALSE)==FALSE) continue;

		CBoringDataEx *pBorEx = pBri->GetBoringDataExByJijumNumber(nJijum);
		if(pBorEx == NULL) continue;

		char *Title[] = { "����", "������Ī", "dH", "N", "c", "Cu", "Eo", "��" };
		int nWidthJoint[] = { 2, 5, 3, 2, 3, 3, 3, 2 };
		long nSizeRow = pBorEx->m_pArrBoring.GetSize() + 1;
		long nSizeCol = 8;

		if(nJijum==0) 
			szJijum = "������ ����";
		else if(nJijum==nSizeJijum-1) 
			szJijum = "������ ����";
		else
			szJijum.Format("�߰������� %d",nJijum-1);

		if(pBri->IsBoxType())
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+%d$ %s, STA. %.3f, EL. %.3f$n",nSizeRow+1,pBorEx->m_sNameBoring,pBorEx->m_dStation,pBorEx->m_EL);
		}
		else
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+%d$ %d)$r%s : %s, STA. %.3f, EL. %.3f$n",nSizeRow+1,nTitleSmall++,szJijum,pBorEx->m_sNameBoring,pBorEx->m_dStation,pBorEx->m_EL);

			XLText->GoPosition(NO_MOVE,COL_POS_2);
		}
		XLText->AddTable(nSizeRow-1,23-1,1,2);
		long j=0; for(j=0; j<nSizeCol; j++)
		{
			XLText->AddMergeFormatText(0,nWidthJoint[j]-1,"$c%s",Title[j]);
			XLText->AddFormatText("$m+0%d",nWidthJoint[j]);
		}
		
		long nLengthStr = 0;
		long i=0; for(i=1; i<nSizeRow; i++)
		{
			long nIndex = i - 1;
			CBoringData *pBor = pBorEx->m_pArrBoring.GetAt(nIndex);
			for(j=0; j<nSizeCol; j++)
			{
				CString szKgf_Cm2 = GetStringUnitByType(UNIT_CGS_KGF_CM2, pData->IsLsdDesign() || pData->IsSI() ? 1 : 0, pData->IsLsdDesign() ? 1 : 2);

				if(j==0) str.Format("%d",i);
				if(j==1) str = sym.m_strSoilName[pBor->m_nNameSoil];
				if(j==2) str.Format("'%s", pData->GetStrDoubleUnitChange(pBor->m_dDepth, UNIT_CGS_M, pData->m_szM));
				if(j==3) str.Format(_T("%d"), pBor->m_nNValue);
				if(j==4) str.Format(_T("'%s"), pData->GetStrDoubleUnitChange(pBor->m_c, UNIT_CGS_TONF_M2, pData->m_szTonf_M2));
				if(j==5) str.Format(_T("'%.3f"), pBor->m_Cu);
				if(j==6) str.Format(_T("'%s"), pData->GetStrDoubleUnitChange(pBor->m_dEo, UNIT_CGS_KGF_CM2, szKgf_Cm2, FALSE, 0));
				if(j==7) str.Format(_T("'%.3f"), pBor->m_RatePoasong);

				if(j==0)	XLText->AddText("$n$h");

				nLengthStr = str.GetLength();

				if(nLengthStr>18)
					XLText->AddMergeFormatText(0,nWidthJoint[j]-1,"$c$tx07%s",str);
				else
					XLText->AddMergeFormatText(0,nWidthJoint[j]-1,"$c$tx09%s",str);
				XLText->AddFormatText("$m+0%d",nWidthJoint[j]);
			}
		}
		*XLText += 2;		
	}
}

void COutDesignBridge::MakeExcelCalcResultSummary(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);
	
	m_pCalc->m_pSummary->WriteCalcResultSummary(&XLText, pBri);
}

void COutDesignBridge::MakeExcelCalcShoeReaction(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	if(pBri->IsRcSlab()==FALSE && pBri->IsPiRahmen()==FALSE) return;

	CXLFormatText XLText(pXL);
	
	long nT_0 = 0;
	long nIndexMiddle = 1;
	XLText(nT_0);
	XLText.AddFormatText("$tb13%d. ������ħ �ݷ� ����$n",m_nIndexTitle);
	XLText += 1;

	// 2D Frame
	XLText.GoPosition(NO_MOVE,COL_POS_1);
	XLText.AddFormatText("$s+07$h$a%d.%d �ݷ� ���� : 2D Frame �ؼ�$n", m_nIndexTitle, nIndexMiddle++);
	MakeExcelCalcShoeReaction_2DFrame(&XLText,pBri);

	// 3D Plate
	if(pBri->m_nSelect3DShellTool != 2)
	{
		XLText.GoPosition(NO_MOVE,COL_POS_1);
		XLText.AddFormatText("$s+07$h$a%d.%d �ݷ� ���� : 3D Plate �ؼ�$n", m_nIndexTitle, nIndexMiddle++);
		MakeExcelCalcShoeReaction_3DPlate(&XLText,pBri);
	}

	// ����ݷ�
	XLText.GoPosition(NO_MOVE,COL_POS_1);
	XLText.AddFormatText("$s+07$h$a%d.%d ���� �ݷ� ����$n", m_nIndexTitle, nIndexMiddle++);
	MakeExcelCalcShoeReaction_Apply(&XLText,pBri);

	// ������ �����
	if(pBri->GetTypeDesingEQ() != 0 && pBri->m_bUsePadShoe)
	{
		XLText.GoPosition(NO_MOVE,COL_POS_1);
		XLText.AddFormatText("$s+07$h$a%d.%d ������ ����� ����$n", m_nIndexTitle, nIndexMiddle++);
		MakeExcelCalcShoeHoriForce(&XLText,pBri);
	}
}

void COutDesignBridge::MakeExcelSTM(CXLControl *pXL, CRcBridgeRebar *pBri, HGINT32 nItem, HGINT32 iKey, HGINT32 nIndexMiddle)
{
 	long nIndexSmall  = 1;
	CXLFormatText XLText(pXL);
	COptionItem *pOpt = m_pStd->m_pXLOption;
	COptionItem	*pOptStm = m_pStd->m_pStmOption;
	COptionItem *pOptEtc = m_pStd->m_pEtcOption;
	HGINT32 nGroup = 0;

	CStmModel*pModel = pBri->GetStmModelByKey(iKey);
	if (!pModel) return;

	HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	XLText.GoPosition(NO_MOVE, 0);

	if(nIndexMiddle == 1)
		XLText.AddFormatText(_T("$h$tb13%d. %s$n"), m_nIndexTitle, pOpt->GetItem(nItem));
	XLText.AddFormatText("$h$tb13%d.%d %s(%s) $n", m_nIndexTitle, nIndexMiddle, pOpt->GetItem(nItem), pBri->GetStringStmModelNameByKey(iKey, 0));
	
	CStmDesignStd StmDesignStd(pModel, m_pStd->m_pARoadOptionStd, m_pStd->m_pView, TRUE);
	StmDesignStd.SetTypeKCI(pBri->m_pARcBridgeDataStd->m_nTypeKciCode == 2 ? 1 : 0);
	StmDesignStd.SetTypeRoadCode(pBri->m_pARcBridgeDataStd->IsDesignKDS2019() ? eRoadCode2019 : eRoadCode2015);
	CStmOutputExcel *pStmExcel = StmDesignStd.GetExcelOut();
	if(pStmExcel == NULL) return;
	StmDesignStd.CalculateStmAll();

	HGINT32 iLevelTitle = 2;
	long nIndexPrint = 0;

	// ��������
	if(pOptStm->IsCheck(hggettext("��������")))
	{
		MakeExcelSTM_Condition(&XLText, pBri, pModel, COL_POS_1, nIndexSmall++, nItem, iKey);
	}

	//�ܸ�����
	if(pOptStm->IsCheck(hggettext("�ܸ�����")))
	{
		switch (nItem)
		{
		case XL_DESIGNOUT_STMARC :
			MakeExcelSTM_ArcCorner_Section(&XLText, pBri, COL_POS_1, nIndexSmall++, iKey);
			break;
		case XL_DESIGNOUT_STMBRACKET :
			MakeExcelSTM_Bracket_Section(&XLText, pBri, COL_POS_1, nIndexSmall++, iKey);
			break;
		case XL_DESIGNOUT_STMFOOT :
			MakeExcelSTM_Footing_Section(&XLText, pBri, COL_POS_1, nIndexSmall++, iKey);
			break;
		case XL_DESIGNOUT_STMFOOTMID :
			MakeExcelSTM_FootingMid_Section(&XLText, pBri, COL_POS_1, nIndexSmall++, iKey);
			break;
		}
		
	}

	// �ؼ� �𵨸�
	if(pOptStm->IsCheck(hggettext("�ؼ��𵨸�")))
	{
		long nNumber = 0;

		XLText.AddFormatText("$s+45$e$h$a%d. �ؼ� �𵨸�$n", nIndexSmall++);

		pStmExcel->MakeExcelStmModeling(&XLText, COL_POS_1, iLevelTitle, m_nIndexTitle, nIndexMiddle, pBri->GetStringStmModelNameByKey(iKey), _T("TOP"), FALSE);

		//������ǥ
		pStmExcel->MakeExcelStmNodeTable(&XLText, COL_POS_1, iLevelTitle, ++nNumber, nNumber);

		if( bLsd )
		{
			// ��������
			pStmExcel->MakeExcelStmElementTable(&XLText, COL_POS_1, iLevelTitle, ++nNumber, nNumber);
			///Restraints
			if(pModel->GetCountRestraint() > 0)
				pStmExcel->MakeExcelStmRestraintsTable(&XLText, COL_POS_1, iLevelTitle, ++nNumber, nNumber);

			//Spring
			if(pModel->GetCountSpring() > 0)
				pStmExcel->MakeExcelStmSpringTable(&XLText, COL_POS_1, iLevelTitle, ++nNumber, nNumber);

			//��Ʈ��/Ÿ�� ����
			pStmExcel->MakeExcelStmStrutTieApply(&XLText, COL_POS_1, iLevelTitle, ++nNumber, nNumber, TRUE, TRUE);
		}

		// ������ �帧��
		pStmExcel->MakeExcelStmStressFlow(&XLText, COL_POS_1, iLevelTitle, ++nNumber, nNumber);

	}

	// ���߻���
	if(pOptStm->IsCheck(hggettext("���߻���")))
	{
		switch (nItem)
		{
		case XL_DESIGNOUT_STMARC :
			MakeExcelSTM_ArcCorner_CalcLoad(&XLText, pBri, COL_POS_1, nIndexSmall++, iKey);
			break;
		case XL_DESIGNOUT_STMBRACKET :
			MakeExcelSTM_Bracket_CalcLoad(&XLText, pBri, COL_POS_1, nIndexSmall++, iKey);
			break;
		case XL_DESIGNOUT_STMFOOT :
		case XL_DESIGNOUT_STMFOOTMID :
			MakeExcelSTM_Footing_CalcLoad(&XLText, pBri, COL_POS_1, nIndexSmall++, iKey);
			break;
		}
	}
		
	XLText.GoPosition(NO_MOVE, COL_POS_1);
	iLevelTitle = 1;
	long nNumber = 1;
	// �������ϵ�	
	if(pOptStm->IsCheck(hggettext("�������ϵ�")))
	{			
		XLText.AddFormatText("$s+45$e$h$a%d. �������ϵ�$n$n", nIndexSmall++);
		pStmExcel->MakeExcelStmLoadCondition(&XLText, COL_POS_1, iLevelTitle, nNumber, -1, 0, -1, -1, FALSE);
	}

	// �ؼ����
	if(pOptStm->IsCheck(hggettext("��Ʈ��/Ÿ�� �ܸ��")))
	{
		XLText.AddFormatText("$s+45$e$h$a%d. ��Ʈ��/Ÿ�� �ܸ��$n$n", nIndexSmall++);
		pStmExcel->MakeExcelStmResult(&XLText, COL_POS_1, iLevelTitle, nNumber, -1, nIndexPrint, FALSE, FALSE, TRUE);
	}

	// ö�ٹ�ġ
	if(pOptStm->IsCheck(hggettext("ö�ٹ�ġ")))
	{
		XLText.AddFormatText("$s+45$e$h$a%d. %s$n$n", nIndexSmall++, bLsd ? _T("����Ÿ���� ���� ����") : _T("ö�� ��ġ"));
		pStmExcel->MakeExcelStmRebar(&XLText, COL_POS_1, iLevelTitle, nNumber, -1, nIndexPrint, FALSE, TRUE);
	}

	// ��Ʈ�� �ִ���ȿ��
	XLText.GoPosition(NO_MOVE, COL_POS_1);
	if(pOptStm->IsCheck(hggettext("��Ʈ�� ���������� �ִ���ȿ�� ����")))
	{
		BOOL bDomyun = pOptEtc->IsCheck(hggettext("�ִ���ȿ�� ���� ��½� �� ����"));
		XLText.AddFormatText("$s+45$e$h$a%d. ��Ʈ�� ���������� �ִ���ȿ�� ����$n$n", nIndexSmall++);
		pStmExcel->MakeExcelStmStrutEffectiveWidth(&XLText, COL_POS_1, iLevelTitle, nNumber, -1, nIndexPrint, FALSE, bDomyun, TRUE);
	}


	// ��Ʈ�� ����
	if(pOptStm->IsCheck(hggettext("��Ʈ�� ��������")))
	{
		XLText.AddFormatText("$s+45$e$h$a%d. ��Ʈ�� ��������$n", nIndexSmall++);
		pStmExcel->MakeExcelStmStrutStrength(&XLText, COL_POS_1, iLevelTitle, nNumber, -1, nIndexPrint, FALSE, TRUE);
	}

	// ���������� ��������
	XLText.GoPosition(NO_MOVE, COL_POS_1);
	if(pOptStm->IsCheck(hggettext("���������� ��������")))
	{
		XLText.AddFormatText("$s+45$e$h$a%d. ���������� ��������$n", nIndexSmall++);
		pStmExcel->MakeExcelStmNodalZoneStrength(&XLText, COL_POS_1, iLevelTitle, nNumber, -1, nIndexPrint, FALSE, TRUE);
	}
	
	// �տ���� ���� �ּ�ö�ٷ��� ���
	// (ARCBRIDGE-2979) ����������϶� �찢�δ� �ּ�ö�ٺ� ���� ���ϵ���.
	if((bLsd || nItem != XL_DESIGNOUT_STMARC) && pOptStm->IsCheck(hggettext("�տ���� ���� �ּ�ö�ٷ��� ���")))
	{
		XLText.AddFormatText("$s+45$e$h$a%d. �տ���� ���� �ּ�ö�ٷ��� ���$n", nIndexSmall++);
		pStmExcel->MakeExcelStmMinRebar(&XLText, COL_POS_1, iLevelTitle, nNumber, -1, nIndexPrint, FALSE, TRUE);
	}

	//ö���� ����(ARCBRIDGE-2779) ��������
}

void COutDesignBridge::MakeExcelCalcShoeReaction_2DFrame(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	if(pBri->IsRcSlab()==FALSE && pBri->IsPiRahmen()==FALSE) return;
	
	CStringArray sArrTitle;
	CDoubleArray dArrWidth;

	CARcBridgeDataStd* pData = pBri->m_pARcBridgeDataStd;
	BOOL bLsd = pData->IsLsdDesign();
	long nQtyJijum = pBri->IsPiRahmen() ? 2 : pBri->GetCountJijum();

	long nSizeRow = bLsd? 7 : 6;
	long nSizeCol = nQtyJijum + 2;
	long nSizeExcelCol = 24;

	CString str = _T("");
	if(pBri->IsPiRahmen() || nQtyJijum==2)
	{
		sArrTitle.Add("�� ��");
		if(bLsd)
		{
			sArrTitle.Add("����1(N/mm)");
			sArrTitle.Add("����2(N/mm)");
		}
		else
		{
			sArrTitle.Add("����1");
			sArrTitle.Add("����2");
		}
		sArrTitle.Add("�� ��");
		dArrWidth.Add(4);
		dArrWidth.Add(7);
		dArrWidth.Add(7);
		dArrWidth.Add(6);
	}
	else
	{
		sArrTitle.Add("�� ��");
		sArrTitle.Add(bLsd? _T("����1(N/mm)") : _T("����1"));
		long i=0; for(i=1; i<nQtyJijum-1; i++)
		{
			if(bLsd)
				str.Format("����%d(N/mm)",i);
			else
				str.Format("����%d",i);

			sArrTitle.Add(str);
		}
		sArrTitle.Add(bLsd? _T("����2(N/mm)") : _T("����2"));
		sArrTitle.Add("�� ��");
		for(i=0; i<nSizeCol; i++)
			dArrWidth.Add(4);
		nSizeExcelCol = nSizeCol*4;
	}

	long nWidthCol = 0;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddTable(nSizeRow-1,nSizeExcelCol-1,1,0);

	long nSizeStr = sArrTitle.GetSize();
	long j=0; for(j=0; j<nSizeCol; j++)
	{
		if(j > nSizeStr-1) break;
		nWidthCol = (long)dArrWidth.GetAt(j);
		str = sArrTitle.GetAt(j);
		XLText->AddMergeFormatText(0,nWidthCol-1,"$c%s",str);
		XLText->AddFormatText("$m+0%d",nWidthCol);
	}

	long nJijum = 0;
	double dValue=0, dV1=0, dV2=0, dV3=0;
	double SlabWidth = bLsd? frM(m_pCalc->GetSlabWidth()) : m_pCalc->GetSlabWidth();
	CStringArray szArrItem;
	if(bLsd)
	{
		szArrItem.Add(_T("��������(DC)"));
		szArrItem.Add(_T("��������(DC)"));
	}
	else
	{
		szArrItem.Add(_T("��������"));
	}
	szArrItem.Add(_T("Ȱ �� ��"));
	szArrItem.Add(_T("�µ�����"));
	szArrItem.Add(_T("����ħ��"));
	szArrItem.Add(_T("��"));

	long i=0; for(i=1; i<nSizeRow; i++)
	{
		for(j=0; j<nSizeCol; j++)
		{
			dValue = 0;
			if(j==0)
			{
				str = szArrItem.GetAt(i-1);
			}
			else 
			{
				if(j==1) // ����1
					nJijum = 0;
				else if(j==nSizeCol-2) // ����2
					nJijum = pBri->GetCountJijum()-1;
				else
					nJijum = j-1;

				if(bLsd)
				{
					if(i==1) dValue = pBri->GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_DC1,TRUE,TRUE);
					if(i==2) dValue = pBri->GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_DW1,TRUE,TRUE);
					if(i==3) dValue = pBri->GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_LL1,TRUE,TRUE);
					if(i==4) 
					{
						dV1    = pBri->GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_TU21 ,TRUE,TRUE);
						dV2    = pBri->GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_TU22 ,TRUE,TRUE);
						dV3    = pBri->GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_SH,TRUE,TRUE);
						dValue = max(dV1+dV3,dV2+dV3);
					}
					if(i==5) dValue = pBri->GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_SD,TRUE,TRUE);
					if(i==6)
					{
						dV1    = pBri->GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_TU21 ,TRUE,FALSE);
						dV2    = pBri->GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_TU22 ,TRUE,FALSE);
						dV3    = pBri->GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_SH,TRUE,FALSE);
						//
						dValue += pBri->GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_DC1,TRUE,FALSE);
						dValue += pBri->GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_DW1,TRUE,FALSE);
						dValue += pBri->GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_LL1,TRUE,FALSE);
						dValue += max(dV1+dV3,dV2+dV3);
						dValue += pBri->GetShoeReactionLsd(nJijum,-1,BRIDGE_LOAD_SUBSID,TRUE,FALSE);
					}
					str.Format("'%.3f",dValue);
				}
				else
				{
					if(i==1) dValue = pBri->GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_DEAD,TRUE,TRUE);
					if(i==2) dValue = pBri->GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_LIVE_FULL,TRUE,TRUE);
					if(i==3) 
					{
						dV1    = pBri->GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_TEMP1 ,TRUE,TRUE);
						dV2    = pBri->GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_TEMP2 ,TRUE,TRUE);
						dV3    = pBri->GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_SHRINK,TRUE,TRUE);
						dValue = max(dV1+dV3,dV2+dV3);
					}
					if(i==4) dValue = pBri->GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_SUBSID,TRUE,TRUE);
					if(i==5)
					{
						dV1    = pBri->GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_TEMP1 ,TRUE,FALSE);
						dV2    = pBri->GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_TEMP2 ,TRUE,FALSE);
						dV3    = pBri->GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_SHRINK,TRUE,FALSE);
						//
						dValue += pBri->GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_DEAD,TRUE,FALSE);
						dValue += pBri->GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_LIVE_FULL,TRUE,FALSE);
						dValue += max(dV1+dV3,dV2+dV3);
						dValue += pBri->GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_SUBSID,TRUE,FALSE);
					}
					str.Format("'%.3f",dValue);
				}
			}
			if(j==nSizeCol-1) str = _T("");

			if(j==0)	XLText->AddText("$n$h");
			nWidthCol = (long)dArrWidth.GetAt(j);
			XLText->AddMergeFormatText(0,nWidthCol-1,"$c%s",str);
			XLText->AddFormatText("$m+0%d",nWidthCol);
		}
	}
	XLText->AddFormatText("$n$h$e�� �������߿� ���� �ݷ��� ���� %s�� ���̸�, �հ�� ������ ��(%s %s)�� ����� ����.", 
		pData->m_szM, pData->GetStrDouble(SlabWidth, UNIT_CGS_M), pData->m_szM);
	XLText->AddFormatText("$n$h$e   ������ ������ħ�� ���� �ݷ��� ��ü�ݷ��� �� ������ ��ħ ������ ���� ���� �����Ѵ�.$n");
	*XLText +=1;
}

void COutDesignBridge::MakeExcelCalcShoeReaction_3DPlate(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	if(pBri->IsRcSlab()==FALSE && pBri->IsPiRahmen()==FALSE) return;

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	CStringArray szArrTitle;

	szArrTitle.Add(_T("�� ��"));
	if(bLsd)
	{
		szArrTitle.Add(_T("��������(DC)"));
		szArrTitle.Add(_T("��������(DW)"));
	}
	else
	{
		szArrTitle.Add(_T("��������"));
	}
	szArrTitle.Add(_T("Ȱ �� ��"));
	szArrTitle.Add(_T("�µ�����"));
	szArrTitle.Add(_T("����ħ��"));
	szArrTitle.Add(_T("��"));

	int nWidthJoint1[] = {4, 4, 4, 4, 4, 4, 4};
	int nWidthJoint2[] = {3, 1, 4, 4, 4, 4, 4, 4};

	long nQtyJijum = pBri->GetCountJijum();
	long nQtyShoe  = 0;
	long nJ=0; for(nJ=0; nJ<nQtyJijum; nJ++)
	{
		CJijum *pJijum = pBri->GetJijum(nJ);
		nQtyShoe += pJijum->m_nQtyShoe[0];
		nQtyShoe += pJijum->m_nQtyShoe[1];
	}

	long nSizeRow = nQtyShoe + 1;
	long nSizeCol = bLsd? 8 : 7;
	long nSizeExcelCol = 24;

	CString str = _T("");

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddTable(nSizeRow-1,nSizeExcelCol-1,1,0);
	long j=0; for(j=0; j<nSizeCol-1; j++)
	{
		XLText->AddMergeFormatText(0,nWidthJoint1[j]-1,"$c%s",szArrTitle.GetAt(j));
		XLText->AddFormatText("$m+0%d",nWidthJoint1[j]);
	}

	double dValue=0, dV1=0, dV2=0, dV3=0;
	for(nJ=0; nJ<nQtyJijum; nJ++)
	{
		CJijum *pJijum = pBri->GetJijum(nJ);
		long nQtyShoeJijum  = 0;
		nQtyShoeJijum += pJijum->m_nQtyShoe[0];
		nQtyShoeJijum += pJijum->m_nQtyShoe[1];
		if(nQtyShoeJijum==0) continue;

		for(long nShoe=0; nShoe<nQtyShoeJijum; nShoe++)
		{
			for(j=0; j<nSizeCol; j++)
			{
				dValue = 0;
				if(j==0)
				{
					if(nJ==0)
						str = _T("����1");
					else if(nJ==nQtyJijum-1) 
						str = _T("����2");
					else
						str.Format("����%d",nJ);
					if(nShoe>0) str = _T("");
				}
				else if(j==1)
				{
					str.Format("%d",nShoe+1);
				}
				else
				{
					if(bLsd)
					{
						if(j==2) dValue = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_DC1,FALSE,TRUE);
						if(j==3) dValue = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_DW1,FALSE,TRUE);
						if(j==4) dValue = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_LL1,FALSE,TRUE);
						if(j==5) 
						{
							dV1    = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_TU21 ,FALSE,TRUE);
							dV2    = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_TU22 ,FALSE,TRUE);
							dV3    = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_SH,FALSE,TRUE);
							dValue = max(dV1+dV3,dV2+dV3);
						}
						if(j==6) dValue = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_SD,FALSE,TRUE);
						if(j==7) 
						{
							dV1    = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_TU21 ,FALSE,FALSE);
							dV2    = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_TU22 ,FALSE,FALSE);
							dV3    = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_SH,FALSE,FALSE);
							//
							dValue += pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_DC1,FALSE,FALSE);
							dValue += pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_DW1,FALSE,FALSE);
							dValue += pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_LL1,FALSE,FALSE);
							dValue += max(dV1+dV3,dV2+dV3);
							dValue += pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_SD,FALSE,FALSE);
						}
						str.Format("'%.3f",dValue);
					}
					else
					{
						if(j==2) dValue = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_DEAD,FALSE,TRUE);
						if(j==3) dValue = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_LIVE_FULL,FALSE,TRUE);
						if(j==4) 
						{
							dV1    = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_TEMP1 ,FALSE,TRUE);
							dV2    = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_TEMP2 ,FALSE,TRUE);
							dV3    = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_SHRINK,FALSE,TRUE);
							dValue = max(dV1+dV3,dV2+dV3);
						}
						if(j==5) dValue = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_SUBSID,FALSE,TRUE);
						if(j==6) 
						{
							dV1    = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_TEMP1 ,FALSE,FALSE);
							dV2    = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_TEMP2 ,FALSE,FALSE);
							dV3    = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_SHRINK,FALSE,FALSE);
							//
							dValue += pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_DEAD,FALSE,FALSE);
							dValue += pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_LIVE_FULL,FALSE,FALSE);
							dValue += max(dV1+dV3,dV2+dV3);
							dValue += pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_SUBSID,FALSE,FALSE);
						}
						str.Format("'%.3f",dValue);
					}
				}
				if(j==0)	XLText->AddText("$n$h");
				if(nShoe==0 && j==0) XLText->AddFormatText("$md%d",nQtyShoeJijum-1);

				XLText->AddMergeFormatText(0,nWidthJoint2[j]-1,"$c%s",str);
				XLText->AddFormatText("$m+0%d",nWidthJoint2[j]);
			}
		}
	}

	*XLText +=2;
}

void COutDesignBridge::MakeExcelCalcShoeReaction_Apply(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	if(pBri->IsRcSlab()==FALSE && pBri->IsPiRahmen()==FALSE) return;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	CStringArray szArrTitle;

	szArrTitle.Add(_T("�� ��"));
	if(bLsd)
	{
		szArrTitle.Add(_T("��������(DC)"));
		szArrTitle.Add(_T("��������(DW)"));
	}
	else
	{
		szArrTitle.Add(_T("��������"));
	}
	szArrTitle.Add(_T("Ȱ �� ��"));
	szArrTitle.Add(_T("�µ�����"));
	szArrTitle.Add(_T("����ħ��"));
	szArrTitle.Add(_T("��"));

	int nWidthJoint1[] = {4, 4, 4, 4, 4, 4, 4};
	int nWidthJoint2[] = {3, 1, 4, 4, 4, 4, 4, 4};

	long nQtyJijum = pBri->GetCountJijum();
	long nQtyShoe  = 0;
	long nJ=0; for(nJ=0; nJ<nQtyJijum; nJ++)
	{
		CJijum *pJijum = pBri->GetJijum(nJ);
		nQtyShoe += pJijum->m_nQtyShoe[0];
		nQtyShoe += pJijum->m_nQtyShoe[1];
	}

	long nSizeRow = nQtyShoe + 1;
	long nSizeCol = bLsd? 8 : 7;
	long nSizeExcelCol = bLsd? 22 : 24;

	if(bLsd)
	{
		for(long ix=1; ix<nSizeCol-1; ++ix)
			nWidthJoint1[ix] = 3;
		for(long ix=2; ix<nSizeCol; ++ix)
			nWidthJoint2[ix] = 3;
	}

	CString str = _T("");

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddTable(nSizeRow-1,nSizeExcelCol-1,1,0);
	long j=0; for(j=0; j<nSizeCol-1; j++)
	{
		XLText->AddMergeFormatText(0,nWidthJoint1[j]-1,"$c%s",szArrTitle.GetAt(j));
		XLText->AddFormatText("$m+0%d",nWidthJoint1[j]);
	}

	double SlabWidth = bLsd? frM(m_pCalc->GetSlabWidth()) : m_pCalc->GetSlabWidth();
	double dValue=0, dV1=0, dV2=0, dV3=0;
	for(nJ=0; nJ<nQtyJijum; nJ++)
	{
		CJijum *pJijum = pBri->GetJijum(nJ);
		long nQtyShoeJijum  = 0;
		nQtyShoeJijum += pJijum->m_nQtyShoe[0];
		nQtyShoeJijum += pJijum->m_nQtyShoe[1];
		if(nQtyShoeJijum==0) continue;

		for(long nShoe=0; nShoe<nQtyShoeJijum; nShoe++)
		{
			for(j=0; j<nSizeCol; j++)
			{
				dValue = 0;
				if(j==0)
				{
					if(nJ==0)
						str = _T("����1");
					else if(nJ==nQtyJijum-1) 
						str = _T("����2");
					else
						str.Format("����%d",nJ);
					if(nShoe>0) str = _T("");
				}
				else if(j==1)
				{
					str.Format("%d",nShoe+1);
				}
				else
				{
					BOOL b2DFrame = pBri->IsShoeReaction2DApply(nJ,nShoe);
					double dLength = b2DFrame ? SlabWidth / (double)nQtyShoeJijum : 1.00;

					if(bLsd)
					{
						if(j==2) dValue = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_DC1,b2DFrame,TRUE);
						if(j==3) dValue = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_DW1,b2DFrame,TRUE);
						if(j==4) dValue = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_LL1,b2DFrame,TRUE);
						if(j==5) 
						{
							dV1    = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_TU21 ,b2DFrame,TRUE);
							dV2    = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_TU22 ,b2DFrame,TRUE);
							dV3    = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_SH,b2DFrame,TRUE);
							dValue = max(dV1+dV3,dV2+dV3);
						}
						if(j==6) dValue = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_SD,b2DFrame,TRUE);
						if(j==7) 
						{
							dV1    = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_TU21 ,b2DFrame,TRUE);
							dV2    = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_TU22 ,b2DFrame,TRUE);
							dV3    = pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_SH,b2DFrame,TRUE);
							//
							dValue += pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_DC1,b2DFrame,TRUE);
							dValue += pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_DW1,b2DFrame,TRUE);
							dValue += pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_LL1,b2DFrame,TRUE);
							dValue += max(dV1+dV3,dV2+dV3);
							dValue += pBri->GetShoeReactionLsd(nJ,nShoe,eLSD_LOADCASE_SD,b2DFrame,TRUE);
						}
						str.Format("'%.0f",dValue*dLength);
					}
					else
					{
						if(j==2) dValue = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_DEAD,b2DFrame,TRUE);
						if(j==3) dValue = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_LIVE_FULL,b2DFrame,TRUE);
						if(j==4) 
						{
							dV1    = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_TEMP1 ,b2DFrame,TRUE);
							dV2    = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_TEMP2 ,b2DFrame,TRUE);
							dV3    = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_SHRINK,b2DFrame,TRUE);
							dValue = max(dV1+dV3,dV2+dV3);
						}
						if(j==5) dValue = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_SUBSID,b2DFrame,TRUE);
						if(j==6) 
						{
							dV1    = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_TEMP1 ,b2DFrame,TRUE);
							dV2    = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_TEMP2 ,b2DFrame,TRUE);
							dV3    = pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_SHRINK,b2DFrame,TRUE);
							//
							dValue += pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_DEAD,b2DFrame,TRUE);
							dValue += pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_LIVE_FULL,b2DFrame,TRUE);
							dValue += max(dV1+dV3,dV2+dV3);
							dValue += pBri->GetShoeReactionUsd(nJ,nShoe,BRIDGE_LOAD_SUBSID,b2DFrame,TRUE);
						}
						str.Format("'%.3f",dValue*dLength);
					}
				}
				if(j==0)	XLText->AddText("$n$h");
				if(nShoe==0 && j==0) XLText->AddFormatText("$md%d",nQtyShoeJijum-1);

				XLText->AddMergeFormatText(0,nWidthJoint2[j]-1,"$c%s",str);
				XLText->AddFormatText("$m+0%d",nWidthJoint2[j]);
			}
		}
	}
	if(pBri->m_nSelect3DShellTool != 2)
	{
		XLText->AddFormatText("$n$h�� ����ݷ��� 2D,3D �ؼ������ ū ���� ������ ������ħ�� �����Ѵ�. (���� : %s)",pData->m_szTonf);
	}
	*XLText +=2;
}

void COutDesignBridge::MakeExcelCalcShoeHoriForce(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	if(pBri->IsRcSlab()==FALSE && pBri->IsPiRahmen()==FALSE) return;

	CStringArray sArrTitle;
	CDoubleArray dArrWidth;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	BOOL bLsd = pData->IsLsdDesign();
	long nQtyJijum = pBri->IsPiRahmen() ? 2 : pBri->GetCountJijum();

	long nSizeRow = 3;
	long nSizeCol = nQtyJijum + 2;
	long nSizeExcelCol = 24;

	CString str = _T("");
	if(pBri->IsPiRahmen() || nQtyJijum==2)
	{
		sArrTitle.Add("�� ��");
		sArrTitle.Add("����1");
		sArrTitle.Add("����2");
		sArrTitle.Add("�� ��");
		dArrWidth.Add(4);
		dArrWidth.Add(7);
		dArrWidth.Add(7);
		dArrWidth.Add(6);
	}
	else
	{
		sArrTitle.Add("�� ��");
		sArrTitle.Add("����1");
		long i=0; for(i=1; i<nQtyJijum-1; i++)
		{
			str.Format("����%d",i);
			sArrTitle.Add(str);
		}
		sArrTitle.Add("����2");
		sArrTitle.Add("�� ��");
		for(i=0; i<nSizeCol; i++)
			dArrWidth.Add(4);
		nSizeExcelCol = nSizeCol*4;
	}

	long nWidthCol = 0;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddTable(nSizeRow-1,nSizeExcelCol-1,1,0);

	long nSizeStr = sArrTitle.GetSize();
	long j=0; for(j=0; j<nSizeCol; j++)
	{
		if(j > nSizeStr-1) break;
		nWidthCol = (long)dArrWidth.GetAt(j);
		str = sArrTitle.GetAt(j);
		XLText->AddMergeFormatText(0,nWidthCol-1,"$c%s",str);
		XLText->AddFormatText("$m+0%d",nWidthCol);
	}

	long nJijum = 0;
	double dValue=0;
	double SlabWidth = bLsd? frM(m_pCalc->GetSlabWidth()) : m_pCalc->GetSlabWidth();

	for(long i=1; i<nSizeRow; i++)
	{
		for(j=0; j<nSizeCol; j++)
		{
			dValue = 0;
			if(j==0)
			{
				if(i==1) str = "��ü �����";
				if(i==2) str = "��ħ �����";
			}
			else 
			{
				if(j==1) // ����1
					nJijum = 0;
				else if(j==nSizeCol-2) // ����2
					nJijum = pBri->GetCountJijum()-1;
				else
					nJijum = j-1;
				if(i==1) dValue = pBri->GetShoeHoriForce(nJijum, -1);
				if(i==2) dValue = pBri->GetShoeHoriForce(nJijum, 0);
				str.Format("'%.3f",dValue);
			}
			if(j==nSizeCol-1) str = _T("");

			if(j==0)	XLText->AddText("$n$h");
			nWidthCol = (long)dArrWidth.GetAt(j);
			XLText->AddMergeFormatText(0,nWidthCol-1,"$c%s",str);
			XLText->AddFormatText("$m+0%d",nWidthCol);
		}
	}
	XLText->AddFormatText("$n$h$e�� ��ü ������� ������ ����(%s %s)�� ���� ����.", 
		pData->GetStrDouble(SlabWidth, UNIT_CGS_M), pData->m_szM);
	XLText->AddFormatText("$n$h$e   ��ħ ������� ��ü������� �� ������ ��ħ ������ ���� ���� �����Ѵ�.$n");
	*XLText +=1;
}
void COutDesignBridge::MakeExcelArchRib(CXLControl *pXL, CRcBridgeRebar *pBri)
{
	CXLFormatText XLText(pXL);
	
	long nT_0 = 0;
	long nIndexMiddle = 1;
	XLText(nT_0);
	XLText.AddFormatText("$tb13%d. ��ġ���� ����$n",m_nIndexTitle);
	XLText += 1;
	
	m_pCalc->m_pArchRib->WriteCalcArchRibDesign(&XLText, pBri, m_nIndexTitle,nIndexMiddle++);
	XLText += 1;
}

void COutDesignBridge::MakeTableSectionForce(CXLFormatText *XLText, CRcBridgeRebar *pBri, ETypeEnvComboLsd eEnvTypeLsd)
{
	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();
	BOOL bGirderCalc = FALSE;
	BOOL bOutSideWall= TRUE; // TRUE-���,FALSE-����

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
//	CString szUnit_CGS = _T("");
// 	CString szUnit_Moment = _T("");
// 	CString szUnit_Force = _T("");
	CString str1, str2;

	CString szUnit_Moment = pData->GetStringUnitType(UNIT_CGS_TONFM);
	CString szUnit_Force  = pData->GetStringUnitType(UNIT_CGS_TONF);

	BOOL bLsd = pData->IsLsdDesign();
	BOOL b3D = (pBri->m_nSelect3DShellTool > 1)? FALSE : TRUE;
	BOOL b3DLoad = pBri->m_FemShell.IsOutputLoaded();

	// ����� �Է½� 3D����� ���� �ʾҴ��� ����Ѵ�.
	// (ARCBRIDGE-3481) �ؼ��� ���� ���� ������ ������Է� ���� �������� �ʵ��� ����
// 	if(pBri->m_bUseShellUserOutput)
// 	{
// 		b3D		= TRUE;
// 		b3DLoad	= TRUE;
// 	}
	// table�� ��ü row��
	long nRowtable = 0;
	long nCntInWall = (nJijumConnect>-1)? pBri->GetCountInWallReal() -1 : pBri->GetCountInWallReal();
	long nCntRowInWall = 0;
	long nCntRowArchRib = pBri->m_pArrConcArchRib.GetSize();
	long nCntRowPRF = pBri->m_bExistProtectionRiseFooting ? 2 : 0;
	
	if(pBri->m_nSelectInWallSectionDesign == 0)
	{
		for(long i=0; i<nCntInWall;i++)
		{
			CWallApp *pWall = pBri->GetInWall(i);
			if(pWall->m_bTopIsColumn && b3D)
				nCntRowInWall += pWall->m_nCountColumn;
			else
			{
				if(pBri->m_nSelectInWallSectionDesign == 0)
					nCntRowInWall += 5;
			}
		}
	}
	
	BOOL bBoxType = pBri->IsBoxType();
	long nRowTitle = bLsd? 2 : 3;
	long nRowTitleLoof = 3;
	long nRowUpperSlab = pBri->m_nQtyJigan * 2 + 1;
	long nRowLowerSlab = bBoxType ? nRowUpperSlab : 0;
	long nCntRow = nRowTitle + nRowUpperSlab + nRowLowerSlab; // 3 : Ÿ��Ʋ
	if(pBri->IsOutsideWall() && (pBri->m_nSelectWallSectionDesign == 0 || (bLsd == FALSE && pBri->m_bOutWallUsebilityCheck)))
	{
		nCntRow += 5*2;
	}
	long nRowBeforInWall = nCntRow + nCntRowArchRib;
	nRowtable = nCntRow + nCntRowInWall + nCntRowArchRib + nCntRowPRF;

	// (ARCBRIDGE-3493) LSD�϶� 3D���谪 ������ ���밪 ǥ��
	XLText->AddTable(nRowtable-1, 25-1, bLsd? 2 : 3, 7);

	//////////////////////////////////////////////////////////////////////////
	// title
	
	
	long nColAr[] = {3, 4, 7};
	if(bLsd)
	{
		nColAr[0] = b3D ? 4 : 3;
		nColAr[1] = 0;
		nColAr[2] = 5;
	}

	long i=0; for(i=0; i<nRowTitleLoof; i++)
	{
		if(i==0)
		{
			str1 = _T("�ڸ��Ʈ(") + szUnit_Moment + (bLsd? _T(") / mm") : _T(") / m"));
			str2 = _T("���ܷ�(") + szUnit_Force + (bLsd? _T(") / mm") : _T(") / m"));
			char *Title[] = {"�� ��", str1.GetBuffer(str1.GetLength()), str2.GetBuffer(str2.GetLength()), "���밪"};
			
			int nWidthJoin[] = {7, 12, 6 ,0};
			if(bLsd)
			{
				nWidthJoin[0] = b3D ? 7 : 9;
				nWidthJoin[1] = b3D ? 6 : 8;
				nWidthJoin[2] = b3D ? 6 : 8;
				nWidthJoin[3] = b3D ? 6 : 8;
			}

			long j=0; for(j=0; j<nColAr[i]; j++)
			{
				if(j == 0) XLText->AddMergeFormatText(0,nWidthJoin[j]-1,"$c%s $md%d",Title[j], nRowTitle-1);
				else XLText->AddMergeFormatText(0,nWidthJoin[j]-1,"$c%s",Title[j]);

				XLText->AddFormatText("$m+%02d",nWidthJoin[j]);
			}
		}
		else if(i==1)
		{
			if(bLsd)
				continue;

			char *Title[] = {"�� ��", "�������", "�������", "�������"};
			int nWidthJoin[] = {7, 6, 6, 6};
			long j=0; for(j=0; j<nColAr[i]; j++)
			{
				XLText->AddMergeFormatText(0,nWidthJoin[j]-1,"$c%s",Title[j]);
				XLText->AddFormatText("$m+0%d",nWidthJoin[j]);
			}
		}
		else if(i==2)
		{
			char *Title[] = {"�� ��", "2D FRAME", "3D PLATE", "2D FRAME", "3D PLATE", bLsd? "�ڸ��Ʈ" : "2D FRAME", bLsd? "���ܷ�" : "3D PLATE"};
			int nWidthJoin[] = {7, 3, 3, 3, 3, 3, 3};
			if(bLsd)
			{
				nWidthJoin[0] = b3D ? 7 : 9;
				nWidthJoin[1] = b3D ? 3 : 4;
				nWidthJoin[2] = b3D ? 3 : 4;
				nWidthJoin[3] = b3D ? 3 : 4;
				nWidthJoin[4] = b3D ? 3 : 4;
				nWidthJoin[5] = b3D ? 3 : 4;
				nWidthJoin[6] = b3D ? 3 : 4;
			}

			long j=0; for(j=0; j<1; j++)
			{
				XLText->AddMergeFormatText(0,nWidthJoin[j]-1,"$c%s",Title[j]);
				XLText->AddFormatText("$m+0%d",nWidthJoin[j]);
			}
			for(j=0; j<((bLsd && !b3D)? 2 : 3); j++)
			{	
				if(b3D)
				{
					XLText->AddMergeFormatText(0,nWidthJoin[1+j*2]-1,"$c%s",Title[1+j*2]);
					XLText->AddFormatText("$m+0%d",nWidthJoin[1+j*2]);

					XLText->AddMergeFormatText(0,nWidthJoin[2+j*2]-1,"$c%s",Title[2+j*2]);
					XLText->AddFormatText("$m+0%d",nWidthJoin[2+j*2]);
				}
				else
				{
					XLText->AddMergeFormatText(0,(nWidthJoin[1+j*2]*2)-1,"$c%s",Title[1+j*2]);
					XLText->AddFormatText("$m+0%d",(nWidthJoin[1+j*2]*2));
				}
			}
		}
		XLText->AddText("$n$h");
	}
	//////////////////////////////////////////////////////////////////////////

	long nSizeCol = (bLsd && b3D) ? 10 : 8;
	int nWidthJoint[] = { 3, 4, 3, 3, 3, 3, 3, 3, 3, 3 };
	if(bLsd && !b3D)
	{
		nWidthJoint[0] = 3;
		nWidthJoint[1] = 6;
		nWidthJoint[2] = 4;
		nWidthJoint[3] = 4;
		nWidthJoint[4] = 4;
		nWidthJoint[5] = 4;
		nWidthJoint[6] = 4;
		nWidthJoint[7] = 4;
	}
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)
	{
		nWidthJoint[0] = 0;
		nWidthJoint[1] = (bLsd && !b3D)? 9 : 7;
	}

	CString str = _T("");
	CString strsub = _T("");
	CString strOpt = _T("");
	long nMergeRow = 0;
	long nJijumIndex = 0;
// 	long nSpan = 0;
	double dValue(0);
	double dApplyM(0), dApplyS(0);

	BOOL bUpperSlab = TRUE;
// 	long nIndexSlab = 0; // �����꿡���� ���� Index
	for(i=nRowTitle; i<nRowBeforInWall; i++)
	{
		bUpperSlab = (i < nRowUpperSlab + nRowTitle) ? TRUE : FALSE;
		long nIndexSlab = bUpperSlab ? i - nRowTitle : i - nRowTitle - nRowUpperSlab;
		long nSpan = (nIndexSlab - 1) / 2;

		double dDistShearLeft(0), dDistShearRight(0);
		if(i < nRowUpperSlab + nRowLowerSlab + nRowTitle)
		{
			dDistShearLeft = pBri->GetDistCheckShearSlab(nSpan, CHECK_POS_LEFT, bUpperSlab, NULL);
			dDistShearRight = pBri->GetDistCheckShearSlab(nSpan, CHECK_POS_RIGHT, bUpperSlab, NULL);
		}
		for(long j=0; j<nSizeCol; j++)
		{
			nMergeRow = 0;
			//////////////////////////////////////////////////////////////////////////
			// ������
			if(i < nRowUpperSlab + nRowLowerSlab + nRowTitle)
			{
				if(pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB && j==0) continue;
				else if(j==0)
				{
					str = bBoxType ? (bUpperSlab ? _T("��� ������") : _T("�Ϻ� ������")) : _T("������");
					if(i==nRowTitle) 
						nMergeRow = nRowUpperSlab - 1;
					else if(bBoxType && (i == nRowTitle + nRowUpperSlab))
						nMergeRow = nRowLowerSlab - 1;
					else 
						nMergeRow = 0;
				}
				else if(j==1)
				{
					if(nIndexSlab == 0) 
						str = "�����ܺ�";
					else if(nIndexSlab == nRowUpperSlab - 1)  
						str = "�����ܺ�";
					else if(nIndexSlab%2 == 0) 
						str.Format("�߰�����%d", nIndexSlab/2);
					else if(nIndexSlab%2 == 1) 
						str.Format("�߾Ӻ�(����%d)", nIndexSlab/2 + 1);
					strOpt = _T("");
				}
				else if(j==2)		// ������� 2D FRAME ���Ʈ
				{
					if(nIndexSlab == 0) 
						dValue = pBri->GetOutValueSlab(nSpan, CHECK_POS_LEFT, COMBO_ULTIMATE, ELE_MOMENT2, TRUE, FALSE, bUpperSlab, eEnvTypeLsd);
					else if(nIndexSlab == nRowUpperSlab - 1) 
						dValue = pBri->GetOutValueSlab(nSpan, CHECK_POS_RIGHT, COMBO_ULTIMATE, ELE_MOMENT2, TRUE, FALSE, bUpperSlab, eEnvTypeLsd);
					else if(nIndexSlab%2 == 0) 
					{
						double dDataR = pBri->GetOutValueSlab(nSpan, CHECK_POS_RIGHT, COMBO_ULTIMATE, ELE_MOMENT2, TRUE, FALSE, bUpperSlab, eEnvTypeLsd);
						double dDataL = pBri->GetOutValueSlab(nSpan+1, CHECK_POS_LEFT, COMBO_ULTIMATE, ELE_MOMENT2, TRUE, FALSE, bUpperSlab, eEnvTypeLsd);
						dValue = (fabs(dDataR)>fabs(dDataL)) ? dDataR : dDataL;
					}
					else if(nIndexSlab%2 == 1) 
						dValue = pBri->GetOutValueSlab(nSpan, CHECK_POS_CENTER, COMBO_ULTIMATE, ELE_MOMENT2, TRUE, FALSE, bUpperSlab, eEnvTypeLsd);
					str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);
					strOpt = _T("");

					dApplyM = dValue;
				}
				else if(j==3 && b3D)		// ������� 3D SHELL
				{
					if(b3DLoad)
					{
						if(nIndexSlab == 0) 
							dValue = pBri->GetOutValueSlabShell(nSpan, CHECK_POS_LEFT, COMBO_ULTIMATE, ELE_MOMENT2, bUpperSlab, eEnvTypeLsd);
						else if(nIndexSlab == nRowUpperSlab - 1) 
							dValue = pBri->GetOutValueSlabShell(nSpan, CHECK_POS_RIGHT, COMBO_ULTIMATE, ELE_MOMENT2, bUpperSlab, eEnvTypeLsd);
						else if(nIndexSlab%2 == 0)
						{
							double dDataR = pBri->GetOutValueSlabShell(nSpan, CHECK_POS_RIGHT, COMBO_ULTIMATE, ELE_MOMENT2, bUpperSlab, eEnvTypeLsd);
							double dDataL = pBri->GetOutValueSlabShell(nSpan+1, CHECK_POS_LEFT, COMBO_ULTIMATE, ELE_MOMENT2, bUpperSlab, eEnvTypeLsd);
							dValue = (fabs(dDataR)>fabs(dDataL)) ? dDataR : dDataL;
						}
						else if(nIndexSlab%2 == 1) 
							dValue = pBri->GetOutValueSlabShell(nSpan, CHECK_POS_CENTER, COMBO_ULTIMATE, ELE_MOMENT2, bUpperSlab, eEnvTypeLsd);

						str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);

						if(fabs(dApplyM) < fabs(dValue))
							dApplyM = dValue;
					}
					else
					{
						str = _T("-");
					}
					strOpt = _T("");
				}
				else if(j==4)		// ������� 2D FRAME ���Ʈ
				{
					if(bLsd) continue;

					if(nIndexSlab == 0) 
						str.Format("$f3%.3f",pBri->GetOutValueSlab(nSpan, CHECK_POS_LEFT, COMBO_SERVICE, ELE_MOMENT2, TRUE, FALSE, bUpperSlab, eEnvTypeLsd));
					else if(nIndexSlab == nRowUpperSlab - 1) 
						str.Format("$f3%.3f",pBri->GetOutValueSlab(nSpan, CHECK_POS_RIGHT, COMBO_SERVICE, ELE_MOMENT2, TRUE, FALSE, bUpperSlab, eEnvTypeLsd));
					else if(nIndexSlab%2 == 0)
					{
						double dDataR = pBri->GetOutValueSlab(nSpan, CHECK_POS_RIGHT, COMBO_SERVICE, ELE_MOMENT2, TRUE, FALSE, bUpperSlab, eEnvTypeLsd);
						double dDataL = pBri->GetOutValueSlab(nSpan+1, CHECK_POS_LEFT, COMBO_SERVICE, ELE_MOMENT2, TRUE, FALSE, bUpperSlab, eEnvTypeLsd);
						str.Format("$f3%.3f", (fabs(dDataR)>fabs(dDataL))? dDataR:dDataL);
					}
					else if(nIndexSlab%2 == 1) 
						str.Format("$f3%.3f",pBri->GetOutValueSlab(nSpan, CHECK_POS_CENTER, COMBO_SERVICE, ELE_MOMENT2, TRUE, FALSE, bUpperSlab, eEnvTypeLsd));
					strOpt = _T("");
				}
				else if(j==5 && b3D)		// ������� 3D SHELL
				{
					if(bLsd) continue;

					if(nIndexSlab == 0) 
						str.Format("$f3%.3f",pBri->GetOutValueSlabShell(nSpan, CHECK_POS_LEFT, COMBO_SERVICE, ELE_MOMENT2, bUpperSlab));
					else if(nIndexSlab == nRowUpperSlab - 1) 
						str.Format("$f3%.3f",pBri->GetOutValueSlabShell(nSpan, CHECK_POS_RIGHT, COMBO_SERVICE, ELE_MOMENT2, bUpperSlab));
					else if(nIndexSlab%2 == 0)
					{
						double dDataR = pBri->GetOutValueSlabShell(nSpan, CHECK_POS_RIGHT, COMBO_SERVICE, ELE_MOMENT2, bUpperSlab);
						double dDataL = pBri->GetOutValueSlabShell(nSpan+1, CHECK_POS_LEFT, COMBO_SERVICE, ELE_MOMENT2, bUpperSlab);
						str.Format("$f3%.3f", (fabs(dDataR)>fabs(dDataL))? dDataR:dDataL);
					}
					else if(nIndexSlab%2 == 1) 
						str.Format("$f3%.3f",pBri->GetOutValueSlabShell(nSpan, CHECK_POS_CENTER, COMBO_SERVICE, ELE_MOMENT2, bUpperSlab));
					strOpt = _T("");
					if(!b3DLoad) str = _T("-");
				}
				else if(j==6)		// ������� 2D FRAME ���ܷ�
				{
					if(nIndexSlab == 0) 
						dValue = pBri->GetOutValueSlab(nSpan, CHECK_POS_LEFT, COMBO_ULTIMATE, ELE_SHEAR_2, TRUE, FALSE, bUpperSlab, eEnvTypeLsd, dDistShearLeft);
					else if(nIndexSlab == nRowUpperSlab - 1) 
						dValue = pBri->GetOutValueSlab(nSpan, CHECK_POS_RIGHT, COMBO_ULTIMATE, ELE_SHEAR_2, TRUE, FALSE, bUpperSlab, eEnvTypeLsd, dDistShearRight);
					else if(nIndexSlab%2 == 0)
					{
						double dDataR = pBri->GetOutValueSlab(nSpan, CHECK_POS_RIGHT, COMBO_ULTIMATE, ELE_SHEAR_2, TRUE, FALSE, bUpperSlab, eEnvTypeLsd, dDistShearRight);
						double dDataL = pBri->GetOutValueSlab(nSpan+1, CHECK_POS_LEFT, COMBO_ULTIMATE, ELE_SHEAR_2, TRUE, FALSE, bUpperSlab, eEnvTypeLsd, dDistShearRight);
						dValue = (fabs(dDataR)>fabs(dDataL)) ? dDataR : dDataL;
					}
					else if(nIndexSlab%2 == 1) 
						dValue = pBri->GetOutValueSlab(nSpan, CHECK_POS_CENTER, COMBO_ULTIMATE, ELE_SHEAR_2, TRUE, FALSE, bUpperSlab, eEnvTypeLsd);

					str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);
					strOpt = _T("");

					dApplyS = dValue;
				}
				else if(j==7 && b3D)		// ������� 3D SHELL ���ܷ�
				{
					if(b3DLoad)
					{
						if(nIndexSlab == 0) 
							dValue = pBri->GetOutValueSlabShell(nSpan, CHECK_POS_LEFT, COMBO_ULTIMATE, ELE_SHEAR_2, bUpperSlab, eEnvTypeLsd);
						else if(nIndexSlab == nRowUpperSlab - 1) 
							dValue = pBri->GetOutValueSlabShell(nSpan, CHECK_POS_RIGHT, COMBO_ULTIMATE, ELE_SHEAR_2, bUpperSlab, eEnvTypeLsd);
						else if(nIndexSlab%2 == 0)
						{
							double dDataR = pBri->GetOutValueSlabShell(nSpan, CHECK_POS_RIGHT, COMBO_ULTIMATE, ELE_SHEAR_2, bUpperSlab, eEnvTypeLsd);
							double dDataL = pBri->GetOutValueSlabShell(nSpan+1, CHECK_POS_LEFT, COMBO_ULTIMATE, ELE_SHEAR_2, bUpperSlab, eEnvTypeLsd);
							dValue = (fabs(dDataR)>fabs(dDataL)) ? dDataR : dDataL;
						}
						else if(nIndexSlab%2 == 1) 
							str.Format("$f3%.3f",pBri->GetOutValueSlabShell(nSpan, CHECK_POS_CENTER, COMBO_ULTIMATE, ELE_SHEAR_2, bUpperSlab, eEnvTypeLsd));

						str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);

						if(fabs(dApplyS) < fabs(dValue))
							dApplyS = dValue;
					}
					else
					{
						str = _T("-");
					}
					strOpt = _T("");
				}
				else if(j==8 && b3D)	//	���밪
				{
					str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dApplyM);
				}
				else if(j==9 && b3D)
				{
					str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dApplyS);
				}
				else
				{
					str = "";
					strOpt = _T("");
				}
			}
			//////////////////////////////////////////////////////////////////////////
			// ������ü, ������ü
			else if((i<nRowUpperSlab + nRowLowerSlab + nRowTitle + 10) && (pBri->IsOutsideWall() && (pBri->m_nSelectWallSectionDesign == 0 || pBri->m_bOutWallUsebilityCheck)))
			{
				if(pBri->IsOutsideWall() && i < nRowUpperSlab + nRowLowerSlab + nRowTitle + 10) 
					nJijumIndex =(i < nRowUpperSlab + nRowLowerSlab + nRowTitle + 5) ? 0 : pBri->GetCountJijum()-1;
				else 
					nJijumIndex = i-nRowBeforInWall+1;

				long nIndexRow = (i-(nRowUpperSlab + nRowLowerSlab + nRowTitle)) % 5;
				str = GetStringSectionForceTableAtWall(pBri, eEnvTypeLsd, nJijumIndex, nIndexRow, j, nMergeRow, strOpt, bLsd, b3D, b3DLoad);

				if(bLsd && (j==4 || j== 5))
					continue;
			}
			else if((i<nRowBeforInWall) && pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)		// ��ġ����
			{
				long nIdxCrown = (nJijumConnect * 2) + 1;
				long nIndexArchRib = i - (nRowUpperSlab + nRowLowerSlab + nRowTitle + 10);
				long nPosCheck = 0;
				BOOL bCheckCompare = FALSE;
				BOOL bCrown = FALSE;
				pBri->TrimRibByFootingUpper();

				CString szTitle = _T("");
				if(nIndexArchRib==0 || nIndexArchRib==nCntRowArchRib-1)							// ������ SPRINGING
				{
					nSpan = (nIndexArchRib==0)? 0 : (nIndexArchRib-1)/2 - 1;
					szTitle = (nIndexArchRib == 0)? _T("���� SPRINGING") : _T("���� SPRINGING");
					nPosCheck = (nIndexArchRib==0)? CHECK_POS_LEFT : CHECK_POS_RIGHT;
				}
				else if(nIndexArchRib==nIdxCrown-1 || nIndexArchRib==nIdxCrown+1)		// ����,������ ���պ�
				{
					nSpan = (nIndexArchRib==nIdxCrown-1)? nJijumConnect-1 : nJijumConnect;
					szTitle.Format("���������պ�%s",(nIndexArchRib==nIdxCrown-1)? "����":"����");
					nPosCheck = CHECK_POS_RIGHT;
					bCheckCompare = TRUE;
				}
				else if(nIndexArchRib == nIdxCrown)			// CROWN
				{
					bCrown = FALSE;
					nSpan = nJijumConnect-1;
					szTitle = _T("CROWN��");
					nPosCheck = CHECK_POS_RIGHT;
				}
				else if(nIndexArchRib%2 == 1)					// �߰���
				{
					nSpan = (nIndexArchRib < nIdxCrown)? (nIndexArchRib-1)/2 : (nIndexArchRib-1)/2-1;
					szTitle.Format("�߰���%d", nIndexArchRib/2+1);
					nPosCheck = CHECK_POS_CENTER;
				}
				else if(nIndexArchRib%2 == 0)					// ������ ������
				{
					nSpan = (nIndexArchRib < nIdxCrown)? (nIndexArchRib-1)/2 : (nIndexArchRib-1)/2-1;
					szTitle.Format("������ ������%d", nIndexArchRib/2);
					nPosCheck = CHECK_POS_RIGHT;
					bCheckCompare = TRUE;
				}

				if(j==0)
				{
					str = "��ġ����";
//					������ġ�� 10�� �Ѿ�� �ȵ�..
//					if(i==nRowSlab+nRowTitle+10) strOpt.Format(" $md%d", nCntRowArchRib-1);
					if(i == nRowUpperSlab + nRowLowerSlab + nRowTitle + 10) nMergeRow = nCntRowArchRib-1;
					else nMergeRow = 0;
				}
				else if(j==1)
				{
					str = szTitle;
					strOpt = _T("");
				}
				else if(j==2)		// ������� 2D FRAME ���Ʈ
				{
					if(bCheckCompare)
					{
						double dDataR = m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan, CHECK_POS_RIGHT, COMBO_ULTIMATE, ELE_MOMENT2, bCrown);
						double dDataL = m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan+1, CHECK_POS_LEFT, COMBO_ULTIMATE, ELE_MOMENT2, bCrown);
						str.Format("$f3%.3f", (fabs(dDataR)>fabs(dDataL))? dDataR:dDataL);
					}
					else str.Format("$f3%.3f",m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan, nPosCheck, COMBO_ULTIMATE, ELE_MOMENT2, bCrown));
					strOpt = _T("");
				}
				else if(j==3 && b3D)		// ������� 3D SHELL
				{
					if(bCheckCompare)
					{
						double dDataR = pBri->GetOutValueArchRibShell(nSpan, CHECK_POS_RIGHT, COMBO_ULTIMATE, ELE_MOMENT2);
						double dDataL = pBri->GetOutValueArchRibShell(nSpan+1, CHECK_POS_LEFT, COMBO_ULTIMATE, ELE_MOMENT2);
						str.Format("$f3%.3f", (fabs(dDataR)>fabs(dDataL))? dDataR:dDataL);
					}
					else str.Format("$f3%.3f",pBri->GetOutValueArchRibShell(nSpan, nPosCheck, COMBO_ULTIMATE, ELE_MOMENT2));
					strOpt = _T("");
					if(!b3DLoad) str = _T("-");
				}
				else if(j==4)		// ������� 2D FRAME ���Ʈ
				{
					if(bCheckCompare)
					{
						double dDataR = m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan, CHECK_POS_RIGHT, COMBO_SERVICE, ELE_MOMENT2, bCrown);
						double dDataL = m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan+1, CHECK_POS_LEFT, COMBO_SERVICE, ELE_MOMENT2, bCrown);
						str.Format("$f3%.3f", (fabs(dDataR)>fabs(dDataL))? dDataR:dDataL);
					}
					else str.Format("$f3%.3f",m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan, nPosCheck, COMBO_SERVICE, ELE_MOMENT2, bCrown));
					strOpt = _T("");
				}
				else if(j==5 && b3D)		// ������� 3D SHELL
				{
					if(bCheckCompare)
					{
						double dDataR = pBri->GetOutValueArchRibShell(nSpan, CHECK_POS_RIGHT, COMBO_SERVICE, ELE_MOMENT2);
						double dDataL = pBri->GetOutValueArchRibShell(nSpan+1, CHECK_POS_LEFT, COMBO_SERVICE, ELE_MOMENT2);
						str.Format("$f3%.3f", (fabs(dDataR)>fabs(dDataL))? dDataR:dDataL);
					}
					else str.Format("$f3%.3f",pBri->GetOutValueArchRibShell(nSpan, nPosCheck, COMBO_SERVICE, ELE_MOMENT2));
					strOpt = _T("");
					if(!b3DLoad) str = _T("-");
				}
				else if(j==6)		// ������� 2D FRAME ���ܷ�
				{
					if(bCheckCompare)
					{
						double dDataR = m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan, CHECK_POS_RIGHT, COMBO_ULTIMATE, ELE_SHEAR_2, bCrown);
						double dDataL = m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan+1, CHECK_POS_LEFT, COMBO_ULTIMATE, ELE_SHEAR_2, bCrown);
						str.Format("$f3%.3f", (fabs(dDataR)>fabs(dDataL))? dDataR:dDataL);
					}
					else str.Format("$f3%.3f",m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan, nPosCheck, COMBO_ULTIMATE, ELE_SHEAR_2, bCrown));
					strOpt = _T("");
				}
				else if(j==7 && b3D)		// ������� 3D SHELL ���ܷ�
				{
					if(bCheckCompare)
					{
						double dDataR = pBri->GetOutValueArchRibShell(nSpan, CHECK_POS_RIGHT, COMBO_ULTIMATE, ELE_SHEAR_2);
						double dDataL = pBri->GetOutValueArchRibShell(nSpan+1, CHECK_POS_LEFT, COMBO_ULTIMATE, ELE_SHEAR_2);
						str.Format("$f3%.3f", (fabs(dDataR)>fabs(dDataL))? dDataR:dDataL);
					}
					else str.Format("$f3%.3f",pBri->GetOutValueArchRibShell(nSpan, nPosCheck, COMBO_ULTIMATE, ELE_SHEAR_2));
					strOpt = _T("");
					if(!b3DLoad) str = _T("-");
					str = _T("-");
				}
				else
				{
					str = "";
					strOpt = _T("");
				}
			}
		
			if((j==2 && !b3D) || (j==4 && !b3D) || (j==6 && !b3D))
			{
				XLText->AddMergeFormatText(nMergeRow, nWidthJoint[j]*2-1, "$c%s%s", str, strOpt);
				XLText->AddFormatText("$m+0%d", nWidthJoint[j]*2);
			}
			else if((j==3 && !b3D) || (j==5 && !b3D) || (j==7 && !b3D))
			{
			}
			else
			{
				XLText->AddMergeFormatText(nMergeRow, nWidthJoint[j]-1, "$c%s%s", str, strOpt);
				XLText->AddFormatText("$m+0%d", nWidthJoint[j]);
			}
		}
		XLText->AddText("$n$h");
	}

	CWallApp *pWall = NULL;

	BOOL bColumnExist = FALSE;
	for(i=0; i<pBri->GetCountInWallReal(); i++)
	{
		if(pBri->m_nSelectInWallSectionDesign == 1)
			break;

		BOOL bColumn = FALSE;
		nJijumIndex = i+1;
		
		pWall = pBri->GetInWall(i);
		
		if(pWall->m_bTopIsColumn)
		{
			bColumn = TRUE;
			bColumnExist = TRUE;
		}

		if(bColumn == FALSE && pBri->m_nSelectInWallSectionDesign ==0)
		{
			for(long nRowIndex=0; nRowIndex<5; ++nRowIndex)
			{
				for(long j=0; j<nSizeCol; j++)
				{
					if(bLsd && (j==4 || j== 5))
						continue;

					str = GetStringSectionForceTableAtWall(pBri, eEnvTypeLsd, nJijumIndex, nRowIndex, j, nMergeRow, strOpt, bLsd, b3D, b3DLoad);

					if((j==2 && !b3D) || (j==4 && !b3D) || (j==6 && !b3D))
					{
						XLText->AddMergeFormatText(nMergeRow, nWidthJoint[j]*2-1, "$c%s%s", str, strOpt);
						XLText->AddFormatText("$m+0%d", nWidthJoint[j]*2);
					}
					else if((j==3 && !b3D) || (j==5 && !b3D) || (j==7 && !b3D))
					{
					}
					else
					{
						XLText->AddMergeFormatText(nMergeRow, nWidthJoint[j]-1, "$c%s%s", str, strOpt);
						XLText->AddFormatText("$m+0%d", nWidthJoint[j]);
					}
				}
				XLText->AddText("$n$h");	
			}
		}
		else
		{
			if(nJijumIndex == nJijumConnect) continue;
			long nCountRow = (bColumn && b3D)? pWall->m_nCountColumn : 1;
			for(long nIdx=0; nIdx<nCountRow ; nIdx++)
			{
				long j=0; for(j=0; j<nSizeCol; j++)
				{
					// ������ ��, �� �� ���� 10�� ������ ��Ʈ�� �߰��� ���ؼ� �����Ҽ� ����.
					long nMerge = 0;
					strOpt = _T("");

					if(bColumn)
					{
						// long nJijumIndex ---> ������ �ε��� 
						// long nColumnInd  ---> ��� �ε���
						if(j==0)
						{
							str = "�߰���ü";
							if(i==0 && nIdx==0 && nCntRowInWall<10) strOpt.Format(" $md%d", nCntRowInWall-1);
							else if(i==0 && nIdx==0) nMerge = nCntRowInWall-1;
							else strOpt = _T("");
						}
						else if(j==1)
						{
							str.Format("�߰�����%d", nJijumIndex);
							if(b3D) strsub.Format("���%d", nIdx+1);
							else strsub = _T("���");
							if(nIdx == 0 && b3D && pWall->m_nCountColumn<10) strOpt.Format("$md%d", pWall->m_nCountColumn-1);
							else if(nIdx == 0 && b3D) nMerge = pWall->m_nCountColumn-1;
							else strOpt = _T("");
						}
						else if(j==2)				// ������� 2D FRAME ���Ʈ
						{
							dValue = pBri->GetOutValueWall(nJijumIndex, CHECK_POS_BOTTOM, COMBO_ULTIMATE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
							str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);
							dApplyM = dValue;
						}
						else if(j==3 && b3D)				// ������� 3D SHELL ���Ʈ
						{
							if(b3DLoad)
							{
								dValue = pBri->GetOutValueWallShellColumn(nJijumIndex,nIdx, CHECK_POS_BOTTOM, COMBO_ULTIMATE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
								str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);

								if(fabs(dApplyM) < fabs(dValue))
									dApplyM = dValue;
							}
							else
								str = _T("-");
						}
						else if(j==4)				// ������� 2D FRAME ���Ʈ
						{
							if(bLsd) continue;
							str.Format("$f3%.3f",pBri->GetOutValueWall(nJijumIndex, CHECK_POS_BOTTOM, COMBO_SERVICE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd));
						}
						else if(j==5 && b3D)				// ������� 3D SHELL ���Ʈ
						{
							if(bLsd) continue;
							str.Format("$f3%.3f",pBri->GetOutValueWallShellColumn(nJijumIndex, nIdx,CHECK_POS_BOTTOM, COMBO_SERVICE, ELE_MOMENT2, bGirderCalc,bOutSideWall));
							if(!b3DLoad) str = _T("-");
						}
						else if(j==6)				// ������� 2D FRAME ���ܷ�
						{
							dValue = pBri->GetOutValueWall(nJijumIndex, CHECK_POS_BOTTOM, COMBO_ULTIMATE, ELE_SHEAR_2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
							str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);
							dApplyS = dValue;
						}
						else if(j==7 && b3D)				// ������� 3D SHELL ���ܷ�
						{
							if(b3DLoad) 
							{
								dValue = pBri->GetOutValueWallShellColumn(nJijumIndex, nIdx,CHECK_POS_BOTTOM, COMBO_ULTIMATE, ELE_SHEAR_2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
								str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);

								if(fabs(dApplyS) < fabs(dValue))
									dApplyS = dValue;
							}
							else
							{
								str = _T("-");
							}
						}
						else
							str = "";
					}
					else
					{
						if(j==0)
						{
							str = "�߰���ü";
							if(i==0) strOpt.Format(" $md%d", nCntRowInWall-1);
							else strOpt = _T("");
						}
						else if(j==1)
							str.Format("�߰�����%d", nJijumIndex);
						else if(j==2)				// ������� 2D FRAME ���Ʈ
						{
							dValue = pBri->GetOutValueWall(nJijumIndex, CHECK_POS_BOTTOM, COMBO_ULTIMATE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
							str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);
							dApplyM = dValue;
						}
						else if(j==3 && b3D)				// ������� 3D SHELL ���Ʈ
						{
							if(b3DLoad)
							{
								dValue = pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_BOTTOM, COMBO_ULTIMATE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
								if(fabs(dApplyM) < fabs(dValue))
									dApplyM = dValue;
							}
							else
								str = _T("-");
						}
						else if(j==4)				// ������� 2D FRAME ���Ʈ
						{
							if(bLsd) continue;
							str.Format("$f3%.3f",pBri->GetOutValueWall(nJijumIndex, CHECK_POS_BOTTOM, COMBO_SERVICE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd));
						}
						else if(j==5 && b3D)				// ������� 3D SHELL ���Ʈ
						{
							if(bLsd) continue;
							str.Format("$f3%.3f",pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_BOTTOM, COMBO_SERVICE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd));
							if(!b3DLoad) str = _T("-");
						}
						else if(j==6)				// ������� 2D FRAME ���ܷ�
						{
							dValue = pBri->GetOutValueWall(nJijumIndex, CHECK_POS_BOTTOM, COMBO_ULTIMATE, ELE_SHEAR_2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
							str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);
							dApplyS = dValue;
						}
						else if(j==7 && b3D)				// ������� 3D SHELL ���ܷ�
						{
							if(b3DLoad)
							{
								dValue = pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_BOTTOM, COMBO_ULTIMATE, ELE_SHEAR_2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
								str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);

								if(fabs(dApplyS) < fabs(dValue))
									dApplyS = dValue;
							}
							else
								str = _T("-");
						}
						else if(j==8)
						{
							str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dApplyM);
						}
						else if(j==9)
						{
							str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dApplyS);
						}
						else
							str = "";
					}

					if(bColumn && j==1)
					{
						XLText->AddMergeFormatText(nMerge, nWidthJoint[j]/2-1, "$c%s%s", str, strOpt);
						XLText->AddFormatText("$m+0%d", nWidthJoint[j]/2);

						XLText->AddMergeFormatText(nMerge, nWidthJoint[j]/2-1, "$c%s%s", strsub, "");
						XLText->AddFormatText("$m+0%d", nWidthJoint[j]/2);
					}
					else if((j==2 && !b3D) || (j==4 && !b3D) || (j==6 && !b3D))
					{
						XLText->AddMergeFormatText(nMerge, nWidthJoint[j]*2-1, "$c%s%s", str, strOpt);
						XLText->AddFormatText("$m+0%d", nWidthJoint[j]*2);
					}
					else if((j==3 && !b3D) || (j==5 && !b3D) || (j==7 && !b3D))
					{
					}
					else
					{
						XLText->AddMergeFormatText(nMerge, nWidthJoint[j]-1, "$c%s%s", str, strOpt);
						XLText->AddFormatText("$m+0%d", nWidthJoint[j]);
					}
				}
				XLText->AddText("$n$h");
			}
		}
	}

	//�λ��������
	if(pBri->m_bExistProtectionRiseFooting)
	{
		for(long iSE = iSTT; iSE <= iEND; ++iSE)
		{		
			BOOL bStt = iSE == iSTT;

			long nSpan = bStt ? 0 : pBri->m_nQtyJigan-1;

			for(long j=0; j<nSizeCol; j++)
			{
				nMergeRow = 0;

				if(j==0 && bStt)
				{
					str = _T("�λ��������");
					nMergeRow = 1;
				}
				else if(j==1)
				{
					str = bStt ? _T("����") : _T("����");
					strOpt = _T("");
				}
				else if(j==2)		// ������� 2D FRAME ���Ʈ
				{
					dValue = pBri->GetOutValuePRF(bStt, COMBO_ULTIMATE, ELE_MOMENT2, eEnvTypeLsd);
					str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);
					dApplyM = dValue;

					strOpt = _T("");
				}
				else if(j==3 && b3D)		// ������� 3D SHELL
				{
					if(b3DLoad)
					{
						dValue = pBri->GetOutValuePRFShell(bStt, COMBO_ULTIMATE, ELE_MOMENT2, eEnvTypeLsd);
						str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);

						if(fabs(dApplyM) < fabs(dValue))
							dApplyM = dValue;
					}
					else
						str = _T("-");

					strOpt = _T("");
				}
				else if(j==4)		// ������� 2D FRAME ���Ʈ
				{
					if(bLsd) continue;

					str.Format("$f3%.3f",pBri->GetOutValuePRF(bStt, COMBO_SERVICE, ELE_MOMENT2, eEnvTypeLsd));
					strOpt = _T("");
				}
				else if(j==5 && b3D)		// ������� 3D SHELL
				{
					if(bLsd) continue;

					str.Format("$f3%.3f",pBri->GetOutValuePRFShell(bStt, COMBO_SERVICE, ELE_MOMENT2));					
					strOpt = _T("");
					if(!b3DLoad) str = _T("-");
				}
				else if(j==6)		// ������� 2D FRAME ���ܷ�
				{
					dValue = pBri->GetOutValuePRF(bStt, COMBO_ULTIMATE, ELE_SHEAR_2, eEnvTypeLsd);
					str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);
					dApplyS = dValue;
					strOpt = _T("");
				}
				else if(j==7 && b3D) // ������� 3D SHELL ���ܷ�
				{
					if(b3DLoad)
					{
						dValue = pBri->GetOutValuePRFShell(bStt, COMBO_ULTIMATE, ELE_SHEAR_2, eEnvTypeLsd);
						str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);

						if(fabs(dApplyS) < fabs(dValue))
							dApplyS = dValue;
					}
					else
						str = _T("-");

					strOpt = _T("");
				}
				else if(j==8)
				{
					str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dApplyM);
				}
				else if(j==9)
				{
					str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dApplyS);
				}
				else
				{
					str = "";
					strOpt = _T("");
				}

				if((j==2 && !b3D) || (j==4 && !b3D) || (j==6 && !b3D))
				{
					XLText->AddMergeFormatText(nMergeRow, nWidthJoint[j]*2-1, "$c%s%s", str, strOpt);
					XLText->AddFormatText("$m+0%d", nWidthJoint[j]*2);
				}
				else if((j==3 && !b3D) || (j==5 && !b3D) || (j==7 && !b3D))
				{
				}
				else
				{
					XLText->AddMergeFormatText(nMergeRow, nWidthJoint[j]-1, "$c%s%s", str, strOpt);
					XLText->AddFormatText("$m+0%d", nWidthJoint[j]);
				}

				XLText->OutXL();
			}
			XLText->AddText("$n$h");
		}
	}

	if(b3D) XLText->AddFormatText("$h$e* 2D FRAME �ؼ������ 3D SHELL �ؼ������ ���Ͽ� �������� �ܸ�� ����$n");
	if(bColumnExist)
		XLText->AddFormatText("$h$e* ��պ������ 2D FRAME�� ����M�� ����̸�, 3D PLATE�� ��� 1��� �����.$n");
}

void COutDesignBridge::MakeTableSectionForceEQ(CXLFormatText *XLText, CRcBridgeRebar *pBri, ETypeEnvComboLsd eEnvTypeLsd)
{
	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();
	long nCntRow;
	BOOL bGirderCalc = FALSE;
	BOOL bOutSideWall= TRUE; // TRUE-���,FALSE-����
	
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	BOOL bLsd = pData->IsLsdDesign();
	CString szUnit_CGS = _T("");
//	CString szUnit_Moment = _T("");
//	CString szUnit_Force = _T("");
	CString str1, str2;
	
	CString szUnit_Moment = pData->GetStringUnitType(UNIT_CGS_TONFM);
	CString szUnit_Force  = pData->GetStringUnitType(UNIT_CGS_TONF);

	// table�� ��ü row��
	BOOL bBoxType = pBri->IsBoxType();
	long nRowUpperSlab = pBri->m_nQtyJigan*2 + 1;
	long nRowLowerSlab = bBoxType ? nRowUpperSlab : 0;
	long nCntInWall = (nJijumConnect>-1)? pBri->GetCountInWallReal() -1 : pBri->GetCountInWallReal();
	long nCntRowArchRib = pBri->m_pArrConcArchRib.GetSize();
	long nCntRowPRF = pBri->m_bExistProtectionRiseFooting ? 2 : 0;
	if(pBri->m_nSelectInWallSectionDesign == 1)
		nCntInWall = 0;
	else
		nCntInWall *= 5;

	nCntRow = 1 + nRowUpperSlab + nRowLowerSlab + nCntInWall + nCntRowArchRib + nCntRowPRF;
	if(pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign == 0)
	{
		nCntRow += 5*2;
	}
	XLText->AddTable(nCntRow-1, 25-1, 1, 9);
	
	//////////////////////////////////////////////////////////////////////////
	// title
	long nRowTitle = 1;
	int nWidthJoin1[] = {9, 8, 8};
	
	str1 = _T("�ڸ��Ʈ(") + szUnit_Moment + (bLsd? _T(") / mm") : _T(") / m"));
	str2 = _T("���ܷ�(") + szUnit_Force + (bLsd? _T(") / mm") : _T(") / m"));
	
	char *Title1[] = {"�� ��", str1.GetBuffer(str1.GetLength()-1), str2.GetBuffer(str2.GetLength()-1)};
	long j=0; for(j=0; j<3; j++)
	{
		XLText->AddMergeFormatText(0,nWidthJoin1[j]-1,"$c%s",Title1[j]);
		XLText->AddFormatText("$m+0%d",nWidthJoin1[j]);
	}
	XLText->AddText("$n$h");
	//////////////////////////////////////////////////////////////////////////
	
	long nSizeCol = 4;
	int nWidthJoint1[] = { 3, 6, 8, 8 };
	
	CString str = _T("");
	CString strOpt = _T("");
	long nMergeRow = 0;
	for(long i=nRowTitle; i<nCntRow-nCntRowPRF; i++)
	{
		long nIdxCrown = (nJijumConnect * 2) + 1;
		long nIndexArchRib = i - (nRowUpperSlab+nRowLowerSlab+nRowTitle+10);
		long nSpan = 0;
		long nPosCheck = 0;
		long nJijumIndex = 0;
		BOOL bCrown = FALSE;
		BOOL bCheckCompare = FALSE;
		
		CString szTitle = _T("");
		if(i >= nRowUpperSlab+nRowLowerSlab+nRowTitle+10 && i < (nRowUpperSlab+nRowLowerSlab+nRowTitle+10+nCntRowArchRib) && pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			if(nIndexArchRib==0 || nIndexArchRib==nCntRowArchRib-1)							// ������ SPRINGING
			{
				nSpan = (nIndexArchRib==0)? 0 : (nIndexArchRib-1)/2 - 1;
				szTitle = (nIndexArchRib == 0)? _T("���� SPRINGING") : _T("���� SPRINGING");
				nPosCheck = (nIndexArchRib==0)? CHECK_POS_LEFT : CHECK_POS_RIGHT;
			}
			else if(nIndexArchRib==nIdxCrown-1 || nIndexArchRib==nIdxCrown+1)		// ����,������ ���պ�
			{
				nSpan = (nIndexArchRib==nIdxCrown-1)? nJijumConnect-1 : nJijumConnect;
				szTitle.Format("���������պ�%s",(nIndexArchRib==nIdxCrown-1)? "����":"����");
				nPosCheck = CHECK_POS_RIGHT;
				bCheckCompare = TRUE;
			}
			else if(nIndexArchRib == nIdxCrown)			// CROWN
			{
				bCrown = TRUE;
				nSpan = nJijumConnect-1;
				szTitle = _T("CROWN��");
				nPosCheck = CHECK_POS_RIGHT;
			}
			else if(nIndexArchRib%2 == 1)					// �߰���
			{
				nSpan = (nIndexArchRib < nIdxCrown)? (nIndexArchRib-1)/2 : (nIndexArchRib-1)/2-1;
				szTitle.Format("�߰���%d", nIndexArchRib/2+1);
				nPosCheck = CHECK_POS_CENTER;
			}
			else if(nIndexArchRib%2 == 0)					// ������ ������
			{
				nSpan = (nIndexArchRib < nIdxCrown)? (nIndexArchRib-1)/2 : (nIndexArchRib-1)/2-1;
				szTitle.Format("������ ������%d", nIndexArchRib/2);
				nPosCheck = CHECK_POS_RIGHT;
				bCheckCompare = TRUE;
			}
		}
		else
		{
			nSpan = MAX(0, (i - nRowTitle - 1)/2);
			if(pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign == 0 && i < nRowUpperSlab+nRowLowerSlab+nRowTitle+10) 
				nJijumIndex = (i < nRowUpperSlab+nRowLowerSlab+nRowTitle+5) ? 0 : pBri->GetCountJijum()-1;
			else 
			{
				long nIndexByRow = (pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign == 0) ? i - nRowUpperSlab - nRowLowerSlab - 10 : i - nRowUpperSlab - nRowLowerSlab;
				if(pBri->m_nSelectInWallSectionDesign == 0)
					nJijumIndex = (nIndexByRow-1) / 5 + 1;
				else
					nJijumIndex = nIndexByRow;
			}

			if(nJijumIndex > pBri->m_nQtyJigan) 
				nJijumIndex -= nCntRowArchRib;
		}
		
		long j=0; for(j=0; j<nSizeCol; j++)
		{
			nMergeRow = 0;
			if(i < nRowUpperSlab+nRowLowerSlab+nRowTitle)			// ������
			{
				BOOL bUpperSlab = (i < nRowUpperSlab + nRowTitle) ? TRUE : FALSE;
				long nIndexSlab = bUpperSlab ? i - nRowTitle : i - nRowTitle - nRowUpperSlab;
				nSpan = (nIndexSlab - 1) / 2;

				if(j==0)
				{
					str = bBoxType ? (bUpperSlab ? _T("��� ������") : _T("�Ϻ� ������")) : _T("������");
					if(i==nRowTitle) 
						nMergeRow = nRowUpperSlab - 1;
					else if(bBoxType && (i == nRowTitle + nRowUpperSlab))
						nMergeRow = nRowLowerSlab - 1;
					else 
						nMergeRow = 0;
				}
				else if(j==1)
				{
					if(nIndexSlab == 0) 
						str = "�����ܺ�";
					else if(nIndexSlab == nRowUpperSlab - 1) 
						str = "�����ܺ�";
					else if(nIndexSlab%2 == 0) 
						str.Format("�߰�����%d", nIndexSlab/2);
					else if(nIndexSlab%2 == 1) 
						str.Format("�߾Ӻ�(����%d)", nIndexSlab/2 + 1);
					strOpt = _T("");
				}
				else if(j==2)		// ������ ���Ʈ
				{
					if(nIndexSlab == 0) 
						str.Format("$f3%.3f",pBri->GetOutValueSlabEQ(nSpan, CHECK_POS_LEFT, ELE_MOMENT2, bUpperSlab, eEnvTypeLsd));
					else if(nIndexSlab == nRowUpperSlab - 1) 
						str.Format("$f3%.3f",pBri->GetOutValueSlabEQ(nSpan, CHECK_POS_RIGHT, ELE_MOMENT2, bUpperSlab, eEnvTypeLsd));
					else if(nIndexSlab%2 == 0) 
					{
						double dData = min(pBri->GetOutValueSlabEQ(nSpan, CHECK_POS_RIGHT, ELE_MOMENT2, bUpperSlab, eEnvTypeLsd) , 
							             pBri->GetOutValueSlabEQ(nSpan+1, CHECK_POS_LEFT, ELE_MOMENT2, bUpperSlab, eEnvTypeLsd) );
						str.Format("$f3%.3f", dData);
					}
					else if(nIndexSlab%2 == 1) 
						str.Format("$f3%.3f",pBri->GetOutValueSlabEQ(nSpan, CHECK_POS_CENTER, ELE_MOMENT2, bUpperSlab, eEnvTypeLsd));
					strOpt = _T("");
				}
				else if(j==3)		// ������ ���ܷ�
				{
					double dDistShearLeft = pBri->GetDistCheckShearSlab(nSpan, CHECK_POS_LEFT, bUpperSlab, NULL);
					double dDistShearRight = pBri->GetDistCheckShearSlab(nSpan, CHECK_POS_RIGHT, bUpperSlab, NULL);

					if(nIndexSlab == 0) 
						str.Format("$f3%.3f",pBri->GetOutValueSlabEQ(nSpan, CHECK_POS_LEFT, ELE_SHEAR_2, bUpperSlab, eEnvTypeLsd, dDistShearLeft));
					else if(nIndexSlab == nRowUpperSlab - 1) 
						str.Format("$f3%.3f",pBri->GetOutValueSlabEQ(nSpan, CHECK_POS_RIGHT, ELE_SHEAR_2, bUpperSlab, eEnvTypeLsd, dDistShearRight));
					else if(nIndexSlab%2 == 0) 
					{
						double dData = max(pBri->GetOutValueSlabEQ(nSpan, CHECK_POS_RIGHT, ELE_SHEAR_2, bUpperSlab, eEnvTypeLsd, dDistShearRight) ,
							             pBri->GetOutValueSlabEQ(nSpan+1, CHECK_POS_LEFT, ELE_SHEAR_2, bUpperSlab, eEnvTypeLsd, dDistShearLeft) );
						str.Format("$f3%.3f", dData);
					}
					else if(nIndexSlab%2 == 1) 
						str.Format("$f3%.3f",pBri->GetOutValueSlabEQ(nSpan, CHECK_POS_CENTER, ELE_SHEAR_2, bUpperSlab, eEnvTypeLsd));
					strOpt = _T("");
				}
				else
				{
					str = "";
					strOpt = _T("");
				}
			}
			else if(i<nRowUpperSlab+nRowLowerSlab+nRowTitle+10 && (pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign == 0))		// ������ü ������ü
			{
				long nRowIndex = (i-(nRowUpperSlab+nRowLowerSlab+nRowTitle))%5;
				str = GetStringSectionForceTableAtWallEQ(pBri, eEnvTypeLsd, nJijumIndex, nRowIndex, j, nMergeRow, strOpt, bLsd);
			}
			else if((i<nCntRow - nCntInWall) && pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)		// ��ġ����
			{
				if(j==0)
				{
					str = "��ġ����";
//					if(i==nRowUpperSlab+nRowLowerSlab+nRowTitle+10) strOpt.Format(" $md%d", nCntRowArchRib-1);
					if(i==nRowUpperSlab+nRowLowerSlab+nRowTitle+10) nMergeRow = nCntRowArchRib-1;
					else nMergeRow = 0;
				}
				else if(j==1)
				{
					str = szTitle;
					strOpt = _T("");
				}
				else if(j==2)		// ������ ���Ʈ
				{
					if(bCheckCompare)
					{
						double dData = min(m_pCalc->m_pArchRib->GetOutValueArchRibBySlabEQ(nSpan, CHECK_POS_RIGHT, ELE_MOMENT2, bCrown),
							m_pCalc->m_pArchRib->GetOutValueArchRibBySlabEQ(nSpan+1, CHECK_POS_LEFT, ELE_MOMENT2, bCrown));
						str.Format("$f3%.3f", dData);
					}
					else str.Format("$f3%.3f",m_pCalc->m_pArchRib->GetOutValueArchRibBySlabEQ(nSpan, nPosCheck, ELE_MOMENT2, bCrown));
					strOpt = _T("");
				}
				else if(j==3)		// ������ ���ܷ�
				{
					if(bCheckCompare)
					{
						double dData = max(m_pCalc->m_pArchRib->GetOutValueArchRibBySlabEQ(nSpan, CHECK_POS_RIGHT, ELE_SHEAR_2, bCrown),
							m_pCalc->m_pArchRib->GetOutValueArchRibBySlabEQ(nSpan+1, CHECK_POS_LEFT, ELE_SHEAR_2, bCrown));
						str.Format("$f3%.3f", dData);
					}
					else str.Format("$f3%.3f",m_pCalc->m_pArchRib->GetOutValueArchRibBySlabEQ(nSpan, nPosCheck, ELE_SHEAR_2, bCrown));
					strOpt = _T("");
				}
				else
				{
					str = "";
					strOpt = _T("");
				}
			}
			else								// �߰���ü
			{
				strOpt = _T("");

				if(pBri->m_nSelectInWallSectionDesign == 0)
				{
					long nOffset = (pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign == 0)? 10 : 0;
					long nRowIndex = (i-(nRowUpperSlab+nRowLowerSlab+nRowTitle+nOffset+nCntRowArchRib))%5;

					if(j==0 && nRowIndex == 0) 
					{
						if(nJijumIndex >= nJijumConnect && nJijumConnect > 0) nJijumIndex++;
					}

					str = GetStringSectionForceTableAtWallEQ(pBri, eEnvTypeLsd, nJijumIndex, nRowIndex, j, nMergeRow, strOpt, bLsd);
				}
				else
				{
					if(j==0) 
					{
						if(nJijumIndex >= nJijumConnect && nJijumConnect > 0) nJijumIndex++;

						str = "�߰���ü";
						long nOffset = (pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign == 0)? 10 : 0;
						if(i==nRowUpperSlab+nRowLowerSlab+nRowTitle+nOffset+nCntRowArchRib) nMergeRow = nCntInWall-1;
						else nMergeRow = 0;
					}
					else if(j==1)
						str.Format("�߰�����%d", nJijumIndex);
					else if(j==2)				// ������ ���Ʈ
						str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_BOTTOM, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd));
					else if(j==3)				// ������ ���ܷ�
						str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_BOTTOM, ELE_SHEAR_2, bGirderCalc,bOutSideWall, eEnvTypeLsd));
					else
						str = "";
				}
			}
			XLText->AddMergeFormatText(nMergeRow, nWidthJoint1[j]-1, "$c%s%s", str, strOpt);
			XLText->AddFormatText("$m+0%d", nWidthJoint1[j]);
		}
		XLText->AddText("$n$h");
	}
	//�λ��������
	if(pBri->m_bExistProtectionRiseFooting)
	{
		for(long iSE = iSTT; iSE <= iEND; ++iSE)
		{		
			BOOL bStt = iSE == iSTT;

			for(long j=0; j<nSizeCol; j++)
			{
				nMergeRow = 0;

				if(j==0 && bStt)
				{
					str = _T("�λ��������");
					nMergeRow = 1;
				}
				else if(j==1)
				{
					str = bStt ? _T("����") : _T("����");
					strOpt = _T("");
				}
				else if(j==2)		// ������ ���Ʈ
				{
					str.Format("$f3%.3f",pBri->GetOutValuePRFEQ(bStt, ELE_MOMENT2, eEnvTypeLsd));
					strOpt = _T("");
				}
				else if(j==3)		// ������ ���ܷ�
				{
					str.Format("$f3%.3f",pBri->GetOutValuePRFEQ(bStt, ELE_SHEAR_2, eEnvTypeLsd));
					strOpt = _T("");
				}			

				XLText->AddMergeFormatText(nMergeRow, nWidthJoint1[j]-1, "$c%s%s", str, strOpt);
				XLText->AddFormatText("$m+0%d", nWidthJoint1[j]);

				XLText->OutXL();
			}
			XLText->AddText("$n$h");
		}
	}	

	double dApplyWidth = bLsd ? frM(m_pCalc->GetSlabWidth(-1)) : m_pCalc->GetSlabWidth(-1);
	XLText->AddFormatText("$h$e * ������ ���� ���� �������� ����Ǹ� ����ܸ���� �ؼ����/����(%s%s)���� �����.$n$h", GetStrDouble(dApplyWidth, bLsd ? 0 : 3, bLsd), bLsd ? _T("mm") : _T("m"));
}

void COutDesignBridge::MakeTableLoadForFooting(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

//	CString szUnit_Moment = _T("");
//	CString szUnit_Force = _T("");
	CString str1, str2;
	
	BOOL bDividUseCase = (pData->IsRailDesign2017() && pData->m_nTypeFootingSafetyApplyCoefficientImpact == 1) ? TRUE : FALSE;
	CString szUnit_Moment = pData->GetStringUnitType(UNIT_CGS_TONFM);
	CString szUnit_Force  = pData->GetStringUnitType(UNIT_CGS_TONF);

	int nWidthJoint2[] = { 4, 5, 5, 5, 6 };
	CString strTemp2 = "�����("+szUnit_Force+")";
	CString strTemp  = "������("+szUnit_Force+")";
	CString strTemp1 = "���Ʈ("+szUnit_Moment+")";
	char *Title2[] = { "�� ��", strTemp.GetBuffer(strTemp.GetLength()), strTemp1.GetBuffer(strTemp1.GetLength()), strTemp2.GetBuffer(strTemp2.GetLength()), "�� ��" };
	CStringArray szArrTitleSub2;
	if(bDividUseCase)
	{
		szArrTitleSub2.Add(_T("$j�������\n(�������)"));
		szArrTitleSub2.Add(_T("$j�������\n(�������)"));
		szArrTitleSub2.Add(_T("$j�������\n(�������)"));
		szArrTitleSub2.Add(_T("$j�������\n(�������)"));
		szArrTitleSub2.Add(_T("$j�������\n(�������)"));
		szArrTitleSub2.Add(_T("$j�������\n(�������)"));
	}
	else
	{
		szArrTitleSub2.Add(_T("�������"));
		szArrTitleSub2.Add(_T("�������"));
		szArrTitleSub2.Add(_T("�������"));
	}

	szArrTitleSub2.Add(_T("�������"));
	szArrTitleSub2.Add(_T("�������"));
	szArrTitleSub2.Add(_T("�������"));
	szArrTitleSub2.Add(_T("������"));

	// table�� ��ü row��
	long nCntRow = pBri->IsApplyEarthQuakeDesign() ? 8 : 7;
	if(bDividUseCase)
		nCntRow += 3;

	XLText->AddTable(nCntRow-1, 25-1, 1, 4);
	
	// Title
	long j=0; for(j=0; j<5; j++)
	{
		XLText->AddMergeFormatText(0,nWidthJoint2[j]-1,"$c%s",Title2[j]);
		XLText->AddFormatText("$m+0%d",nWidthJoint2[j]);
	}
	XLText->AddText("$n$h");

	long nIndexAdd = -1;

	BOOL bTrap2 = (pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2);
	BOOL bArch  = (pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH);
	if(bTrap2) 
	{
		if(nJijum==0) nIndexAdd = 1;
		else if(nJijum==1) nIndexAdd = 0;
		else if(nJijum==pBri->m_nQtyJigan) nIndexAdd = pBri->m_nQtyJigan-1;
		else if(nJijum==pBri->m_nQtyJigan-1) nIndexAdd = pBri->m_nQtyJigan;
	}
	else if(bArch) 
	{
		if(nJijum==0) nIndexAdd = pBri->m_nQtyJigan + 1;
		else if(nJijum==pBri->m_nQtyJigan+1) nIndexAdd = 0;
		else if(nJijum==pBri->m_nQtyJigan) nIndexAdd = pBri->m_nQtyJigan+2;
		else if(nJijum==pBri->m_nQtyJigan+2) nIndexAdd = pBri->m_nQtyJigan;	
	}
	
	CString str = _T("");
	CString strOpt = _T("");
	double Moment = 0;
	double Shear = 0;
	double Axial = 0;
	// data
	double nTypeJongOld = pBri->m_nTypeJongdanForRebar;
	pBri->GetTvVertSection_Std(0, FALSE, FALSE, FALSE, bArch? 1 : 0);

	for(int k=1; k<nCntRow; k ++)
	{
		long nTypeCheck = FOOTING_CHECK_EQ;
		if(k==1 || k==4 || k==7) nTypeCheck = FOOTING_CHECK_AXIAL;
		else if(k==2 || k==5 || k==8) nTypeCheck = FOOTING_CHECK_MOMENT_MINUS;
		else if(k==3 || k==6 || k==9) nTypeCheck = FOOTING_CHECK_MOMENT_PLUS;
		if(k == nCntRow-1) nTypeCheck = FOOTING_CHECK_EQ;

		BOOL bUseForce = (k < (bDividUseCase? 7 : 4)) ? TRUE : FALSE;
		BOOL bNoneImpect = (bDividUseCase && bUseForce && k < 4) ? TRUE : FALSE;
		long nCombo = pBri->GetOutValueForFootingDesign(nJijum,nTypeCheck,Moment,Shear,Axial,bUseForce,nIndexAdd, -1, bNoneImpect);
		for(j=0; j<5; j++)
		{
			if(j == 0) str = szArrTitleSub2.GetAt(k-1);
			else if(j == 1) str.Format("$f3%.3f",Axial);
			else if(j == 2) str.Format("$f3%.3f",Moment);
			else if(j == 3) str.Format("$f3%.3f",Shear);
			else if(j == 4) 
			{
				str.Format("������ (EQ%02d)", nCombo+1);
				if(k==1 || k==4) str.Format("��� �ִ� (%s%02d%s)",bUseForce ? "SV" : "UL",nCombo+1, bNoneImpect ? _T("-S") : _T(""));
				else if(k==2 || k==5) str.Format("�θ��Ʈ �ִ� (%s%02d%s)",bUseForce ? "SV" : "UL",nCombo+1, bNoneImpect ? _T("-S") : _T(""));
				else if(k==3 || k==6) str.Format("�����Ʈ �ִ� (%s%02d%s)",bUseForce ? "SV" : "UL",nCombo+1, bNoneImpect ? _T("-S") : _T(""));
			}
			if(j==0 && (k==1 || k==4 || (bDividUseCase && k==7))) 
				strOpt = " $md2";
			else 
				strOpt = "";
			XLText->AddMergeFormatText(0, nWidthJoint2[j]-1, "$c%s%s", str, strOpt);
			XLText->AddFormatText("$m+0%d",nWidthJoint2[j]);
		}
		XLText->AddText("$n$h");
	}
	*XLText += 2;

	pBri->m_nTypeJongdanForRebar = (long)nTypeJongOld;
	pBri->m_nTypeJongdanForRebarOld = (long)nTypeJongOld;
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, (long)nTypeJongOld);
//	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE);
}

void COutDesignBridge::MakeExcelSTM_Condition(CXLFormatText *pXLText, CRcBridgeRebar *pBri, CStmModel*pModel, long nColPos, long nIndexTitle, HGINT32 nItem, HGINT32 iKey)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	pXLText->GoPosition(NO_MOVE, nColPos);

	double dFck = pModel->GetStmDataConfig()->m_dFck;
	double dFy = pModel->GetStmDataConfig()->m_dFy;

	long nIndexSmall = 1;
	pXLText->AddFormatText("$e$h$a%d. ��������$n",nIndexTitle);
	*pXLText += 1;

	if(pData->IsLsdDesign())
	{
		CString szCase = pBri->GetStringStmModelNameByKey(iKey, 4);
		pXLText->AddFormatText("$h(%d) �Ѱ����$m+03$c:$r%s$n",nIndexSmall++, szCase);
	}
	pXLText->AddFormatText("$h(%d) ��ü����$m+03$c:$rfck = %3.0f %s$n",nIndexSmall++, dFck, pData->m_szKgf_CM2);
	pXLText->AddFormatText("$h(%d) ö�ٰ���$m+03$c:$rfy  = %3.0f %s$n",nIndexSmall++, dFy, pData->m_szKgf_CM2);
	if (nItem == XL_DESIGNOUT_STMFOOT || nItem == XL_DESIGNOUT_STMFOOTMID)
	{
		CFootingApp *pFoot = &pBri->m_footingStt;
		HGDOUBLE dWidth = pFoot->GetSideWidth(TRUE);
		pXLText->AddFormatText("$h(%d) ������$m+03$c:$r%.0f mm$n",nIndexSmall++, dWidth);
	}

	*pXLText += 1;
}


void COutDesignBridge::MakeExcelSTM_ArcCorner_Section( CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey)
{
	pXLText->GoPosition(NO_MOVE, nColPos);
	pXLText->AddFormatText("$e$h$a%d. �ܸ�����$n", nIndexSmall);
	*pXLText += 1;

	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	CHgBaseDrawStd baseDraw;

	// ���鵵
	CDomyun *pDomSide = new CDomyun;
	pDomSide->SetScaleDim(50);
	pDomSide->SetLTScale(50*5);

	CDomyun DomDimLR(pDomSide), DomDimTB(pDomSide);
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomDimLR,HCAD_DIML);
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomDimTB,HCAD_DIML);	


	CTwinVectorArray tvArrCornerCut, tvArrDimTop, tvArrDimTopBot, tvArrDimLeftRight;

	BOOL bStt	= (iKey == STM_MODEL_CORNER_STT_UPPER_PLUS || iKey == STM_MODEL_CORNER_STT_LOWER_PLUS || iKey == STM_MODEL_CORNER_STT_UPPER_MINUS|| iKey == STM_MODEL_CORNER_STT_LOWER_MINUS
		|| iKey == STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_1 || iKey == STM_MODEL_CORNER_STT_LOWER_PLUS_EXT_1 || iKey == STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1|| iKey == STM_MODEL_CORNER_STT_LOWER_MINUS_EXT_1
		|| iKey == STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_2 || iKey == STM_MODEL_CORNER_STT_LOWER_PLUS_EXT_2 || iKey == STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2|| iKey == STM_MODEL_CORNER_STT_LOWER_MINUS_EXT_2) ? TRUE : FALSE;
	BOOL bUpper	= (iKey == STM_MODEL_CORNER_STT_UPPER_PLUS || iKey == STM_MODEL_CORNER_END_UPPER_PLUS|| iKey == STM_MODEL_CORNER_STT_UPPER_MINUS|| iKey == STM_MODEL_CORNER_END_UPPER_MINUS || 
		iKey == STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_1 || iKey == STM_MODEL_CORNER_END_UPPER_PLUS_EXT_1 || iKey == STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1 || iKey == STM_MODEL_CORNER_END_UPPER_MINUS_EXT_1 ||
		iKey == STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_2 || iKey == STM_MODEL_CORNER_END_UPPER_PLUS_EXT_2 || iKey == STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2 || iKey == STM_MODEL_CORNER_END_UPPER_MINUS_EXT_2) ? TRUE : FALSE;

	HGINT32 nPos = 0;
	if (bStt && bUpper)
		nPos = POS_L_UP;
	else if (!bStt && bUpper)
		nPos = POS_R_UP;
	else if (bStt && !bUpper)
		nPos = POS_L_LO;
	else if (!bStt && !bUpper)
		nPos = POS_R_LO;

	GetTvArrSTM_Corner_Section(tvArrCornerCut, tvArrDimTopBot, tvArrDimLeftRight, pBri, nPos);

	///���鵵 ���� �׸���
	m_pStd->m_pARoadOptionStd->SetEnvType(pDomSide, HCAD_CONC);
	baseDraw.DrawTvArray(pDomSide, tvArrCornerCut );

	///ġ����
	CString sDirTopBot = nPos == POS_L_UP || nPos == POS_R_UP? _T("TOP") : _T("BOTTOM");
	CString sDirLeftRight = nPos == POS_L_UP || nPos == POS_L_LO? _T("LEFT") : _T("RIGHT");

	//�� �Ϻ� ġ����
	DomDimTB.SetDirection(sDirTopBot);
	long nDirTB	=0;
	for (long nx=0; nx < tvArrDimTopBot.GetSize(); nx++)
	{
		CTwinVector tv		= tvArrDimTopBot.GetAt(nx);
		CDPoint xyBaseTB	= tv.m_v1;
		double dLenDim		= tv.GetLength();

		if(nDirTB == 0)
			nDirTB = tv.GetXyDir().x <0? (long) -1 : (long)+1;

		DomDimTB.DimMoveTo(xyBaseTB.x, xyBaseTB.y, 0);
		DomDimTB.DimLineTo(dLenDim * nDirTB, 0, COMMA(dLenDim));
	}
	*pDomSide << DomDimTB;	

	//�� �� ġ����
	DomDimLR.SetDirection(sDirLeftRight);
	long nDirLR = 0;	
	for (long nx=0; nx < tvArrDimLeftRight.GetSize(); nx++)
	{
		CTwinVector tv		= tvArrDimLeftRight.GetAt(nx);
		CDPoint xyBaseLR	= tv.m_v1;
		double dLenDim		= tv.GetLength();

		if(nDirLR == 0)
			nDirLR = tv.GetXyDir().y <0? (long) -1 : (long)+1;

		DomDimLR.DimMoveTo(xyBaseLR.x, xyBaseLR.y, 0);
		DomDimLR.DimLineTo(dLenDim * nDirLR, 0, COMMA(dLenDim));
	}
	*pDomSide << DomDimLR;	


	m_pStd->m_pARoadOptionStd->SetEnvType(pDomSide, HCAD_CONC);

	pXLText->InsertDomyun(pDomSide, m_pStd->m_pView, 20, CXLFormatText::INSERT_DOM_CENTER | CXLFormatText::INSERT_DOM_MOVEROW);
	*pXLText += 1;
}


void COutDesignBridge::MakeExcelSTM_ArcCorner_CalcLoad( CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey )
{
	CARcBridgeCalcStm *pCalcStm = m_pCalc->m_pStm;

	CStmModel*pModel = pBri->GetStmModelByKey(iKey);
	if (!pModel) return;

	CARcBridgeDataStd *pData = m_pStd->m_pARcBridgeDataStd;

	pXLText->GoPosition(NO_MOVE, nColPos);
	pXLText->AddFormatText("$s+45$e$h$a%d. ���߻���$n", nIndexSmall);
	*pXLText += 1;
	pCalcStm->WriteExcelSTM_ArcCorner_CalcLoad(pModel, pData, pBri, pXLText, nColPos, iKey);
	*pXLText += 1;
}

void COutDesignBridge::MakeExcelSTM_Bracket_Section(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey)
{
	pXLText->GoPosition(NO_MOVE, nColPos);
	pXLText->AddFormatText("$e$h$a%d. �ܸ�����$n", nIndexSmall);
	*pXLText += 1;

	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	CHgBaseDrawStd baseDraw;

	// ���鵵
	CDomyun *pDomSide = new CDomyun;
	pDomSide->SetScaleDim(50);
	pDomSide->SetLTScale(50*5);

	CTwinVector tvUpper, tvUpperSide, tvSide, tvHunch;
	CTwinVectorArray tvArr; 
	CDPoint xyCen(0, 0);
	HGDOUBLE dRadius = 0;
	BOOL bStt	= iKey == STM_MODEL_BRACKET_STT ? TRUE : FALSE;

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2);
	tvArr = pBri->m_tvArrVertSection;

	CString sStt	= bStt ? "����" : "����";
	tvArr.GetTvByInfo(sStt+_T("����ϻ��"), tvUpper);
	tvArr.GetTvByInfo(sStt+_T("����ϻ������"), tvUpperSide);
	tvArr.GetTvByInfo(sStt+_T("���������"), tvSide);
	tvArr.GetTvByInfo(sStt+_T("�������ġ"), tvHunch);

	CDRect rect(0, 0, 0, 0);

	if(bStt)
	{
		rect.left = tvSide.m_v2.x - 100.0;
		rect.top = tvUpperSide.m_v2.y + 1000.0;
		rect.right = tvUpper.m_v2.x + pBri->m_dWS + pBri->m_dHHS+100;
		rect.bottom = tvHunch.m_v1.y -1000.0;
	}
	else
	{
		rect.right = tvUpper.m_v2.x + 100.0;
		rect.top = tvUpperSide.m_v2.y + 1000.0;
		rect.left = tvUpper.m_v1.x - pBri->m_dWE - pBri->m_dHHE-100;
		rect.bottom = tvHunch.m_v1.y -1000.0;
	}

	xyCen = rect.CenterPoint() + CDPoint(245.0, -100.0);
	dRadius = (rect.Width()-490.0)/2.0 + 750.0;

	m_pStd->m_pARoadOptionStd->SetEnvType(pDomSide, HCAD_CONC);
	DBStd.DrawCuttingTvArray(pDomSide, tvArr, rect);

	m_pStd->m_pARoadOptionStd->SetEnvType(pDomSide, HCAD_DIML);
	DBStd.m_pDrawVertSection->DimVerSectionBracketHunch(pDomSide, 0, bStt, FALSE);

	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	long nDimDan	= 0;
	CString sDir	= bStt ? "LEFT" : "RIGHT";

	xyArrDim.Add(tvHunch.GetXyBottom());
	xyArrDim.Add(tvSide.GetXyBottom());
	xyArrDim.Add(tvSide.GetXyTop());
	xyArrDim.Add(tvUpperSide.GetXyTop());

	baseDraw.AutoDimCont(pDomSide, CDPoint(tvSide.m_v1.x, xyArrDim.GetAt(0).y), xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, TRUE);

	pXLText->InsertDomyun(pDomSide, m_pStd->m_pView, 20, CXLFormatText::INSERT_DOM_CENTER | CXLFormatText::INSERT_DOM_MOVEROW);
	*pXLText += 1;
}

void COutDesignBridge::MakeExcelSTM_Bracket_CalcLoad(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey)
{
	CARcBridgeCalcStm *pCalcStm = m_pCalc->m_pStm;

	CStmModel*pModel = pBri->GetStmModelByKey(iKey);
	if (!pModel) return;

	CARcBridgeDataStd *pData = m_pStd->m_pARcBridgeDataStd;

	pXLText->GoPosition(NO_MOVE, nColPos);
	pXLText->AddFormatText("$s+45$e$h$a%d. ���߻���$n", nIndexSmall);
	*pXLText += 1;
 	pCalcStm->WriteExcelSTM_Bracket_CalcLoad(pModel, pData, pBri, pXLText, nColPos, iKey);
 	*pXLText += 1;
}


void COutDesignBridge::MakeExcelSTM_Footing_Section(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey)
{
	pXLText->GoPosition(NO_MOVE, nColPos);
	pXLText->AddFormatText("$e$h$a%d. �ܸ�����$n", nIndexSmall);
	*pXLText += 1;

	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	CHgBaseDrawStd baseDraw;

	// ���鵵
	CDomyun *pDom = new CDomyun;
	pDom->SetScaleDim(50);
	pDom->SetLTScale(50*5);

	CTwinVectorArray tvArr; 
	CTwinVectorArray tvArrFootingStt;
	CTwinVectorArray tvArrFootingEnd;
	CTwinVectorArray tvArrExFootingStt;
	CTwinVectorArray tvArrExFootingEnd;

	CDPoint xyCen(0, 0);
	HGDOUBLE dRadius = 0;
	BOOL bStt = TRUE;
	HGBOOL bLsd = m_pStd->m_pARcBridgeDataStd->IsLsdDesign();

	if(bLsd)
	{
		bStt = (iKey >= STM_MODEL_FOOT_STT_MAX_AXIAL &&  iKey <= STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_2) ? TRUE : FALSE;
	}
	else
	{
		bStt = (iKey == STM_MODEL_FOOT_STT_MAXP || iKey == STM_MODEL_FOOT_STT_MAXM || iKey == STM_MODEL_FOOT_STT_MINM || iKey == STM_MODEL_FOOT_STT_EARTH) ? TRUE : FALSE;
	}

	if (bStt)
	{
		DBStd.m_pDrawFooting->DrawFront(pDom, &pBri->m_footingStt, &tvArrFootingStt);
		DBStd.m_pDrawExFooting->DrawFront(pDom, &pBri->m_footingStt.m_exFooting, FALSE, FALSE, &tvArrExFootingStt);
	}
	else
	{
		DBStd.m_pDrawFooting->DrawFront(pDom, &pBri->m_footingEnd, &tvArrFootingEnd);
		DBStd.m_pDrawExFooting->DrawFront(pDom, &pBri->m_footingEnd.m_exFooting, FALSE, FALSE, &tvArrExFootingEnd);
	}

	CDRect rect = pDom->GetExtRect();
	rect.bottom += 2000;

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2);
	tvArr = pBri->m_tvArrVertSection;

	if (bStt)
	{
 		DBStd.m_pDrawFooting->DimFrontSide(pDom, &pBri->m_footingStt, 0, TRUE, TRUE, FALSE);
 		DBStd.m_pDrawFooting->DimTextFrontEL(pDom, &pBri->m_footingStt, FALSE, TRUE);
 		DBStd.m_pDrawFooting->DimFrontUpper(pDom, &pBri->m_footingStt, 0, TRUE, FALSE);
 		// Ȯ�����
 		DBStd.m_pDrawExFooting->DimFrontLower(pDom, &pBri->m_footingStt.m_exFooting, 0, TRUE);
	}
	else
	{
		DBStd.m_pDrawFooting->DimFrontSide(pDom, &pBri->m_footingEnd, 0, TRUE, TRUE, FALSE);
		DBStd.m_pDrawFooting->DimTextFrontEL(pDom, &pBri->m_footingEnd, TRUE, TRUE);
		DBStd.m_pDrawFooting->DimFrontUpper(pDom, &pBri->m_footingEnd, 0, TRUE, TRUE);
		// Ȯ�����
		DBStd.m_pDrawExFooting->DimFrontLower(pDom, &pBri->m_footingEnd.m_exFooting, 0, TRUE);
	}

	CTwinVectorArray *pTvArr = bStt ? &pBri->m_footingStt.m_tvArrFront : &pBri->m_footingEnd.m_tvArrFront;
	CTwinVector tvLeft, tvRight;
	pTvArr->GetTvByInfo("���ʻ����������", tvLeft);
	pTvArr->GetTvByInfo("���ʻ���������", tvRight);
	tvLeft.m_v1.x -= 200;
	tvRight.m_v2.x += 200;
 	DBStd.DrawCuttingTvArray(pDom, tvArr, rect, FALSE);
 	pDom->CutLightning(tvLeft.m_v1.x, rect.BottomLeft().y, tvRight.m_v2.x, rect.BottomRight().y, FALSE, 1);

	pXLText->InsertDomyun(pDom, m_pStd->m_pView, 20, CXLFormatText::INSERT_DOM_CENTER | CXLFormatText::INSERT_DOM_MOVEROW);
	*pXLText += 1;
}

void COutDesignBridge::MakeExcelSTM_Footing_CalcLoad(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey)
{
	CARcBridgeCalcStm *pCalcStm = m_pCalc->m_pStm;

	CStmModel*pModel = pBri->GetStmModelByKey(iKey);
	if (!pModel) return;

	CARcBridgeDataStd *pData = m_pStd->m_pARcBridgeDataStd;

	pXLText->GoPosition(NO_MOVE, nColPos);
	pXLText->AddFormatText("$s+45$e$h$a%d. ���߻���$n", nIndexSmall);
	*pXLText += 1;
	pCalcStm->WriteExcelSTM_Footing_CalcLoad(pModel, pData, pBri, pXLText, nColPos, iKey);
	*pXLText += 1;
}

void COutDesignBridge::GetKeySttEnd( HGINT32 nItem, HGINT32 &iKeyStt, HGINT32 &iKeyEnd )
{
	HGBOOL bLsd = m_pStd->m_pARcBridgeDataStd->IsLsdDesign();
	switch (nItem)
	{
	case XL_DESIGNOUT_STMARC :
		{
			iKeyStt = STM_MODEL_CORNER_STT_UPPER_MINUS;
			iKeyEnd = STM_MODEL_CORNER_END_LOWER_PLUS;
			break;
		}
	case XL_DESIGNOUT_STMBRACKET :
		{
			iKeyStt = STM_MODEL_BRACKET_STT;
			iKeyEnd = STM_MODEL_BRACKET_END;
			break;
		}
	case XL_DESIGNOUT_STMFOOT :
		{
			iKeyStt = bLsd==TRUE ? STM_MODEL_FOOT_STT_MAX_AXIAL : STM_MODEL_FOOT_STT_MAXP;
			iKeyEnd = bLsd==TRUE ? STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2 : STM_MODEL_FOOT_END_EARTH;
			break;
		}
	case XL_DESIGNOUT_STMFOOTMID :
		{
			iKeyStt = bLsd==TRUE ? STM_MODEL_FOOT_MID_MAX_AXIAL : STM_MODEL_MIDFOOT_HEAD;
			iKeyEnd = bLsd==TRUE ? (STM_MODEL_FOOT_MID_MIN_HORI_FORCE_EXT_2 + MAX_JIJUM_COUNT_RC * 5 - 1) : (STM_MODEL_MIDFOOT_HEAD + MAX_JIJUM_COUNT_RC * 4 - 1);
			break;
		}
	}
}

void COutDesignBridge::MakeExcelSTM_FootingMid_Section(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexSmall, HGINT32 iKey)
{
	pXLText->GoPosition(NO_MOVE, nColPos);
	pXLText->AddFormatText("$e$h$a%d. �ܸ�����$n", nIndexSmall);
	*pXLText += 1;

	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	CHgBaseDrawStd baseDraw;

	// ���鵵
	CDomyun *pDom = new CDomyun;
	pDom->SetScaleDim(50);
	pDom->SetLTScale(50*5);

	CTwinVectorArray tvArr; 
	CTwinVectorArray tvArrInWall;
	CTwinVectorArray tvArrFooting;
	CTwinVectorArray tvArrExFooting;

	CDPoint xyCen(0, 0);
	HGDOUBLE dRadius = 0;
//	BOOL bStt = iKey == STM_MODEL_FOOT_STT_MAXP || iKey == STM_MODEL_FOOT_STT_MAXM || iKey == STM_MODEL_FOOT_STT_MINM || iKey == STM_MODEL_FOOT_STT_EARTH ? TRUE : FALSE;

	HGINT32 nKeyStt(0), nKeyEnd(0);
	GetKeySttEnd(XL_DESIGNOUT_STMFOOTMID, nKeyStt, nKeyEnd);
	HGINT32 nIndex = (iKey - nKeyStt) / (nKeyEnd - nKeyStt);
	CWallApp *pWall = pBri->GetInWall(nIndex);
	if(!pWall) return;

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2);
	tvArr = pBri->m_tvArrVertSection;

	DBStd.m_pDrawWall->DrawFront(pDom, pWall, TRUE, &tvArrInWall);
	DBStd.m_pDrawFooting->DrawFront(pDom, &pWall->m_footing, &tvArrFooting);
	DBStd.m_pDrawExFooting->DrawFront(pDom, &pWall->m_footing.m_exFooting, FALSE, TRUE, &tvArrExFooting);

	CDRect rect = pDom->GetExtRect();
	rect.bottom += 2000;

	DBStd.m_pDrawExFooting->DimFrontLower(pDom, &pWall->m_footing.m_exFooting, 0, TRUE);
	BOOL bLeft	= FALSE;
	if(pWall->m_footing.m_nType == FOOTING_TYPE_RIGHTSLOPE)
		bLeft	= TRUE;
	DBStd.m_pDrawExFooting->DimFrontSide(pDom, &pWall->m_footing.m_exFooting, bLeft, 0, TRUE);

	DBStd.m_pDrawFooting->DimTextFrontEL(pDom, &pWall->m_footing, FALSE, TRUE);
	DBStd.m_pDrawFooting->DimFrontUpper(pDom, &pWall->m_footing, 0, TRUE);
	DBStd.m_pDrawFooting->DimFrontSide(pDom, &pWall->m_footing, 0, TRUE, TRUE);

	DBStd.DrawCuttingTvArray(pDom, tvArr, rect, FALSE);
	pDom->CutLightning(rect.BottomLeft().x, rect.BottomLeft().y, rect.BottomRight().x, rect.BottomRight().y, FALSE, 1);

	pXLText->InsertDomyun(pDom, m_pStd->m_pView, 20, CXLFormatText::INSERT_DOM_CENTER | CXLFormatText::INSERT_DOM_MOVEROW);
	*pXLText += 1;
}

// void COutDesignBridge::MakeExcelSTM_FootingMid_CalcLoad(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nColPos, long nIndexTitle, long nIndexMiddle, HGINT32 iKey)
// {
// 
// }

void COutDesignBridge::GetTvArrSTM_Corner_Section( CTwinVectorArray &tvArrCorner, CTwinVectorArray &tvArrDimTopBot , CTwinVectorArray &tvArrDimLeftRight, CRcBridgeRebar *pBri, HGINT32 nPos )
{
	//���� 
	long nDirSlab = -1; 
	long nDirWall = -1; 

	CTwinVectorArray tvArr;
	CTwinVector tv;

	if(nPos == POS_L_UP)
	{
		nDirWall = +1;
		nDirSlab = +1;		
	}
	else if(nPos == POS_L_LO)
	{
		nDirWall = +1;
		nDirSlab = -1;		
	}
	else if(nPos == POS_R_UP)
	{
		nDirWall = -1;
		nDirSlab = +1;		
	}
	else //POS_R_LO
	{
		nDirWall = -1;
		nDirSlab = -1;		
	}

	double dextend = 2000;
	double dThickWall = 0;
	double dThickSalb = 0;
	double dHunchWidth =0, dHunchHeight =0;
	CDPoint xy1(0,0);
	HGBOOL bUpper = (nPos == POS_L_UP || nPos == POS_R_UP) ? TRUE : FALSE;
	HGBOOL bStt = (nPos == POS_L_UP || nPos == POS_L_LO) ? TRUE : FALSE;

	dThickSalb = bUpper? pBri->m_dTS : pBri->m_dTS_Lower;
	dThickWall = pBri->GetThickWall(bStt? 0 : pBri->m_nQtyJigan, FALSE);
	dHunchWidth = pBri->GetWidthHunch(bStt? 0 : pBri->m_nQtyJigan, !bStt, bUpper);
	dHunchHeight = pBri->GetThickHunch(bStt? 0 : pBri->m_nQtyJigan, !bStt, FALSE, bUpper);
	dextend = MAX(2000, dHunchWidth + 500);

	/////���������� ���� �찢�θ� ����(��ġ�� ���߿� ���)
	//1
	tv.m_v1 = xy1;

	tv.m_v2 = tv.m_v1;
	tv.m_v2.x += dThickWall * nDirWall;
	tvArr.Add(tv);

	//2
	tv.m_v1 = tv.m_v2;
	tv.m_v2.x += dextend * nDirWall;
	tvArr.Add(tv);

	//3
	tv.m_v1 = tv.m_v2;
	tv.m_v2.y -= dThickSalb * nDirSlab;
	tvArr.Add(tv);

	//4
	tv.m_v1 = tv.m_v2;
	tv.m_v2.x -= dextend * nDirWall;
	tvArr.Add(tv);

	//5
	tv.m_v1 = tv.m_v2;
	tv.m_v2.y -= dextend * nDirSlab;
	tvArr.Add(tv);

	//6
	tv.m_v1 = tv.m_v2;
	tv.m_v2.x -= dThickWall * nDirWall;
	tvArr.Add(tv);

	//7
	tv.m_v1 = tv.m_v2;
	tv.m_v2.y += dextend * nDirSlab;
	tvArr.Add(tv);

	//8
	tv.m_v1 = tv.m_v2;
	tv.m_v2.y += dThickSalb * nDirSlab;
	tvArr.Add(tv);

	//Hunch
	CTwinVector tvHunch, tv1, tv2;
	CTwinVectorArray tvArrTemp;
	CDPoint vAng1(0,0), vAng2(0,0);

	//4�� 5�� tv 
	tv1 = tvArr.GetAt(3);
	tv2 = tvArr.GetAt(4);

	vAng1 = tv1.GetXyDir(TRUE);
	vAng2 = tv2.GetXyDir(TRUE);

	tv1.m_v2.x -=  dHunchWidth * nDirWall * -1;
	tv2.m_v1.y += dHunchHeight * nDirSlab * -1;

	tvArr.SetAt(3, tv1);
	tvArr.SetAt(4, tv2);

	tvArrTemp.AddFromTvArray(tvArr);

	tvArr.RemoveAt(4);
	tvArr.RemoveAt(4);
	tvArr.RemoveAt(4);
	tvArr.RemoveAt(4);

	tvArrTemp.RemoveAt(0);
	tvArrTemp.RemoveAt(0);
	tvArrTemp.RemoveAt(0);
	tvArrTemp.RemoveAt(0);

	tvHunch.m_v1 = tv1.m_v2;
	tvHunch.m_v2 = tv2.m_v1;

	tvArr.Add(tvHunch);		

	for (long nIdx =0; nIdx < tvArrTemp.GetSize(); nIdx++)
	{
		tv = tvArrTemp.GetAt(nIdx);
		tvArr.Add(tv);
	}

	tvArrCorner.AddFromTvArray(tvArr);


	tv = tvArrCorner.GetAt(0);

	tvArrDimTopBot.Add(tv);

	CTwinVector tvTmp;

	tvTmp = tv;
	tvTmp.m_v1= tvTmp.m_v2;
	tvTmp.m_v2.x += dHunchWidth * nDirWall;

	tvArrDimTopBot.Add(tvTmp);

	CTwinVector tvLR;
	tvLR = tvArrCorner.GetAt(tvArrCorner.GetSize()-1);
	swap(tvLR.m_v1, tvLR.m_v2);

	tvArrDimLeftRight.Add(tvLR);

	tvTmp = tvLR;
	tvTmp.m_v1 = tvLR.m_v2;
	tvTmp.m_v2.y -= dHunchHeight * nDirSlab;

	tvArrDimLeftRight.Add(tvTmp);	
}

void COutDesignBridge::MakeExcelConditionUseMaterial(CXLFormatText *pXLText, long &nIndexSmall, long &nIndexTiny, long &nTypeNumber, long nT_1, long nT_2)
{
	CARcBridgeDataStd *pData = m_pStd->m_pARcBridgeDataStd;

	double Fck = GetValueUnitChange(pData->m_Fck,UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2);
	double Fy  = GetValueUnitChange(pData->m_Fy ,UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2);
	double Ec  = GetValueUnitChange(pData->m_Ec ,UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2);
	double Es  = GetValueUnitChange(pData->m_Es ,UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2);
	double Wc  = (pData->m_pBasicConcInfo->m_UWeightConcrete>0) ? pData->m_pBasicConcInfo->m_UWeightConcrete/1000 : 2.350;

	pXLText->AddFormatText("$h %d) �� ũ �� Ʈ$n",nIndexSmall++);
	pXLText->GoPosition(NO_MOVE,nT_2);
//	pXLText->(nT_2);
	pXLText->AddFormatText("$h%s$rfck$r$c=$r %.1f %s$n",GetNumberString(nIndexTiny++,nTypeNumber),Fck,pData->m_szKgf_CM2);

	double dEcCheck = GetValueEcConc(pData->m_Fck,Wc,pData->IsSI(),pData->m_nTypeKciCode);
	// ��� ����� ���Ŀ� ���Ѱ��� �ٸ���� ����� �Է°����� �Ǵ��Ͽ� �߰����� �����Ѵ�.
	if(Round(pData->m_Ec, -2) != Round(dEcCheck, -2))
	{
		pXLText->AddFormatText("$h%s$rEc $r$c=$r%.1f %s$n",GetNumberString(nIndexTiny++,nTypeNumber),Ec,pData->m_szKgf_CM2);
	}
	else if(Compare(Wc,2.30,"="))
	{
		if(pData->IsRailDesign2017())
		{
			pXLText->AddFormatText("$h%s$rEc$r$c=$r%.1f %s$n",GetNumberString(nIndexTiny++,nTypeNumber),Ec,pData->m_szKgf_CM2);
		}
		else
		{
			if(pData->IsSI())
			{
				if(pData->m_nTypeKciCode==0)
				{
					if(pData->m_Fck>300)
						pXLText->AddFormatText("$h%s$rEc $r$c=$r$p[EC_SI_4]$m+05$c=$r%.1f %s$n",GetNumberString(nIndexTiny++,nTypeNumber),Ec,pData->m_szKgf_CM2);
					else
						pXLText->AddFormatText("$h%s$rEc $r$c=$r$p[EC_SI_2]$m+03$c=$r%.1f %s$n",GetNumberString(nIndexTiny++,nTypeNumber),Ec,pData->m_szKgf_CM2);
				}
				else if (pData->m_nTypeKciCode == 1) // Kci2007
				{
					pXLText->AddFormatText("$h%s$rEc $r$c=$r$p[EC_SI_2_07]$m+05$c=$r%.1f %s , �̶� fcu=fck+8 (MPa)$n",GetNumberString(nIndexTiny++,nTypeNumber),Ec,pData->m_szKgf_CM2);
				}
				else // Kci2012
				{
					pXLText->AddFormatText("$h%s$rEc $r$c=$r$p[EC_SI_2_07]$m+05$c=$r%.1f %s , �̶� fcu=fck+��f (Mpa)$n",GetNumberString(nIndexTiny++,nTypeNumber),Ec,pData->m_szKgf_CM2);
				}
			}
			else
			{
				if(pData->m_Fck>300)
					pXLText->AddFormatText("$h%s$rEc $r$c=$r 10500 x ��fck + 70000 = %.1f %s$n",GetNumberString(nIndexTiny++,nTypeNumber),Ec,pData->m_szKgf_CM2);
				else
					pXLText->AddFormatText("$h%s$rEc $r$c=$r 15000 x ��fck = %.1f %s$n",GetNumberString(nIndexTiny++,nTypeNumber),Ec,pData->m_szKgf_CM2);
			}
		}
	}
	else
	{
		if(pData->IsSI())
		{
			if(pData->m_nTypeKciCode==0)
			{
				if(pData->m_Fck>300)
					pXLText->AddFormatText("$h%s$rEc $r$c=$r$p[EC_SI_3]$m+06$c=$r0.030 x %.1f��%.1f + 7700$c=$r%.1f %s$n",GetNumberString(nIndexTiny++,nTypeNumber),Wc,Fck,Ec,pData->m_szKgf_CM2);
				else
					pXLText->AddFormatText("$h%s$rEc $r$c=$r$p[EC_SI_1]$m+04$c=$r0.043 x %.1f��%.1f$c=$r%.1f %s$n",GetNumberString(nIndexTiny++,nTypeNumber),Wc,Fck,Ec,pData->m_szKgf_CM2);
			}
			else if (pData->m_nTypeKciCode == 1) // Kci2007
			{
				pXLText->AddFormatText("$h%s$rEc $r$c=$r$p[EC_SI_1_07]$m+05$c=$r%.1f %s , �̶� fcu=fck+8 (MPa)$n",GetNumberString(nIndexTiny++,nTypeNumber),Ec,pData->m_szKgf_CM2);
			}
			else // Kci2012
			{
				pXLText->AddFormatText("$h%s$rEc $r$c=$r$p[EC_SI_1_07]$m+05$c=$r%.1f %s , �̶� fcu=fck+��f (Mpa)$n",GetNumberString(nIndexTiny++,nTypeNumber),Ec,pData->m_szKgf_CM2);
			}
		}
		else
		{
			if(pData->m_Fck>300)
				pXLText->AddFormatText("$h%s$rEc $r$c=$r 3000 x %.1f^1.5 x ��%.1f + 70000 = %.1f %s$n",GetNumberString(nIndexTiny++,nTypeNumber),Wc,Fck,Ec,pData->m_szKgf_CM2);
			else
				pXLText->AddFormatText("$h%s$rEc $r$c=$r 4270 x %.1f^1.5 x ��%.1f = %.1f %s$n",GetNumberString(nIndexTiny++,nTypeNumber),Wc,Fck,Ec,pData->m_szKgf_CM2);
		}
	}

	pXLText->AddFormatText("$n");
	nIndexTiny = 1;
	//	XLText(nT_1);
	pXLText->GoPosition(NO_MOVE,nT_1);
	pXLText->AddFormatText("$h %d) ö ��$n",nIndexSmall);
//	XLText(nT_2);
	pXLText->GoPosition(NO_MOVE,nT_2);
	pXLText->AddFormatText("$h%s$rfy $r$c=$r %.1f %s$n",GetNumberString(nIndexTiny++,nTypeNumber),Fy,pData->m_szKgf_CM2);
	pXLText->AddFormatText("$h%s$rEs $r$c=$r %.1f %s$n",GetNumberString(nIndexTiny++,nTypeNumber),Es,pData->m_szKgf_CM2);
}

void COutDesignBridge::MakeExcelConditionUseMaterialLsd(CXLFormatText *pXLText, CRcBridgeRebar *pBri)
{
	CARcBridgeDataStd *pData = m_pStd->m_pARcBridgeDataStd;

	if(pBri->GetMaterialDetail())
	{
		CStringArray sArrTitle;
		CDoubleArray dArrFck, dArrFcm, dArrEc, dArrFctm, dArrFctk, dArrFy, dArrFvy, dArrEs;

		for (HGINT32 ix = ePartUpperSlab; ix < ePartCount; ++ix)
		{
			EPartOfBridge eNum = static_cast<EPartOfBridge>(ix);

			if(pBri->IsBoxType() && (ix == ePartFootStt || ix == ePartFootEnd || ix == ePartInFoot))
				continue;
			if(pBri->m_nQtyJigan < 2 && (ix == ePartInFoot || ix == ePartInWall))
				continue;
			if(!pBri->IsBoxType() && ix == ePartLowerSlab)
				continue;


			double dFck = GetValueUnitChange(pBri->m_Fck[ix],UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2);
			dArrFck.Add(dFck);				// fck
			dArrFcm.Add(GetValueFcm(dFck));	// fcm
			dArrEc.Add(GetValueUnitChange(pBri->m_Ec[ix],UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2));					// ec
			dArrFctm.Add(GetValueFctm(dFck));	// fctm
			dArrFctk.Add(GetValueFctk(dFck));	// fctk
			dArrFy.Add(GetValueUnitChange(pBri->m_Fy[ix],UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2));					// fy
			dArrFvy.Add(GetValueUnitChange(pBri->m_Fvy[ix],UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2));				// fvy
			dArrEs.Add(GetValueUnitChange(pData->m_Es,UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2));					// es

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

			sArrTitle.Add(strPart);
		}

		CStringArray sArrFactorName;
		sArrFactorName.Add(_T("���ؾ��భ��(fck)"));
		sArrFactorName.Add(_T("��վ��భ��(fcm)"));
		sArrFactorName.Add(_T("ź�����(Ec)"));
		sArrFactorName.Add(_T("������尭��(fctm)"));
		sArrFactorName.Add(_T("�������尭��(fctk)"));
		sArrFactorName.Add(_T("�����׺�����(fy)"));
		sArrFactorName.Add(_T("�����׺�����(fvy)"));
		sArrFactorName.Add(_T("��� ź�����(Es)"));

		////////////////////////////////////////////////////////////////////////// Table
		HGINT32 icTitle = sArrTitle.GetCount();

		for (HGINT32 ix = 0; ix < icTitle; ix+=4)
		{
			CString strText;
			CString strDec;

			// Title
			if(sArrTitle.GetSize() > ix+3)
			{
				pXLText->AddFormatText(_T("$h$m+23$i%s %s$n$h"), _T("���� :"), pData->m_szTonf_M2);
				pXLText->AddTable(8, 23, 1, 8);

				strText.Format(_T("$h$c$me7���� $m+08$c$me3%s $m+04$c$me3%s $m+04$c$me3%s $m+04$c$me3%s $n"), 
					sArrTitle.GetAt(ix), sArrTitle.GetAt(ix+1), sArrTitle.GetAt(ix+2), sArrTitle.GetAt(ix+3));
			}
			else
			{
				long nCntCol = 7; //ix
				long nIdx =0;
				for (nIdx = 0; nIdx < 3; nIdx++)
				{
					if( ix+nIdx < sArrTitle.GetSize())
						nCntCol += 4;
					else
						break;
				}

				pXLText->AddFormatText(_T("$h$m+%02d$i%s %s$n$h"), nCntCol, _T("���� :"), pData->m_szTonf_M2);
				pXLText->AddTable(8, nCntCol, 1, 8);

				CString sText1, sText2;
				sText1.Format(_T("$h$c$me7���� "));

				for (long lx =0; lx < nIdx; lx++)
				{
					CString sName = sArrTitle.GetAt(ix + lx);
					if(lx == 0)
						sText2.Format(_T("$m+08$c$me3%s "), sName);
					else
						sText2.Format(_T("$m+04$c$me3%s "), sName);
					sText1 += sText2;
				}
				sText1 += _T("$n");

				strText = sText1;
			}

			pXLText->AddText(hggettext(strText));
			//pXLText->OutXL();

			// Data
			for (HGINT32 iy = 0; iy < 8; ++iy)
			{
				HGINT32 nDecConc = (iy==3 || iy==4) ? 3 : 0;
				strDec.Format(_T("%%.%df"), nDecConc);
				// Title
				strText.Format(_T("%s"), iy < 5? _T("��ũ��Ʈ") : _T("ö ��"));
				if( iy == 0)
					pXLText->AddFormatText(_T("$h$me2$md4$c%s"), strText);
				else if(iy == 5)
					pXLText->AddFormatText(_T("$h$me2$md2$c%s"), strText);

				// ����
				strText.Format(_T("%s"), sArrFactorName.GetAt(iy));
				pXLText->AddFormatText(_T("$m+03$me4$c%s"), strText);

				for ( HGINT32 iz = ix; iz < ix+4; ++iz )
				{
					HGINT32 nM = iz==ix ? 5 : 4;
					double dResult(0.);

					if(iz >= dArrFck.GetSize())
						continue;

					if( iy == 0 ) dResult = dArrFck.GetAt(iz);
					else if( iy == 1 ) dResult = dArrFcm.GetAt(iz);
					else if( iy == 2 ) dResult = dArrEc.GetAt(iz);
					else if( iy == 3 ) dResult = dArrFctm.GetAt(iz);
					else if( iy == 4 ) dResult = dArrFctk.GetAt(iz);
					else if( iy == 5 ) dResult = dArrFy.GetAt(iz);
					else if( iy == 6 ) dResult = dArrFvy.GetAt(iz);
					else if( iy == 7 ) dResult = dArrEs.GetAt(iz);

					strText = GetStrDouble(dResult, nDecConc, pData->IsLsdDesign(), FALSE, FALSE);
					pXLText->AddFormatText(_T("$m+%02d$me3$c%s"), nM, strText);
				}

				pXLText->AddFormatText(_T("$n$h"));
			}

			pXLText->AddFormatText(_T("$h���⼭, fctm = 0.3(fcm)^��, fctk = 0.70 fctm$n"));
			pXLText->AddFormatText(_T("$n"));		
		}
	}
	else
	{
		long nIdxCircle = 0;
		double dFck = GetValueUnitChange(pBri->m_Fck[ePartUpperSlab],UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2);
		double dFcm = GetValueFcm(dFck);
		double dEc = GetValueUnitChange(pBri->m_Ec[ePartUpperSlab],UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2);
		double dfctm = GetValueFctm(dFck);
		double dfctk = GetValueFctk(dFck);
		double dFy = GetValueUnitChange(pBri->m_Fy[ePartUpperSlab],UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2);
		double dFvy = GetValueUnitChange(pBri->m_Fvy[ePartUpperSlab],UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2);
		double dEs = GetValueUnitChange(pData->m_Es,UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2);
		
		pXLText->AddFormatText(_T("$h 1) �� ũ �� Ʈ$n"));
		pXLText->AddFormatText(_T("$h$r%s$rfck$m+02$c=$m+04$i%.1f N/mm^2$n"), GetNumberString(++nIdxCircle, NUM_CIRCLE), dFck);
		pXLText->AddFormatText(_T("$h$r%s$rfcm$m+02$c=$m+04$i%.1f N/mm^2$n"), GetNumberString(++nIdxCircle, NUM_CIRCLE), dFcm);
		pXLText->AddFormatText(_T("$h$r%s$rEc$m+02$c=$r0.077mc^(1.5)��^3��fcm = %.1f N/mm^2, �̶� fcm=fck+��f (N/mm^2)$n"), GetNumberString(++nIdxCircle, NUM_CIRCLE), dEc);
		pXLText->AddFormatText(_T("$h$r%s$rfctm$m+02$c=$m+04$i%.2f N/mm^2$n"), GetNumberString(++nIdxCircle, NUM_CIRCLE), dfctm);
		pXLText->AddFormatText(_T("$h$r%s$rfctk$m+02$c=$m+04$i%.3f N/mm^2$n"), GetNumberString(++nIdxCircle, NUM_CIRCLE), dfctk);
		*pXLText += 1;

		nIdxCircle = 0;
		pXLText->AddFormatText(_T("$h 2) ö ��$n"));
		pXLText->AddFormatText(_T("$h$r%s$rfy$m+02$c=$m+04$i%.1f N/mm^2$n"), GetNumberString(++nIdxCircle, NUM_CIRCLE), dFy);
		pXLText->AddFormatText(_T("$h$r%s$rfvy$m+02$c=$m+04$i%.1f N/mm^2$n"), GetNumberString(++nIdxCircle, NUM_CIRCLE), dFvy);
		pXLText->AddFormatText(_T("$h$r%s$rEs$m+02$c=$m+04$i%.1f N/mm^2$n"), GetNumberString(++nIdxCircle, NUM_CIRCLE), dEs);
	}
	
}

void COutDesignBridge::MakeExcelCombine_Lsd( CXLControl *pXL, CRcBridgeRebar *pBri )
{
	long nIndexMiddle = 1;

	m_pStd->SetExcelFormat(pXL, XFL_ROWSTT, XL_ROWEND_DESIGN);

	CXLFormatText XLText(pXL);
	XLText.AddFormatText("$e$tb13%d. ��������$n$n",m_nIndexTitle);			
	
	////��������(����)
	CLsdManager *pLsdManager = pBri->m_plsdManager;
	if(pLsdManager == NULL) return;

	long nTypeWrite			= m_pStd->m_pARcBridgeDataStd->m_bPrintConditionApply? LSD_COMB_BASE : LSD_COMB_DETAIL;			//���� ����� �����ų��

	if(nTypeWrite == LSD_COMB_BASE)
	{
		pLsdManager->WriteTableLsdCombine_Base(&XLText, m_nIndexTitle, nIndexMiddle, COL_POS_1);
	}
	else
	{
		ELSDLoadCaseDir eDir =  eLSD_LOADCASE_DIR_BRI;

		for(long ixComboType=0; ixComboType<eLSD_COMBINATION_FATIGUE; ++ixComboType)
		{
			pLsdManager->WriteTableLsdCombine_Detail(&XLText, eDir, ixComboType, m_nIndexTitle, nIndexMiddle, COL_POS_1);
			XLText += 1;
		}
	}
}

void COutDesignBridge::MakeExcelForceSummarySheet_Lsd( CXLControl *pXL, CRcBridgeRebar *pBri )
{
	CXLFormatText XLText(pXL);
	CString str1, str2;

	long nT_1 = 1;
	long nIndexMiddle = 1;

	pBri->TrimRibByFootingUpper();

	XLText.AddFormatText("$tb11%d. �ܸ�� ����$n$n",m_nIndexTitle);				
	//////////////////////////////////////////////////////////////////////////
	// 1. ���߼������ ����ǥ
	XLText(nT_1);
	XLText.AddFormatText("$h$a%d.%d ���߼������(��) ����ǥ$n",m_nIndexTitle,nIndexMiddle++);
	XLText += 1;
	long nIndexSub(1);
	long icTable = pBri->m_plsdManager->GetLsdLoadModifiedCount();
	for(long ix=0; ix<icTable; ++ix)
	{
		// �ش� �������� �ִ��� üũ����.
		if(ix == pBri->m_plsdManager->GetindexLoadModifiedFactor(ePartOutWallStt))	// ��ü
		{
			if(pBri->IsOutsideWall() == FALSE)
				continue;
		}
		else if(ix == pBri->m_plsdManager->GetindexLoadModifiedFactor(ePartFootStt))	// ��������
		{
			if(pBri->IsBoxType() || pBri->IsFooting(TRUE) == FALSE)
				continue;
		}
		else if(ix == pBri->m_plsdManager->GetindexLoadModifiedFactor(ePartWingWallStt))	// ������
		{
			if(pBri->IsWingWall() == FALSE)
				continue;
		}
		else if(ix == pBri->m_plsdManager->GetindexLoadModifiedFactor(ePartApsStt))	// ���ӽ�����
		{
			if(pBri->IsAps(TRUE, TRUE) == FALSE && pBri->IsAps(FALSE, TRUE) == FALSE)
				continue;
		}
		else if(ix == pBri->m_plsdManager->GetindexLoadModifiedFactor(ePartInWall))	// ������ü
		{
			if(pBri->GetCountInWall() == 0)
				continue;
		}
		else if(ix == pBri->m_plsdManager->GetindexLoadModifiedFactor(ePartInFoot))	// ��������
		{
			if(pBri->GetCountInWall() == 0 || pBri->IsBoxType() || pBri->IsArchType() || pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2)
				continue;
		}
		m_pCalc->m_pLoad->MakeTableOutValueEta_Lsd(&XLText, ix, nIndexSub++);
	}
	XLText += 1;
	
	XLText(nT_1);
	XLText.AddFormatText("$h$a%d.%d �ڴܸ���� �����$n",m_nIndexTitle,nIndexMiddle++);
	XLText += 1;

	long nIdxCircle(0);
	for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
	{
		ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
		CString szTitle = pBri->GetStringComboEnvType_Lsd(eEnvType);
		if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
			continue;

		XLText.GoPosition(NO_MOVE, COL_POS_1);
		XLText.AddFormatText(_T("$h%s$r%s$n$h"), GetNumberString(++nIdxCircle, NUM_CIRCLE), szTitle);

		if(eEnvType == eEnvExtremeI || eEnvType == eEnvExtremeII)
		{
			if(pBri->IsBoxType())
			{
				MakeTableSectionForceEQ_BOX(&XLText, pBri, TRUE, eEnvType);
				XLText += 1;
				MakeTableSectionForceEQ_BOX(&XLText, pBri, FALSE, eEnvType);
			}
			else
				MakeTableSectionForceEQ(&XLText, pBri, eEnvType);
		}
		else
		{
			MakeTableSectionForce(&XLText, pBri, eEnvType);
		}

		XLText += 1;
	}
	XLText += 1;

	if(pBri->IsBoxType() == FALSE)
	{
		XLText(nT_1);
		XLText.AddFormatText("$h$a%d.%d ���ʰ��� �����$n",m_nIndexTitle,nIndexMiddle++);
		XLText += 1;

		nIdxCircle = 0;
		//////////////////////////////////////////////////////////////////////////
		// 3. ��������ü ���ʰ���� ����	4. ������ ��ü ���ʰ���� ����
		if(pBri->IsOutsideWall())
		{
			for(long ix=0; ix<2; ix++)
			{
				str1 = (ix==0)? "������":"������";
				XLText.GoPosition(NO_MOVE, COL_POS_1);
				XLText.AddFormatText(_T("$h%s$r%s ����$n$h"), GetNumberString(++nIdxCircle, NUM_CIRCLE), str1);
				
				m_pCalc->m_pFooting->MakeTableLoadForFooting_Lsd(&XLText, pBri, (ix==0)? 0 : pBri->m_nQtyJigan, FALSE, TRUE, TRUE);
				if(pBri->IsArchType() || pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2)
				{
					// (ARCBRIDGE-2837) ���ʿ� ������ �ΰ� ���� Ÿ���� ���̺��� �ϳ� �� ����ش�.
					XLText.AddFormatText(_T("$h%s$r%s ���� ��米��$n$h"), GetNumberString(++nIdxCircle, NUM_CIRCLE), str1);
					m_pCalc->m_pFooting->MakeTableLoadForFooting_Lsd(&XLText, pBri, (ix==0)? 0 : pBri->m_nQtyJigan, TRUE, TRUE, TRUE);
				}
				XLText += 1;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		// 5. ����(n)���ʰ���� ���� ~
		long nCntInWall = pBri->GetCountInWallReal();
		if(pBri->IsArchType() || pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2)
		{
			nCntInWall = 0;
		}
		for(long ix=0; ix<nCntInWall; ix++)
		{
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH) break;
			str1.Format("�߰���ü(�߰�����%d) ����", ix+1);

			XLText.GoPosition(NO_MOVE, COL_POS_1);
			XLText.AddFormatText(_T("$h%s$r%s$n$h"), GetNumberString(++nIdxCircle, NUM_CIRCLE), str1);

			m_pCalc->m_pFooting->MakeTableLoadForFooting_Lsd(&XLText, pBri, ix+1, FALSE, TRUE, TRUE);
			XLText += 2;
		}
	}

	if((pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign == 1) || (pBri->GetCountInWall() > 0 && pBri->m_nSelectInWallSectionDesign == 1))
	{
		XLText(nT_1);
		XLText.AddFormatText("$h$a%d.%d ��ü���� �����$n",m_nIndexTitle,nIndexMiddle++);
		XLText += 1;
	}

	nIdxCircle = 0;
	//////////////////////////////////////////////////////////////////////////
	// 3. ��������ü ���ʰ���� ����	4. ������ ��ü ���ʰ���� ����
	if(pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign == 1)
	{
		for(long ix=0; ix<2; ix++)
		{
			str1 = (ix==0)? "������":"������";
			XLText.GoPosition(NO_MOVE, COL_POS_1);
			XLText.AddFormatText(_T("$h%s$r%s ��ü$n$h"), GetNumberString(++nIdxCircle, NUM_CIRCLE), str1);
			
			m_pCalc->m_pColGir->MakeTableLoadForWallCheck_Lsd(&XLText, pBri, (ix==0)? 0 : pBri->m_nQtyJigan);
			XLText += 2;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 5. ����(n)���ʰ���� ���� ~
	if(pBri->m_nSelectInWallSectionDesign == 1)
	{
		long nCntInWall = pBri->GetCountInWallReal();
		for(long ix=0; ix<nCntInWall; ix++)
		{
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH) break;
			str1.Format("�߰���ü(�߰�����%d)", ix+1);

			XLText.GoPosition(NO_MOVE, COL_POS_1);
			XLText.AddFormatText(_T("$h%s$r%s$n$h"), GetNumberString(++nIdxCircle, NUM_CIRCLE), str1);

			m_pCalc->m_pColGir->MakeTableLoadForWallCheck_Lsd(&XLText, pBri, ix+1);
			XLText += 2;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// �ܸ�µ� (BMD,SFD,AFD)
	XLText.AddFormatText("$s+45$h$a$e%d.%d �ܸ�µ�$n", m_nIndexTitle, nIndexMiddle++);
	XLText += 1;
	MakeDiagramBMD_SFD_Lsd(&XLText,pBri);
}

void COutDesignBridge::MakeDiagramBMD_SFD_Lsd( CXLFormatText *XLText, CRcBridgeRebar *pBri )
{
	CFEMManage *pFemNormal = &pBri->m_FemNormal;
	CFEMManage *pFemEQ1    = pBri->GetFemManage(COMBO_EARTHQUAKE);
	CFEMManage *pFemEQ2    = pBri->GetFemManage(COMBO_EARTHQUAKE2);

	if(pFemNormal->IsOutputLoaded() == FALSE)
		return;

	long nCountEQ(0);
	if(pFemEQ1 && pFemEQ1->IsOutputLoaded()) ++nCountEQ;
	if(pFemEQ2 && pFemEQ2->IsOutputLoaded()) ++nCountEQ;

	double dScaleDim = 200;
	CDomyun *pDomMain = new CDomyun;
	pDomMain->SetCWnd(m_pStd->m_pView);
	pDomMain->SetScaleDim(dScaleDim);
	pDomMain->SetLTScale(dScaleDim*5);

	long nIndexMiddle = 1;
	long nSizeRow = 15;

	CDRect rc;
	XLText->GoPosition(NO_MOVE,COL_POS_1);

	for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
	{
		ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
		if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
			continue;

		BOOL bEarthQuake = (eEnvType == eEnvExtremeI || eEnvType == eEnvExtremeII)? TRUE : FALSE;
		long nCountModel = bEarthQuake? nCountEQ : 1;
		if(nCountModel == 0)
			continue;
	
		for(long ixPart=0; ixPart<ePartCount; ++ixPart)
		{
			if(pBri->m_bEtaCombinEach == FALSE && ixPart > 0)
				break;

			if(pBri->IsValidBridgePart((EPartOfBridge)ixPart) == FALSE)
				continue;

			if(pBri->IsUsePart((EPartOfBridge)ixPart, 1) == FALSE)
				continue;

			ELSDCombKind eComboKind = pBri->GetComboKindByEnvType(eEnvType, (EPartOfBridge)ixPart, CHECK_POS_CENTER);
			CString strDescription = pBri->GetStringComboEnv_Lsd(eComboKind, (EPartOfBridge)ixPart, FALSE);
			CString szEnvCombo = pBri->GetStringComboEnv(0, eEnvType, (EPartOfBridge)ixPart, CHECK_POS_CENTER);

			XLText->AddFormatText("$h$c%d)$r%s(%s)$n$n", nIndexMiddle++, strDescription, szEnvCombo);
			
			long nIndexSmall  = 1;
			for(long ixModel=0; ixModel<nCountModel; ++ixModel)
			{
				CFEMManage *pFem = bEarthQuake? ((ixModel == 0)? pFemEQ1 : pFemEQ2) : pFemNormal;
				long nIdx = pFem->GetLoadCaseByString(szEnvCombo);
				if(nIdx < 0) 
					continue;

				CString szModel(_T(""));
				if(bEarthQuake && nCountEQ > 1)
				{
					szModel = (ixModel == 0)? _T(" : (��������)") : _T(" : (��������)");
				}

				for(long ixForce=0; ixForce<2; ++ixForce)	// B.M.D , S.F.D
				{
					BOOL bBMD = (ixForce == 0);
					XLText->AddFormatText("$h$r$c%s$r%s%s$n$n",GetNumberString(nIndexSmall++,NUM_CIRCLE), bBMD? _T("B.M.D") : _T("S.F.D"), szModel);
					
					CDomyun *pDom1 = new CDomyun(pDomMain);
					CForceDiagram ForceDiagram(pFem);
					ForceDiagram.SetScaleDim(pDomMain->GetScaleDim());

					ForceDiagram.DrawDiagramForce(nIdx, bBMD? ELE_MOMENT2 : ELE_SHEAR_2,FALSE,m_pStd->m_pARoadOptionStd, 1000);
					rc = ForceDiagram.GetExtRect();
					ForceDiagram.Point(rc.TopLeft().x-2, rc.TopLeft().y+2);
					ForceDiagram.Point(rc.BottomRight().x+2, rc.BottomRight().y+2);
					
					*pDom1 << ForceDiagram;

					CDRect rc1 = pDom1->GetExtRect();
					if(nSizeRow * rc1.Width()/rc1.Height() > 32) nSizeRow = (long)(32*rc1.Height()/rc1.Width());
					XLText->InsertDomyun(pDom1,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_MOVEROW);

					*XLText += 1;
				}
			}

			if(pBri->m_nSelect3DShellTool==1) // MIDAS
			{
				XLText->AddFormatText("$h$r$c%s$r3D SHELL : %s$n$n",GetNumberString(nIndexSmall++,NUM_CIRCLE), _T("B.M.D"));
				*XLText += 1;
				XLText->AddFormatText("$h$r$c%s$r3D SHELL : %s$n$n",GetNumberString(nIndexSmall++,NUM_CIRCLE), _T("S.F.D"));
				*XLText += 1;
			}
		}
	}

	delete pDomMain;
}

CString COutDesignBridge::GetStringSectionForceTableAtWall( CRcBridgeRebar *pBri, ETypeEnvComboLsd eEnvTypeLsd, long nJijumIndex, long nRow, long nCol, long &nMergRow, CString &strOpt, BOOL bLsd, BOOL b3D, BOOL b3DLoad )
{
	CString str(_T(""));
	strOpt = _T("");
	BOOL bGirderCalc = FALSE;
	BOOL bOutSideWall= TRUE; // TRUE-���,FALSE-����
	nMergRow = 0;
	double dValue(0);

	if(nCol==0)
	{
		if(nJijumIndex == 0)
			str = _T("������ü");
		else if(nJijumIndex == pBri->m_nQtyJigan)
			str = _T("������ü");
		else
			str.Format(_T("�߰�����%d"), nJijumIndex);

		if(nRow == 0) nMergRow = 4;
		else nMergRow = 0;
	}
	else if(nCol==1)
	{
		switch(nRow) 
		{
		case 4:
			str = "�ϴ�(���)";
			break;
		case 3:
			str = "�ϴ�(����)";
			break;
		case 2:
			str = "�߾�(���)";
			break;
		case 1:
			str = "�߾�(����)";
			break;
		case 0:
			str = "��ܺ�";
			break;
		}
		strOpt = _T("");
	}
	else if(nCol==2)      // ������� 2D FRAME ���Ʈ
	{
		switch(nRow) 
		{
		case 4:
			dValue = pBri->GetOutValueWall(nJijumIndex, CHECK_POS_BOTTOM, COMBO_ULTIMATE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
			break;
		case 3:
			dValue = pBri->GetOutValueWall(nJijumIndex, CHECK_POS_BOTTOM, COMBO_ULTIMATE, ELE_MOMENT2, bGirderCalc,!bOutSideWall, eEnvTypeLsd);
			break;
		case 2:
			dValue = pBri->GetOutValueWall(nJijumIndex, CHECK_POS_CENTER, COMBO_ULTIMATE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
			break;
		case 1:
			dValue = pBri->GetOutValueWall(nJijumIndex, CHECK_POS_CENTER, COMBO_ULTIMATE, ELE_MOMENT2, bGirderCalc,!bOutSideWall, eEnvTypeLsd);
			break;
		case 0:
			dValue = pBri->GetOutValueWall(nJijumIndex, CHECK_POS_TOP, COMBO_ULTIMATE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
			break;
		}

		str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);

		strOpt = _T("");
	}
	else if(nCol==3 && b3D)      // ������� 3D SHELL ���Ʈ
	{
		switch(nRow) 
		{

		case 4:
			dValue = pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_BOTTOM, COMBO_ULTIMATE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
			break;
		case 3:
			dValue = pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_BOTTOM, COMBO_ULTIMATE, ELE_MOMENT2, bGirderCalc,!bOutSideWall, eEnvTypeLsd);
			break;
		case 2:
			dValue = pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_CENTER, COMBO_ULTIMATE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
			break;
		case 1:
			dValue = pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_CENTER, COMBO_ULTIMATE, ELE_MOMENT2, bGirderCalc,!bOutSideWall, eEnvTypeLsd);
			break;
		case 0:
			dValue = pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_TOP, COMBO_ULTIMATE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
			break;
		}
		strOpt = _T("");
		if(!b3DLoad) str = _T("-");
		else str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);
	}
	else if(nCol==4)      // ������� 2D FRAME ���Ʈ
	{
		if(bLsd) return str;

		switch(nRow) 
		{
		case 4:
			str.Format("$f3%.3f",pBri->GetOutValueWall(nJijumIndex, CHECK_POS_BOTTOM, COMBO_SERVICE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd));
			break;
		case 3:
			str.Format("$f3%.3f",pBri->GetOutValueWall(nJijumIndex, CHECK_POS_BOTTOM, COMBO_SERVICE, ELE_MOMENT2, bGirderCalc,!bOutSideWall, eEnvTypeLsd));
			break;
		case 2:
			str.Format("$f3%.3f",pBri->GetOutValueWall(nJijumIndex, CHECK_POS_CENTER, COMBO_SERVICE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd));
			break;
		case 1:
			str.Format("$f3%.3f",pBri->GetOutValueWall(nJijumIndex, CHECK_POS_CENTER, COMBO_SERVICE, ELE_MOMENT2, bGirderCalc,!bOutSideWall, eEnvTypeLsd));
			break;
		case 0:
			str.Format("$f3%.3f",pBri->GetOutValueWall(nJijumIndex, CHECK_POS_TOP, COMBO_SERVICE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd));
			break;
		}
		strOpt = _T("");
	}
	else if(nCol==5 && b3D)      // ������� 3D SHELL ���Ʈ
	{
		if(bLsd) return str;

		switch(nRow) 
		{
		case 4:
			str.Format("$f3%.3f",pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_BOTTOM, COMBO_SERVICE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd));
			break;
		case 3:
			str.Format("$f3%.3f",pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_BOTTOM, COMBO_SERVICE, ELE_MOMENT2, bGirderCalc,!bOutSideWall, eEnvTypeLsd));
			break;
		case 2:
			str.Format("$f3%.3f",pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_CENTER, COMBO_SERVICE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd));
			break;
		case 1:
			str.Format("$f3%.3f",pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_CENTER, COMBO_SERVICE, ELE_MOMENT2, bGirderCalc,!bOutSideWall, eEnvTypeLsd));
			break;
		case 0:
			str.Format("$f3%.3f",pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_TOP, COMBO_SERVICE, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd));
			break;
		}
		strOpt = _T("");
		if(!b3DLoad) str = _T("-");
	}
	else if(nCol==6)      // ������� 2D FRAME ���ܷ�
	{
		double dDistShearTop = pBri->GetDistCheckShearWall(nJijumIndex, CHECK_POS_TOP, NULL);
		double dDistShearBot = pBri->GetDistCheckShearWall(nJijumIndex, CHECK_POS_BOTTOM, NULL);

		switch(nRow) 
		{
		case 4:
			dValue = pBri->GetOutValueWall(nJijumIndex, CHECK_POS_BOTTOM, COMBO_ULTIMATE, ELE_SHEAR_2, bGirderCalc,bOutSideWall, eEnvTypeLsd, dDistShearBot);
			break;
		case 3:
			dValue = pBri->GetOutValueWall(nJijumIndex, CHECK_POS_BOTTOM, COMBO_ULTIMATE, ELE_SHEAR_2, bGirderCalc,!bOutSideWall, eEnvTypeLsd, dDistShearBot);
			break;
		case 2:
			dValue = pBri->GetOutValueWall(nJijumIndex, CHECK_POS_CENTER, COMBO_ULTIMATE, ELE_SHEAR_2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
			break;
		case 1:
			dValue = pBri->GetOutValueWall(nJijumIndex, CHECK_POS_CENTER, COMBO_ULTIMATE, ELE_SHEAR_2, bGirderCalc,!bOutSideWall, eEnvTypeLsd);
			break;
		case 0:
			dValue = pBri->GetOutValueWall(nJijumIndex, CHECK_POS_TOP, COMBO_ULTIMATE, ELE_SHEAR_2, bGirderCalc,bOutSideWall, eEnvTypeLsd, dDistShearTop);
			break;
		}

		str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);

		strOpt = _T("");
	}
	else if(nCol==7 && b3D)      // ������� 3D SHELL ���ܷ�
	{
		switch(nRow) 
		{
		case 4:
			dValue = pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_BOTTOM, COMBO_ULTIMATE, ELE_SHEAR_2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
			break;
		case 3:
			dValue = pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_BOTTOM, COMBO_ULTIMATE, ELE_SHEAR_2, bGirderCalc,!bOutSideWall, eEnvTypeLsd);
			break;
		case 2:
			dValue = pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_CENTER, COMBO_ULTIMATE, ELE_SHEAR_2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
			break;
		case 1:
			dValue = pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_CENTER, COMBO_ULTIMATE, ELE_SHEAR_2, bGirderCalc,!bOutSideWall, eEnvTypeLsd);
			break;
		case 0:
			dValue = pBri->GetOutValueWallShell(nJijumIndex, CHECK_POS_TOP, COMBO_ULTIMATE, ELE_SHEAR_2, bGirderCalc,bOutSideWall, eEnvTypeLsd);
			break;
		}

		strOpt = _T("");
		if(!b3DLoad) str = _T("-");
		else str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);
	}
	else if((nCol==8 || nCol == 9) && b3D)
	{
		long nPos(0);
		double dDist(0);
		BOOL bPosOutSide(FALSE);
		switch(nRow) 
		{
		case 4:
			nPos = CHECK_POS_BOTTOM;
			bPosOutSide = bOutSideWall;
			dDist = nCol == 8 ? 0 : pBri->GetDistCheckShearWall(nJijumIndex, CHECK_POS_TOP, NULL);
			break;
		case 3:
			nPos = CHECK_POS_BOTTOM;
			bPosOutSide = !bOutSideWall;
			dDist = nCol == 8 ? 0 : pBri->GetDistCheckShearWall(nJijumIndex, CHECK_POS_TOP, NULL);
			break;
		case 2:
			nPos = CHECK_POS_CENTER;
			bPosOutSide = bOutSideWall;
			break;
		case 1:
			nPos = CHECK_POS_CENTER;
			bPosOutSide = !bOutSideWall; 
			break;
		case 0:
			nPos = CHECK_POS_TOP;
			bPosOutSide = bOutSideWall;
			dDist = nCol == 8 ? 0 : pBri->GetDistCheckShearWall(nJijumIndex, CHECK_POS_BOTTOM, NULL);
			break;
		}

		long nTypeForce = nCol == 8 ? ELE_MOMENT2 : ELE_SHEAR_2;
		double dValue1 = pBri->GetOutValueWall(nJijumIndex, nPos, COMBO_ULTIMATE, nTypeForce, bGirderCalc,bPosOutSide, eEnvTypeLsd, dDist);
		double dValue2 = b3DLoad ? pBri->GetOutValueWallShell(nJijumIndex, nPos, COMBO_ULTIMATE, nTypeForce, bGirderCalc,bPosOutSide, eEnvTypeLsd) : 0;
		dValue = fabs(dValue1) > fabs(dValue2) ? dValue1 : dValue2;

		str.Format("$f%d%.3f", (bLsd && b3D) ? 1 : 3, dValue);
	}
	else
	{
		str = "";
		strOpt = _T("");
	}

	return str;
}

CString COutDesignBridge::GetStringSectionForceTableAtWallEQ( CRcBridgeRebar *pBri, ETypeEnvComboLsd eEnvTypeLsd, long nJijumIndex, long nRow, long nCol, long &nMergRow, CString &strOpt, BOOL bLsd )
{
	CString str(_T(""));
	strOpt = _T("");
	BOOL bGirderCalc = FALSE;
	BOOL bOutSideWall= TRUE; // TRUE-���,FALSE-����
	nMergRow = 0;

	if(nCol==0)
	{
		if(nJijumIndex == 0)
			str = _T("������ü");
		else if(nJijumIndex == pBri->m_nQtyJigan)
			str = _T("������ü");
		else
			str.Format(_T("�߰�����%d"), nJijumIndex);

		if(nRow == 0) nMergRow = 4;
		else nMergRow = 0;
	}
	else if(nCol==1)
	{
		switch(nRow) 
		{
		case 4:
			str = "�ϴ�(���)";
			break;
		case 3:
			str = "�ϴ�(����)";
			break;
		case 2:
			str = "�߾�(���)";
			break;
		case 1:
			str = "�߾�(����)";
			break;
		case 0:
			str = "��ܺ�";
			break;
		}
		strOpt = _T("");
	}
	else if(nCol==2)      // ������ ���Ʈ
	{
		switch(nRow) 
		{
		case 4:
			str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_BOTTOM, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd));
			break;
		case 3:
			str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_BOTTOM, ELE_MOMENT2, bGirderCalc,!bOutSideWall, eEnvTypeLsd));
			break;
		case 2:
			str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_CENTER, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd));
			break;
		case 1:
			str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_CENTER, ELE_MOMENT2, bGirderCalc,!bOutSideWall, eEnvTypeLsd));
			break;
		case 0:
			str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_TOP, ELE_MOMENT2, bGirderCalc,bOutSideWall, eEnvTypeLsd));
			break;
		}
		strOpt = _T("");
	}
	else if(nCol==3)      // ������ ���ܷ�
	{
		double dDistShear = pBri->GetDistCheckShearWall(nJijumIndex, nRow==0? CHECK_POS_TOP : CHECK_POS_BOTTOM, NULL);
		
		switch(nRow) 
		{
		case 4:
			str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_BOTTOM, ELE_SHEAR_2, bGirderCalc,bOutSideWall, eEnvTypeLsd, dDistShear));
			break;
		case 3:
			str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_BOTTOM, ELE_SHEAR_2, bGirderCalc,!bOutSideWall, eEnvTypeLsd, dDistShear));
			break;
		case 2:
			str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_CENTER, ELE_SHEAR_2, bGirderCalc,bOutSideWall, eEnvTypeLsd));
			break;
		case 1:
			str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_CENTER, ELE_SHEAR_2, bGirderCalc,!bOutSideWall, eEnvTypeLsd));
			break;
		case 0:
			str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_TOP, ELE_SHEAR_2, bGirderCalc,bOutSideWall, eEnvTypeLsd, dDistShear));
			break;
		}
		strOpt = _T("");
	}
	else
	{
		str = "";
		strOpt = _T("");
	}

	return str;
}

void COutDesignBridge::MakeTableSectionForceEQ_BOX( CXLFormatText *XLText, CRcBridgeRebar *pBri, BOOL bMoment, ETypeEnvComboLsd eEnvTypeLsd/*=eEnvLimit*/ )
{
	long nCntRow;
	BOOL bGirderCalc = FALSE;
	BOOL bOutSideWall= TRUE; // TRUE-���,FALSE-����

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bKDS17 = pData->m_nTypeEarthQuakeApply == 0 ? FALSE : TRUE;
	BOOL bLsd = pData->IsLsdDesign();
	CString szUnit_CGS = _T("");
	CString str1;

	long nTypeForce = bMoment ? ELE_MOMENT2 : ELE_SHEAR_2;
	CString szUnit = pData->GetStringUnitType(bMoment ? UNIT_CGS_TONFM : UNIT_CGS_TONF);
	
	// table�� ��ü row��
	long nRowUpperSlab = pBri->m_nQtyJigan*2 + 1;
	long nRowLowerSlab = nRowUpperSlab;
	long nCntInWall = pBri->GetCountInWallReal();
	long nCntRowPRF = pBri->m_bExistProtectionRiseFooting ? 2 : 0;
	if(pBri->m_nSelectInWallSectionDesign == 1)
		nCntInWall = 0;
	else
		nCntInWall *= 5;

	long nRowTitle = 2;
	nCntRow = nRowTitle + nRowUpperSlab + nRowLowerSlab + nCntInWall + nCntRowPRF;
	if(pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign == 0)
	{
		nCntRow += 5*2;
	}
	XLText->AddTable(nCntRow-1, 25-1, 2, 9);

	//////////////////////////////////////////////////////////////////////////
	// title
	int nWidthJoin1[] = {9, 16};

	str1 = (bMoment ? _T("�ڸ��Ʈ(") : _T("���ܷ�(")) + szUnit + (bLsd? _T(") / mm") : _T(") / m"));

	char *Title1[] = {"�� ��", str1.GetBuffer(str1.GetLength()-1)};
	long j=0; for(j=0; j<2; j++)
	{
		XLText->AddMergeFormatText(j == 0 ? 1 : 0,nWidthJoin1[j]-1,"$c%s",Title1[j]);
		XLText->AddFormatText("$m+0%d",nWidthJoin1[j]);
	}
	XLText->AddText("$n$h");

	long nWidth = bKDS17 ? 3 : 5;
	XLText->AddMergeFormatText(0,nWidthJoin1[0]-1,"$c%s", _T("�� ��"));
	XLText->AddFormatText("$m+0%d",nWidthJoin1[0]);
	for(long ix=0; ix<eGoalEarthQuakeCount; ++ix)
	{
		if(bKDS17 == FALSE && ix > eGoalEQ_Function) break;

		XLText->AddMergeFormatText(0,nWidth-1,"$c%s", pBri->GetStringGoalEarthQuake(static_cast<EGoalOfEarthQuake>(ix), FALSE));
		XLText->AddFormatText("$m+0%d",nWidth);
	}

	nWidth = bKDS17 ? 4 : 6;
	XLText->AddMergeFormatText(0,nWidth-1,"$c%s", _T("���밪"));
	XLText->AddFormatText("$m+0%d",nWidth);
	XLText->AddText("$n$h");
	//////////////////////////////////////////////////////////////////////////

	long nSizeCol = bKDS17 ? 7 : 5;
	int nWidthJoint1[] = { 3, 6, 3, 3, 3, 3, 4 };
	if(bKDS17 == FALSE)
	{
		nWidthJoint1[2] = 5;
		nWidthJoint1[3] = 5;
		nWidthJoint1[4] = 6;
	}

	CString str = _T("");
	CString strOpt = _T("");
	long nMergeRow = 0;
	for(long i=nRowTitle; i<nCntRow-nCntRowPRF; i++)
	{
		long nSpan = 0;
		long nPosCheck = 0;
		long nJijumIndex = 0;
		BOOL bCrown = FALSE;
		BOOL bCheckCompare = FALSE;

		CString szTitle = _T("");

		nSpan = MAX(0, (i - nRowTitle - 1)/2);
		if(pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign == 0 && i < nRowUpperSlab+nRowLowerSlab+nRowTitle+10) 
			nJijumIndex = (i < nRowUpperSlab+nRowLowerSlab+nRowTitle+5) ? 0 : pBri->GetCountJijum()-1;
		else 
		{
			long nIndexByRow = (pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign == 0) ? i - nRowUpperSlab - nRowLowerSlab - 10 : i - nRowUpperSlab - nRowLowerSlab;
			if(pBri->m_nSelectInWallSectionDesign == 0)
				nJijumIndex = (nIndexByRow-1) / 5 + 1;
			else
				nJijumIndex = nIndexByRow;
		}

		long j=0; for(j=0; j<nSizeCol; j++)
		{
			EGoalOfEarthQuake eGoal = static_cast<EGoalOfEarthQuake>(j-2);
			long nIdxModel = pBri->GetEarthQuakeModelIndex(eGoal);
			
			nMergeRow = 0;
			if(i < nRowUpperSlab+nRowLowerSlab+nRowTitle)			// ������
			{
				BOOL bUpperSlab = (i < nRowUpperSlab + nRowTitle) ? TRUE : FALSE;
				long nIndexSlab = bUpperSlab ? i - nRowTitle : i - nRowTitle - nRowUpperSlab;
				nSpan = (nIndexSlab - 1) / 2;

				if(j==0)
				{
					str = bUpperSlab ? _T("��� ������") : _T("�Ϻ� ������");
					if(i==nRowTitle) 
						nMergeRow = nRowUpperSlab - 1;
					else if(i == nRowTitle + nRowUpperSlab)
						nMergeRow = nRowLowerSlab - 1;
					else 
						nMergeRow = 0;
				}
				else if(j==1)
				{
					if(nIndexSlab == 0) 
						str = "�����ܺ�";
					else if(nIndexSlab == nRowUpperSlab - 1) 
						str = "�����ܺ�";
					else if(nIndexSlab%2 == 0) 
						str.Format("�߰�����%d", nIndexSlab/2);
					else if(nIndexSlab%2 == 1) 
						str.Format("�߾Ӻ�(����%d)", nIndexSlab/2 + 1);
					strOpt = _T("");
				}
				else if(j==nSizeCol-1)		// ���밪
				{
					double dDistL = bMoment ? 0 : pBri->GetDistCheckShearSlab(nSpan, CHECK_POS_LEFT, bUpperSlab, NULL);
					double dDistR = bMoment ? 0 : pBri->GetDistCheckShearSlab(nSpan, CHECK_POS_RIGHT, bUpperSlab, NULL);

					if(nIndexSlab == 0) 
					{
						str.Format("$f3%.3f",pBri->GetOutValueSlabEQ(nSpan, CHECK_POS_LEFT, nTypeForce, bUpperSlab, eEnvTypeLsd, dDistL));
					}
					else if(nIndexSlab == nRowUpperSlab - 1) 
					{
						str.Format("$f3%.3f",pBri->GetOutValueSlabEQ(nSpan, CHECK_POS_RIGHT, nTypeForce, bUpperSlab, eEnvTypeLsd, dDistR));
					}
					else if(nIndexSlab%2 == 0) 
					{
						double dDataR = pBri->GetOutValueSlabEQ(nSpan, CHECK_POS_RIGHT, nTypeForce, bUpperSlab, eEnvTypeLsd, dDistR);
						double dDataL = pBri->GetOutValueSlabEQ(nSpan+1, CHECK_POS_LEFT, nTypeForce, bUpperSlab, eEnvTypeLsd, dDistL);
						str.Format("$f3%.3f", bMoment ? min(dDataL,dDataR) : max(dDataL,dDataR));
					}
					else if(nIndexSlab%2 == 1) 
						str.Format("$f3%.3f",pBri->GetOutValueSlabEQ(nSpan, CHECK_POS_CENTER, nTypeForce, bUpperSlab, eEnvTypeLsd));
					strOpt = _T("");
				}
				else				// ���� ������ �����
				{
					if(pBri->m_bGoalEarthQuake[eGoal])
					{
						double dDistL = bMoment ? 0 : pBri->GetDistCheckShearSlab(nSpan, CHECK_POS_LEFT, bUpperSlab, NULL);
						double dDistR = bMoment ? 0 : pBri->GetDistCheckShearSlab(nSpan, CHECK_POS_RIGHT, bUpperSlab, NULL);

						if(nIndexSlab == 0) 
						{
							str.Format("$f3%.3f",pBri->GetOutValueSlab(nSpan, CHECK_POS_LEFT, COMBO_EARTHQUAKE+nIdxModel, nTypeForce, TRUE, FALSE, bUpperSlab, eEnvTypeLsd, dDistL));
						}
						else if(nIndexSlab == nRowUpperSlab - 1) 
						{
							str.Format("$f3%.3f",pBri->GetOutValueSlab(nSpan, CHECK_POS_RIGHT, COMBO_EARTHQUAKE+nIdxModel, nTypeForce, TRUE, FALSE, bUpperSlab, eEnvTypeLsd, dDistR));
						}
						else if(nIndexSlab%2 == 0) 
						{
							double dDataR = pBri->GetOutValueSlab(nSpan, CHECK_POS_RIGHT, COMBO_EARTHQUAKE+nIdxModel, nTypeForce, TRUE, FALSE, bUpperSlab, eEnvTypeLsd, dDistR);
							double dDataL = pBri->GetOutValueSlab(nSpan+1, CHECK_POS_LEFT, COMBO_EARTHQUAKE+nIdxModel, nTypeForce, TRUE, FALSE, bUpperSlab, eEnvTypeLsd, dDistL);
							str.Format("$f3%.3f", bMoment ? min(dDataL,dDataR) : max(dDataL,dDataR));
						}
						else if(nIndexSlab%2 == 1) 
							str.Format("$f3%.3f",pBri->GetOutValueSlab(nSpan, CHECK_POS_CENTER, COMBO_EARTHQUAKE+nIdxModel, nTypeForce, TRUE, FALSE, bUpperSlab, eEnvTypeLsd));
					}
					else
					{
						str = _T("");
					}
					strOpt = _T("");
				}
			}
			else if(i<nRowUpperSlab+nRowLowerSlab+nRowTitle+10 && (pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign == 0))		// ������ü ������ü
			{
				long nRowIndex = (i-(nRowUpperSlab+nRowLowerSlab+nRowTitle))%5;
				str = GetStringSectionForceTableAtWallEQ_BOX(pBri, eEnvTypeLsd, nJijumIndex, bMoment, nRowIndex, j, nMergeRow, strOpt, bLsd);
			}
			else								// �߰���ü
			{
				strOpt = _T("");

				if(pBri->m_nSelectInWallSectionDesign == 0)
				{
					long nOffset = (pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign == 0)? 10 : 0;
					long nRowIndex = (i-(nRowUpperSlab+nRowLowerSlab+nRowTitle+nOffset))%5;

					str = GetStringSectionForceTableAtWallEQ_BOX(pBri, eEnvTypeLsd, nJijumIndex, bMoment, nRowIndex, j, nMergeRow, strOpt, bLsd);
				}
				else
				{
					if(j==0) 
					{
						str = "�߰���ü";
						long nOffset = (pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign == 0)? 10 : 0;
						if(i==nRowUpperSlab+nRowLowerSlab+nRowTitle+nOffset) nMergeRow = nCntInWall-1;
						else nMergeRow = 0;
					}
					else if(j==1)
						str.Format("�߰�����%d", nJijumIndex);
					else if(j==nSizeCol-1)		// ���밪
						str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_BOTTOM, nTypeForce, bGirderCalc,bOutSideWall, eEnvTypeLsd));
					else
					{
						if(pBri->m_bGoalEarthQuake[eGoal])
							str.Format("$f3%.3f",pBri->GetOutValueWall(nJijumIndex, CHECK_POS_BOTTOM, COMBO_EARTHQUAKE+nIdxModel, nTypeForce, bGirderCalc, bOutSideWall, eEnvTypeLsd));
						else 
							str = _T("");
					}
				}
			}
			XLText->AddMergeFormatText(nMergeRow, nWidthJoint1[j]-1, "$c%s%s", str, strOpt);
			XLText->AddFormatText("$m+0%d", nWidthJoint1[j]);
		}
		XLText->AddText("$n$h");
	}
	//�λ��������
	if(pBri->m_bExistProtectionRiseFooting)
	{
		for(long iSE = iSTT; iSE <= iEND; ++iSE)
		{		
			BOOL bStt = iSE == iSTT;

			for(long j=0; j<nSizeCol; j++)
			{
				EGoalOfEarthQuake eGoal = static_cast<EGoalOfEarthQuake>(j-2);
				long nIdxModel = pBri->GetEarthQuakeModelIndex(eGoal);

				nMergeRow = 0;

				if(j==0 && bStt)
				{
					str = _T("�λ��������");
					nMergeRow = 1;
				}
				else if(j==1)
				{
					str = bStt ? _T("����") : _T("����");
					strOpt = _T("");
				}
				else if(j==nSizeCol-1)		// ���밪
				{
					str.Format("$f3%.3f",pBri->GetOutValuePRFEQ(bStt, nTypeForce, eEnvTypeLsd));
					strOpt = _T("");
				}
				else
				{
					if(pBri->m_bGoalEarthQuake[eGoal])
						str.Format("$f3%.3f",pBri->GetOutValuePRF(bStt, ELE_SHEAR_2, COMBO_EARTHQUAKE+nIdxModel, eEnvTypeLsd));
					else 
						str = _T("");
					strOpt = _T("");
				}			

				XLText->AddMergeFormatText(nMergeRow, nWidthJoint1[j]-1, "$c%s%s", str, strOpt);
				XLText->AddFormatText("$m+0%d", nWidthJoint1[j]);

				XLText->OutXL();
			}
			XLText->AddText("$n$h");
		}
	}	

	double dApplyWidth = bLsd ? frM(m_pCalc->GetSlabWidth(-1)) : m_pCalc->GetSlabWidth(-1);
	XLText->AddFormatText("$h$e * ������ ���� ���� �������� ����Ǹ� ����ܸ���� �ؼ����/����(%s%s)���� �����.$n$h", GetStrDouble(dApplyWidth, bLsd ? 0 : 3, bLsd), bLsd ? _T("mm") : _T("m"));
}

CString COutDesignBridge::GetStringSectionForceTableAtWallEQ_BOX( CRcBridgeRebar *pBri, ETypeEnvComboLsd eEnvTypeLsd, long nJijumIndex, BOOL bMoment, long nRow, long nCol, long &nMergRow, CString &strOpt, BOOL bLsd )
{
	BOOL bKDS17 = pBri->m_pARcBridgeDataStd->m_nTypeEarthQuakeApply == 0 ? FALSE : TRUE;
	long nSizeCol = bKDS17 ? 7 : 5;

	long nTypeForce = bMoment ? ELE_MOMENT2 : ELE_SHEAR_2;

	CString str(_T(""));
	strOpt = _T("");
	BOOL bGirderCalc = FALSE;
	BOOL bOutSideWall= TRUE; // TRUE-���,FALSE-����
	nMergRow = 0;

	EGoalOfEarthQuake eGoal = static_cast<EGoalOfEarthQuake>(nCol-2);
	long nIdxModel = pBri->GetEarthQuakeModelIndex(eGoal);

	if(nCol==0)
	{
		if(nJijumIndex == 0)
			str = _T("������ü");
		else if(nJijumIndex == pBri->m_nQtyJigan)
			str = _T("������ü");
		else
			str.Format(_T("�߰�����%d"), nJijumIndex);

		if(nRow == 0) nMergRow = 4;
		else nMergRow = 0;
	}
	else if(nCol==1)
	{
		switch(nRow) 
		{
		case 4:
			str = "�ϴ�(���)";
			break;
		case 3:
			str = "�ϴ�(����)";
			break;
		case 2:
			str = "�߾�(���)";
			break;
		case 1:
			str = "�߾�(����)";
			break;
		case 0:
			str = "��ܺ�";
			break;
		}
		strOpt = _T("");
	}
	else if(nCol==nSizeCol-1)		// ���밪
	{
		double dDist = bMoment ? 0 : pBri->GetDistCheckShearWall(nJijumIndex, nRow==0? CHECK_POS_TOP : CHECK_POS_BOTTOM, NULL);

		switch(nRow) 
		{
		case 4:
			str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_BOTTOM, nTypeForce, bGirderCalc,bOutSideWall, eEnvTypeLsd, dDist));
			break;
		case 3:
			str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_BOTTOM, nTypeForce, bGirderCalc,!bOutSideWall, eEnvTypeLsd, dDist));
			break;
		case 2:
			str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_CENTER, nTypeForce, bGirderCalc,bOutSideWall, eEnvTypeLsd));
			break;
		case 1:
			str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_CENTER, nTypeForce, bGirderCalc,!bOutSideWall, eEnvTypeLsd));
			break;
		case 0:
			str.Format("$f3%.3f",pBri->GetOutValueWallEQ(nJijumIndex, CHECK_POS_TOP, nTypeForce, bGirderCalc,bOutSideWall, eEnvTypeLsd, dDist));
			break;
		}
		strOpt = _T("");
	}
	else
	{
		if(pBri->m_bGoalEarthQuake[eGoal])
		{
			double dDist = bMoment ? 0 : pBri->GetDistCheckShearWall(nJijumIndex, nRow==0? CHECK_POS_TOP : CHECK_POS_BOTTOM, NULL);

			switch(nRow) 
			{
			case 4:
				str.Format("$f3%.3f",pBri->GetOutValueWall(nJijumIndex, CHECK_POS_BOTTOM, COMBO_EARTHQUAKE+nIdxModel, nTypeForce, bGirderCalc,bOutSideWall, eEnvTypeLsd, dDist));
				break;
			case 3:
				str.Format("$f3%.3f",pBri->GetOutValueWall(nJijumIndex, CHECK_POS_BOTTOM, COMBO_EARTHQUAKE+nIdxModel, nTypeForce, bGirderCalc,!bOutSideWall, eEnvTypeLsd, dDist));
				break;
			case 2:
				str.Format("$f3%.3f",pBri->GetOutValueWall(nJijumIndex, CHECK_POS_CENTER, COMBO_EARTHQUAKE+nIdxModel, nTypeForce, bGirderCalc,bOutSideWall, eEnvTypeLsd));
				break;
			case 1:
				str.Format("$f3%.3f",pBri->GetOutValueWall(nJijumIndex, CHECK_POS_CENTER, COMBO_EARTHQUAKE+nIdxModel, nTypeForce, bGirderCalc,!bOutSideWall, eEnvTypeLsd));
				break;
			case 0:
				str.Format("$f3%.3f",pBri->GetOutValueWall(nJijumIndex, CHECK_POS_TOP, COMBO_EARTHQUAKE+nIdxModel, nTypeForce, bGirderCalc,bOutSideWall, eEnvTypeLsd, dDist));
				break;
			}
		}
		else
			str = _T("");
		
		strOpt = _T("");
	}
	
	return str;
}
