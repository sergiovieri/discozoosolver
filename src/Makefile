ifeq ($(COMP),mingw)
EXE = discozoosolver.exe
else
EXE = discozoosolver
endif

SRCS = main.cpp animal.cpp solver.cpp

OBJS = $(notdir $(SRCS:.cpp=.o))

CXX=g++
CXXFLAGS += -Wall -Wcast-qual -fno-exceptions -std=c++17 -march=native -Ofast
DEPENDFLAGS += -std=c++17

build: $(EXE) .depend

$(EXE): $(OBJS)
	+$(CXX) -o $@ $(OBJS)

.depend: $(SRCS)
	-@$(CXX) $(DEPENDFLAGS) -MM $(SRCS) > $@

-include .depend
