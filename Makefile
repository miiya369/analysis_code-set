
#  Make file of the Analysis code set
# Since : Sun Jul 17 01:07:30 JST 2016

# You may change this for your environment

CXX	  = g++-5
CXXFLAGS  = -Wall -O3 -g -fopenmp

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

TERGETS1    = mas wave pot fit phase eigen gfix jkbin ave makeJK tensor disp param
TERGETS2    = diff diffwave waveave deco pot2 phase2 tensor2
FOR_MIYA    = 

### Find the dependence with .cpp files under the SRCDIR and MAINDIR ###
vpath %.cpp $(wildcard $(SRCDIR)/*) $(MAINDIR) $(MAINDIR)/for_miyamoto $(MAINDIR)/for_miyamoto/test
vpath %.C $(wildcard $(SRCDIR)/extern/yukawa*)   # <- for yukawa

.PHONY: all
all: $(TERGETS1) $(TERGETS2)

mas: $(OBJDIR)/effective_mass.o $(COMMON) $(CORR) $(FIT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

wave: $(OBJDIR)/wave_function.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

pot: $(OBJDIR)/potential.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

fit: $(OBJDIR)/fitting.o $(COMMON) $(FIT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

phase: $(OBJDIR)/phase_shift.o $(COMMON) $(OBS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

eigen: $(OBJDIR)/eigen_energy_schrodinger.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

gfix: $(OBJDIR)/gfix_from_gmat.o
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

jkbin: $(OBJDIR)/jack_knife_bin_check.o $(COMMON) $(CORR)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

ave: $(OBJDIR)/average_compress.o $(COMMON) $(yukawa)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

makeJK: $(OBJDIR)/make_JK_samples.o $(COMMON) $(yukawa)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

tensor: $(OBJDIR)/tensor_force.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

disp: $(OBJDIR)/dispersion_relation.o $(COMMON) $(CORR) $(FIT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

param: $(OBJDIR)/analysis_fitparam.o $(COMMON) $(FIT) $(OBS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

diff: $(OBJDIR)/difference_chack.o $(COMMON) $(yukawa) $(NBS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

diffwave: $(OBJDIR)/difference_wave.o $(COMMON) $(yukawa) $(NBS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

waveave: $(OBJDIR)/wave_average_compress.o $(COMMON) $(yukawa)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

deco: $(OBJDIR)/wave_decompression.o $(COMMON) $(yukawa) $(NBS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

phase2: $(OBJDIR)/phase_shfit.2x2.o $(COMMON) $(OBS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

pot2: $(OBJDIR)/potential.2x2.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

tensor2: $(OBJDIR)/tensor_force.2x2.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $@ $(BINDIR)

###### For Miyamoto ######

##########################

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<
$(OBJDIR)/extern/yukawa/%.o: %.C
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

.PHONY: clean
clean:
	rm -f -r $(BINDIR)/* $(OBJDIR)/*.o $(OBJDIR)/extern/*/*.o *.dSYM
#===================================================================================#
#===================================================================================#
#===================================================================================#

###### FOR TEST ######
.PHONY: test
test: $(OBJDIR)/test.o 
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

.PHONY: ALL
ALL: $(TERGETS1) $(TERGETS2) $(FOR_MIYA)
######################

