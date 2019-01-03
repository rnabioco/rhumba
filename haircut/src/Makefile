CXXFLAGS = -Wall -O2 -std=c++0x 
LIBS = -lz -lm -lhts -Lhtslib/

all: .PHONY tag_bam get_molecule_info 

.PHONY: htslib

.cpp: htslib
	$(CXX) $(CXXFLAGS) -Ihtslib -o $@ $< bamreader.cpp molecules.cpp $(LIBS) htslib/libhts.a

htslib:
	cd htslib && ./configure && $(MAKE) CC="$(CC)" CFLAGS="$(CFLAGS) $(CPICFLAGS)" AR="$(AR)" RANLIB="$(RANLIB)"
