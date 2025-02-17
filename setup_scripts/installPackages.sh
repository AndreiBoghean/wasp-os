#! /bin/bash

cd $(dirname $(readlink -f $0))/.. # move into wasp-os folder, relative to this script's location in wasp-os/setup_scripts
# ^ needed since pip below uses a relative link for reqs.txt

purple=$(printf '\033[1;35m')
green=$(printf '\033[1;32m')
bigGreen=$(printf '\033[30;102m')
reset=$(printf '\033[0m')
echo "${purple}installing packages base-devel and unzip${reset}"
sudo pacman -Sy base-devel unzip
echo "${green}finished installing packages${reset}"

echo "${purple}downloading submodules${reset}"
make submodules # needed for tools/nrfutil
echo "${green}finished downloading submodules${reset}"

echo "${purple}installing python dependencies${reset}"
pip install --break-system-packages -r wasp/requirements.txt
pip install --break-system-packages -r tools/nrfutil/requirements.txt
echo "${green}finished installing python dependencies${reset}"

echo "${bigGreen}script complete${reset}"
