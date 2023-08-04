#!/bin/bash

# ending slashes are important:
SRC=/data/proj/imgui_app_base
DST=~/data/proj/

# -r = recursive
# -t = update modification times
# -p = copy permissions (incl executable)
# -o = copy owner
# -g = copy group
# -l = copy symlinks
# -h = human readable numbers
# -v = verbose
# --delete = delete extra on destination

# -n = dry run

rsync -rtvh --stats --exclude={build,build_release,build.win,'.*.swp'} $SRC $DST

echo "FINISHED"

