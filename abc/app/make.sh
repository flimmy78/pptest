#!/bin/bash -e

export HOST_BINARY=/home/job1/bd1compiler/mips-2014.05/bin





export CURPATH=${PWD}
export ROOTFS_PATH=${CURPATH}/../rootfs
export APP_FOLDER=${CURPATH}
export CROSS_COMPILE=$HOST_BINARY/mips-linux-gnu-
export PACKAGE_ROOT=$APP_FOLDER/package_root

cd $APP_FOLDER


rm -rf mips_build

#Pre Build
#===================
rm -rf $PACKAGE_ROOT/

#Remove previous binary file in target device
sudo rm -rf $ROOTFS_PATH/root/video_server
rm -rf $APP_FOLDER/bin
rm -rf $APP_FOLDER/package_root

#Create folder to keep binary and data.
mkdir -p $APP_FOLDER/bin/
mkdir -p $APP_FOLDER/bin/conf

#Create temporary folder for MIPS build.
mkdir -p mips_build

#Build
#===================
# Build project
cd mips_build

cmake  -DCMAKE_SYSTEM_NAME="Linux" \
	-DCMAKE_C_COMPILER="${CROSS_COMPILE}gcc" \
	-DCMAKE_CXX_COMPILER="${CROSS_COMPILE}g++" \
	-DCMAKE_AR="${CROSS_COMPILE}ar" \
	-DCMAKE_C_FLAGS="-EL -g  -O2 -fPIC"  \
	-DCMAKE_CXX_FLAGS="-EL -g  -O2 -fPIC" \
	../

make


# Build RTSP video streamer.
cd ${APP_FOLDER}/src/video_server/streamer/rtsp 
make -f Makefile.slave clean
make -f Makefile.slave
cp bin/slave/server $APP_FOLDER/bin/
cp activator/slave.conf $APP_FOLDER/bin/conf


#Back to app folder to finish.
cd $APP_FOLDER


#Post Build
cp scripts/run_video_server.sh $APP_FOLDER/bin/

# Strip files
${CROSS_COMPILE}strip $APP_FOLDER/bin/server
${CROSS_COMPILE}strip $APP_FOLDER/bin/bd_encoder

#Deploy everything to target file system.
#===================
sudo mkdir -p $ROOTFS_PATH/root/video_server

rm -rf  $PACKAGE_ROOT
mkdir -p $PACKAGE_ROOT
mkdir -p $PACKAGE_ROOT/etc
mkdir -p $PACKAGE_ROOT/etc/'init.d'
mkdir -p $PACKAGE_ROOT/usr
mkdir -p $PACKAGE_ROOT/usr/bin
mkdir -p $PACKAGE_ROOT/usr/lib
mkdir -p $PACKAGE_ROOT/lib
mkdir -p $PACKAGE_ROOT/root
mkdir -p $PACKAGE_ROOT/root/video_server

#Copy nginx configuration script for service.
#sudo cp $APP_FOLDER/config_src/nginx.conf $PACKAGE_ROOT/etc/nginx/

#Copy initial script for service.
sudo chmod +x $APP_FOLDER/scripts/*
sudo cp $APP_FOLDER/scripts/inittab 		$PACKAGE_ROOT/etc/
sudo cp $APP_FOLDER/scripts/S90video 		$PACKAGE_ROOT/etc/init.d/
sudo cp $APP_FOLDER/scripts/S60telnet 		$PACKAGE_ROOT/etc/init.d/

#Deploy other files.
sudo cp -rf $APP_FOLDER/bin/* 			$PACKAGE_ROOT/root/video_server
sudo chmod +x $PACKAGE_ROOT/root/video_server/run_video_server.sh


cp -rf usrlib/* $PACKAGE_ROOT/usr/lib


#Copy $PACKAGE_ROOT to $ROOTFS_PATH
sudo cp -ar $PACKAGE_ROOT/.  $ROOTFS_PATH/.

cd $APP_FOLDER/

cp bin/bd_encoder /home/jmnt/bd1prj/imagecur/rootfs/ramdiskfs/rootfs/opt/rtsptest/
cp bin/server /home/jmnt/bd1prj/imagecur/rootfs/ramdiskfs/rootfs/opt/rtsptest/
