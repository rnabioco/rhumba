#include "scrunchy.h"

void splitName(const std::string &name, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(name);
    std::string item;
    while (std::getline(ss, item, delim)) {
      elems.push_back(item);
    }
  }


int main(int argc, char *argv[])
{
  if (argc < 6) {
    std::cerr << "Usage: <in.bam> <out.bam> <delim> <cbc_field> <umi_field> \n" << argv[0] ;
    return 1;
  } 
  int cbc_field = std::stoi(argv[4]) - 1;
  int umi_field = std::stoi(argv[5]) - 1;
  char delim = argv[3][0] ;
  
  //read in bam
  BamReader bfile(argv[1], false) ;  //handle opening bam
  char *fn_out = argv[2] ; //output handle
  const bam_hdr_t *header = sam_hdr_read(bfile.in) ; // get header
  bam1_t *aln = bam_init1() ; // initialize empty alignment container
  samFile *fp_out = sam_open(fn_out, "wb") ; //initialize output bam
  
  int herr = sam_hdr_write(fp_out, header) ;
  if (herr < 0){
    std::cerr << "header write error" << std::endl;
  }
  

  while (bam_read1(bfile.bz, aln) > 0) { // negative return values are errors
    
    if (((aln)->core.flag&BAM_FUNMAP) != 0) {
      sam_write1(fp_out, header, aln) ;
      continue ; 
    }

    std::string id = bam_get_qname(aln) ; //get name
    auto pos_zero = (aln)->core.pos ;
    auto pos_one  = pos_zero + 1 ;
    auto pos = std::to_string(pos_one) ; // chrom position as one-based
    auto chrom = std::string(header->target_name[(aln)->core.tid]) ;
    auto xt_info = chrom + '_' + pos ; 
            
    std::vector<std::string> id_elements ;
    splitName(id, delim, id_elements) ; //split up name field

    auto cbc = id_elements[cbc_field] ;
    auto umi = id_elements[umi_field] ;

    auto cbc_c = cbc.c_str() ;
    auto umi_c = umi.c_str() ;
    auto xt_c = xt_info.c_str() ;
    
    bam_aux_append(aln, "XT", 'Z', strlen(xt_c) + 1, (uint8_t *)(xt_c));
    bam_aux_append(aln, "BO", 'Z', strlen(umi_c) + 1, (uint8_t *)(umi_c));
    bam_aux_append(aln, "CB", 'Z', strlen(cbc_c) + 1, (uint8_t *)(cbc_c));
    
    int err = sam_write1(fp_out, header, aln) ; //write alignment ;
    if (err < 0) {
      std::cerr << "alignment writing error" << std::endl;
    }


  }
  
  bam_destroy1(aln) ;
  sam_close(fp_out) ;

  return 0;
}
