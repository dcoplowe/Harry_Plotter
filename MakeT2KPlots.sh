source ~/.bashrc

module load root
plotter=/data/t2k/coplowe/software/Harry_Plotter/

PATH=${PATH}:${plotter}

infile=$1

echo $PATH

dir=$(dirname ${infile})

savename="${dir##*/}"_plots_${TODAY}.root

echo Outfile: $savename
cd ${dir}
pwd

echo "Starting to produce plots."
ProducePlots -t -i ${infile} -v -o ${dir}/${savename}
echo "Finished making plots."
