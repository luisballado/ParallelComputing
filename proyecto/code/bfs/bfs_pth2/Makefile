TARGETS = bfs_pth2

CROSS_TOOL =
CC_CPP = $(CROSS_TOOL)g++

CFLAGS = -std=c++11 -pthreads

all: clean $(TARGETS)

$(TARGETS):
	$(CC_CPP) $(CFLAGS) $@.cpp -o $@

clean:
	rm -f $(TARGETS)
