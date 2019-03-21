
# Snakemake pipeline to count functional data

To run this pipeline on new data edit `config.yaml` to specify the following important parameters:

1. `RAW_DATA`: This is the directory that contains the raw fastq data
   
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
     - JH141_PBMC1
   ```

5. `10X_WHITELIST`:
   Path to the whitelist of 10x genomics cell barcodes. These are used to
   filter for cellbarcodes that match the known barcodes.

   i.e.
   ```
   10X_WHITELIST:
     - "/cluster/software/modules-sw/cellranger/3.0.2/cellranger-cs/3.0.2/lib/python/cellranger/barcodes/737K-august-2016.txt"
   ```

6. `CELL_BARCODES`:
   A list of `barcodes.tsv.gz` files that supply the cell barcodes inferred from `cellranger count` 
   run on mRNA data. The ordering of these samples is very important, as the order determines
   which `HAIRCUT_SAMPLE` to match to the `CELL_BARCODES`. 

   i.e.  
   ```
   HAIRCUT_SAMPLES:
     - JH134_2

   CELL_BARCODES:
     - "2019-03-20_mrna/JH133/outs/filtered_feature_bc_matrix/barcodes.tsv.gz"
   ```
   
   The haircut sample `JH134_2` will be assumed to be matched to the
   `JH133` mrna sample. Cell barcodes from `JH133` will be used to
   generate the `filtered` matrices produced by the haircut pipeline. 
   

7. `SRC`: This is the `src` directory that contains the C++ scripts in
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
-- bam  
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

