#! /bin/bash
#! /bin/bash -v

wasp_dir="$(dirname $(readlink -f $0))/.."
wasp_libs="$wasp_dir/wasp_libs"

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
