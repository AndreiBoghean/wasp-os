#! /bin/bash -xv

cd ../ # move into wasp-os folder, relative to this script's location in wasp-os/setup_scripts

# download wasp-os
# git clone git@github.com:wasp-os/wasp-os.git
# download wasp-os dependencies

# download wasp-os dependencies
mkdir wasp_libs
cd wasp_libs
# x86_64 Linux hosted cross toolchains for an AArch32 bare-metal target (arm-none-eabi)
# from https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads
# wget https://developer.arm.com/-/media/Files/downloads/gnu/13.3.rel1/binrel/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi.tar.xz
url='https://developer.arm.com/-/media/Files/downloads/gnu-rm/10-2020q4/gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.bz2?revision=ca0cbf9c-9de2-491c-ac48-898b5bbc0443&rev=ca0cbf9c9de2491cac48898b5bbc0443&hash=72D7BCC38C586E3FE39D2A1DB133305C64CA068B'
$filename='gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.bz2'
wget $url -O $filename
tar -xvf $filename

cd -
