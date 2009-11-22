CPP = /usr/bin/gcc
BINARY = bafprp

SOURCE_DIR = src
BIN_DIR = bin
BUILD_DIR = build

INCLUDE = -Iinclude
LDFLAGS=-lodbc -lstdc++

OPT_FLAGS = -O3 -pipe -s -funroll-loops -fno-rtti
DEBUG_FLAGS = -g -ggdb3

OBJECTS = \
bafdefines.cpp \
baffile.cpp \
bafprp.cpp \
base64.cpp \
compat.cpp \
consoleoutput.cpp \
crc32.cpp \
duplicate.cpp \
emailoutput.cpp \
fileoutput.cpp \
bafrecord.cpp \
ifield.cpp \
mailer.cpp \
mssqloutput.cpp \
mysqloutput.cpp \
csvoutput.cpp \
nooutput.cpp \
output.cpp \
field_defs.cpp \
date_dt.cpp \
duration_dt.cpp \
number_dt.cpp \
phonenumber_dt.cpp \
record_defs.cpp \
switch_dt.cpp \
time_dt.cpp \
number_switch_dt.cpp

#
# Do not edit below
#

VPATH=$(SOURCE_DIR)

CFLAGS = $(OPT_FLAGS) -fpermissive -D_LINUX -DNDEBUG -Wno-deprecated
DCFLAGS = $(DEBUG_FLAGS) -v -fpermissive -D_LINUX -D_DEBUG -Wno-deprecated
 
OBJ_LINUX := $(OBJECTS:%.cpp=$(BUILD_DIR)/%.o)
DOBJ_LINUX := $(OBJECTS:%.cpp=$(BUILD_DIR)/%_d.o)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CPP) $(INCLUDE) $(CFLAGS) -o $@ -c $<
	
$(BUILD_DIR)/%_d.o: $(SOURCE_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CPP) $(INCLUDE) $(DCFLAGS) -o $@ -c $<

default: all

all: $(BINARY) $(BINARY)_debug

$(BINARY): $(OBJ_LINUX)
	mkdir -p $(BIN_DIR)
	$(CPP) $^ $(LDFLAGS) -s -o$(BIN_DIR)/$(BINARY)

$(BINARY)_debug: $(DOBJ_LINUX)
	mkdir -p $(BIN_DIR)
	$(CPP) $^ $(LDFLAGS) -o$(BIN_DIR)/$(BINARY)_d

install: $(BINARY)
	cp $(BIN_DIR)/$(BINARY) /usr/bin/$(BINARY)
	mkdir -p /var/log/$(BINARY)

uninstall:
	rm -f /usr/bin/$(BINARY)
	rm -rf /var/log/$(BINARY)

clean:
	rm -rf $(BUILD_DIR)
