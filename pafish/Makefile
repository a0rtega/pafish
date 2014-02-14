CC        = i686-pc-mingw32-gcc
WINDRES   = i686-pc-mingw32-windres
OBJDIR    = Objects/MingW
SRC       = $(wildcard *.c)
OBJ       = $(SRC:%.c=$(OBJDIR)/%.o) $(OBJDIR)/pafish_private.res
BIN       = Output/MingW/pafish.exe
LINKOBJ   = $(OBJDIR)/*.o $(OBJDIR)/pafish_private.res
LIBS      = -s -lws2_32 -liphlpapi
CXXFLAGS  = -fexpensive-optimizations -O1 -static-libgcc
CFLAGS    = -fexpensive-optimizations -O1
GPROF     = i686-pc-mingw32-gprof
LINKFLAGS = -static-libgcc

ifeq ($(OS),Windows_NT)
   RM = del /Q
   FixPath = $(subst /,\,$1)
else
   RM = rm -f
   FixPath = $1
endif
LINK      = i686-pc-mingw32-g++

.PHONY: all all-before all-after clean clean-custom
all: all-before $(BIN) all-after

clean: clean-custom
	$(RM) $(call FixPath,$(LINKOBJ)) $(call FixPath,$(BIN))

$(BIN): $(OBJ)
	$(CC) $(LINKFLAGS) $(LINKOBJ) -o $@ $(LIBS) 

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(DIRS) -c $^ -o $@

$(OBJDIR)/pafish_private.res: $(OBJDIR)/pafish_private.rc 
	$(WINDRES) --input-format=rc -o $(OBJDIR)/pafish_private.res $(OBJDIR)/pafish_private.rc -O coff
