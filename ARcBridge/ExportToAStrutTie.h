#pragma once

class CARcBridgeDataStd;
class CExportToAStrutTie : public CObject
{
public:
	CExportToAStrutTie(CARcBridgeDataStd *pDataStd);
	~CExportToAStrutTie(void);

	CARcBridgeDataStd *m_pDataStd;

	CStmDataExchange *m_pExSTM;

	void SetExportType(long eTypeDesign, long nARcBridge);
	void ExportStmData();

private:
	long m_nTypeDesign;
	long m_nARcBridge;

	void ExportBracket();
	void ExportArcCorner();
	void ExportFooting(HGBOOL bStt);
	void ExportFootingMid(long nJijum);

	void MakeStmModeling(HGINT32 ixKey);
	void SetExportArea(CStmModel *pStm);
	void SetExportCombo(CStmModel *pStm);
 	
	void CopyStmModelingTruss(CStmModel *pTarget, CStmModel *pSource);
	void CopyStmModelingCombo(CStmModel *pTarget, CStmModel *pSource);
//	void CopyStmModelingMaterial(CStmModel *pTarget, CStmModel *pSource);
	void CopyStmModelingBoundary(CStmModel *pTarget, CStmModel *pSource);

	void SetDataGeneralProperties();

	long m_nIndexCombo;
};