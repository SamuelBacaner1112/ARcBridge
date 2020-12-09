#pragma once

//////////////////////////////////////////////////////////////////////////
// �ۿ����� ������ Ŭ����
//////////////////////////////////////////////////////////////////////////
const int BANGUM					= 0;
const int NANGAN					= 1;

const int LEFT_CANTILEVER_FLOOR			= 0;	// ����ĵƿ����(�������κ��� ���� ���-����Ϻ����˺�)
const int RIGHT_CANTILEVER_FLOOR		= 1;	// ����ĵƿ����(�������κ��� ���� ���-����Ϻ����˺�)

const int USERLOAD_SIZE				= 2;

// ������, ����
struct _USER_LOAD
{
	HGBOOL m_bExist;
	double m_dWeight;
	double m_dHeight;
	double m_dDist;
	BOOL m_bDw;		// LSD �������ϰ������ - ������(�¿�)
};

struct _BODO	// ��������
{
	BOOL m_bExist;
	double m_dWeightStt;
	double m_dWeightEnd;
	double m_dHeight;
};	

class AFX_EXT_CLASS CWorkingLoad
{
public:
	CWorkingLoad(void);
	~CWorkingLoad(void);

	void Serialize(CArchive &ar);
	CWorkingLoad& operator=(const CWorkingLoad &Obj);

private:
	_USER_LOAD m_UserLoad[USERLOAD_SIZE];		// [������,����]
	_BODO m_BodoLoad;	

	// ��������
	BOOL m_bWalkLoad;
	long m_nWorkLoadType;				// �ְŴ� �ؼ��� �������� ���� ���	- 0: �������� Ÿ�� , 1: �̵����� Ÿ��

	// 	double	m_dCo_H;
	// 
	double	m_dHoriF;
	double	m_dVerF;
	long	m_nHorType;					// �ٴ��� ���� ������� -> "�������� �������" : ����� ����Ÿ�� 0:�Ϲݵ���, 1:���ɵ���, 2:������Է�

public:
 	_USER_LOAD &GetUserLoad(long nType);
 	_BODO &GetBodoWeight();
	BOOL &GetWalkLoad();
	long &GetWorkLoadType();
	double &GetHoriF();
	double &GetVertF();
	long &GetHoriType();
};

//�Ѱ���� ������߰��	//ARoad-4553
const static double _dImpactLoad[] = {0.7, 0.25, 0.15};


class AFX_EXT_CLASS CCheckCantileverData
{
public:
	CCheckCantileverData(void);
	~CCheckCantileverData(void);

	void Serialize(CArchive& ar);
	CCheckCantileverData& operator=(const CCheckCantileverData &Obj);

public:
	CWorkingLoad *GetWorkingLoadData(HGBOOL bLeft);
	
	//////////////////////////////////////////////////////////////////////////
	// ���輱�û���
	//////////////////////////////////////////////////////////////////////////
	double	m_dCenter_R[3];					//����ݰ� [��][��][�߾�]
	double	m_dCo_V;						//����ӵ�

	BOOL m_bCentMugun;										// �ߺд� ������ũ��Ʈ
	BOOL m_bMugun;			// ������ũ��Ʈ
	BOOL m_bFloorTank;
	BOOL m_bFloorTrailer;

	double	m_dWindStrength;		// ǳ���� ����
	BOOL m_bWind;		// m_bWind �� ���� ( ö���� / ���α� ����X )
	BOOL m_bFloowWind;	// �ٴ��� ǳ���� ����

	// ARoad�� CBaseDesignCtl Ŭ������ ������������.
	long	m_nSlabCentDesignMethod;					// �ٴ��� �߾Ӻ� ö�� �����

	long	m_nCent_HeightType;		// �ۿ���ġ Ÿ�� ( 0:HL����, 1:LS����, 2:EL����, 3:���������)

	// ��ũ��Ʈ �Ǻ� �β�
	double	m_dCoverUp;
	double	m_dCoverDn;
	double	m_dCoverSd;

private:
	CWorkingLoad m_WorkingLoadData[2];		// [��,��] - �ۿ�����
};