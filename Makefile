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

CURL = -L$(EXTERN)/curl/lib/ -lcurl
CURL_INC = -I$(EXTERN)/curl/include/

all: clean mf_add_user mf_new_experiment mf_api mf_client mf_update

mf_add_user: $(TOOLS_SRC)/mf_register_wf.c $(CONTRIB_SRC)/mf_publisher.c
	$(CC) $^ -o $@ -I. $(CFLAGS) $(LFLAGS)

mf_new_experiment: $(TOOLS_SRC)/mf_create_experiment.c $(CONTRIB_SRC)/mf_publisher.c
	$(CC) $^ -o $@ -I. $(CFLAGS) $(LFLAGS)

mf_api: $(SRC)/mf_api.c $(SRC)/mf_util.c $(CONTRIB_SRC)/mf_publisher.c
	$(CC) -shared $^ -o $@.so -lrt -ldl -Wl,--export-dynamic $(CFLAGS) $(LFLAGS)

mf_client: $(TOOLS_SRC)/mf_client.c $(SRC)/mf_api.c $(SRC)/mf_util.c $(CONTRIB_SRC)/mf_publisher.c
	$(CC) $^ -o $@ -I. $(CFLAGS) $(LFLAGS)

mf_update: $(TOOLS_SRC)/mf_update.c $(SRC)/mf_api.c $(SRC)/mf_util.c $(CONTRIB_SRC)/mf_publisher.c
	$(CC) $^ -o $@ -I. $(CFLAGS) $(LFLAGS)

install:
	@mkdir -p dist/
	@mkdir -p lib/
	mv -f mf_add_user dist/
	mv -f mf_new_experiment dist/
	mv -f mf_client dist/
	mv -f mf_update dist/
	mv -f mf_api.so lib/

clean:
	rm -rf *.o
	rm -rf *.so
	rm -rf mf_add_user
	rm -rf mf_new_experiment
	rm -rf mf_client
	rm -rf mf_update
	rm -rf dist
	rm -rf lib

doc: $(FILES)
	doxygen Doxyfile

