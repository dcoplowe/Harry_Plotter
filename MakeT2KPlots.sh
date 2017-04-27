source ~/.bashrc

module load root

plotter=/data/t2k/coplowe/software/Harry_Plotter/

PATH=${PATH}:${plotter}

infile=$1

echo $PATH

dir=$(dirname ${infile})

cd ${dir}
pwd

echo "Starting to produce plots."
ProducePlots -t -i ${infile}
echo "Finished making plots."