#! /usr/bin/env bash

#BSUB -J spaceranger
#BSUB -o logs/spaceranger_%J.out
#BSUB -e logs/spaceranger_%J.err
#BSUB -R "select[mem>4] rusage[mem=4]" 
#BSUB -q rna

set -o nounset -o pipefail -o errexit -x

mkdir -p logs
module load python/3.8.5
run_snakemake() {
    local config_file=$1
    
    drmaa_args='
        -oo {log}.out 
        -eo {log}.err 
        -J {params.job_name} 
        -R "{params.memory} span[hosts=1] " 
        -n {threads} '

    snakemake \
        --snakefile Snakefile \
        --drmaa "$drmaa_args" \
        --jobs 100 \
        --latency-wait 60 \
        --rerun-incomplete \
        --configfile $config_file
}

run_snakemake config.yaml


