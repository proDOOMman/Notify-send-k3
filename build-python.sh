#!/bin/bash
#TODO: sqlite, readline, zlib modules
#FIXME: _md5 module?
cd /tmp
wget http://www.python.org/ftp/python/2.7.1/Python-2.7.1.tar.bz2
tar xvjf Python-2.7.1.tar.bz2
cd Python-2.7.1

#build host python
./configure
make python Parser/pgen
mv python hostpython
mv Parser/pgen Parser/hostpgen
make distclean

#build kindle's python
wget "http://randomsplat.com/wp-content/uploads/2011/05/Python-2.7.1-xcompile.patch"
patch -p1 < Python-2.7.1-xcompile.patch
source /opt/ELDK-ARM/eldk_init arm # <<< YOUR ELDK HERE
CC=arm-linux-gcc CXX=arm-linux-g++ AR=arm-linux-ar RANLIB=arm-linux-ranlib ./configure --host=arm-linux --build=i686-pc-linux-gnu --prefix=/mnt/us/python
make HOSTPYTHON=./hostpython HOSTPGEN=./Parser/hostpgen BLDSHARED="arm-linux-gcc -shared" CROSS_COMPILE=arm-linux- CROSS_COMPILE_TARGET=yes
make install HOSTPYTHON=./hostpython BLDSHARED="arm-linux-gcc -shared" CROSS_COMPILE=arm-linux- CROSS_COMPILE_TARGET=yes PREFIX=/mnt/usr/python
cd /mnt/us/python/bin
arm-linux-strip python python2.7
cd ../lib
rm -f ./libpython2.7.a
#cd python2.7
#zip -r -y python27.zip .
cd /mnt/us/
tar -zcvf python-2.7.tgz ./python/
echo "Your python targball here: /mnt/us/python-2.7.tgz"
