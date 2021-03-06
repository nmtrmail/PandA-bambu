#!/bin/bash
export PATH=/opt/panda/bin:$PATH

mkdir -p icrc
cd icrc
echo "#synthesis of icrc"
bambu ../spec.c --top-fname=icrc --simulator=VERILATOR --device-name=xc7z020,-1,clg484,YOSYS-VVD --simulate --generate-tb=../test_icrc.xml --channels-type=MEM_ACC_N1 --experimental-setup=BAMBU --no-iob -v2 2>&1 | tee icrc.log
cd ..

mkdir -p main
cd main
echo "#synthesis of main"
bambu ../spec.c  --simulator=VERILATOR --device-name=xc7z020,-1,clg484,YOSYS-VVD --simulate --generate-tb=../test.xml --channels-type=MEM_ACC_N1 --experimental-setup=BAMBU --no-iob -v2 2>&1 | tee main.log


