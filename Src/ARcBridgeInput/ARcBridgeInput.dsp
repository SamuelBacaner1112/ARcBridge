# Microsoft Developer Studio Project File - Name="ARcBridgeInput" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ARcBridgeInput - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ARcBridgeInput.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ARcBridgeInput - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../../Engine/BaseEng/Include" /I "../../../../Engine/BaseComp/Include" /I "../../../../Engine/FrameEng/Include" /I "../../../../Engine/Bim3DEng/Include" /I "../../../../Lib/Ubicon_Occ/Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ARcBridgeData.lib ARcBridgeDBDraw.lib HgBaseBM.lib RCModule.lib BearingInfo.lib /nologo /subsystem:windows /dll /machine:I386 /def:".\ARcBridgeInput.def" /out:"../../CommonCompile/Output/ARcBridgeInput.dll" /implib:"../../CommonCompile/Output/ARcBridgeInput.lib" /libpath:"../../../../Engine/BaseEng/CommonCompile/Output" /libpath:"../../../../Engine/BaseComp/CommonCompile/Output" /libpath:"../../../../Engine/FrameEng/CommonCompile/Output" /libpath:"../../../../Engine/Bim3DEng/CommonCompile/Output" /libpath:"../../../../Lib/Ubicon_Occ/CommonCompile/Output" /libpath:"../../CommonCompile/Output" /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/Output"
# SUBTRACT LINK32 /pdb:none /incremental:yes /debug

!ELSEIF  "$(CFG)" == "ARcBridgeInput - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Exclude_From_Build 1
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../../Engine/BaseEng/Include" /I "../../../../Engine/BaseComp/Include" /I "../../../../Engine/FrameEng/Include" /I "../../../../Engine/Bim3DEng/Include" /I "../../../../Lib/Ubicon_Occ/Include" /D "_AFXEXT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_XTP_EXCLUDE_XML" /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ARcBridgeDataD.lib ARcBridgeDBDrawD.lib HgBaseBMD.lib RCModuleD.lib BearingInfoD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\ARcBridgeInputD.def" /out:"../../CommonCompile/OutputD/ARcBridgeInputD.dll" /implib:"../../CommonCompile/OutputD/ARcBridgeInputD.lib" /pdbtype:sept /libpath:"../../../../Engine/BaseEng/CommonCompile/OutputD" /libpath:"../../../../Engine/BaseComp/CommonCompile/OutputD" /libpath:"../../../../Engine/FrameEng/CommonCompile/OutputD" /libpath:"../../../../Engine/Bim3DEng/CommonCompile/OutputD" /libpath:"../../../../Lib/Ubicon_Occ\CommonCompile\OutputD" /libpath:"../../CommonCompile/OutputD" /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/OutputD"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ARcBridgeInput - Win32 Release"
# Name "ARcBridgeInput - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ApsSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ARcBridgeInput.cpp
# End Source File
# Begin Source File

SOURCE=.\ARcBridgeInput.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ARcBridgeInput.rc
# End Source File
# Begin Source File

SOURCE=.\ARcBridgeInputStd.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseDesignFactDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseFactDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BoringTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BridgeLevelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BridgeTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogCrackEnv.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogDom.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogEHPLink.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogHDanLength.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAddRatePageByDia.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAddRatePageByMainRebar.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAddRateSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\EarthquakeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EarthquakeFactDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EarthquakeGradeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EValueInputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FinitePileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FixShoeSelectInputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GuardWallDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\InputBaseBridgeFormFactor.cpp
# End Source File
# Begin Source File

SOURCE=.\InputBaseCrossConstruct.cpp
# End Source File
# Begin Source File

SOURCE=.\InputBaseCrossPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\InputBaseDesignBasicData.cpp
# End Source File
# Begin Source File

SOURCE=.\InputBaseGroundInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\InputBaseLineInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\InputBaseProjectInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\InputBaseShoePosition.cpp
# End Source File
# Begin Source File

SOURCE=.\InputBaseSpanConstruct.cpp
# End Source File
# Begin Source File

SOURCE=.\InputCheckArchRib.cpp
# End Source File
# Begin Source File

SOURCE=.\InputCheckEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\InputCheckGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\InputComment.cpp
# End Source File
# Begin Source File

SOURCE=.\InputDialogBase.cpp
# End Source File
# Begin Source File

SOURCE=.\InputDialogPane.cpp
# End Source File
# Begin Source File

SOURCE=.\InputGenAps.cpp
# End Source File
# Begin Source File

SOURCE=.\InputGenColumn.cpp
# End Source File
# Begin Source File

SOURCE=.\InputGenDesignVariable.cpp
# End Source File
# Begin Source File

SOURCE=.\InputGenFooting.cpp
# End Source File
# Begin Source File

SOURCE=.\InputGenFrameAnalysis.cpp
# End Source File
# Begin Source File

SOURCE=.\InputGenHyunTa.cpp
# End Source File
# Begin Source File

SOURCE=.\InputGenJongSection.cpp
# End Source File
# Begin Source File

SOURCE=.\InputGenMonument.cpp
# End Source File
# Begin Source File

SOURCE=.\InputGenPlane.cpp
# End Source File
# Begin Source File

SOURCE=.\InputGenShellAnalysis.cpp
# End Source File
# Begin Source File

SOURCE=.\InputGenSodan.cpp
# End Source File
# Begin Source File

SOURCE=.\InputGenTogong.cpp
# End Source File
# Begin Source File

SOURCE=.\InputGenWingWall.cpp
# End Source File
# Begin Source File

SOURCE=.\InputRebarAps.cpp
# End Source File
# Begin Source File

SOURCE=.\InputRebarColumn.cpp
# End Source File
# Begin Source File

SOURCE=.\InputRebarGagak.cpp
# End Source File
# Begin Source File

SOURCE=.\InputRebarJewon.cpp
# End Source File
# Begin Source File

SOURCE=.\InputRebarMain.cpp
# End Source File
# Begin Source File

SOURCE=.\InputRebarMain2.cpp
# End Source File
# Begin Source File

SOURCE=.\InputRebarMain3.cpp
# End Source File
# Begin Source File

SOURCE=.\InputRebarOthers.cpp
# End Source File
# Begin Source File

SOURCE=.\InputRebarSection.cpp
# End Source File
# Begin Source File

SOURCE=.\InputRebarWingWall.cpp
# End Source File
# Begin Source File

SOURCE=.\MidasInputProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OptimizeRebar.cpp
# End Source File
# Begin Source File

SOURCE=.\PhcPileJewonDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PostBaseLineDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RebarDanDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RebarHunchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RebarJoint1CycleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RebarJointDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RebarShearDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RebarSupportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RebarTable.cpp
# End Source File
# Begin Source File

SOURCE=.\RebarWingWallDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RoundDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SlabShapeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StirrupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TypeCycle3Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UValueInputDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ApsSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\ARcBridgeInputStd.h
# End Source File
# Begin Source File

SOURCE=.\BaseDesignFactDlg.h
# End Source File
# Begin Source File

SOURCE=.\BaseFactDlg.h
# End Source File
# Begin Source File

SOURCE=.\BoringTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\BridgeLevelDlg.h
# End Source File
# Begin Source File

SOURCE=.\BridgeTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\CheckComboBox.h
# End Source File
# Begin Source File

SOURCE=.\DialogCrackEnv.h
# End Source File
# Begin Source File

SOURCE=.\DialogDom.h
# End Source File
# Begin Source File

SOURCE=.\DialogEHPLink.h
# End Source File
# Begin Source File

SOURCE=.\DialogHDanLength.h
# End Source File
# Begin Source File

SOURCE=.\DlgAddRatePageByDia.h
# End Source File
# Begin Source File

SOURCE=.\DlgAddRatePageByMainRebar.h
# End Source File
# Begin Source File

SOURCE=.\DlgAddRateSheet.h
# End Source File
# Begin Source File

SOURCE=.\DlgProgress.h
# End Source File
# Begin Source File

SOURCE=.\EarthquakeDlg.h
# End Source File
# Begin Source File

SOURCE=.\EarthquakeFactDlg.h
# End Source File
# Begin Source File

SOURCE=.\EarthquakeGradeDlg.h
# End Source File
# Begin Source File

SOURCE=.\EValueInputDlg.h
# End Source File
# Begin Source File

SOURCE=.\FinitePileDlg.h
# End Source File
# Begin Source File

SOURCE=.\FixShoeSelectInputDlg.h
# End Source File
# Begin Source File

SOURCE=.\GuardWallDlg.h
# End Source File
# Begin Source File

SOURCE=.\ImageListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\InputBaseBridgeFormFactor.h
# End Source File
# Begin Source File

SOURCE=.\InputBaseCrossConstruct.h
# End Source File
# Begin Source File

SOURCE=.\InputBaseCrossPosition.h
# End Source File
# Begin Source File

SOURCE=.\InputBaseDesignBasicData.h
# End Source File
# Begin Source File

SOURCE=.\InputBaseGroundInfo.h
# End Source File
# Begin Source File

SOURCE=.\InputBaseLineInfo.h
# End Source File
# Begin Source File

SOURCE=.\InputBaseProjectInfo.h
# End Source File
# Begin Source File

SOURCE=.\InputBaseShoePosition.h
# End Source File
# Begin Source File

SOURCE=.\InputBaseSpanConstruct.h
# End Source File
# Begin Source File

SOURCE=.\InputCheckArchRib.h
# End Source File
# Begin Source File

SOURCE=.\InputCheckEdit.h
# End Source File
# Begin Source File

SOURCE=.\InputCheckGrid.h
# End Source File
# Begin Source File

SOURCE=.\InputComment.h
# End Source File
# Begin Source File

SOURCE=.\InputDialogBase.h
# End Source File
# Begin Source File

SOURCE=.\InputDialogPane.h
# End Source File
# Begin Source File

SOURCE=.\InputGenAps.h
# End Source File
# Begin Source File

SOURCE=.\InputGenColumn.h
# End Source File
# Begin Source File

SOURCE=.\InputGenDesignVariable.h
# End Source File
# Begin Source File

SOURCE=.\InputGenFooting.h
# End Source File
# Begin Source File

SOURCE=.\InputGenFrameAnalysis.h
# End Source File
# Begin Source File

SOURCE=.\InputGenHyunTa.h
# End Source File
# Begin Source File

SOURCE=.\InputGenJongSection.h
# End Source File
# Begin Source File

SOURCE=.\InputGenMonument.h
# End Source File
# Begin Source File

SOURCE=.\InputGenPlane.h
# End Source File
# Begin Source File

SOURCE=.\InputGenShellAnalysis.h
# End Source File
# Begin Source File

SOURCE=.\InputGenSodan.h
# End Source File
# Begin Source File

SOURCE=.\InputGenTogong.h
# End Source File
# Begin Source File

SOURCE=.\InputGenWingWall.h
# End Source File
# Begin Source File

SOURCE=.\InputRebarAps.h
# End Source File
# Begin Source File

SOURCE=.\InputRebarColumn.h
# End Source File
# Begin Source File

SOURCE=.\InputRebarGagak.h
# End Source File
# Begin Source File

SOURCE=.\InputRebarJewon.h
# End Source File
# Begin Source File

SOURCE=.\InputRebarMain.h
# End Source File
# Begin Source File

SOURCE=.\InputRebarMain2.h
# End Source File
# Begin Source File

SOURCE=.\InputRebarMain3.h
# End Source File
# Begin Source File

SOURCE=.\InputRebarOthers.h
# End Source File
# Begin Source File

SOURCE=.\InputRebarSection.h
# End Source File
# Begin Source File

SOURCE=.\InputRebarWingWall.h
# End Source File
# Begin Source File

SOURCE=.\MidasInputProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\OptimizeRebar.h
# End Source File
# Begin Source File

SOURCE=.\PhcPileJewonDlg.h
# End Source File
# Begin Source File

SOURCE=.\PostBaseLineDlg.h
# End Source File
# Begin Source File

SOURCE=.\RebarDanDlg.h
# End Source File
# Begin Source File

SOURCE=.\RebarHunchDlg.h
# End Source File
# Begin Source File

SOURCE=.\RebarJoint1CycleDlg.h
# End Source File
# Begin Source File

SOURCE=.\RebarJointDlg.h
# End Source File
# Begin Source File

SOURCE=.\RebarShearDlg.h
# End Source File
# Begin Source File

SOURCE=.\RebarSupportDlg.h
# End Source File
# Begin Source File

SOURCE=.\RebarTable.h
# End Source File
# Begin Source File

SOURCE=.\RebarWingWallDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RoundDlg.h
# End Source File
# Begin Source File

SOURCE=.\SlabShapeDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StirrupDlg.h
# End Source File
# Begin Source File

SOURCE=.\TypeCycle3Dlg.h
# End Source File
# Begin Source File

SOURCE=.\UValueInputDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ARcBridgeInput.rc2
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_r.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_11.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_12.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_13.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_14.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_15.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_17.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_18.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_19.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_20.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_21.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_22.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_23.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_25.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_26.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_27.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_28.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_29.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_30.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_31.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_32.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_33.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_34.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_35.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_36.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_37.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boring_9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Bri_Arch.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Bri_Box.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Bri_Pi.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Bri_Portal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Bri_Slab.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Bri_Trap.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Bri_Trap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Bri_V.bmp
# End Source File
# Begin Source File

SOURCE=.\res\comment.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DongB1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DongB2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ehp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\finiteLengthpile.bmp
# End Source File
# Begin Source File

SOURCE=.\res\help.bmp
# End Source File
# Begin Source File

SOURCE=.\res\help.ico
# End Source File
# Begin Source File

SOURCE=.\res\tank.bmp
# End Source File
# Begin Source File

SOURCE=.\res\trailer.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Åº¼º°è¼ö.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Åº¼º°è¼öSI.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Æ÷¾Æ½¼ºñ.bmp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section ARcBridgeInput : {2CCE51BD-933A-4913-AE6A-90869DA5C4E3}
# 	1:13:IDD_EHP_POPUP:8000
# 	2:16:Resource Include:resource.h
# 	2:21:CLASS: CDialogEHPLink:CDialogEHPLink
# 	2:13:IDD_EHP_POPUP:IDD_EHP_POPUP
# 	2:15:DialogEHPLink.h:DialogEHPLink.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:arcbridgeinput.h
# 	2:17:DialogEHPLink.cpp:DialogEHPLink.cpp
# End Section
