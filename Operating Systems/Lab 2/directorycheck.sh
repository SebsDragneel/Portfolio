#! /bin/bash
find -maxdepth 1 -type d -print | wc -l > directorydata
find -maxdepth 1 -type f -print | wc -l >> directorydata
find -size 0 -type f -print >> directorydata
du -h >> directorydata
