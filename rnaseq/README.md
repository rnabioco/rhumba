# RNA-seq pipeline

Standard RNA-Seq pipeline to quantify transcript level expression with
`Salmon` and gene-level counting with `STAR` + `featureCounts`

Edit `config.yaml` to specify relevant directories and parameters for 
various rules. 

Fastqs are expected to be in directory named `fastq/raw_data` that is 
a subdirectory of the path specified in DATA in the config file. 

`snakecharmer.sh` is an example bsub script to run the pipeline
