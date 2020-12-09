// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeMeshCalc.h: interface for the CARcBridgeMeshCalc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGEMESHCALC_H__AFC9C2B4_2B75_4866_ACAD_81E9B2ECB275__INCLUDED_)
#define AFX_ARCBRIDGEMESHCALC_H__AFC9C2B4_2B75_4866_ACAD_81E9B2ECB275__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MIN_LEN_SHELL	5

#include "MeshFace.h"
class AFX_EXT_CLASS CARcBridgeMeshCalc  
{
public:
	CARcBridgeMeshCalc();
	virtual ~CARcBridgeMeshCalc();

	CARcBridgeCalcStd *m_pStd;

public:
	BOOL IsShoePoint(long nIndexJoint, long &nJijum, long &nShoe, long &nVerPos);
	long GetCountHDanPos();
	long GetJointJijumFixStt(long nJijum);
	long GetJointJijumFixEnd(long nJijum);
	void GetPointArrayLaneGroupStt(long nGroup, CDoubleArray &dArrPoint);
	long GetCountLane(long nGroup);
	long GetCountLaneForRailLoad(long nGroup, long &nStt, long&nEnd);
	double GetWidthLane(long nGroup);
	double GetWidthLaneForRailLoad(long nGroup);
//	double GetAngleBridgeDirPos(long nIndex);
	long GetSizeLaneGroup();
	long GetPosLaneGroupStt(long nLaneGroup);
	HGDOUBLE GetHeightWall(long nJijum);

	BOOL IsWallFixPoint(long nJijum, long nIndexJoint);
	BOOL IsArchRibFixPoint(long nIndexJoint);
	CTypedPtrArray <CObArray, CMeshFace*> m_pListMeshSlab[2];
	CTypedPtrArray <CObArray, CMeshFace*> m_pListMeshWall[MAX_JIJUM_COUNT_RC];
	CTypedPtrArray <CObArray, CMeshFace*> m_pListMeshArchRib[MAX_JIJUM_COUNT_RC];
	CDoubleArray m_dArrStation;
	CDoubleArray m_dArrHDanPos;
	CDoubleArray m_dArrStation_Archrib;
	CDoubleArray m_dArrHDanPos_Archrib;
	CDoubleArray m_dArrThick_Archrib;
	CDoubleArray m_dArrHDanPosGagak[2][2];
	CDoubleArray m_dArrStation_Pf[2];	// �λ���� ���� ���� ����

	CVectorArray m_vArrSlab[2];
	CVectorArray m_vArrArchRib;
	CVectorArray m_vArrWall[MAX_JIJUM_COUNT_RC];
	CVectorArray m_vArrSlabvPoint_real;

	long m_nIdxJijumStation[MAX_JIJUM_COUNT_RC];		// ������ġ�� �����̼� Index
	long m_nIdxJijumShoePos[2][MAX_JIJUM_COUNT_RC];	// ��ħ��ġ�� �����̼� Index
	CDoubleArray m_dArrPointJijum[2][MAX_JIJUM_COUNT_RC];	// ������ġ�� ��ǥ Index
	CDoubleArray m_dArrPointJijumArch[MAX_JIJUM_COUNT_RC];	// ������ġ�� ��ġ����� ��ǥ Index
	CDoubleArray m_dArrHDanType;
	CDoubleArray m_dArrHDanIndex;
	CDoubleArray m_dArrPointWallFix[MAX_JIJUM_COUNT_RC];	// ��ü�ϴ��� RESTRAINT ��ǥ Index
	CDoubleArray m_dArrJointColumnUpper[MAX_JIJUM_COUNT_RC];
	CDoubleArray m_dArrJointColumnLower[MAX_JIJUM_COUNT_RC];
	CDoubleArray m_dArrJointShoeoPos[2][MAX_JIJUM_COUNT_RC];
	CDoubleArray m_dArrPointArchRibFix[2];	// ��ġ���� ���������� RESTRAINT ��ǥ Index
	CDoubleArray m_dArrHDanType_Archrib;
	CDoubleArray m_dArrHDanIndex_Archrib;

	// ��ġ����� ������ ���ձ����� Constraint�Ǵ� ������ Index����
	long m_nIndexStaConstraintStt;
	long m_nIndexStaConstraintEnd;

	CDoubleArray m_dArrConstraintJointIdx_Slab;
	CDoubleArray m_dArrConstraintJointIdx_Archrib;
	// ���� �κ� ������ ���� �����̼� Index
	long m_nIdxGagakStation[2][2];
	void CalculateAll();
	// ������

private:
	BOOL IsSpanCheckStt(double dStation1, double dStation2);
	BOOL IsSpanCheckEnd(double dStation1, double dStation2);
	long GetNumberSlabSpan(double dStation);
	BOOL IsExistValueInArray(CDoubleArray dArr, double dValue);
	BOOL IsShoePos(double dHPos, long nJijum, BOOL bSttShoe);
	CDPoint m_xyCenter;
	void MakeMeshWall();
	void MakeMeshUpperSlab();
	void MakeMeshLowerSlab();
	void MakeMeshSlab(long nIndexFace, long nSumJointIdx, long nSizeBeforeJoint, const CVectorArray &vArrSlab, CTypedPtrArray <CObArray, CMeshFace*> &arrListMeshSlab);
	void MakeMeshSlabCoord(HGBOOL bUpper);
	void MakeMeshArchRib();
	void MakeStationAndHDanPos();
	void MakeStationAndHDanPosTypeArch();
	void MakeStationforPF();
	HGDOUBLE AddStationHDan(HGBOOL bArch, HGDOUBLE *pDW=NULL);
	void AddHDanInfo(HGDOUBLE dStation);
	void AddStationForGagak();
	void AddHDanPosForGagak();
	void AddShoeAndColPos();
	double GetStationGagakStt(BOOL bStt, BOOL bLeft);
	double GetStationGagakPos(BOOL bStt, BOOL bLeft, double dDist);
	long GetArrayHDanPosByStationIndex(long nIdxSta, CDoubleArray &dArr, CDoubleArray &dArrType, CDoubleArray &dArrIndex);
	HGDOUBLE GetWidthLane(long nGroup, HGBOOL bRail);
	HGBOOL GetDistHDan(long nGroup, HGBOOL bRail, double &dDistStt, double &dDistEnd);

	// Ȯ�� ���� �Լ�
	long GetArrayHDanPosForExtLine(long nIdxSta, CDoubleArray &dArr);
	// ���� ����
	void InsertHDanGagakByStation(long nIdxSta);
	long InsertStation(double dSta, BOOL bResetIndex=FALSE);
	void InsertHDanPosForExtLine(long nIdxSta, double dDistPos, long nIdxInsertPos);
	double GetDistHDanByStationForGagak(BOOL bStt, BOOL bLeft, long nIdxSta);
	long GetNumberJoint(long nCntSumPrev, long nCntCur, long nCntNext, long nPos, long nIdxSta, BOOL bLeft, BOOL bUpper);
	// PosIndex�� �����꿡�� ��ġ����� �ٲ��ִ��Լ� �߰�...
	long GetConvertMatchIndexSlabToArchRib(long nIdxSlab);
	long GetIdxArchRibThick(double dXLeft, double dXRight);
	long GetNumberArchRibSpan(double dStation);
	BOOL IsArchRibSpanCheckStt(double dStation1, double dStation2);
	BOOL IsArchRibSpanCheckEnd(double dStation1, double dStation2);
};

#endif // !defined(AFX_ARCBRIDGEMESHCALC_H__AFC9C2B4_2B75_4866_ACAD_81E9B2ECB275__INCLUDED_)
