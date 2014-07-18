#!/bin/bash
echo hello world
echo good bye
copy_files() {
    local new_copy="$1"
    local old_copy="$2"
    local versioned=`git rev-parse --short HEAD`"_$1"
    if diff "$new_copy" "$old_copy" 2>/dev/null ; then
	echo Same file nothing to do
    else
	echo File changed make copies.
	cp "$1" "$2"
	cp "$1" "/home/gitlab_ci_runner/artifacts/closed_sesame/build/"`git rev-parse --short HEAD`_"$1"
    fi    
}

git submodule update --init
ls -la
cd hardware
make clean
make CUSTOMPCB="/usr/src/pcb/src/pcb" \
     CUSTOMPCB_ARGS="--photo-mask-colour red \
     --photo-silk-colour white --photo-plating  tinned"
make CUSTOMPCB="/usr/src/pcb/src/pcb" \
     CUSTOMPCB_ARGS="--photo-mask-colour red \
     --photo-silk-colour white  --photo-plating  tinned" photos
ls black_board.png board.png bom order xy schematic.png schematic.pdf gerbers/closed_sesame_make.bottom.gbr > /dev/null 2>&1
if [ "$?" != "0" ]; then
    echo "I failed to create all the files I should have. build failed"
    exit 1
fi
New_closed_sesame=`git diff HEAD^ HEAD -- closed_sesame.pcb|wc -l`
New_schematic=`git diff HEAD^ HEAD -- closed_sesame.sch|wc -l`

if [ "$New_closed_sesame" == "0" ]; then
    echo "No changes to closed_sesame don't bother saving image."
else
    copy_files board.png ~/artifacts/closed_sesame/board.png
    zip -r gerbers.zip gerbers/
    copy_files gerbers.zip ~/artifacts/closed_sesame/gerbers.zip
fi

if [ "$New_schematic" == "0" ]; then
    echo "No changes to schematic don't bother saving the image."
else
    copy_files schematic.png ~/artifacts/closed_sesame/schematic.png
fi
cd ..

cd firmware
make clean
cp ~/blah/closed_sesame.txt ~/.arduino15/preferences.txt
make ARDUINO_DIR=/usr/src/arduino-1.5.6-r2/
ls bin/application.hex
if [ "$?" != "0" ]; then
    echo "I failed to create all the files I should have. build failed"
    exit 1
fi
cd bin

copy_files application.hex ~/artifacts/closed_sesame/application.hex

cd ..

cd ..
cd android/RFDuinoTest
JAVA_HOME=/usr/lib/jvm/java-7-openjdk-amd64/  ANDROID_HOME=/opt/adt-bundle-linux-x86_64-20140702/sdk/ gradle clean
JAVA_HOME=/usr/lib/jvm/java-7-openjdk-amd64/  ANDROID_HOME=/opt/adt-bundle-linux-x86_64-20140702/sdk/ gradle build
cd build/apk/
copy_files RFDuinoTest-debug-unaligned.apk ~/artifacts/closed_sesame/closed_sesame.apk
cd ../..
cd ../..

