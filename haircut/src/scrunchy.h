#ifndef subsampling__subsampling_H 
#define subsampling__subsampling_H

#include <array>
#include <zlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <set>
#include "htslib/hts.h"
#include "htslib/sam.h"
#include "htslib/bgzf.h"

// class for handling bam file opening and closing
class BamReader {
public:
  samFile* in;
  hts_idx_t* idx;
  BGZF* bz;
  BamReader(const std::string& bampath, 
            bool check_idx = true,
            int cache_size=10*BGZF_MAX_BLOCK_SIZE) ;
  
  ~BamReader(){
    hts_idx_destroy(idx);
    sam_close(in);
  }
};

// Class for handling umis
class Molecule {
public:
  std::string cb, bx, xt ;
  uint64_t ug, umibit ; 
  bool has_tags ; 
  Molecule(bam1_t*) ;
};


#endif
