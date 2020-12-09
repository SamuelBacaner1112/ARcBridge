#pragma once


#define SUPPORT_REBAR_AREA_FACTOR	0.75
#define SUPPORT_REBAR_CTC_FACTOR	3.0
#define SUPPORT_REBAR_CTC_MAX		450.0
#define DEFELECTION_DIV_FACTOR_CAN	10.0
#define DEFELECTION_DIV_FACTOR_MID	28.0

// ����
struct stCantileverLoadUnit
{
	stCantileverLoadUnit()
	{
		sName = _T("");
		m_sDescription = _T("");

		m_dForceVert = 0;
		m_dForceHori = 0;
		m_dFactor = 0;
	}

	stCantileverLoadUnit(stCantileverLoadUnit *pLoad)
	{
		*this = *pLoad;
	}

	~stCantileverLoadUnit()
	{

	}

	double GetMoment(BOOL bFactor) const
	{
		double dMoment = m_dForceVert * m_xyLoadPosition.x + m_dForceHori * m_xyLoadPosition.y;

		if(bFactor)
		{
			return dMoment * m_dFactor;
		}
		else
		{
			return dMoment;
		}
	}

	ELSDLoadCaseKind GetLoadCaseKind() const
	{
		return m_eLoadCaseKind;
	}

	CString sName;
	CString m_sDescription;

	CDPoint m_xyLoadPosition;
	double m_dForceVert;
	double m_dForceHori;
	double m_dFactor;

	ELSDLoadCaseKind m_eLoadCaseKind;
};

// �޺�
struct stCantileverCombo
{
	stCantileverCombo()
	{
		m_sName = _T("");
		m_dMoment = 0;
		m_eStatus = eLSD_COMBINATION_LIMIT;
		m_dEta = 0.;
		m_nIndexCombo = -1;
	}

	~stCantileverCombo()
	{
		ClearVectorPtr(m_vLoad);
	}

	void AddLoad(stCantileverLoadUnit *pLoad)
	{
		m_vLoad.push_back(pLoad);
	}

	void Calculate()
	{
		double dMo = 0, dEta = 0;

		m_dMoment = 0;

		long nSize = m_vLoad.size();
		for (long nIdx =0; nIdx < nSize; nIdx++)
		{
			stCantileverLoadUnit *pLoad = m_vLoad[nIdx];
			if(pLoad == NULL) continue;

			dEta = m_dEta;
			dMo  = pLoad->GetMoment(TRUE) * dEta;

			m_dMoment += dMo;		
		}
	}

	CString m_sName;
	CString m_strCombo;
	double m_dEta;

	vector<stCantileverLoadUnit*> m_vLoad;
	double m_dMoment;

	ELSDCombKind m_eComboKind;	// �������� 13��
	long m_nIndexCombo;	// m_eComboKind������ Index

	ELSDLoadGroupKind m_eStatus; //�Ѱ���� ����
};

class AFX_EXT_CLASS CCantileverCheckPosition
{
public:
	CCantileverCheckPosition(void);
	~CCantileverCheckPosition(void);

	CLongArray m_lArrStatus;	// m_vConc�� match

public:
	CString GetName() const;
	void SetName(CString strName);
	double GetMoment(ELSDLoadGroupKind eGroup) const;
	void AddLoad(stCantileverLoadUnit *pLoad );
	void AddComboData(stCantileverCombo *pCombo, ELSDLoadGroupKind eGroupKind);

	void ClearConcData();
	void AddConcData(CConcBasic *pConc);
	CConcBasic *GetConcData(long nIndex);
	long GetSizeConcData();

	vector<stCantileverLoadUnit*> GetLoads();
	BOOL IsLoad( ELSDLoadCaseKind ePos );
	stCantileverLoadUnit* GetLoad( CString strName );

	long GetSizeCombo(ELSDLoadGroupKind eStatus);
	long GetSizeCombo();
	stCantileverCombo *GetCombo(ELSDLoadGroupKind eGroupKind, long nIdex);

	void SetMomentByGroup(CLsdManager *pLsdManager);

	void CalculateSlabThick(double dThick);
	double GetThickSlab();
	double GetThickSlabRequire(BOOL bMinThick);
	BOOL IsOkMinThick(BOOL bMinThick);
	//
	void CalculateSupportRebar();
	double GetAsReqSupport();
	double GetAsUseSupport();
	double GetCtcUseSupport();
	double GetCtcMaxSupport();
	BOOL IsOkSupportAs();
	BOOL IsOkSupportCtc();
	//
	void CalculateDeflection(double dSpanLength);
	double GettMinDivide();
	double GettMinSpanLength();
	double GettMinDeflection();
	BOOL IsNeedDeflectionCheck();
	BOOL IsSameLoadCase();
	double GetSumMomentLoadCase(ELSDLoadCaseKind eLoadCase);
	CString GetStringRebarCheck(ELSDLoadGroupKind eGroupKind) const;

private:
	CString m_sName; //��Ī

	double m_dMoment[eLSD_COMBINATION_COUNT]; // �Ѱ���º� ���Ʈ
	CString m_sRebarCheck[eLSD_COMBINATION_COUNT];

	vector<CConcBasic*> m_vConc; // �Ѱ���º� CConcBasic (������ �ִ� ���¸� ����)

	vector<stCantileverLoadUnit*> m_vLoad; // ������ġ�� �ش��ϴ� ����
	vector<stCantileverCombo*> m_vCombo[eLSD_COMBINATION_COUNT]; // �Ѱ���º� ��������

	// �β������� ����
	double m_dThickSlab;
	double m_dThickSlabReqMin;
	double m_dThickSlabReqGroove;

	// ���ö�� ����
	double m_dAsReq_Support;
	double m_dAsUse_Support;
	double m_dCtcMax_Support;
	double m_dCtcUse_Support;

	// �ܺ�ö��
	double m_dAsUse_EndSupport;
	double m_dAsUse_EndMain;
	double m_dAsReq_EndMain;

	// ó������
	double m_dtMin_SpanLength;
	double m_dtMin_Deflection;
	double m_dtmin_Div;
};