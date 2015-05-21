#########################################################################
#
#  To compile dynamically using the shared Qt library, build Qt by
#     ./configure -shared
#     make && make install
#  To compile Qt into a static library that is attached to ddplot
#  standalone executable, build Qt by
#     ./configure -static
#     make && make install
#  and make sure CONFIG += static is included below. 
#  ddplot is then about 7MB large. Check that the application really 
#  links statically by:
#    ldd ./ddplot
#
#  Shared build:
#    qmake && make
#  Static build:
#    add CONFIG += static
#    qmake && make
#
#########################################################################

TARGET = ddplot
TEMPLATE = app
VPATH = src
OBJECTS_DIR = src
MOC_DIR = src
RCC_DIR = src
DESTDIR = bin
UI_DIR = src
RESOURCES = ddplot.qrc
CONFIG += qt release warn_off #static

HEADERS = animate.h atomstyle.h ddplot.h fformat.h messages.h appwin.h misc.h neighlist.h	\
	pltwin.h rdf.h scalepos.h selectplots.h selectzlayers.h strings.h \
        calcs.h cline.h darray.h

SOURCES	= animate.cpp atomstyle.cpp ddplot.cpp fformat.cpp messages.cpp appwin.cpp appwin-func.cpp \
	misc.cpp neighlist.cpp pltwin.cpp pltwin-calc.cpp pltwin-geom.cpp \
        pltwin-func.cpp rdf.cpp scalepos.cpp selectplots.cpp selectzlayers.cpp \
	strings.cpp calcs.cpp cline.cpp darray.cpp

FORMS = ui/dabout.ui ui/dburgvect.ui ui/dprojvector.ui ui/dscalearrow.ui ui/datomstyle.ui \
        ui/darrowstyle.ui ui/dselectplots.ui ui/danimate.ui ui/dshowplanes.ui \
        ui/darrneighbors.ui ui/dcoordsys.ui ui/dtransform.ui ui/drelaxinfo.ui \
        ui/dscalepos.ui ui/dselectzlayers.ui ui/dnewcutoff.ui ui/dcomparedisp.ui \
        ui/dcoordatoms.ui ui/dztolerance.ui


unix:static {
  message( "ddplot will be compiled as a stand-alone UNIX application" )
  QMAKE_LIBS_QT =
  QMAKE_LIBS_QT_THREAD =
  LIBS += -L$(QTDIR)/lib -lqt
}

mac:RC_FILE = ico/ddplot.icns
mac:static {
  message( "ddplot will be compiled as a stand-alone MAC OS X application" )
  QMAKE_LIBS_QT =
  QMAKE_LIBS_QT_THREAD =
  LIBS += $(QTDIR)/lib/libqt.a -lz -framework Carbon
}

