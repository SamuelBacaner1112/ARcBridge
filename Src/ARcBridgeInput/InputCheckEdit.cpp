// InputCheckEdit.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../../Src/ARcBridgeCalculate/ARcBridgeCalculate.h"
#include "InputCheckEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputCheckEdit dialog


CInputCheckEdit::CInputCheckEdit(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CInputCheckEdit::IDD, pParent)
{
	m_pBri	= NULL;
	m_pCalc = NULL;
	m_nInputIDD = -1;
	m_nTypeCheck = -1;
} 

CInputCheckEdit::CInputCheckEdit(CRcBridgeRebar *pBri, long nInputIDD, long nTypeCheck, CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CInputCheckEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputCheckEdit)
	m_szCheckEdit = _T("");
	//}}AFX_DATA_INIT
	m_pBri		 = pBri;
	m_nInputIDD  = nInputIDD;
	m_nTypeCheck = nTypeCheck;
	m_pCalc = new CARcBridgeCalcStd(pBri);
}

CInputCheckEdit::~CInputCheckEdit()
{
	delete m_pCalc;
}

void CInputCheckEdit::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputCheckEdit)
	DDX_Control(pDX, IDC_TAB2, m_ctlTab);
	DDX_Control(pDX, IDC_EDIT_CHECK, m_RichEdit);
	DDX_Text(pDX, IDC_EDIT_CHECK, m_szCheckEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputCheckEdit, CXTResizeDialog)
	//{{AFX_MSG_MAP(CInputCheckEdit)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB2, OnTabSelchanging)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputCheckEdit message handlers


BOOL CInputCheckEdit::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetResize(IDC_EDIT_CHECK,	 SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB2,	 SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	CenterWindow();
	
	InitTab();
	SetDataInit();
	
	ShowDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputCheckEdit::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	Calculate();
	ReadOutFile();
	return;
}

void CInputCheckEdit::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	return;
}

void CInputCheckEdit::InitTab()
{
	m_ctlTab.SendInitialUpdate(TRUE);
	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	m_ctlTab.RemoveAllTabs();
	
	switch(m_nInputIDD)
	{
	case IDD_DIALOG_HYUNTA :
		{
			SetWindowText("현장타설 말뚝 검토");
			m_ctlTab.InsertItem(0, "축력 최대");
			m_ctlTab.InsertItem(1, "모멘트 최대");
			m_ctlTab.InsertItem(2, "편심 최대");
			break;
		}
	case IDD_REBAR_APS:
		{
			BOOL bStt = (m_nTypeCheck == 0)? TRUE:FALSE;
			SetWindowText("접속 슬래브 검토");
			m_ctlTab.InsertItem(0, "단면검토");
			m_ctlTab.InsertItem(1, "균열검토");
			if((bStt && m_pCalc->m_pApsStt[0]->IsCobel()) || (!bStt && m_pCalc->m_pApsEnd[0]->IsCobel()))
				m_ctlTab.InsertItem(2, "받침부검토");
			break;
		}
	case IDD_REBAR_MAIN3:
		{
			SetWindowText("헌치 및 기타 철근 배치");
			m_ctlTab.InsertItem(0, "받침부검토(시점부)");
			m_ctlTab.InsertItem(1, "받침부검토(종점부)");
			break;
		}
	case IDD_REBAR_COLUMN:
		{
			if(m_nTypeCheck >= MAX_JIJUM_COUNT_RC)
			{
				SetWindowText("종거더 검토");
				m_ctlTab.InsertItem(0, "중앙부");
			}
			else
			{
				SetWindowText("보 검토");
				m_ctlTab.InsertItem(0, "지점부 : 휨");
				m_ctlTab.InsertItem(1, "지점부 : 깊은보");
				m_ctlTab.InsertItem(2, "중앙부");
			}
		}
	default : break;
	}
}

void CInputCheckEdit::SetDataInit()
{

}

void CInputCheckEdit::ShowDlg()
{
	CSize WSize[] = { 
		CSize(700,500), // IDD_DIALOG_HYUNTA
		CSize(700,500), // IDD_GEN_APS
		CSize(700,500), // IDD_REBAR_MAIN3
		CSize(700,500), // IDD_REBAR_COLUMN
	};
	switch(m_nInputIDD)
	{
	case IDD_DIALOG_HYUNTA:		
		SetWindowPos(&wndTop, 0,   0, WSize[0].cx, WSize[0].cy, SWP_NOMOVE | SWP_DRAWFRAME);	break;
	case IDD_REBAR_APS:		
		SetWindowPos(&wndTop, -1, -1, WSize[1].cx, WSize[1].cy, SWP_NOMOVE | SWP_DRAWFRAME);	break;										
	case IDD_REBAR_MAIN3:		
		SetWindowPos(&wndTop, -1, -1, WSize[2].cx, WSize[1].cy, SWP_NOMOVE | SWP_DRAWFRAME);	break;
	case IDD_REBAR_COLUMN:		
		SetWindowPos(&wndTop, -1, -1, WSize[3].cx, WSize[1].cy, SWP_NOMOVE | SWP_DRAWFRAME);	break;
	}
}

int CInputCheckEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTResizeDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	//m_RichEdit.SetFont(GetFont());
	
	return 0;
}

void CInputCheckEdit::ReadOutFile()
{	
	m_RichEdit.Clear();	
	
	CHAR ReadBuff[20000] = "";
	CFile file(m_sFilename,CFile::modeReadWrite|CFile::typeBinary);	
	file.Read(ReadBuff, 20000);	
	m_szCheckEdit = ReadBuff;
	m_szCheckEdit.Replace("","");
	UpdateData(FALSE);
	file.Close();
	
	//parse();
}

void CInputCheckEdit::Check()
{
	if(!GetSafeHwnd()) return;
	Calculate();
	ReadOutFile();
	UpdateData(FALSE);
}

BOOL CInputCheckEdit::GetVerify()
{
	Calculate();
	
	CString szText;	
	CHAR ReadBuff[20000] = "";
	CFile file(m_sFilename,CFile::modeReadWrite|CFile::typeBinary);	
	file.Read(ReadBuff, 20000);	
	szText = ReadBuff;
	szText.Replace("","");	
	file.Close();
	
	return szText.Find("N.G")==-1 ? TRUE : FALSE;
}

void CInputCheckEdit::Calculate()
{
//	CString szPath  = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	char path[MAX_PATH];
	::GetTempPath(MAX_PATH,path);
	CString szPath = path;
	
	CStdioFile OutFile;
	CString OutFileName = szPath + "RcBridgeTextOut.tmp";
	if( OutFile.Open(OutFileName,CFile::modeCreate | CFile::modeWrite ) == FALSE)
	{
		return;
	}

	m_sFilename = OutFileName;
	
	long nTab = m_ctlTab.GetCurSel();
	if(m_nInputIDD == IDD_REBAR_APS)
	{
		CARcBridgeCalcAps *pCalcAsp		= NULL;
		CARcBridgeCalcAps *pCalcAsp2	= NULL;
		
		BOOL bRightCheck = FALSE;
		BOOL bStt = (m_nTypeCheck == 0)? TRUE:FALSE;
		if(bStt)
		{
			pCalcAsp = m_pCalc->m_pApsStt[0];
			bRightCheck = (m_pCalc->m_pApsStt[1])? TRUE : FALSE;
		}
		else
		{
			pCalcAsp =m_pCalc->m_pApsEnd[0];
			bRightCheck = (m_pCalc->m_pApsEnd[1])? TRUE : FALSE;
		}

		if(bRightCheck)
		{
			pCalcAsp2 = bStt? m_pCalc->m_pApsStt[1] : m_pCalc->m_pApsEnd[1];
		}

		pCalcAsp->CalculateAll();
		if(pCalcAsp2) pCalcAsp2->CalculateAll();
		
		CString str = _T("");

		if(nTab == 0) 
		{
			if(bRightCheck)
			{
				str.Format("▶ %s 좌측", bStt? "시점":"종점");
				OutFile.WriteString(str);
			}
			CRectBeam pBeam(pCalcAsp->m_pConc, m_pBri->m_bCheckMainSlabAndFootTemper);
//			pBeam.MakeOutputDesignTextFile(&OutFile, pCalcAsp->m_pAps->m_rebarShearDetail.m_nType!=TYPE_SHEAR_SPACER);
			pBeam.MakeOutputDesignTextFile(&OutFile, TRUE);

			if(bRightCheck)
			{
				str.Format("▶ %s 우측", bStt? "시점":"종점");
				OutFile.WriteString(str);

				CRectBeam pBeam2(pCalcAsp2->m_pConc, m_pBri->m_bCheckMainSlabAndFootTemper);
//				pBeam2.MakeOutputDesignTextFile(&OutFile, pCalcAsp2->m_pAps->m_rebarShearDetail.m_nType!=TYPE_SHEAR_SPACER);
				pBeam2.MakeOutputDesignTextFile(&OutFile, TRUE);
			}
		}
		else if(nTab == 1) 
		{
			if(bRightCheck)
			{
				str.Format("▶ %s 좌측", bStt? "시점":"종점");
				OutFile.WriteString(str);
			}
			CRectBeam pBeam(pCalcAsp->m_pConc, m_pBri->m_bCheckMainSlabAndFootTemper);
			pBeam.MakeOutputCrackDesign(&OutFile);

			if(bRightCheck)
			{
				str.Format("▶ %s 우측", bStt? "시점":"종점");
				OutFile.WriteString(str);

				CRectBeam pBeam2(pCalcAsp2->m_pConc, m_pBri->m_bCheckMainSlabAndFootTemper);
				pBeam2.MakeOutputCrackDesign(&OutFile);
			}
		}
		else if(nTab == 2) 
		{
			CFactor *pFactor = new CFactor;
			pFactor->m_Pi_C = m_pBri->m_pARcBridgeDataStd->m_Pi_C;
			pFactor->m_Pi_V = m_pBri->m_pARcBridgeDataStd->m_Pi_V;
			pFactor->m_Pi_F = m_pBri->m_pARcBridgeDataStd->m_Pi_F;
			pFactor->m_Pi_Cobel = m_pBri->m_pARcBridgeDataStd->m_Pi_F_Cobel;
			pFactor->m_Fck  = m_pBri->m_pARcBridgeDataStd->m_Fck;
			pFactor->m_Fy   = m_pBri->m_pARcBridgeDataStd->m_Fy;
			pFactor->m_Ec   = m_pBri->m_pARcBridgeDataStd->m_Ec;
			pFactor->m_Es   = m_pBri->m_pARcBridgeDataStd->m_Es;

			CCobel pCobel(pCalcAsp->m_pConcCobel_,pFactor,m_pBri->m_pARcBridgeDataStd->m_nTypeUnit);
			pCobel.MakeOutputCobel(&OutFile);

			delete pFactor;
		}
	}
	
	if(m_nInputIDD == IDD_REBAR_MAIN3)
	{
		CARcBridgeCalcAps *pCalcAsp	= NULL;
		
		BOOL bStt = (nTab == 0)? TRUE:FALSE;
		if(bStt) pCalcAsp = m_pCalc->m_pApsStt[0];
		else pCalcAsp =m_pCalc->m_pApsEnd[0];

		if(m_pBri->IsBracket(bStt))
		{
			pCalcAsp->CalculateAll();

			CFactor *pFactor = new CFactor;
			pFactor->m_Pi_C = m_pBri->m_pARcBridgeDataStd->m_Pi_C;
			pFactor->m_Pi_V = m_pBri->m_pARcBridgeDataStd->m_Pi_V;
			pFactor->m_Pi_F = m_pBri->m_pARcBridgeDataStd->m_Pi_F;
			pFactor->m_Pi_Cobel = m_pBri->m_pARcBridgeDataStd->m_Pi_F_Cobel;
			pFactor->m_Fck  = m_pBri->GetValueFck(ePartUpperSlab);
			pFactor->m_Fy   = m_pBri->GetValueFy(ePartUpperSlab);
			pFactor->m_Ec   = m_pBri->GetValueEc(ePartUpperSlab);
			pFactor->m_Es   = m_pBri->m_pARcBridgeDataStd->m_Es;

			CCobel pCobel(pCalcAsp->m_pConcCobel_,pFactor,m_pBri->m_pARcBridgeDataStd->m_nTypeUnit);
			pCobel.MakeOutputCobel(&OutFile);

			delete pFactor;
		}
		else
		{
			CString str = hggettext("검토할 받침부가 없습니다.");
			OutFile.WriteString(str);
		}
	}

	if(m_nInputIDD == IDD_DIALOG_HYUNTA)
	{
		long nTab = m_ctlTab.GetCurSel();
		long nJijum = m_nTypeCheck;

		m_pBri->SetConcData(FALSE,FALSE,TRUE,FALSE,FALSE);
		m_pCalc->m_pFooting->CalculateAll();

		CDesignPile *pCalcPile = m_pCalc->GetDesignPile(nJijum);
		if(pCalcPile && nTab>-1)
		{
			double dMMaxDepth = pCalcPile->m_pPile->m_dH0 + hgatof(pCalcPile->m_sMatHyunta(1,6));
			double dMoment = GetValueUnitChange(atof(pCalcPile->m_sMatHyunta(nTab,DESIGNPILE_HYUNTA_M)),m_pBri->m_pARcBridgeDataStd->m_szTonfM,UNIT_CGS_TONFM);
			double dAxial  = GetValueUnitChange(atof(pCalcPile->m_sMatHyunta(nTab,DESIGNPILE_HYUNTA_P)),m_pBri->m_pARcBridgeDataStd->m_szTonf,UNIT_CGS_TONF);
			CConcBasic *pConc = new CConcBasic;
			pConc->m_Fck = pCalcPile->m_pPile->m_pHyunTa->m_dHP_Fck;
			pConc->m_Fy  = pCalcPile->m_pPile->m_pHyunTa->m_dHP_Fy;
			pConc->m_nTypeUnit = m_pBri->m_pARcBridgeDataStd->m_nTypeUnit;
			pConc->m_nTypeSection = SECTION_COLUMN;
			pConc->m_Ro = pCalcPile->m_pPile->m_dDia/2;
			long i=0; for(i=0; i<REBAR_MAX_LAYER; i++)
			{
				pConc->m_RbT1C_dc[i] = pCalcPile->m_pPile->m_pHyunTa->m_dHP_MainRebarDist[i];
				pConc->m_RbT1C_DIA[i]  = pCalcPile->m_pPile->GetDiaRebarMain(dMMaxDepth,i); // 최대 모멘트 위치로, pCalcPile->m_pPile->m_pHyunTa->m_dHP_MainRebarDia[i];
				pConc->m_RbT1C_RE[i] = pCalcPile->m_pPile->m_pHyunTa->m_dHP_MainRebarEa[i];
			}
			pConc->m_sTitle = pCalcPile->m_sMatHyunta(nTab,5);
			pConc->m_Axial	= fabs(dAxial);
			pConc->m_Mux	= fabs(dMoment);
			CColumn Column(pConc, 0, 2);
			Column.CalculateAll(FALSE);
			Column.MakeOutputText(&OutFile);

			delete pConc;
		}
	}

	if(m_nInputIDD == IDD_REBAR_COLUMN)
	{
		const HGBOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();

		m_pCalc->m_pColGir->CalculateAll();

		if(m_nTypeCheck >= MAX_JIJUM_COUNT_RC)		// 종거더 검토
		{
			long nTypeCheck = m_nTypeCheck - MAX_JIJUM_COUNT_RC;
			long nSize = m_pCalc->m_pColGir->m_pArrConcJongGir.GetSize();
			
			if(nTypeCheck < 0 || nTypeCheck >= nSize)
			{
				OutFile.Close();
				return;
			}
			else
			{
				CConcBasic *pConc = m_pCalc->m_pColGir->m_pArrConcJongGir.GetAt(nTypeCheck);
				if (bLsd)
				{
					CLsdBeam pBeam(pConc, LSD_ULS_DESIGN);
					pBeam.MakeOutputDesignTextFile(&OutFile, TRUE);
				}
				else
				{
					CRectBeam pBeam(pConc, m_pBri->m_bCheckMainSlabAndFootTemper);
					pBeam.MakeOutputDesignTextFile(&OutFile, TRUE);
				}
			}
		}
		else
		{
			CGirderAndColumn *pCalCol = NULL;
			
			long nSize = m_pCalc->m_pColGir->m_pList.GetSize();
			long i=0; for(i=0; i<nSize; i++)
			{
				CGirderAndColumn *pCalCol2 = m_pCalc->m_pColGir->m_pList.GetAt(i);
				if(m_nTypeCheck==pCalCol2->m_nJijum)
				{
					pCalCol = pCalCol2;
					break;
				}
			}
			if(pCalCol==NULL)
			{
				OutFile.Close();
				return;
			}
			if(nTab==0) 
			{
				if (bLsd)
				{
					for (HGINT32 ix = 0; ix < eEnvService; ++ix)
					{
						if(m_pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ix)) == FALSE)
							continue;
						CLsdBeam pBeam(pCalCol->pConcGirEnd[iUPPER][ix], (ix == eEnvLimit)? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);
						pBeam.MakeOutputDesignTextFile(&OutFile, TRUE);
					}
				}
				else
				{
					CRectBeam pBeam(pCalCol->pConcGirEnd[iUPPER][0], m_pBri->m_bCheckMainSlabAndFootTemper);
					pBeam.MakeOutputDesignTextFile(&OutFile, TRUE);
				}
				
			}
			if(nTab==1) 
			{
				for (HGINT32 ix = 0; ix < eEnvService; ++ix)
				{
					if(bLsd == FALSE && ix > 0)
						break;
					if(bLsd && m_pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ix)) == FALSE)
						continue;

					if(pCalCol->pConcGirEnd[iUPPER][ix]->IsDeepBeam())
					{
						CRectBeam pBeam(pCalCol->pConcGirEnd[iUPPER][ix], m_pBri->m_bCheckMainSlabAndFootTemper);
						pBeam.MakeOutputDeepBeamTextFile(&OutFile);
					}
					else
					{
						if (bLsd)
						{
							CLsdBeam pBeam(pCalCol->pConcGirEnd[iUPPER][ix], (ix == eEnvLimit)? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);
							pBeam.MakeOutputDesignTextFile(&OutFile, TRUE);
						}
						else
						{
							CRectBeam pBeam(pCalCol->pConcGirEnd[iUPPER][0], m_pBri->m_bCheckMainSlabAndFootTemper);
							pBeam.MakeOutputDesignTextFile(&OutFile, TRUE);
						}
					}
				}
			}
			if(nTab==2) 
			{
				if (bLsd)
				{
					for (HGINT32 ix = 0; ix < eEnvService; ++ix)
					{
						if(m_pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ix)) == FALSE)
							continue;
						CLsdBeam pBeam(pCalCol->pConcGirMid[iUPPER][ix], ix == eEnvLimit? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);
						pBeam.MakeOutputDesignTextFile(&OutFile, FALSE);	// (ARCBRIDGE-2764) 거더 중앙부는 전단검토 출력안하도록
					}
				}
				else
				{
					CRectBeam pBeam(pCalCol->pConcGirMid[iUPPER][0], m_pBri->m_bCheckMainSlabAndFootTemper);
					pBeam.MakeOutputDesignTextFile(&OutFile, TRUE);	// (ARCBRIDGE-2764) 거더 중앙부는 전단검토 출력안하도록
				}
			}
		}
	}
	
	OutFile.Close();
}


void CInputCheckEdit::SetBriAndContent(CRcBridgeRebar *pBri, long nInputIDD, long nTypeCheck)
{
	m_pBri		 = pBri;
	m_nInputIDD  = nInputIDD;
	m_nTypeCheck = nTypeCheck;
	m_pCalc		 = new CARcBridgeCalcStd(pBri);
}


void CInputCheckEdit::ShowAndCheck(CWnd *pWndParent)
{
	if(!GetSafeHwnd())	
		Create(IDD, pWndParent);
	
	if(m_ctlTab.GetItemCount()>0 && !IsWindowVisible())	m_ctlTab.SetCurSel(0);
	
	int nTab = m_ctlTab.GetCurSel();	

	Calculate();
	ReadOutFile();

	ShowDlg();

	m_ctlTab.SetCurSel(nTab);

	ShowWindow(SW_SHOW);
	SetDataInit();
}