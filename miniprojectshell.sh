#!/bin/sh
echo 'Starting programs on both processors...'

cd miniproject_2/sw/nios/application_0
nios2-download -g cpu_0_proj.elf --device 2 --instance 0
cd ../application_1
nios2-download -g cpu_1_proj.elf --device 2 --instance 1