
#  Make file of the Analysis code set
# Since : Tue Aug 25 16:55:07 JST 2015

# You may change this for your environment

CXX	  = g++-5
CXXFLAGS  = -Wall -O3 -g -fopenmp

#===================================================================================#
#============================= DON’T CHANGE FROM HERE ==============================#
#===================================================================================#
MAINDIR   = ./main
SRCDIR	  = ./src
OBJDIR	  = ./obj
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

TERGETS	    = mas pot fit obs 
FOR_MIYA    = ccp zfac iso red gfix

### SRCDIR と MAINDIR 以下の全ての .cpp ファイルの依存関係を探す ###
vpath %.cpp $(wildcard $(SRCDIR)/*) $(MAINDIR) $(MAINDIR)/for_miyamoto
vpath %.C $(wildcard $(SRCDIR)/extern/yukawa*)   # <- for yukawa

.PHONY: all
all: $(TERGETS)

mas: $(OBJDIR)/effective_mass.o $(COMMON) $(CORR) $(FIT)
	$(CXX) $(CXXFLAGS) -o $@ $^

pot: $(OBJDIR)/potential.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^

fit: $(OBJDIR)/fitting.o $(COMMON) $(FIT)
	$(CXX) $(CXXFLAGS) -o $@ $^

obs: $(OBJDIR)/observable.o $(COMMON) $(OBS)
	$(CXX) $(CXXFLAGS) -o $@ $^

iso: $(OBJDIR)/isospin_proj.o $(COMMON)
	$(CXX) $(CXXFLAGS) -o $@ $^

ccp: $(OBJDIR)/coupled_channel_potential.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^

zfac: $(OBJDIR)/extract_Z-factor.o $(COMMON) $(CORR)
	$(CXX) $(CXXFLAGS) -o $@ $^

red: $(OBJDIR)/data_reduction_check.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^

gfix: $(OBJDIR)/check_coulomb_gfix.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<
$(OBJDIR)/extern/yukawa/%.o: %.C
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

.PHONY: clean
clean:
	rm -f -r $(TERGETS) $(FOR_MIYA) test obj/*.o obj/extern/*/*.o *.dSYM
#===================================================================================#
#===================================================================================#
#===================================================================================#

###### FOR TEST ######
.PHONY: test
test: $(OBJDIR)/test.o $(COMMON) $(CORR) $(NBS) $(yukawa) $(RCORR) $(POT)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: ALL
ALL: $(TERGETS) $(FOR_MIYA)
