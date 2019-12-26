# A simple tool to secure a file for bit errors

## Description

This is a simple project that I made during my studies. One can secure a
file with error correction bits - Reed-Solomon code is used. The resulting file
is not bigger than 175% (considering 1MB+ files) and should be (partially)
tolerant to burst errors as well. Obtained 10/10 points.

Use on your own risk.

## Build

For static:
```sh
make
```
For shared:
```sh
make shared
```

## Use

For static:
```sh
./rsenc you.file
./rsdec you.file.rsecc
```
For shared:
```sh
LD_LIBRARY_PATH=. ./rsenc-shared you.file
LD_LIBRARY_PATH=. ./rsdec-shared you.file.rsecc
```

2019
