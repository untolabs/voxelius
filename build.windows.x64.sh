#!/bin/sh
cmake -B build64 -A x64 -DENABLE_EXPERIMENTS=ON && \
cmake --build build64 --config Release --parallel && \
cpack --config build64/CPackConfig.cmake -G ZIP
