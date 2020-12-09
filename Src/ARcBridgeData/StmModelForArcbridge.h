#pragma once


class CStmModelForArcbridge :
	public CStmModel
{
public:
	CStmModelForArcbridge(void);
	~CStmModelForArcbridge(void);

	// ���� ���� ������
	HGINT32 m_nTypePosRebarTie;
	HGDOUBLE m_dUserPosRebarTie;
	HGINT32 m_nTypePosCompressStrut;
	HGDOUBLE m_dUserPosCompressStrut;

	// �찢�� ���뵥����
	HGINT32 m_nTypeLoadStressFlow;		//�����帧�� �������Ϲ��
	HGINT32 m_nTypeStmModeling;			//�𵨸� Type

	HGINT32 m_nTypeDistCornerRebar;		//���������� ��ġ
	HGDOUBLE m_dUserDistCornerRebar;		//���������� ��ġ ������
	HGINT32 m_nTypeDistHunchRebar;		//���������� ��ġ
	HGDOUBLE m_dUserDistHunchRebar;		//���������� ��ġ ������
	HGINT32 m_ixInnerAngle;		//����β��� ū ��� ���ذ���
	HGINT32 m_nTypeStirrupInnerTension;	//�������� ��Ʈ������
	HGDOUBLE m_dUserStirrupInnerTension;	//�������� ��Ʈ������ ������


	void InitData();
	void Serialize(CArchive &ar);
};

