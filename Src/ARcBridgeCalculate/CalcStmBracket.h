#pragma once

class CStmBracket;
class CARcBridgeCalcStm;
class AFX_EXT_CLASS CCalcStmBracket
{
public:
	CCalcStmBracket(CARcBridgeCalcStm* pCalcStm, BOOL bStt);
	~CCalcStmBracket(void);

	CARcBridgeCalcStm *m_pCalcStm;

public:
	void SetPosStt(HGBOOL bStt);
	void MakeStmModelByTemplete(CStmModel *pStmModel, HGBOOL bStressFlow);
	void ResetLoadAtModel(CStmModel *pStmModel);
	
private: 
 	void SetBracketTemplate(CStmBracket *pStmBk);
	void SetBracketLoad(CStmBracket *pStmBk);
	void SetBracketRebar(CStmModel *pStmModel);

	double GetDistBracketH1();
	double GetDistBracketB1();
	double GetDistBracketB2();
	double GetDistBracketB3();

	HGDOUBLE GetDistRebarTie();
	HGDOUBLE GetDistCompressStrut();

 	HGBOOL m_bStt;
	HGDOUBLE m_dLoadBracket[2];
};

