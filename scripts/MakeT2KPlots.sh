source ~/.bashrc

module load root
plotter=/data/t2k/coplowe/software/Harry_Plotter/

PATH=${PATH}:${plotter}

fgd1=/data/t2k/coplowe/numuCC1P1PiOutput/march17/neutP6BWA/FGD1/nall_data_FGD1_type1_wome_fboff_minaclev0_310517/allTree.root
fgd2=/data/t2k/coplowe/numuCC1P1PiOutput/march17/neutP6BWA/FGD2/nall_data_FGD2_type1_wome_fboff_minaclev0_090818/allTree.root

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
cd ${dir}
pwd

echo "Starting to produce plots."
ProducePlots -t -i ${infile} -o ${dir}/${savename} ${opt} ${cutson} -b 1
echo "Finished making plots."
