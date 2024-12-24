#!/bin/sh
cmake -B build -DCMAKE_BUILD_TYPE=Release -DENABLE_EXPERIMENTS=ON && \
cmake --build build --parallel $(nproc) && \
cpack --config build/CPackConfig.cmake -G ZIP
