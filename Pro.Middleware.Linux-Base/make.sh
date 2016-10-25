#!/bin/sh

sudo apt-get install libasound2-dev

gcc -o lplay lplay.c sndwav_common.c wav_parser.c -lasound
gcc -o lrecord lrecord.c sndwav_common.c wav_parser.c -lasound
gcc -o lmain frbuf_keyb.c
