# rMATS Turbo Snakemake Pipeline

A snakemake pipeline for mutivariate analysis of transcript splicing using the rMATS Turbo version for increased speed and improved memory footprint.  

[rMATS](https://rnaseq-mats.sourceforge.net/) | [rMATS-turbo](https://github.com/Xinglab/rmats-turbo)

Intended for running on a HPC using a scheduler like LSF.  

Uses a prebuilt Docker container configured to work with Singularity.  
[RBI Dockerhub rMATS Turbo Image](https://hub.docker.com/repository/docker/rnabioco/rmats-turbo/)

## Usage

### Requirements
- Snakemake >= 7.18.2
- Singularity >= 3.9.2
- 
Ensure that the [Snakemake](https://snakemake.readthedocs.io/en/stable/) workflow manager is installed. This can be done a number of ways:
`conda install snakemake -c bioconda`
or
`pip install snakemake`

Clone or copy this folder from the repository.  

Edit the `config.yaml` file with the appropriate sample, path, and argument configurations.  

Run the `run_pipeline.sh` script to launch the workflow.
`bsub < run_pipeline.sh`

### Inputs

- BAM or FASTQ files
- GTF annotation file
- List of samples and comparisons

### Outputs

- Pairwise comparison splicing results as: `*_splicing_results.xlsx`
- Comparison folders containing raw rMATS outputs and splicing event information.

## Acknowledgements

RNA Biosciences Initiative, Anschutz Medical Campus  
https://medschool.cuanschutz.edu/rbi  

@rnabioco
https://github.com/rnabioco
