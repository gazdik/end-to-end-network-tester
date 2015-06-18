CXX=g++
CXXFLAGS= -std=c++11 -g -pthread -DNDEBUG
# the parameters to be passed to the preprocessor
CPPFLAGS=
# the parameters to be passed to the linker
LIBRARIES=-lboost_program_options
LIBDIRS=
LDFLAGS= $(LIBDIRS) $(LIBRARIES) 

######################################

# names of binaries
SERVER=ipkperfserver
CLIENT=ipkperfclient
# names of sources and objects
SERVER_SRC=src/ipkperfserver.cpp $(wildcard src/Server*.cpp) src/UDPAcceptor.cpp src/UDPStream.cpp
CLIENT_SRC=src/ipkperfclient.cpp $(wildcard src/Client*.cpp) src/UDPConnector.cpp src/UDPStream.cpp	
SERVER_OBJ=$(SERVER_SRC:.cpp=.o)
CLIENT_OBJ=$(CLIENT_SRC:.cpp=.o)
OBJ=$(SERVER_OBJ) $(CLIENT_OBJ)

######################################

all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) $(SERVER_OBJ) -o $@ $(LDFLAGS)

$(CLIENT): $(CLIENT_OBJ)
	$(CXX) $(CXXFLAGS) $(CLIENT_OBJ) -o $@ $(LDFLAGS)

######################################

# generating prerequsites automatically
%.d: %.cpp
	$(CXX) -M -MF $@ $<
	-include $(OBJ:.o=.d)

######################################

.PHONY: clean test pack libs
libs:
	echo "ipk2015" | sudo -S apt-get install -y libboost-all-dev
clean:
	rm -f src/*.o $(SERVER) $(CLIENT) ipkperf-* xgazdi03.tgz
test:
	bash test.sh
pack:
	tar -cvzf xgazdi03.tgz src/*.cpp src/*.h Makefile README test.sh
