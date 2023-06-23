# make some test data, using data from hypoxia vs normoxia RNA-seq
# libraries (from Schweppe lab)

grep "HNRNPH1" /beevol/home/riemondy/Projects/schweppe/nuclear-fak/dbases/gencode.v26.chicken.fak.gtf > hnrnph1.gtf

# chr5:179614179-179624669 is the HNRNRPH1 gene
coords="chr5:179614179-179624669"
bam_dir=/beevol/home/riemondy/Projects/schweppe/nuclear-fak/data/merged/star

samples=(
19_BCPAP_Parental_Hypoxia_12_hours_Rep_1
25_BCPAP_Parental_Hypoxia_12_hours_Rep_2
31_BCPAP_Parental_Hypoxia_12_hours_Rep_3
1_BCPAP_Parental_Normoxia_12_hours_Rep_1
7_BCPAP_Parental_Normoxia_12_hours_Rep_2
13_BCPAP_Parental_Normoxia_12_hours_Rep_3
)

ids=(
hypo_1
hypo_2
hypo_3
norm_1
norm_2
norm_3
)

for i in "${!samples[@]}"; do 
    smpl=${samples[i]}
    id=${ids[i]}
    echo $smpl $id
    samtools view -b $bam_dir"/"$smpl"/"$smpl"_sorted.bam" $coords > $id".bam"
done


