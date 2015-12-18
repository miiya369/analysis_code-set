
# Make file of the Analysis code set
# You may change this for your environment

CXX	  = g++
CXXFLAGS  = -Wall -O2 -g

MAINDIR   = ./main
SRCDIR	  = ./src
OBJDIR	  = ./obj
HEADERDIR = ./include

#===================================================================================#
#============================= DON’T CHANGE FROM HERE ==============================#
#===================================================================================#

SRCS_cmn = $(wildcard $(SRCDIR)/common/*.cpp)
SRCS_mas = $(MAINDIR)/effective_mass.cpp $(wildcard $(SRCDIR)/potential/*.cpp) \
$(wildcard $(SRCDIR)/fitting/*.cpp) $(SRCS_cmn)
SRCS_pot = $(MAINDIR)/potential.cpp $(wildcard $(SRCDIR)/potential/*.cpp) $(SRCS_cmn)
SRCS_fit = $(MAINDIR)/fitting.cpp $(wildcard $(SRCDIR)/fitting/*.cpp) $(SRCS_cmn)
SRCS_obs = $(MAINDIR)/observable.cpp $(wildcard $(SRCDIR)/observable/*.cpp) $(SRCS_cmn)
SRCS_iso = $(MAINDIR)/isospin_proj.cpp $(SRCS_cmn)
SRCS_ccp = $(MAINDIR)/coupled_channel_potential.cpp $(SRCS_cmn) \
$(wildcard $(SRCDIR)/potential/*.cpp)

OBJS_mas = $(patsubst %,$(OBJDIR)/%,$(notdir $(SRCS_mas:.cpp=.o)))
OBJS_pot = $(patsubst %,$(OBJDIR)/%,$(notdir $(SRCS_pot:.cpp=.o)))
OBJS_fit = $(patsubst %,$(OBJDIR)/%,$(notdir $(SRCS_fit:.cpp=.o)))
OBJS_obs = $(patsubst %,$(OBJDIR)/%,$(notdir $(SRCS_obs:.cpp=.o)))
OBJS_iso = $(patsubst %,$(OBJDIR)/%,$(notdir $(SRCS_iso:.cpp=.o)))
OBJS_ccp = $(patsubst %,$(OBJDIR)/%,$(notdir $(SRCS_ccp:.cpp=.o)))

INCLUDES = $(patsubst %,-I%,$(HEADERDIR))

TERGETS		= mas pot fit obs
FOR_MIYA	= iso ccp

### SRCDIR と MAINDIR 以下の全ての .cpp ファイルの依存関係を探す ###
vpath %.cpp $(wildcard $(SRCDIR)/*) $(MAINDIR)

.PHONY: all
all: $(TERGETS)

mas: $(OBJS_mas)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_mas)

pot: $(OBJS_pot)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_pot)

fit: $(OBJS_fit)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_fit)

obs: $(OBJS_obs)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_obs)

iso: $(OBJS_iso)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_iso)

ccp: $(OBJS_ccp)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_ccp)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

.PHONY: clean
clean:
	rm -f $(TERGETS) $(FOR_MIYA) obj/*.o

#===================================================================================#
#===================================================================================#
#===================================================================================#

###### FOR TEST ######
.PHONY: test
test:
	@echo $(SRCS_pot) $(OBJS_pot)

.PHONY: ALL
ALL: $(TERGETS) $(FOR_MIYA)
