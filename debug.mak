CXX = g++
CPPFLAGS =  \
	-g \
	-Wall \
	-pipe \
	-mthreads \
	-O2 \
	-std=gnu++14

INCLUDES =  \
	-I$(WXINCLUDE) \
	-I$(WXINCLUDESETUP_DBG)

DEFS =  \
	-D__WXMSW__ \
	-D_UNICODE

LIBS = \
	-lwxbase32ud \
	-lwxbase32ud_net \
	-lwxbase32ud_xml \
	-lwxexpatd \
	-lwxjpegd \
	-lwxmsw32ud_adv \
	-lwxmsw32ud_aui \
	-lwxmsw32ud_core \
	-lwxmsw32ud_gl \
	-lwxmsw32ud_html \
	-lwxmsw32ud_media \
	-lwxmsw32ud_propgrid \
	-lwxmsw32ud_ribbon \
	-lwxmsw32ud_richtext \
	-lwxmsw32ud_stc \
	-lwxmsw32ud_webview \
	-lwxmsw32ud_xrc \
	-lwxpngd \
	-lwxregexud \
	-lwxscintillad \
	-lwxtiffd \
	-lwxzlibd \
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
	$(OBJDIR)\Vektor\Vektor.o \
	$(OBJDIR)\RUZmBIApp.o \
	$(OBJDIR)\RUZmBIMain.o \
	$(OBJDIR)\RUZLayerDialoge.o \
	$(OBJDIR)\RUZThCtrl.o \
	$(OBJDIR)\resource.res
OUT = RUZ.exe

.PHONEY: all clean

all: $(OBJS)
	$(CXX) $(CPPFLAGS) $(DEFS) -L$(WXLIBPATH_DBG) -o $(OUT) $(OBJS) -s $(LIBS)

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
	del $(OBJS) $(OUT)
