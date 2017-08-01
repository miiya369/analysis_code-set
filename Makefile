
# Make file of the Analysis code set
# Since : Fri Feb 17 21:09:33 JST 2017

# You may change this for your environment

CXX	  = g++-5
CXXFLAGS  = -Wall -O3 -g -fopenmp #-DDEBUG_MODE

AR	  = ar

#===================================================================================#
#============================= DON’T CHANGE FROM HERE ==============================#
#===================================================================================#
MAINDIR   = ./main
SRCDIR	  = ./src
OBJDIR	  = ./obj
BINDIR	  = ./bin
HEADERDIR = ./include ./include/extern

INCLUDES    = $(patsubst %,-I%,$(HEADERDIR))

SRCS_COMMON = $(wildcard $(SRCDIR)/common/*.cpp)
SRCS_NBS    = $(wildcard $(SRCDIR)/NBSwave/*.cpp)
SRCS_CORR   = $(wildcard $(SRCDIR)/correlator/*.cpp)
SRCS_RCORR  = $(wildcard $(SRCDIR)/R_correlator/*.cpp)
SRCS_POT    = $(wildcard $(SRCDIR)/potential/*.cpp)
SRCS_FIT    = $(wildcard $(SRCDIR)/fitting/*.cpp)
SRCS_OBS    = $(wildcard $(SRCDIR)/observable/*.cpp)
SRCS_yukawa = $(wildcard $(SRCDIR)/extern/yukawa*/*.C)

COMMON      = $(patsubst %,$(OBJDIR)/%,$(notdir $(SRCS_COMMON:.cpp=.o)))
NBS         = $(patsubst %,$(OBJDIR)/%,$(notdir $(SRCS_NBS:.cpp=.o)))
CORR        = $(patsubst %,$(OBJDIR)/%,$(notdir $(SRCS_CORR:.cpp=.o)))
RCORR       = $(patsubst %,$(OBJDIR)/%,$(notdir $(SRCS_RCORR:.cpp=.o)))
POT         = $(patsubst %,$(OBJDIR)/%,$(notdir $(SRCS_POT:.cpp=.o)))
FIT         = $(patsubst %,$(OBJDIR)/%,$(notdir $(SRCS_FIT:.cpp=.o)))
OBS         = $(patsubst %,$(OBJDIR)/%,$(notdir $(SRCS_OBS:.cpp=.o)))
yukawa	    = $(patsubst %,$(OBJDIR)/extern/yukawa/%,$(notdir $(SRCS_yukawa:.C=.o)))

LIB_ARPACK  = $(HOME)/local/lib/libarpack.a $(HOME)/local/lib/liblapack.a

TERGETS1    = mas wave pot tensor fit phase pot2 pot3 Tmat2 eigen jkbin
TERGETS2    = gfix disp deco eigenarp fpot
FOR_MIYA    = tensor2

### Find the dependence with .cpp files under the SRCDIR and MAINDIR ###
vpath %.cpp $(wildcard $(SRCDIR)/*) $(MAINDIR) \
$(MAINDIR)/zzz.ForMiyamoto $(MAINDIR)/zzz.ForMiyamoto/test
vpath %.C $(wildcard $(SRCDIR)/extern/yukawa*)   # <- for yukawa

.PHONY: all
all: $(TERGETS1)
.PHONY: All
All: $(TERGETS1) $(TERGETS2)
.PHONY: ALL
ALL: $(TERGETS1) $(TERGETS2) $(FOR_MIYA)

mas: $(OBJDIR)/main_effective_mass.o $(COMMON) $(CORR) $(FIT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

wave: $(OBJDIR)/main_wave_function.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

pot: $(OBJDIR)/main_potential.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

tensor: $(OBJDIR)/main_tensor_force.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

fit: $(OBJDIR)/main_fitting.o $(COMMON) $(FIT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

phase: $(OBJDIR)/main_phase_shift.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT) $(OBS) $(FIT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

pot2: $(OBJDIR)/main_potential.2x2.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

pot3: $(OBJDIR)/main_potential.3x3.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

Tmat2: $(OBJDIR)/main_Tmatrix.2x2.o $(COMMON) $(OBS) $(FIT) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

gfix: $(OBJDIR)/main_gfix_from_gmat.o
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

jkbin: $(OBJDIR)/main_jack_knife_bin_check.o $(COMMON) $(CORR)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

disp: $(OBJDIR)/main_dispersion_relation.o $(COMMON) $(CORR) $(FIT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

deco: $(OBJDIR)/main_wave_decompression.o $(COMMON) $(yukawa) $(NBS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

eigen: $(OBJDIR)/main_sch_eigenvalue.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

eigenarp: $(OBJDIR)/main_sch_eigen_arpack.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT) $(LIB_ARPACK)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lgfortran
	mv $@ $(BINDIR)

fpot: $(OBJDIR)/main_folding_potential.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT) $(FIT) $(OBS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

###### For Miyamoto ######
tensor2: $(OBJDIR)/main_tensor_force.2x2.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)
##########################

lib: $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT) $(FIT) $(OBS)
	$(AR) rcs analysisHAL.a $^

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<
$(OBJDIR)/extern/yukawa/%.o: %.C
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

.PHONY: clean
clean:
	rm -f -r $(BINDIR)/* $(OBJDIR)/*.o $(OBJDIR)/extern/*/*.o *.dSYM *.a
#===================================================================================#
#===================================================================================#
#===================================================================================#

###### FOR TEST ######
FOR_TEST = test test_pot test_tensor test_rot test_wave test_phase test_density

.PHONY: test
test: $(OBJDIR)/test.o 
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

.PHONY: test_pot
test_pot: $(OBJDIR)/test_potential.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

.PHONY: test_tensor
test_tensor: $(OBJDIR)/test_tensor.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

.PHONY: test_rot
test_rot: $(OBJDIR)/test_rot.o $(COMMON) $(NBS) $(yukawa)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

.PHONY: test_wave
test_wave: $(OBJDIR)/test_wave.o $(COMMON) $(NBS) $(yukawa)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

.PHONY: test_phase
test_phase: $(OBJDIR)/test_phase.o $(COMMON) $(OBS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

.PHONY: test_density
test_density: $(OBJDIR)/test_density.o $(COMMON) $(OBS) $(FIT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

.PHONY: ALL_TEST
ALL: $(TERGETS1) $(TERGETS2) $(FOR_MIYA) $(FOR_TEST)
######################

