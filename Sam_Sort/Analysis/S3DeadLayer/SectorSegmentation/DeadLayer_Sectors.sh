#!/bin/bash
calibrationRun=$1

echo "Removing existing DeadLayerThickness.dat file"
rm -f DeadLayerThickness.dat
mkdir "Sectors/$calibrationRun"
for sector in {0..31}; do
    mkdir "Sectors/$calibrationRun/$sector"
    ./S3SectorDeadLayer $calibrationRun $sector
done

file="DeadLayerThickness.dat"

awk '
{
    x = $NF
    sum += x
    sumsq += x*x
    n++
}
END {
    mean = sum/n
    stddev = sqrt((sumsq - sum*sum/n)/(n-1))

    printf "Number of sectors: %d\n", n
    printf "Mean dead layer:    %.6f um\n", mean
    printf "Standard deviation: %.6f um\n", stddev
}
' "$file"

