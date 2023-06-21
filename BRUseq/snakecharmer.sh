#!/usr/bin/env bash

#BSUB -J BRUseq 
#BSUB -o snakemake_%J.out
#BSUB -e snakemake_%J.err
#BSUB -R "select[mem>4] rusage[mem=4] " 
#BSUB -q rna
set -o nounset -o pipefail -o errexit -x

args=' 
  -q rna 
  -o {log}.out 
  -e {log}.err 
  -J {params.job_name} 
  -R "{params.memory} span[hosts=1] " 
  -n {threads} ' 
    
#### load necessary programs ####

# If programs are not all in the path then modify code to load 
# the necessary programs

# load modules
. /usr/share/Modules/init/bash
module load modules modules-init modules-python

module load fastqc
module load samtools
module load STAR
module load subread 
module load python/3.8.5


#### execute snakemake ####

snakemake --drmaa "$args" \
    --snakefile Snakefile \
    --jobs 72 \
    --resources all_threads=72 \
    --latency-wait 50 \
    --rerun-incomplete  \
    --configfile config.yaml \
    --printshellcmds
