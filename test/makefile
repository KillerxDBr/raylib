SRC := main.c
BLD := build
OBJ := $(SRC:%.c=$(BLD)/%.o)

NPROCS := $(shell echo %NUMBER_OF_PROCESSORS%)
MAKEFLAGS += -j$(NPROCS) --output-sync=target

RLDIR := ../src/
RLSRC := rcore.c rshapes.c rtextures.c rtext.c rglfw.c rmodels.c raudio.c
RLOBJ_W32 := $(RLSRC:%.c=$(BLD)/%_w32.o)
RLOBJ_GL := $(RLSRC:%.c=$(BLD)/%_gl.o)
RLSRC := $(RLSRC:%.c=$(RLDIR)%.c)

LIBS := -lopengl32 -lgdi32 -lwinmm -luser32 -lshell32

DEPFLAGS = -MT $@ -MMD -MP -MF $(BLD)/$*.d

DEPFILES_W32 := $(RLOBJ_W32:%.o=%.d)
DEPFLAGS_W32 = -MT $@ -MMD -MP -MF $(BLD)/$*_w32.d

DEPFILES_GL := $(RLOBJ_GL:%.o=%.d)
DEPFLAGS_GL = -MT $@ -MMD -MP -MF $(BLD)/$*_gl.d

DEPFILES := $(DEPFILES_W32)
DEPFILES += $(DEPFLAGS_GL)

FLAGS := -DUNICODE -D_CRT_SECURE_NO_WARNINGS 
RLFLAGS_W32 := $(FLAGS) -DPLATFORM_DESKTOP_WIN32 -DGRAPHICS_API_OPENGL_33
RLFLAGS_GL := $(FLAGS) -DPLATFORM_DESKTOP_GLFW -DGRAPHICS_API_OPENGL_33

.PHONY: dir

all: dir w32.exe gl.exe

dir:
	if not exist $(BLD) mkdir $(BLD)

$(DEPFILES):

include $(wildcard $(DEPFILES))

w32.exe: $(OBJ) $(RLOBJ_W32)
	clang -o w32.exe $(RLFLAGS_W32) $(OBJ) $(RLOBJ_W32) $(DEPFLAGS) $(LIBS)

gl.exe: $(OBJ) $(RLOBJ_GL)
	clang -o gl.exe $(RLFLAGS_GL) $(OBJ) $(RLOBJ_GL) $(DEPFLAGS) $(LIBS)

$(BLD)/%.o: %.c
	clang -c -o $@ $< $(FLAGS) -I$(RLDIR) $(DEPFLAGS) -DGRAPHICS_API_OPENGL_33 -DPLATFORM_DESKTOP

$(BLD)/%_w32.o: $(RLDIR)%.c
	clang -c -o $@ $< $(RLFLAGS_W32) -I$(RLDIR) $(DEPFLAGS_W32) -I$(RLDIR)external -I$(RLDIR)external/glfw/include

$(BLD)/%_gl.o: $(RLDIR)%.c
	clang -c -o $@ $< $(RLFLAGS_GL) -I$(RLDIR) $(DEPFLAGS_GL) -I$(RLDIR)external -I$(RLDIR)external/glfw/include
