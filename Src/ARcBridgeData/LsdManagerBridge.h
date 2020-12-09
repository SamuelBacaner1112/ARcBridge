#pragma once

class AFX_EXT_CLASS CLsdManagerBridge : public CLsdManager
{
public:
	CLsdManagerBridge(CRcBridgeRebar *pBri, BOOL bForCaltilever=FALSE);
	~CLsdManagerBridge(void);

	CRcBridgeRebar *m_pBri;
	BOOL m_bForCaltilever;

protected:
	virtual void InitEnum();

public:
	void SetDataDefaultSelectLoad();
	void SetDataDefaultLimitCombineSet(ELSDLoadGroupKind Type, ELSDLoadLoadingDir Dir, BOOL bApplyWater);
	virtual BOOL IsValidColumn(ELSDLoadGroupKind GroupKind, ELSDLoadCaseDir Dir, ELSDLoadCaseKind Case, BOOL bCheckUse=FALSE);
	BOOL IsValidColumn(ELSDCombKind eKind, ELSDLoadCaseDir Dir, ELSDLoadCaseKind Case);

	CString GetStringSelectLoadLsd(ELSDLoadSelectKind eSelectKind, BOOL bMark=FALSE);
	CString GetStringLoadCaseLsd(ELSDLoadCaseKind eLoadCaseKind, BOOL bMark=FALSE);
	void SetLoadDirOption();
	BOOL IsSelectedLoadDesign(ELSDLoadSelectKind eKind);
	BOOL IsUsedLoadCaseAtComboination(ELSDLoadGroupKind eComboKind, ELSDLoadCaseKind Case);

	BOOL IsSafeValidLoad(ELSDLoadCaseKind Case);
	ELSDLoadSelectKind GetLoadSelectKindFromLoadCaseKind(ELSDLoadCaseKind casekind);

	long GetindexLoadModifiedFactor(long ixPart);
	double GetValueLoadModifiedFactor(ELSDCombKind ekind, long ixPart, CLsdLoadCase *pLsdLoadCase=NULL);
	double GetLoadFactor(long  nTypeCombo, CString sCombo, ELSDLoadCaseKind Case);

	void GetStringArrayLsdLoadCaseKindForUserLoad(CStringArray &sArr);
	long GetIndexLsdLoadCaseKindForUserLoad(CString sLsdLoadCase);

	CLsdLoadCase *GetLsdLoadCaseWingWallOrAps(HGDOUBLE &dEta, CString &strName, HGBOOL bUse, HGBOOL bJudge, BOOL bWingWall, BOOL bStt);

	double GetEtaValue(/*EPartOfBridge*/long nPart, CString szComboFirst);

private:
	void InitEnumForBridge();
	void InitEnumForCantilever();
	void SetDataDefaultSelectLoadForBridge();
	void SetDataDefaultSelectLoadForCantilever();
};

