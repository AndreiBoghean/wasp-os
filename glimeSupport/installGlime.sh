#! /bin/bash

# args :
# (1) wasp-os directory
# (2) glime repo download location.

purple=$(printf '\033[1;35m')
green=$(printf '\033[1;32m')
yellow=$(printf '\033[1;33m')
bigGreen=$(printf '\033[30;102m')
reset=$(printf '\033[0m')

wasp=$1
glime=$2

choice="y"
if [ -f $glime ]; then read -e -p "you've not specified a path to an existing glime download... shall this script download it for you? (notherwise.. this script terminate.) [y/n] " choice; fi
if [[ "$choice" != [Yy]* ]]; then exit 1; fi


if [ -f $wasp ]; then wasp="$(dirname $(readlink -f $0))/.."; fi
if [ -f $glime ]; then glime=$wasp/glimeSupport/glime; git clone https://github.com/AndreiBoghean/glime.git $glime; fi

wasp=$(readlink $wasp -f)
glime=$(readlink $glime -f)

echo "${yellow}wasp: $wasp${reset}"
echo "${yellow}glime: $glime${reset}"

# "install" process

cd $wasp

echo "${purple}setting up symlinks from wasp->glime${reset}"
# insert the wrapper app which acts as the "barrier" between OS and external app.
# the wrapper app calls the main() of the external app.
ln -v -s $wasp/glimeSupport/wrapApp.py $wasp/apps/wrap.py

mkdir -p $wasp/wasp/modules/crossApp

# insert the compatability interface and wasp-os specific implementation as a C-implemented micropython module.
ln -v -s $glime/compat.h $wasp/wasp/modules/crossApp/compat.h
ln -v -s $wasp/glimeSupport/compat.py.c $wasp/wasp/modules/crossApp/compat.py.c
# also insert the actual OS-agnostic external app
ln -v -s $glime/externApp.h $wasp/wasp/modules/crossApp/externApp.h
ln -v -s $glime/externApp.c $wasp/wasp/modules/crossApp/externApp.c
# lastly add the module config files that define the bindings between C and micropython.
ln -v -s $wasp/glimeSupport/internal_module/gateway.c $wasp/wasp/modules/crossApp/gateway.c
ln -v -s $wasp/glimeSupport/internal_module/micropython.mk $wasp/wasp/modules/crossApp/micropython.mk


echo "${green}done setting up symlinks from wasp->glime${reset}"

echo "${bigGreen}finished integrating glime${reset}"
