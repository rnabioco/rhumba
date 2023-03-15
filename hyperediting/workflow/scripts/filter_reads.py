#!/usr/bin/env python3
import argparse
import os, sys
import gzip
import pysam 
from scipy.stats.mstats import mquantiles
from itertools import groupby
import edit_utils as utils

def check_nucleotide_content(seq):
    
    """ check sequence for good nucleotide content 
    no single nucleotide > 60% or < 10%
    no N's > 10% of read
    no strech of a single nt longer than 20
    return False if any conditions are not satisfied
    """
    seq_length = len(seq)
    nt_percent = dict() 
    nt_count = utils.base_counter()
    
    for i in seq:
        nt_count[i] += 1
     
    nt_percent = dict() 
    
    for nt, count in nt_count.items():
        nt_percent[nt] = 100 * (count / seq_length)

    for nt, percent in nt_percent.items():
        if nt == "N" and percent > 10:
            return False
        if nt == "N":
            continue
        if percent > 60 or percent < 10:
            return False

    #compute run length encoding for seq
    seq_rle = [(len(list(count)), nt) for nt, count in groupby(seq)]
    
    # if single nucleotide is repeated for > 20 positions drop read
    for unique_seq in seq_rle:
        if unique_seq[0] > 20:
            return False
    return True

def check_qual(qual):
    """check seq for mean quality < 25 
       drop qual scores in lowest decile
    """
    
    quals = []
    for i in qual:
        quals.append(ord(i) - 33)

    # drop lowest decile of qual scores
    decile = float(mquantiles(quals, prob = [0.1]))
    
    quals = [x for x in quals if x > decile]

    mean_qual = float(sum(quals)) / max(len(quals), 1)
    
    if mean_qual < 25:
        return False
    else:
        return True

def check_repeats(seq, repeats, num_repeats = 10):
    
    """ check for repeat but exclude single nucleotide repeats """

    for test_pattern in repeats:     
        if len(test_pattern) == 1:
            continue

        test_pattern_rpt = test_pattern * num_repeats

        if test_pattern_rpt in seq:
            return False

    return True 

def is_mapped(x, mate):
    mapped = False
    mates = ["R1", "R2"]
    # take last comment element as output from STAR to query
    map_status = x.split()[-1]
    if mate not in mates:
        sys.exit("unknown read mate format, one of R1 or R2")

    if map_status[mates.index(mate)] == "0":
        mapped = False
    else:
        mapped = True

    return mapped
   

def process_fastq(fastq_file, repeats_file, mate, gzip_fh_out):
    
    test_patterns = [x.strip('\r\n') for x in repeats_file]

    for read in fastq_file:
        
        # expects STAR 00,01,10 as last comment in comment field of read name
        if is_mapped(read.comment, mate):
            continue
        
        # if single nucleotide is > 60% or < 10% drop read
        # or if N content is > 10%
        # or single nt repeated > 20 in a row
        if not check_nucleotide_content(read.sequence):
            continue
        
        # drop low quality reads
        if not check_qual(read.quality):
            continue
        
        # drop reads with high levels of repeats
        if not check_repeats(read.sequence, test_patterns):
            continue

        else:
            gzip_fh_out.write(str(read) + "\n")

def main():
    
    parser = argparse.ArgumentParser(description="""
    Filter fastq (optionally gzipped) to remove possible
    sequencing artifacts. Used for prefiltering reads
    for detecting hyperedited RNA-Seq reads.
    See: 
    Porath et al. A genome-wide map of hyper-edited RNA reveals numerous
    new sites. Nature Communications. 2014. doi:10.1038/ncomms5726
    """)

    parser.add_argument('-i',
                          '--fastq',
                          help ='fastq to filter',
                       required = True)
    parser.add_argument('-m',
                          '--mate',
                          help ='R1 or R2',
                       required = True)
    parser.add_argument('-r',
                          '--repeats',
                          help ='repeats_file',
                       required = True)
    parser.add_argument('-o',
                          '--output',
                          help ='output filename for gzipped fastq',
                       required = True)

    args=parser.parse_args()
    
    gzopen = lambda f: gzip.open(f, 'rt') if utils.is_gz_file(f) else open(f)
    gzwrite = lambda f: gzip.open(f, 'wt') if f.endswith(".gz") else open(f, 'w')    
    
    fq = pysam.FastxFile(args.fastq)
    rpts = gzopen(args.repeats)
    outf = gzwrite(args.output)
    mate = args.mate

    process_fastq(fq, rpts, mate, outf)
    
    outf.close()
    rpts.close()
    fq.close()

if __name__ == '__main__': main()


