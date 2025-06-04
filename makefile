CXX = g++
CC = clang
CPPFLAGS =  \
	-Wall \
	-pipe \
	-mthreads \
	-O2 \
	-std=gnu++14

INCLUDES =  \
	-I$(WXINCLUDE) \
	-I$(WXINCLUDESETUP)

DEFS =  \
	-D__WXMSW__ \
	-D_UNICODE \
	-DNDEBUG \
	-DWXUSINGDLL

LIBS = \
	-lwxbase32u \
	-lwxbase32u_net \
	-lwxbase32u_xml \
	-lwxexpat \
	-lwxjpeg \
	-lwxmsw32u_adv \
	-lwxmsw32u_aui \
	-lwxmsw32u_core \
	-lwxmsw32u_gl \
	-lwxmsw32u_html \
	-lwxmsw32u_media \
	-lwxmsw32u_propgrid \
	-lwxmsw32u_ribbon \
	-lwxmsw32u_richtext \
	-lwxmsw32u_stc \
	-lwxmsw32u_webview \
	-lwxmsw32u_xrc \
	-lwxpng \
	-lwxregexu \
	-lwxscintilla \
	-lwxtiff \
	-lwxzlib \
	-lkernel32 \
	-luser32 \
	-lgdi32 \
	-lcomdlg32 \
	-lwinspool \
	-lwinmm \
	-lshell32 \
	-lshlwapi \
	-lcomctl32 \
	-lole32 \
	-loleaut32 \
	-luuid \
	-lrpcrt4 \
	-ladvapi32 \
	-lversion \
	-lws2_32 \
	-lwininet \
	-loleacc \
	-luxtheme

OBJDIR = obj
OBJS = \
	$(OBJDIR)\aruIntegral\aruIntegral.o \
	$(OBJDIR)\aruZeichner\aruZeichner.o \
	$(OBJDIR)\Dbl_Eingabe\Dbl_Eingabe.o \
	$(OBJDIR)\DXF\DXF_Handler.o \
	$(OBJDIR)\Kamera\Kamera_3D.o \
	$(OBJDIR)\Matrix\Matrix.o \
	$(OBJDIR)\RUZ\RUZObjekte.o \
	$(OBJDIR)\RUZ\RUZVerwaltung.o \
	$(OBJDIR)\RUZ\RUZThreadInfo.o \
	$(OBJDIR)\RUZ\LinienGeist.o \
	$(OBJDIR)\Vektor\Vektor.o \
	$(OBJDIR)\RUZmBIApp.o \
	$(OBJDIR)\RUZmBIMain.o \
	$(OBJDIR)\RUZLayerDialoge.o \
	$(OBJDIR)\RUZThCtrl.o \
	$(OBJDIR)\resource.res
OUT = RUZ.exe

.PHONEY: all clean

all: $(OBJS)
	$(CXX) $(CPPFLAGS) $(DEFS) -L$(WXLIBPATH) -o $(OUT) $(OBJS) -s $(LIBS)

$(OBJDIR)\aruIntegral\aruIntegral.o: source\aruIntegral\aruIntegral.cpp
	if not exist $(OBJDIR)\aruIntegral mkdir $(OBJDIR)\aruIntegral
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\aruZeichner\aruZeichner.o: source\aruZeichner\aruZeichner.cpp
	if not exist $(OBJDIR)\aruZeichner mkdir $(OBJDIR)\aruZeichner
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\Dbl_Eingabe\Dbl_Eingabe.o: source\Dbl_Eingabe\Dbl_Eingabe.cpp
	if not exist $(OBJDIR)\Dbl_Eingabe mkdir $(OBJDIR)\Dbl_Eingabe
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\DXF\DXF_Handler.o: source\DXF\DXF_Handler.cpp
	if not exist $(OBJDIR)\DXF mkdir $(OBJDIR)\DXF
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\Kamera\Kamera_3D.o: source\Kamera\Kamera_3D.cpp
	if not exist $(OBJDIR)\Kamera mkdir $(OBJDIR)\Kamera
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\Matrix\Matrix.o: source\Matrix\Matrix.cpp
	if not exist $(OBJDIR)\Matrix mkdir $(OBJDIR)\Matrix
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\RUZ\RUZObjekte.o: source\RUZ\RUZObjekte.cpp
	if not exist $(OBJDIR)\RUZ mkdir $(OBJDIR)\RUZ
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\RUZ\RUZVerwaltung.o: source\RUZ\RUZVerwaltung.cpp
	if not exist $(OBJDIR)\RUZ mkdir $(OBJDIR)\RUZ
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\RUZ\RUZThreadInfo.o: source\RUZ\RUZThreadInfo.cpp
	if not exist $(OBJDIR)\RUZ mkdir $(OBJDIR)\RUZ
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\RUZ\LinienGeist.o: source\RUZ\LinienGeist.c
	if not exist $(OBJDIR)\RUZ mkdir $(OBJDIR)\RUZ
	$(CC) -c $< -o $@

$(OBJDIR)\Vektor\Vektor.o: source\Vektor\Vektor.cpp
	if not exist $(OBJDIR)\Vektor mkdir $(OBJDIR)\Vektor
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\RUZmBIApp.o: source\RUZmBIApp.cpp
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\RUZmBIMain.o: source\RUZmBIMain.cpp
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\RUZLayerDialoge.o: source\RUZLayerDialoge.cpp
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\RUZThCtrl.o: source\RUZThCtrl.cpp
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\resource.res: source\resource.rc
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	windres.exe $(INCLUDES) -J rc -O coff -i source\resource.rc -o $(OBJDIR)\resource.res

clean:
	del /s /q $(OBJDIR)
	del /s /q $(BIN)
