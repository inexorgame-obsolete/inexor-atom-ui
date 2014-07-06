CC=gcc
CXX=g++
LD=g++

ATOMSHELL=/opt/atom-shell

CXXFLAGS=                                                                             \
	-I$(ATOMSHELL)/vendor/node/src                                                      \
	-I$(ATOMSHELL)/vendor/node/deps/uv/include                                          \
	-I$(ATOMSHELL)/vendor/brightray/vendor/download/libchromiumcontent/src/v8/include/  \
	-fPIC                                                                               \
	-D'_LARGEFILE_SOURCE'                                                               \
	-D'_FILE_OFFSET_BITS=64'                                                            \
	-D'BUILDING_NODE_EXTENSION'                                                         \
	-Wall                                                                               \
	-Wextra                                                                             \
	-Wno-unused-parameter                                                               \
	-m64                                                                                \
	-pthread                                                                            \
	-O4                                                                                 \
	-ffunction-sections                                                                 \
	-fdata-sections                                                                     \
	-fno-omit-frame-pointer                                                             \
	-fno-rtti                                                                           \
	-fno-exceptions                                                                     \
	-fno-tree-vrp                                                                       \
	-fno-tree-sink                                                                      \
	#-MMD                                                                               \
	#-MF                                                                                \

CFLAGS=$(CXXFLAGS)

LDFLAGS=                             \
	-shared                            \
	-pthread                           \
	-rdynamic                          \
	-L$(ATOMSHELL)/out/Release/        \
	-lchromiumcontent                  \
	-m64                               \
	-Wl,-gc-selections,--start-group,  \
	-O4                                \
	#-lstdc++                           \

compile:
	$(CXX) $(CXXFLAGS) -c -o boop.o boop.cc
	$(LD) $(LDFLAGS) -o boop -Wl,--soname=boop boop.o
