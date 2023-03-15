# hyperediting detection pipeline

Hyperedited reads are often overlooked because they fail to align
to the genome. 

This snakemake pipeline will take unaligned reads in fastq format 
and identify hyperedited reads.

The required dependencies can be installed as a conda environment
which will be named `hyperediting`. Alternatively, the required dependencies
can be installed manually and should work as long as they are found in 
the PATH. 

```bash
conda env create -f environment.yaml
```

```bash
conda activate hyperediting
```

Edit the config/config_hyperediting.yaml and test the snakemake pipeline

```bash
snakemake -npr 
```

Modify the snakecharmer.sh script depending on your cluster/local setup
and run the pipeline

```bash
# on a lsf cluster
bsub < snakecharmer.sh

# on a local run
bash snakecharmer.sh
```

