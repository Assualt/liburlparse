#!/usr/bin/env bash
gprof -b client gmon.out > report.txt
python3 gprof2dot.py report.txt > report.dot
dot -Tpng -o hello.png report.dot
