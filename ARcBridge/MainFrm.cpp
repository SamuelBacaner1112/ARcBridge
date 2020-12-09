// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ARcBridge.h"

#include "MainFrm.h"
#include "ARcBridgeDoc.h"
#include "ARcBridgeView.h"
#include "ARcBridgeStmView.h"
#include "../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "../Src/ARcBridgeDrawing/ARcBridgeDrawing.h"
#include "../Src/ARcBridgeDrawingOption/ARcBridgeDrawingOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_UNIT_COMBO 77
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_CONTROLPANE, OnViewControlpane)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CONTROLPANE, OnUpdateViewControlpane)
	ON_COMMAND(ID_VIEW_INPUTPANE, OnViewInputpane)
	ON_UPDATE_COMMAND_UI(ID_VIEW_INPUTPANE, OnUpdateViewInputpane)
	ON_COMMAND(ID_SET_CREATION, OnSetCreation)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_VIEW_COMMENT, OnViewComment)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COMMENT, OnUpdateViewComment)
	ON_COMMAND(ID_APP_HOME, OnAppHome)
	ON_COMMAND(ID_APP_QNA, OnAppQna)
	ON_COMMAND(ID_APP_UPDATE, OnAppUpdate)
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_APP_HELP_PRO, OnAppHelpPro)
	ON_COMMAND(ID_APP_HELP_PRO1, OnAppHelpPro1)
	ON_COMMAND(ID_APP_HELP_PRO2, OnAppHelpPro2)
	ON_COMMAND(ID_APP_HELP_ARCBRIDGE1, OnAppHelpArcbridge1)
	ON_COMMAND(ID_APP_HELP_ARCBRIDGE, OnAppHelpArcbridge)
	ON_COMMAND(ID_TOOL_HGEXPORT, OnToolHgexport)
	ON_UPDATE_COMMAND_UI(ID_TOOL_HGEXPORT, OnUpdateToolHgexport)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_REGMSG_SHOW_UPDATEDLG, OnShowUpdateDlg)
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_MESSAGE(XTPWM_SHORTCUTBAR_NOTIFY, OnShortcutBarNotify)
	ON_COMMAND_RANGE(ID_INPUT_BASE1, ID_INPUT_STM5, OnInput)
	ON_UPDATE_COMMAND_UI_RANGE(ID_INPUT_BASE1, ID_INPUT_STM5, OnUpdateInput)	
	ON_MESSAGE(WM_CREATE_INPUT, OnCreateInputPane)
	ON_MESSAGE(WM_UPDATE_INPUT_TITLE, OnUpdateInputTitle)
	ON_MESSAGE(WM_UPDATE_DRAWING_TITLE, OnUpdateDrawingTitle)
	ON_MESSAGE(WM_UPDATE_TREE, OnUpdateTree)
	ON_MESSAGE(WM_UPDATE_STATUSBAR, OnUpdateStatusBar)
	ON_MESSAGE(WM_UPDATE_TOOLBAR_DRAW, OnUpdateToolbarDraw)
	ON_XTP_CREATECONTROL()	
	ON_CBN_SELENDOK(ID_UNIT_COMBO, OnUnitComboChange)
	ON_MESSAGE(WM_UPDATE_UNITCOMBO, OnUnitComboChangeOnDialog)
	ON_MESSAGE(WM_USER_APPLYDOMYUN, OnApplyDomyun)
	ON_MESSAGE(WM_USER+1234, OnCheckLine)
	ON_COMMAND(ID_CREATE_LICENSE, &CMainFrame::OnCreateKey)
// INSERT_ATESTCODE
	ON_MESSAGE(WM_ATT_MAINMSG, OnAttMainFrm)
	ON_MESSAGE(WM_SWITCH_VIEW, OnSwitchView)
	ON_MESSAGE(WM_PAGEINFO, OnPageInfo)	
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_PRJ,
	ID_INDICATOR_ENG,
	ID_INDICATOR_UNIT,
	ID_INDICATOR_DIR,
//	ID_INDICATOR_VERSION,
};

static UINT shortcuts[] =
{
	ID_SHORTCUT_START,
    ID_SHORTCUT_INPUT,
    ID_SHORTCUT_DRAWING,
	ID_SHORTCUT_MODEL,
	ID_SHORTCUT_BM
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	//m_pADManage = NULL;
	m_bMessageDraw = FALSE;
	m_bPrintPreview	= FALSE;	// 미리 보기 중인지?	
	m_pView	= NULL;
	m_pHgExportMain		= new CHgExportMain(this);

	m_nLockOnlyCheck = -1;	
}

CMainFrame::~CMainFrame()
{
	delete m_pHgExportMain;
}

HGBOOL CMainFrame::CreateStatusBar()
{
	if(!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("상태바 생성 실패!\n");
		return FALSE;
	}

	HFONT font;
	font = CreateFontSize("굴림", 12);

	m_ComboUnit.Create(WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, CRect(0,0,0,0), &m_wndStatusBar, ID_UNIT_COMBO);
	m_ComboUnit.SendMessage(WM_SETFONT, (WPARAM)font, 0);
	m_ComboUnit.AddString("CGS 단위계");
	m_ComboUnit.AddString("SI 단위계");
	m_ComboUnit.SetCurSel(0);
	m_ComboUnit.SetTheme(xtpControlThemeOffice2003);

	CXTPStatusBarPane* pPane = m_wndStatusBar.AddIndicator(ID_UNIT_COMBO, UNIT_TEXT);
	int nIndex = m_wndStatusBar.CommandToIndex(ID_UNIT_COMBO);
	m_wndStatusBar.SetPaneWidth(nIndex, 90);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex) | SBPS_POPOUT);
	m_wndStatusBar.AddControl(&m_ComboUnit, ID_UNIT_COMBO, FALSE);
	pPane->SetCustomizationVisible(TRUE);
	
	return TRUE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CFrameWnd::OnCreate(lpCreateStruct)==-1)
		return -1;

	//----------------------------------------------------------------------------------------------
	//  SKIN
	//----------------------------------------------------------------------------------------------
// 	XTPSkinManager()->SetAutoApplyNewThreads(FALSE);
// 	XTPSkinManager()->GetResourceFile()->SetModuleHandle(AfxGetInstanceHandle());
// 	XTPSkinManager()->SetApplyOptions(XTPSkinManager()->GetApplyOptions() | xtpSkinApplyFrame | xtpSkinApplyMetrics | xtpSkinApplyColors);
// 	XTPSkinManager()->LoadSkin(NULL, _T("NORMALBLUE.INI"));	

	
	// Create Status bar. 
	// Important: All control bars including the Status Bar 
	// must be created before CommandBars....
	if (CreateStatusBar() == FALSE)
	{
		TRACE0("Failed to create status bar\n");
		return -1;
	}


	// Initialize the command bars
	if(!InitCommandBars())
	{
		TRACE0("커맨드바 초기화 실패!\n");
		return -1;
	}

	// Set Office 2003 Theme
//	CXTPPaintManager::SetTheme(xtpThemeOffice2007);

	SetThemeDll();

	// 테마
	XTPPaintManager()->SetTheme(xtpThemeVisualStudio2012Light);
	XTPPaintManager()->GetIconsInfo()->bUseFadedIcons = FALSE;

	m_paneManager.SetTheme((XTPDockingPanePaintTheme)xtpPaneThemeVisualStudio2012);
	m_paneManager.SetDockingContextStickerStyle(xtpPaneStickerStyleVisualStudio2012Light);
	m_paneManager.SetThemedFloatingFrames(TRUE);

	// Get a pointer to the command bars object.
	m_pCommandBars = GetCommandBars();
	if(m_pCommandBars==NULL)
	{
		TRACE0("커맨드바 객체 생성 실패!\n");
		return -1;
	}

	// Add the menu bar
	CXTPMenuBar* pMenuBar = m_pCommandBars->SetMenu(_T("메뉴 모음"), IDR_MAINFRAME); 
	if(pMenuBar==NULL)
	{
		TRACE0("메뉴바 생성 실패!\n");
		return -1;
	}	

	//메뉴바 폰트 및 크기 통일화
	LOGFONT lFont;
	CFont *pFont2 = XTPPaintManager()->GetRegularFont();
	pFont2->GetLogFont(&lFont);
	lFont.lfHeight = -12;
	::hgstrcpy(lFont.lfFaceName, _countof(lFont.lfFaceName), _T("맑은 고딕"));			
	XTPPaintManager()->SetCommandBarsFontIndirect(&lFont);

	// Create ToolBar
//	XTPImageManager()->SetMaskColor(RGB(192, 192, 192));

	CXTPToolBar* pToolBarFile = (CXTPToolBar*)m_pCommandBars->Add(_T("파일"), xtpBarTop);
	if(!pToolBarFile || !pToolBarFile->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("파일 도구바 생성 실패!\n");
		return -1;
	}

	CString str	= MAKEINTRESOURCE(IDR_TOOLBAR_BASE);
	CXTPToolBar* pToolBarBase = (CXTPToolBar*)m_pCommandBars->Add(_T("교량 기본 설정"), xtpBarTop);
	if(!pToolBarBase || !pToolBarBase->LoadToolBar(IDR_TOOLBAR_BASE))
	{
		TRACE0("교량 기본 설정 도구바 생성 실패!\n");
		return -1;
	}
	DockRightOf(pToolBarBase, pToolBarFile);

	CXTPToolBar* pToolBarGeneral = (CXTPToolBar*)m_pCommandBars->Add(_T("일반 입력"), xtpBarTop);
	if(!pToolBarGeneral || !pToolBarGeneral->LoadToolBar(IDR_TOOLBAR_GENERAL))
	{
		TRACE0("일반 입력 도구바 생성 실패!\n");
		return -1;
	}
	DockRightOf(pToolBarGeneral, pToolBarBase);

	CXTPToolBar* pToolBarRebar = (CXTPToolBar*)m_pCommandBars->Add(_T("철근 배치"), xtpBarTop);
	if(!pToolBarRebar || !pToolBarRebar->LoadToolBar(IDR_TOOLBAR_REBAR))
	{
		TRACE0("철근 배치 도구바 생성 실패!\n");
		return -1;
	}
	DockRightOf(pToolBarRebar, pToolBarGeneral);

	CXTPToolBar* pToolBarSTM = (CXTPToolBar*)m_pCommandBars->Add(_T("STM 해석"), xtpBarTop);
	if(!pToolBarSTM || !pToolBarSTM->LoadToolBar(IDR_TOOLBAR_STM))
	{
		TRACE0("STM 해석 도구바 생성 실패!\n");
		return -1;
	}
	DockRightOf(pToolBarSTM, pToolBarRebar);

	CXTPToolBar* pToolBarOutput = (CXTPToolBar*)m_pCommandBars->Add(_T("결과물 출력"), xtpBarTop);
	if(!pToolBarOutput || !pToolBarOutput->LoadToolBar(IDR_TOOLBAR_OUTPUT))
	{
		TRACE0("결과물 출력 도구바 생성 실패!\n");
		return -1;
	}

	CXTPToolBar* pToolBarDraw = (CXTPToolBar*)m_pCommandBars->Add(_T("도면 그리기"), xtpBarTop);
	if(!pToolBarDraw || !pToolBarDraw->LoadToolBar(IDR_TOOLBAR_DRAW))
	{
		TRACE0("도면 그리기 도구바 생성 실패!\n");
		return -1;
	}
	DockRightOf(pToolBarDraw, pToolBarOutput);

	CXTPToolBar* pToolBarFormat = (CXTPToolBar*)m_pCommandBars->Add(_T("도면 형식 및 선형"), xtpBarTop);
	if(!pToolBarFormat || !pToolBarFormat->LoadToolBar(IDR_TOOLBAR_FORMAT))
	{
		TRACE0("도면 형식 및 선형 도구바 생성 실패!\n");
		return -1;
	}
	DockRightOf(pToolBarFormat, pToolBarDraw);

	CXTPToolBar* pToolBarZoom = (CXTPToolBar*)m_pCommandBars->Add(_T("줌"), xtpBarTop);
	if(!pToolBarZoom || !pToolBarZoom->LoadToolBar(IDR_TOOLBAR_ZOOM))
	{
		TRACE0("줌 도구바 생성 실패!\n");
		return -1;
	}

	CXTPToolBar* pToolBarOSnap = (CXTPToolBar*)m_pCommandBars->Add(_T("객체 스냅"), xtpBarTop);
	if(!pToolBarOSnap || !pToolBarOSnap->LoadToolBar(IDR_TOOLBAR_OSNAP))
	{
		TRACE0("객체 스냅 도구바 생성 실패!\n");
		return -1;
	}
	DockRightOf(pToolBarOSnap, pToolBarZoom);

	CXTPToolBar* pToolBarAlign = (CXTPToolBar*)m_pCommandBars->Add(_T("도면 객체 정렬"), xtpBarTop);
	if(!pToolBarAlign || !pToolBarAlign->LoadToolBar(IDR_TOOLBAR_ALIGN))
	{
		TRACE0("도면 객체 정렬 도구바 생성 실패!\n");
		return -1;
	}
	DockRightOf(pToolBarAlign, pToolBarOSnap);

	CXTPToolBar* pToolBarOthers = (CXTPToolBar*)m_pCommandBars->Add(_T("기타 도구 및 설정"), xtpBarTop);
	if(!pToolBarOthers || !pToolBarOthers->LoadToolBar(IDR_TOOLBAR_OTHERS))
	{
		TRACE0("기타 도구 및 설정 도구바 생성 실패!\n");
		return -1;
	}
	DockRightOf(pToolBarOthers, pToolBarAlign);
	
	CXTPToolBar* pToolBarStmEdit = (CXTPToolBar*)m_pCommandBars->Add(hggettext("STM 수정"), xtpBarTop);
	if(!pToolBarStmEdit || !pToolBarStmEdit->LoadToolBar(IDR_STM_TOOLBAR_EDIT))
	{
		TRACE0("Failed to create toolbar and STM Edit tools and settings!\n");
		return -1;
	}

	CXTPToolBar* pToolBarStmView = (CXTPToolBar*)m_pCommandBars->Add(hggettext("STM 뷰"), xtpBarTop);
	if(!pToolBarStmView || !pToolBarStmView->LoadToolBar(IDR_STM_TOOLBAR_VIEW))
	{
		TRACE0("Failed to create toolbar and STM View tools and settings!\n");
		return -1;
	}
	DockRightOf(pToolBarStmView, pToolBarStmEdit);

	CXTPToolBar* pToolBarStmSelect = (CXTPToolBar*)m_pCommandBars->Add(hggettext("STM 선택"), xtpBarTop);
	if(!pToolBarStmSelect || !pToolBarStmSelect->LoadToolBar(IDR_STM_TOOLBAR_SELECT))
	{
		TRACE0("Failed to create toolbar and STM Select tools and settings!\n");
		return -1;
	}
	DockRightOf(pToolBarStmSelect, pToolBarStmView);

	CXTPToolBar* pToolBarStmDesign = (CXTPToolBar*)m_pCommandBars->Add(hggettext("STM 디자인"), xtpBarTop);
	if(!pToolBarStmDesign || !pToolBarStmDesign->LoadToolBar(IDR_STM_TOOLBAR_DESIGN))
	{
		TRACE0("Failed to create toolbar and STM Design tools and settings!\n");
		return -1;
	}
	DockRightOf(pToolBarStmDesign, pToolBarStmSelect);

	CXTPToolBar* pToolBarStmDefine = (CXTPToolBar*)m_pCommandBars->Add(hggettext("STM 정의"), xtpBarTop);
	if(!pToolBarStmDefine || !pToolBarStmDefine->LoadToolBar(IDR_STM_TOOLBAR_DEFINE))
	{
		TRACE0("Failed to create toolbar and STM Define tools and settings!\n");
		return -1;
	}
	DockRightOf(pToolBarStmDefine, pToolBarStmDesign);

	CXTPToolBar* pToolBarStmAssign = (CXTPToolBar*)m_pCommandBars->Add(hggettext("STM 할당"), xtpBarTop);
	if(!pToolBarStmAssign || !pToolBarStmAssign->LoadToolBar(IDR_STM_GENERAL_TOOLBAR_ASSIGN))
	{
		TRACE0("Failed to create toolbar and STM Assign tools and settings!\n");
		return -1;
	}
	DockRightOf(pToolBarStmAssign, pToolBarStmDefine);

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseDisabledIcons = TRUE;
	pIconsInfo->bIconsWithShadow = TRUE;

	//////////////////////////////////////////////////////////////////////////
	m_paneManager.InstallDockingPanes(this);

	m_paneManager.SetAlphaDockingContext(TRUE);
	m_paneManager.SetDockingContextStickerStyle(xtpPaneStickerStyleVisualStudio2012Light);
	m_paneManager.SetShowDockingContextStickers(TRUE);

	m_paneManager.CreatePane(IDR_PANE_INPUT, CRect(0, 0, 250, 250), xtpPaneDockBottom);
	m_paneManager.CreatePane(IDR_PANE_CONTROL, CRect(0, 0, 250, 250), xtpPaneDockLeft);

// 	if(m_pDockingFrame == NULL)
// 	{
// 		m_pDockingFrame = (CDockingFrame*)CDockingFrame::CreateObject();
// 		m_pDockingFrame->Create(NULL, NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CXTPEmptyRect(), this, 0);
// 		m_pDockingFrame->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
// 	}

	m_paneManager.ClosePane(IDR_PANE_INPUT);
	//////////////////////////////////////////////////////////////////////////

	LoadCommandBars(_T("CommandBars"));
	PostMessage(WM_SYSCOLORCHANGE);
/*	
	// Initialize the docking pane manager and set the
	// initial them for the docking panes.  Do this only after all
	// control bars objects have been created and docked.
	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeOutlook2007);
	// Create docking panes.
	m_paneManager.CreatePane(IDR_PANE_INPUT, CRect(0, 0, 250, 250), xtpPaneDockBottom);
	m_paneManager.CreatePane(IDR_PANE_CONTROL, CRect(0, 0, 227, 250), xtpPaneDockLeft);	
	m_paneManager.ClosePane(IDR_PANE_INPUT);	
	*/
	// 20111018 MainFrame 이 활성화 되지 않는 버그로 인하여 강제로 활성화 시킴
	// 활성화 되지 않는 이유는 찾지 못하였음
	// 파일을 클릭으로 열 경우 다운됨
	// - 신종화
	SetActiveWindow();

	return 0;
}

void CMainFrame::SetThemeDll()
{
	HMODULE hModule = LoadLibrary(NOTRANS("VisualStudio2012.dll"));
	SetCommandBarsTheme(hModule, NOTRANS("VisualStudio2012Light.INI"));
}

void CMainFrame::SetCommandBarsTheme( HMODULE hModule/*=NULL*/, LPCTSTR lpszINI/*=NULL*/ )
{
	CXTPCommandBars* pCommandBars = GetCommandBars();

	if (pCommandBars != NULL)
	{
		pCommandBars->GetImageManager()->RemoveAll();
		pCommandBars->GetImageManager()->SetIcons(IDR_MAINFRAME);

		if (hModule != NULL && lpszINI != NULL)
		{
			XTPResourceImages()->SetHandle(hModule, lpszINI);
		}

		m_bAllowDwm = FALSE;
		EnableFrameTheme(pCommandBars);
		pCommandBars->SetAllCaps(TRUE);
	}
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// Dialog Title 변경
//	cs.style &= ~FWS_ADDTOTITLE;

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	WNDCLASS wndcls;	
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!::GetClassInfo(hInst, szARcBridgeClass, &wndcls))
	{
		// get default stuff
		::GetClassInfo(hInst, cs.lpszClass, &wndcls);
		wndcls.style &= ~(CS_HREDRAW|CS_VREDRAW);
		// register a new class
		wndcls.lpszClassName = szARcBridgeClass;
		wndcls.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDR_MAINFRAME));
		ASSERT(wndcls.hIcon != NULL);
		if (!AfxRegisterClass(&wndcls))
			AfxThrowResourceException();
	}
	cs.lpszClass = szARcBridgeClass;	
	cs.style |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	m_pViewStm = new CARcBridgeStmView;																	// 서로 다른 뷰 전환
	m_pViewStm->Create(NULL, NULL, WS_CHILD, CXTPFrameWnd::rectDefault, this, VIEWID_UBICON, pContext);	// 서로 다른 뷰 전환

	return CXTPFrameWnd::OnCreateClient(lpcs, pContext);

	/*
	if(!m_wndSplitter.CreateStatic(this, 2, 1))
	{
		TRACE0("Failed to create splitter window\n");
		return FALSE;
	}

	if(!m_wndCaption.Create(&m_wndSplitter, _T("도면"), CPWS_EX_RAISED_EDGE,
		WS_VISIBLE, CRect(0, 0, 0, 0), m_wndSplitter.IdFromRowCol(0, 0)))
	{
		TRACE0("Failed to caption window.\n");
		return -1;
	}

//	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_wndCaption.SetCaptionColors(XTAuxData().clr3DFace, XTAuxData().clr3DShadow, XTAuxData().clrWindow);
	m_wndCaption.ModifyCaptionStyle(0, NULL, NULL, NULL);

	m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CARcBridgeView), CSize(0, 0), pContext);

	CRect rc;
	GetClientRect(rc);
	m_wndSplitter.SetRowInfo (0, 22, 0);
	m_wndSplitter.SetSplitterStyle(XT_SPLIT_NOSIZE);

	m_wndSplitter.SetActivePane(1, 0);

	return TRUE;
	*/
}

int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if(lpCreateControl->bToolBar)
	{
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);
		if(!pToolBar)
			return FALSE;

		if(pToolBar->GetBarID()==IDR_TOOLBAR_DRAW)
		{
			if(lpCreateControl->nID==IDC_TEDIT_PAGE)
			{
				CXTPControlEdit* pPage = (CXTPControlEdit*)CXTPControlEdit::CreateObject();
				pPage->SetCaption(_T("페이지"));
				pPage->SetWidth(72);	// 여기서는 사이즈 영향 없음.
				lpCreateControl->pControl = pPage;
				return TRUE;
			}
			if(lpCreateControl->nID==IDC_TCOMBO_BRIDGE)
			{
				CXTPControlComboBox* pBridge = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
				pBridge->SetCaption(_T("교량 선택"));
				pBridge->SetWidth(200);	// 여기서는 사이즈 영향 없음.
				lpCreateControl->pControl = pBridge;
				return TRUE;
			}
			if(lpCreateControl->nID==IDC_TCOMBO_CATEGORY)
			{
				CXTPControlComboBox* pCategory = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
				pCategory->SetCaption(_T("카테고리 선택"));
				pCategory->SetWidth(112);	// 여기서는 사이즈 영향 없음.
				lpCreateControl->pControl = pCategory;
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CMainFrame::CreateShortcutBar()
{	
	m_wndShortcutBar.Create(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CXTPEmptyRect(), this, 0);
	m_wndShortcutBar.SetOwner(this);
	m_wndShortcutBar.GetToolTipContext()->SetStyle(xtpToolTipOffice2013);
 	m_wndShortcutBar.SetTheme(xtpShortcutThemeOfficeXP);

	COLORREF clr = XTPImageManager()->SetMaskColor(RGB(192, 192, 192));
	XTPImageManager()->SetIcons(IDB_BITMAP_SHORTCUTS_SMALL, shortcuts, sizeof(shortcuts), CSize(16, 16));
	XTPImageManager()->SetMaskColor(clr);
	XTPImageManager()->SetIcons(IDB_BITMAP_SHORTCUTS_BIG, shortcuts, sizeof(shortcuts), CSize(24, 24));
	XTPImageManager()->SetMaskColor(clr);

	VERIFY(m_paneShortcutStart.Create(_T(""), &m_wndShortcutBar));
	VERIFY(m_paneShortcutInput.Create(_T(""), &m_wndShortcutBar));
	VERIFY(m_paneShortcutDrawing.Create(_T(""), &m_wndShortcutBar));
	VERIFY(m_paneShortcutBM.CreateStyle1(_T(""), &m_wndShortcutBar));
//	VERIFY(m_paneShortcutModel.Create(_T(""), &m_wndShortcutBar));

	CXTPShortcutBarItem* pItemStart = m_wndShortcutBar.AddItem(ID_SHORTCUT_START, &m_paneShortcutStart);
	m_wndShortcutBar.AddItem(ID_SHORTCUT_INPUT, &m_paneShortcutInput);
	m_wndShortcutBar.AddItem(ID_SHORTCUT_DRAWING, &m_paneShortcutDrawing);
	m_wndShortcutBar.AddItem(ID_SHORTCUT_BM, &m_paneShortcutBM);
//	m_wndShortcutBar.AddItem(ID_SHORTCUT_MODEL, &m_paneShortcutModel);

	m_wndShortcutBar.SelectItem(pItemStart);
	m_wndShortcutBar.LoadState(_T("ShortcutBar"));
	m_wndShortcutBar.SetExpandedLinesCount(1);
}

void CMainFrame::OnClose() 
{
	SaveCommandBars(_T("CommandBars"));
	m_wndShortcutBar.SaveState(_T("ShortcutBar"));
	
	CFrameWnd::OnClose();
}

void CMainFrame::OnCustomize()
{
	CXTPCustomizeSheet cs(GetCommandBars());

	CXTPCustomizeOptionsPage pageOptions(&cs);
	cs.AddPage(&pageOptions);

	CXTPCustomizeCommandsPage* pCommands = cs.GetCommandsPage();
	pCommands->AddCategories(IDR_MAINFRAME);

	pCommands->InsertAllCommandsCategory();
	pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
	pCommands->InsertNewMenuCategory();

	cs.DoModal();
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	CMainFrame *pFrm	= (CMainFrame *)AfxGetMainWnd();
	if(!pFrm) return FALSE;
	if(pFrm != this) return FALSE;
	
	if(wParam==XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
    
		if(!pPane->IsValid())
		{
			switch(pPane->GetID())
			{
			case IDR_PANE_CONTROL:
				if(m_wndShortcutBar.GetSafeHwnd()==0)
				{
					CreateShortcutBar();
				}
				pPane->Attach(&m_wndShortcutBar);
				break;
			}
		}
		return TRUE;
	}

	return FALSE;
}

LRESULT CMainFrame::OnShortcutBarNotify(WPARAM wParam, LPARAM lParam)
{
	if(!LDK::aCheckLicenseHGKey()) return 0;
	if(m_bPrintPreview) return FALSE;	

	if(wParam==XTP_SBN_SELECTION_CHANGING)
	{
		CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
		CARcBridgeView* pView = ((CMainFrame*)AfxGetMainWnd())->m_pView;
		if (pView == NULL || IsWindow(pView->GetSafeHwnd()) == FALSE)
		{
			return 0;
		}

		CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;
		BOOL bDomView = GetActiveView()->IsKindOf(RUNTIME_CLASS(CARcBridgeView));

		CXTPDockingPane* pPane = m_paneManager.FindPane(IDR_PANE_CONTROL);
		CXTPShortcutBarItem* pItem = (CXTPShortcutBarItem*)lParam;
		UINT nShorcutID = pItem->GetID();
		m_nCurShortcutID= nShorcutID;

		if(bDomView)
		{
			pView->SettingViewByShortcutBar();
		}
		else
		{
			CARcBridgeStmView *pViewStm = (CARcBridgeStmView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
			pViewStm->SettingViewByShortcutBar();
		}

		if(nShorcutID==ID_SHORTCUT_START)
			pPane->SetTitle(_T("시작"));
		if(nShorcutID==ID_SHORTCUT_INPUT)
		{
			pPane->SetTitle(_T("입력"));
			
			UINT nMenuID = pIn->GetCurrentMenuID();
			if(nMenuID<ID_INPUT_BASE1 || nMenuID>ID_INPUT_GENERAL14)
				nMenuID = ID_INPUT_BASE1;

			pDoc->m_bFileOpen = TRUE;
			OnCreateInputPane(nMenuID, 0);
			m_paneManager.ShowPane(IDR_PANE_INPUT);
		}
		if(nShorcutID==ID_SHORTCUT_DRAWING)
		{
			pPane->SetTitle(_T("도면"));

			if(!m_bMessageDraw)
			{
				CARcBridgeDataStd* pStd = pIn->m_pARcBridgeDataStd;
				CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;

				// 트리를 갱신함.
				m_paneShortcutDrawing.InitTree();

				// 교량별 도면 옵션 생성
				CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
				pOpt->MakeDrawingOption();
				
				CDrawPageMng* pDrawMng = pDraw->GetDrawPageMng();

				if(pStd->GetBridgeSize()>0)
				{
					pDrawMng->SetPageInit();
					if(pDraw->m_nCurPage<1)
						pDraw->m_nCurPage = 1;
					if(pDraw->m_nCurPage>pDraw->GetCountTotalPage())
						pDraw->m_nCurPage = pDraw->GetCountTotalPage();

					COutMsgDlg msgDlg(this, pDrawMng);
					pView->SetGL(FALSE);
					msgDlg.Run();

					long nSubPage = pDraw->GetSubPage(pDraw->m_nCurPage) + 1;
					m_paneShortcutDrawing.SetItemPositionUpdate(nSubPage);
					OnUpdateDrawingTitle(pDraw->m_nCurPage, 0);
					OnUpdateToolbarDraw(0,0);
				}

				pDoc->m_pARcBridgeInputStd->HideDialog();
				m_paneManager.ClosePane(IDR_PANE_INPUT);
				pDoc->m_pARcBridgeInputStd->SetSaveMenuIDD(ID_INPUT_END);

				UINT nMenuID = pIn->GetCurrentMenuID();
				SwitchView(VIEWID_DEFAULT, nMenuID);
			}
		}
/*		if(nShorcutID == ID_SHORTCUT_MODEL)
		{
			pDoc->m_pARcBridgeInputStd->HideDialog();
			m_paneManager.ClosePane(IDR_PANE_INPUT);
			pDoc->m_pARcBridgeInputStd->SetSaveMenuIDD(ID_INPUT_END);

			pPane->SetTitle(_T("3DView"));
		}
*/

		if(nShorcutID == ID_SHORTCUT_BM)
		{
			pPane->SetTitle(_T("수량"));

			pDoc->m_pARcBridgeInputStd->HideDialog();
			m_paneManager.ClosePane(IDR_PANE_INPUT);
			pDoc->m_pARcBridgeInputStd->SetSaveMenuIDD(ID_INPUT_END);

			if(!bDomView)
			{
				UINT nMenuID = pIn->GetCurrentMenuID();
				SwitchView(VIEWID_DEFAULT, nMenuID);
			}

			long nSize = m_paneShortcutBM.m_dlgMenuOutBM.m_ctlTreeBM.GetCount();
			if(nSize == 0)
				m_paneShortcutBM.m_dlgMenuOutBM.RunOnButtonBmByStruct();
		}
		
		m_bMessageDraw = FALSE;

		return TRUE;
	}

	return FALSE;
}


void CMainFrame::InitBMTree()
{
	if(m_paneShortcutBM.GetSafeHwnd())
		m_paneShortcutBM.m_dlgMenuOutBM.m_ctlTreeBM.DeleteAllItems();	
}


LRESULT CMainFrame::OnUpdateInputTitle(WPARAM nMenuID, LPARAM)
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;
	CARcBridgeDataStd* pStd = pIn->m_pARcBridgeDataStd;

	CString strBri = _T("");
	CRcBridgeApp* pBri = pStd->GetBridge(pIn->m_nCurrentBridge);
	if(pBri)
	{
		CString strBri = _T("");
		if(nMenuID>ID_INPUT_BASE4)
			strBri = pBri->m_strBridgeName + _T(" : ");
	}

	CString strTitle = _T("");
	UINT nItemSize = m_paneShortcutInput.m_cmdIDArr.GetSize();
	UINT i=0;
	for(i=0; i<nItemSize; i++)
	{
		UINT nID = m_paneShortcutInput.m_cmdIDArr.GetAt(i);
		if(nID==nMenuID)
			break;
	}
	strTitle = m_paneShortcutInput.m_cmdStrArr.GetAt(i);

	strTitle = strBri + strTitle;

	CXTPDockingPane* pPane = m_paneManager.FindPane(IDR_PANE_INPUT);
	pPane->SetTitle(strTitle);

//	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_wndCaption.UpdateCaption(strTitle, NULL);

	return 1;
}

LRESULT CMainFrame::OnUpdateDrawingTitle(WPARAM nPage, LPARAM)
{
	/*
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeDataStd* pStd = pDoc->m_pARcBridgeDataStd;
	CARcBridgeDrawingStd* pDraw = pDoc->m_pARcBridgeDrawingStd;

	long nSubPage = pDraw->GetSubPage(nPage) + 1;
	long nBri = pDraw->GetBridgeNumber(nSubPage);
	CRcBridgeApp* pBri = pStd->GetBridge(nBri);

	CPageInfo* pInfo = pDraw->GetPageInfo(nSubPage);
	CString strCategory = _T(" : ") + pDraw->GetTitleCategory(pInfo->m_nCategory) + _T(" - ");
	CString strPage = _T("");
	strPage.Format(_T(" <%ld / %ld>"), nSubPage, pDraw->GetCountTotalPage());

	CString strTitle = pBri->m_strBridgeName + strCategory + pInfo->m_strTitle + strPage;
//	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_wndCaption.UpdateCaption(strTitle, NULL);
	*/

	return 1;
}

void CMainFrame::OnViewControlpane() 
{
	if(m_paneManager.IsPaneClosed(IDR_PANE_CONTROL))
		m_paneManager.ShowPane(IDR_PANE_CONTROL);
	else
		m_paneManager.ClosePane(IDR_PANE_CONTROL);
}

void CMainFrame::OnUpdateViewControlpane(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!m_paneManager.IsPaneClosed(IDR_PANE_CONTROL));
}

void CMainFrame::OnViewInputpane() 
{
	if(m_paneManager.IsPaneClosed(IDR_PANE_INPUT))
		m_paneManager.ShowPane(IDR_PANE_INPUT);
	else
		m_paneManager.ClosePane(IDR_PANE_INPUT);
}

void CMainFrame::OnUpdateViewInputpane(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!m_paneManager.IsPaneClosed(IDR_PANE_INPUT));
}

void CMainFrame::OnInput(UINT nID)
{
	if(!aIsLockOK() && !LDK::aIsLogIn()) // 2015. 07. 28. Software Lock 작업, 김형준
		return;

// 	if(!GetActiveView()->IsKindOf(RUNTIME_CLASS(CARcBridgeView))) 	
// 		SetActiveView(m_pView);

	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeView* pView = (CARcBridgeView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;

	if(pDoc)
	{
		pDoc->m_bFileOpen = TRUE;
		if(pIn->GetCurrentMenuID()!=nID)
		{
			OnCreateInputPane(nID, 0);

			CXTPShortcutBarItem* pSelItem = m_wndShortcutBar.GetItem(1);
			m_wndShortcutBar.SelectItem(pSelItem);
		}

		/*
		if(pIn->GetCurrentDialog())
		{
			CRect rect;
			int nTitleHeight = 25;
			int nHelpHeight = 0;
			pIn->GetCurrentDialog()->GetWindowRect(&rect);
			pIn->SetSaveMenuIDD(pIn->GetCurrentDialog()->GetIDD());
			//if(pIn->GetCurrentDialog()->IsGL())	pView->SetGL(TRUE);
			m_paneManager.SetFixedHeight(IDR_PANE_INPUT, rect.Height() + nTitleHeight + nHelpHeight);
			pView->SetClippingVolume();
		}
		*/
		m_paneManager.ShowPane(IDR_PANE_INPUT);
	}
}

void CMainFrame::OnUpdateInput(CCmdUI* pCmdUI)
{
	if(!aIsLockOK() && !LDK::aIsLogIn()) // 2015. 07. 28. Software Lock 작업, 김형준
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;

	int nCurID = pIn->GetCurrentMenuID();
	for(int i=ID_INPUT_BASE1; i<=ID_INPUT_STM5; i++)
		pCmdUI->SetCheck(pCmdUI->m_nID==nCurID);

	if(ID_INPUT_BASE1==pCmdUI->m_nID)
	{
		pCmdUI->Enable(TRUE);
		return;
	}

	if(!GetActiveView()->IsKindOf(RUNTIME_CLASS(CARcBridgeView)))
	{
		pCmdUI->Enable(FALSE);
	}

	if(pDoc)
	{
		BOOL bEnable = pDoc->m_pARcBridgeInputStd->IsEnableDialog(pCmdUI->m_nID);
		pCmdUI->Enable(bEnable);
	}
}

LRESULT CMainFrame::OnCreateInputPane(UINT nID, LPARAM)
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeView* pView = (CARcBridgeView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();

	CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;
	CXTPDockingPane* pPane = m_paneManager.FindPane(IDR_PANE_INPUT);

	pPane->Detach();
	if(pIn->m_paneInputDialog.GetSafeHwnd())
		pIn->m_paneInputDialog.DestroyWindow();

	OnUpdateInputTitle(nID, 0);

	CInputDialogBase* pDlg = pIn->GetDialogByMenuID(nID);
	pIn->SetCurrentDialog(pDlg);
	if(pIn->m_paneInputDialog.GetSafeHwnd()==0)
	{
		pDlg->m_pView = (CDomyunGLView*)pView;
		pIn->m_paneInputDialog.Create(NULL, NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
	}

	m_paneShortcutInput.SetItemPosition(nID);

	// 싸이즈 조정 ///////
	CRect rect;
	int nTitleHeight = 25;
	int nHelpHeight = 25;
	pIn->GetCurrentDialog()->GetWindowRect(&rect);
	pIn->SetSaveMenuIDD(pDlg->GetIDD());
	ResizePane(IDR_PANE_INPUT, CSize(250, rect.Height() + nTitleHeight + nHelpHeight), TRUE);
	////////////////
	
	pPane->Attach(&pIn->m_paneInputDialog);
	pPane->Select();

	m_paneManager.RedrawWindow();
	m_paneShortcutStart.m_wndTaskPanel.RedrawWindow();
	m_paneShortcutInput.m_wndTreeInput.RedrawWindow();
	m_paneShortcutDrawing.m_wndTreeDrawing.RedrawWindow();
	m_paneShortcutBM.RedrawWindow();
	m_wndShortcutBar.RedrawWindow();
	pView->RedrawWindow();

	return 1;
}

LRESULT CMainFrame::OnUpdateTree(WPARAM wParam, LPARAM)
{	
	int nID = (int)wParam;

	if(nID<0)
		m_paneShortcutInput.InitTreeState();
	else
		m_paneShortcutInput.SetItemPositionUpdate(nID, TRUE);

	m_paneShortcutDrawing.InitTree();

	return 1;
}

LRESULT CMainFrame::OnUpdateStatusBar(WPARAM wParam, LPARAM)
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeView* pView = (CARcBridgeView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	CARcBridgeDataStd* pData = pDoc->m_pARcBridgeDataStd;
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;

	m_wndStatusBar.SetPaneStyle(PRJ_TEXT, pDoc->m_bFileOpen ? SBPS_NORMAL : SBPS_DISABLED);
	m_wndStatusBar.SetPaneText(PRJ_TEXT, pData->m_strProjectName);
	m_wndStatusBar.SetPaneStyle(ENG_TEXT, pDoc->m_bFileOpen ? SBPS_NORMAL : SBPS_DISABLED);
	m_wndStatusBar.SetPaneText(ENG_TEXT, pData->m_strEngName);
	
	if(pDoc->m_bFileOpen)
	{
		if (pData->IsLsdDesign())
		{
			m_ComboUnit.EnableWindow(FALSE);
		}
		else
		{
			if(pData->IsDoro2010())
				m_ComboUnit.EnableWindow(FALSE);
			else
				m_ComboUnit.EnableWindow(TRUE);
		}
	}
	else
	{
		m_ComboUnit.EnableWindow(FALSE);
	}
	
	m_wndStatusBar.SetPaneStyle(UNIT_TEXT, pDoc->m_bFileOpen ? SBPS_NORMAL : SBPS_DISABLED);
	m_wndStatusBar.SetPaneText(UNIT_TEXT, _T("CGS 단위계"));

	long nPage = pDraw->m_nCurPage;
	COutMovePoint* pMove = pDraw->GetMovePoint(nPage);
	if(pMove)
		pMove->SetVHMode(pView->GetOnlyVHDraw());

	if(pDoc->m_bFileOpen && pMove)
	{
		m_wndStatusBar.SetPaneStyle(DIR_TEXT, pDoc->m_bFileOpen ? SBPS_NORMAL : SBPS_DISABLED);
		if(pMove->IsVHMode())
			m_wndStatusBar.SetPaneText(DIR_TEXT, _T("직각"));
		else
			m_wndStatusBar.SetPaneText(DIR_TEXT, _T("자유"));		
	}	

	
	// 버전정보
	//UpdateLockInfo()이쪽에서 호출

	return 1;
}

void CMainFrame::OnSetCreation() 
{
	if(m_bPrintPreview) return;

	CString strTitle = _T("설정");
	CEnvRegist env;
	BOOL bInitStart = env.LoadEnvInt(CEnvRegist::SET_PRGOPT, _T("0"));
	if(!bInitStart)
		((CARcBridgeApp*)AfxGetApp())->SetFixedDirectory();

	CPrgOptSheet OptDlg(strTitle,this);
	OptDlg.DoModal();

	CARcBridgeView* pView = (CARcBridgeView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();

	CEnvRegist reg;
	if(reg.LoadEnvInt(CEnvRegist::USE_SHXFONT))
		pView->SetUseShx(TRUE);
	else
		pView->SetUseShx(FALSE);

	pView->SetCrossAngle(reg.LoadEnvDouble(CEnvRegist::CROSSCURSOR_ANGLE));
	pView->SetUseToolTip(reg.LoadEnvInt(CEnvRegist::USE_MOVETOOLTIP));
	pView->SetAutoSave(reg.LoadEnvInt(CEnvRegist::AUTO_SAVE));
	COutMsgDlg::m_bStatsMessage = reg.LoadEnvInt(CEnvRegist::USE_PROGRESSDLG);

	CRect rect;
	long nLen = 0;
	pView->GetClientRect(&rect);
	double dLenRatio = reg.LoadEnvDouble(CEnvRegist::CROSSCURSOR_LENGTH);
	if(dLenRatio==0)
		nLen = 30;
	else
	{
		if(dLenRatio!=100) 
			nLen = (long)(rect.Width()/2*dLenRatio/100);
	}
	pView->SetCrossLength(nLen);

	pView->RedrawAll();
}

LRESULT CMainFrame::OnUpdateToolbarDraw(WPARAM, LPARAM)
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;
	CARcBridgeDataStd* pStd = pIn->m_pARcBridgeDataStd;
	CARcBridgeDrawingStd* pDraw = pDoc->m_pARcBridgeDrawingStd;

	CXTPToolBar* pToolBar = m_pCommandBars->GetToolBar(IDR_TOOLBAR_DRAW);
	CXTPControlEdit* pPage = (CXTPControlEdit*)pToolBar->GetControl(0);
	CXTPControlComboBox* pBridge = (CXTPControlComboBox*)pToolBar->GetControl(1);
	CXTPControlComboBox* pCategory = (CXTPControlComboBox*)pToolBar->GetControl(2);

	int nSubPage = 0;
	int nBri = 0;
	int nCategory = 0;
	int nCountBri = pStd->GetBridgeSize();
	CString str	= _T("");
	if(nCountBri>0)
	{
		nSubPage = pDraw->GetSubPage(pDraw->m_nCurPage) + 1;
		nBri = pDraw->GetBridgeNumber(nSubPage);
		nCategory = pDraw->GetCategoryNumber(nSubPage);

		// 페이지
		CString str = _T("");
		str.Format(_T("%d"), nSubPage);
		pPage->SetEditText(str);

		// 교량 선택
		pBridge->ResetContent();
		pBridge->SetWidth(250);	// 실제로 콘보 사이즈가 여기서 조정됨
		CRcBridgeApp* pBri = NULL;
		int i=0; for(i=0; i<nCountBri; i++)
		{
			pBri = pStd->GetBridge(i);
			str = pStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");

			pBridge->AddString(str);
		}
		pBridge->SetCurSel(nBri);

		// 카테고리 선택
		pCategory->ResetContent();
		int nCountCate = pDraw->GetCountCategory();
		for(i=0; i<nCountCate; i++)
		{
			str = pDraw->GetTitleCategory(i);
			pCategory->AddString(str);
		}
		if(nCountCate>0)
			pCategory->SetCurSel(nCategory);
	}
	else
	{
		pPage->SetEditText(_T(""));
		pBridge->ResetContent();
		pCategory->ResetContent();
	}

	return 1;
}

void CMainFrame::OnFileNew() 
{
	CARcBridgeApp* pApp = (CARcBridgeApp*)AfxGetApp();
	pApp->OnFileNew();

	m_paneShortcutInput.InitTreeState();
	m_paneShortcutDrawing.InitTree();
	InitBMTree();

	CXTPShortcutBarItem* pSelItem = m_wndShortcutBar.GetSelectedItem();
	if(pSelItem->GetID()==ID_SHORTCUT_INPUT)
	{
		OnInput(ID_INPUT_BASE1);
	}
	else
	{
		pSelItem = m_wndShortcutBar.GetItem(1);
		m_wndShortcutBar.SelectItem(pSelItem);
	}
}

void CMainFrame::CreateSaveCompleteDlg()
{
	if(!m_dlgSaveComplete.GetSafeHwnd())
		m_dlgSaveComplete.Create(IDD_DIALOG_SAVE_COMPLETE, this);
	m_dlgSaveComplete.ShowWindow(SW_SHOW);
}

void CMainFrame::OnViewComment() 
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;

	pIn->m_bShowComment = !pIn->m_bShowComment;
	pIn->m_paneInputDialog.m_pInputDialog->Invalidate();
	pIn->m_paneInputDialog.Invalidate();
}

void CMainFrame::OnUpdateViewComment(CCmdUI* pCmdUI) 
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;

	pCmdUI->SetCheck(pIn->m_bShowComment);
}

void CMainFrame::OnAppHome() 
{
	CString strURL = _T("http://www.aroad.co.kr");
	ShellExecute(NULL, _T("open"), strURL, NULL, NULL, SW_SHOW);
}

void CMainFrame::OnAppQna() 
{
	CString strURL = _T("http://aroad.co.kr/customer/qna.asp?boardName=p_qna&mode=&key=&keystring=&gtype=&combo1=1_5");
	ShellExecute(NULL, _T("open"), strURL, NULL, NULL, SW_SHOW);
}

void CMainFrame::OnAppUpdate() 
{
	CString szPath = ((CARcBridgeApp*)AfxGetApp())->GetAUpdateProgramInstallPath();
	szPath += "\\AUpdate.exe";
	CFileFind ff;
	if(!ff.FindFile(szPath))
	{		
		if(AfxMessageBox("AUpdate 모듈이 존재하지 않습니다.\n웹에서 다운로드 후 설치하시겠습니까 ?", MB_YESNO | MB_ICONINFORMATION)==IDYES)
		{
			ShellExecute(NULL,NULL, "http://www.aroad.co.kr/22-download/RealUpdate_AUpdate/AUpdate_Setup.exe" ,NULL,NULL,SW_SHOW);
		}
	}
	else
		ShellExecute(NULL,NULL, szPath ,NULL,NULL,SW_SHOW);
	ff.Close();
	
}


void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	// ADomManager실행중에는 크기조정 및 이동 못함
	CXTPFrameWnd::OnSize(nType, cx, cy);
	
	if(m_ComboUnit.GetSafeHwnd())
	{	
 		CRect rect;
  		m_wndStatusBar.GetItemRect(UNIT_TEXT, rect);
		rect.top -= 1;
		rect.bottom -= 1;
 		m_ComboUnit.MoveWindow(&rect);
	}	
	//if(m_pADManage)
	//{
		//m_pADManage->MoveWindow(0,0,cx,cy);
	//}	
}


void CMainFrame::OnUnitComboChange()
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeView* pView = (CARcBridgeView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	CARcBridgeDataStd* pData = pDoc->m_pARcBridgeDataStd;

	CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;
	if(pData->m_nTypeUnit != m_ComboUnit.GetCurSel())	
	{
		if(m_ComboUnit.GetCurSel() == -1)
		{			
			m_ComboUnit.SetCurSel(pData->m_nTypeUnit);
		}
		else pData->m_nTypeUnit = m_ComboUnit.GetCurSel();							
		pData->SetStringUnitType();
		pData->m_pBasicConcInfo->SetUnit(pData->IsSI());
		pData->m_pBasicConcInfo->SetDataDefaultRebarFactor();
	}	
	if(pIn->GetCurrentDialog()) 
	{
		//pIn->GetCurrentDialog()->SetGridTitle();
		pIn->GetCurrentDialog()->SetGridData();
		pIn->GetCurrentDialog()->SetDataInit();
	}
}

LRESULT CMainFrame::OnUnitComboChangeOnDialog(WPARAM, LPARAM)
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();	
	CARcBridgeDataStd* pData = pDoc->m_pARcBridgeDataStd;

	m_ComboUnit.SetCurSel(pData->m_nTypeUnit);

	return 1;
}

LRESULT CMainFrame::OnApplyDomyun(WPARAM, LPARAM)
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	
	pDoc->m_pShareWithADManagerStd->ApplyFromADManager();

	return 1;
}


void CMainFrame::OnMove(int x, int y) 
{
	CFrameWnd::OnMove(x, y);
	
	// TODO: Add your message handler code here
	if(m_pView)
	{
		if(m_pView->m_bShellADManager)
		{
			HWND hWnd	= ::FindWindow(NULL, "dom.adm - ADManager");
			if(hWnd)
			{
				m_pView->SetFrameSizeAtRegForADManager();
				::SendMessage(hWnd, WM_SIZE, 0, 0);
			}
		}	
	}
}

void CMainFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CFrameWnd::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	if(m_pView)
	{
		if(m_pView->m_bShellADManager)
		{
			HWND hWnd	= ::FindWindow(NULL, "dom.adm - ADManager");
			if(hWnd)
			{
				m_pView->SetFrameSizeAtRegForADManager();
				::SendMessage(hWnd, WM_SIZE, 0, 0);
			}
		}	
	}
}

void CMainFrame::OnAppHelpPro() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnAppHelpPro1() 
{
	// TODO: Add your command handler code here
	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	szPath += "\\";
	ShellExecute(NULL, NULL, szPath+"ABorder.chm", "", NULL, SW_SHOWNORMAL);
}

void CMainFrame::OnAppHelpPro2() 
{
	// TODO: Add your command handler code here
	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	szPath += "\\";
	ShellExecute(NULL, NULL, szPath+"ALine.chm", "", NULL, SW_SHOWNORMAL);
}

void CMainFrame::OnAppHelpArcbridge1() 
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeDataStd* pData = pDoc->m_pARcBridgeDataStd;
	
	// TODO: Add your command handler code here
	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	CString szFileName = pData->IsLsdDesign() ? _T("ARcBridge_manual_LSD.pdf") : _T("ARcBridge_manual.pdf");

	szPath += "\\";
	ShellExecute(NULL, NULL, szPath+szFileName, "", NULL, SW_SHOWNORMAL);
	
}

void CMainFrame::OnAppHelpArcbridge() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnToolHgexport() 
{
	// TODO: Add your command handler code here
	if(m_pHgExportMain->IsRun())
		m_pHgExportMain->Stop();
	else
	{
		CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
		m_pHgExportMain->Run(pDoc->m_pARoadOptionStd);
	}
}

void CMainFrame::OnUpdateToolHgexport(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_pHgExportMain->IsRun());
	
}

// 선형 체크 
LRESULT CMainFrame::OnCheckLine(WPARAM wParam, LPARAM lParam)
{
	CLineInfoApp	*pLineApp	= (CLineInfoApp*)wParam;
	if(!pLineApp) return 0;

	CString	szTemp		= _T("");
	CString	szMessage	= _T("");
	CString	szLineName	= _T("");
	BOOL	bError		= FALSE;
	BOOL	bFirstErr	= FALSE;
	CStringArray	szArrIP;

	for(long nLine=0; nLine<pLineApp->GetLinesu(); nLine++)
	{
		CLineInfo	*pLineInfo = pLineApp->GetLine(nLine);
		bFirstErr	= FALSE;
		for(long nIp=0; nIp<pLineInfo->GetQtyIp(); nIp++)
		{
			if(pLineInfo->m_pIpRec[nIp].m_dR_IpRecord==0)
			{
				szTemp.Format("IP[%d] ", nIp+1);
				szArrIP.Add(szTemp);
				bError		= TRUE;
				bFirstErr	= TRUE;
			}
		}
		if(bFirstErr)
		{
			szLineName.Format("%s - ", pLineInfo->GetNameLineEach());
			for(long n=0; n<szArrIP.GetSize(); n++)
				szLineName += szArrIP.GetAt(n);
		}
	}
	if(bError)
	{
		szMessage.Format("반경(R)=0인 IP는 지원하지 않습니다.\nR값을 입력하시거나 IP를 삭제해주시기 바랍니다.\n계속 진행시 오류가 발생할 수 있습니다.\n\n%s", szLineName);

		// INSERT_ATESTCODE - FULL_APPNAME_TO_OPENFILE
		MessageBox(szMessage, FULL_APPNAME_TO_OPENFILE, MB_OK | MB_ICONWARNING | MB_TASKMODAL);
	}

	return 0;
	}

void CMainFrame::OnTimer(UINT nIDEvent) 
{		
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)GetActiveDocument();
	if (LDK::aIsLogIn())	//SLLock_20150526
	{
		LDK::aIsLockOKOnTimer(nIDEvent, pDoc);
	}
	else
	{
		aIsLockOKOnTimer(nIDEvent, pDoc);		
	}			
}

LRESULT CMainFrame::OnShowUpdateDlg(WPARAM wp,LPARAM lp)
{
	CARcBridgeApp* pApp = (CARcBridgeApp*)AfxGetApp();
	if(!pApp)	return 0;

	CString sVersion = pApp->GetStringVersion();
	CString sLockInfo = pApp->GetStringLockInfo();	// ARcBridge는 프로그램 이름에 버전이 표현됨(Plus)
	CString sEtcInfo = pApp->GetStringEtcInfo();

	CDialogAbout	Dlg;
	Dlg.SetProgramName(pApp->GetARcBridgeName());
	Dlg.SetLockInfo(sLockInfo);
	Dlg.SetEtcInfo(sEtcInfo);
	Dlg.SetVersion(sVersion);
	Dlg.DoModal();

	return 0;
}

void CMainFrame::ResizePane(UINT nID, CSize size, BOOL bHor)
{
	CXTPDockingPane* pPane = m_paneManager.FindPane(nID);
	if (!pPane)	return;

	if(bHor)
	{	
		pPane->SetMinTrackSize(CSize(size.cx, size.cy));
		pPane->SetMaxTrackSize(CSize(32767, size.cy));		
	}
	else
	{
		pPane->SetMinTrackSize(CSize(size.cx, size.cy));
		pPane->SetMaxTrackSize(CSize(size.cx, 32767));		
	}
	m_paneManager.RecalcFrameLayout(pPane);
	m_paneManager.NormalizeSplitters();
	pPane->SetMinTrackSize(CSize(0, 32));
	pPane->SetMaxTrackSize(CSize(32767, 32767));	
}


void CMainFrame::UpdateLockInfo()
{
	// 버전정보
	CARcBridgeApp* pApp = (CARcBridgeApp*)AfxGetApp();
	if(pApp)
	{
		CString sVersion = _T("");
		sVersion.Format("버전%s", pApp->GetStringVersion()); 
		m_wndStatusBar.SetPaneText(VERSION_TEXT, sVersion);


		CFrameWnd *pFrameWnd	= (CFrameWnd *)pApp->m_pMainWnd;
		CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
		if(pFrameWnd && pDoc)
		{
			CString sTitle = _T("");
			CString sInfo	 = _T("");
			CString sLock = pApp->GetStringLockInfo();
			CString sMaintenance = LDK::aGetMaintenanceInfo();

			if(aGetLockSerial() == _T("0097"))  //한길에서만 작동
			{
				sInfo = pDoc->m_sFileSaveInfo;
			}
			sTitle.Format("ARcBridge %s %s (%s)",sLock, sInfo, sMaintenance);

			pFrameWnd->SetTitle(sTitle);
			pFrameWnd->SetWindowText(sTitle);
		}


		//#27449
		/*
		CString sLockInfo = pApp->GetStringLockInfo();
		CString sEtcInfo = pApp->GetStringEtcInfo();
		CString sVersion = pApp->GetStringVersion();

		CString sMessage = _T("");
		if(sEtcInfo.IsEmpty()==FALSE)
		{
			sMessage.Format(" %s 버전%s / %s ", sLockInfo,sVersion,sEtcInfo);
		}
		else
		{
			sMessage.Format(" %s 버전%s ", sLockInfo,sVersion);
		}

		m_wndStatusBar.SetPaneText(VERSION_TEXT, sMessage);
		
		CDC* pDC = GetDC();
		if(pDC != NULL)
		{
			HDC hDc = pDC->GetSafeHdc();

			SIZE size;
			GetTextExtentPoint32(hDc, sMessage, sMessage.GetLength(), &size);
			long nWidth = size.cx;

			m_wndStatusBar.SetPaneInfo(VERSION_TEXT,ID_INDICATOR_VERSION, SBPS_NORMAL, nWidth);
			m_wndStatusBar.SetPaneText(VERSION_TEXT, sMessage, TRUE);
		}
	*/
	}

	m_wndStatusBar.Invalidate(TRUE);
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if(!LDK::aCheckLicenseHGKey() && nID != ID_APP_EXIT && nID !=ID_CREATE_LICENSE)
		return FALSE;

	return CXTPFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnCreateKey()
{		
	//인증 발급창
	LDK::aLicenseDlgProc(LDK::aGetLockInfo(), LDK::aGetLockResult());	
}

void CMainFrame::SwitchView( HGINT32 idView, HGINT32 idDlg )
{
	CView *pViewOld = GetActiveView();
	CView *pViewNew = NULL;

	switch(idView)
	{
	case VIEWID_DEFAULT:
		pViewNew = (CView*)m_pView;
		break;
	case VIEWID_UBICON:
		pViewNew = (CView*)m_pViewStm;
		m_pViewStm->SetDomyunView(m_pView);
		break;
	}

	if(pViewNew)
	{
		if(pViewOld == pViewNew) return;

		pViewOld->ShowWindow(SW_HIDE);
		pViewOld->SetDlgCtrlID(idView);

		pViewNew->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
		pViewNew->ShowWindow(SW_SHOW);
		SetActiveView(pViewNew);
		RecalcLayout();

		if(idView == VIEWID_DEFAULT)
		{
			CARcBridgeDoc *pDoc = (CARcBridgeDoc*)GetActiveDocument();
			CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;
			CInputDialogBase* pDlg = pIn->GetDialogByMenuID(idDlg);
			if(pDlg)
				pDlg->m_pView = (CDomyunGLView*)pViewNew;
		}
	}
}

LRESULT CMainFrame::OnSwitchView( WPARAM wParam, LPARAM lParam )
{
	HGINT32 idView = (HGINT32)wParam;
	HGINT32 idDlg = (HGINT32)lParam;
	SwitchView(idView, idDlg);

	return 0;
}

BOOL CMainFrame::DestroyWindow()
{
	// Memory leak 방지
	SwitchView(VIEWID_DEFAULT, ID_BASE1);

	return CFrameWnd::DestroyWindow();
}



/*
	현재 설정된 구조체 개수를 반환한다 (교량 개수)
*/
HGLONG32 CMainFrame::attGetCountStru()
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();

	CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;
	CARcBridgeDataStd* pStd = pIn->m_pARcBridgeDataStd;
	return pStd->GetBridgeSize();
}

/*
	현재 입력다이얼로그 체크버튼을 check/uncheck한다.
*/
void CMainFrame::attSetCheckAllBridge(BOOL bCheck)
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();

	CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;

	UINT nMenuID = pIn->GetCurrentMenuID();
	if( nMenuID < ID_STM2 || ID_STM5 < nMenuID )
		return;

	CInputStmModelBase *pDlg = reinterpret_cast<CInputStmModelBase*>(pDoc->m_pARcBridgeInputStd->GetCurrentDialog());
	pDlg->CheckDlgButton(IDC_CHECK_ALLBRIDGE, bCheck);
}
/*
	현재 입력다이얼로그 체크버튼을 check/uncheck한다.
*/
void CMainFrame::attSetCheckAllMode(BOOL bCheck)
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();

	CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;

	UINT nMenuID = pIn->GetCurrentMenuID();
	if( nMenuID < ID_STM2 || ID_STM5 < nMenuID )
		return;

 	CInputStmModelBase *pDlg = reinterpret_cast<CInputStmModelBase*>(pDoc->m_pARcBridgeInputStd->GetCurrentDialog());
	pDlg->CheckDlgButton(IDC_CHECK_ALLCORNER, bCheck);
}
/*
	현재 입력다이얼로그 체크버튼을 check/uncheck한다.
*/
void CMainFrame::attSetCheckAllCombination(BOOL bCheck)
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();

	CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;

	UINT nMenuID = pIn->GetCurrentMenuID();
	if( nMenuID < ID_STM2 || ID_STM5 < nMenuID )
		return;

 	CInputStmModelBase *pDlg = reinterpret_cast<CInputStmModelBase*>(pDoc->m_pARcBridgeInputStd->GetCurrentDialog());
	pDlg->CheckDlgButton(IDC_CHECK_ALLCOMBO, bCheck);
}

LRESULT CMainFrame::OnPageInfo( WPARAM, LPARAM )
{
// 	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
// 
// 	if(pDoc->m_bFileOpen == 0) return 0;	
// 	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;	
// 
// 	if(!m_PageInfoDlg.Create(this)) return 0;	
// 	if(!pDraw->GetDrawPageMng()->IsPageInit()) pDraw->GetDrawPageMng()->SetPageInit();
// 	long i=0; for(i=0; i<pDraw->GetDrawPageMng()->GetPagePartSize();i++)
// 	{				
// 		m_PageInfoDlg.AddPageInfo(pDraw->GetDrawPageMng()->GetPage(i)->GetPartTitle(),pDraw->GetDrawPageMng()->GetPageByIdx(i),pDraw->GetDrawPageMng()->GetPage(i)->m_nPage);
// 	}		
// 	m_PageInfoDlg.SetListData();
// 	m_PageInfoDlg.Show();

	return 0;
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_F8)
	{
		CARcBridgeStmView *pModelView = dynamic_cast<CARcBridgeStmView*>(GetActiveView());
		if(pModelView == NULL) return;

		CStmUbRenderer *pRenderer = pModelView->GetUbRenderer();
		if(pRenderer == NULL) return;

		::SetFocus(pRenderer->GetWnd());
		pRenderer->OnKeyDown(VK_F8, 0, 0);
	}

	CXTPFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}