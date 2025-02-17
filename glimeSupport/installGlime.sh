#! /bin/bash -vx

# args :
# (1) wasp-os directory
# (2) glime repo download location.

wasp=$1
glime=$2

if [ -f $wasp ]; then wasp="$(dirname $(readlink -f $0))/.."; fi
if [ -f $glime ]; then glime=$wasp/glimeSupport/glime; git clone https://github.com/AndreiBoghean/glime.git $glime; fi

wasp=$(readlink $wasp -f)
glime=$(readlink $glime -f)

echo "wasp: $wasp"
echo "glime: $glime"

# "install" process

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
