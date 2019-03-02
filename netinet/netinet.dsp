# Microsoft Developer Studio Project File - Name="netinet" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=netinet - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "netinet.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "netinet.mak" CFG="netinet - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "netinet - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "netinet - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "netinet - Win32 Release"

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

!ELSEIF  "$(CFG)" == "netinet - Win32 Debug"

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
# ADD LIB32 /nologo /out:"..\lib\netinet.lib"

!ENDIF 

# Begin Target

# Name "netinet - Win32 Release"
# Name "netinet - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\if_ether.c
# End Source File
# Begin Source File

SOURCE=.\igmp.c
# End Source File
# Begin Source File

SOURCE=.\in.c
# End Source File
# Begin Source File

SOURCE=.\in_cksum.c
# End Source File
# Begin Source File

SOURCE=.\in_pcb.c
# End Source File
# Begin Source File

SOURCE=.\in_proto.c
# End Source File
# Begin Source File

SOURCE=.\ip_icmp.c
# End Source File
# Begin Source File

SOURCE=.\ip_input.c
# End Source File
# Begin Source File

SOURCE=.\ip_mroute.c
# End Source File
# Begin Source File

SOURCE=.\ip_output.c
# End Source File
# Begin Source File

SOURCE=.\raw_ip.c
# End Source File
# Begin Source File

SOURCE=.\tcp_debug.c
# End Source File
# Begin Source File

SOURCE=.\tcp_input.c
# End Source File
# Begin Source File

SOURCE=.\tcp_output.c
# End Source File
# Begin Source File

SOURCE=.\tcp_subr.c
# End Source File
# Begin Source File

SOURCE=.\tcp_timer.c
# End Source File
# Begin Source File

SOURCE=.\tcp_usrreq.c
# End Source File
# Begin Source File

SOURCE=.\udp_usrreq.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\icmp_var.h
# End Source File
# Begin Source File

SOURCE=.\if_ether.h
# End Source File
# Begin Source File

SOURCE=.\igmp.h
# End Source File
# Begin Source File

SOURCE=.\igmp_var.h
# End Source File
# Begin Source File

SOURCE=.\in.h
# End Source File
# Begin Source File

SOURCE=.\in_pcb.h
# End Source File
# Begin Source File

SOURCE=.\in_systm.h
# End Source File
# Begin Source File

SOURCE=.\in_var.h
# End Source File
# Begin Source File

SOURCE=.\ip.h
# End Source File
# Begin Source File

SOURCE=.\ip_icmp.h
# End Source File
# Begin Source File

SOURCE=.\ip_mroute.h
# End Source File
# Begin Source File

SOURCE=.\ip_var.h
# End Source File
# Begin Source File

SOURCE=.\tcp.h
# End Source File
# Begin Source File

SOURCE=.\tcp_debug.h
# End Source File
# Begin Source File

SOURCE=.\tcp_fsm.h
# End Source File
# Begin Source File

SOURCE=.\tcp_seq.h
# End Source File
# Begin Source File

SOURCE=.\tcp_timer.h
# End Source File
# Begin Source File

SOURCE=.\tcp_var.h
# End Source File
# Begin Source File

SOURCE=.\tcpip.h
# End Source File
# Begin Source File

SOURCE=.\udp.h
# End Source File
# Begin Source File

SOURCE=.\udp_var.h
# End Source File
# End Group
# End Target
# End Project
