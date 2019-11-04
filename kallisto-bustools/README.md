
# Snakemake pipeline to run kallisto-bustools on 10X Genomics 3' data

This directory contains a snakemake pipeine to execute the kallisto-bustools
pipeline on 10X Genomics v3 mRNA and ADT/HTO data. 

The pipeline expects both the `kallisto` and `bustools` binaries to be in your `PATH`.

To run this pipeline on new data edit `config.yaml` to specify the following important parameters:

1. `DATA`: This is the directory that the output results will be placed
   into. Adirectory called `raw_data`  is required in this directory.
   Subdirectories called `gex`, `adt`, and `hto` must contain the gene expression,
   ADT and HTO FASTQs, respectively (as appropriate).

2. `GEX_INDEX`, `ADT_INDEX`, and `HTO_INDEX`: Directories containing the kallisto
   indices for gene expression, ADT and HTO libraries. Each directory must contain
   `index.idx` and `index.t2g` files.
   
3. `VELOCITY_INDEX`: [CURRENTLY BROKEN]

4. `BARCODES`: File containing a whitelist of cell barcodes.

5. `CHEMISTRY`: A Kallisto library type (i.e. `10xv3`).

6. `GEX`, `ADT`, `HTO`: Sample base-names for the gene expression, ADT, and HTO
   libraries, respectively.

Lastly, the `snakecharmer.sh` script is a BSUB submission script that initiates the
snakemake executable.
