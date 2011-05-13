CROSS_COMPILE = arm-linux-
PRJ_DIR        = .
PROJ_NAME	= notify-send

# Dirs
#------------------------------------
SRCDIR := $(PRJ_DIR)/src
BINDIR := $(PRJ_DIR)/bin
INCDIR := $(PRJ_DIR)/include
# Set search paths for source files
# Add any subdirs here
#------------------------------------
ADDITIONALSOURCE = 

BUILD_SPEC     = Release

# basic compiler flags. add  -march=armv6 when compile for Kindle?
CFLAGS = -I$(PRJ_DIR)/ -I$(PRJ_DIR)/include -I/opt/ELDK-ARM/arm/usr/include/ -msoft-float  -Wall -L/opt/ELDK-ARM/arm/lib/ -lpthread
# application-specific flags
APPFLAGS =

# Object files to build,
# Add file names without extensions
#------------------------------------
FILES =  \
		main		\
		launchpad	\
		font	\
		pixop	\
		screen	\
		statusbar
		
#Directory Search Paths
vpath %.c $(SRCDIR) $(ADDITIONALSOURCE)


ifeq ($(BUILD_SPEC),Release)
##FLAGS += -static -O2 -g0
	CFLAGS += -O2 -g0
	BINDIR        = $(PRJ_DIR)/bin
endif

ifeq ($(BUILD_SPEC),Debug)
#	CFLAGS += -static -O0 -g3
	CFLAGS += -O0 -g3
	BINDIR        = $(PRJ_DIR)/bindebug
endif

ifeq ($(BUILD_SPEC),Kindle)
##FLAGS += -static -O2 -g0 -march=armv6
	CFLAGS += -O2 -g0 -march=armv6
	BINDIR        = $(PRJ_DIR)/binarm
endif

ifeq ($(BUILD_SPEC),armDebug)
#	CFLAGS += -static -O0 -g3
	CFLAGS += -O0 -g3
	BINDIR        = $(PRJ_DIR)/armdebug
endif

#Create some lists
#------------------------------------
OBJS := $(strip $(patsubst %,%.o,$(strip $(FILES))))
OBJS_PATH := $(strip $(patsubst %,$(BINDIR)/%.o,$(strip $(FILES))))
HEADERS := $(wildcard $(INCDIR)/*.h)

## component-configuration info

## build-configuration info

AR               = $(CROSS_COMPILE)ar
AS               = $(CROSS_COMPILE)gcc
CC               = $(CROSS_COMPILE)gcc
CPP              = $(CROSS_COMPILE)gcc -E
LD               = $(CROSS_COMPILE)ld
NM               = $(CROSS_COMPILE)nm
STRIP            = $(CROSS_COMPILE)strip

# .o 
# compiles .c files into object code
#------------------------------------
$(BINDIR)/%.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) $(APPFLAGS) -c -o $@ $< 


# bin directory 
# creates the bin directory if it doesn't exist
#------------------------------------
$(BINDIR):
	$(MKDIR) $(BINDIR) 


# Target Directives
#------------------------------------
.PHONY: all clean
##.PHONY: all clean debug release

## build-configuration info

all:	$(PROJ_NAME)	

$(PROJ_NAME):	$(OBJS_PATH)
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(BINDIR)/*.o -lpthread
ifeq ($(BUILD_SPEC),Release)
	$(STRIP) $(BINDIR)/$@
endif



clean:
	rm -rf $(BINDIR)/*.o
	rm -rf $(BINDIR)/$(PROJ_NAME)
