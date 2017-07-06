#!/usr/bin/env bash

function run()
{
    local dataset=$1
    local type=$2
    local algo=${4}_${5}_${3}
    local sr=$4
    local scale=$5

    result=result_scale_rate/${1:9}_${algo}
    resultML=result_scale_rate/${1:9}_${algo}_ML
    data_algo=${1:9}_${algo}

    ./bin/FlowSizeIP $dataset $result $type 0 ${data_algo} ${sr} ${scale}
    echo `date +%Y-%m-%d\ %H:%M:%S` ${data_algo} done.
    python FlowSizeML.py ./MLData/${data_algo}_sketch.train ./Para/${data_algo}_sketch.para
    echo `date +%Y-%m-%d\ %H:%M:%S` ${data_algo} Machine Learning dataset generated.
    ./bin/FlowSizeIP $dataset $resultML $type 1 ${data_algo} ${sr} ${scale}
    echo `date +%Y-%m-%d\ %H:%M:%S` ${data_algo} Machine Learning done.
}

sr=(5 10 20 40 100 200 500 1000)
scale=(0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5 0.55 0.6 0.65 0.7 0.75 0.8 0.85)

for f in FlsmData/*
do
    echo `date +%Y-%m-%d\ %H:%M:%S` $f
    dataset=$f

    for r in ${sr[@]}
    do
        for sc in ${scale[@]}
        do
            rate=`echo "scale=4; ${r} * ${sc}" | bc`
            run $dataset 4 CSM $r $rate &
        done

        rate=`echo "scale=4; ${r} * 0.9" | bc`
        run $dataset 4 CSM $r $rate
    done
done