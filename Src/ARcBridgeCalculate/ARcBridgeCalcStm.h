#pragma once

enum eTypeLoadFoot{ ePmax, eMmax, eMmin, eEarthQuake };

class CARcBridgeCalcStd;
class CCalcStmBracket;
class CCalcStmFoot;
class CCalcStmMidFoot;
class CCalcStmCorner;

class AFX_EXT_CLASS CARcBridgeCalcStm
{
public:
	CARcBridgeCalcStm(CARcBridgeCalcStd* pStd);
	~CARcBridgeCalcStm(void);

	CARcBridgeCalcStd *m_pStd;

	CCalcStmBracket *m_pCalcStmBracket;
	CCalcStmCorner *m_pCalcStmCorner;
	CCalcStmFoot *m_pCalcStmFoot;
	CCalcStmMidFoot *m_pCalcStmMidFoot;

	void SetFootRebar(CStmModel *pStmModel, HGINT32 nJijum);

	void WriteExcelSTM_Bracket_CalcLoad(CStmModel *pStm, CARcBridgeDataStd *pData, CRcBridgeRebar *pBri,CXLFormatText *XLText, long nColPos, HGINT32 iKey);
	void WriteExcelSTM_Footing_CalcLoad(CStmModel *pStm, CARcBridgeDataStd *pData, CRcBridgeRebar *pBri, CXLFormatText *XLText, long nColPos, HGINT32 iKey);
	void WriteExcelSTM_ArcCorner_CalcLoad(CStmModel *pStm, CARcBridgeDataStd *pData, CRcBridgeRebar *pBri, CXLFormatText *XLText, long nColPos, HGINT32 iKey);

	void GetTotalLoadNodeConditionStatics(CStmModel *pStm, std::map<CString,CVector> &mVecLoad, HGBOOL bTopLoad = FALSE);
	void SetConcData(HGBOOL bSlab=FALSE, HGBOOL bWall=FALSE, HGBOOL bFoot=FALSE);

	HGDOUBLE GetLoadMax(CStmModel *pStm, CString strNameLoadConditionStatic, long nSw=1);

	BOOL m_bSimpleModeling;
};

