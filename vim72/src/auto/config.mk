#
# config.mk.in -- autoconf template for Vim on Unix		vim:ts=8:sw=8:
#
# DO NOT EDIT config.mk!!  It will be overwritten by configure.
# Edit Makefile and run "make" or run ./configure with other arguments.
#
# Configure does not edit the makefile directly. This method is not the
# standard use of GNU autoconf, but it has two advantages:
#   a) The user can override every choice made by configure.
#   b) Modifications to the makefile are not lost when configure is run.
#
# I hope this is worth being nonstandard. jw.



VIMNAME		= vim
EXNAME		= ex
VIEWNAME	= view

CC		= /usr/bin/gcc-4.0
DEFS		= -DHAVE_CONFIG_H
CFLAGS		= -O -D_FORTIFY_SOURCE=1
CPPFLAGS	= -I/opt/local/include
srcdir		= .

LDFLAGS		= -L. -L/opt/local/lib -L/opt/local/lib -L/usr/local/lib
LIBS		= -lm  -lncurses -liconv
TAGPRG		= ctags -I INIT+ --fields=+S

CPP		= /usr/bin/cpp-4.0
CPP_MM		= M
DEPEND_CFLAGS_FILTER = | sed 's+-I */+-isystem /+g'
X_CFLAGS	= -I/opt/local/include 
X_LIBS_DIR	= -L/opt/local/lib -R/opt/local/lib  
X_PRE_LIBS	=  -lSM -lICE -lXpm
X_EXTRA_LIBS	=  -lXdmcp -lSM -lICE
X_LIBS		= -lXt -lX11

MZSCHEME_LIBS	= 
MZSCHEME_SRC	= 
MZSCHEME_OBJ	= 
MZSCHEME_CFLAGS	= 
MZSCHEME_PRO	= 
MZSCHEME_EXTRA  = 
MZSCHEME_MZC	= 

PERL		= 
PERLLIB		= 
PERL_LIBS	= 
SHRPENV		= 
PERL_SRC	= 
PERL_OBJ	= 
PERL_PRO	= 
PERL_CFLAGS	= 

PYTHON_SRC	= 
PYTHON_OBJ	= 
PYTHON_CFLAGS	= 
PYTHON_LIBS	= 
PYTHON_CONFDIR	= 
PYTHON_GETPATH_CFLAGS = 

TCL		= 
TCL_SRC		= 
TCL_OBJ		= 
TCL_PRO		= 
TCL_CFLAGS	= 
TCL_LIBS	= 

HANGULIN_SRC	= 
HANGULIN_OBJ	= 

WORKSHOP_SRC	= 
WORKSHOP_OBJ	= 

NETBEANS_SRC	= netbeans.c
NETBEANS_OBJ	= objects/netbeans.o

RUBY		= /opt/local/bin/ruby
RUBY_SRC	= if_ruby.c
RUBY_OBJ	= objects/if_ruby.o
RUBY_PRO	= if_ruby.pro
RUBY_CFLAGS	= -I/opt/local/lib/ruby/1.8/i686-darwin9
RUBY_LIBS	= -lruby -lpthread -ldl -lobjc 

SNIFF_SRC	= 
SNIFF_OBJ	= 

AWK		= gawk

STRIP		= strip

EXEEXT		= 

COMPILEDBY	= 

INSTALLVIMDIFF	= installvimdiff
INSTALLGVIMDIFF	= installgvimdiff
INSTALL_LANGS	= 
INSTALL_TOOL_LANGS	= 

### Line break character as octal number for "tr"
NL		= "\\012"

### Top directory for everything
prefix		= /opt/local

### Top directory for the binary
exec_prefix	= ${prefix}

### Prefix for location of data files
BINDIR		= ${exec_prefix}/bin

### For autoconf 2.60 and later (avoid a warning)
datarootdir	= ${prefix}/share

### Prefix for location of data files
DATADIR		= ${datarootdir}

### Prefix for location of man pages
MANDIR		= /opt/local/share/man

### Do we have a GUI
GUI_INC_LOC	= -I/opt/local/include/gtk-2.0 -I/opt/local/lib/gtk-2.0/include -I/opt/local/include/atk-1.0 -I/opt/local/include/cairo -I/opt/local/include/pango-1.0 -I/opt/local/include -I/opt/local/include/glib-2.0 -I/opt/local/lib/glib-2.0/include -I/opt/local/include/pixman-1 -I/opt/local/include/freetype2 -I/opt/local/include/libpng12  
GUI_LIB_LOC	= -L/opt/local/lib  
GUI_SRC		= $(GTK_SRC)
GUI_OBJ		= $(GTK_OBJ)
GUI_DEFS	= $(GTK_DEFS)
GUI_IPATH	= $(GTK_IPATH)
GUI_LIBS_DIR	= $(GTK_LIBS_DIR)
GUI_LIBS1	= $(GTK_LIBS1)
GUI_LIBS2	= $(GTK_LIBS2)
GUI_INSTALL	= $(GTK_INSTALL)
GUI_TARGETS	= $(GTK_TARGETS)
GUI_MAN_TARGETS	= $(GTK_MAN_TARGETS)
GUI_TESTTARGET	= $(GTK_TESTTARGET)
GUI_TESTARG	= $(GTK_TESTARG)
GUI_BUNDLE	= $(GTK_BUNDLE)
NARROW_PROTO	= 
GUI_X_LIBS	= 
MOTIF_LIBNAME	= 
GTK_LIBNAME	= -L/opt/local/lib -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgdk_pixbuf-2.0 -lpangocairo-1.0 -lgio-2.0 -lXinerama -lXi -lXrandr -lXcursor -lXcomposite -lXdamage -lpangoft2-1.0 -lXext -lXfixes -lcairo -lpixman-1 -lpng12 -lXrender -lX11 -lXau -lXdmcp -lpango-1.0 -lm -lfontconfig -lexpat -lfreetype -lz -lgobject-2.0 -lgmodule-2.0 -lglib-2.0 -lintl -liconv  

### Any OS dependent extra source and object file
OS_EXTRA_SRC	= 
OS_EXTRA_OBJ	= 

### If the *.po files are to be translated to *.mo files.
MAKEMO		= 

# Make sure that "make first" will run "make all" once configure has done its
# work.  This is needed when using the Makefile in the top directory.
first: all
