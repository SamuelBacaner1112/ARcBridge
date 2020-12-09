# Microsoft Developer Studio Generated NMAKE File, Based on ARcBridgeInput.dsp
!IF "$(CFG)" == ""
CFG=ARcBridgeInput - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ARcBridgeInput - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ARcBridgeInput - Win32 Release" && "$(CFG)" != "ARcBridgeInput - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ARcBridgeInput.mak" CFG="ARcBridgeInput - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ARcBridgeInput - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ARcBridgeInput - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ARcBridgeInput - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\CommonCompile\Output\ARcBridgeInput.dll"

!ELSE 

ALL : "ARcBridgeCalculate - Win32 Release" "..\..\CommonCompile\Output\ARcBridgeInput.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ARcBridgeCalculate - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ApsSettingDlg.obj"
	-@erase "$(INTDIR)\ARcBridgeInput.obj"
	-@erase "$(INTDIR)\ARcBridgeInput.pch"
	-@erase "$(INTDIR)\ARcBridgeInput.res"
	-@erase "$(INTDIR)\ARcBridgeInputStd.obj"
	-@erase "$(INTDIR)\BaseDesignFactDlg.obj"
	-@erase "$(INTDIR)\BaseFactDlg.obj"
	-@erase "$(INTDIR)\BoringTypeDlg.obj"
	-@erase "$(INTDIR)\BridgeLevelDlg.obj"
	-@erase "$(INTDIR)\BridgeTypeDlg.obj"
	-@erase "$(INTDIR)\CheckComboBox.obj"
	-@erase "$(INTDIR)\DialogCrackEnv.obj"
	-@erase "$(INTDIR)\DialogDom.obj"
	-@erase "$(INTDIR)\DialogEHPLink.obj"
	-@erase "$(INTDIR)\DlgAddRatePageByDia.obj"
	-@erase "$(INTDIR)\DlgAddRatePageByMainRebar.obj"
	-@erase "$(INTDIR)\DlgAddRateSheet.obj"
	-@erase "$(INTDIR)\DlgProgress.obj"
	-@erase "$(INTDIR)\EarthquakeDlg.obj"
	-@erase "$(INTDIR)\EarthquakeFactDlg.obj"
	-@erase "$(INTDIR)\EarthquakeGradeDlg.obj"
	-@erase "$(INTDIR)\EValueInputDlg.obj"
	-@erase "$(INTDIR)\FinitePileDlg.obj"
	-@erase "$(INTDIR)\FixShoeSelectInputDlg.obj"
	-@erase "$(INTDIR)\GuardWallDlg.obj"
	-@erase "$(INTDIR)\ImageListCtrl.obj"
	-@erase "$(INTDIR)\InputBaseBridgeFormFactor.obj"
	-@erase "$(INTDIR)\InputBaseCrossConstruct.obj"
	-@erase "$(INTDIR)\InputBaseCrossPosition.obj"
	-@erase "$(INTDIR)\InputBaseDesignBasicData.obj"
	-@erase "$(INTDIR)\InputBaseGroundInfo.obj"
	-@erase "$(INTDIR)\InputBaseLineInfo.obj"
	-@erase "$(INTDIR)\InputBaseProjectInfo.obj"
	-@erase "$(INTDIR)\InputBaseShoePosition.obj"
	-@erase "$(INTDIR)\InputBaseSpanConstruct.obj"
	-@erase "$(INTDIR)\InputCheckArchRib.obj"
	-@erase "$(INTDIR)\InputCheckEdit.obj"
	-@erase "$(INTDIR)\InputCheckGrid.obj"
	-@erase "$(INTDIR)\InputComment.obj"
	-@erase "$(INTDIR)\InputDialogBase.obj"
	-@erase "$(INTDIR)\InputDialogPane.obj"
	-@erase "$(INTDIR)\InputGenAps.obj"
	-@erase "$(INTDIR)\InputGenColumn.obj"
	-@erase "$(INTDIR)\InputGenDesignVariable.obj"
	-@erase "$(INTDIR)\InputGenFooting.obj"
	-@erase "$(INTDIR)\InputGenFrameAnalysis.obj"
	-@erase "$(INTDIR)\InputGenHyunTa.obj"
	-@erase "$(INTDIR)\InputGenJongSection.obj"
	-@erase "$(INTDIR)\InputGenMonument.obj"
	-@erase "$(INTDIR)\InputGenPlane.obj"
	-@erase "$(INTDIR)\InputGenShellAnalysis.obj"
	-@erase "$(INTDIR)\InputGenSodan.obj"
	-@erase "$(INTDIR)\InputGenTogong.obj"
	-@erase "$(INTDIR)\InputGenWingWall.obj"
	-@erase "$(INTDIR)\InputRebarAps.obj"
	-@erase "$(INTDIR)\InputRebarColumn.obj"
	-@erase "$(INTDIR)\InputRebarGagak.obj"
	-@erase "$(INTDIR)\InputRebarJewon.obj"
	-@erase "$(INTDIR)\InputRebarMain.obj"
	-@erase "$(INTDIR)\InputRebarMain2.obj"
	-@erase "$(INTDIR)\InputRebarMain3.obj"
	-@erase "$(INTDIR)\InputRebarOthers.obj"
	-@erase "$(INTDIR)\InputRebarSection.obj"
	-@erase "$(INTDIR)\InputRebarWingWall.obj"
	-@erase "$(INTDIR)\MidasInputProgressDlg.obj"
	-@erase "$(INTDIR)\OptimizeRebar.obj"
	-@erase "$(INTDIR)\PhcPileJewonDlg.obj"
	-@erase "$(INTDIR)\PostBaseLineDlg.obj"
	-@erase "$(INTDIR)\RebarDanDlg.obj"
	-@erase "$(INTDIR)\RebarHunchDlg.obj"
	-@erase "$(INTDIR)\RebarJoint1CycleDlg.obj"
	-@erase "$(INTDIR)\RebarJointDlg.obj"
	-@erase "$(INTDIR)\RebarShearDlg.obj"
	-@erase "$(INTDIR)\RebarSupportDlg.obj"
	-@erase "$(INTDIR)\RebarTable.obj"
	-@erase "$(INTDIR)\RebarWingWallDlg.obj"
	-@erase "$(INTDIR)\RoundDlg.obj"
	-@erase "$(INTDIR)\SlabShapeDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StirrupDlg.obj"
	-@erase "$(INTDIR)\TypeCycle3Dlg.obj"
	-@erase "$(INTDIR)\UValueInputDlg.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\..\CommonCompile\lib\ARcBridgeInput.exp"
	-@erase "..\..\CommonCompile\lib\ARcBridgeInput.lib"
	-@erase "..\..\CommonCompile\Output\ARcBridgeInput.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeInput.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridgeInput.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridgeInput.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=XTP8610Lib.lib ARcBridgeData.lib ARcBridgeDBDraw.lib HgBaseBM.lib RCModule.lib BearingInfo.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\ARcBridgeInput.pdb" /machine:I386 /def:".\ARcBridgeInput.def" /out:"../../CommonCompile/Output/ARcBridgeInput.dll" /implib:"../../CommonCompile/lib/ARcBridgeInput.lib" /libpath:"$(HANGIL_ENGINE)/bin/lib" /libpath:"$(HANGIL_MODULE)/bin/lib" /libpath:"$(HANGIL_FRAME)/bin/lib" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/release" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/release" /libpath:"../../CommonCompile/lib" 
LINK32_OBJS= \
	"$(INTDIR)\ApsSettingDlg.obj" \
	"$(INTDIR)\ARcBridgeInput.obj" \
	"$(INTDIR)\ARcBridgeInputStd.obj" \
	"$(INTDIR)\BaseDesignFactDlg.obj" \
	"$(INTDIR)\BaseFactDlg.obj" \
	"$(INTDIR)\BoringTypeDlg.obj" \
	"$(INTDIR)\BridgeLevelDlg.obj" \
	"$(INTDIR)\BridgeTypeDlg.obj" \
	"$(INTDIR)\CheckComboBox.obj" \
	"$(INTDIR)\DialogCrackEnv.obj" \
	"$(INTDIR)\DialogDom.obj" \
	"$(INTDIR)\DialogEHPLink.obj" \
	"$(INTDIR)\DlgAddRatePageByDia.obj" \
	"$(INTDIR)\DlgAddRatePageByMainRebar.obj" \
	"$(INTDIR)\DlgAddRateSheet.obj" \
	"$(INTDIR)\DlgProgress.obj" \
	"$(INTDIR)\EarthquakeDlg.obj" \
	"$(INTDIR)\EarthquakeFactDlg.obj" \
	"$(INTDIR)\EarthquakeGradeDlg.obj" \
	"$(INTDIR)\EValueInputDlg.obj" \
	"$(INTDIR)\FinitePileDlg.obj" \
	"$(INTDIR)\FixShoeSelectInputDlg.obj" \
	"$(INTDIR)\GuardWallDlg.obj" \
	"$(INTDIR)\ImageListCtrl.obj" \
	"$(INTDIR)\InputBaseBridgeFormFactor.obj" \
	"$(INTDIR)\InputBaseCrossConstruct.obj" \
	"$(INTDIR)\InputBaseCrossPosition.obj" \
	"$(INTDIR)\InputBaseDesignBasicData.obj" \
	"$(INTDIR)\InputBaseGroundInfo.obj" \
	"$(INTDIR)\InputBaseLineInfo.obj" \
	"$(INTDIR)\InputBaseProjectInfo.obj" \
	"$(INTDIR)\InputBaseShoePosition.obj" \
	"$(INTDIR)\InputBaseSpanConstruct.obj" \
	"$(INTDIR)\InputCheckArchRib.obj" \
	"$(INTDIR)\InputCheckEdit.obj" \
	"$(INTDIR)\InputCheckGrid.obj" \
	"$(INTDIR)\InputComment.obj" \
	"$(INTDIR)\InputDialogBase.obj" \
	"$(INTDIR)\InputDialogPane.obj" \
	"$(INTDIR)\InputGenAps.obj" \
	"$(INTDIR)\InputGenColumn.obj" \
	"$(INTDIR)\InputGenDesignVariable.obj" \
	"$(INTDIR)\InputGenFooting.obj" \
	"$(INTDIR)\InputGenFrameAnalysis.obj" \
	"$(INTDIR)\InputGenHyunTa.obj" \
	"$(INTDIR)\InputGenJongSection.obj" \
	"$(INTDIR)\InputGenMonument.obj" \
	"$(INTDIR)\InputGenPlane.obj" \
	"$(INTDIR)\InputGenShellAnalysis.obj" \
	"$(INTDIR)\InputGenSodan.obj" \
	"$(INTDIR)\InputGenTogong.obj" \
	"$(INTDIR)\InputGenWingWall.obj" \
	"$(INTDIR)\InputRebarAps.obj" \
	"$(INTDIR)\InputRebarColumn.obj" \
	"$(INTDIR)\InputRebarGagak.obj" \
	"$(INTDIR)\InputRebarJewon.obj" \
	"$(INTDIR)\InputRebarMain.obj" \
	"$(INTDIR)\InputRebarMain2.obj" \
	"$(INTDIR)\InputRebarMain3.obj" \
	"$(INTDIR)\InputRebarOthers.obj" \
	"$(INTDIR)\InputRebarSection.obj" \
	"$(INTDIR)\InputRebarWingWall.obj" \
	"$(INTDIR)\MidasInputProgressDlg.obj" \
	"$(INTDIR)\OptimizeRebar.obj" \
	"$(INTDIR)\PhcPileJewonDlg.obj" \
	"$(INTDIR)\PostBaseLineDlg.obj" \
	"$(INTDIR)\RebarDanDlg.obj" \
	"$(INTDIR)\RebarHunchDlg.obj" \
	"$(INTDIR)\RebarJoint1CycleDlg.obj" \
	"$(INTDIR)\RebarJointDlg.obj" \
	"$(INTDIR)\RebarShearDlg.obj" \
	"$(INTDIR)\RebarSupportDlg.obj" \
	"$(INTDIR)\RebarTable.obj" \
	"$(INTDIR)\RebarWingWallDlg.obj" \
	"$(INTDIR)\RoundDlg.obj" \
	"$(INTDIR)\SlabShapeDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StirrupDlg.obj" \
	"$(INTDIR)\TypeCycle3Dlg.obj" \
	"$(INTDIR)\UValueInputDlg.obj" \
	"$(INTDIR)\ARcBridgeInput.res" \
	"..\..\CommonCompile\lib\ARcBridgeCalculate.lib"

"..\..\CommonCompile\Output\ARcBridgeInput.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ARcBridgeInput - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\CommonCompile\OutputD\ARcBridgeInputD.dll"

!ELSE 

ALL : "ARcBridgeCalculate - Win32 Debug" "..\..\CommonCompile\OutputD\ARcBridgeInputD.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ARcBridgeCalculate - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ApsSettingDlg.obj"
	-@erase "$(INTDIR)\ARcBridgeInput.obj"
	-@erase "$(INTDIR)\ARcBridgeInput.pch"
	-@erase "$(INTDIR)\ARcBridgeInput.res"
	-@erase "$(INTDIR)\ARcBridgeInputStd.obj"
	-@erase "$(INTDIR)\BaseDesignFactDlg.obj"
	-@erase "$(INTDIR)\BaseFactDlg.obj"
	-@erase "$(INTDIR)\BoringTypeDlg.obj"
	-@erase "$(INTDIR)\BridgeLevelDlg.obj"
	-@erase "$(INTDIR)\BridgeTypeDlg.obj"
	-@erase "$(INTDIR)\CheckComboBox.obj"
	-@erase "$(INTDIR)\DialogCrackEnv.obj"
	-@erase "$(INTDIR)\DialogDom.obj"
	-@erase "$(INTDIR)\DialogEHPLink.obj"
	-@erase "$(INTDIR)\DlgAddRatePageByDia.obj"
	-@erase "$(INTDIR)\DlgAddRatePageByMainRebar.obj"
	-@erase "$(INTDIR)\DlgAddRateSheet.obj"
	-@erase "$(INTDIR)\DlgProgress.obj"
	-@erase "$(INTDIR)\EarthquakeDlg.obj"
	-@erase "$(INTDIR)\EarthquakeFactDlg.obj"
	-@erase "$(INTDIR)\EarthquakeGradeDlg.obj"
	-@erase "$(INTDIR)\EValueInputDlg.obj"
	-@erase "$(INTDIR)\FinitePileDlg.obj"
	-@erase "$(INTDIR)\FixShoeSelectInputDlg.obj"
	-@erase "$(INTDIR)\GuardWallDlg.obj"
	-@erase "$(INTDIR)\ImageListCtrl.obj"
	-@erase "$(INTDIR)\InputBaseBridgeFormFactor.obj"
	-@erase "$(INTDIR)\InputBaseCrossConstruct.obj"
	-@erase "$(INTDIR)\InputBaseCrossPosition.obj"
	-@erase "$(INTDIR)\InputBaseDesignBasicData.obj"
	-@erase "$(INTDIR)\InputBaseGroundInfo.obj"
	-@erase "$(INTDIR)\InputBaseLineInfo.obj"
	-@erase "$(INTDIR)\InputBaseProjectInfo.obj"
	-@erase "$(INTDIR)\InputBaseShoePosition.obj"
	-@erase "$(INTDIR)\InputBaseSpanConstruct.obj"
	-@erase "$(INTDIR)\InputCheckArchRib.obj"
	-@erase "$(INTDIR)\InputCheckEdit.obj"
	-@erase "$(INTDIR)\InputCheckGrid.obj"
	-@erase "$(INTDIR)\InputComment.obj"
	-@erase "$(INTDIR)\InputDialogBase.obj"
	-@erase "$(INTDIR)\InputDialogPane.obj"
	-@erase "$(INTDIR)\InputGenAps.obj"
	-@erase "$(INTDIR)\InputGenColumn.obj"
	-@erase "$(INTDIR)\InputGenDesignVariable.obj"
	-@erase "$(INTDIR)\InputGenFooting.obj"
	-@erase "$(INTDIR)\InputGenFrameAnalysis.obj"
	-@erase "$(INTDIR)\InputGenHyunTa.obj"
	-@erase "$(INTDIR)\InputGenJongSection.obj"
	-@erase "$(INTDIR)\InputGenMonument.obj"
	-@erase "$(INTDIR)\InputGenPlane.obj"
	-@erase "$(INTDIR)\InputGenShellAnalysis.obj"
	-@erase "$(INTDIR)\InputGenSodan.obj"
	-@erase "$(INTDIR)\InputGenTogong.obj"
	-@erase "$(INTDIR)\InputGenWingWall.obj"
	-@erase "$(INTDIR)\InputRebarAps.obj"
	-@erase "$(INTDIR)\InputRebarColumn.obj"
	-@erase "$(INTDIR)\InputRebarGagak.obj"
	-@erase "$(INTDIR)\InputRebarJewon.obj"
	-@erase "$(INTDIR)\InputRebarMain.obj"
	-@erase "$(INTDIR)\InputRebarMain2.obj"
	-@erase "$(INTDIR)\InputRebarMain3.obj"
	-@erase "$(INTDIR)\InputRebarOthers.obj"
	-@erase "$(INTDIR)\InputRebarSection.obj"
	-@erase "$(INTDIR)\InputRebarWingWall.obj"
	-@erase "$(INTDIR)\MidasInputProgressDlg.obj"
	-@erase "$(INTDIR)\OptimizeRebar.obj"
	-@erase "$(INTDIR)\PhcPileJewonDlg.obj"
	-@erase "$(INTDIR)\PostBaseLineDlg.obj"
	-@erase "$(INTDIR)\RebarDanDlg.obj"
	-@erase "$(INTDIR)\RebarHunchDlg.obj"
	-@erase "$(INTDIR)\RebarJoint1CycleDlg.obj"
	-@erase "$(INTDIR)\RebarJointDlg.obj"
	-@erase "$(INTDIR)\RebarShearDlg.obj"
	-@erase "$(INTDIR)\RebarSupportDlg.obj"
	-@erase "$(INTDIR)\RebarTable.obj"
	-@erase "$(INTDIR)\RebarWingWallDlg.obj"
	-@erase "$(INTDIR)\RoundDlg.obj"
	-@erase "$(INTDIR)\SlabShapeDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StirrupDlg.obj"
	-@erase "$(INTDIR)\TypeCycle3Dlg.obj"
	-@erase "$(INTDIR)\UValueInputDlg.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ARcBridgeInputD.pdb"
	-@erase "..\..\CommonCompile\libD\ARcBridgeInputD.exp"
	-@erase "..\..\CommonCompile\libD\ARcBridgeInputD.lib"
	-@erase "..\..\CommonCompile\OutputD\ARcBridgeInputD.dll"
	-@erase "..\..\CommonCompile\OutputD\ARcBridgeInputD.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeInput.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridgeInput.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridgeInput.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=XTP8610Libd.lib ARcBridgeDataD.lib ARcBridgeDBDrawD.lib HgBaseBMD.lib RCModuleD.lib BearingInfoD.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\ARcBridgeInputD.pdb" /debug /machine:I386 /def:".\ARcBridgeInputD.def" /out:"../../CommonCompile/OutputD/ARcBridgeInputD.dll" /implib:"../../CommonCompile/libD/ARcBridgeInputD.lib" /pdbtype:sept /libpath:"$(HANGIL_ENGINE)/bin/libD" /libpath:"$(HANGIL_MODULE)/bin/libD" /libpath:"$(HANGIL_FRAME)/bin/libD" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/debug" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/debug" /libpath:"../../CommonCompile/libD" 
LINK32_OBJS= \
	"$(INTDIR)\ApsSettingDlg.obj" \
	"$(INTDIR)\ARcBridgeInput.obj" \
	"$(INTDIR)\ARcBridgeInputStd.obj" \
	"$(INTDIR)\BaseDesignFactDlg.obj" \
	"$(INTDIR)\BaseFactDlg.obj" \
	"$(INTDIR)\BoringTypeDlg.obj" \
	"$(INTDIR)\BridgeLevelDlg.obj" \
	"$(INTDIR)\BridgeTypeDlg.obj" \
	"$(INTDIR)\CheckComboBox.obj" \
	"$(INTDIR)\DialogCrackEnv.obj" \
	"$(INTDIR)\DialogDom.obj" \
	"$(INTDIR)\DialogEHPLink.obj" \
	"$(INTDIR)\DlgAddRatePageByDia.obj" \
	"$(INTDIR)\DlgAddRatePageByMainRebar.obj" \
	"$(INTDIR)\DlgAddRateSheet.obj" \
	"$(INTDIR)\DlgProgress.obj" \
	"$(INTDIR)\EarthquakeDlg.obj" \
	"$(INTDIR)\EarthquakeFactDlg.obj" \
	"$(INTDIR)\EarthquakeGradeDlg.obj" \
	"$(INTDIR)\EValueInputDlg.obj" \
	"$(INTDIR)\FinitePileDlg.obj" \
	"$(INTDIR)\FixShoeSelectInputDlg.obj" \
	"$(INTDIR)\GuardWallDlg.obj" \
	"$(INTDIR)\ImageListCtrl.obj" \
	"$(INTDIR)\InputBaseBridgeFormFactor.obj" \
	"$(INTDIR)\InputBaseCrossConstruct.obj" \
	"$(INTDIR)\InputBaseCrossPosition.obj" \
	"$(INTDIR)\InputBaseDesignBasicData.obj" \
	"$(INTDIR)\InputBaseGroundInfo.obj" \
	"$(INTDIR)\InputBaseLineInfo.obj" \
	"$(INTDIR)\InputBaseProjectInfo.obj" \
	"$(INTDIR)\InputBaseShoePosition.obj" \
	"$(INTDIR)\InputBaseSpanConstruct.obj" \
	"$(INTDIR)\InputCheckArchRib.obj" \
	"$(INTDIR)\InputCheckEdit.obj" \
	"$(INTDIR)\InputCheckGrid.obj" \
	"$(INTDIR)\InputComment.obj" \
	"$(INTDIR)\InputDialogBase.obj" \
	"$(INTDIR)\InputDialogPane.obj" \
	"$(INTDIR)\InputGenAps.obj" \
	"$(INTDIR)\InputGenColumn.obj" \
	"$(INTDIR)\InputGenDesignVariable.obj" \
	"$(INTDIR)\InputGenFooting.obj" \
	"$(INTDIR)\InputGenFrameAnalysis.obj" \
	"$(INTDIR)\InputGenHyunTa.obj" \
	"$(INTDIR)\InputGenJongSection.obj" \
	"$(INTDIR)\InputGenMonument.obj" \
	"$(INTDIR)\InputGenPlane.obj" \
	"$(INTDIR)\InputGenShellAnalysis.obj" \
	"$(INTDIR)\InputGenSodan.obj" \
	"$(INTDIR)\InputGenTogong.obj" \
	"$(INTDIR)\InputGenWingWall.obj" \
	"$(INTDIR)\InputRebarAps.obj" \
	"$(INTDIR)\InputRebarColumn.obj" \
	"$(INTDIR)\InputRebarGagak.obj" \
	"$(INTDIR)\InputRebarJewon.obj" \
	"$(INTDIR)\InputRebarMain.obj" \
	"$(INTDIR)\InputRebarMain2.obj" \
	"$(INTDIR)\InputRebarMain3.obj" \
	"$(INTDIR)\InputRebarOthers.obj" \
	"$(INTDIR)\InputRebarSection.obj" \
	"$(INTDIR)\InputRebarWingWall.obj" \
	"$(INTDIR)\MidasInputProgressDlg.obj" \
	"$(INTDIR)\OptimizeRebar.obj" \
	"$(INTDIR)\PhcPileJewonDlg.obj" \
	"$(INTDIR)\PostBaseLineDlg.obj" \
	"$(INTDIR)\RebarDanDlg.obj" \
	"$(INTDIR)\RebarHunchDlg.obj" \
	"$(INTDIR)\RebarJoint1CycleDlg.obj" \
	"$(INTDIR)\RebarJointDlg.obj" \
	"$(INTDIR)\RebarShearDlg.obj" \
	"$(INTDIR)\RebarSupportDlg.obj" \
	"$(INTDIR)\RebarTable.obj" \
	"$(INTDIR)\RebarWingWallDlg.obj" \
	"$(INTDIR)\RoundDlg.obj" \
	"$(INTDIR)\SlabShapeDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StirrupDlg.obj" \
	"$(INTDIR)\TypeCycle3Dlg.obj" \
	"$(INTDIR)\UValueInputDlg.obj" \
	"$(INTDIR)\ARcBridgeInput.res" \
	"..\..\CommonCompile\libD\ARcBridgeCalculateD.lib"

"..\..\CommonCompile\OutputD\ARcBridgeInputD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ARcBridgeInput.dep")
!INCLUDE "ARcBridgeInput.dep"
!ELSE 
!MESSAGE Warning: cannot find "ARcBridgeInput.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ARcBridgeInput - Win32 Release" || "$(CFG)" == "ARcBridgeInput - Win32 Debug"
SOURCE=.\ApsSettingDlg.cpp

"$(INTDIR)\ApsSettingDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\ARcBridgeInput.cpp

"$(INTDIR)\ARcBridgeInput.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\ARcBridgeInput.rc

"$(INTDIR)\ARcBridgeInput.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ARcBridgeInputStd.cpp

"$(INTDIR)\ARcBridgeInputStd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\BaseDesignFactDlg.cpp

"$(INTDIR)\BaseDesignFactDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\BaseFactDlg.cpp

"$(INTDIR)\BaseFactDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\BoringTypeDlg.cpp

"$(INTDIR)\BoringTypeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\BridgeLevelDlg.cpp

"$(INTDIR)\BridgeLevelDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\BridgeTypeDlg.cpp

"$(INTDIR)\BridgeTypeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\CheckComboBox.cpp

"$(INTDIR)\CheckComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\DialogCrackEnv.cpp

"$(INTDIR)\DialogCrackEnv.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\DialogDom.cpp

"$(INTDIR)\DialogDom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\DialogEHPLink.cpp

"$(INTDIR)\DialogEHPLink.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\DlgAddRatePageByDia.cpp

"$(INTDIR)\DlgAddRatePageByDia.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\DlgAddRatePageByMainRebar.cpp

"$(INTDIR)\DlgAddRatePageByMainRebar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\DlgAddRateSheet.cpp

"$(INTDIR)\DlgAddRateSheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\DlgProgress.cpp

"$(INTDIR)\DlgProgress.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\EarthquakeDlg.cpp

"$(INTDIR)\EarthquakeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\EarthquakeFactDlg.cpp

"$(INTDIR)\EarthquakeFactDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\EarthquakeGradeDlg.cpp

"$(INTDIR)\EarthquakeGradeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\EValueInputDlg.cpp

"$(INTDIR)\EValueInputDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\FinitePileDlg.cpp

"$(INTDIR)\FinitePileDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\FixShoeSelectInputDlg.cpp

"$(INTDIR)\FixShoeSelectInputDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\GuardWallDlg.cpp

"$(INTDIR)\GuardWallDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\ImageListCtrl.cpp

"$(INTDIR)\ImageListCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputBaseBridgeFormFactor.cpp

"$(INTDIR)\InputBaseBridgeFormFactor.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputBaseCrossConstruct.cpp

"$(INTDIR)\InputBaseCrossConstruct.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputBaseCrossPosition.cpp

"$(INTDIR)\InputBaseCrossPosition.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputBaseDesignBasicData.cpp

"$(INTDIR)\InputBaseDesignBasicData.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputBaseGroundInfo.cpp

"$(INTDIR)\InputBaseGroundInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputBaseLineInfo.cpp

"$(INTDIR)\InputBaseLineInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputBaseProjectInfo.cpp

"$(INTDIR)\InputBaseProjectInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputBaseShoePosition.cpp

"$(INTDIR)\InputBaseShoePosition.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputBaseSpanConstruct.cpp

"$(INTDIR)\InputBaseSpanConstruct.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputCheckArchRib.cpp

"$(INTDIR)\InputCheckArchRib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputCheckEdit.cpp

"$(INTDIR)\InputCheckEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputCheckGrid.cpp

"$(INTDIR)\InputCheckGrid.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputComment.cpp

"$(INTDIR)\InputComment.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputDialogBase.cpp

"$(INTDIR)\InputDialogBase.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputDialogPane.cpp

"$(INTDIR)\InputDialogPane.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputGenAps.cpp

"$(INTDIR)\InputGenAps.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputGenColumn.cpp

"$(INTDIR)\InputGenColumn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputGenDesignVariable.cpp

"$(INTDIR)\InputGenDesignVariable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputGenFooting.cpp

"$(INTDIR)\InputGenFooting.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputGenFrameAnalysis.cpp

"$(INTDIR)\InputGenFrameAnalysis.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputGenHyunTa.cpp

"$(INTDIR)\InputGenHyunTa.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputGenJongSection.cpp

"$(INTDIR)\InputGenJongSection.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputGenMonument.cpp

"$(INTDIR)\InputGenMonument.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputGenPlane.cpp

"$(INTDIR)\InputGenPlane.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputGenShellAnalysis.cpp

"$(INTDIR)\InputGenShellAnalysis.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputGenSodan.cpp

"$(INTDIR)\InputGenSodan.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputGenTogong.cpp

"$(INTDIR)\InputGenTogong.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputGenWingWall.cpp

"$(INTDIR)\InputGenWingWall.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputRebarAps.cpp

"$(INTDIR)\InputRebarAps.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputRebarColumn.cpp

"$(INTDIR)\InputRebarColumn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputRebarGagak.cpp

"$(INTDIR)\InputRebarGagak.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputRebarJewon.cpp

"$(INTDIR)\InputRebarJewon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputRebarMain.cpp

"$(INTDIR)\InputRebarMain.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputRebarMain2.cpp

"$(INTDIR)\InputRebarMain2.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputRebarMain3.cpp

"$(INTDIR)\InputRebarMain3.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputRebarOthers.cpp

"$(INTDIR)\InputRebarOthers.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputRebarSection.cpp

"$(INTDIR)\InputRebarSection.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\InputRebarWingWall.cpp

"$(INTDIR)\InputRebarWingWall.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\MidasInputProgressDlg.cpp

"$(INTDIR)\MidasInputProgressDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\OptimizeRebar.cpp

"$(INTDIR)\OptimizeRebar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\PhcPileJewonDlg.cpp

"$(INTDIR)\PhcPileJewonDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\PostBaseLineDlg.cpp

"$(INTDIR)\PostBaseLineDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\RebarDanDlg.cpp

"$(INTDIR)\RebarDanDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\RebarHunchDlg.cpp

"$(INTDIR)\RebarHunchDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\RebarJoint1CycleDlg.cpp

"$(INTDIR)\RebarJoint1CycleDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\RebarJointDlg.cpp

"$(INTDIR)\RebarJointDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\RebarShearDlg.cpp

"$(INTDIR)\RebarShearDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\RebarSupportDlg.cpp

"$(INTDIR)\RebarSupportDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\RebarTable.cpp

"$(INTDIR)\RebarTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\RebarWingWallDlg.cpp

"$(INTDIR)\RebarWingWallDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\RoundDlg.cpp

"$(INTDIR)\RoundDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\SlabShapeDlg.cpp

"$(INTDIR)\SlabShapeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ARcBridgeInput - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeInput.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ARcBridgeInput.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ARcBridgeInput - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeInput.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ARcBridgeInput.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StirrupDlg.cpp

"$(INTDIR)\StirrupDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\TypeCycle3Dlg.cpp

"$(INTDIR)\TypeCycle3Dlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


SOURCE=.\UValueInputDlg.cpp

"$(INTDIR)\UValueInputDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeInput.pch"


!IF  "$(CFG)" == "ARcBridgeInput - Win32 Release"

"ARcBridgeCalculate - Win32 Release" : 
   cd "\Hangil_Project\ARcBridge\INDLL\ARcBridgeCalculate"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeCalculate.mak CFG="ARcBridgeCalculate - Win32 Release" 
   cd "..\ARcBridgeInput"

"ARcBridgeCalculate - Win32 ReleaseCLEAN" : 
   cd "\Hangil_Project\ARcBridge\INDLL\ARcBridgeCalculate"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeCalculate.mak CFG="ARcBridgeCalculate - Win32 Release" RECURSE=1 CLEAN 
   cd "..\ARcBridgeInput"

!ELSEIF  "$(CFG)" == "ARcBridgeInput - Win32 Debug"

"ARcBridgeCalculate - Win32 Debug" : 
   cd "\Hangil_Project\ARcBridge\INDLL\ARcBridgeCalculate"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeCalculate.mak CFG="ARcBridgeCalculate - Win32 Debug" 
   cd "..\ARcBridgeInput"

"ARcBridgeCalculate - Win32 DebugCLEAN" : 
   cd "\Hangil_Project\ARcBridge\INDLL\ARcBridgeCalculate"
   $(MAKE) /$(MAKEFLAGS) /F .\ARcBridgeCalculate.mak CFG="ARcBridgeCalculate - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\ARcBridgeInput"

!ENDIF 


!ENDIF 

