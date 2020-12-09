# Microsoft Developer Studio Project File - Name="ARcBridge" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ARcBridge - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ARcBridge.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ARcBridge.mak" CFG="ARcBridge - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ARcBridge - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ARcBridge - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ARcBridge - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../Engine/BaseEng/Include" /I "../Include" /I "../../../Engine/BaseComp/Include" /I "../../../Engine/FrameEng/Include" /I "../../../Engine/Bim3DEng/Include" /I "../../../Lib/Ubicon_Occ/Include" /I "../../../LibInclude" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "WNT" /FR /Yu"stdafx.h" /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /i "../../../Lib/XTPToolkitPro/source" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 CommonBridge.lib opengl32.lib glu32.lib ARcBridgeDrawingOption.lib ARcBridgeInput.lib DrawManage.lib ARcBridgeData.lib ARoadConfig.lib ARcBridgeDrawing.lib ARcBridgeOutBM.lib ARcBridgeOutDesign.lib AException.lib ARcBridgeDBDraw.lib shlwapi.lib HgBaseBM.lib ShareWithADManager.lib RCModule.lib BearingInfo.lib ARcBridgeCalculate.lib UbGround.lib HGFrame.lib HGRender.lib TKBrep.lib TKBool.lib TKernel.lib TKFeat.lib TKFillet.lib TKG3d.lib TKGeomAlgo.lib TKGeomBase.lib TKHLR.lib TKMath.lib TKOffset.lib TKPrim.lib TKTopAlgo.lib TKG2d.lib /nologo /subsystem:windows /machine:I386 /out:"../CommonCompile/Output/ARcBridge.exe" /libpath:"../../../Engine/BaseEng/CommonCompile/Output" /libpath:"../../../Engine/BaseComp/CommonCompile/Output" /libpath:"../../../Engine/FrameEng/CommonCompile/Output" /libpath:"../../../Engine/Bim3DEng/CommonCompile/Output" /libpath:"../../../Lib/Ubicon_Occ/CommonCompile/Output" /libpath:"../CommonCompile/Output" /libpath:"../../../Lib" /libpath:"../../../Lib/XTPToolkitPro/CommonCompile/Output"
# SUBTRACT LINK32 /pdb:none /incremental:yes /debug

!ELSEIF  "$(CFG)" == "ARcBridge - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../Engine/BaseEng/Include" /I "../Include" /I "../../../Engine/BaseComp/Include" /I "../../../Engine/FrameEng/Include" /I "../../../Engine/Bim3DEng/Include" /I "../../../Lib/Ubicon_Occ/Include" /I "../../../LibInclude" /D "WNT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_XTP_EXCLUDE_XML" /FR /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /i "../../../Lib/XTPToolkitPro/source" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 CommonBridgeD.lib opengl32.lib glu32.lib ARcBridgeDrawingOptionD.lib ARcBridgeInputD.lib DrawManageD.lib ARcBridgeDataD.lib ARoadConfigD.lib ARcBridgeDrawingD.lib ARcBridgeOutBMD.lib ARcBridgeOutDesignD.lib AExceptionD.lib ARcBridgeDBDrawD.lib shlwapi.lib HgBaseBMD.lib ShareWithADManagerD.lib RCModuleD.lib BearingInfoD.lib ARcBridgeCalculateD.lib UbGroundD.lib HGFrameD.lib HGRenderD.lib TKBrep.lib TKBool.lib TKernel.lib TKFeat.lib TKFillet.lib TKG3d.lib TKGeomAlgo.lib TKGeomBase.lib TKHLR.lib TKMath.lib TKOffset.lib TKPrim.lib TKTopAlgo.lib TKG2d.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../CommonCompile/OutputD/ARcBridge.exe" /pdbtype:sept /libpath:"../../../Engine/BaseEng/CommonCompile/OutputD" /libpath:"../../../Engine/BaseComp/CommonCompile/OutputD" /libpath:"../../../Engine/FrameEng/CommonCompile/OutputD" /libpath:"../../../Engine/Bim3DEng/CommonCompile/OutputD" /libpath:"../../../Lib/Ubicon_Occ\CommonCompile\OutputD" /libpath:"../CommonCompile/OutputD" /libpath:"../../../Lib" /libpath:"../../../Lib/XTPToolkitPro/CommonCompile/OutputD"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ARcBridge - Win32 Release"
# Name "ARcBridge - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ARcBridge.cpp
# End Source File
# Begin Source File

SOURCE=.\ARcBridge.rc
# End Source File
# Begin Source File

SOURCE=.\ARcBridgeDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ARcBridgeView.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcLineInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcStationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LockDataInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveCompleteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneDrawing.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneInput.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneStart.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WaitForSubProgram.cpp
# End Source File
# Begin Source File

SOURCE=.\WmfSizeDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ARcBridge.h

!IF  "$(CFG)" == "ARcBridge - Win32 Release"

# Begin Custom Build
WkspDir=.
InputPath=.\ARcBridge.h

"../CommonCompile/Output/Out" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo Run WriteBuildDate.exe For Date Version....... 
	"$(WkspDir)\WriteBuildDate.exe" $(WkspDir)\..\include\  $(WkspDir)\..\include\versionNumber.inc $(BUILD_NUMBER) 
	CopyDll_Release.bat 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "ARcBridge - Win32 Debug"

# Begin Custom Build
WkspDir=.
InputPath=.\ARcBridge.h

"../CommonCompile/OutputD/OutD" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo Run WriteBuildDate.exe For Date Version....... 
	"$(WkspDir)\WriteBuildDate.exe" $(WkspDir)\..\include\  $(WkspDir)\..\include\versionNumber.inc BUILD_NUMBER 
	CopyDll_Debug.bat 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ARcBridgeDoc.h
# End Source File
# Begin Source File

SOURCE=.\ARcBridgeView.h
# End Source File
# Begin Source File

SOURCE=.\CalcLineInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\CalcStationDlg.h
# End Source File
# Begin Source File

SOURCE=.\DockPaneProc.h
# End Source File
# Begin Source File

SOURCE=.\LockDataInfo.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SaveCompleteDlg.h
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneDrawing.h
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneInput.h
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneStart.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\WaitForSubProgram.h
# End Source File
# Begin Source File

SOURCE=.\WmfSizeDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\about.bmp
# End Source File
# Begin Source File

SOURCE=.\res\about_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\activesi.cur
# End Source File
# Begin Source File

SOURCE=.\res\ADManagerButton.bmp
# End Source File
# Begin Source File

SOURCE=.\res\align.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ARc_icon6.ico
# End Source File
# Begin Source File

SOURCE=.\res\ARc_icon7.ico
# End Source File
# Begin Source File

SOURCE=.\res\ARcBridge.ico
# End Source File
# Begin Source File

SOURCE=.\res\ARcBridge.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ARcBridgeDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_about.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\draw.bmp
# End Source File
# Begin Source File

SOURCE=.\res\drawing.bmp
# End Source File
# Begin Source File

SOURCE=.\res\file.bmp
# End Source File
# Begin Source File

SOURCE=.\res\file2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\file_disable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\format.bmp
# End Source File
# Begin Source File

SOURCE=.\res\input_base.bmp
# End Source File
# Begin Source File

SOURCE=.\res\input_gen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\input_head.bmp
# End Source File
# Begin Source File

SOURCE=.\res\input_rebar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\nodrop.cur
# End Source File
# Begin Source File

SOURCE=.\res\osnap.bmp
# End Source File
# Begin Source File

SOURCE=.\res\others.bmp
# End Source File
# Begin Source File

SOURCE=.\res\out.bmp
# End Source File
# Begin Source File

SOURCE=.\res\output.bmp
# End Source File
# Begin Source File

SOURCE=.\res\shortcut_big.bmp
# End Source File
# Begin Source File

SOURCE=.\res\shortcut_small.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash_plus.bmp
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\support.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar01.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar02.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar03.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar04.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar05.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar06.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar07.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar08.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar09.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar11.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\zoom.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\app.manifest
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section ARcBridge : {72ADFD78-2C39-11D0-9903-00A0C91BC942}
# 	1:11:IDB_SPLASH1:102
# 	2:21:SplashScreenInsertKey:4.0
# End Section
# Section ARcBridge : {F9043C85-F6F2-101A-A3C9-08002B2F49FB}
# 	2:21:DefaultSinkHeaderFile:commondialog.h
# 	2:16:DefaultSinkClass:CCommonDialogEx
# End Section
# Section ARcBridge : {083039C2-13F4-11D1-8B7E-0000F8754DA1}
# 	2:5:Class:CCommonDialogEx
# 	2:10:HeaderFile:commondialog.h
# 	2:8:ImplFile:commondialog.cpp
# End Section
