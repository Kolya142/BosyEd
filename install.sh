#!/bin/sh
clang ed.c -o ed
cp ./ed ~/.local/bin/bosedr
chmod +x ./run.sh
cp ./run.sh ~/.local/bin/bosed