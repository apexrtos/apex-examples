#
# Example program including libraries
#

TYPE = prog
TARGET = example_program
SOURCES := main.cpp example.c
LIBS := ../lib1/lib1.a lib2/lib2.a
MK := ../lib1/lib1.mk lib2/lib2.mk
INCLUDE := $(CONFIG_SRCDIR)			# or ..
