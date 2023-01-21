# Microsoft Developer Studio Project File - Name="Panel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Panel - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "panel.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "panel.mak" CFG="Panel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Panel - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Panel - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Panel - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "src\panel\lib\Release"
# PROP BASE Intermediate_Dir "src\panel\lib\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "src\panel\lib\Release"
# PROP Intermediate_Dir "src\panel\lib\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".\include" /I ".\src\panel" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Panel - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "src\panel\lib\Debug"
# PROP BASE Intermediate_Dir "src\panel\lib\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "src\panel\lib\Debug"
# PROP Intermediate_Dir "src\panel\lib\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".\include" /I ".\src\panel" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "Panel - Win32 Release"
# Name "Panel - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\panel\button.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\cycle.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\dial.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\frame.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\g_gets.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\graphframe.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\icon.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\iconmenu.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\list.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\mapwfind.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\mouse.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\mslider.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\oldtypein.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\palette.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\panel.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\polyframe.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\puck.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\script.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\scroll.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\signal.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\slider.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\slideroid.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\strip.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\submenu.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\typein.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\typeout.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\user.c
# End Source File
# Begin Source File

SOURCE=.\src\panel\viewframe.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
