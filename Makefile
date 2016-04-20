## Copyright 2016 University of Stuttgart
## Authors: Dennis Hoppe

CC = /usr/bin/gcc

COPT_SO = $(CFLAGS) -fpic

CFLAGS = -std=gnu99 -pedantic -Wall -fPIC -Wwrite-strings -Wpointer-arith \
-Wcast-align -O0 -ggdb $(CURL_INC)

LFLAGS =  -lm $(CURL)

DEBUG ?= 1
ifeq ($(DEBUG), 0)
    CFLAGS += -DDEBUG -g
else
    CFLAGS += -DNDEBUG
endif

COMMON = .
EXTERN = $(COMMON)/bin

SRC = $(COMMON)/src
CONTRIB_SRC = $(SRC)/contrib
TOOLS_SRC = $(SRC)/tools
TEST_SRC = $(SRC)/test

CURL = -L$(EXTERN)/curl/lib/ -lcurl
CURL_INC = -I$(EXTERN)/curl/include/

all: clean mf_add_user mf_new_experiment mf_api mf_api_test mf_update

mf_add_user: $(TOOLS_SRC)/mf_add_user.c $(CONTRIB_SRC)/mf_publisher.c
	$(CC) $^ -o $@ -I. $(CFLAGS) $(LFLAGS)

mf_new_experiment: $(TOOLS_SRC)/mf_new_experiment.c $(CONTRIB_SRC)/mf_publisher.c
	$(CC) $^ -o $@ -I. $(CFLAGS) $(LFLAGS)

mf_api: $(SRC)/mf_api.c $(SRC)/mf_util.c $(CONTRIB_SRC)/mf_publisher.c
	$(CC) -shared $^ -o $@.so -lrt -ldl -Wl,--export-dynamic $(CFLAGS) $(LFLAGS)

mf_api_test: $(TEST_SRC)/mf_api_test.c $(SRC)/mf_api.c $(SRC)/mf_util.c $(CONTRIB_SRC)/mf_publisher.c
	$(CC) $^ -o $@ -I. $(CFLAGS) $(LFLAGS)

mf_update: $(TOOLS_SRC)/mf_update.c $(SRC)/mf_api.c $(SRC)/mf_util.c $(CONTRIB_SRC)/mf_publisher.c
	$(CC) $^ -o $@ -I. $(CFLAGS) $(LFLAGS)

install:
	@mkdir -p dist/
	@mkdir -p lib/
	@mkdir -p test/
	mv -f mf_add_user dist/
	mv -f mf_new_experiment dist/
	mv -f mf_api_test test/
	mv -f mf_update dist/
	mv -f mf_api.so lib/

clean:
	rm -rf *.o
	rm -rf *.so
	rm -rf mf_add_user
	rm -rf mf_new_experiment
	rm -rf mf_api_test
	rm -rf mf_update
	rm -rf dist
	rm -rf lib
	rm -rf test

doc: $(FILES)
	doxygen Doxyfile

