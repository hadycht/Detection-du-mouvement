# -------------- #
# -- Makefile -- #
# -------------- #

# -- Lile list ----------

FILE_NRC      = nrutil.c nralloc_mod.c
FILE_MOTION   = sigmadelta.c motion.c
FILE_MORPHO   = morpho_erosion.c       morpho_dilatation.c       morpho_ouverture.c       morpho_fermeture.c morpho_min.c morpho_max.c morpho.c
FILE_MORPHO8  = morpho_erosion_swp8.c  morpho_dilatation_swp8.c  morpho_ouverture_swp8.c  morpho_fermeture_swp8.c
FILE_MORPHO32 = morpho_erosion_swp32.c morpho_dilatation_swp32.c morpho_ouverture_swp32.c morpho_fermeture_swp32.c
FILE_MORPHO64 = morpho_erosion_swp64.c morpho_dilatation_swp64.c morpho_ouverture_swp64.c morpho_fermeture_swp64.c
FILE_SWP      = swp.c 
FILE_TEST     = nralloc_mod_test.c swp_test.c morpho_test.c motion_test.c
FILE_UE_L3_VISION = ue_l3_vision.c
FILE_MAIN     = main.c  

FILE = $(FILE_MAIN) $(FILE_TEST) $(FILE_NRC) $(FILE_MORPHO) $(FILE_MORPHO8) $(FILE_MORPHO32) $(FILE_MORPHO64) $(FILE_MOTION) $(FILE_SWP) 
FILE = $(FILE_MAIN) $(FILE_TEST) $(FILE_NRC) $(FILE_MORPHO) $(FILE_MORPHO8) $(FILE_MORPHO32) $(FILE_MORPHO64) $(FILE_MOTION) $(FILE_SWP)  $(FILE_UE_L3_VISION)

# -- paths ----------
SRC_PATH = src
OBJ_PATH = obj
EXE_PATH = exe
INC_PATH = include

# -- lib paths ----------
#LIB_LIB_PATH = -L../NRC2/lib
#LIB_INC_PATH = -I../NRC2/include

# -- Macros ----------
CC = gcc

# -- Flags ----------
C_OPTIMISATION_FLAGS = -std=c99 -O3 -fstrict-aliasing -Wno-comment
C_OPTIMISATION_FLAGS = -std=c99 -O0 -g -fstrict-aliasing -Wno-comment
C_OPTIMISATION_FLAGS = -std=c99 -O0 -g -Wno-comment -fsanitize=address -fno-omit-frame-pointer

C_ARCH_FLAGS = -mtune=native -march=native
C_ARCH_FLAGS =
C_INC_FLAGS = -I$(INC_PATH)

CFLAGS  = $(C_OPTIMISATION_FLAGS) $(C_ARCH_FLAGS) $(C_INC_FLAGS) $(LIB_INC_PATH)
LDFLAGS = $(C_OPTIMISATION_FLAGS) $(C_ARCH_FLAGS) $(C_INC_FLAGS) $(LIB_LIB_PATH)

# -- Final product ----------
PRODUCT   = motion_acc.exe

# -- src and obj List ----------
SRC = $(addprefix ${SRC_PATH}/, $(FILE))
OBJ = $(addprefix ${OBJ_PATH}/, $(addsuffix .o, $(basename $(FILE))))

# -- Base rules ----------
$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
   
#-----Main rule ----------
$(EXE_PATH)/$(PRODUCT): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(INC) $(LIB_LIB_PATH) -lm

# -- Other stuff ----------
clean:
	rm -f $(OBJ)
	rm -f $(PRODUCT)
	#rm -f ${LIB_PATH}/${PRODUCT}

