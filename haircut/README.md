
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
   process.

   To process a fastq named `JH134_2_S49_L004_R1_001.fastq.gz`,
   supply the prefix preceeding the `_S[0-9]+_L00[0-9]_R[12]_001.fastq.gz`

   i.e. 
   
   ```
   HAIRCUT_SAMPLES:
     - JH134_2
   ```

5. `SRC`: This is the `src` directory that contains the C++ scripts in
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

If successful you should generated the following files (assuming your
sample was named `JH134_2`).

```bash
$ tree  
.
|-- bam  
|   |-- JH134_2_haircut.bam  <- aligned
|   |-- JH134_2_haircut_tagged.bam <- Tags added: XT (CHROM_POS), BO (UMI), and CB (BARCODE)
|   |-- JH134_2_haircut_tagged.bam.bai 
|   |-- JH134_2_haircut_umitagged_sorted.bam <- BX (Corrected UMI), UG (UMI group), sorted by CB
|-- counts
|   |-- JH134_2_mtx   <- sparseMatrix formatted output for all barcodes
|   |   |-- barcodes.tsv
|   |   |-- features.tsv
|   |   `-- matrix.mtx.gz
|   `-- JH134_2_umitools_counts.tsv.gz <- UMItools flatfile output 
|-- fastqs
|   |-- JH134_2_R1_umi.fastq.gz <- umi moved to read id
|   |-- JH134_2_R2_umi.fastq.gz
|   `-- JH134_2_R2_umi_trimmed.fastq.gz <- trimmed fastq 
`-- molecules
    `-- JH134_2
        |-- barcodes.tsv 
        |-- features.tsv
        `-- molecules.tsv.gz <- large flat file with per molecule information

6 directories, 14 files
```



