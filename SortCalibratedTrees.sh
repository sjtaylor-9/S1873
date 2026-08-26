#!/bin/bash

ANALDIR=./CalibratedTrees
HISTDIR=./HistFiles
DLEN=${#ANALDIR}
CFILE_2019_BEFORE=./Calibrations/CalibFileSetup/MyCalibrations/Dec2019/Before/CalibrationFile_Dec19_Before.cal
CFILE_2019_AFTER=./Calibrations/CalibFileSetup/MyCalibrations/Dec2019/After/CalibrationFile_Dec19_After.cal
CFILE_2020_BEFORE=./Calibrations/CalibFileSetup/MyCalibrations/Nov2020/Before/CalibrationFile_2020_Before.cal
CFILE_2020_20V_AFTER=./Calibrations/CalibFileSetup/MyCalibrations/Nov2020/After/20V/CalibrationFile_2020_20V.cal
CFILE_2020_40V_AFTER=./Calibrations/CalibFileSetup/MyCalibrations/Nov2020/After/40V/CalibrationFile_2020_40V.cal


HPGeCALRUNS=(52018 52020 52022 52169 52170 52171 52172 52777 52779 52781 52932 52933 52934 52937)
n=52753 # First November 2020 run was the triple alpha calibration run, 52754 

shopt -s globstar nullglob

if [ $# -eq 0 ]; then # If no positional arguments are passed to the script then the script runs through all MIDAS files
for f in $ANALDIR/**/*.root; do
 g=$(basename "$f")
 
 [[ $g != analysis* ]] && continue # skip fragment trees
 j="${g#analysis}"   # 52016_000.root
 j="${j%.root}"      # 52016_000
 i="${j%%_*}"        # 52016
 h="$j"              # 52016_000
 
 if [[ " ${HPGeCALRUNS[*]} " =~ " $i " ]]; then
	SORTCODE=./Calibrations/GammaCalibration/GammaSort
	CUTFILE="NULL"
	TARGET="NULL"
 else
	SORTCODE=./Sam_Sort/SortData_Sam
	CUTFILE=./Sam_Sort/cuts.root
	TARGET=one # one, two, three
 fi

 if [[ $f == *WithBeforeCal* ]]; then 
	HFILE_BEFORE=$HISTDIR/WithBeforeCal/Hist_${h}.root
	if [[ ! -f $HFILE_BEFORE ]]; then
		if [ ! -f $CUTFILE ]; then
			
			if [ $i -lt $n ]; then  # If the run number is less than 52753, then it is part of the 2019 data set
				echo "$SORTCODE $f $CFILE_2019_BEFORE $HFILE_BEFORE $TARGET"
				$SORTCODE $f $CFILE_2019_BEFORE $HFILE_BEFORE $TARGET
			else # If the run number is greater than 52753, then it is part of the 2020 data set
				echo "$SORTCODE $f $CFILE_2020_BEFORE $HFILE_BEFORE $TARGET"
				$SORTCODE $f $CFILE_2020_BEFORE $HFILE_BEFORE $TARGET
			fi # end of 2020 or 2019 determination
	
		else # If cut file does exist
			
			if [[ $i -lt $n ]]; then  # If the run number is less than 52753, then it is part of the 2019 data set
				echo "$SORTCODE $f $CFILE_2019_BEFORE $HFILE_BEFORE $TARGET $CUTFILE"
				$SORTCODE $f $CFILE_2019_BEFORE $HFILE_BEFORE $TARGET $CUTFILE
			else # If the run number is greater than 52753, then it is part of the 2020 data set
				echo "$SORTCODE $f $CFILE_2020_BEFORE $HFILE_BEFORE $TARGET $CUTFILE"
				$SORTCODE $f $CFILE_2020_BEFORE $HFILE_BEFORE $TARGET $CUTFILE
			fi # end of 2020 or 2019 determination

		fi # end of if cut file exists
	fi # end of if histogram file exists

	if [[ -f $HFILE_BEFORE ]]; then
		if [ $HFILE_BEFORE -ot $SORTCODE ] || [ $HFILE_BEFORE -ot $f ]; then
			if [ ! -f $CUTFILE ]; then
				
				if [[ $i -lt $n ]]; then  # If the run number is less than 52753, then it is part of the 2019 data set
					echo "$SORTCODE $f $CFILE_2019_BEFORE $HFILE_BEFORE $TARGET"
					$SORTCODE $f $CFILE_2019_BEFORE $HFILE_BEFORE $TARGET
				else # If the run number is greater than 52753, then it is part of the 2020 data set
					echo "$SORTCODE $f $CFILE_2020_BEFORE $HFILE_BEFORE $TARGET"
					$SORTCODE $f $CFILE_2020_BEFORE $HFILE_BEFORE $TARGET
				fi # end of 2020 or 2019 determination

			else # If cut file does exist

				if [[ $i -lt $n ]]; then  # If the run number is less than 52753, then it is part of the 2019 data set
					echo "$SORTCODE $f $CFILE_2019_BEFORE $HFILE_BEFORE $TARGET $CUTFILE"
					$SORTCODE $f $CFILE_2019_BEFORE $HFILE_BEFORE $TARGET $CUTFILE
				else # If the run number is greater than 52753, then it is part of the 2020 data set
					echo "$SORTCODE $f $CFILE_2020_BEFORE $HFILE_BEFORE $TARGET $CUTFILE"
					$SORTCODE $f $CFILE_2020_BEFORE $HFILE_BEFORE $TARGET $CUTFILE
				fi # end of 2020 or 2019 determination

			fi # end of if cut file exists
		fi # end of checking age of sort code and analysis tree
	fi # end of if histogram file exists
 
 elif [[ $f == *WithAfterCal* ]]; then 
 	HFILE_AFTER=$HISTDIR/WithAfterCal/Hist_${h}.root
	if [[ ! -f $HFILE_AFTER ]]; then
		if [ ! -f $CUTFILE ]; then
			
			if [ $i -lt $n ]; then  # If the run number is less than 52753, then it is part of the 2019 data set
				echo "$SORTCODE $f $CFILE_2019_AFTER $HFILE_AFTER $TARGET"
				$SORTCODE $f $CFILE_2019_AFTER $HFILE_AFTER $TARGET
			else # If the run number is greater than 52753, then it is part of the 2020 data set
				if [[ $i == "52930" ]]; then # The S3 bias was reduced from 40V to 20V after run 52885. Run 52930 was the after 3-alpha calibration run taking at 20V. The HPGe calibration runs are indepedent of S3 bias; however, to be consisitent the 40V calibration should be used. 
					echo "$SORTCODE $f $CFILE_2020_20V_AFTER $HFILE_AFTER $TARGET"
					$SORTCODE $f $CFILE_2020_20V_AFTER $HFILE_AFTER $TARGET
				else
					echo "$SORTCODE $f $CFILE_2020_40V_AFTER $HFILE_AFTER $TARGET"
					$SORTCODE $f $CFILE_2020_40V_AFTER $HFILE_AFTER $TARGET

				fi # end of S3 bias determination
			fi # end of 2020 or 2019 determination
	
		else # If cut file does exist
			
			if [[ $i -lt $n ]]; then  # If the run number is less than 52753, then it is part of the 2019 data set
				echo "$SORTCODE $f $CFILE_2019_AFTER $HFILE_AFTER $TARGET $CUTFILE"
				$SORTCODE $f $CFILE_2019_AFTER $HFILE_AFTER $TARGET $CUTFILE
			else # If the run number is greater than 52753, then it is part of the 2020 data set
				if [[ $i == "52930" ]]; then # The S3 bias was reduced from 40V to 20V after run 52885. Run 52930 was the after 3-alpha calibration run taking at 20V. The HPGe calibration runs are indepedent of S3 bias; however, to be consisitent the 40V calibration should be used. 
					echo "$SORTCODE $f $CFILE_2020_20V_AFTER $HFILE_AFTER $TARGET $CUTFILE"
					$SORTCODE $f $CFILE_2020_20V_AFTER $HFILE_AFTER $TARGET $CUTFILE
				else
					echo "$SORTCODE $f $CFILE_2020_40V_AFTER $HFILE_AFTER $TARGET $CUTFILE"
					$SORTCODE $f $CFILE_2020_40V_AFTER $HFILE_AFTER $TARGET $CUTFILE

				fi # end of S3 bias determination
			fi # end of 2020 or 2019 determination

		fi # end of if cut file exists
	fi # end of if histogram file exists

	if [[ -f $HFILE_AFTER ]]; then
		if [ $HFILE_AFTER -ot $SORTCODE ] || [ $HFILE_AFTER -ot $f ]; then
			if [ ! -f $CUTFILE ]; then
				
				if [[ $i -lt $n ]]; then  # If the run number is less than 52753, then it is part of the 2019 data set
					echo "$SORTCODE $f $CFILE_2019_AFTER $HFILE_AFTER $TARGET"
					$SORTCODE $f $CFILE_2019_AFTER $HFILE_AFTER $TARGET
				else # If the run number is greater than 52753, then it is part of the 2020 data set
					if [[ $i == "52930" ]]; then # The S3 bias was reduced from 40V to 20V after run 52885. Run 52930 was the after 3-alpha calibration run taking at 20V. The HPGe calibration runs are indepedent of S3 bias; however, to be consisitent the 40V calibration should be used. 
						echo "$SORTCODE $f $CFILE_2020_20V_AFTER $HFILE_AFTER $TARGET"
						$SORTCODE $f $CFILE_2020_20V_AFTER $HFILE_AFTER $TARGET
					else
						echo "$SORTCODE $f $CFILE_2020_40V_AFTER $HFILE_AFTER $TARGET"
						$SORTCODE $f $CFILE_2020_40V_AFTER $HFILE_AFTER $TARGET

					fi # end of S3 bias determination
				fi # end of 2020 or 2019 determination

			else # If cut file does exist

				if [[ $i -lt $n ]]; then  # If the run number is less than 52753, then it is part of the 2019 data set
					echo "$SORTCODE $f $CFILE_2019_AFTER $HFILE_AFTER $TARGET $CUTFILE"
					$SORTCODE $f $CFILE_2019_AFTER $HFILE_AFTER $TARGET $CUTFILE
				else # If the run number is greater than 52753, then it is part of the 2020 data set
					if [[ $i == "52930" ]]; then # The S3 bias was reduced from 40V to 20V after run 52885. Run 52930 was the after 3-alpha calibration run taking at 20V. The HPGe calibration runs are indepedent of S3 bias; however, to be consisitent the 40V calibration should be used. 
						echo "$SORTCODE $f $CFILE_2020_20V_AFTER $HFILE_AFTER $TARGET $CUTFILE"
						$SORTCODE $f $CFILE_2020_20V_AFTER $HFILE_AFTER $TARGET $CUTFILE
					else
						echo "$SORTCODE $f $CFILE_2020_40V_AFTER $HFILE_AFTER $TARGET $CUTFILE"
						$SORTCODE $f $CFILE_2020_40V_AFTER $HFILE_AFTER $TARGET $CUTFILE

					fi # end of S3 bias determination
				fi # end of 2020 or 2019 determination

			fi # end of if cut file exists
		fi # end of checking age of sort code and analysis tree
	fi # end of if histogram file exists
 fi # end of before or after cal root file determination

done

else # if a positional argument is supplied

for f in $ANALDIR/**/analysis"$@"_*.root; do
 
 echo "$f"
 g=$(basename "$f")
 
 [[ $g != analysis* ]] && continue # skip fragment trees
 j="${g#analysis}"   # 52016_000.root
 j="${j%.root}"      # 52016_000
 i="${j%%_*}"        # 52016
 h="$j"              # 52016_000
 
 if [[ " ${HPGeCALRUNS[*]} " =~ " $i " ]]; then
	SORTCODE=./Calibrations/GammaCalibration/GammaSort
	CUTFILE="NULL"
	TARGET="NULL"
 else
	SORTCODE=./Sam_Sort/SortData_Sam
	CUTFILE=./Sam_Sort/cuts.root
	TARGET=one # one, two, three
 fi

 if [[ $f == *WithBeforeCal* ]]; then 
	HFILE_BEFORE=$HISTDIR/WithBeforeCal/Hist_${h}.root
	if [[ ! -f $HFILE_BEFORE ]]; then
		if [ ! -f $CUTFILE ]; then
			
			if [ $i -lt $n ]; then  # If the run number is less than 52753, then it is part of the 2019 data set
				echo "$SORTCODE $f $CFILE_2019_BEFORE $HFILE_BEFORE $TARGET"
				$SORTCODE $f $CFILE_2019_BEFORE $HFILE_BEFORE $TARGET
			else # If the run number is greater than 52753, then it is part of the 2020 data set
				echo "$SORTCODE $f $CFILE_2020_BEFORE $HFILE_BEFORE $TARGET"
				$SORTCODE $f $CFILE_2020_BEFORE $HFILE_BEFORE $TARGET
			fi # end of 2020 or 2019 determination
	
		else # If cut file does exist
			
			if [[ $i -lt $n ]]; then  # If the run number is less than 52753, then it is part of the 2019 data set
				echo "$SORTCODE $f $CFILE_2019_BEFORE $HFILE_BEFORE $TARGET $CUTFILE"
				$SORTCODE $f $CFILE_2019_BEFORE $HFILE_BEFORE $TARGET $CUTFILE
			else # If the run number is greater than 52753, then it is part of the 2020 data set
				echo "$SORTCODE $f $CFILE_2020_BEFORE $HFILE_BEFORE $TARGET $CUTFILE"
				$SORTCODE $f $CFILE_2020_BEFORE $HFILE_BEFORE $TARGET $CUTFILE
			fi # end of 2020 or 2019 determination

		fi # end of if cut file exists
	fi # end of if histogram file exists

	if [[ -f $HFILE_BEFORE ]]; then
		if [ $HFILE_BEFORE -ot $SORTCODE ] || [ $HFILE_BEFORE -ot $f ]; then
			if [ ! -f $CUTFILE ]; then
				
				if [[ $i -lt $n ]]; then  # If the run number is less than 52753, then it is part of the 2019 data set
					echo "$SORTCODE $f $CFILE_2019_BEFORE $HFILE_BEFORE $TARGET"
					$SORTCODE $f $CFILE_2019_BEFORE $HFILE_BEFORE $TARGET
				else # If the run number is greater than 52753, then it is part of the 2020 data set
					echo "$SORTCODE $f $CFILE_2020_BEFORE $HFILE_BEFORE $TARGET"
					$SORTCODE $f $CFILE_2020_BEFORE $HFILE_BEFORE $TARGET
				fi # end of 2020 or 2019 determination

			else # If cut file does exist

				if [[ $i -lt $n ]]; then  # If the run number is less than 52753, then it is part of the 2019 data set
					echo "$SORTCODE $f $CFILE_2019_BEFORE $HFILE_BEFORE $TARGET $CUTFILE"
					$SORTCODE $f $CFILE_2019_BEFORE $HFILE_BEFORE $TARGET $CUTFILE
				else # If the run number is greater than 52753, then it is part of the 2020 data set
					echo "$SORTCODE $f $CFILE_2020_BEFORE $HFILE_BEFORE $TARGET $CUTFILE"
					$SORTCODE $f $CFILE_2020_BEFORE $HFILE_BEFORE $TARGET $CUTFILE
				fi # end of 2020 or 2019 determination

			fi # end of if cut file exists
		fi # end of checking age of sort code and analysis tree
	fi # end of if histogram file exists
 
 elif [[ $f == *WithAfterCal* ]]; then 
 	HFILE_AFTER=$HISTDIR/WithAfterCal/Hist_${h}.root
	if [[ ! -f $HFILE_AFTER ]]; then
		if [ ! -f $CUTFILE ]; then
			
			if [ $i -lt $n ]; then  # If the run number is less than 52753, then it is part of the 2019 data set
				echo "$SORTCODE $f $CFILE_2019_AFTER $HFILE_AFTER $TARGET"
				$SORTCODE $f $CFILE_2019_AFTER $HFILE_AFTER $TARGET
			else # If the run number is greater than 52753, then it is part of the 2020 data set
				if [[ $i == "52930" ]]; then # The S3 bias was reduced from 40V to 20V after run 52885. Run 52930 was the after 3-alpha calibration run taking at 20V. The HPGe calibration runs are indepedent of S3 bias; however, to be consisitent the 40V calibration should be used. 
					echo "$SORTCODE $f $CFILE_2020_20V_AFTER $HFILE_AFTER $TARGET"
					$SORTCODE $f $CFILE_2020_20V_AFTER $HFILE_AFTER $TARGET
				else
					echo "$SORTCODE $f $CFILE_2020_40V_AFTER $HFILE_AFTER $TARGET"
					$SORTCODE $f $CFILE_2020_40V_AFTER $HFILE_AFTER $TARGET

				fi # end of S3 bias determination
			fi # end of 2020 or 2019 determination
	
		else # If cut file does exist
			
			if [[ $i -lt $n ]]; then  # If the run number is less than 52753, then it is part of the 2019 data set
				echo "$SORTCODE $f $CFILE_2019_AFTER $HFILE_AFTER $TARGET $CUTFILE"
				$SORTCODE $f $CFILE_2019_AFTER $HFILE_AFTER $TARGET $CUTFILE
			else # If the run number is greater than 52753, then it is part of the 2020 data set
				if [[ $i == "52930" ]]; then # The S3 bias was reduced from 40V to 20V after run 52885. Run 52930 was the after 3-alpha calibration run taking at 20V. The HPGe calibration runs are indepedent of S3 bias; however, to be consisitent the 40V calibration should be used. 
					echo "$SORTCODE $f $CFILE_2020_20V_AFTER $HFILE_AFTER $TARGET $CUTFILE"
					$SORTCODE $f $CFILE_2020_20V_AFTER $HFILE_AFTER $TARGET $CUTFILE
				else
					echo "$SORTCODE $f $CFILE_2020_40V_AFTER $HFILE_AFTER $TARGET $CUTFILE"
					$SORTCODE $f $CFILE_2020_40V_AFTER $HFILE_AFTER $TARGET $CUTFILE

				fi # end of S3 bias determination
			fi # end of 2020 or 2019 determination

		fi # end of if cut file exists
	fi # end of if histogram file exists

	if [[ -f $HFILE_AFTER ]]; then
		if [ $HFILE_AFTER -ot $SORTCODE ] || [ $HFILE_AFTER -ot $f ]; then
			if [ ! -f $CUTFILE ]; then
				
				if [[ $i -lt $n ]]; then  # If the run number is less than 52753, then it is part of the 2019 data set
					echo "$SORTCODE $f $CFILE_2019_AFTER $HFILE_AFTER $TARGET"
					$SORTCODE $f $CFILE_2019_AFTER $HFILE_AFTER $TARGET
				else # If the run number is greater than 52753, then it is part of the 2020 data set
					if [[ $i == "52930" ]]; then # The S3 bias was reduced from 40V to 20V after run 52885. Run 52930 was the after 3-alpha calibration run taking at 20V. The HPGe calibration runs are indepedent of S3 bias; however, to be consisitent the 40V calibration should be used. 
						echo "$SORTCODE $f $CFILE_2020_20V_AFTER $HFILE_AFTER $TARGET"
						$SORTCODE $f $CFILE_2020_20V_AFTER $HFILE_AFTER $TARGET
					else
						echo "$SORTCODE $f $CFILE_2020_40V_AFTER $HFILE_AFTER $TARGET"
						$SORTCODE $f $CFILE_2020_40V_AFTER $HFILE_AFTER $TARGET

					fi # end of S3 bias determination
				fi # end of 2020 or 2019 determination

			else # If cut file does exist

				if [[ $i -lt $n ]]; then  # If the run number is less than 52753, then it is part of the 2019 data set
					echo "$SORTCODE $f $CFILE_2019_AFTER $HFILE_AFTER $TARGET $CUTFILE"
					$SORTCODE $f $CFILE_2019_AFTER $HFILE_AFTER $TARGET $CUTFILE
				else # If the run number is greater than 52753, then it is part of the 2020 data set
					if [[ $i == "52930" ]]; then # The S3 bias was reduced from 40V to 20V after run 52885. Run 52930 was the after 3-alpha calibration run taking at 20V. The HPGe calibration runs are indepedent of S3 bias; however, to be consisitent the 40V calibration should be used. 
						echo "$SORTCODE $f $CFILE_2020_20V_AFTER $HFILE_AFTER $TARGET $CUTFILE"
						$SORTCODE $f $CFILE_2020_20V_AFTER $HFILE_AFTER $TARGET $CUTFILE
					else
						echo "$SORTCODE $f $CFILE_2020_40V_AFTER $HFILE_AFTER $TARGET $CUTFILE"
						$SORTCODE $f $CFILE_2020_40V_AFTER $HFILE_AFTER $TARGET $CUTFILE

					fi # end of S3 bias determination
				fi # end of 2020 or 2019 determination

			fi # end of if cut file exists
		fi # end of checking age of sort code and analysis tree
	fi # end of if histogram file exists
 fi # end of before or after cal root file determination
done
fi # end of checking if positional arguments are supplied