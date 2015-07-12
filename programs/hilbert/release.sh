#!/bin/bash

program="hilbert"

date_string=`date "+%Y%m%d"`

output_folder_short="${program}_$date_string"
output_folder="./$output_folder_short"

zip_file_name_short="$output_folder_short.zip"
zip_file_name="./$zip_file_name_short"


if [ -d "$output_folder" ]; then
	rm -r $output_folder
fi

if [ -f "$zip_file_name" ]; then
	rm $zip_file_name
fi


mkdir -p $output_folder/program/$program
cp -r ./*.cpp ./*.h ./Makefile ./release.sh $output_folder/program/$program


mkdir $output_folder/mylib
cp ../../mylib/fps.cpp ../../mylib/fps.h $output_folder/mylib


mkdir -p $output_folder/program/$program/build_mingw
cp -r ./build_mingw/$program.exe $output_folder/program/$program/build_mingw
cp -r ../../external/dll/mingw/* $output_folder/program/$program/build_mingw
cp -r ../../external/dll/glut/* $output_folder/program/$program/build_mingw


mkdir -p $output_folder/external/include
cp -r ../../external/include/GL $output_folder/external/include


mkdir -p $output_folder/external/lib
cp -r ../../external/lib/libfreeglut.a $output_folder/external/lib


cd $output_folder/..

zip -r $zip_file_name_short $output_folder_short

rm -r $output_folder

