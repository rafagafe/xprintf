
CC = gcc
CFLAGS = -g -O2 -std=gnu99 -Wall -MMD

build_dir = build

test_src_dir = .
test_obj_dir = $(build_dir)/test

module_src_dir = ..
module_obj_dir = $(build_dir)/module

test_src  = $(wildcard $(test_src_dir)/*.c)
test_obj  = $(patsubst $(test_src_dir)/%.c,$(test_obj_dir)/%.o,$(test_src))

module_src  = $(wildcard $(module_src_dir)/*.c)
module_obj  = $(patsubst $(module_src_dir)/%.c,$(module_obj_dir)/%.o,$(module_src))

src = $(test_src) $(module_src)
obj = $(test_obj) $(module_obj)
dep = $(obj:.o=.d)

.PHONY: build all clean

build: $(build_dir)/module-test

all: clean build

clean:
	rm -rf $(dep)
	rm -rf $(obj)
	rm -rf $(build_dir)/module-test

test: $(build_dir)/module-test
	$(build_dir)/module-test

$(build_dir)/module-test: $(obj)
	$(CC) $(CFLAGS) -o $@ $^

$(test_obj_dir)/%.o: $(test_src_dir)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(module_obj_dir)/%.o: $(module_src_dir)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

-include $(dep)
