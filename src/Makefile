################################################################################
##
## Filename: 	Makefile
##
## Project:	tttt, a simple 4x4x4 Tic-Tac-Toe Program
##
## Purpose:	To coordinate the build of the 4x4x4 Tic-Tac-Toe program of
##		course!
##
##	This build is customized to work with cross compilers by two environment
##	variables:
##
##	$(ARCH)	Specifies the name of your architecture.  All object files
##		will be placed into a directory with $(ARCH) as its suffix,
##		or obj-pc if none is given.
##
##	$(CROSS) specifies the prefix of the cross-compiler.  For the ZipCPU,
##		this is currently "zip-".  For other machines, other prefixes
##		may be appropriate.  If no prefix is given, the Makefile will
##		assume that the compiler is given by "gcc".
##
## Targets:
##
##	all	Builds the program for the current architecture
##
##	depends	Rebuilds the dependency list for the current architecture
##
##	clean	Remove all build products for the current architecture
##
##	pc	Builds the program for the host architecture, ARCH=pc and
##		CROSS= (nothing).  This is the build you will get from using
##		gcc.
##
##	tags	Build a tags file, suitable for searching through the source
##		code to look for definitions of things.
##
##	zip	Build this program for the ZipCPU.
##
##
##
## Creator:	Dan Gisselquist, Ph.D.
##		Gisselquist Technology, LLC
##
################################################################################
##
## Copyright (C) 2017, Gisselquist Technology, LLC
##
## This program is free software (firmware): you can redistribute it and/or
## modify it under the terms of  the GNU General Public License as published
## by the Free Software Foundation, either version 3 of the License, or (at
## your option) any later version.
##
## This program is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or
## FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
## for more details.
##
## You should have received a copy of the GNU General Public License along
## with this program.  (It's in the $(ROOT)/doc directory, run make with no
## target there if the PDF file isn't present.)  If not, see
## <http://www.gnu.org/licenses/> for a copy.
##
## License:	GPL, v3, as defined and found on www.gnu.org,
##		http://www.gnu.org/licenses/gpl.html
##
##
################################################################################
##
##
CROSS ?=
ARCH  ?= pc
CC      := $(CROSS)gcc
OBJDIR  := obj-$(ARCH)
SOURCES := comborow.c comboset.c gboard.c strategy.c vset.c main.c
OBJECTS := $(addprefix $(OBJDIR)/,$(subst .c,.o,$(SOURCES)))

all: $(OBJDIR)/ $(CROSS)tttt

#
# Set some eXtra make variables, such as might be used by your CPU of interest
#
ifeq ($(ARCH), zip)
XLIBD    := ../../branch8b/sw/zlib
XLIBS    := -L$(XLIBD) -Wl,--start-group -Wl,--Map=zip-tttt.map -larty
LDSCRIPT := $(XLIBD)/../board/arty.ld
XFLAGS   := -T$(LDSCRIPT)
CFLAGS  := -O3 -Wall
else
XLIBD   :=
XLIBS  :=
XFLAGS :=
CFLAGS  := -g -Og -Wall
endif


# Build a copy of this for the ZipCPU, by calling make recursively to make 
# certain that the environment is properly set
zip:
	make ARCH=zip CROSS=zip-

pc:
	make ARCH=pc CROSS=

# Make sure we have our object file directory
$(OBJDIR)/:
	@bash -c "if [ ! -e $(OBJDIR) ]; then mkdir -p $(OBJDIR); fi"

# Compile the .c files into object files in our object file directory
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# For compiler debugging purposes, allow us to look at the assembly generated
# for any file
%.s: $(OBJDIR)/%.s
	@echo $< has been built
	@cp $< $@
$(OBJDIR)/%.s: %.c
	$(CC) $(CFLAGS) -S -fdump-rtl-all $< -o $@

# Build the final executable
$(CROSS)tttt: $(OBJECTS)
	$(CC) $(XFLAGS) $(OBJECTS) $(XLIBS) -o $@

$(CROSS)tttt.txt: $(CROSS)tttt
	$(CROSS)objdump -dr $(CROSS)tttt > $(CROSS)tttt.txt


# Build dependency information
define	build-depends
	@echo "Building dependency file"
	@$(CC) $(CFLAGS) -MM $(SOURCES) > $(OBJDIR)/xdepends.txt
	@sed -e 's/^.*.o: /$(OBJDIR)\/&/' < $(OBJDIR)/xdepends.txt > $(OBJDIR)/depends.txt
	@rm $(OBJDIR)/xdepends.txt
endef

.PHONY: depends
depends: $(OBJDIR)/ tags
	$(build-depends)


# Build the tags file for this target--depends upon ctags
tags:	$(SOURCES)
	@echo "Generating tags"
	@ctags $(SOURCES)

# The "clean" target--removes everything from the object file directory, as
# well as our executable.  This includes removing the depends file.
.PHONY: clean
clean:
	rm -rf $(OBJDIR)/
	rm -f $(CROSS)tttt

# The rule to rebuild the depends file if it doesn't exist.  This rule will
# *ALWAYS* be invoked, since depends is a PHONY target, so dependencies will
# *ALWAYS* be redetermined at the beginning of any build.
$(OBJDIR)/depends.txt: depends

# Include our depends file as though it were a part of this makefile
-include $(OBJDIR)/depends.txt
