# Microsoft Developer Studio Project File - Name="ARcBridgeDrawingOption" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ARcBridgeDrawingOption - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ARcBridgeDrawingOption.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ARcBridgeDrawingOption.mak" CFG="ARcBridgeDrawingOption - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ARcBridgeDrawingOption - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ARcBridgeDrawingOption - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ARcBridgeDrawingOption - Win32 Release"

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
# ADD LINK32 ARoadConfig.lib ARcBridgeData.lib ARcBridgeDBDraw.lib DrawManage.lib HgGhostConc.lib RCModule.lib /nologo /subsystem:windows /dll /machine:I386 /def:".\ARcBridgeDrawingOption.def" /out:"../../CommonCompile/Output/ARcBridgeDrawingOption.dll" /implib:"../../CommonCompile/Output/ARcBridgeDrawingOption.lib" /libpath:"../../../../Engine/BaseEng/CommonCompile/Output" /libpath:"../../../../Engine/BaseComp/CommonCompile/Output" /libpath:"../../../../Engine/FrameEng/CommonCompile/Output" /libpath:"../../../../Engine/Bim3DEng/CommonCompile/Output" /libpath:"../../../../Lib/Ubicon_Occ/CommonCompile/Output" /libpath:"../../CommonCompile/Output" /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/Output"
# SUBTRACT LINK32 /pdb:none /incremental:yes /debug

!ELSEIF  "$(CFG)" == "ARcBridgeDrawingOption - Win32 Debug"

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
# ADD LINK32 ARoadConfigD.lib ARcBridgeDataD.lib ARcBridgeDBDrawD.lib DrawManageD.lib HgGhostConcD.lib RCModuleD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\ARcBridgeDrawingOptionD.def" /out:"../../CommonCompile/OutputD/ARcBridgeDrawingOptionD.dll" /implib:"../../CommonCompile/OutputD/ARcBridgeDrawingOptionD.lib" /pdbtype:sept /libpath:"../../../../Engine/BaseEng/CommonCompile/OutputD" /libpath:"../../../../Engine/BaseComp/CommonCompile/OutputD" /libpath:"../../../../Engine/FrameEng/CommonCompile/OutputD" /libpath:"../../../../Engine/Bim3DEng/CommonCompile/OutputD" /libpath:"../../../../Lib/Ubicon_Occ\CommonCompile\OutputD" /libpath:"../../CommonCompile/OutputD" /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/OutputD"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ARcBridgeDrawingOption - Win32 Release"
# Name "ARcBridgeDrawingOption - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ARcBridgeDrawingOption.cpp
# End Source File
# Begin Source File

SOURCE=.\ARcBridgeDrawingOption.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ARcBridgeDrawingOption.rc
# End Source File
# Begin Source File

SOURCE=.\ARcBridgeDrawingOptionData.cpp
# End Source File
# Begin Source File

SOURCE=.\ARcBridgeDrawingOptionStd.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionAps.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionBoring.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionCns.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionCommon.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionGeneral.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionHyunTa.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionOthers.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionRebar.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionShoeArrange.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionSlabStandardOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionVerticalPlane.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionBase.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ARcBridgeDrawingOptionData.h
# End Source File
# Begin Source File

SOURCE=.\ARcBridgeDrawingOptionStd.h
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionAps.h
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionBoring.h
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionCns.h
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionCommon.h
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionGeneral.h
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionHyunTa.h
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionOthers.h
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionRebar.h
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionShoeArrange.h
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionSlabStandardOutput.h
# End Source File
# Begin Source File

SOURCE=.\DrawingOptionVerticalPlane.h
# End Source File
# Begin Source File

SOURCE=.\OptionBase.h
# End Source File
# Begin Source File

SOURCE=.\OptionSheet.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ARcBridgeDrawingOption.rc2
# End Source File
# Begin Source File

SOURCE=.\res\check_off.bmp
# End Source File
# Begin Source File

SOURCE=.\res\check_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\node.bmp
# End Source File
# Begin Source File

SOURCE=.\res\radio_off.bmp
# End Source File
# Begin Source File

SOURCE=.\res\radio_on.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
