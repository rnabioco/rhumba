
# Snakemake pipeline to count functional data

To run this pipeline on new data edit `config.yaml` to specify the following important parameters:

1. `HAIRCUT_DATA`: This is the directory that contains the raw fastq data
   
   i.e. 
   ```
   HAIRCUT_DATA: "raw_data/haircut"
   ```

2. `DATA`: This is the directory that the output results will be placed. 
   
   i.e.  
   ```
   DATA:  "2018-12-26"
   ```

3. `HAIRCUT_FASTA`: This is the path to a fasta file that contains the
   hairpin sequences. 


   i.e.  
   ```
   HAIRCUT_FASTA:  "genome/hairpin.fa"
   ```

4. `HAIRCUT_SAMPLES`: This is the prefix of the fastqs that you want to
   process. The order of these sample prefixes should match those given
   for `10X_SAMPLES`. 

   To process a fastq named `JH134_2_S49_L004_R1_001.fastq.gz`,
   supply the prefix preceeding the `_S[0-9]+_L00[0-9]_R[12]_001.fastq.gz`

   i.e. 
   
   ```
   HAIRCUT_SAMPLES:
     - JH134_2
   ```

6. `10X_SAMPLES`: The prefixes of the fastqs that you want to process. The  
   ordering of these samples is very important, as the order determines
   which `HAIRCUT_SAMPLE` to match to the `10X_SAMPLE`. 

   i.e.  
   ```
   HAIRCUT_SAMPLES:
     - JH134_2
   10X_SAMPLES:
     - JH133
   ```
   
   The haircut sample `JH134_2` will be assumed to be matched to the
   `JH133` mrna sample. Cell barcodes from `JH133` will be used to
   generate the `filtered` matrices produced by the haircut pipeline. 
   
7. `10X_DATA`: Path to the location of the fastqs from the mRNA sequencing
   to be processed by the `10x Genomics` cellranger `count` pipeline.

   i.e. 

   ```
   10X_DATA: "raw_data/mrna"
   ```

8. `10X_REF`: Path to the 10x reference directory to be used by cellranger
   `count` to process the mrna data.

   i.e. 

   ```
   10X_REF: "~Projects/shared_dbases/cell_ranger/refdata-cellranger-GRCh38-3.0.0"
   ```

9. `MAX_10X_JOBS`: Maximum number of jobs to submit for each cellranger
   count command execution.  

   i.e. 
   ```
   MAX_10X_JOBS: 18 
   ```

10. `SRC`: This is the `src` directory that contains the C++ scripts in
   the `rhumba/haircut/src` directory. 
   
   i.e.
   ```
   SRC: "../src"
   ```

The `C++` code needs to be compiled. Execute the following to generate
the executables `tag_bam` and `get_molecule_info`:

```bash
cd src  
make  
```

On Macos it may be necessary to specify `clang` as the compiler.

```bash
make CC=clang CXX=clang++
```

Lastly, the `snakecharmer.sh` script is a BSUB submission script that initiates the
snakemake executable. Edit the `.sh` script as necessary to ensure that
the listed programs are in the path.

To see what jobs snakemake will execute:

```bash
cd rhumba/haircut  
snakemake -npr  
```

To run the pipeline:

```bash
bsub < snakecharmer.sh
```


# Output files

If successful you should generate the following files (assuming your
haircut sample was named `JH134_2` and match mrna sample was `JH133`).

```
$ tree
.
|-- 10x_mrna
|   |-- JH133 <- per sample 10x count output
|   |   |-- JH133.mri.tgz
|   |   |-- SC_RNA_COUNTER_CS
|   |   |   |-- CLOUPE_PREPROCESS
|   |   |   |-- EXPAND_SAMPLE_DEF
|   |   |   |-- SC_RNA_COUNTER
|   |   |   `-- fork0
|   |   |-- _cmdline
|   |   |-- _filelist
|   |   |-- _finalstate
|   |   |-- _invocation
|   |   |-- _jobmode
|   |   |-- _log
|   |   |-- _mrosource
|   |   |-- _perf
|   |   |-- _sitecheck
|   |   |-- _tags
|   |   |-- _timestamp
|   |   |-- _uuid
|   |   |-- _vdrkill
|   |   |-- _versions
|   |   `-- outs
|   |       |-- analysis
|   |       |-- cloupe.cloupe
|   |       |-- filtered_feature_bc_matrix
|   |       |-- filtered_feature_bc_matrix.h5
|   |       |-- metrics_summary.csv
|   |       |-- molecule_info.h5
|   |       |-- possorted_genome_bam.bam
|   |       |-- possorted_genome_bam.bam.bai
|   |       |-- raw_feature_bc_matrix
|   |       |-- raw_feature_bc_matrix.h5
|   |       `-- web_summary.html
|   `-- logs
|       `-- JH133_complete.txt  <- Log file tracked by snakemake per sample
|-- bam  
|   |-- JH134_2_haircut.bam  <- aligned
|   |-- JH134_2_haircut_tagged.bam <- Tags added: XT (CHROM_POS), BO (UMI), and CB (BARCODE)
|   |-- JH134_2_haircut_tagged.bam.bai 
|   |-- JH134_2_haircut_umitagged_sorted.bam <- BX (Corrected UMI), UG (UMI group), sorted by CB
|-- counts
|   `-- JH134_2
|       |-- filtered  <- sparseMatrix formatted output for cell containing barcodes
|       |   |-- barcodes.tsv.gz
|       |   |-- features.tsv.gz
|       |   `-- matrix.mtx.gz
|       |-- umitools_counts.tsv.gz <- umi_tools flatfile produced by count command
|       `-- unfiltered <- sparseMatrix formatted output for all barcodes
|           |-- barcodes.tsv.gz
|           |-- features.tsv.gz
|           `-- matrix.mtx.gz
|-- fastqs
|   |-- JH134_2_R1_umi.fastq.gz <- umi moved to read id
|   |-- JH134_2_R2_umi.fastq.gz
|   `-- JH134_2_R2_umi_trimmed.fastq.gz <- trimmed fastq 
`-- molecules
    `-- JH134_2
        |-- barcodes.tsv 
        |-- features.tsv
        `-- molecules.tsv.gz <- large flat file with per molecule information
20 directories, 41 files
```


## Structure of the molecules output

The `molecules` directory contains a series of files that 
provide per UMI summaries for all UMIs found in every barcode. These
files are analagous to the `molecule_info.h5` produced by cellranger 
count, but are simplified.
 
These files can be read and processed in `R` using the scrunchy 
package. See `scrunchy::read_molecules()`

The `molecules.tsv.gz` file contains 4 columns. 

barcode_idx: one-based index of the barcode in the `barcodes.tsv.gz` file  

feature_idx: one-based index of the feature in the `features.tsv.gz` file  

UMI_2bit_sequence: inferred UMI sequence encoded in 2bit format as a uint64 integer  
 
read_count: number of reads associated with each `UMI`  

