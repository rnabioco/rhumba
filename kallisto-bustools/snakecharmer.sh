#!/usr/bin/env bash
#BSUB -J kb10x 
#BSUB -o logs/kb10x_%J.out
#BSUB -e logs/kb10x_%J.err
#BSUB -R "select[mem>4] rusage[mem=4] " 
#BSUB -q rna

set -o nounset -o pipefail -o errexit -x

mkdir -p logs

args=' -o {log}.out -e {log}.err -q rna -J {params.job_name} -R "{params.memory} span[hosts=1] " -n {threads}  '

snakemake \
  --drmaa "$args" \
  --snakefile Snakefile \
  --jobs 100 \
  --latency-wait 50 \
  --rerun-incomplete  \
  --configfile config.yaml 
