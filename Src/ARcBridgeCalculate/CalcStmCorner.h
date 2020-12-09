#pragma once

class CStmCorner;
class CARcBridgeCalcStm;
class AFX_EXT_CLASS CCalcStmCorner
{
public:
	CCalcStmCorner(CARcBridgeCalcStm* pCalcStm);
	~CCalcStmCorner(void);

	CARcBridgeCalcStm *m_pCalcStm;

public:
	void SetPosCorner(HGINT32 ixStmKey);
	void MakeStmModelByTemplete(CStmModelForArcbridge *pStmModel, HGBOOL bStressFlow);
	void ResetLoadAtModel(CStmModelForArcbridge *pStmModel);

	// �ִ� ������� ���� public���� �ű�
	void SetCornerTemplate(CStmCorner *pStmCorner);
	void SetCornerLoad(CStmCorner *pStmCorner);

private:
	void SetCornerRebar(CStmModel *pStmModel, HGINT32 nType);

	HGDOUBLE GetDistDanbuRebar();
	HGDOUBLE GetDistRebarTie(HGBOOL bSlab);
	HGDOUBLE GetDistCompressStrut(HGBOOL bSlab);

	HGBOOL m_bStt;
	HGBOOL m_bUpper;
	HGBOOL m_bPlus;
	ETypeEnvComboLsd m_eTypeEnv;
	HGINT32 m_ixStmKey;
};

