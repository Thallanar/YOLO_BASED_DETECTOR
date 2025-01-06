#!/bin/bash

# Gera o arquivo de lista automaticamente
rm -f file_list.txt
for file in *.mp4; do
    echo "file '$file'" >> file_list.txt
done

# Concatena os vídeos
ffmpeg -f concat -safe 0 -i file_list.txt -movflags faststart -c copy output.mp4
