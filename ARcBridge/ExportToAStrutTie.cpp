#include "stdafx.h"
#include "ExportToAStrutTie.h"
#include "../Src/ARcBridgeData/ARcBridgeData.h"
#include "../Src/ARcBridgeCalculate/ARcBridgeCalculate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_STATIC_LOAD 100

CExportToAStrutTie::CExportToAStrutTie(CARcBridgeDataStd *pDataStd)
{
	m_pDataStd = pDataStd;
	m_pExSTM = new CStmDataExchange;

	m_nTypeDesign	= 0;
	m_nARcBridge	= 0;
	m_nIndexCombo	= 0;
}

CExportToAStrutTie::~CExportToAStrutTie(void)
{
	delete m_pExSTM;
}

void CExportToAStrutTie::SetExportType(long eTypeDesign, long nARcBridge)
{
	m_nARcBridge = nARcBridge;
	m_nTypeDesign = eTypeDesign;
	m_nIndexCombo = 0;
}

void CExportToAStrutTie::ExportStmData()
{
	CRcBridgeApp *pBri = m_pDataStd->GetBridge(m_nARcBridge);
	if(pBri == NULL) return;

	pBri->MakeStmModelSyncAll(TRUE);

	if(m_nTypeDesign == eStmBracketExport)
	{
 		ExportBracket();
	}
	else if(m_nTypeDesign == eStmCornerExport)
	{
		ExportArcCorner();
	}
	else if(m_nTypeDesign == eStmFootSttExport || m_nTypeDesign == eStmFootEndExport)
	{
		ExportFooting(m_nTypeDesign==eStmFootSttExport ? TRUE : FALSE);
	}
	else if( m_nTypeDesign >= eStmMidFootExport )
	{
		ExportFootingMid(m_nTypeDesign-eStmMidFootExport);
	}
}

 void CExportToAStrutTie::ExportBracket()
{
	for (HGINT32 ixStt=0; ixStt<2; ++ixStt)
	{
		HGINT32 ixKey = ixStt==0 ? STM_MODEL_BRACKET_STT : STM_MODEL_BRACKET_END;
		MakeStmModeling(ixKey);
	}
}

 void CExportToAStrutTie::ExportArcCorner()
 {
	 CRcBridgeApp *pBri = m_pDataStd->GetBridge(m_nARcBridge);
	 if(pBri == NULL) return;

	 HGBOOL bLsd = m_pDataStd->IsLsdDesign();

	 HGINT32 icCase = bLsd ? 3 : 1;
	 for (HGINT32 ixCase=eEnvLimit; ixCase<icCase; ++ixCase)
	 {
		 if( pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ixCase)) == FALSE ) return;

		 HGINT32 ixKeyStt = STM_MODEL_CORNER_STT_UPPER_MINUS;
		 HGINT32 ixKeyEnd = STM_MODEL_CORNER_END_LOWER_PLUS;

		 if( 0 < ixCase )
		 {
			 ixKeyStt = (ixCase==1) ? STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1 : STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2;
			 ixKeyEnd = (ixCase==1) ? STM_MODEL_CORNER_END_LOWER_PLUS_EXT_1 : STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2;
		 }

		 for (HGINT32 ixKey=ixKeyStt; ixKey<=ixKeyEnd; ++ixKey)
		 {
			 MakeStmModeling(ixKey);
		 }
	 }
 }

 void CExportToAStrutTie::ExportFooting(HGBOOL bStt)
 {
	 CRcBridgeApp *pBri = m_pDataStd->GetBridge(m_nARcBridge);
	 if(pBri == NULL) return;

	 HGBOOL bLsd = m_pDataStd->IsLsdDesign();
	 
	 HGINT32 ixKeyStt = bStt ? STM_MODEL_FOOT_STT_MAXP : STM_MODEL_FOOT_END_MAXP;
	 HGINT32 ixKeyEnd = bStt ? STM_MODEL_FOOT_END_MAXP : STM_MODEL_FOOT_END_EARTH;
	 if ( bLsd )
	 {
		 ixKeyStt = bStt ? STM_MODEL_FOOT_STT_MAX_AXIAL : STM_MODEL_FOOT_END_MAX_AXIAL;
		 ixKeyEnd = bStt ? STM_MODEL_FOOT_STT_MIN_HORI_FORCE : STM_MODEL_FOOT_END_MIN_HORI_FORCE;
	 }

	 HGINT32 icCase = (bLsd) ? eEnvExtremeII : 1;
	 for (HGINT32 ixCase=eEnvLimit; ixCase<icCase; ++ixCase)
	 {
		 if( pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ixCase)) == FALSE ) continue;

		 if( 0 < ixCase )
		 {
			 ixKeyStt = ixCase==1 ? (bStt ? STM_MODEL_FOOT_STT_MAX_AXIAL_EXT_1 : STM_MODEL_FOOT_END_MAX_AXIAL_EXT_1) : 
				 (bStt ? STM_MODEL_FOOT_STT_MAX_AXIAL_EXT_2 : STM_MODEL_FOOT_END_MAX_AXIAL_EXT_2);
			 ixKeyEnd = ixCase==1 ? (bStt ? STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_1 : STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_1) : 
				 (bStt ? STM_MODEL_FOOT_END_MAX_AXIAL_EXT_2 : STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2);
		 }

		 for (HGINT32 ixKey=ixKeyStt; ixKey<=ixKeyEnd; ++ixKey)
		 {
			 MakeStmModeling(ixKey);
		 }
	 }
 }

 void CExportToAStrutTie::ExportFootingMid(long nJijum)
 {

	CRcBridgeApp *pBri = m_pDataStd->GetBridge(m_nARcBridge);
	if(pBri == NULL) return;

	HGBOOL bLsd = m_pDataStd->IsLsdDesign();

	HGINT32 nCnt = bLsd ? 15 : 4;

	HGINT32 ixKeyStt = bLsd ? (STM_MODEL_FOOT_MID_MAX_AXIAL + nCnt * nJijum) : (STM_MODEL_MIDFOOT_HEAD + nCnt * nJijum);
	HGINT32 ixKeyEnd = ixKeyStt + nCnt;

	for (HGINT32 ixKey=ixKeyStt; ixKey<ixKeyEnd; ++ixKey)
	{
		MakeStmModeling(ixKey);
	}
 }

 void CExportToAStrutTie::MakeStmModeling(HGINT32 ixKey)
 {
	 CRcBridgeRebar *pBridge = m_pDataStd->GetBridgeRebar(m_nARcBridge);
	 if (!pBridge) return;

	 CRcBridgeApp *pBri = m_pDataStd->GetBridge(m_nARcBridge);
	 if(pBri == NULL) return;

	 CStmModel *pStm = pBri->GetStmModelByKey(ixKey, TRUE);
	 if( pStm == NULL ) return;

	 HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	 SetDataGeneralProperties();
	 m_pExSTM->m_vpTruss.clear();

	 // 해석
	 CARcBridgeCalcStd CalcStd(pBridge);

	 pStm->SetLock(FALSE);
	 pStm->SetStmModifiedFlag(FALSE);

	 if( m_nTypeDesign == eStmBracketExport )
	 {
		 HGBOOL bStt = (ixKey == STM_MODEL_BRACKET_STT)? TRUE : FALSE;

		 CalcStd.m_pStm->m_pCalcStmBracket->SetPosStt(bStt);
		 CalcStd.m_pStm->m_pCalcStmBracket->MakeStmModelByTemplete(pStm, FALSE);
	 }
	else if(m_nTypeDesign == eStmCornerExport)
	{
		CalcStd.m_pStm->m_pCalcStmCorner->SetPosCorner(ixKey);
		CalcStd.m_pStm->m_pCalcStmCorner->MakeStmModelByTemplete(static_cast<CStmModelForArcbridge*>(pStm), FALSE);
	}
	else if(m_nTypeDesign == eStmFootSttExport || m_nTypeDesign == eStmFootEndExport)
	{
		HGBOOL bStt = bLsd ? ((ixKey < STM_MODEL_FOOT_END_MAX_AXIAL) ? TRUE : FALSE) : ((ixKey < STM_MODEL_FOOT_END_MAXP) ? TRUE : FALSE);
		eTypeLoadFoot eTypeLoad = (eTypeLoadFoot)(ixKey - (bStt? STM_MODEL_FOOT_STT_MAXP : STM_MODEL_FOOT_END_MAXP));
		EFootingCheckLsd eTypeLoadLsd = (EFootingCheckLsd)(ixKey - (bStt? STM_MODEL_FOOT_STT_MAX_AXIAL : STM_MODEL_FOOT_END_MAX_AXIAL));
		ETypeEnvComboLsd eTypeEnv =  pBri->GetTypeEnvComboLsdFooting(ixKey, bStt==TRUE ? 0 : pBri->GetCountJijum() );

		CalcStd.m_pStm->m_pCalcStmFoot->SetPosFooting(bStt);

		if( bLsd )
		{
			HGBOOL bEarthQuake = (bStt && STM_MODEL_FOOT_STT_MIN_HORI_FORCE < ixKey) || (!bStt && STM_MODEL_FOOT_END_MIN_HORI_FORCE < ixKey) ? TRUE : FALSE;
			CalcStd.m_pStm->m_pCalcStmFoot->SetTypeLoadCaseLsd(eTypeLoadLsd);
			CalcStd.m_pStm->m_pCalcStmFoot->SetEarthQuakeLsd(bEarthQuake);
		}
		else
			CalcStd.m_pStm->m_pCalcStmFoot->SetTypeLoadCase(eTypeLoad);


		CalcStd.m_pStm->m_pCalcStmFoot->MakeStmModelByTemplete(static_cast<CStmModelForArcbridge*>(pStm), FALSE, eTypeEnv);
	}
	else if( m_nTypeDesign > eStmMidFootExport )
	{
		HGINT32 iInex = bLsd ? (ixKey - STM_MODEL_FOOT_MID_MAX_AXIAL) : (ixKey - STM_MODEL_MIDFOOT_HEAD);
		HGINT32 nJijum = bLsd ? (iInex / 15) : (iInex / STM_MODEL_FOOT_COUNT + 1);

		eTypeLoadFoot eTypeLoad = (eTypeLoadFoot)(iInex%STM_MODEL_FOOT_COUNT);
		ETypeEnvComboLsd eTypeEnv =  pBri->GetTypeEnvComboLsdFooting(ixKey, nJijum );
		EFootingCheckLsd eTypeLoadLsd = (EFootingCheckLsd)(iInex%STM_MODEL_FOOT_LSD_COUNT);

		CalcStd.m_pStm->m_pCalcStmMidFoot->SetPosFooting(nJijum);

		if( bLsd )
		{
			HGBOOL bEarthQuake = (ixKey < STM_MODEL_FOOT_MID_MIN_HORI_FORCE) ? TRUE : FALSE;
			CalcStd.m_pStm->m_pCalcStmMidFoot->SetTypeLoadCaseLsd(eTypeLoadLsd);
			CalcStd.m_pStm->m_pCalcStmMidFoot->SetEarthQuakeLsd(bEarthQuake);
		}
		else
			CalcStd.m_pStm->m_pCalcStmMidFoot->SetTypeLoadCase(eTypeLoad);

		CalcStd.m_pStm->m_pCalcStmMidFoot->MakeStmModelByTemplete(static_cast<CStmModelForArcbridge*>(pStm), FALSE, eTypeEnv);
	}

	 long nSizeCombo = 1;
	 long nSizeLoad = pStm->GetCountLoadConditionStatic();

	 m_pExSTM->m_dMeshSizeStressFlow = pBri->m_dMeshSizeBracket;
	 m_pExSTM->m_icCombination = nSizeCombo;
	 m_pExSTM->m_MatCombo.Resize(nSizeCombo, nSizeLoad);
	 m_pExSTM->m_eBoundaryLoadDirection = eStmUpper;

	 CStmModel *pModel = new CStmModel;
	 CopyStmModelingTruss(pModel, pStm);
	 SetExportArea(pModel);
	 SetExportCombo(pStm);

	 m_pExSTM->m_vpTruss.push_back(pModel);
 }

 // Area가 없는 경우 바운더리를 기준으로 생성
 void CExportToAStrutTie::SetExportArea(CStmModel *pStm)
 {
	 long nCountArea = pStm->GetCountPropertyArea(TRUE);
	 if(nCountArea > 0) return;

	 CStmObjBoundary *pObjBoundary = pStm->GetBoundaryObj(eStmBoundaryOuter);
	 if(pObjBoundary == NULL || pObjBoundary->IsEnable() == FALSE) return;

	 pStm->AddPropertyArea(pObjBoundary->GetBoundaryPos());
 }


 void CExportToAStrutTie::SetExportCombo(CStmModel *pStm)
 {
	 long nIdxLoad = 0;
	 m_nIndexCombo = 0;
	 CString sLoad[MAX_STATIC_LOAD] = {_T(""), };
	 std::map<HGINT32,CStmObjLoadConditionStatic*> mpStaticLoad = pStm->GetLoadConditionStatics();
	 for(auto posLoadStatic = mpStaticLoad.begin(); posLoadStatic != mpStaticLoad.end(); ++posLoadStatic)
	 {
		 CStmObjLoadConditionStatic *pLoadStatic = posLoadStatic->second;
		 sLoad[nIdxLoad++] = pLoadStatic->GetName();
	 }

	 CString sStaticLoad = _T("");
	 std::map<HGINT32,CStmObjLoadConditionCombo*> mpStaticCombo = pStm->GetLoadConditionCombos();
	 for(auto posLoadCombo = mpStaticCombo.begin(); posLoadCombo != mpStaticCombo.end(); ++posLoadCombo)
	 {
		 if(m_pExSTM->m_MatCombo.GetCol() == 0 && m_pExSTM->m_MatCombo.GetRow() == 0) continue;

		 CStmObjLoadConditionCombo *pLoadCombo = posLoadCombo->second;
		 for(long ix = 0; ix < m_pExSTM->m_MatCombo.GetCol(); ix++)
		 {
			 m_pExSTM->m_MatCombo(m_nIndexCombo, ix) = 0.0;
		 }

		 std::map<HGINT32,CStmObjLoadComboFactor*> mpFactor = pLoadCombo->GetComboFactors();
		 for(auto posFactor = mpFactor.begin(); posFactor != mpFactor.end(); ++posFactor)
		 {
			 CStmObjLoadComboFactor *pFactor = posFactor->second;
			 if(pFactor == NULL) continue;
			 sStaticLoad = pFactor->GetNameLoadCondition();
			 for(nIdxLoad=0; nIdxLoad<MAX_STATIC_LOAD; nIdxLoad++)
			 {
				 if(sLoad[nIdxLoad].IsEmpty()) break;

				 if(sStaticLoad == sLoad[nIdxLoad])
				 {
					 m_pExSTM->m_MatCombo(m_nIndexCombo, nIdxLoad) = pFactor->GetScaleFactor();
					 break;
				 }
			 }
		 }
		 m_nIndexCombo++;
	 }
 }

 void CExportToAStrutTie::CopyStmModelingTruss(CStmModel *pTarget, CStmModel *pSource)
 {
	 CRcBridgeApp *pBri = m_pDataStd->GetBridge(m_nARcBridge);
	 if(pBri == NULL) return;

	 if(pTarget == pSource) return;

	 // Boundary
	 CopyStmModelingBoundary(pTarget, pSource);

	 // Node
	 std::map<HGINT32,CStmObjNode*> mpNode = pSource->GetNodes();
	 for(auto posNode = mpNode.begin(); posNode != mpNode.end(); ++posNode)
	 {
		 pTarget->AddNode(pSource->GetNode(posNode->second->GetId()));
		 if(pSource->GetPlane() == eStmPlaneYz)
		 {
			 CStmObjNode *pNode = pTarget->GetNode(posNode->second->GetId());
			 CVector vPos = pNode->GetCoord();
			 vPos.x = vPos.y;
			 vPos.y = 0;
			 pNode->SetCoord(vPos);
		 }
	 }

	 // Element
	 std::map<HGINT32,CStmObjElement*> mpElement = pSource->GetElements();
	 for(auto posEle = mpElement.begin(); posEle != mpElement.end(); ++posEle)
	 {
		 pTarget->AddElement(pSource->GetElement(posEle->second->GetId()));
	 }

	 // Type Define ( Tie Type )
	 CStmDataDefine *pSourceDefine = pSource->GetStmDataDefine();
	 CStmDataDefine *pTargetDefine = pTarget->GetStmDataDefine();
	 *pTargetDefine = *pSourceDefine;

	 // Restraint
	 std::map<HGINT32,CStmObjRestraint*> mpRestraint = pSource->GetRestraints();
	 for(auto posRes = mpRestraint.begin(); posRes != mpRestraint.end(); ++posRes)
	 {
		 pTarget->AddRestraint(pSource->GetRestraint(posRes->second->GetId()));
	 }

	 // Spring
	 std::map<HGINT32,CStmObjSpring*> mpSpring = pSource->GetSprings();
	 for(auto posSpring = mpSpring.begin(); posSpring != mpSpring.end(); ++posSpring)
	 {
		 pTarget->AddSpring(pSource->GetSpring(posSpring->second->GetId()));
	 }

	 // Bearing Plate
	 std::map<HGINT32,CStmObjBearingPlate*> mpBearing = pSource->GetBearingPlates();
	 for(auto posBearing = mpBearing.begin(); posBearing != mpBearing.end(); ++posBearing)
	 {
		 CStmObjBearingPlate *pBearingPlate = posBearing->second;
		 pTarget->AddBearingPlate(pBearingPlate->GetIdNode(), pBearingPlate->GetProperty());
	 }

	 // 	// Material, Area
	 // 	CopyStmModelingMaterial(pTarget, pSource);

	 // Section
	 std::map<HGINT32,CStmObjSection*> mpSection = pSource->GetSections();
	 if(mpSection.size() == mpElement.size())
	 {
		 for(auto posSection = mpSection.begin(); posSection != mpSection.end(); ++posSection)
		 {
			 pTarget->AddSection(posSection->second);
		 }
	 }
	 else
	 {
		 CARcBridgeCalcStd CalcStd(static_cast<CRcBridgeRebar*>(pBri));
		 CalcStd.MakeStmFrameSection(pTarget);
	 }

	 // Staic Loads
	 std::map<HGINT32,CStmObjLoadConditionStatic*> mpStaticLoad = pSource->GetLoadConditionStatics();
	 for(auto posLoadStatic = mpStaticLoad.begin(); posLoadStatic != mpStaticLoad.end(); ++posLoadStatic)
	 {
		 CStmObjLoadConditionStatic *pLoadStaticSource = posLoadStatic->second;
		 CStmObjLoadConditionStatic *pLoadStaticTarget = pTarget->AddLoadConditionStatic(pLoadStaticSource->GetName(), pLoadStaticSource->GetComment(), pLoadStaticSource->GetSelfWeight());

		 std::map<HGINT32,CStmObjLoadNode*> mpLoadNode = pLoadStaticSource->GetLoadNodes();
		 for(auto posLoad = mpLoadNode.begin(); posLoad != mpLoadNode.end(); ++posLoad)
		 {
			 CStmObjLoadNode *pLoadNode = posLoad->second;
			 if(pLoadNode == NULL) continue;

			 CStmObjLoadNode *pLoadNodeTarget = pLoadStaticTarget->AddLoadNode(pLoadNode->GetName(), pLoadNode->GetIdNode(), pLoadNode->GetLoad());
			 if(pSource->GetPlane() == eStmPlaneYz)
			 {
				 CVector vLoad = pLoadNodeTarget->GetLoad();
				 vLoad.x = vLoad.y;
				 vLoad.y = 0;

				 pLoadNodeTarget->SetLoad(vLoad);
			 }
		 }
	 }

	 // Combo
	 CopyStmModelingCombo(pTarget, pSource);
 }

//  void CExportToAStrutTie::CopyStmModelingMaterial(CStmModel *pTarget, CStmModel *pSource)
//  {
// 	 std::map<HGINT32,CStmObjMaterial*> mpMaterial = pSource->GetMaterials();
// 	 for(auto posMaterial = mpMaterial.begin(); posMaterial != mpMaterial.end(); ++posMaterial)
// 	 {
// 		 pTarget->AddMaterial(pSource->GetMaterial(posMaterial->second->GetId()));
// 	 }
// 
// 	 SetExportArea(pTarget);
//  }

void CExportToAStrutTie::CopyStmModelingBoundary(CStmModel *pTarget, CStmModel *pSource)
{
	//Yz일경우 Xz로 변환.
	if(pSource->GetPlane() == eStmPlaneYz)
	{
		 std::vector<CVector> vTargetBoundary;
		 std::map<HGINT32, CStmObjBoundary*> mpBoundarys = pSource->GetBoundarys(eStmBoundaryOuter);
		 std::map<HGINT32,CStmObjBoundary*>::iterator iterpos;
		 for(iterpos = mpBoundarys.begin(); iterpos != mpBoundarys.end(); ++iterpos)
		 {
			 vTargetBoundary.clear();

			 std::vector<CVector> vBoundaryPos = pSource->GetBoundary(eStmBoundaryOuter, iterpos->second->GetId());
			 HGINT32 icBoundary = vBoundaryPos.size();
			 for(HGINT32 ix = 0; ix < icBoundary; ++ix)
			 {
				 CVector vPos = vBoundaryPos[ix];
				 vPos.x = vPos.y;
				 vPos.y = 0;

				 vTargetBoundary.push_back(vPos);
			 }
			 pTarget->SetBoundary(eStmBoundaryOuter, vTargetBoundary);
		 }
	}
	else
	{
		 pTarget->SetBoundary(eStmBoundaryOuter, pSource->GetBoundary(eStmBoundaryOuter));
	}

	std::vector<SStmBoundaryRef> vRef = pSource->GetBoundaryRefs();
	HGINT32 icRef = vRef.size();
	for(HGINT32 ix = 0; ix< icRef; ++ix)
	{
		 SStmBoundaryRef sRef = vRef[ix];
		 if(pSource->GetPlane() == eStmPlaneYz)
		 {
			 sRef.point1.x = sRef.point1.y;
			 sRef.point1.y = 0;
			 sRef.point2.x = sRef.point2.y;
			 sRef.point2.y = 0;
		 }
		 pTarget->AddBoundaryRef(sRef.point1, sRef.point2);
	}
}

void CExportToAStrutTie::CopyStmModelingCombo(CStmModel *pTarget, CStmModel *pSource)
{
	 std::map<HGINT32,CStmObjLoadConditionCombo*> mpStaticCombo = pSource->GetLoadConditionCombos();
	 for(auto posLoadCombo = mpStaticCombo.begin(); posLoadCombo != mpStaticCombo.end(); ++posLoadCombo)
	 {
		 CStmObjLoadConditionCombo *pLoadComboSource = posLoadCombo->second;
		 CStmObjLoadConditionCombo *pLoadComboTarget = pTarget->AddLoadConditionCombo(pLoadComboSource->GetName(), pLoadComboSource->GetType());

		 std::map<HGINT32,CStmObjLoadComboFactor*> mpFactor = pLoadComboSource->GetComboFactors();
		 for(auto posFactor = mpFactor.begin(); posFactor != mpFactor.end(); ++posFactor)
		 {
			 CStmObjLoadComboFactor *pFactor = posFactor->second;
			 if(pFactor == NULL) continue;

			 pLoadComboTarget->AddLoadComboFactor(pFactor->GetNameLoadCondition(), pFactor->GetTypeLoadCondition(), pFactor->GetScaleFactor());
		 }
	 }
}


void CExportToAStrutTie::SetDataGeneralProperties()
{
// 	CAbutApp *pAbut = m_bAbut ? m_pDataStd->GetAbut(m_nAbutPier) : NULL;
// 	CPierApp *pPier = !m_bAbut ? m_pDataStd->GetPier(m_nAbutPier) : NULL;

	CRcBridgeApp *pBri = m_pDataStd->GetBridge(m_nARcBridge);
	if(pBri == NULL) return;

	m_pExSTM->m_szProjectName = m_pDataStd->m_strProjectName;
	m_pExSTM->m_szDesigner = m_pDataStd->m_strEngName;

	CTime time = CTime::GetCurrentTime();
	m_pExSTM->m_szDate.Format(NOTRANS("%04d-%02d-%02d"), time.GetYear(), time.GetMonth(), time.GetDay());

	CString strStm = _T("");
//	CString strPos = _T("");

	if( m_nTypeDesign == eStmBracketExport ) strStm = NOTRANS("Bracket");
	else if(m_nTypeDesign == eStmCornerExport) strStm = NOTRANS("Corner");
	else if(m_nTypeDesign == eStmFootSttExport || m_nTypeDesign == eStmFootEndExport) strStm = NOTRANS("Footing");
	else if(m_nTypeDesign == eStmMidFootExport) strStm = NOTRANS("Footing Mid");

	m_pExSTM->m_szProjectDescription.Format(NOTRANS("%s"), strStm/*, strPos*/);

	m_pExSTM->m_iTypeCode = 0;
	m_pExSTM->m_iTypeUnit = 0;

	for(long ix = 0; ix < STM_MODEL_COUNT; ix++)
	{
		m_pExSTM->m_bCheckAnalysis[ix] = TRUE;
	}

	// General Properties
	CStmDataGeneralProperty *pGeneral = m_pExSTM->m_pGeneral;
	CStmDataConfig *pStmData = pBri->m_pStmSolverInput->GetStmDataConfig();
	*m_pExSTM->m_pConfig = *pStmData;
	
	// Rebar
	pGeneral->ClearRebarInfo();
	CBasicConcInfo *pBasicConc = m_pDataStd->m_pBasicConcInfo;
	CString sHead = GetCharRebarMark(m_pExSTM->m_pConfig->m_dFy, FALSE);
	CString sMark = _T("");
	double dDia[REBAR_WEIGHT_ROW] = {10, 13, 16, 19, 22, 25, 29, 32, 35, 38, 41, 51};
	for(long ix = 0; ix < REBAR_WEIGHT_ROW; ix++)
	{
		sMark.Format(_T("%s%.0f"), sHead, dDia[ix]);
		pGeneral->AddRebarInfo(sMark, pBasicConc->m_RebarWeight[ix][BASIC_WEIGHT_UNIT], 
			pBasicConc->m_RebarWeight[ix][BASIC_AREA_UNIT], 
			pBasicConc->m_RebarWeight[ix][BASIC_DIAMEATER], 
			pBasicConc->m_RebarWeight[ix][BASIC_GIRTH] );
	}

	SStmMaterialProperty *pMatConc = pGeneral->GetConcrete();
	pMatConc->weight = pBasicConc->m_UWeightConcrete;
	pMatConc->mass = pMatConc->weight / 10.;
	pMatConc->poisson = m_pDataStd->m_RatePoasong;
	//
	SStmMaterialProperty *pMatRConc = pGeneral->GetSteelConcrete();
	pMatRConc->weight = pBasicConc->m_UWeightSteelConcrete;
	pMatRConc->mass = pMatConc->weight / 10.;
	pMatRConc->poisson = m_pDataStd->m_RatePoasong;
	//
	SStmMaterialProperty *pMatSteel = pGeneral->GetSteel();
	pMatSteel->weight = pBasicConc->m_UWeightSteel;
	pMatSteel->mass = pMatConc->weight / 10.;
	pMatSteel->poisson = m_pDataStd->m_RatePoasong;
}