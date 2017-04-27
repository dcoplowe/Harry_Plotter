source ~/.bashrc

module load root

plotter=/data/t2k/coplowe/software/Harry_Plotter/

PATH=${PATH}:${plotter}

infile=$1

echo $PATH

dir=$(dirname ${infile})

cd ${dir}
pwd

ProducePlots -t -i ${infile}