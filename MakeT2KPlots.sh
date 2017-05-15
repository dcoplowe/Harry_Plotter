source ~/.bashrc

module load root
plotter=/data/t2k/coplowe/software/Harry_Plotter/

PATH=${PATH}:${plotter}

infile=$1
opt=$2

if [[ ! -z ${opt} ]]; then
	opt="-e ${opt}"
fi

echo $PATH

dir=$(dirname ${infile})

savename="${dir##*/}"_plots_${TODAY}.root

echo Outfile: $savename
cd ${dir}
pwd

echo "Starting to produce plots."
ProducePlots -t -i ${infile} -v -o ${dir}/${savename} ${opt}
echo "Finished making plots."
