#! /bin/bash
#! /bin/bash -v

echo "CD-ing to $(dirname $0)/../"
cd $(dirname $0)/../

wasp_dir="$(pwd)/$(find -name "wasp-os" | head -1)"
wasp_libs="$(pwd)/$(find -name "wasp_libs" | head -1)" # head -1 needed because of unexplained dir copying wierdness.

echo "$wasp_dir"
echo "$wasp_libs"

cd $wasp_dir

if [[ -e "./bin-.*"  ]]
then
	echo "bins exist"
	# rm -rf ./bin-*
fi

PATH="$PATH:$wasp_libs/gcc-arm-none-eabi-10-2020-q4-major/bin/"

make submodules
make softdevice

make -j `nproc` BOARD=pinetime clean
make -j `nproc` BOARD=pinetime all
