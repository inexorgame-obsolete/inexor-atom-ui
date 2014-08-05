CC=clang
CXX=clang++
LD=clang
LDXX=clang++

ATOMSHELL=../../atom-shell/
VENDOR="$(ATOMSHELL)/out/Debug/obj/vendor"

CXXFLAGS=                      \
	-I$(ATOMSHELL)/include       \
	-I$(ATOMSHELL)               \
	-I$(ATOMSHELL)/include/libchromiumcontent/ \
	-I$(ATOMSHELL)/vendor/brightray/vendor/download/libchromiumcontent/src/ \
	-I$(ATOMSHELL)/vendor/brightray/ \
	-I$(ATOMSHELL)/vendor/node/deps/uv/include/ \
	-I$(ATOMSHELL)/vendor/node/src/ \
	-I$(ATOMSHELL)/vendor/brightray/vendor/libchromiumcontent/vendor/chromium/src \
	-I$(ATOMSHELL)/vendor/brightray/vendor/download/libchromiumcontent/src/third_party/skia/include/core/ \
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
	-fno-exceptions              \
	-fno-rtti                    \
	$(shell pkg-config --cflags gdk-pixbuf-2.0) \
	$(shell pkg-config gtk+-2.0 --cflags) \
	$(shell pkg-config libgnomeui-2.0 --cflags) \
	$(shell pkg-config --cflags cairo)    \
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
	$(shell pkg-config --libs gdk-pixbuf-2.0) \
	$(shell pkg-config gtk+-2.0 --libs) \
	$(shell pkg-config libgnomeui-2.0 --libs) \
	$(shell pkg-config --libs cairo)    \
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
	$(CXX) $(CXXFLAGS) --std=c++11 -c -o boop.o boop.cc
	$(CC) $(CFLAGS) --std=c11 -c -o ghbcompositor.o ghbcompositor.c
	$(LD) -v -Wl,--start-group boop.o ghbcompositor.o $(LDFLAGS) -o boop -Wl,--end-group
	#$(LD) $(LDFLAGS) -shared -rdynamic -o boop.node -Wl,--soname=boop.node  boop.o
