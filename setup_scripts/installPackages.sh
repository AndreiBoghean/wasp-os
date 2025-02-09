#! /bin/bash

cd $(dirname $(readlink -f $0))/.. # move into wasp-os folder, relative to this script's location in wasp-os/setup_scripts
# ^ needed since pip below uses a relative link for reqs.txt

sudo pacman -Sy base-devel unzip

make submodules # needed for tools/nrfutil
pip install --break-system-packages -r wasp/requirements.txt
pip install --break-system-packages -r tools/nrfutil/requirements.txt
