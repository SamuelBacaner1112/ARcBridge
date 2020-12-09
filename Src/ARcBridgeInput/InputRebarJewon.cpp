// InputRebarJewon.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TYPE_MAIN				0	// ��ö��
#define TYPE_ECT				1	// ��Ÿ ö��
#define TYPE_COLUMN				2	// ��, ���
#define TYPE_WING				3	// ������
#define TYPE_APS				4	// ���ӽ�����
#define TYPE_PILE_HYUNTA		5	// ����Ÿ�� ����
#define TYPE_GUARDWALL			6	// ��ȣ��
#define TYPE_CENTERGUARD		7	// �ߺд�
#define TYPE_CENTERGUARDHALF	8	// �ߺд�1/2
#define TYPE_HANDRAIL			9	// ��������
#define TYPE_SOUNDPROOF			10	// ������ ����
#define TYPE_RETAININGWALL		11	// �븷�̺�
#define TYPE_FOOTWAYBLOCK		12	// ����+����
#define TYPE_RAILGUARDFENCE		13	// ����
#define TYPE_RAILHOLE			14	// ��ű�
#define TYPE_RAILHOLEFOOTWAY	15	// ��ű�+����
#define TYPE_RAILDETECTGRAVEL	16	// �ڰ�����
#define TYPE_MONUMENT			17	// ������ ��ħ

#define TYPE_COL	13
#define	JOINT_COL	11

#define REBAR_INDEX_COLUMN	21

#define MAKE_NULL if(str=="0") str=_T("")

/////////////////////////////////////////////////////////////////////////////
// CInputRebarJewon dialog


CInputRebarJewon::CInputRebarJewon(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputRebarJewon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputRebarJewon)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bViewLsd		= FALSE;
}


void CInputRebarJewon::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputRebarJewon)
	DDX_Control(pDX, IDC_BUTTON_ADDRATE, m_btnAddRate);
	DDX_Control(pDX, IDC_BUTTON_RECOMMAND_ALL, m_btnRecommandAll);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_comboBridgeNo);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_CHECK_VIEW_LSDINFO, m_checkViewlsd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputRebarJewon, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputRebarJewon)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMAND_ALL, OnButtonRecommandAll)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_BN_CLICKED(IDC_BUTTON_ADDRATE, OnButtonAddrate)
	ON_BN_CLICKED(IDC_BUTTON_ADD_REBAR, OnButtonAddRebar)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_REBAR, OnButtonRemoveRebar)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_BN_CLICKED(IDC_CHECK_VIEW_LSDINFO, OnBnClickedCheckViewLsdinfo)
	ON_BN_CLICKED(IDC_BUTTON_JOINT, OnButtonDetJoint)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputRebarJewon message handlers
void CInputRebarJewon::OnPreInitDialog()
{
	// �����ϸ鼭 pick box�� ũ�⸦ ������
	if(m_pView) 
	{
		CTwinVector tv;
		tv.SetPickBoxSize(m_pView->GetSelectionRectSize()/m_pView->GetRatioWindow());
	}
	
	InitTabCtrl();
	InitComboCtrl();
	SetStringDia();

	SetResize(IDC_TAB, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_RECOMMAND_ALL,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_ADDRATE,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_ADD_REBAR,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_REMOVE_REBAR,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_VIEW_LSDINFO, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_JOINT, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetHyperHelp(m_pIn->GetManualPath());

	m_Grid.InitGrid(m_pIn->m_pARcBridgeDataStd->m_Fy);
	SetGridData();
}

BOOL CInputRebarJewon::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().col == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}
	
	structGridCell *cell = m_Grid.GetCurrentCell();
	if(cell)
	{
	}

	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputRebarJewon::SetGridTitleFirst()
{
	long nCol = 0;
	m_Grid.SetGridDefault(2,4,1,3,TRUE);
	m_Grid.SetCellSize(20,-1);
	m_Grid.AddCell(0,nCol++,"����");
	m_Grid.AddCell(0,nCol++,"");
	m_Grid.AddCell(0,nCol++,"���");
	m_Grid.AddCell(0,nCol++,"��ȣ");
	m_Grid.AddCell(0,nCol++,"����");
	m_Grid.AddCell(0,nCol++,"A");
	m_Grid.AddCell(0,nCol++,"B");
	m_Grid.AddCell(0,nCol++,"C");
	m_Grid.AddCell(0,nCol++,"D");
	m_Grid.AddCell(0,nCol++,"E");
	m_Grid.AddCell(0,nCol++,"R");
	m_Grid.AddCell(0,nCol++,"����");
	m_Grid.AddCell(0,nCol++,"������");
	m_Grid.AddCell(0,nCol++,"��");	
	m_Grid.AddCell(0,nCol++,"����");
	m_Grid.AddCell(0,nCol++,"����");
	m_Grid.AddCell(0,nCol++,"a");
	m_Grid.AddCell(0,nCol++,"b");
	m_Grid.AddCell(0,nCol++,"c");
	m_Grid.AddCell(0,nCol++,"d");
	m_Grid.AddCell(0,nCol++,"��ö��");
	m_Grid.AddCell(0,nCol++,"Index");
	m_Grid.AddCell(0,nCol++,"GW");	//��ȣ���� ������ ����Ǿ���
	m_Grid.SetLastCellSize(-1, 0);

	if(m_checkViewlsd.GetCheck())
	{
		m_Grid.AddCell(0, nCol++, hggettext("�ܸ���"));
		m_Grid.AddCell(0, nCol++, hggettext("����"));
		m_Grid.AddCell(0, nCol++, hggettext("�ѷ�"));

		m_Grid.AddCell(0, nCol++, hggettext("fck"));
		m_Grid.AddCell(0, nCol++, hggettext("fctm"));
		m_Grid.AddCell(0, nCol++, hggettext("fctk"));
		m_Grid.AddCell(0, nCol++, hggettext("fy"));
		m_Grid.AddCell(0, nCol++, hggettext("��c"));
		m_Grid.AddCell(0, nCol++, hggettext("��s"));

		m_Grid.AddCell(0, nCol++, hggettext("ö������"));
		m_Grid.AddCell(0, nCol++, hggettext("s"));		// ����

		m_Grid.AddCell(0, nCol++, hggettext("dc"));
		m_Grid.AddCell(0, nCol++, hggettext("c"));		// Min(s, dc/2)
		m_Grid.AddCell(0, nCol++, hggettext("sB"));	// Ⱦö�ٰ���
		m_Grid.AddCell(0, nCol++, hggettext("dB"));		// Ⱦö������
		m_Grid.AddCell(0, nCol++, hggettext("K"));
		m_Grid.AddCell(0, nCol++, hggettext("fr"));	// ��������
		m_Grid.AddCell(0, nCol++, hggettext("Ro"));		// ��ħ��������
		m_Grid.AddCell(0, nCol++, hggettext("Ras"));	// �ʿ�/���ö�ٷ���
		m_Grid.AddCell(0, nCol++, hggettext("��1"));
		m_Grid.AddCell(0, nCol++, hggettext("��2"));

		m_Grid.AddCell(0, nCol++, hggettext("fbd"));	// �⺻��������
		m_Grid.AddCell(0, nCol++, hggettext("ld"));
		m_Grid.AddCell(0, nCol++, hggettext("��1"));
		m_Grid.AddCell(0, nCol++, hggettext("��2"));
		m_Grid.AddCell(0, nCol++, hggettext("��At"));
		m_Grid.AddCell(0, nCol++, hggettext("��Atmin"));
		m_Grid.AddCell(0, nCol++, hggettext("��3"));
		m_Grid.AddCell(0, nCol++, hggettext("��5"));
		m_Grid.AddCell(0, nCol++, hggettext("��6"));
		m_Grid.AddCell(0, nCol++, hggettext("lo"));		// ������������
		m_Grid.AddCell(0, nCol++, hggettext("lo,min"));	// �ּҼ��� ��������
		m_Grid.AddCell(0, nCol++, hggettext("lo,min"));	// �ּ� ��������
	}
}

/*
#define DEFAULT_REBARJEWON_MAIN		0x00000001
#define DEFAULT_REBARJEWON_ETC		0x00000002
#define DEFAULT_REBARJEWON_COLUMN	0x00000004
#define DEFAULT_REBARJEWON_WINGSL	0x00000008
#define DEFAULT_REBARJEWON_WINGSR	0x00000010
#define DEFAULT_REBARJEWON_WINGEL	0x00000020
#define DEFAULT_REBARJEWON_WINGER	0x00000040
#define DEFAULT_REBARJEWON_APS		0x00000080
#define DEFAULT_REBARJEWON_HYUNTA	0x00000100
#define DEFAULT_REBARJEWON_ETC2		0x00000200
*/

void CInputRebarJewon::SetDataDefault()
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pStd->IsLsdDesign();
	
	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) return;
		pBri->SyncBridge_DesignData();
		int nOriginTab = GetTabCurOriginSel();

		BOOL bVertDir	= pBri->IsVertDir();
		pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, FALSE);

		if(!IsDefaulted(bri, DEFAULT_REBARJEWON_MAIN) || (nOriginTab==0 && IsClickDefaultButton(bri)))		// ��ö��(��ü)
			pBri->SetDataDefaultRebarJewonMainStd();
		if(!IsDefaulted(bri, DEFAULT_REBARJEWON_ETC)|| (nOriginTab==1 && IsClickDefaultButton(bri)))		// ��� �� ���� ö��(��ü)
			pBri->SetDataDefaultRebarJewonEtcStd();
		if(!IsDefaulted(bri, DEFAULT_REBARJEWON_COLUMN) || (nOriginTab==2 && IsClickDefaultButton(bri)))	// ��, ���
			pBri->SetDataDefaultRebarJewonColumnAndGirderStd();
		if(!IsDefaulted(bri, DEFAULT_REBARJEWON_WINGSL) || (nOriginTab==3 && IsClickDefaultButton(bri)))	// ������ ���� ����
			pBri->SetDataDefaultRebarJewonWingWall(TRUE, TRUE);
		if(!IsDefaulted(bri, DEFAULT_REBARJEWON_WINGSR) || (nOriginTab==4 && IsClickDefaultButton(bri)))	// ������ ���� ����
			pBri->SetDataDefaultRebarJewonWingWall(TRUE, FALSE);
		if(!IsDefaulted(bri, DEFAULT_REBARJEWON_WINGEL) || (nOriginTab==5 && IsClickDefaultButton(bri)))	// ������ ���� ����
			pBri->SetDataDefaultRebarJewonWingWall(FALSE, TRUE);
		if(!IsDefaulted(bri, DEFAULT_REBARJEWON_WINGER) || (nOriginTab==6 && IsClickDefaultButton(bri)))	// ������ ���� ����
			pBri->SetDataDefaultRebarJewonWingWall(FALSE, FALSE);
		if(!IsDefaulted(bri, DEFAULT_REBARJEWON_APS) || (nOriginTab==7 && IsClickDefaultButton(bri)))		// ���� ������
		{
			
			CString strHead	= _T("");
			for(long stt = 0; stt < 2; stt++)
			{
				BOOL bStt	= stt == iSTT;
				for(long left = 0; left < 2; left++)
				{
					pBri->SyncBridgeAps(left);

					if(bStt)
					{
						pBri->m_apsStt[left].m_nTypeAddRate = pBri->m_nTypeAddRate;		// 0 : ���溰, 1 : ��ö��,��Ÿö��
						pBri->m_apsStt[left].m_dAddRateLower = pBri->m_dAddRateLower;		// ���� ������
						pBri->m_apsStt[left].m_dAddRateUpper = pBri->m_dAddRateUpper;		// ���� ������
						pBri->m_apsStt[left].m_dAddRateDiaOrg = pBri->m_dAddRateDiaOrg;	// ���溰�ϰ�� ���� ����
						
						pBri->m_apsStt[left].SetDataDefaultRebarJewon();

						for (long ix =0; ix < pBri->m_apsStt[left].m_pArrRebar.GetSize(); ix++)
						{
							CRebar *pRBAps = pBri->m_apsStt[left].m_pArrRebar.GetAt(ix);
							if(pRBAps->m_sDescription.Find(_T("����")) > -1 && bLsd)
							{
								double dFyAps = pBri->GetValueFy(ePartApsStt, TRUE);
								pRBAps->m_Fy = dFyAps;								
							}
						}						
					}
					else
					{
						pBri->m_apsEnd[left].m_nTypeAddRate = pBri->m_nTypeAddRate;		// 0 : ���溰, 1 : ��ö��,��Ÿö��
						pBri->m_apsEnd[left].m_dAddRateLower = pBri->m_dAddRateLower;		// ���� ������
						pBri->m_apsEnd[left].m_dAddRateUpper = pBri->m_dAddRateUpper;		// ���� ������
						pBri->m_apsEnd[left].m_dAddRateDiaOrg = pBri->m_dAddRateDiaOrg;	// ���溰�ϰ�� ���� ����

						pBri->m_apsEnd[left].SetDataDefaultRebarJewon();

						for (long ix =0; ix < pBri->m_apsEnd[left].m_pArrRebar.GetSize(); ix++)
						{
							CRebar *pRBAps = pBri->m_apsEnd[left].m_pArrRebar.GetAt(ix);
							if(pRBAps->m_sDescription.Find(_T("����")) > -1 && bLsd)
							{
								double dFyAps = pBri->GetValueFy(ePartApsEnd, TRUE);
								pRBAps->m_Fy = dFyAps;
							}
						}
					}

					for(long cns = 0; cns < 2; cns++)
					{
						if(bStt)
						{
							pBri->m_cnsStt[left][cns].m_nTypeAddRate = pBri->m_nTypeAddRate;		// 0 : ���溰, 1 : ��ö��,��Ÿö��
							pBri->m_cnsStt[left][cns].m_dAddRateLower = pBri->m_dAddRateLower;		// ���� ������
							pBri->m_cnsStt[left][cns].m_dAddRateUpper = pBri->m_dAddRateUpper;		// ���� ������
							pBri->m_cnsStt[left][cns].m_dAddRateDiaOrg = pBri->m_dAddRateDiaOrg;	// ���溰�ϰ�� ���� ����

							pBri->m_cnsStt[left][cns].SetDataDefaultRebarJewon();

							for (long ix =0; ix < pBri->m_cnsStt[left][cns].m_pArrRebar.GetSize(); ix++)
							{
								CRebar *pRBAps = pBri->m_cnsStt[left][cns].m_pArrRebar.GetAt(ix);
								if(pRBAps->m_sDescription.Find(_T("����")) > -1 && bLsd)
								{
									double dFyAps = pBri->GetValueFy(ePartApsStt, TRUE);
									pRBAps->m_Fy = dFyAps;
								}
							}	
						}
						else
						{
							pBri->m_cnsEnd[left][cns].m_nTypeAddRate = pBri->m_nTypeAddRate;		// 0 : ���溰, 1 : ��ö��,��Ÿö��
							pBri->m_cnsEnd[left][cns].m_dAddRateLower = pBri->m_dAddRateLower;		// ���� ������
							pBri->m_cnsEnd[left][cns].m_dAddRateUpper = pBri->m_dAddRateUpper;		// ���� ������
							pBri->m_cnsEnd[left][cns].m_dAddRateDiaOrg = pBri->m_dAddRateDiaOrg;	// ���溰�ϰ�� ���� ����

							pBri->m_cnsEnd[left][cns].SetDataDefaultRebarJewon();

							for (long ix =0; ix < pBri->m_cnsEnd[left][cns].m_pArrRebar.GetSize(); ix++)
							{
								CRebar *pRBAps = pBri->m_cnsEnd[left][cns].m_pArrRebar.GetAt(ix);
								if(pRBAps->m_sDescription.Find(_T("����")) > -1 && bLsd)
								{
									double dFyAps = pBri->GetValueFy(ePartApsEnd, TRUE);
									pRBAps->m_Fy = dFyAps;
								}
							}	
						}
					}
				}
			}
		}
		if(!IsDefaulted(bri, DEFAULT_REBARJEWON_HYUNTA) || (nOriginTab==8 && IsClickDefaultButton(bri)))	// ����Ÿ��
		{
// 			long nBaseNum		= 1;
			long i = 0; for(i = 0; i < pBri->GetCountFooting(); i++)
			{
				CFootingApp *pFooting	= pBri->GetFooting(i);
				if(!pFooting) continue;
				if(pFooting->m_exFooting.m_nType != EXFOOTING_TYPE_PILE) continue;

				CExPileApp *pPile	= &pFooting->m_exFooting.m_Pile;
				if(pPile->m_nType != PILE_TYPE_HYUNJANG) continue;
				pPile->MakeRebarJewon(TRUE, pFooting->m_pBasicConcInfo);
			}
		}
		if(!IsDefaulted(bri, DEFAULT_REBARJEWON_ETC2) || (nOriginTab==9 && IsClickDefaultButton(bri)))	// ��Ÿö��
		{
			SetDefaultRebarJewonETC2(bri);	// ���Ǿ����� �⺻���� ����.
			SetDefaultRebarJewonETC2(bri);	// �ߺ����� �˾ƺ��� ���� �ѹ� ��..
		}

		pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, TRUE);
	}

	DrawInputDomyunView();

	SetGridData();
	SetDataInit();
}

void CInputRebarJewon::SetDefaultRebarJewonETC2(long bri)
{
	CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);

	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pStd->IsLsdDesign();

	pBri->m_Util.SyncRebarAddRateOpt();
	long nQtyHDan = pBri->GetQtyHDan();
	long nHDan = -1;

	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		CGuardWallRC *pGW	= pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		long j=0; for(j=0; j<pGW->m_pArrRebar.GetSize(); j++)
		{
			CRebar* pRB = pGW->m_pArrRebar[j];
			if(pRB->m_bLock == FALSE)
				pRB->InitData();
		}

		if((pGW->IsTypeHDanRoadGuardFence() || pGW->IsTypeHDanRailDetectGravel()) && !pGW->m_bSoundProof)
		{
			BOOL bCheck = pGW->IsTypeHDanRoadGuardFence() ? IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_GUARDFENCE)
														  : IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_DETECTGRAVEL);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck && nHDan>=0 && nHDan<i)	// �ߺ��̰� ���� ���� ������ٸ� �ٽ� �� ������ �ʴ´�.
			{
				CGuardWallRC* pPrevGW = pBri->GetGuardWallByHDan(nHDan);
				if(pPrevGW)
					pBri->m_Util.MakeGuardFenceRebarJewon(pGW, pPrevGW, bCheck);
			}
			else
				pBri->m_Util.MakeGuardFenceRebarJewon(pGW, NULL, bCheck);	// �ߺ� �Ȱ��� ������ ��ȣ C, otherwise CL, CR 
		}
		else if(pGW->IsTypeHDanRoadCentralWallFull() && !pGW->m_bSoundProof)
		{
			if(pGW->m_bGWhalf)
				pBri->m_Util.MakeCenterGuardRebarJewon(pGW, NULL, FALSE);	// �ߺ� �Ȱ��� ������ ��ȣ C, otherwise CL, CR 
			else
				pBri->m_Util.MakeCenterGuard2RebarJewon(pGW, NULL, FALSE);
		}
		else if(pGW->IsTypeHDanRoadCentralWallHalf() && !pGW->m_bSoundProof)
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_CENTERGUARDHALF);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck && nHDan>=0 && nHDan<i)	// �ߺ��̰� ���� ���� ������ٸ� �ٽ� �� ������ �ʴ´�.
			{
				CGuardWallRC* pPrevGW = pBri->GetGuardWallByHDan(nHDan);
				if(pPrevGW)
					pBri->m_Util.MakeCenterGuardHalfRebarJewon(pGW, pPrevGW, bCheck);
			}
			else
				pBri->m_Util.MakeCenterGuardHalfRebarJewon(pGW, NULL, bCheck);	// �ߺ� �Ȱ��� ������ ��ȣ C, otherwise CL, CR 
		}
		if(pGW->IsTypeHDanRoadHandRail() && !pGW->m_bSoundProof)
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_HANDRAIL);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck && nHDan>=0 && nHDan<i)	// �ߺ��̰� ���� ���� ������ٸ� �ٽ� �� ������ �ʴ´�.
			{
				CGuardWallRC* pPrevGW = pBri->GetGuardWallByHDan(nHDan);
				if(pPrevGW)
					pBri->m_Util.MakeHandRailRebarJewon(pGW, pPrevGW, bCheck);
			}
			else
				pBri->m_Util.MakeHandRailRebarJewon(pGW, NULL, bCheck);	// �ߺ� �Ȱ��� ������ ��ȣ C, otherwise CL, CR 
		}
		if(pGW->IsTypeHDanRoadSoundProof() || pGW->m_bSoundProof)
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_SOUNDPROOF);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck && nHDan>=0 && nHDan<i)	// �ߺ��̰� ���� ���� ������ٸ� �ٽ� �� ������ �ʴ´�.
			{
				CGuardWallRC* pPrevGW = pBri->GetGuardWallByHDan(nHDan);
				if(pPrevGW)
					pBri->m_Util.MakeSoundProofRebarJewon(pGW, pPrevGW, bCheck);
			}
			else
				pBri->m_Util.MakeSoundProofRebarJewon(pGW, NULL, bCheck);	// �ߺ� �Ȱ��� ������ ��ȣ C, otherwise CL, CR 
		}
		if(pGW->IsTypeHDanRoadRetainingWall())
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_RETAININGWALL);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck && nHDan>=0 && nHDan<i)	// �ߺ��̰� ���� ���� ������ٸ� �ٽ� �� ������ �ʴ´�.
			{
				CGuardWallRC* pPrevGW = pBri->GetGuardWallByHDan(nHDan);
				if(pPrevGW)
					pBri->m_Util.MakeRetainingWallRebarJewon(pGW, pPrevGW, bCheck);
			}
			else
				pBri->m_Util.MakeRetainingWallRebarJewon(pGW, NULL, bCheck);	// �ߺ� �Ȱ��� ������ ��ȣ C, otherwise CL, CR 
		}
		if(pGW->IsTypeHDanRailFootwayBlock())
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_FOOTWAYBLOCK);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck && nHDan>=0 && nHDan<i)	// �ߺ��̰� ���� ���� ������ٸ� �ٽ� �� ������ �ʴ´�.
			{
				CGuardWallRC* pPrevGW = pBri->GetGuardWallByHDan(nHDan);
				if(pPrevGW)
					pBri->m_Util.MakeFootwayBlockRebarJewon(pGW, pPrevGW, bCheck);
			}
			else
				pBri->m_Util.MakeFootwayBlockRebarJewon(pGW, NULL, bCheck);	// �ߺ� �Ȱ��� ������ ��ȣ C, otherwise CL, CR 
		}
		if(pGW->IsTypeHDanRailGuardFence())
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_GUARDFENCE);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck && nHDan>=0 && nHDan<i)	// �ߺ��̰� ���� ���� ������ٸ� �ٽ� �� ������ �ʴ´�.
			{
				CGuardWallRC* pPrevGW = pBri->GetGuardWallByHDan(nHDan);
				if(pPrevGW)
					pBri->m_Util.MakeRailGuardFenceRebarJewon(pGW, pPrevGW, bCheck);
			}
			else
				pBri->m_Util.MakeRailGuardFenceRebarJewon(pGW, NULL, bCheck);	// �ߺ� �Ȱ��� ������ ��ȣ C, otherwise CL, CR 
		}
		if(pGW->IsTypeHDanRailHole())
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_HOLE);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck && nHDan>=0 && nHDan<i)	// �ߺ��̰� ���� ���� ������ٸ� �ٽ� �� ������ �ʴ´�.
			{
				CGuardWallRC* pPrevGW = pBri->GetGuardWallByHDan(nHDan);
				if(pPrevGW)
					pBri->m_Util.MakeRailTotalHoleRebarJewon(pGW, pPrevGW, bCheck);
			}
			else
				pBri->m_Util.MakeRailTotalHoleRebarJewon(pGW, NULL, bCheck);	// �ߺ� �Ȱ��� ������ ��ȣ C, otherwise CL, CR 
		}
		if(pGW->IsTypeHDanRailHoleFootway())
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_HOLEFOOTWAY);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck && nHDan>=0 && nHDan<i)	// �ߺ��̰� ���� ���� ������ٸ� �ٽ� �� ������ �ʴ´�.
			{
				CGuardWallRC* pPrevGW = pBri->GetGuardWallByHDan(nHDan);
				if(pPrevGW)
					pBri->m_Util.MakeRailTotalHoleRebarJewon(pGW, pPrevGW, bCheck);
			}
			else
				pBri->m_Util.MakeRailTotalHoleRebarJewon(pGW, NULL, bCheck);	// �ߺ� �Ȱ��� ������ ��ȣ C, otherwise CL, CR 
		}
	}
	pBri->MakeMonumentRebarJewon();
}

void CInputRebarJewon::SetDataInit()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_Grid.UpdateData(FALSE);
	
	//��ȣ���� �ߺд� �� ����
	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols);
	m_Grid.MergeGrid(1,nRows,0,1);
}

void CInputRebarJewon::SetDataSave()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_Grid.UpdateData(TRUE);

	m_pIn->m_pDoc->SetModifiedFlag();
}

void CInputRebarJewon::SetGridData()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	if(m_pIn->m_pARcBridgeDataStd->GetBridgeSize()==0) return;	

	CWingWall* pWing		= NULL;
	CExPileApp* pPile		= NULL;
	CFootingApp *pFooting	= NULL;
	long stt(0), left(0), cns(0);
	BOOL bStt(TRUE), bLeft(TRUE);
	CString str	= _T("");
	CSafetyTypedPtrArray <CObArray, CRebar*> *pArrRebar	= NULL;
	m_pArrRebar.RemoveAll();

	SetGridTitleFirst();

	long nCountFooting	= pBri->GetCountFooting();
	long nCountInWall	= pBri->GetCountInWall();
	long i = 0;

	int nOriginTab = GetTabCurOriginSel();
	switch(nOriginTab)
	{
	case 0:
		SetGridJewonData(NULL, 0, 0, TRUE);	//Grid Row�ʱ�ȭ
		SetGridJewonInit(&pBri->m_pArrRebarMain, TYPE_MAIN);
		// ������ü
		for(i = 0; i < nCountInWall; i++)
		{
			CWallApp *pWall	= pBri->GetInWall(i);
			if(pWall)
				SetGridJewonInit(&pWall->m_pArrRebarMain, TYPE_MAIN);
		}
		// ����
		for(i = 0; i < nCountFooting; i++)
		{
			CFootingApp *pFooting	= pBri->GetFooting(i);
			if(pFooting)
				SetGridJewonInit(&pFooting->m_pArrRebarMain, TYPE_MAIN);
		}
		SetGridJewonInit(&pBri->m_pArrRebarMain_User, TYPE_MAIN);

		break;
	case 1:
		SetGridJewonData(NULL, 1, 0, TRUE);	//Grid Row�ʱ�ȭ
		// ���ö�� ////////////////////
		SetGridJewonInit(&pBri->m_pArrRebarSupport, TYPE_ECT);
		// ������ü
		for(i = 0; i < nCountInWall; i++)
		{
			CWallApp *pWall	= pBri->GetInWall(i);
			if(pWall)
				SetGridJewonInit(&pWall->m_pArrRebarSupport, TYPE_ECT);
		}
		// ����
		for(i = 0; i < nCountFooting; i++)
		{
			CFootingApp *pFooting	= pBri->GetFooting(i);
			if(pFooting)
				SetGridJewonInit(&pFooting->m_pArrRebarSupport, TYPE_ECT);
		}
		// ����ö�� //////////////
		SetGridJewonInit(&pBri->m_pArrRebarShear, TYPE_ECT);
		// ������ü
		for(i = 0; i < nCountInWall; i++)
		{
			CWallApp *pWall	= pBri->GetInWall(i);
			if(pWall)
				SetGridJewonInit(&pWall->m_pArrRebarShear, TYPE_ECT);
		}
		// ����
		for(i = 0; i < nCountFooting; i++)
		{
			CFootingApp *pFooting	= pBri->GetFooting(i);
			if(pFooting)
				SetGridJewonInit(&pFooting->m_pArrRebarShear, TYPE_ECT);
		}

		// ��Ÿö�� ///////////////
		SetGridJewonInit(&pBri->m_pArrRebarEtc, TYPE_ECT);
		// ����
		for(i = 0; i < nCountFooting; i++)
		{
			CFootingApp *pFooting	= pBri->GetFooting(i);
			if(pFooting)
				SetGridJewonInit(&pFooting->m_pArrRebarEtc, TYPE_ECT);
		}
		break;
	case 2:
		SetGridJewonData(NULL, 1, 0, TRUE);	//Grid Row�ʱ�ȭ
		// ������ü
		for(i = 0; i < nCountInWall; i++)
		{
			CWallApp *pWall	= pBri->GetInWall(i);
			if(pWall)
			{
				if(pWall->m_bTopIsColumn)
					SetGridJewonInit(&pWall->m_pArrRebarColumn, TYPE_ECT);
			}
		}		

		// ��(�Ŵ�)
		SetGridJewonInit(&pBri->m_pArrRebarGirder, TYPE_ECT);
		SetGridJewonInit(&pBri->m_pArrRebarGirder_User, TYPE_MAIN);
		break;
	case 3:
		pWing = pBri->GetWingWall(TRUE, TRUE);
		SetGridJewonData(NULL, 0, 0, TRUE);	//Grid Row�ʱ�ȭ
		SetGridJewonInit(&pWing->m_pArrRebar, TYPE_WING);
		SetGridJewonInit(&pWing->m_pArrRebar_User, TYPE_WING);
		break;
	case 4:
		pWing = pBri->GetWingWall(TRUE, FALSE);
		SetGridJewonData(NULL, 0, 0, TRUE);	//Grid Row�ʱ�ȭ
		SetGridJewonInit(&pWing->m_pArrRebar, TYPE_WING);
		SetGridJewonInit(&pWing->m_pArrRebar_User, TYPE_WING);
		break;
	case 5:
		pWing = pBri->GetWingWall(FALSE, TRUE);
		SetGridJewonData(NULL, 0, 0, TRUE);	//Grid Row�ʱ�ȭ
		SetGridJewonInit(&pWing->m_pArrRebar, TYPE_WING);
		SetGridJewonInit(&pWing->m_pArrRebar_User, TYPE_WING);
		break;
	case 6:
		pWing = pBri->GetWingWall(FALSE, FALSE);
		SetGridJewonData(NULL, 0, 0, TRUE);	//Grid Row�ʱ�ȭ
		SetGridJewonInit(&pWing->m_pArrRebar, TYPE_WING);
		SetGridJewonInit(&pWing->m_pArrRebar_User, TYPE_WING);
		break;
	case 7:
		SetGridJewonData(NULL, 0, 0, TRUE);	//Grid Row�ʱ�ȭ
		for(stt = 0; stt < 2; stt++)
		{
			bStt	= stt == iSTT;
			for(left = 0; left < 2; left++)
			{
				bLeft	= left == iLEFT;
				if(bStt)
				{
					if(!pBri->m_apsStt[left].m_bIs) continue;
					pArrRebar = &pBri->m_apsStt[left].m_pArrRebar;
				}
				else
				{
					if(!pBri->m_apsEnd[left].m_bIs) continue;
					pArrRebar = &pBri->m_apsEnd[left].m_pArrRebar;
				}

				str.Format("%s����\n����\n������\n(%s)", bStt ? "��" : "��", bLeft ? "��" : "��");
				SetGridJewonInit(pArrRebar, TYPE_APS, 0, FALSE, str);
	
				for(cns = 0; cns < 2; cns++)
				{
					if(bStt)
					{
						if(!pBri->m_cnsStt[left][cns].m_bIs) continue;
						pArrRebar = &pBri->m_cnsStt[left][cns].m_pArrRebar;
					}
					else
					{
						if(!pBri->m_cnsEnd[left][cns].m_bIs) continue;
						pArrRebar = &pBri->m_cnsEnd[left][cns].m_pArrRebar;
					}

					str.Format("%s����\n����\n������\n(%s%d)", bStt ? "��" : "��", bLeft ? "��" : "��", cns+1);
					SetGridJewonInit(pArrRebar, TYPE_APS, 0, FALSE, str);
				}
			}
		}
		break;
	case 8:
		SetGridJewonData(NULL, 0, 0, TRUE);		//Grid Row�ʱ�ȭ

		for(i = 0; i <= pBri->m_nQtyJigan; i++)
		{
			pFooting	= pBri->GetFootingByJijumNumber(i);
			if(!pFooting) continue;
			if(pFooting->m_exFooting.m_nType != EXFOOTING_TYPE_PILE) continue;
			
			pPile	= &pFooting->m_exFooting.m_Pile;
			if(pPile->m_nType != PILE_TYPE_HYUNJANG) continue;
			pArrRebar	= &pPile->m_pHyunTa->m_pArrRebar;
			if(pArrRebar != NULL) 
			{
				if(i==0 || i== pBri->m_nQtyJigan) str = (i == 0)? _T("�� ��") : _T("�� ��");
				else str.Format("����%d", i+1);
				SetGridJewonInit(pArrRebar, TYPE_PILE_HYUNTA, 0, FALSE, str);
			}
		}
		break;
	case 9:
		SetGridJewonData(NULL, 0, 0, TRUE);		//Grid Row�ʱ�ȭ
		SetGridJewonDataETC();
		break;
	}
			
	// �� width ����
	m_Grid.MergeGrid(0,0,0,1);
	long nRows = 0;
	long nCols = 0;
	m_Grid.GetCellCount(nRows, nCols);
	m_Grid.SetRows(nRows);
	m_Grid.SetCols(nCols);
	if(nRows > 0)
	{
		m_Grid.SetColumnWidthAll(44);
		m_Grid.SetColumnWidth(0, 100);
		m_Grid.SetColumnWidth(1, 142);
		m_Grid.SetColumnWidth(11, 49);
		m_Grid.SetColumnWidth(19, 49);
		m_Grid.SetColumnWidth(21, 0);
	}
	if(nCols > 24)
	{
		long nColStt = 23;
		m_Grid.SetColumnWidth(nColStt + 8, 70);
		m_Grid.SetColumnWidth(nColStt + 9, 70);
		m_Grid.SetColumnWidth(nColStt + 11, 70);
		m_Grid.SetColumnWidth(nColStt + 12, 70);
		m_Grid.SetColumnWidth(nColStt + 13, 70);
		m_Grid.SetColumnWidth(nColStt + 27, 70);
	}
}

void CInputRebarJewon::SetGridJewonDataETC()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	if(m_pIn->m_pARcBridgeDataStd->GetBridgeSize()==0) return;	

	CGuardWallRC* pGW = NULL;
	CSafetyTypedPtrArray <CObArray, CRebar*> *pArrRebar	= NULL;

	// �ߺ����� ��ȣ ����
	CDoubleArray dGuardFenceArr;		dGuardFenceArr.RemoveAll();	
	CDoubleArray dCenterGuardArr;		dCenterGuardArr.RemoveAll();
	CDoubleArray dCenterGuardHalfArr;	dCenterGuardHalfArr.RemoveAll();
	CDoubleArray dHandRailArr;			dHandRailArr.RemoveAll();
	CDoubleArray dSoundProofArr;		dSoundProofArr.RemoveAll();
	CDoubleArray dRetainingWallArr;		dRetainingWallArr.RemoveAll();
	CDoubleArray dFootwayBlockArr;		dFootwayBlockArr.RemoveAll();
	CDoubleArray dRailGuardFenceArr;	dRailGuardFenceArr.RemoveAll();
	CDoubleArray dRailHoleArr;			dRailHoleArr.RemoveAll();
	CDoubleArray dRailHoleFootwayArr;	dRailHoleFootwayArr.RemoveAll();

	// ��Ÿ ö���� ö�� ��ũ �����Ҽ� �ֵ��� ����.(ARCBRIDGE-1647)
	BOOL bEditMark = TRUE;

	long nHDan = -1;					// �ߺ��Ǵ� ��ȣ���� �ִ� ����
	long nQtyHDan = pBri->GetQtyHDan();

	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if((pGW->IsTypeHDanRoadGuardFence() || pGW->IsTypeHDanRailDetectGravel()) && !pGW->m_bSoundProof)
		{
			BOOL bCheck = pGW->IsTypeHDanRoadGuardFence() ? IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_GUARDFENCE)
													      : IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_DETECTGRAVEL);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck)
				dGuardFenceArr.Add(i+1);					// �ߺ��Ǵ� ��ȣ�� ���� ����
			
//			if(bCheck && nHDan>=0 && nHDan<i)				// �ߺ��̰� ���� ���� ������ٸ� ������ ���Ѵ�.
//			{
//				CGuardWallRC* pPrevGW = pBri->GetGuardWallByHDan(nHDan);
//				pBri->m_Util.MakeGuardFenceRebarJewon(pGW, pPrevGW, bCheck);
//			}
//			else
//				pBri->m_Util.MakeGuardFenceRebarJewon(pGW, NULL, bCheck);	// �ߺ� �Ȱ��� ������ ��ȣ C, otherwise CL, CR 
		}
//		else if(pGW->IsTypeHDanRoadCentralWallFull())
//		{
//				//pBri->m_Util.MakeCenterGuardRebarJewon(pGW, NULL, FALSE);	// �ߺ� �Ȱ��� ������ ��ȣ C, otherwise CL, CR 
//				pBri->m_Util.MakeCenterGuard2RebarJewon(pGW, NULL, FALSE);
//		}
		else if(pGW->IsTypeHDanRoadCentralWallHalf() && !pGW->m_bSoundProof)
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_CENTERGUARDHALF);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck)
				dCenterGuardHalfArr.Add(i+1);				// �ߺ��Ǵ� ��ȣ�� ���� ����			
		}
		else if(pGW->IsTypeHDanRoadHandRail() && !pGW->m_bSoundProof)
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_HANDRAIL);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck)
				dHandRailArr.Add(i+1);				// �ߺ��Ǵ� ��ȣ�� ���� ����		
		}
		else if(pGW->IsTypeHDanRoadSoundProof() || pGW->m_bSoundProof)
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_SOUNDPROOF);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck)
				dSoundProofArr.Add(i+1);				// �ߺ��Ǵ� ��ȣ�� ���� ����			
		}
		else if(pGW->IsTypeHDanRoadRetainingWall())
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_RETAININGWALL);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck)
				dRetainingWallArr.Add(i+1);					// �ߺ��Ǵ� ��ȣ�� ���� ����
		}
		else if(pGW->IsTypeHDanRailFootwayBlock())
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_FOOTWAYBLOCK);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck)
				dFootwayBlockArr.Add(i+1);					// �ߺ��Ǵ� ��ȣ�� ���� ����
		}
		else if(pGW->IsTypeHDanRailGuardFence())
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_GUARDFENCE);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck)
				dRailGuardFenceArr.Add(i+1);					// �ߺ��Ǵ� ��ȣ�� ���� ����
		}
		else if(pGW->IsTypeHDanRailHole())
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_HOLE);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck)
				dRailHoleArr.Add(i+1);					// �ߺ��Ǵ� ��ȣ�� ���� ����
		}
		else if(pGW->IsTypeHDanRailHoleFootway())
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_HOLEFOOTWAY);	// �ߺ��Ǵ� ���� �ִ��� �˻�
			
			if(bCheck)
				dRailHoleFootwayArr.Add(i+1);					// �ߺ��Ǵ� ��ȣ�� ���� ����
		}
	}
	
	// �׸��忡 ���
	for(i=0; i<nQtyHDan; i++)
	{
		pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if((pGW->IsTypeHDanRoadGuardFence() || pGW->IsTypeHDanRailDetectGravel()) && !pGW->m_bSoundProof)
		{
			BOOL bCheck = pGW->IsTypeHDanRoadGuardFence() ? IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_GUARDFENCE) 
														  : IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_DETECTGRAVEL);	// �ߺ��Ǵ� ���� �ִ��� �˻�

			if(!bCheck || (bCheck && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
			{
				pArrRebar = &pGW->m_pArrRebar;
				if(pArrRebar!=NULL)
					pGW->IsTypeHDanRoadGuardFence() ? SetGridJewonInit(pArrRebar, TYPE_GUARDWALL, i+1, FALSE, _T(""), &dGuardFenceArr, bEditMark)
													: SetGridJewonInit(pArrRebar, TYPE_RAILDETECTGRAVEL, i+1, FALSE, _T(""), &dGuardFenceArr, bEditMark);
			}
		}
		else if(pGW->IsTypeHDanRoadCentralWallFull() && !pGW->m_bSoundProof)
		{
			pArrRebar = &pGW->m_pArrRebar;
			if(pArrRebar!=NULL)
				SetGridJewonInit(pArrRebar, TYPE_CENTERGUARD, i+1, FALSE, _T(""), &dCenterGuardArr);
		}
		else if(pGW->IsTypeHDanRoadCentralWallHalf() && !pGW->m_bSoundProof)
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_CENTERGUARDHALF);	
			if(!bCheck || (bCheck && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
			{
				pArrRebar = &pGW->m_pArrRebar;
				if(pArrRebar!=NULL)
					SetGridJewonInit(pArrRebar, TYPE_CENTERGUARDHALF, i+1, FALSE, _T(""), &dCenterGuardHalfArr);
			}
		}
		else if(pGW->IsTypeHDanRoadHandRail() && !pGW->m_bSoundProof)
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_HANDRAIL);	
			if(!bCheck || (bCheck && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
			{
				pArrRebar = &pGW->m_pArrRebar;
				if(pArrRebar!=NULL)
					SetGridJewonInit(pArrRebar, TYPE_HANDRAIL, i+1, FALSE, _T(""), &dHandRailArr);
			}
		}
		else if(pGW->IsTypeHDanRoadSoundProof() || pGW->m_bSoundProof)
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_SOUNDPROOF);	
			if(!bCheck || (bCheck && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
			{
				pArrRebar = &pGW->m_pArrRebar;
				if(pArrRebar!=NULL)
					SetGridJewonInit(pArrRebar, TYPE_SOUNDPROOF, i+1, FALSE, _T(""), &dSoundProofArr, bEditMark);
			}
		}		
		else if(pGW->IsTypeHDanRoadRetainingWall())
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_RETAININGWALL);	
			if(!bCheck || (bCheck && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
			{
				pArrRebar = &pGW->m_pArrRebar;
				if(pArrRebar!=NULL)
					SetGridJewonInit(pArrRebar, TYPE_RETAININGWALL, i+1, FALSE, _T(""), &dRetainingWallArr);
			}
		}		
		else if(pGW->IsTypeHDanRailFootwayBlock())
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_FOOTWAYBLOCK);	
			if(!bCheck || (bCheck && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
			{
				pArrRebar = &pGW->m_pArrRebar;
				if(pArrRebar!=NULL)
					SetGridJewonInit(pArrRebar, TYPE_FOOTWAYBLOCK, i+1, FALSE, _T(""), &dFootwayBlockArr);
			}
		}	
		else if(pGW->IsTypeHDanRailGuardFence())
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_FOOTWAYBLOCK);	
			if(!bCheck || (bCheck && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
			{
				pArrRebar = &pGW->m_pArrRebar;
				if(pArrRebar!=NULL)
					SetGridJewonInit(pArrRebar, TYPE_RAILGUARDFENCE, i+1, FALSE, _T(""), &dRailGuardFenceArr, bEditMark);
			}
		}	
		else if(pGW->IsTypeHDanRailHole())
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_HOLE);	
			if(!bCheck || (bCheck && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
			{
				pArrRebar = &pGW->m_pArrRebar;
				if(pArrRebar!=NULL)
					SetGridJewonInit(pArrRebar, TYPE_RAILHOLE, i+1, FALSE, _T(""), &dRailHoleArr);
			}
		}	
		else if(pGW->IsTypeHDanRailHoleFootway())
		{
			BOOL bCheck = IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_FOOTWAYBLOCK);	
			if(!bCheck || (bCheck && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
			{
				pArrRebar = &pGW->m_pArrRebar;
				if(pArrRebar!=NULL)
					SetGridJewonInit(pArrRebar, TYPE_RAILHOLEFOOTWAY, i+1, FALSE, _T(""), &dRailHoleFootwayArr);
			}
		}			
	}

	SetGridJewonInit(&pBri->m_pArrRebarMonument, TYPE_MONUMENT, 0, FALSE);
}

BOOL CInputRebarJewon::IsDuplicate(long n, long& nHDan, long nType)
{
	// ������ ö�������� ������ ���� �ʾƼ�
	// ������ �ߺ� ���� �ʵ��� ��.
	// 2007.08.07 .
	return FALSE;

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	long nQtyHDan		= pBri->GetQtyHDan();
	CGuardWallRC* pCurrGW = pBri->GetGuardWallByHDan(n);
	double dCurrLen = pBri->GetGuardWallTotalLength(pCurrGW);
	
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		if(i!=n)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i); 
			double dLen = pBri->GetGuardWallTotalLength(pGW);
			if(!pGW) continue;
			if((pGW->m_nTypeGuardWall==nType) || (pCurrGW->m_bSoundProof && pGW->m_bSoundProof))
			{
				if(*pCurrGW == *pGW && dCurrLen == dLen)
				{
					nHDan = i;
					return TRUE;
				}
			}
		}
	}
	return FALSE;	// �ߺ��Ȱ��� ���ٸ�
}

void CInputRebarJewon::SetGridJewonInit(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrRebar, long nType, long nGW, BOOL bDisableEditJewon, CString strType, CDoubleArray* pArr, BOOL bEditMark)
{
// 	CRebar* pRB = NULL;
	long nSize = pArrRebar->GetSize();
	long i=0; for(i=0; i<nSize; i++)
	{
		CRebar* pRB = pArrRebar->GetAt(i);
		if(pRB->IsTrue())
			SetGridJewonData(pRB, nType, nGW, FALSE, bDisableEditJewon, strType, pArr, bEditMark);
	}
}

// BOOL bDisableEditJewon : ������ ������ �� ���� ���� �����ؾ� �Ǵ���..
// CString strTypeTmp : ������ strType�� ���⼭ �����ϰ� ������ strTypeTmp�� ������ ����
void CInputRebarJewon::SetGridJewonData(CRebar *pRB, long nType, long nGW, BOOL bInit, BOOL bDisableEditJewon, CString strTypeTmp, CDoubleArray* pArr, BOOL bEditMark)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();


	/*---------Grid Row�ʱ�ȭ-----------*/
	static long nGridRow = 0;
	if(bInit) 
	{
		nGridRow = 0;
		return;
	}
	nGridRow++;
	/*----------------------------------*/

	CStringArray sArr;
	CString str = _T("");
	CString strType = _T("");

	long nSum = 0;
	if(pArr!=NULL)
	{
		long i=0; for(i=0; i<pArr->GetSize(); i++)
			nSum += (long)pArr->GetAt(i);		// �ߺ��� ��ȣ���� ������ 0
	}
			CString strDan = _T("");
	switch(nType)
	{
		case TYPE_MAIN:
			strType	= "";
			break;
		case TYPE_ECT:
			break;
		case TYPE_COLUMN:
			break;
		case TYPE_WING:	
			if(pRB->m_sDescription.Right(3) == _T("2��"))
				strDan = _T("\n(2��)");
			if(pRB->m_nPos==REBAR_POS_WING_BACK)
				strType = "���\n" + strDan;
			else if(pRB->m_nPos==REBAR_POS_WING_FRONT)
				strType = "����\n" + strDan;
			else
				strType = "��Ÿ\n";
			break;
		case TYPE_APS:
			strType = _T("");
			break;
		case TYPE_PILE_HYUNTA:
			strType = _T("");
			break;
		case TYPE_GUARDWALL:
			strType = GetGuganTitle(nGW, nSum, nType, pArr);
			break;
		case TYPE_CENTERGUARD:
			strType.Format("�ߺд�\n(%d����)", nGW);
			break;
		case TYPE_CENTERGUARDHALF:
			strType = GetGuganTitle(nGW, nSum, nType, pArr);
			break;
		case TYPE_HANDRAIL:
			strType = GetGuganTitle(nGW, nSum, nType, pArr);
			break;
		case TYPE_SOUNDPROOF:
			strType = GetGuganTitle(nGW, nSum, nType, pArr);
			break;
		case TYPE_RETAININGWALL:
			strType = GetGuganTitle(nGW, nSum, nType, pArr);
			break;
		case TYPE_FOOTWAYBLOCK:
			strType = GetGuganTitle(nGW, nSum, nType, pArr);
			break;
		case TYPE_RAILGUARDFENCE:
			strType = GetGuganTitle(nGW, nSum, nType, pArr);
			break;
		case TYPE_RAILHOLE:
			strType = GetGuganTitle(nGW, nSum, nType, pArr);
			break;
		case TYPE_RAILHOLEFOOTWAY:
			strType = GetGuganTitle(nGW, nSum, nType, pArr);
			break;	
		case TYPE_RAILDETECTGRAVEL:
			strType = GetGuganTitle(nGW, nSum, nType, pArr);
			break;				
		case TYPE_MONUMENT:
			strType = _T("������\n��ħ");
			break;
	}

	if(strTypeTmp != "")
		strType = strTypeTmp;

	UINT nLine = bDisableEditJewon ? CELL_READONLY : CELL_LINE_NO;
	UINT nFormat = DT_CENTER | DT_VCENTER;

	BOOL bUseRebar = (pRB->m_sDescription.Find(_T("�߰�ö��")) != -1) ? TRUE : FALSE;
	
	long nCol = 0;
	str.Format("%d"	,pRB->m_nTypeRebar);
	m_Grid.AddCell(nGridRow,nCol++,strType != "" ? strType : pRB->m_sDescription);
	m_Grid.AddCell(nGridRow,nCol++,pRB->m_sDescription);

	m_Grid.AddCell(nGridRow, nCol++, &pRB->m_bLock, nFormat, bUseRebar ? CELL_READONLY : nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_sMark,DT_CENTER|DT_VCENTER,(bUseRebar || bEditMark) ? CELL_LINE_NO : CELL_READONLY);
	m_Grid.AddCell(nGridRow,nCol++,pRB->GetStringDia(),DT_CENTER|DT_VCENTER,bUseRebar ? CELL_LINE_NO : CELL_READONLY);

	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_dLengthMax_A, nFormat, nLine);	m_Grid.SetCellDec(CELL_DEC_0);  m_Grid.SetCellPrintZero(FALSE);
	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_dLengthMax_B, nFormat, nLine);	m_Grid.SetCellDec(CELL_DEC_0);  m_Grid.SetCellPrintZero(FALSE);
	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_dLengthMax_C, nFormat, nLine);	m_Grid.SetCellDec(CELL_DEC_0);  m_Grid.SetCellPrintZero(FALSE);
	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_dLengthMax_D, nFormat, nLine);	m_Grid.SetCellDec(CELL_DEC_0);  m_Grid.SetCellPrintZero(FALSE);
	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_E, nFormat, nLine);				m_Grid.SetCellDec(CELL_DEC_0);  m_Grid.SetCellPrintZero(FALSE);
	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_R, nFormat, nLine);				m_Grid.SetCellDec(CELL_DEC_0);  m_Grid.SetCellPrintZero(FALSE);
	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_dJointLength, nFormat, nLine);	m_Grid.SetCellDec(CELL_DEC_0);  m_Grid.SetCellPrintZero(FALSE);
	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_nJoint, nFormat, FALSE, nLine);	m_Grid.SetCellDec(CELL_DEC_0);  m_Grid.SetCellPrintZero(FALSE);
	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_nTypeRebar,DT_CENTER|DT_VCENTER,FALSE,nLine,str,CELL_TYPE_AUTO, sArr,&m_RebarTableDlg);	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_nEa, nFormat, FALSE, nLine);		m_Grid.SetCellDec(CELL_DEC_0);  m_Grid.SetCellPrintZero(FALSE);
	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_dRateAdd);						m_Grid.SetCellDec(CELL_DEC_2);  m_Grid.SetCellPrintZero(FALSE);
	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_dLengthMin_A, nFormat, nLine);	m_Grid.SetCellDec(CELL_DEC_0);  m_Grid.SetCellPrintZero(FALSE);
	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_dLengthMin_B, nFormat, nLine);	m_Grid.SetCellDec(CELL_DEC_0);  m_Grid.SetCellPrintZero(FALSE);
	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_dLengthMin_C, nFormat, nLine);	m_Grid.SetCellDec(CELL_DEC_0);  m_Grid.SetCellPrintZero(FALSE);
	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_dLengthMin_D, nFormat, nLine);	m_Grid.SetCellDec(CELL_DEC_0);  m_Grid.SetCellPrintZero(FALSE);
	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_bMainRebar,DT_CENTER|DT_VCENTER,CELL_LINE_NO,"",CELL_TYPE_CHECK);	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(nGridRow,nCol++,&pRB->m_nIdxNumber, nFormat, FALSE);		m_Grid.SetCellDec(CELL_DEC_0);  m_Grid.SetCellPrintZero(FALSE);
	m_Grid.AddCell(nGridRow,nCol++,Comma(nGW));

	if(m_checkViewlsd.GetCheck())
	{
		CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
		CLsdRebarManager *pLsdRebar = pBri->m_plsdRebar;
		SRebarEnvOption *pLsdOpt = pRB->m_pOptionLsd;

		SRebarEnvOption *pLsdOptManager = &pLsdRebar->GetOption();
		*pLsdOptManager = *pLsdOpt;
		//ARCBRIDGE-2430 ����ȭ�� ���ؼ� �س��� ���Դϴ� ���� ����


		CString str(_T(""));
		nLine = CELL_READONLY;

		if(pRB->m_nJoint > 0 && pLsdOpt->dFck > 0)
		{
			ERebarDia eDia = pLsdRebar->GetIndexDia(pRB->m_dDia);
			str.Format(_T("%.1f"), pLsdRebar->GetRebarInfo(eDia, eRebarArea));
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);
			str.Format(_T("%.1f"), pLsdRebar->GetRebarInfo(eDia, eRebarDia));
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);
			str.Format(_T("%.1f"), pLsdRebar->GetRebarInfo(eDia, eRebarBoundLength));
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);

			m_Grid.AddCell(nGridRow, nCol++, &pLsdOpt->dFck, nFormat, nLine);		m_Grid.SetCellDec(CELL_DEC_0); 
			str.Format(_T("%.0f"), pLsdRebar->Getfctm(pLsdOpt));
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);
			str.Format(_T("%.0f"), pLsdRebar->GetFctk(pLsdOpt));
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);
			m_Grid.AddCell(nGridRow, nCol++, &pLsdOpt->dFy, nFormat, nLine);		m_Grid.SetCellDec(CELL_DEC_0); 
			m_Grid.AddCell(nGridRow, nCol++, &pLsdOpt->dPic, nFormat, nLine);		m_Grid.SetCellDec(CELL_DEC_2); 
			m_Grid.AddCell(nGridRow, nCol++, &pLsdOpt->dPis, nFormat, nLine);		m_Grid.SetCellDec(CELL_DEC_2); 

			str = (pLsdOpt->nRebarType == eStraitRebar)? hggettext("����") : hggettext("�");
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);

			m_Grid.AddCell(nGridRow, nCol++, &pLsdOpt->dCheckCTC, nFormat, nLine);		m_Grid.SetCellDec(CELL_DEC_0); 
			m_Grid.AddCell(nGridRow, nCol++, &pLsdOpt->dCover, nFormat, nLine);		m_Grid.SetCellDec(CELL_DEC_0); 

			str.Format(_T("%.1f"), pLsdRebar->Getc(eDia));
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);

			m_Grid.AddCell(nGridRow, nCol++, &pLsdOpt->dHorCTC, nFormat, nLine);		m_Grid.SetCellDec(CELL_DEC_0); 
			//m_Grid.AddCell(nGridRow, nCol++, &(long&)pLsdOpt->nHorDia, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, m_sArrDia);
			str.Format(_T("%.1f"), pLsdRebar->GetRebarInfo(pLsdOpt->nHorDia, eRebarDia));
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);

			m_Grid.AddCell(nGridRow, nCol++, &pLsdOpt->dK, nFormat, nLine);		m_Grid.SetCellDec(CELL_DEC_3); 
			m_Grid.AddCell(nGridRow, nCol++, &pLsdOpt->dRestraintStress, nFormat, nLine);		m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.AddCell(nGridRow, nCol++, &pLsdOpt->dOverRatio, nFormat, nLine);		m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(nGridRow, nCol++, &pLsdOpt->dAsRatio, nFormat, nLine);		m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(nGridRow, nCol++, pLsdOpt->bUpperRebar ? &pLsdOpt->dn1[1] :&pLsdOpt->dn1[0], nFormat, nLine);		m_Grid.SetCellDec(CELL_DEC_2);
			if(pRB->m_dDia > 32)
			{
				ERebarDia eDia = pLsdRebar->GetIndexDia(pRB->m_dDia);
				CString sDia = _T("");
				sDia.Format(_T("%.2f"), pLsdRebar->Getn2(eDia));
				m_Grid.AddCell(nGridRow, nCol++, sDia, nFormat, nLine);		
			}
			else
			{
				m_Grid.AddCell(nGridRow, nCol++, &pLsdOpt->dn2, nFormat, nLine);		m_Grid.SetCellDec(CELL_DEC_2); 
			}

			str.Format(_T("%.0f"), pLsdRebar->Getfbd(eDia));				// fbd
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);
			str.Format(_T("%.0f"), pLsdRebar->GetBasicSettle(eDia));				// �⺻��������
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);
			str.Format(_T("%.2f"), pLsdRebar->GetAlpha1(eDia, pLsdOpt->ePosType));				// ��1
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);
			str.Format(_T("%.2f"), pLsdRebar->GetAlpha2(eDia, eRebarPosTension));				// ��2
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);
			str.Format(_T("%.2f"), pLsdRebar->GetSigmaAt(eDia));				// ��At
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);
			str.Format(_T("%.1f"), pLsdRebar->GetSigmaAtMin(eDia, FALSE));				// ��Atmin
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);
			str.Format(_T("%.6f"), pLsdRebar->GetAlpha3(eDia, eRebarPosTension));				// ��3
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);
			str.Format(_T("%.2f"), pLsdRebar->GetAlpha5(eDia, eRebarPosTension));				// ��5
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);
			str.Format(_T("%.2f"), pLsdRebar->GetAlpha6(eDia));				// ��6
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);

			str.Format(_T("%.0f"), pLsdRebar->GetLengthDesignConnect(eDia));				// ������������
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);
			str.Format(_T("%.0f"), pLsdRebar->GetLengthMinConnect(eDia));				// �ּҼ�����������
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);
			str.Format(_T("%.0f"), pLsdRebar->GetLengthConnect(eDia));				// �ּ�������
			m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);
		}
		else
		{
			for(HGINT32 ix=0; ix<33; ++ix)
			{
				m_Grid.AddCell(nGridRow, nCol++, str, nFormat, CELL_READONLY);
			}
		}
	}

	m_pArrRebar.Add(pRB);
	//m_Grid.AddCellForSave(nGridRow,nCol, (void *)pRB);
}

CString CInputRebarJewon::GetGuganTitle(long nGW, long nSum, long nType, CDoubleArray* pArr)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	CGuardWallRC* pGW = NULL;
	pGW = pBri->GetGuardWallByHDan(nGW-1);

	CString strName = _T("");
	CString strType = _T("");
	CString strGugan = _T("����)");

	switch(nType)
	{
		case TYPE_GUARDWALL:
			strName = _T("��ȣ��");
			break;
		case TYPE_CENTERGUARDHALF:
			strName = _T("�ߺд�1/2");
			break;
		case TYPE_HANDRAIL:
			strName = _T("��������");
			break;
		case TYPE_SOUNDPROOF:
			{
				if(pGW && pGW->m_bSoundProof)
					strName = _T("��ȣ��");
				else
					strName = _T("����������");
				break;
			}
		case TYPE_RETAININGWALL:
			strName = _T("�븷�̺�");
			break;
		case TYPE_FOOTWAYBLOCK:
			strName = _T("����+����");
			break;
		case TYPE_RAILGUARDFENCE:
			strName = _T("����");
			break;
		case TYPE_RAILHOLE:
			strName = _T("��ű�");
			break;
		case TYPE_RAILHOLEFOOTWAY:
			strName = _T("��ű�+����");
			break;
		case TYPE_RAILDETECTGRAVEL:
			strName = _T("�ڰ�����(ö��)");
			break;
	}
		
	if(nSum==0)
		strType.Format(_T("%s\n(%d����)"), strName, nGW);
	else
	{
		strType.Format(_T("%s\n("), strName);
		long nSize = pArr->GetSize();
		CString str(_T(""));
		long i=0; for(i=0; i<nSize; i++)
		{
			long nGugan = (long)pArr->GetAt(i);
			if(nGugan>0)
			{
				str.Format(_T("%d,"), nGugan);
				strType += str;
			}
		}
		strType.Delete(strType.GetLength()-1,1);	// ������ ',' ����
		strType += strGugan;
	}
	
	return strType;
}

void CInputRebarJewon::InitTabCtrl()
{
	m_Tab.RemoveAllTabs();
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	CStringArray sArrTabName;
	sArrTabName.Add("��ö��(��ü)");
	sArrTabName.Add("��� �� ���� ö��(��ü)");
	sArrTabName.Add("��, ���");

	if(pBri->IsWingWall(TRUE, TRUE))
		sArrTabName.Add("������ ���� ����");
	if(pBri->IsWingWall(TRUE, FALSE))
		sArrTabName.Add("������ ���� ����");
	if(pBri->IsWingWall(FALSE, TRUE))
		sArrTabName.Add("������ ���� ����");
	if(pBri->IsWingWall(FALSE, FALSE))
		sArrTabName.Add("������ ���� ����");
	if(pBri->IsAnyAps())
		sArrTabName.Add("���� ������");

	if(pBri->IsExsitHyunTa())
		sArrTabName.Add("���� Ÿ�� ����");

	sArrTabName.Add("��Ÿ ö��");

	long i=0; for(i=0; i<sArrTabName.GetSize(); i++)
		m_Tab.InsertItem(i, sArrTabName[i]);
}

void CInputRebarJewon::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataSave();

	return;
}

void CInputRebarJewon::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetFocus();
	SetGridData();
	SetDataInit();
	SetControl();
	if(m_Grid.GetRowCount()>1)
		m_Grid.SetFocusCell(1,3);
	DrawInputDomyunView(TRUE);

	return;
}

void CInputRebarJewon::InitComboCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	CString str = _T("");
	// ����
	long nSize = pStd->GetBridgeSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(i);
		str = pStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_comboBridgeNo.AddString(str);
	}
	m_comboBridgeNo.SetCurSel(m_pIn->m_nCurrentBridge);
	
	SetControl();
}

void CInputRebarJewon::OnSelchangeComboBridgeNo() 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	m_pIn->m_nCurrentBridge = m_comboBridgeNo.GetCurSel();
	InitTabCtrl();
	SetControl();
	SetGridData();
	SetDataInit();
	SetPaneTitle();
	DrawInputDomyunView(TRUE);
	m_comboBridgeNo.SetFocus();
}

CDRect CInputRebarJewon::DrawGuardWall(CDomyun *pDom, BOOL bInput)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = m_pIn->GetRcBridgeRebar();
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	pDom->SetScaleDim(15);
	
	CCellID cID = m_Grid.GetFocusCell();
	CGuardWallRC* pGW = GetGuardWallByRow(cID.row);
	long nHDan = GetHDanByRow(cID.row);	
		
	CDomyun Dom(pDom);

	if((pGW->IsTypeHDanRoadGuardFence() || pGW->IsTypeHDanRailDetectGravel()) && !pGW->m_bSoundProof)
		DBStd.m_pDrawCrossSection->DrawRebarGuardFence(&Dom, nHDan, TRUE, TRUE, bInput);
	else if(pGW->IsTypeHDanRoadCentralWallFull() && !pGW->m_bSoundProof)
		DBStd.m_pDrawCrossSection->DrawRebarCenterGuard(&Dom, nHDan, TRUE, TRUE, bInput);
	else if(pGW->IsTypeHDanRoadCentralWallHalf() && !pGW->m_bSoundProof)
		DBStd.m_pDrawCrossSection->DrawRebarCenterGuardHalf(&Dom, nHDan, TRUE, TRUE, bInput);
	else if(pGW->IsTypeHDanRoadHandRail() && !pGW->m_bSoundProof)
		DBStd.m_pDrawCrossSection->DrawRebarHandRail(&Dom, nHDan, TRUE, TRUE, bInput);
	else if(pGW->IsTypeHDanRoadSoundProof() || pGW->m_bSoundProof)
		DBStd.m_pDrawCrossSection->DrawRebarSoundProof(&Dom, nHDan, TRUE, TRUE, bInput);
	else if(pGW->IsTypeHDanRoadRetainingWall())
		DBStd.m_pDrawCrossSection->DrawRebarRetainingWall(&Dom, nHDan, TRUE, TRUE, bInput);
	else if(pGW->IsTypeHDanRailGuardFence())
		DBStd.m_pDrawCrossSection->DrawRebarRailGuardFence(&Dom, nHDan, TRUE, TRUE, bInput);
	else if(pGW->IsTypeHDanRailFootwayBlock())
	{
		CString str = m_Grid.GetTextMatrix(cID.row,1);
		if(str.Find(_T("�߾Ӻ�")) > 0)
			DBStd.m_pDrawCrossSection->DrawRebarFootwayBlock(&Dom, nHDan, TRUE, TRUE, bInput, TRUE);
		else
			DBStd.m_pDrawCrossSection->DrawRebarFootwayBlock(&Dom, nHDan, TRUE, TRUE, bInput, FALSE);
	}
	else if(pGW->IsTypeHDanRailTotalHole())
	{
		CString str = m_Grid.GetTextMatrix(cID.row,1);
		if(str.Find(_T("�߾Ӻ�")) > 0)
			DBStd.m_pDrawCrossSection->DrawRebarTotalHole(&Dom, nHDan, TRUE, TRUE, bInput, TRUE);
		else
			DBStd.m_pDrawCrossSection->DrawRebarTotalHole(&Dom, nHDan, TRUE, TRUE, bInput, FALSE);
	}
	
	Dom.SetPosition(0,0);
	CDRect RectDom = Dom.GetExtRect();

	*pDom << Dom;

	return RectDom;
}

CDRect CInputRebarJewon::DrawWingWall(CDomyun *pDom, BOOL bStt, BOOL bLeft, int nOut)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = m_pIn->GetRcBridgeRebar();
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	int se = bStt ? -1 : 1;
	int nLeft = bLeft ? -1*se : 1*se;	// ������ ��� ���� ���� ������ �¿�� �������� �¿찡 �ݴ���.

	//long nTypeJongdanForRebarOrg = pBri->m_nTypeJongdanForRebar;
	//pBri->m_nTypeJongdanForRebar = 2;
	//pBri->SyncBridge(nLeft, TRUE);
	pBri->GetTvVertSection_Std(nLeft, TRUE, FALSE, FALSE, 2);

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	BOOL bSide		= (pWing->m_nAttachPos == 0 )? FALSE : TRUE;
	CDPoint vAng = pBri->GetAngleWingWallBetweenWall(bStt, bLeft);
	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	double dSpareT = 0;
	if(bSide)
	{
		double dAngle	= vAng.GetAngleDegree();
		double dAngJijum = pBri->GetAngleJijum(bStt? 0 : pBri->m_nQtyJigan).GetAngleDegree();
		double dAngA1 = (dAngle - dAngJijum) * (bLeft? -1 : 1);
		
		dSpareT = (nOut==0)? (dT2)/tan(ToRadian(dAngA1)) : 0.0;
	}
	else dSpareT =(nOut==0) ? max(dT1, dT2)/vAng.y*vAng.x : 0.0;

	double dSpare1 = 500.0;
	double dSpare2 = 500.0;
	se = (int)(bStt ? -1.0 : 1.0);
	double lr = bLeft ? -1.0 : 1.0;
	
	pDom->SetScaleDim(50);
	// ���鵵
	CDomyun DomFront(pDom);

	// (ARCBRIDGE-1520) ���鵵 ������ ��� ������ ����ϱ� ������ ���⼭ �ʱ�ȭ�Ѵ�.
	pBri->GetTvWingWallPlane(bStt, TRUE, TRUE);

	//.. �Ϲݵ�
	m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_CONC);
	DBStd.m_pDrawWingWall->DrawGenFront(&DomFront, bStt, bLeft, nOut);
	DBStd.m_pDrawWingWall->DrawGenCutBriSect(&DomFront, bStt, bLeft, nOut);
	DBStd.m_pDrawWingWall->DrawBlockBoundary(&DomFront, bStt, bLeft, nOut);

	//.. ������
	long nDan = 0;
	m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_BARC);
	DBStd.m_pDrawWingWall->DrawRebarBlockAMainFront(&DomFront, bStt, bLeft, nOut, nDan);
	DBStd.m_pDrawWingWall->DrawRebarBlockBMainFront(&DomFront, bStt, bLeft, nOut, nDan);
	DBStd.m_pDrawWingWall->DrawRebarBlockCMainFront(&DomFront, bStt, bLeft, nOut, nDan);
	DBStd.m_pDrawWingWall->DrawRebarBlockDMainFront(&DomFront, bStt, bLeft, nOut, nDan);

	DBStd.m_pDrawWingWall->DrawRebarBlockASupportFront(&DomFront, bStt, bLeft, nOut, nDan);
	DBStd.m_pDrawWingWall->DrawRebarBlockDSupportFront(&DomFront, bStt, bLeft, nOut, nDan);

	DBStd.m_pDrawWingWall->DrawRebarBlockAShearFront(&DomFront, bStt, bLeft, nOut);
	DBStd.m_pDrawWingWall->DrawRebarBlockBCShearFront(&DomFront, bStt, bLeft, nOut);
	DBStd.m_pDrawWingWall->DrawRebarBlockDShearFront(&DomFront, bStt, bLeft, nOut);

	DBStd.m_pDrawWingWall->DrawRebarHunchFootingFront(&DomFront, bStt, bLeft, nOut);
	DBStd.m_pDrawWingWall->DrawRebarHunchWallFront(&DomFront, bStt, bLeft, nOut);

	DBStd.m_pDrawWingWall->MarkRebarBlockAFront(&DomFront, bStt, bLeft, nOut);
	DBStd.m_pDrawWingWall->MarkRebarBlockBCFront(&DomFront, bStt, bLeft, nOut);
	DBStd.m_pDrawWingWall->MarkRebarBlockDFront(&DomFront, bStt, bLeft, nOut);
	if(nOut==0)
	{
		DBStd.m_pDrawWingWall->MarkRebarHunchWallFront(&DomFront, bStt, bLeft);
		DBStd.m_pDrawWingWall->MarkRebarHunchFootingFront(&DomFront, bStt, bLeft);
	}

	CDRect rcFront = DomFront.GetExtRect();

	m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_SMALL_TITLE);
	DomFront.SetTextAlignHorz(TA_CENTER);
	DomFront.SetTextHeight(7.5);
	DomFront.TextOut(rcFront.CenterPoint().x, rcFront.bottom+dSpare1, _T("�� �� ��"));

	rcFront = DomFront.GetExtRect();
	DomFront.Move(dSpareT*se, 0);

	*pDom << DomFront;

	// �ܸ鵵
	CDomyun DomSect(pDom);

	CTwinVectorArray tvArrHu, tvArrGen;
	CRcBridgeRebar* pBriR = m_pIn->GetRcBridgeRebar();
	pBriR->GetTvArrayRebarWingWallHunchFootingSect(tvArrHu, bStt, bLeft);
	CDPoint xyHu1 = tvArrHu.GetAt(0).m_v1;
	CDPoint xyHu2 = tvArrHu.GetAt(2).m_v2;
	double dHuW = pWing->m_dHuW1;
	double dCoverWingOut = pBriR->m_dCoverWingOut[0];
	double dSpare = (!bSide)? fabs(xyHu1.x-xyHu2.x) + dCoverWingOut*2 - (dT2+dHuW)
		: fabs(xyHu1.x-xyHu2.x) + dCoverWingOut*2 + (dT2+dHuW)+1000;
	if(pBri->IsBoxType())
		dSpare = dT2 + dCoverWingOut*2;
	m_pIn->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_CONC);
	DBStd.m_pDrawWingWall->DrawGenSection(&DomSect, bStt, bLeft, dSpare);

	m_pIn->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_BARC);
	DBStd.m_pDrawWingWall->DrawRebarSection(&DomSect, bStt, bLeft);

	DBStd.m_pDrawWingWall->MarkRebarSection(&DomSect, bStt, bLeft, 0);
	DBStd.m_pDrawWingWall->MarkRebarSection(&DomSect, bStt, bLeft, 1);

	CDRect rcSect = DomSect.GetExtRect();

	m_pIn->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_SMALL_TITLE);
	DomSect.SetTextAlignHorz(TA_CENTER);
	DomSect.SetTextHeight(7.5);
	DomSect.TextOut(rcSect.CenterPoint().x, rcSect.bottom+dSpare1, _T("�� �� ��"));

	if(se*lr>0)
		DomSect.Move((rcFront.Width()+rcSect.Width())*se, 0);
	else
		DomSect.Move((rcFront.Width())*se, 0);

	*pDom << DomSect;

	// ��鵵
	CDomyun DomPlane(pDom);

	CString strLeft	= bLeft ? "����" :  "����";
/*	if(bStt)
	{
		strLeft	= bLeft ? "��������" : "��������";
	}
	else
	{
		strLeft	= bLeft ? "��������" : "��������";
	}
*/
	CTwinVector tvEnd, tvOut, tvSlab;
	pBri->m_tvArrPlaneWingWall.GetTvByInfo(strLeft+_T("����������"), tvEnd);
	pBri->m_tvArrPlaneWingWall.GetTvByInfo(strLeft+_T("����������"), tvOut);
	pBri->m_tvArrPlaneWingWall.GetTvByInfo(_T("�����곡�ܿ���"), tvSlab);
	CDPoint vSlab = tvSlab.GetXyDir().Rotate90();
	CDPoint xyMid(0, 0), xyCut(0, 0);
	xyMid = tvEnd.GetXyMid();
	BOOL bCross = GetXyMatchSegAndLine(tvSlab.m_v1, tvSlab.m_v2, xyMid, vSlab, xyCut);
	double dCutLen = bCross ? ~(xyCut-tvOut.m_v1) : 1500;
	if(dCutLen<1500)
		dCutLen = 1500;

	pBriR->GetTvArrayRebarWingWallHunchWallPlane(tvArrHu, bStt, bLeft, TRUE);
	xyHu1 = tvArrHu.GetAt(0).m_v1;
	xyHu2 = tvArrHu.GetAt(2).m_v2;
	CDPoint vHu1 = tvArrHu.GetAt(0).GetXyDir();
	CDPoint vHu2 = tvArrHu.GetAt(2).GetXyDir();
	if(!bSide) GetXyMatchLineAndLine(xyHu1, vHu1, xyHu2, vHu2, xyHu1);
	dSpare = max(~(xyHu1-xyHu2) + dCoverWingOut*2, dCutLen);
	if(pWing->m_bAddRebarReinforce && !bSide)
	{
		CDPoint xyRf[3];
		pBri->GetXyRebarWingWallReinforce(xyRf, bStt, bLeft);
		dSpare = max(~(xyRf[0]-xyRf[1])+200, dSpare);
	}


	m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_CONC);
	DBStd.m_pDrawWingWall->DrawGenCutPlane(&DomPlane, bStt, bLeft, dSpare, TRUE);

	m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_BARC);
	DBStd.m_pDrawWingWall->DrawRebarPlane(&DomPlane, bStt, bLeft);

	DBStd.m_pDrawWingWall->MarkRebarPlaneUpper(&DomPlane, bStt, bLeft, 0);
	DBStd.m_pDrawWingWall->MarkRebarPlaneUpper(&DomPlane, bStt, bLeft, 1);

	CDRect rcPlane = DomPlane.GetExtRect();

	m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_SMALL_TITLE);
	DomPlane.SetTextAlignHorz(TA_CENTER);
	DomPlane.SetTextHeight(7.5);
	DomPlane.TextOut(rcPlane.CenterPoint().x, rcPlane.bottom+dSpare1, _T("�� �� ��"));

	CDPoint xyMove = pBri->GetXySectionWingWallStt(bStt, TRUE);
	xyMove.y = rcFront.bottom - rcPlane.top + dSpare2;
	DomPlane.Move(xyMove);

	*pDom << DomPlane;
	//pBri->m_nTypeJongdanForRebar = nTypeJongdanForRebarOrg;
	pBri->GetTvVertSection_Std(nLeft, TRUE, FALSE, TRUE);

	pDom->SetCalcExtRect();
	CDRect rc = pDom->GetExtRect();

	return rc;
}

CDRect CInputRebarJewon::DrawMonument(CDomyun *pDom)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = m_pIn->GetRcBridgeRebar();
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	pDom->SetScaleDim(20);
	CDomyun DomFront(pDom);
	CDomyun DomPlane(pDom);

	double dSpace1 = 200.0;
	double dSpace2 = 300.0;

	DBStd.m_pDrawCrossSection->DrawMonumentFront(&DomFront);
	DBStd.m_pDrawCrossSection->DrawRebarMonumentFront(&DomFront);
	DBStd.m_pDrawCrossSection->MarkRebarMonuFront(&DomFront, FALSE);

	CDRect rcFront = DomFront.GetExtRect();

	m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_SMALL_TITLE);
	DomFront.SetTextAlignHorz(TA_CENTER);
	DomFront.SetTextHeight(5);
	DomFront.TextOut(rcFront.CenterPoint().x, rcFront.bottom+dSpace1, _T("�� �� ��"));

	DomFront.SetCalcExtRect();
	CDRect rect= DomFront.GetExtRect();
	DomFront.SetLineColor(-1);
	DomFront.Point(rect.left-rect.Width()/15,rect.bottom+rect.Height()/15);

	*pDom << DomFront;

	DBStd.m_pDrawCrossSection->DrawMonumentPlane(&DomPlane);
	DBStd.m_pDrawCrossSection->DrawRebarMonumentPlane(&DomPlane);
	DBStd.m_pDrawCrossSection->MarkRebarMonuPlane(&DomPlane, FALSE);

	CDRect rcPlane = DomPlane.GetExtRect();

	m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_SMALL_TITLE);
	DomPlane.SetTextAlignHorz(TA_CENTER);
	DomPlane.SetTextHeight(5);
	DomPlane.TextOut(rcPlane.CenterPoint().x, rcPlane.bottom+dSpace1, _T("�� �� ��"));

	DomPlane.Move(rcFront.Width()+dSpace2, rcFront.bottom-rcPlane.bottom);

	DomPlane.SetCalcExtRect();
	rcPlane= DomPlane.GetExtRect();
	DomPlane.SetLineColor(-1);
	DomPlane.Point(rcPlane.right+rcPlane.Width()/15,rect.top-rect.Height()/15);

	*pDom << DomPlane;

	return rect;
}

// long nPosDetail : �󼼰� ���� ��ġ
// 0 : ����, 1 : ��, 2 : ��, 3 : ��
void CInputRebarJewon::DrawRebarDetail(CDomyun *pDom, CDRect rect, double dScale, long nPosDetail)
{
	CDomyun Dom(pDom);
	CConcSymbol symDB(m_pIn->m_pARoadOptionStd);
	
	Dom.SetScaleDim(dScale);
	CCellID cID = m_Grid.GetFocusCell();
	long nType = m_Grid.GetTextMatrixLong(cID.row, TYPE_COL);
	long nJoint = m_Grid.GetTextMatrixLong(cID.row, JOINT_COL);
	CObInsert *pIns = symDB.RebarDetailBasic(&Dom,nType,CDPoint(0,0),nJoint>0);
	
	// R1, R2, C1, C2���� ������ �־��ش� ////////
	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols, TRUE);
	structGridCell *pCell	= m_Grid.GetCell(cID.row, nCols-1);
	
	if(pCell)
	{
		long nIdx	= cID.row-1;
		CRebar *pRB	= m_pArrRebar.GetSize() > nIdx ? m_pArrRebar.GetAt(cID.row-1) : NULL;
		if(pRB)
		{
			CString c1Max = _T("0");
			CString r1Max = _T("0");
			CString c2Max = _T("0");
			CString r2Max = _T("0");

			CString c1Min = _T("0");
			CString r1Min = _T("0");
			CString c2Min = _T("0");
			CString r2Min = _T("0");

			if(pRB->m_dLengthMax_C1 > 0) c1Max.Format("%.0f", pRB->m_dLengthMax_C1);
			if(pRB->m_dLengthMax_R1 > 0) r1Max.Format("%.0f", pRB->m_dLengthMax_R1);
			if(pRB->m_dLengthMax_C2 > 0) c2Max.Format("%.0f", pRB->m_dLengthMax_C2);
			if(pRB->m_dLengthMax_R2 > 0) r2Max.Format("%.0f", pRB->m_dLengthMax_R2);	

			c1Min.Format("%.0f", pRB->m_dLengthMin_C1);
			if(c1Max != c1Min) c1Max += ("~"+c1Min);

			r1Min.Format("%.0f", pRB->m_dLengthMin_R1);
			if(r1Max != r1Min) r1Max += ("~"+r1Min);

			c2Min.Format("%.0f", pRB->m_dLengthMin_C2);
			if(c2Max != c2Min) c2Max += ("~"+c2Min);
			
			r2Min.Format("%.0f", pRB->m_dLengthMin_R2);
			if(r2Max != r2Min) r2Max += ("~"+r2Min);
			
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "C1", c1Max);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "R1", r1Max);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "C2", c2Max);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "R2", r2Max);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "c1", c1Max);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "r1", r1Max);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "c2", c2Max);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "r2", r2Max);
		}
	}
	///////////////////////////////////////////////

	Dom.SetCalcExtRect();
	CDRect rectCur	= Dom.GetExtRect();

	double dGap	= Dom.Always(20);
	switch(nPosDetail)
	{
	case 0:	// ��
		Dom.SetPosition(rect.right+dGap, rect.top+rect.Height()/2-rectCur.Height()/2);
		break;
	case 1: // ��
		Dom.SetPosition(rect.left+rect.Width()/2-rectCur.Width()/2, rect.bottom+dGap);
		break;
	case 2:	// ��
		Dom.SetPosition(rect.left-dGap-rectCur.Width(), rect.top+rect.Height()/2-rectCur.Height()/2);
		break;
	case 3: 
		Dom.SetPosition(rect.left+rect.Width()/2+rectCur.Width()/2, rect.top-rectCur.Height()-dGap);
		break;
	default: break;
	}
	
	*pDom << Dom;
}

void CInputRebarJewon::DrawInputDomyunView(BOOL bZoomAll)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);	

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	if(m_Grid.GetFocusCell().row<1)
	{
		CInputDialogBase::DrawInputDomyunView(bZoomAll);
		return;
	}

	CDRect rect(0, 0, 0, 0);

	CDomyun Dom(pDom);

	CRebar* pRB		 = NULL;
	CWingWall* pWing = NULL;
	CWallApp *pWall	 = NULL;
//	long nIdxWall	 = 0;
	long nOriginTab	 = GetTabCurOriginSel();

	CCellID cID  = m_Grid.GetFocusCell();
	int nRBIndex = m_Grid.GetTextMatrixInt(cID.row, REBAR_INDEX_COLUMN);
	int nOut(0);

	// ���� ���õ� row�� ���� CRebar *�� ������ added by jaeho 2005.1.5
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(!pCell) return;
	CString sMark	= m_Grid.GetTextMatrix(pCell->nRow, 2);
	CString sTitle	= m_Grid.GetTextMatrix(pCell->nRow, 1);

	long nPosDetail	= 2;
	long nCycle		= 0;

	switch(nOriginTab)
	{
	case 0:	// ��ö��
		// ����Ŭ �Ǵ� �ϴ� ����� ���� �����̴�. �� �� ���õǰ� �ٲ� �ʿ� ����.
		if(sTitle.Find("��",0)!= -1) nCycle			= 0;
		else if(sTitle.Find("��",0)!= -1) nCycle	= 1;
		else if(sTitle.Find("��",0)!= -1) nCycle	= 2;
		else if(sTitle.Find("��",0)!= -1) nCycle	= 3;
		rect	= DrawRebarMain_VertSection(&Dom, nCycle, sMark);
		DrawRebarDetail(&Dom, rect, 150, 1);
		break;
	case 1:	// ��Ÿö��
		if(sTitle.Find("�����", 0) != -1)
		{
			BOOL bStt	= TRUE;
			if(sTitle.Find("����", 0) != -1)
				bStt	= FALSE;
			rect	= DrawRebarBracket_VertSection(&Dom, bStt, sMark);
		}
		else if(sTitle.Find("�찢��", 0) != -1)
		{
			rect	= DrawRebarMain_VertSection(&Dom, 0, sMark);
		}
		else
		{
			rect	= DrawRebarSupport_VertSection(&Dom, sMark);
			nPosDetail	= 1;
		}
		DrawRebarDetail(&Dom, rect, 150, nPosDetail);
		break;
	case 2:	// ��, ���
		if(sTitle.Find("�Ŵ���", 0) != -1)
		{
			// ���� �Ǵ��ϴ� ����� ���� �����̴�. �� �� ���õǰ� �ٲ� �ʿ� ����. 
			long nJijum	= 0;
			long nFind	= sTitle.Find("�߰�����", 0);
			char a[1]	= {sTitle.GetAt(nFind+8)};
			nJijum	= atoi(a);
			rect	= DrawRebarGirder_VertSection(&Dom, nJijum, sMark);
			DrawRebarDetail(&Dom, rect, 150, nPosDetail);
		}
		else
		{
			long nIdxWall	 = 0;
			pWall	= pBri->GetInWall(nIdxWall);
			if(pWall)
			{
				if(sTitle.Find("��ö��", 0) != -1)
				{
					long nDan	= 0;
					if(sTitle.Find("3��", 0) != -1) nDan = 2;
					else if(sTitle.Find("2��", 0) != -1) nDan = 1;
					else nDan = 0;
									
					DBStd.m_pDrawVertSection->DrawDetail_Column_VertSection(&Dom, nIdxWall);
					DBStd.m_pDrawColumn->DrawRebarMain_Front(&Dom, &pWall->m_Column, FALSE, nDan, FALSE, TRUE, FALSE, TRUE, FALSE); 
				}
				else if(sTitle.Find("��ö��", 0) != -1 || sTitle.Find("ũ�ν�Ÿ��", 0) != -1)
				{
					if(pWall->m_bTopIsColumn)
					{
						DBStd.m_pDrawColumn->DrawRebar_PlaneStd(&Dom, pWall, FALSE, FALSE, TRUE, FALSE);
					}
				
				}
				Dom.SetCalcExtRect();
				rect	= Dom.GetExtRect();
				DrawRebarDetail(&Dom, rect, 150);
			}
		}
		break;
	case 3:
		pWing = pBri->GetWingWall(TRUE, TRUE);
		if(nRBIndex >= 0)
		{
			pRB = pWing->m_pArrRebar[nRBIndex];
			nOut = pRB->m_nPos==REBAR_POS_WING_FRONT ? 1 : 0;
		}
		rect = DrawWingWall(&Dom, TRUE, TRUE, nOut);
		DrawRebarDetail(&Dom, rect, 150);
		break;
	case 4:
		pWing = pBri->GetWingWall(TRUE, FALSE);
		if(nRBIndex >= 0)
		{
			pRB = pWing->m_pArrRebar[nRBIndex];
			nOut = pRB->m_nPos==REBAR_POS_WING_FRONT ? 1 : 0;
		}
		rect = DrawWingWall(&Dom, TRUE, FALSE, nOut);
		DrawRebarDetail(&Dom, rect, 150);
		break;
	case 5:
		pWing = pBri->GetWingWall(FALSE, TRUE);
		if(nRBIndex >= 0)
		{
			pRB = pWing->m_pArrRebar[nRBIndex];
			nOut = pRB->m_nPos==REBAR_POS_WING_FRONT ? 1 : 0;
		}
		rect = DrawWingWall(&Dom, FALSE, TRUE, nOut);
		DrawRebarDetail(&Dom, rect, 150);
		break;
	case 6:
		pWing = pBri->GetWingWall(FALSE, FALSE);
		if(nRBIndex >= 0)
		{
			pRB = pWing->m_pArrRebar[nRBIndex];
			nOut = pRB->m_nPos==REBAR_POS_WING_FRONT ? 1 : 0;
		}
		rect = DrawWingWall(&Dom, FALSE, FALSE, nOut);
		DrawRebarDetail(&Dom, rect, 150);
		break;
	case 7:
		DrawRebarDetail(&Dom, rect, 15);
		break;
	case 8:
		rect = DrawHyunTa(&Dom);
		DrawRebarDetail(&Dom, rect, 100);
		break;
	case 9:
		CString str = m_Grid.GetTextMatrix(cID.row,0);
		if(str.Find(_T("������")) > 0)
			rect = DrawMonument(&Dom);
		else
			rect = DrawGuardWall(&Dom, FALSE);			
		DrawRebarDetail(&Dom, rect, 20);
		break;
	}

	*pDom << Dom;

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputRebarJewon::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell)
	{
		m_pIn->SetComment(pCell->sTitle);
		
		m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(pCell->nRow, 2));

		m_pIn->SetComment("ö�� ��ġ�� �������� ��쿡�� �ǰ��(��ü �ǰ��)�� �����ؼ� ö�����踦 �缳�� �ؾ� �˴ϴ�.");
	}
}

void CInputRebarJewon::SetControl()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	int nOriginTab = GetTabCurOriginSel();

	BOOL bEnable = (nOriginTab != 1 && nOriginTab != 7 && nOriginTab != 8 && nOriginTab != 9) ? TRUE : FALSE; // ���,����Ÿ��,���ӽ�����,��Ÿö��
	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	GetDlgItem(IDC_BUTTON_ADD_REBAR)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_REMOVE_REBAR)->EnableWindow(bEnable);

	GetDlgItem(IDC_CHECK_VIEW_LSDINFO)->ShowWindow(bLsd ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BUTTON_JOINT)->ShowWindow(bLsd ? SW_SHOW : SW_HIDE);
	if(pBri->m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_bViewLsd = FALSE;

	m_checkViewlsd.SetCheck(m_bViewLsd);
}

void CInputRebarJewon::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	CCellID cell = m_Grid.GetFocusCell();
	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
	
	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)pNotifyStruct;	
	long nRow = pGridView->iRow;
	long nCol = pGridView->iColumn;

	// (AAB-5596) ö�� ������ ����ɶ� m_dEa�� ����ϰ� �ִٸ� ���� �ٲ��ش�.
	CString sTitle	= m_Grid.GetTextMatrix(0, nCol);
	long nIdx	= nRow-1;

	if(sTitle == hggettext("����"))
	{
		CRebar *pRB	= m_pArrRebar.GetSize() > nIdx ? m_pArrRebar.GetAt(nIdx) : NULL;
		if(pRB && pRB->m_dEa > 0)
		{
			pRB->m_dEa = (double)pRB->m_nEa;
		}
	}
	else if(sTitle == _T("����")) //������ �׸��忡���� ������ ��ȭ�� �� ����.
	{
		CRebar *pRB	= m_pArrRebar.GetSize() > nIdx ? m_pArrRebar.GetAt(nIdx) : NULL;
		
		CString sDia(_T(""));

		CString sValUser = m_Grid.GetTextMatrix(nRow, nCol);
		for (long ix =1; ix < sValUser.GetLength(); ix++)
		{
			char cVal = sValUser.GetAt(ix);
			sDia += cVal;
		}
		double dDiaUse = atof(sDia);
		pRB->m_dDia = dDiaUse;

		
	}
}

void CInputRebarJewon::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
	DrawInputDomyunView();
}


// ��ö��(���ܸ�) �� ���ؼ� ö�� �� �׸���
CDRect CInputRebarJewon::DrawRebarMain_VertSection(CDomyun *pDomP, long nCycle, CString sMark)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = m_pIn->GetRcBridgeRebar();
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);	
	DBStd.SetMarkForMarkRebar(sMark);

	CDomyun Dom(pDomP);

	//pBri->SyncBridge(0, !pBri->IsVertDir());
	pBri->GetTvVertSection_Std(0, !pBri->IsVertDir(), FALSE);
	pBri->SyncBridgeRebar(FALSE, 0);
	DBStd.m_pDrawVertSection->DrawVertSectionAll(&Dom, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);
	DBStd.m_pDrawVertSection->DrawRebarMain_VertSectionAll(&Dom, nCycle, TRUE, FALSE, TRUE, 0, FALSE);

	Dom.SetCalcExtRect();
	*pDomP << Dom;
	pBri->GetTvVertSection_Std(0, !pBri->IsVertDir(), FALSE, TRUE);
	DBStd.SetMarkForMarkRebar("");
	return Dom.GetExtRect();
}

// ���ö��(���ܸ�)�� ���ؼ� ö�� �� �׸���
CDRect CInputRebarJewon::DrawRebarSupport_VertSection(CDomyun *pDomP, CString sMark)
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
	DBStd.SetMarkForMarkRebar(sMark);

	CDomyun Dom(pDomP);

	pBri->MakeRebarInfoCycleStd(FALSE);
	DBStd.m_pDrawVertSection->DrawStandardSectionStd(&Dom, FALSE, TRUE, TRUE);

	Dom.SetCalcExtRect();
	*pDomP << Dom;

	DBStd.SetMarkForMarkRebar("");
	return Dom.GetExtRect();
}

// ������ �Ŵ��� ö�� �� �׸�.
CDRect CInputRebarJewon::DrawRebarGirder_VertSection(CDomyun *pDomP, long nIdxJijum, CString sMark)
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	DBStd.SetMarkForMarkRebar(sMark);

	double dScale	= 50;
	CDomyun Dom(pDomP);
	
	Dom.SetScaleDim(dScale);

	long nIdxInWall	= nIdxJijum-1;
	DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(&Dom, nIdxInWall, FALSE, TRUE, -1, FALSE, FALSE);
	DBStd.m_pDrawVertSection->DrawDetail_Girder(&Dom, nIdxInWall);
	Dom.RedrawByScale(100/dScale);
	Dom.SetCalcExtRect();
	*pDomP << Dom;

	DBStd.SetMarkForMarkRebar("");
	return Dom.GetExtRect();
}

// ������ ����� ö��
CDRect CInputRebarJewon::DrawRebarBracket_VertSection(CDomyun *pDomP, BOOL bStt, CString sMark)
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	DBStd.SetMarkForMarkRebar(sMark);

	double dScale	= 50;
	CDomyun Dom(pDomP);
	
	Dom.SetScaleDim(dScale);

	DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_Bracket(&Dom, bStt, FALSE, TRUE, FALSE, 0, CDRect(0, 0, 0, 0), 0);
	DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_Bracket(&Dom, bStt, FALSE, TRUE, FALSE, 0, CDRect(0, 0, 0, 0), 1);
	DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_Bracket(&Dom, bStt, FALSE, TRUE, FALSE);
	DBStd.m_pDrawVertSection->DrawRebarShear_VertSection_Bracket(&Dom, bStt, FALSE, TRUE, FALSE);


	Dom.RedrawByScale(100/dScale);
	Dom.SetCalcExtRect();
	*pDomP << Dom;

	DBStd.SetMarkForMarkRebar("");
	return Dom.GetExtRect();
}

CDRect CInputRebarJewon::DrawHyunTa(CDomyun *pDomP)
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	CFootingApp *pFooting	= pBri->GetFooting(0);
	if(!pFooting) return CDRect(0, 0, 0, 0);
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;

	CDrawExPile drawExPile(pPile, m_pIn->m_pARoadOptionStd);

	CDRect rcSect;
	double dScale	= 50;
	CDomyun Dom(pDomP);

	Dom.SetScaleDim(dScale);
	
	drawExPile.DrawPlanDetail(&Dom);
	drawExPile.DrawPlanRebarCrossTieDetail(&Dom);
	drawExPile.DrawSectionPlanRebarMain_D(&Dom, 0);		// ����° ������ dDepth ���.. 

	Dom.RedrawByScale(100/dScale);
	Dom.SetCalcExtRect();
	rcSect = Dom.GetExtRect();

	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SMALL_TITLE);
	Dom.SetTextAlignHorz(TA_CENTER);
	Dom.SetTextHeight(7.5);
	Dom.TextOut(rcSect.CenterPoint().x, rcSect.bottom+500, _T("�� �� ��"));

	*pDomP << Dom;

	dScale	= 100;
//	Dom.SetScaleDim(dScale);

	drawExPile.DrawFrontDetail_D(&Dom, 1);
	drawExPile.DrawFrontRebarMainDetail_D(&Dom, 1);
	drawExPile.DrawFrontRebarRoundDetail_D(&Dom, 0, 1);

	Dom.RedrawByScale(100/dScale);

	CDRect rcTemp = Dom.GetExtRect();

	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SMALL_TITLE);
	Dom.SetTextAlignHorz(TA_CENTER);
	Dom.SetTextHeight(7.5);
	Dom.TextOut(rcTemp.CenterPoint().x, rcTemp.bottom+500, _T("�� �� ��"));

	Dom.Move(rcSect.right-rcTemp.left+Dom.Always(10), -rcTemp.Height()/2);

	*pDomP << Dom;

	return  rcSect;
}

int CInputRebarJewon::GetTabCurOriginSel()
{
	int nTab = m_Tab.GetCurSel();
	TCITEM tcItem;
	char cTabName[256];
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = cTabName;
    tcItem.cchTextMax = 255;
	m_Tab.GetItem(nTab, &tcItem);
	CString strTab = tcItem.pszText;

	int nIndex = 0;
	if(strTab==_T("��ö��(��ü)"))
		nIndex = 0;
	if(strTab==_T("��� �� ���� ö��(��ü)"))
		nIndex = 1;
	if(strTab==_T("��, ���"))
		nIndex = 2;
	if(strTab==_T("������ ���� ����"))
		nIndex = 3;
	if(strTab==_T("������ ���� ����"))
		nIndex = 4;
	if(strTab==_T("������ ���� ����"))
		nIndex = 5;
	if(strTab==_T("������ ���� ����"))
		nIndex = 6;
	if(strTab==_T("���� ������"))
		nIndex = 7;
	if(strTab==_T("���� Ÿ�� ����"))
		nIndex = 8;
	if(strTab==_T("��Ÿ ö��"))
		nIndex = 9;

	return nIndex;
}

// ��ȣ�� �� �ߺд� ö���� �� ����
// int CInputRebarJewon::GetCountRebarGuardWall()
// {
// 	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
// 
// //	CGuardWallRC* pGW = NULL;
// 
// 	int nCountRebar = 0;
// 	int nQtyHDan = pBri->GetQtyHDan();
// 	for(long nHDan=0; nHDan<nQtyHDan; nHDan++)
// 	{
// 		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
// 		if(!pGW) continue;
// 		if(pGW->IsTypeHDanRoadTotalGuardFence())
// 		{
// 			int j=0; for(j=0; j<pGW->m_pArrRebar.GetSize(); j++)
// 			{
// 				if(pGW->m_pArrRebar.GetAt(j)->IsTrue())
// 					nCountRebar++;
// 			}
// 		}
// 	}
// 
// 	return nCountRebar;
// }

// �׸��� row�� ���� CGuardWallRC�� ���� �´�.
CGuardWallRC* CInputRebarJewon::GetGuardWallByRow(int nRow)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	CGuardWallRC* pGW = NULL;

	int nCountRebar = 0;
	int nQtyHDan = pBri->GetQtyHDan();
	int i=0; for(i=0; i<nQtyHDan; i++)
	{
		pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadTotalGuardFence())
		{
			int j=0; for(j=0; j<pGW->m_pArrRebar.GetSize(); j++)
			{
				if(pGW->m_pArrRebar.GetAt(j)->IsTrue())
					nCountRebar++;
			}
			if(nRow<=nCountRebar)
				break;
		}
	}

	return pGW;
}

// �׸��� row�� ���� Ⱦ�� ������ ���� �´�.
int CInputRebarJewon::GetHDanByRow(int nRow)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

//	CGuardWallRC* pGW = NULL;

	int nCountRebar = 0;
	int nQtyHDan = pBri->GetQtyHDan();
	int i=0; for(i=0; i<nQtyHDan; i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadTotalGuardFence())
		{
			int j=0; for(j=0; j<pGW->m_pArrRebar.GetSize(); j++)
			{
				if(pGW->m_pArrRebar.GetAt(j)->IsTrue())
					nCountRebar++;
			}
			if(nRow<=nCountRebar)
				break;
		}
	}

	return i;
}

// ��ü �ǰ��
void CInputRebarJewon::OnButtonRecommandAll() 
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox("���� ������ ��� �������� ���ؼ� ö������ �ǰ���� ���� �մϴ�. ��� �Ͻðڽ��ϱ�?  ", MB_YESNO | MB_ICONQUESTION)==IDNO)
		return;

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	pBri->SetDefaultedState(GetIDD(), 0x00000000);
	SetDataDefault();
}

// ���ڿ����� ���ڸ� �и�
double CInputRebarJewon::GetNumberInString(CString str) 
{
	CString strNumber(_T(""));
	long i = 0; for(i = 0; i< str.GetLength(); i++)
	{
		char ch = str.GetAt(i);
		if(ch >= '0' && ch <= '9') 
			strNumber += ch;
	}
	double dNumber = atof(strNumber);

	return dNumber;
}

// ������ ����
void CInputRebarJewon::OnButtonAddrate() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	if(m_pIn->m_pDlgAddRateSheet)
	{
		CSafetyTypedPtrArray <CObArray, CRebar*> pArrRebar;
		pBri->MakeRebarList(&pArrRebar, -1, TRUE, TRUE, 0, TRUE);

		CDlgAddRateSheet *pSheet	= m_pIn->m_pDlgAddRateSheet;
		pSheet->m_nIdxPage	= -1;
		pSheet->SetActivePage(pBri->m_nTypeAddRate);
		pSheet->m_dDiaOrg		= pBri->m_dAddRateDiaOrg;
		pSheet->m_dRateLower	= pBri->m_dAddRateLower;
		pSheet->m_dRateUpper	= pBri->m_dAddRateUpper;
		pSheet->m_pArrRebar		= &pArrRebar;

		if(pSheet->DoModal())
		{
			SetGridData();
			SetDataInit();

			pBri->m_dAddRateDiaOrg	= pSheet->m_dDiaOrg;
			pBri->m_dAddRateLower	= pSheet->m_dRateLower;
			pBri->m_dAddRateUpper	= pSheet->m_dRateUpper;
			pBri->m_nTypeAddRate	= pSheet->m_nIdxPage;
		}
	}
}

void CInputRebarJewon::OnButtonAddRebar()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	double dFy = pBri->m_pARcBridgeDataStd->m_Fy;
	CSafetyTypedPtrArray <CObArray, CRebar*> *pArrRebar;

	int nOriginTab = GetTabCurOriginSel();
	switch(nOriginTab)
	{
	case 0:
		pArrRebar = &pBri->m_pArrRebarMain_User;
		dFy = pBri->m_Fy[ePartUpperSlab];
		break;
	case 2:
		pArrRebar = &pBri->m_pArrRebarGirder_User;
		dFy = pBri->m_Fy[ePartUpperSlab];
		break;
	case 3:
		pArrRebar = &pBri->m_pWingWallStt[0]->m_pArrRebar_User;
		dFy = pBri->m_Fy[ePartWingWallStt];
		break;
	case 4:
		pArrRebar = &pBri->m_pWingWallStt[1]->m_pArrRebar_User;
		dFy = pBri->m_Fy[ePartWingWallStt];
		break;
	case 5:
		pArrRebar = &pBri->m_pWingWallEnd[0]->m_pArrRebar_User;
		dFy = pBri->m_Fy[ePartWingWallEnd];
		break;
	case 6:
		pArrRebar = &pBri->m_pWingWallEnd[1]->m_pArrRebar_User;
		dFy = pBri->m_Fy[ePartWingWallEnd];
		break;
	default:
		break;
	}

	CHgBaseConcStd baseConc;
	dFy = baseConc.GetInputRebarFy(dFy, pBri->m_pARcBridgeDataStd->m_szKgf_CM2, UNIT_CGS_KGF_CM2);
	if(dFy == 0)
		return;

	long nSize = pArrRebar->GetSize();
	long nDescription = 0;
	if(nSize > 0)
	{
		CRebar *pRB2 = pArrRebar->GetAt(nSize-1);
		nDescription = atol(pRB2->m_sDescription.Right(2));
	}

	CRebar *pRB  = new CRebar;
	pRB->m_sDescription.Format(hggettext("�߰�ö�� %02d"), nDescription + 1);
	pRB->m_nPos			= REBAR_POS_ADD_USER;
	pRB->m_nTypeRebar	= 1;
	pRB->m_Fy			= dFy;
	pRB->m_dDia			= 25;
	pRB->m_sMark.Format(_T("UA%d"), nDescription + 1);
	pRB->m_dLengthMax_A = 1000;
	pRB->m_nEa			= 1;
	pRB->m_dRateAdd		= 1.03;
	pRB->m_bLock		= TRUE;
	pArrRebar->Add(pRB);

	SetGridData();
	SetDataInit();
}

void CInputRebarJewon::OnButtonRemoveRebar()
{
	structGridCell *pCell	= m_Grid.GetCurrentCell();
	if(!pCell) return;

	long nCol = 1;
	CString sDescription	= m_Grid.GetTextMatrix(pCell->nRow, nCol);
	sDescription.Replace(_T(" "), _T(""));

	if(sDescription.Find(_T("�߰�ö��")) == -1)
	{
		AfxMessageBox(_T("����� �߰�ö���� �ƴ� ��� ������ �� �����ϴ�."));
		return;
	}

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CSafetyTypedPtrArray <CObArray, CRebar*> *pArrRebar;

	int nOriginTab = GetTabCurOriginSel();
	switch(nOriginTab)
	{
	case 0:
		pArrRebar = &pBri->m_pArrRebarMain_User;
		break;
	case 2:
		pArrRebar = &pBri->m_pArrRebarGirder_User;
		break;
	case 3:
		pArrRebar = &pBri->m_pWingWallStt[0]->m_pArrRebar_User;
		break;
	case 4:
		pArrRebar = &pBri->m_pWingWallStt[1]->m_pArrRebar_User;
		break;
	case 5:
		pArrRebar = &pBri->m_pWingWallEnd[0]->m_pArrRebar_User;
		break;
	case 6:
		pArrRebar = &pBri->m_pWingWallEnd[1]->m_pArrRebar_User;
		break;
	default:
		break;
	}

	CRebar *pRB	= NULL;
	CString sDescTmp	= _T("");
	long nSize = pArrRebar->GetSize();
	for(long i = 0; i < nSize; i++)
	{
		pRB	= pArrRebar->GetAt(i);
		sDescTmp	= pRB->m_sDescription;
		sDescTmp.Replace(_T(" "), _T(""));
		if(sDescTmp == sDescription)
		{
			delete pRB;
			pArrRebar->RemoveAt(i);
			break;
		}
	}

	SetGridData();
	SetDataInit();
}

void CInputRebarJewon::OnBnClickedCheckViewLsdinfo()
{
	m_checkViewlsd.UpdateData(FALSE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bViewLsd = m_checkViewlsd.GetCheck();
	SetGridData();
	SetDataInit();
}

void CInputRebarJewon::SetStringDia()
{
	m_sArrDia.RemoveAll();

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	if(pBri->m_pARcBridgeDataStd->IsLsdDesign())
	{
		pBri->m_plsdRebar->GetStringArrDia(m_sArrDia);
	}
}

void CInputRebarJewon::OnButtonDetJoint()
{
	long nIdxRebar = m_Grid.GetCurrentCell()->nRow-1;
	if(nIdxRebar < 0 || nIdxRebar >= m_pArrRebar.GetSize()) return;

	CRebar *pRebar = m_pArrRebar.GetAt(nIdxRebar);
	if(pRebar == NULL) return;

	CDialogLsdJointDetail Dlg(*pRebar->m_pOptionLsd);
	Dlg.SetRebarInfo(pRebar->m_dDia, pRebar->m_sMark, pRebar->m_sDescription, pRebar->m_nPos);
	Dlg.DoModal();
}
