#!/usr/bin/env bash

function prepare()
{
    local dataset=$1
    local type=$2
    local algo=$3

    result=result_seq/${1:9}_${algo}
    resultML=result_seq/${1:9}_${algo}_ML
    data_algo=${algo}

    ./bin/FlowSizeIP $dataset $result $type 0 ${data_algo}
    echo `date +%Y-%m-%d\ %H:%M:%S` ${data_algo} done.
    python FlowSizeML.py ./MLData/${data_algo}_sketch.train ./Para/${data_algo}_sketch.para
    echo `date +%Y-%m-%d\ %H:%M:%S` ${data_algo} Machine Learning dataset generated.
}

function run()
{
    local dataset=$1
    local type=$2
    local algo=$3

    result=result_seq/${1:9}_${algo}
    resultML=result_seq/${1:9}_${algo}_ML
    data_algo=${algo}
    da=${1:9}_${algo}

    ./bin/FlowSizeIP $dataset $result $type 0 ${da}
    echo `date +%Y-%m-%d\ %H:%M:%S` ${data_algo}_${dataset} done.
    python FlowSizeML.py ./MLData/${da}_sketch.train ./Para/${da}_sketch.para
    ./bin/FlowSizeIP $dataset $resultML $type 1 ${data_algo}
    echo `date +%Y-%m-%d\ %H:%M:%S` ${data_algo}_${dataset} Machine Learning done.
    ./bin/FlowSizeIP $dataset ${resultML}_raw $type 1 ${da}
    echo `date +%Y-%m-%d\ %H:%M:%S` ${data_algo}_${dataset}_raw Machine Learning done.
}

folder=FlowLg01

# generate training set and do ml
st=$folder/u00
echo `date +%Y-%m-%d\ %H:%M:%S` $st

prepare $st 0 CM
for f in ${folder}/*
do
    echo `date +%Y-%m-%d\ %H:%M:%S` $f
    dataset=$f
    run $dataset 0 CM &
done

prepare $st 2 CU
sleep 1m
for f in ${folder}/*
do
    echo `date +%Y-%m-%d\ %H:%M:%S` $f
    dataset=$f
    run $dataset 2 CU &
done

prepare $st 4 CSM
sleep 1m
for f in ${folder}/*
do
    echo `date +%Y-%m-%d\ %H:%M:%S` $f
    dataset=$f
    run $dataset 4 CSM &
done