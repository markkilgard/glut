
# Be sure to modify the definitions in this file to agree with your
# systems installation.
#  NOTE: be sure that the install directories use '\' not '/' for paths.

# Use the DOS shell comment command REM as a poor man's echo
ECHO = REM

# Microsoft Visual C++ 4.x install directories
VC4_LIBINSTALL     = c:\msdev\lib
VC4_INCLUDEINSTALL = c:\msdev\include\GL

# Microsoft Visual C++ 5.0 install directories
VC5_LIBINSTALL     = "c:\Program Files\DevStudio\VC\lib"
VC5_INCLUDEINSTALL = "c:\Program Files\DevStudio\VC\include\GL"

# Microsoft Visual C++ 6.0 install directories
# UNCOMMENT next 2 lines if you use VC++ 6.0
VC6_LIBINSTALL     = "c:\Program Files\Microsoft Visual Studio\VC98\Lib"
VC6_INCLUDEINSTALL = "c:\Program Files\Microsoft Visual Studio\VC98\Include\GL"

# Microsoft Visual Studio .NET 2003 install directories
# UNCOMMENT next 2 lines if you use VC++ 7.0
VC7_LIBINSTALL     = "c:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\PlatformSDK\Lib"
VC7_INCLUDEINSTALL = "c:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\PlatformSDK\Include\gl"

# Microsoft Visual Studio .NET 2003 install directories
# UNCOMMENT next 2 lines if you use VC++ 8.0
VC8_LIBINSTALL     = "c:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Lib"
VC8_INCLUDEINSTALL = "c:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Include\gl"

# Win95 dll directory
DLLINSTALL     = c:\windows\system

# WinNT dll directory
#DLLINSTALL     = c:\winnt\system32

# Microsoft OpenGL libraries
#
MS_GLU     = glu32.lib
MS_OPENGL  = opengl32.lib
MS_GLUT    = $(TOP)/lib/glut/glut32.lib
MS_GLUTLIB = glut32.lib
MS_GLUTDLL = glut32.dll

# SGI OpenGL for Windows libraries (formerly Cosmo OpenGL)
#
SGI_GLU     = \oglsdk\lib\glu.lib
SGI_OPENGL  = \oglsdk\lib\opengl.lib
SGI_GLUT    = $(TOP)/lib/glut/glut.lib
SGI_GLUTLIB = glut.lib
SGI_GLUTDLL = glut.dll

# >> See note below about enabling SGI OpenGL for Windows support.
GLU     = $(MS_GLU)
OPENGL  = $(MS_OPENGL)
GLUT    = $(MS_GLUT)
GLUTLIB = $(MS_GLUTLIB)
GLUTDLL = $(MS_GLUTDLL)

# >> To use SGI OpenGL, uncomment lines below and comment out the similiar
# >> lines above.  You can download SGI OpenGL for Windows for
# >> free from http://www.meer.net/~gold/OpenGL/opengl2.exe
#GLU     = $(SGI_GLU)
#OPENGL  = $(SGI_OPENGL)
#GLUT    = $(SGI_GLUT)
#GLUTLIB = $(SGI_GLUTLIB)
#GLUTDLL = $(SGI_GLUTDLL)

# The Micro UI lib
MUI     = $(TOP)/lib/mui/mui.lib

# The OpenGL Extrusion and Tubing lib
GLE     = $(TOP)/lib/gle/gle.lib

# The OpenGL Sphere Mapping lib
GLSMAP  = $(TOP)/lib/glsmap/glsmap.lib

# The OpenGL Sphere Mapping lib
GLCMAP  = $(TOP)/lib/glcmap/glcmap.lib

# So Visual Studio 8 won't complain about 
#ALLOW_DEPRECATED_APIS = /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE
ALLOW_DEPRECATED_APIS = /D_CRT_SECURE_NO_WARNINGS

# common definitions used by all makefiles
CFLAGS	= $(cflags) $(cdebug) $(EXTRACFLAGS) -DWIN32 -I$(TOP)/include $(ALLOW_DEPRECATED_APIS)
LIBS	= $(lflags) $(ldebug) $(EXTRALIBS) $(GLUT) $(GLU) $(OPENGL) $(guilibs)
EXES	= $(SRCS:.c=.exe) $(CPPSRCS:.cpp=.exe) $(MORE_EXES)

!IFNDEF NODEBUG
#lcommon = /INCREMENTAL:NO /DEBUG /NOLOGO
lcommon = /INCREMENTAL:NO /DEBUG /NOLOGO
# For Microsoft Visual C++ 5.0 and beyond
lflags  = $(lcommon) $(lflags)
!ENDIF

!ifdef VCINSTALLDIR
!if exist("$(VCINSTALLDIR)\Vc7\bin\cl.exe")
cc = "$(VCINSTALLDIR)\Vc7\bin\cl.exe"
!endif
!endif

!ifdef VCINSTALLDIR
!if exist("$(VCINSTALLDIR)\Vc7\bin\link.exe")
link = "$(VCINSTALLDIR)\Vc7\bin\link.exe"
!endif
!endif

!ifdef VCINSTALLDIR
!if exist("$(VCINSTALLDIR)\bin\cl.exe")
cc = "$(VCINSTALLDIR)\bin\cl.exe"
!endif
!endif

!ifdef VCINSTALLDIR
!if exist("$(VCINSTALLDIR)\bin\link.exe")
link = "$(VCINSTALLDIR)\bin\link.exe"
!endif
!endif

# default rule
default	: $(EXES)

# cleanup rules
clean	::
	-del *.obj
	-del *.pdb
	-del *.ilk
	-del *.ncb
	-del *~
	-del *.exp

clobber	:: clean
!if "$(EXES)" != "  "
	-del *.exe
!endif
	-del *.dll
	-del *.lib
	-del $(LDIRT)

# inference rules
!if "$(EXES)" != "  "
$(EXES)	: $*.obj $(DEPLIBS)
	$(link) -out:$@ $** $(LIBS)
!endif
.c.obj	: 
	$(cc) $(CFLAGS) $<
.cpp.obj : 
	$(cc) $(CFLAGS) $<
