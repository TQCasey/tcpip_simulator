# Microsoft Developer Studio Project File - Name="net" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=net - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "net.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "net.mak" CFG="net - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "net - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "net - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "net - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "net - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /MTd /W3 /WX /Gm /GX /ZI /Od /X /I "../include/bsd" /I ".." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\net.lib"

!ENDIF 

# Begin Target

# Name "net - Win32 Release"
# Name "net - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\bpf.c
# End Source File
# Begin Source File

SOURCE=.\bpf_filter.c
# End Source File
# Begin Source File

SOURCE=.\if.c
# End Source File
# Begin Source File

SOURCE=.\if_ethersubr.c
# End Source File
# Begin Source File

SOURCE=.\if_loop.c
# End Source File
# Begin Source File

SOURCE=.\if_sl.c
# End Source File
# Begin Source File

SOURCE=.\if_softeth.c
# End Source File
# Begin Source File

SOURCE=.\radix.c
# End Source File
# Begin Source File

SOURCE=.\raw_cb.c
# End Source File
# Begin Source File

SOURCE=.\raw_usrreq.c
# End Source File
# Begin Source File

SOURCE=.\route.c
# End Source File
# Begin Source File

SOURCE=.\rtsock.c
# End Source File
# Begin Source File

SOURCE=.\slcompress.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\bpf.h
# End Source File
# Begin Source File

SOURCE=.\bpf_compat.h
# End Source File
# Begin Source File

SOURCE=.\bpfdesc.h
# End Source File
# Begin Source File

SOURCE=.\if.h
# End Source File
# Begin Source File

SOURCE=.\if_arp.h
# End Source File
# Begin Source File

SOURCE=.\if_dl.h
# End Source File
# Begin Source File

SOURCE=.\if_llc.h
# End Source File
# Begin Source File

SOURCE=.\if_slvar.h
# End Source File
# Begin Source File

SOURCE=.\if_types.h
# End Source File
# Begin Source File

SOURCE=.\netisr.h
# End Source File
# Begin Source File

SOURCE=.\radix.h
# End Source File
# Begin Source File

SOURCE=.\raw_cb.h
# End Source File
# Begin Source File

SOURCE=.\route.h
# End Source File
# Begin Source File

SOURCE=.\slcompress.h
# End Source File
# Begin Source File

SOURCE=.\slip.h
# End Source File
# End Group
# End Target
# End Project
