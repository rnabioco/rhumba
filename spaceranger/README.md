
# Snakemake pipeline to run 10x Genomics spaceranger for spatial transcriptomics from the Visium platform

Pipeline originally created/modified by Rui Fu

This directory contains a snakemake pipeine to execute the 10x genomics
spaceranger count and aggregate pipelines on multiple samples. 

The spaceranger-1.3.1 software is already available on `bodhi`. 
It is located at the path: `~/bin/spaceranger-1.3.1/spaceranger` and should be accessible by this script with no modifications.

To run this pipeline on new data edit `config.yaml` to specify the following important parameters:

1. `DATA`: This directory that contains the raw fastq data. 
   
2. `RESULTS`: This is the path to the directory where the spaceranger `count` and `aggr` 10x executables will generate and store the outputs.

3. `SAMPLES`: Sample names for gene expression fastqs.
   The genomics core often outputs 4 fastqs for each sample.
   This snakemake pipeline will match the common prefix from the
   samplename and pass all of the fastqs to spaceranger count. Check the
   log (stderr) from the spaceranger count commands to make sure that the
   right fastqs are passed to spaceranger.  

   i.e. 
   ```
   SAMPLES:
     - sample_1 
     - sample_2
   ```
   
   The raw fastqs will likely be named:
   ```
   sample_1_1_S1_L001_R1_001.fastq.gz  
   sample_1_2_S2_L001_R1_001.fastq.gz  
   sample_1_3_S3_L001_R1_001.fastq.gz
   sample_1_4_S4_L001_R1_001.fastq.gz
   sample_2_1_S1_L001_R1_001.fastq.gz
   sample_2_2_S2_L001_R1_001.fastq.gz
   sample_2_3_S3_L001_R1_001.fastq.gz
   sample_2_4_S4_L001_R1_001.fastq.gz
   ```

4. `IMAGES`: Images to use for each sample. These are typically acquired separately from the raw sequencing data, either in a separate email from the core or the investigator. Filenames typically end in `.TIF` and the files should be located in the `DATA` path folder with the raw data.  
   
5. `GROUPS`: Sample groups that should be used with spaceranger aggr. Include the gene expression names separated by commas for samples that should be aggregated together. Include multiple entries if multiple groups of samples are being aggregated. Leave blank to skip this step.  

6. `SLIDE`: Visium slide serial number for each sample. If a single value is provided, this serial number will be used for all samples.  
   
7. `AREA`: Visium capture area identifier for each sample. Options include A1, B1, C1, or D1.
   
8.  `GENOME`: This should point to the directory containing the 10x
   genomics formatted references. 

9.  `MAX_JOBS`: This argument is passed to ` --maxjobs` argument for
   `count` and `aggr`. Note that this is the maximum number of jobs
   submitted not maximum number of cores used. 

10. `LSF_TEMPLATE`: This argument specifies the default job submission
   template that cellranger will use to submit jobs to the queue. Edit
   this file to remove the `-q rna` BSUB argument if you are not on the
  `rna` queue.  
   
Lastly, the `snakecharmer.sh` script is a BSUB submission script that initiates the
snakemake executable.

To see what jobs snakemake will execute:

```bash
cd pipelines/10x_3p_pipeline
snakemake -npr
```

To run the pipeline:

```bash
bsub < snakecharmer.sh
```

snakemake will execute the `spaceranger count` command on each sample. These jobs will be named count. To see the progress of the count runs use `bpeek`

```bash
# show current running jobs
bjobs

# show stderr and stdout for most recent job
bpeek

#show stderr and stdout for count job (using job id listed by bjobs)
bpeek [JOBID]
```

When each count command is completed a file named `SAMPLE_complete.txt` will be generated in the 'results/SAMPLE/logs/` directory. If you would like to rerun the count commands, remove this file and reexecute the pipeline.


