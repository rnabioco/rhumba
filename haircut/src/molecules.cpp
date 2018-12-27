#include "scrunchy.h"

std::map<char, int> nucmap = {{'A', 0}, 
                              {'C', 1}, 
                              {'G', 2},
                              {'T', 3}};

                              
std::array<char, 4> nucs = {'A', 'C', 'G', 'T'}; 

uint64_t compress_seq(const std::string& seq){
    // compress into uint64
  if (seq.size() > 31){
    std::cerr << "sequence too large for bit encoding" << std::endl ;
    exit(1) ;
  }
  
  uint64_t result(0) ;
  
  for(const char& nuc:seq) {
    if(nucmap.find(nuc) == nucmap.end()){
    // non-standard nucleotide found, set most significant bit
      result |= 1UL << 63 ;
      return result ;
    }
    result = result << 2 ;
    result = result | nucmap[nuc] ;
  }

  return result ;

}

std::string decompress_seq(uint64_t bitseq, int seq_length = 10){
  
  if (seq_length > 31){
    std::cerr << "sequence too large to extract from uint64" << std::endl ;
    exit(1) ;
  }
  
  if (bitseq & (1UL << 63)){
    // error in compressed sequence return Ns 
    return std::string(seq_length, 'N') ;
  }
  
  uint64_t bin_value = 0b11 ;
  uint64_t shift_value = 2 ;
   
  std::string seq(seq_length, ' ');
  
  for (int i = 0; i < seq_length; i++){
    seq[(seq_length - 1) - i] = nucs[bitseq & bin_value] ;
    bitseq = bitseq >> shift_value ; 
  }
  
  return seq ;
} 


Molecule::Molecule(bam1_t *aln){
  uint8_t *tag ;
  
  if ((tag = bam_aux_get(aln, "BX"))) {
    bx = std::string(bam_aux2Z(tag)) ;
    umibit = compress_seq(bx) ;
  } else {
    has_tags = false ;
    return ;
  }
  
  if ((tag = bam_aux_get(aln, "UG"))) {
    ug = bam_aux2i(tag) ;
  } else {
    has_tags = false ;
    return ;
  }
  
  if ((tag = bam_aux_get(aln, "XT"))) {
    xt = std::string(bam_aux2Z(tag)) ;
  } else {
    has_tags = false ;
    return ;
  } 
  
  if ((tag = bam_aux_get(aln, "CB"))) {
    cb = std::string(bam_aux2Z(tag)) ;
  } else {
    has_tags = false ;
    return ;
  } 
  
  has_tags = true ; 
}

