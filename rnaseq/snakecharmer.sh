#!/usr/bin/env bash

#BSUB -J RNAseek 
#BSUB -o logs/snakemake_%J.out
#BSUB -e logs/snakemake_%J.err
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

#module load ucsc/v308 
module load fastqc/0.11.7
module load samtools/1.5
module load STAR/2.5.2a
module load subread 

# other programs (not in modules)
# Salmon-0.11.2

#### execute snakemake ####

snakemake --drmaa "$args" \
    --snakefile Snakefile \
    --jobs 72 \
    --resources all_threads=72 \
    --latency-wait 50 \
    --rerun-incomplete  \
    --configfile config_files/config.yaml 
