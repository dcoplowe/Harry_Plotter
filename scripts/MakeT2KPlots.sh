source ~/.bashrc

module load root
plotter=/data/t2k/coplowe/software/Harry_Plotter/

PATH=${PATH}:${plotter}

infile=$1
opt=$2
cutson=$3

if [[ ! -z ${opt} ]]; then
	opt="-e ${opt}"
fi

if [[ ! -z ${cutson} ]]; then
	cutson="-c ${cutson}"
fi

echo $PATH

dir=$(dirname ${infile})

savename="${dir##*/}"_plots_${TODAY}.root

echo Outfile: $savename
# cd ${dir}
pwd

echo "Starting to produce plots."
ProducePlots -t -i ${infile} -o ${savename} ${opt} ${cutson}
# ProducePlots -t -i ${infile} -o ${dir}/${savename} ${opt} ${cutson}
# -b 1
echo "Finished making plots."
