#pragma once

class CStmAbutFooting;
class CARcBridgeCalcStm;
class AFX_EXT_CLASS CCalcStmFoot
{
public:
	CCalcStmFoot(CARcBridgeCalcStm* pCalcStm, BOOL bStt);
	~CCalcStmFoot(void);

	CARcBridgeCalcStm *m_pCalcStm;

public:
	void SetPosFooting(HGBOOL bStt);
	void SetTypeLoadCase(eTypeLoadFoot eTypeLoad);
	void SetTypeLoadCaseLsd(EFootingCheckLsd eTypeLoad);
	void SetEarthQuakeLsd(HGBOOL bEarthQuake);

	void MakeStmModelByTemplete(CStmModel *pStmModel, HGBOOL bStressFlow, ETypeEnvComboLsd eTypeEnv);
	void ResetLoadAtModel(CStmModel *pStmModel, ETypeEnvComboLsd eTypeEnv);
	BOOL IsStt() { return m_bStt; };

private: 
	void SetFootTemplate(CStmAbutFooting *pStmFoot);
	void SetFootLoad(CStmAbutFooting *pStmFoot, ETypeEnvComboLsd eTypeEnv);
	void SetFootSetting(CStmAbutFooting *pStmFoot, CStmModel *pStmModel);
	void SetFootFoundation(CStmAbutFooting *pStmFoot, CStmModel *pStmModel);

	void SetFootRebar(CStmModel *pStmModel);

	HGDOUBLE GetDistRebarTie();
	HGDOUBLE GetDistCompressStrut();
	HGDOUBLE GetDistRebarWall();

	HGINT32 m_bStt;
	eTypeLoadFoot m_eTypeLoad;
	EFootingCheckLsd m_eTypeLoadLsd;
	ETypeEnvComboLsd m_eTypeEnv;

	HGBOOL m_bEarthQuakeLsd;
};

