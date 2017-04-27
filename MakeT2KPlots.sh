source ~/.bashrc

module load root

plotter=/data/t2k/coplowe/software/Harry_Plotter/

PATH=${PATH}:${plotter}

infile=$1

dir=${infile}

dir=$(echo ${dir} | rev | cut -c 13- | rev)

cd ${dir}
pwd

ProducePlots -t -i ${infile}