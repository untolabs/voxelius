#!/bin/sh
cmake -B build32 -A Win32 -DENABLE_EXPERIMENTS=ON && \
cmake --build build32 --config Release --parallel && \
cpack --config build32/CPackConfig.cmake -G ZIP
