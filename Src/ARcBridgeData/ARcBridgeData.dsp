# Microsoft Developer Studio Project File - Name="ARcBridgeData" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ARcBridgeData - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ARcBridgeData.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ARcBridgeData.mak" CFG="ARcBridgeData - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ARcBridgeData - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ARcBridgeData - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ARcBridgeData - Win32 Release"

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
# ADD LINK32 RCModule.lib /nologo /subsystem:windows /dll /machine:I386 /def:".\ARcBridgeData.def" /out:"../../CommonCompile/Output/ARcBridgeData.dll" /implib:"../../CommonCompile/Output/ARcBridgeData.lib" /libpath:"../../../../Engine/BaseEng/CommonCompile/Output" /libpath:"../../../../Engine/BaseComp/CommonCompile/Output" /libpath:"../../../../Engine/FrameEng/CommonCompile/Output" /libpath:"../../../../Engine/Bim3DEng/CommonCompile/Output" /libpath:"../../../../Lib/Ubicon_Occ/CommonCompile/Output" /libpath:"../../CommonCompile/Output" /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/Output"
# SUBTRACT LINK32 /pdb:none /incremental:yes /debug

!ELSEIF  "$(CFG)" == "ARcBridgeData - Win32 Debug"

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
# ADD LINK32 RCModuleD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\ARcBridgeDataD.def" /out:"../../CommonCompile/OutputD/ARcBridgeDataD.dll" /implib:"../../CommonCompile/OutputD/ARcBridgeDataD.lib" /pdbtype:sept /libpath:"../../../../Engine/BaseEng/CommonCompile/OutputD" /libpath:"../../../../Engine/BaseComp/CommonCompile/OutputD" /libpath:"../../../../Engine/FrameEng/CommonCompile/OutputD" /libpath:"../../../../Engine/Bim3DEng/CommonCompile/OutputD" /libpath:"../../../../Lib/Ubicon_Occ\CommonCompile\OutputD" /libpath:"../../CommonCompile/OutputD" /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/OutputD"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ARcBridgeData - Win32 Release"
# Name "ARcBridgeData - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ARcBridgeData.cpp
# End Source File
# Begin Source File

SOURCE=.\ARcBridgeData.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ARcBridgeData.rc
# End Source File
# Begin Source File

SOURCE=.\ARcBridgeDataStd.cpp
# End Source File
# Begin Source File

SOURCE=.\FlexibleJoint.cpp
# End Source File
# Begin Source File

SOURCE=.\FootWay.cpp
# End Source File
# Begin Source File

SOURCE=.\GuardWall.cpp
# End Source File
# Begin Source File

SOURCE=.\Jijum.cpp
# End Source File
# Begin Source File

SOURCE=.\Parabola.cpp
# End Source File
# Begin Source File

SOURCE=.\RcBridgeApp.cpp
# End Source File
# Begin Source File

SOURCE=.\RcBridgeData.cpp
# End Source File
# Begin Source File

SOURCE=.\RcBridgeRebar.cpp
# End Source File
# Begin Source File

SOURCE=.\RcBridgeRebarUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveCrossSection.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveDistRigidEndSlab.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveDistRigidEndWall.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveOrgInWall.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveXyMatchLineAndSlabUpper.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveXySlabUpperByLenFrSttStation.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TypeDefault.cpp
# End Source File
# Begin Source File

SOURCE=.\WallApp.cpp
# End Source File
# Begin Source File

SOURCE=.\WallData.cpp
# End Source File
# Begin Source File

SOURCE=.\WingWall.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ARcBridgeDataStd.h
# End Source File
# Begin Source File

SOURCE=.\FlexibleJoint.h
# End Source File
# Begin Source File

SOURCE=.\FootWay.h
# End Source File
# Begin Source File

SOURCE=.\GuardWall.h
# End Source File
# Begin Source File

SOURCE=.\Jijum.h
# End Source File
# Begin Source File

SOURCE=.\Parabola.h
# End Source File
# Begin Source File

SOURCE=.\RcBridgeApp.h
# End Source File
# Begin Source File

SOURCE=.\RcBridgeData.h
# End Source File
# Begin Source File

SOURCE=.\RcBridgeRebar.h
# End Source File
# Begin Source File

SOURCE=.\RcBridgeRebarUtil.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SaveCrossSection.h
# End Source File
# Begin Source File

SOURCE=.\SaveDistRigidEndSlab.h
# End Source File
# Begin Source File

SOURCE=.\SaveDistRigidEndWall.h
# End Source File
# Begin Source File

SOURCE=.\SaveOrgInWall.h
# End Source File
# Begin Source File

SOURCE=.\SaveXyMatchLineAndSlabUpper.h
# End Source File
# Begin Source File

SOURCE=.\SaveXySlabUpperByLenFrSttStation.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TypeDefault.h
# End Source File
# Begin Source File

SOURCE=.\WallApp.h
# End Source File
# Begin Source File

SOURCE=.\WallData.h
# End Source File
# Begin Source File

SOURCE=.\WingWall.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ARcBridgeData.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
