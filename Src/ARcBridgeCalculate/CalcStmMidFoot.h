#pragma once

class CStmPierFooting;
class CARcBridgeCalcStm;
class AFX_EXT_CLASS CCalcStmMidFoot
{
public:
	CCalcStmMidFoot(CARcBridgeCalcStm* pCalcStm);
	~CCalcStmMidFoot(void);

	CARcBridgeCalcStm *m_pCalcStm;

public:
	void SetPosFooting(HGINT32 nJijum);
	void SetTypeLoadCase(eTypeLoadFoot eTypeLoad);
	void SetTypeLoadCaseLsd(EFootingCheckLsd eTypeLoad);
	void SetEarthQuakeLsd(HGBOOL bEarthQuake);
	void MakeStmModelByTemplete(CStmModel *pStmModel, HGBOOL bStressFlow, ETypeEnvComboLsd eTypeEnv);
	void ResetLoadAtModel(CStmModel *pStmModel, ETypeEnvComboLsd eTypeEnv);
	HGINT32 GetJijum(){ return m_nJijum; };

private: 
	void SetMidFootTemplate(CStmPierFooting *pStmMidFoot);
	void SetMidFootLoad(CStmPierFooting *pStmMidFoot, ETypeEnvComboLsd eTypeEnv);
	void SetMidFootSetting(CStmPierFooting *pStmFoot, CStmModel *pStmModel);
	void SetMidFootFoundation(CStmPierFooting *pStmFoot, CStmModel *pStmModel);

	void SetMidFootRebar(CStmModel *pStmModel);

	HGDOUBLE GetDistRebarTie();
	HGDOUBLE GetDistCompressStrut();

	HGINT32 m_nJijum;
	eTypeLoadFoot m_eTypeLoad;
	EFootingCheckLsd m_eTypeLoadLsd;
	ETypeEnvComboLsd m_eTypeEnv;

	HGBOOL m_bEarthQuakeLsd;
};

