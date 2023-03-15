#!/usr/bin/env bash
#BSUB -J hyperedits
#BSUB -o logs/snakemake_%J.out
#BSUB -e logs/snakemake_%J.err
#BSUB -R "select[mem>8] rusage[mem=8] "
mkdir -p logs

set -o nounset -o pipefail -o errexit -x

args='
  -q rna
  -o {log[0]}
  -e {log[1]}
  -J {params.job_name}
  -R " {params.memory} span[hosts=1]  "
  -n {threads}
  '
snakemake --drmaa "$args" \
    --snakefile workflow/Snakefile \
    --jobs 200 \
    --printshellcmds \
    --resources all_threads=100 \
    --latency-wait 50 \
    --rerun-incomplete \
    --keep-going \
    --scheduler greedy \
    --configfile config/config_hyperediting.yaml 
