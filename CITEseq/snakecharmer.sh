#! /usr/bin/env bash

#BSUB -J cellranger
#BSUB -o logs/cellranger_%J.out
#BSUB -e logs/cellranger_%J.err
#BSUB -R "select[mem>4] rusage[mem=4]" 
#BSUB -q rna

set -o nounset -o pipefail -o errexit -x

mkdir -p logs

run_snakemake() {
    local config_file=$1
    
    drmaa_args='
        -o {log}.out 
        -e {log}.err 
        -J {params.job_name} 
        -R "{params.memory} span[hosts=1] " 
        -n {threads} '

    snakemake \
        --snakefile Snakefile \
        --drmaa "$drmaa_args" \
        --jobs 24 \
        --latency-wait 60 \
        --rerun-incomplete \
        --configfile $config_file
}

run_snakemake config.yaml


