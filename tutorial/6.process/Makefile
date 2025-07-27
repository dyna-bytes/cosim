# Makefile for a SystemC program using pkg-config

# --- Variables ---

# Compiler
CXX = g++

# SystemC paths
SYSTEMC_LIB_DIR := /workspace/cosim/lib-linux64

# Use pkg-config to get flags
PKG_CONFIG_PATH := $(SYSTEMC_LIB_DIR)/pkgconfig
SYSTEMC_CXXFLAGS := $(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags systemc)
SYSTEMC_LDFLAGS := $(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs --static systemc)

# Compiler and Linker flags
CXXFLAGS += -std=c++11 $(SYSTEMC_CXXFLAGS)
# Add rpath to tell the runtime linker where to find the .so file
LDFLAGS += $(SYSTEMC_LDFLAGS) -Wl,-rpath,$(SYSTEMC_LIB_DIR)

method: method.cc
	$(CXX) $(CXXFLAGS) -o method method.cc $(LDFLAGS)

thread: thread.cc
	$(CXX) $(CXXFLAGS) -o thread thread.cc $(LDFLAGS)

clock_thread: clock_thread.cc
	$(CXX) $(CXXFLAGS) -o clock_thread clock_thread.cc $(LDFLAGS)