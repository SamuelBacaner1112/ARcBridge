# Microsoft Developer Studio Generated NMAKE File, Based on ARcBridgeData.dsp
!IF "$(CFG)" == ""
CFG=ARcBridgeData - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ARcBridgeData - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ARcBridgeData - Win32 Release" && "$(CFG)" != "ARcBridgeData - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ARcBridgeData - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\CommonCompile\Output\ARcBridgeData.dll"


CLEAN :
	-@erase "$(INTDIR)\ARcBridgeData.obj"
	-@erase "$(INTDIR)\ARcBridgeData.pch"
	-@erase "$(INTDIR)\ARcBridgeData.res"
	-@erase "$(INTDIR)\ARcBridgeDataStd.obj"
	-@erase "$(INTDIR)\FlexibleJoint.obj"
	-@erase "$(INTDIR)\FootWay.obj"
	-@erase "$(INTDIR)\GuardWall.obj"
	-@erase "$(INTDIR)\Jijum.obj"
	-@erase "$(INTDIR)\Parabola.obj"
	-@erase "$(INTDIR)\RcBridgeApp.obj"
	-@erase "$(INTDIR)\RcBridgeData.obj"
	-@erase "$(INTDIR)\RcBridgeRebar.obj"
	-@erase "$(INTDIR)\RcBridgeRebarUtil.obj"
	-@erase "$(INTDIR)\SaveCrossSection.obj"
	-@erase "$(INTDIR)\SaveDistRigidEndSlab.obj"
	-@erase "$(INTDIR)\SaveDistRigidEndWall.obj"
	-@erase "$(INTDIR)\SaveOrgInWall.obj"
	-@erase "$(INTDIR)\SaveXyMatchLineAndSlabUpper.obj"
	-@erase "$(INTDIR)\SaveXySlabUpperByLenFrSttStation.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TypeDefault.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WallApp.obj"
	-@erase "$(INTDIR)\WallData.obj"
	-@erase "$(INTDIR)\WingWall.obj"
	-@erase "..\..\CommonCompile\lib\ARcBridgeData.exp"
	-@erase "..\..\CommonCompile\lib\ARcBridgeData.lib"
	-@erase "..\..\CommonCompile\Output\ARcBridgeData.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeData.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridgeData.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridgeData.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=RCModule.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\ARcBridgeData.pdb" /machine:I386 /def:".\ARcBridgeData.def" /out:"../../CommonCompile/Output/ARcBridgeData.dll" /implib:"../../CommonCompile/lib/ARcBridgeData.lib" /libpath:"$(HANGIL_ENGINE)/bin/lib" /libpath:"$(HANGIL_MODULE)/bin/lib" /libpath:"$(HANGIL_FRAME)/bin/lib" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/release" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/release" /libpath:"../../CommonCompile/lib" 
LINK32_OBJS= \
	"$(INTDIR)\ARcBridgeData.obj" \
	"$(INTDIR)\ARcBridgeDataStd.obj" \
	"$(INTDIR)\FlexibleJoint.obj" \
	"$(INTDIR)\FootWay.obj" \
	"$(INTDIR)\GuardWall.obj" \
	"$(INTDIR)\Jijum.obj" \
	"$(INTDIR)\Parabola.obj" \
	"$(INTDIR)\RcBridgeApp.obj" \
	"$(INTDIR)\RcBridgeData.obj" \
	"$(INTDIR)\RcBridgeRebar.obj" \
	"$(INTDIR)\RcBridgeRebarUtil.obj" \
	"$(INTDIR)\SaveCrossSection.obj" \
	"$(INTDIR)\SaveDistRigidEndSlab.obj" \
	"$(INTDIR)\SaveDistRigidEndWall.obj" \
	"$(INTDIR)\SaveOrgInWall.obj" \
	"$(INTDIR)\SaveXyMatchLineAndSlabUpper.obj" \
	"$(INTDIR)\SaveXySlabUpperByLenFrSttStation.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TypeDefault.obj" \
	"$(INTDIR)\WallApp.obj" \
	"$(INTDIR)\WallData.obj" \
	"$(INTDIR)\WingWall.obj" \
	"$(INTDIR)\ARcBridgeData.res"

"..\..\CommonCompile\Output\ARcBridgeData.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ARcBridgeData - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\CommonCompile\OutputD\ARcBridgeDataD.dll"


CLEAN :
	-@erase "$(INTDIR)\ARcBridgeData.obj"
	-@erase "$(INTDIR)\ARcBridgeData.pch"
	-@erase "$(INTDIR)\ARcBridgeData.res"
	-@erase "$(INTDIR)\ARcBridgeDataStd.obj"
	-@erase "$(INTDIR)\FlexibleJoint.obj"
	-@erase "$(INTDIR)\FootWay.obj"
	-@erase "$(INTDIR)\GuardWall.obj"
	-@erase "$(INTDIR)\Jijum.obj"
	-@erase "$(INTDIR)\Parabola.obj"
	-@erase "$(INTDIR)\RcBridgeApp.obj"
	-@erase "$(INTDIR)\RcBridgeData.obj"
	-@erase "$(INTDIR)\RcBridgeRebar.obj"
	-@erase "$(INTDIR)\RcBridgeRebarUtil.obj"
	-@erase "$(INTDIR)\SaveCrossSection.obj"
	-@erase "$(INTDIR)\SaveDistRigidEndSlab.obj"
	-@erase "$(INTDIR)\SaveDistRigidEndWall.obj"
	-@erase "$(INTDIR)\SaveOrgInWall.obj"
	-@erase "$(INTDIR)\SaveXyMatchLineAndSlabUpper.obj"
	-@erase "$(INTDIR)\SaveXySlabUpperByLenFrSttStation.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TypeDefault.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WallApp.obj"
	-@erase "$(INTDIR)\WallData.obj"
	-@erase "$(INTDIR)\WingWall.obj"
	-@erase "$(OUTDIR)\ARcBridgeDataD.pdb"
	-@erase "..\..\CommonCompile\libD\ARcBridgeDataD.exp"
	-@erase "..\..\CommonCompile\libD\ARcBridgeDataD.lib"
	-@erase "..\..\CommonCompile\OutputD\ARcBridgeDataD.dll"
	-@erase "..\..\CommonCompile\OutputD\ARcBridgeDataD.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeData.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ARcBridgeData.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ARcBridgeData.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=RCModuleD.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\ARcBridgeDataD.pdb" /debug /machine:I386 /def:".\ARcBridgeDataD.def" /out:"../../CommonCompile/OutputD/ARcBridgeDataD.dll" /implib:"../../CommonCompile/libD/ARcBridgeDataD.lib" /pdbtype:sept /libpath:"$(HANGIL_ENGINE)/bin/libD" /libpath:"$(HANGIL_MODULE)/bin/libD" /libpath:"$(HANGIL_FRAME)/bin/libD" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/bin/debug" /libpath:"$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/bin/debug" /libpath:"../../CommonCompile/libD" 
LINK32_OBJS= \
	"$(INTDIR)\ARcBridgeData.obj" \
	"$(INTDIR)\ARcBridgeDataStd.obj" \
	"$(INTDIR)\FlexibleJoint.obj" \
	"$(INTDIR)\FootWay.obj" \
	"$(INTDIR)\GuardWall.obj" \
	"$(INTDIR)\Jijum.obj" \
	"$(INTDIR)\Parabola.obj" \
	"$(INTDIR)\RcBridgeApp.obj" \
	"$(INTDIR)\RcBridgeData.obj" \
	"$(INTDIR)\RcBridgeRebar.obj" \
	"$(INTDIR)\RcBridgeRebarUtil.obj" \
	"$(INTDIR)\SaveCrossSection.obj" \
	"$(INTDIR)\SaveDistRigidEndSlab.obj" \
	"$(INTDIR)\SaveDistRigidEndWall.obj" \
	"$(INTDIR)\SaveOrgInWall.obj" \
	"$(INTDIR)\SaveXyMatchLineAndSlabUpper.obj" \
	"$(INTDIR)\SaveXySlabUpperByLenFrSttStation.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TypeDefault.obj" \
	"$(INTDIR)\WallApp.obj" \
	"$(INTDIR)\WallData.obj" \
	"$(INTDIR)\WingWall.obj" \
	"$(INTDIR)\ARcBridgeData.res"

"..\..\CommonCompile\OutputD\ARcBridgeDataD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ARcBridgeData.dep")
!INCLUDE "ARcBridgeData.dep"
!ELSE 
!MESSAGE Warning: cannot find "ARcBridgeData.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ARcBridgeData - Win32 Release" || "$(CFG)" == "ARcBridgeData - Win32 Debug"
SOURCE=.\ARcBridgeData.cpp

"$(INTDIR)\ARcBridgeData.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\ARcBridgeData.rc

"$(INTDIR)\ARcBridgeData.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ARcBridgeDataStd.cpp

"$(INTDIR)\ARcBridgeDataStd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\FlexibleJoint.cpp

"$(INTDIR)\FlexibleJoint.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\FootWay.cpp

"$(INTDIR)\FootWay.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\GuardWall.cpp

"$(INTDIR)\GuardWall.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\Jijum.cpp

"$(INTDIR)\Jijum.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\Parabola.cpp

"$(INTDIR)\Parabola.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\RcBridgeApp.cpp

"$(INTDIR)\RcBridgeApp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\RcBridgeData.cpp

"$(INTDIR)\RcBridgeData.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\RcBridgeRebar.cpp

"$(INTDIR)\RcBridgeRebar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\RcBridgeRebarUtil.cpp

"$(INTDIR)\RcBridgeRebarUtil.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\SaveCrossSection.cpp

"$(INTDIR)\SaveCrossSection.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\SaveDistRigidEndSlab.cpp

"$(INTDIR)\SaveDistRigidEndSlab.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\SaveDistRigidEndWall.cpp

"$(INTDIR)\SaveDistRigidEndWall.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\SaveOrgInWall.cpp

"$(INTDIR)\SaveOrgInWall.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\SaveXyMatchLineAndSlabUpper.cpp

"$(INTDIR)\SaveXyMatchLineAndSlabUpper.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\SaveXySlabUpperByLenFrSttStation.cpp

"$(INTDIR)\SaveXySlabUpperByLenFrSttStation.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ARcBridgeData - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeData.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ARcBridgeData.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ARcBridgeData - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(HANGIL_ENGINE)/include" /I "$(HANGIL_MODULE)/include" /I "$(HANGIL_FRAME)/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Engine/include" /I "$(UBICON_ENGINE_TOTAL)/Ubicon_Occ/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ARcBridgeData.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ARcBridgeData.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TypeDefault.cpp

"$(INTDIR)\TypeDefault.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\WallApp.cpp

"$(INTDIR)\WallApp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\WallData.cpp

"$(INTDIR)\WallData.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"


SOURCE=.\WingWall.cpp

"$(INTDIR)\WingWall.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ARcBridgeData.pch"



!ENDIF 

