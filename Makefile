CXX = g++
INC = c:/msys64/mingw64/include/

INC_PARAMS=$(foreach d, $(INC), -I$d)

CXXFLAGS = -Wall $(INC_PARAMS)

SLGenTokens.exe: SLGenTokens.cpp
	$(CXX) $(CXXFLAGS) -o SLGenTokens.exe SLGenTokens.cpp