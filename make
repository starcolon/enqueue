#!/bin/sh

g++ -m64 -o q q.cpp -L/opt/mqm/lib64 -Wl,-rpath=/opt/mqm/lib64 \
 -Wl,-rpath=/usr/lib64 -limqc23gl_r -limqb23gl_r -lmqic_r


g++ -m64 -o sample_put sample_put.cpp -L/opt/mqm/lib64 -Wl,-rpath=/opt/mqm/lib64 \
 -Wl,-rpath=/usr/lib64 -limqc23gl_r -limqb23gl_r -lmqic_r
 