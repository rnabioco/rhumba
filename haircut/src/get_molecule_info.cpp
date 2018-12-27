#include "scrunchy.h"
#include <iostream>
#include <fstream>
#include <algorithm>

int usage(){
  std::cerr << "./get_molecule_info <in.bam> <output_directory> " << std::endl ;
  return 1 ;
}

void write_output(uint64_t current_cell,
                  std::map<std::array<uint64_t, 2>, int> umimap,
                  std::ofstream& outfile){

  for(auto const &entry : umimap) {
    outfile << current_cell << "\t"
            << entry.first[0] << "\t"
            << entry.first[1] << "\t"
            << entry.second << std::endl ;
  }
}

void get_molecules(std::string filename, std::string path){
  // make map to track each unique umi::gene combo
  // store UMI as bits
  // store feature as index to feature table
  typedef std::array<uint64_t, 2> umi_features ;

  std::map<umi_features, int> umimap ;
  
  BamReader bfile(filename, false) ;
  bam_hdr_t *h = sam_hdr_read(bfile.in) ; // get header
  bam1_t *aln = bam_init1() ;
  
  std::map<std::string, uint64_t> txmap ; 
  std::map<std::string, uint64_t> bcmap ; 
  
  std::string infofile_fn = path + "/" + "molecules.tsv" ;
  std::ofstream infofile ;
  infofile.open(infofile_fn) ;

  std::string current_cell;
  while (bam_read1(bfile.bz, aln) > 0){
    Molecule molecule(aln) ;
    if (!molecule.has_tags) {
      continue ;
    }
    if (current_cell.empty()){
      current_cell = molecule.cb ;
    }
    
    // add to maps if key is unique, storing one-based index
    txmap.emplace(molecule.xt, txmap.size() + 1);    
    bcmap.emplace(molecule.cb, bcmap.size() + 1);    
    

    // use 2bit umi and txmap index as key value
    std::array<uint64_t, 2> key_value = {txmap[molecule.xt], molecule.umibit} ;
    
    if(molecule.cb == current_cell){
      umimap[key_value] += 1 ; 
    } else {
      write_output(bcmap[current_cell], umimap, infofile) ;
      current_cell = molecule.cb ;
      umimap.clear() ;
      umimap[key_value] += 1 ; 
    }
  }

  // write out final map
  write_output(bcmap[current_cell], umimap, infofile) ;
  
  infofile.close() ;

  // write out barcodes tsv
  std::string bcfile_fn = path + "/" + "barcodes.tsv" ;
  std::ofstream bcfile ;
  bcfile.open(bcfile_fn) ;
  typedef std::pair<std::string, uint64_t> strPair ;
  
  // sort by value (index)
  std::vector<strPair> bcs(bcmap.begin(), bcmap.end() );
  std::sort(bcs.begin(), bcs.end(), 
          [](const strPair &left, const strPair &right) {
              return left.second < right.second;
              });
  
  for(auto bc:bcs) {
    bcfile << bc.first << std::endl ;
  }

  bcfile.close() ;

  // write out feature tsv
  std::string feature_fn = path + "/" + "features.tsv" ;
  std::ofstream featurefile ;
  featurefile.open(feature_fn) ;
  
  // sort by value (index)
  std::vector<strPair> flist(txmap.begin(), txmap.end() );
  std::sort(flist.begin(), flist.end(), 
          [](const strPair &left, const strPair &right) {
              return left.second < right.second;
              });
  
  for(auto f:flist) {
    featurefile << f.first << std::endl ;
  }
  
  featurefile.close();
}  
  
int main(int argc, char *argv[]){
  if (argc == 1) {
    return usage() ;
  } 
  
  std::string filename(argv[1]) ;
  std::string output_path = "" ;
  
  if (argc == 3) {
    output_path = argv[2] ;
  }

  try {
    get_molecules(filename, output_path) ;
  } catch (const std::runtime_error& e) {
    std::cerr << e.what() ;
  }
}
