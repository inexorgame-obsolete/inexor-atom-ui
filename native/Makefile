CC=clang
CXX=clang++
LD=clang
LDXX=clang++

ATOMSHELL=/opt/atom-shell
VENDOR="$(ATOMSHELL)/out/Debug/obj/vendor"

CXXFLAGS=                      \
	--std=c++11                         \
	-I$(ATOMSHELL)/include       \
	-I$(ATOMSHELL)               \
	-I$(ATOMSHELL)/include/libchromiumcontent/ \
	-I$(ATOMSHELL)/vendor/brightray/vendor/download/libchromiumcontent/src/ \
	-I$(ATOMSHELL)/vendor/brightray/ \
	-I$(ATOMSHELL)/vendor/node/deps/uv/include/ \
	-I$(ATOMSHELL)/vendor/node/src/ \
	-fPIC                        \
	-D'_LARGEFILE_SOURCE'        \
	-D'_FILE_OFFSET_BITS=64'     \
	-D'BUILDING_NODE_EXTENSION'  \
	-Wall                        \
	-Wextra                      \
	-Wno-unused-parameter        \
	-pthread                     \
	-O0                          \
	-g                           \
	-ffunction-sections          \
	-fdata-sections              \
	-fno-omit-frame-pointer      \
	-fno-rtti                    \
	-fno-exceptions              \
	$(shell pkg-config gtk+-3.0 --cflags) \
	#-fno-tree-vrp                \
	#-fno-tree-sink               \
	#-MMD                        \
	#-MF                         \

CFLAGS=$(CXXFLAGS)

LDFLAGS=                              \
	--std=c++11                         \
	-L$(ATOMSHELL)/out/Debug/           \
	-L$(ATOMSHELL)/out/Debug/obj/       \
	-L$(VENDOR)/breakpad                \
	-L$(VENDOR)/brightray               \
	-L$(VENDOR)/node                    \
	-L$(VENDOR)/node/deps/zlib          \
	-L$(VENDOR)/node/deps/cares         \
	-L$(VENDOR)/node/deps/http_parser   \
	-L$(VENDOR)/node/deps/uv            \
	-L$(VENDOR)/node/deps/openssl       \
	-L$(VENDOR)/brightray/              \
	-lpthread                           \
	-lstdc++                            \
	-lchromiumcontent                   \
	-lSDL                               \
	-latom_lib                          \
	$(shell pkg-config gtk+-2.0 --libs) \
	-ldl                                \
	-lbreakpad_client                   \
	-lnode_lib                          \
	-lchrome_zlib                       \
	-lcares                             \
	-lhttp_parser                       \
	-luv                                \
	-lopenssl                           \
	-lc                                 \
	-lbrightray                         \
	-lnotify                            \
	-O0                                 \
	-g                                  \

compile:
	$(CXX) $(CXXFLAGS) -c -o boop.o boop.cc
	$(LD) -Wl,--start-group boop.o $(LDFLAGS) -o boop -Wl,--end-group
	#$(LD) $(LDFLAGS) -shared -rdynamic -o boop.node -Wl,--soname=boop.node  boop.o
