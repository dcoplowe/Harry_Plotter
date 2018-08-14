# fgd1=/data/t2k/coplowe/numuCC1P1PiOutput/march17/neutP6BWA/FGD1/nall_data_FGD1_type1_wome_fboff_minaclev0_310517/allTree.root
fgd1=/data/t2k/coplowe/numuCC1P1PiOutput/march17/neutP6BWA/FGD1/nall_data_FGD1_type1_wome_fboff_minaclev0_100817/allTree.root
fgd2=/data/t2k/coplowe/numuCC1P1PiOutput/march17/neutP6BWA/FGD2/nall_data_FGD2_type1_wome_fboff_minaclev0_090818/allTree.root

jno=0
for opt in 0 1 4 6 13 28; do
	for fgd in $fgd1 $fgd2; do
		jno=$(expr $jno + 1)
		jobfile=job${jno}.sh
		cat > $jobfile <<EOF
./MakeT2KPlots.sh $fgd $opt
EOF
		chmod +x $jobfile
		qsub -l cput=02:00:00 -N $jobfile ./$jobfile
		qsubresult=$?
		while ! [ $qsubresult  -eq 0 ];do
			sleep 4
			qsub -l cput=02:00:00 -N $jobfile  ./$jobfile
			qsubresult=$?
		done
	done 
done
echo "Done"