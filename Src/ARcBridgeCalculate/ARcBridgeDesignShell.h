// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeDesignShell.h: interface for the CARcBridgeDesignShell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGEDESIGNSHELL_H__96C1603E_FF85_431E_8D40_C1413CEFA1A0__INCLUDED_)
#define AFX_ARCBRIDGEDESIGNSHELL_H__96C1603E_FF85_431E_8D40_C1413CEFA1A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CARcBridgeDesignShell  
{
public:
	void MakeMidasData(BOOL bRemoveAllData);
	void MakeSapData(BOOL bStt, BOOL bLeft, BOOL bRemoveAllData);
	CARcBridgeDesignShell();
	virtual ~CARcBridgeDesignShell();

	CARcBridgeCalcStd *m_pStd;

private:
	// 본체 Plate
	void MakeMidasFrameSection(CFEMManage *pFEM);
	void MakeMidasFrameElement(CFEMManage *pFEM);
	void MakeMidasJoint(CFEMManage *pFEM);
	void MakeMidasMaterial(CFEMManage *pFEM);
	void MakeMidasShellSection(CFEMManage *pFEM);
	void MakeMidasShellElement(CFEMManage *pFEM);
	void MakeMidasLoad(CFEMManage *pFEM);
	void MakeMidasLoad_Lsd(CFEMManage *pFEM);
	void MakeMidasCombine(CFEMManage *pFEM);
	void MakeMidasConstraints(CFEMManage *pFEM);
	BOOL IsPileJoint(CDPoint xyJoint);
	void SetSpringPilePosForExtLine(CFEMManage *pFEM, long nSttJointIndex);

	void MakeSapDataLoadDead_Lsd( CFEMManage *pFEM, long &nLoadCaseIndex );
	void MakeSapDataLoadHoriSoilPress_Lsd( CFEMManage *pFEM, long &nLoadCaseIndex );
	void MakeSapDataLoadWaterPress_Lsd( CFEMManage *pFEM, long &nLoadCaseIndex );
	void MakeSapDataLoadWaterPress_Usd( CFEMManage *pFEM, long &nLoadCaseIndex );
	void MakeSapDataLoadTemperature( CFEMManage *pFEM, long &nLoadCaseIndex );
	void MakeSapDataLoadSubsid( CFEMManage *pFEM, long &nLoadCaseIndex );
	void MakeSapDataLoadLiveFootWay( CFEMManage *pFEM, long &nLoadCaseIndex );
	void MakeSapDataLoadLiveLoadAll( CFEMManage *pFEM, long &nLoadCaseIndex );
	void MakeSapDataLoadLiveAlways( CFEMManage *pFEM, long &nLoadCaseIndex );

	// 날개벽 Plate
	BOOL m_bInitialMesh;
	CVectorArray m_vArrWing;
	CLongArray m_lArrFix;
	CIntFourArray m_EleArray;
	CDPoint m_xyThick;
	CDPoint m_xyWingUpperStt;
	CDPoint m_xyWingUpperEnd;
	CDPoint m_xyWingFootStt;
	CDPoint m_xyWingFootEnd;
	void MakeInitialMesh(CTwinVector tvLower, CTwinVector tvUpper, CDPointArray &xyArrInit, CDPoint xyBracket, CDPoint xyNoriEnd, CIntThreeArray &EleArray, BOOL bTriangle, BOOL bStt, BOOL bLeft);
	void MakeMeshWingWall(BOOL bStt, BOOL bLeft);
	void MakeSapJoint(CFEMManage *pFEM, BOOL bStt, BOOL bLeft);
	void MakeSapMaterial(CFEMManage *pFEM, BOOL bStt, BOOL bLeft);
	void MakeSapShellSection(CFEMManage *pFEM, BOOL bStt, BOOL bLeft);
	void MakeSapShellElement(CFEMManage *pFEM, BOOL bStt, BOOL bLeft);
	void MakeSapLoad(CFEMManage *pFEM, BOOL bStt, BOOL bLeft);
	void MakeSapCombine(CFEMManage *pFEM, BOOL bStt, BOOL bLeft);
	void MakeSapConstraints(CFEMManage *pFEM, BOOL bStt, BOOL bLeft);
};

#endif // !defined(AFX_ARCBRIDGEDESIGNSHELL_H__96C1603E_FF85_431E_8D40_C1413CEFA1A0__INCLUDED_)
