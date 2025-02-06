#! /bin/bash -vx

# args :
# (1) wasp-os directory
# (2) glime repo download location.

git clone git@github.com:AndreiBoghean/glime.git

wasp=$1
glime=$2

# assumes we're running from inside wasp-os/glimeSupport/installGlime.sh
if [ -f $wasp ]; then wasp="../"; fi
if [ -f $glime ]; then glime="glime"; fi

wasp=$(readlink $wasp -f)
glime=$(readlink $glime -f)

echo "wasp: $wasp"
echo "glime: $glime"

# "install" process

# insert the wrapper app which acts as the "barrier" between OS and external app.
# the wrapper app calls the main() of the external app.
ln -v -s $(readlink $wasp/glimeSupport/wrapApp.py -f) $wasp/apps/wrap.py

mkdir -p $wasp/modules/crossApp

# insert the compatability interface and wasp-os specific implementation as a C-implemented micropython module.
ln -v -s $(readlink $glime/compat.h -f) $wasp/modules/crossApp/compat.h
ln -v -s $(readlink $wasp/glimeSupport/compat.py.cpp -f) $wasp/modules/crossApp/compat.py.cpp
# also insert the actual OS-agnostic external app
ln -v -s $(readlink $glime/externApp.h -f) $wasp/modules/crossApp/externApp.h
ln -v -s $(readlink $glime/externApp.cpp -f) $wasp/modules/crossApp/externApp.cpp
# lastly add the module config files that define the bindings between C and micropython.
ln -v -s $(readlink $wasp/glimeSupport/internal_module/gateway.c -f) $wasp/modules/crossApp/gateway.c
ln -v -s $(readlink $wasp/glimeSupport/internal_module/micropython.mk -f) $wasp/modules/crossApp/micropython.mk
