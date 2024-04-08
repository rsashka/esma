#!/bin/bash

root="."
if [ ! -z $1 ]; then
    root="$1"
fi


#
#alldoc="newlang_doc"
#
#lang="
#    en
#    ru
#    "
#
#list="
#    about
#    roadmap
#    syntax
#    types
#    "
#

function check_new_file() {
    if [ ! -f $1.temp ]; then
         echo File name $1.temp not found!
         exit 1
    fi

    if [ ! -f $1 ]; then
        mv $1.temp $1
    else

       if cmp -s $1 $1.temp; then
           rm $1.temp
           echo Use file $1 for current build
       else
           rm $1
           mv $1.temp $1
           echo Use new file $1
       fi
    fi
}

#mkdir $root/output/site
#
#for flang in $lang; do
#    fdir="$root/docs/$flang"
#
#    echo -n '' > $fdir/$alldoc.md
#    header=""
#
#    for ffile in $list; do
#
#        fpath="$fdir/$ffile.md"
#        if [ ! -f "$fpath" ]; then
#            echo "File '$fpath' not found!"
#            exit 1
#        fi
#
#        #All headings must be second level except the first
#        echo -n "$header" >> $fdir/$alldoc.md
#        header="#"
#
#        cat $fpath >> $fdir/$alldoc.md
#        echo -e "------\n$ffile\n\n" >> $fdir/$alldoc.md
#
#        cp  $fpath  $root/output/site/$ffile.$flang.md
#
#    done
#
#    pandoc -f gfm -t plain $fdir/$alldoc.md > $fdir/$alldoc.txt
#
#    $root/contrib/text2cpp/output/bin/text2cpp $fdir/$alldoc.md  $root/lib/syntax_help_$flang.cpp.temp  newlang_syntax_help_$flang c
#
#    rm $fdir/$alldoc.md
#
#    check_new_file   $root/lib/syntax_help_$flang.cpp
#
#done

VERSION_MAJOR=0
VERSION_MINOR=5
VERSION_PATCH=0
VERSION_HEADER="$root/lib/version.h"
VERSION_FILE="$root/lib/version.cpp"
VERSION_HEADER_TEMP="$root/lib/version.h.temp"
VERSION_FILE_TEMP="$root/lib/version.cpp.temp"


GIT_TAG_VERSION=`git describe --abbrev=0 --tags`
GIT_SHORT_HASH=`git rev-parse --short HEAD`
GIT_SOURCE_ID="$GIT_TAG_VERSION-$GIT_SHORT_HASH"
DATE_BUILD=`date +'%Y.%m.%d %H:%M:%S'`


if [ ! "$GIT_TAG_VERSION" = "v$VERSION_MAJOR.$VERSION_MINOR.$VERSION_PATCH" ]; then
    echo "Git TAG $GIT_TAG_VERSION differ version v$VERSION_MAJOR.$VERSION_MINOR.$VERSION_PATCH"
    exit 1
fi

echo "$VERSION_MAJOR.$VERSION_MINOR.$VERSION_PATCH" > $root/TAG.temp
check_new_file $root/TAG


if [ -z "$GIT_SHORT_HASH" ]; then
    echo Undefined version for build
    exit 1
fi
if [ -f $VERSION_HEADER_TEMP ]; then
    rm $VERSION_HEADER_TEMP
fi

echo "/** @file $VERSION_HEADER" > $VERSION_HEADER_TEMP
echo "* Auto generate file for identification current build" >> $VERSION_HEADER_TEMP
echo "* Date build $DATE_BUILD" >> $VERSION_HEADER_TEMP
echo "*/"  >> $VERSION_HEADER_TEMP
echo ""  >> $VERSION_HEADER_TEMP

echo "#define VERSION_DATE_BUILD_STR \"$DATE_BUILD\""  >> $VERSION_HEADER_TEMP
echo "#define VERSION_SOURCE_FULL_ID \"$GIT_SOURCE_ID $DATE_BUILD\""  >> $VERSION_HEADER_TEMP
echo ""  >> $VERSION_HEADER_TEMP

echo "#include <stdint.h>"  >> $VERSION_HEADER_TEMP

echo "extern const uint8_t VERSION_MAJOR;"  >> $VERSION_HEADER_TEMP
echo "extern const uint8_t VERSION_MINOR;"  >> $VERSION_HEADER_TEMP
echo "extern const uint8_t VERSION_PATCH;"  >> $VERSION_HEADER_TEMP
echo ""  >> $VERSION_HEADER_TEMP

echo "#define VERSION ($VERSION_MAJOR << 4 | $VERSION_MINOR)"  >> $VERSION_HEADER_TEMP
echo "#define VERSION_GIT_SOURCE \"$GIT_SOURCE_ID\""  >> $VERSION_HEADER_TEMP
echo ""  >> $VERSION_HEADER_TEMP

echo "extern const char * GIT_SOURCE;"  >> $VERSION_HEADER_TEMP
echo "extern const char * DATE_BUILD_STR;"  >> $VERSION_HEADER_TEMP
echo "extern const char * SOURCE_FULL_ID; "  >> $VERSION_HEADER_TEMP
echo ""  >> $VERSION_HEADER_TEMP

if [ -f $VERSION_FILE_TEMP ]; then
    rm $VERSION_FILE_TEMP
fi

echo "/** @file $VERSION_FILE" > $VERSION_FILE_TEMP
echo "* Auto generate file for identification current build" >> $VERSION_FILE_TEMP
echo "* Date build $DATE_BUILD" >> $VERSION_FILE_TEMP
echo "*/"  >> $VERSION_FILE_TEMP
echo ""  >> $VERSION_FILE_TEMP

echo "#include \"version.h\""  >> $VERSION_FILE_TEMP
echo ""  >> $VERSION_FILE_TEMP

echo "const uint8_t VERSION_MAJOR=$VERSION_MAJOR;"  >> $VERSION_FILE_TEMP
echo "const uint8_t VERSION_MINOR=$VERSION_MINOR;"  >> $VERSION_FILE_TEMP
echo "const uint8_t VERSION_PATCH=$VERSION_PATCH;"  >> $VERSION_FILE_TEMP
echo ""  >> $VERSION_FILE_TEMP

echo "const char * GIT_SOURCE = VERSION_GIT_SOURCE;"  >> $VERSION_FILE_TEMP
echo "const char * DATE_BUILD_STR = VERSION_DATE_BUILD_STR;"  >> $VERSION_FILE_TEMP
echo "const char * SOURCE_FULL_ID = VERSION_SOURCE_FULL_ID;"  >> $VERSION_FILE_TEMP
echo ""  >> $VERSION_FILE_TEMP

if cmp --ignore-initial=250:250 -s $VERSION_HEADER $VERSION_HEADER_TEMP; then
    rm $VERSION_HEADER_TEMP
    echo Use file $VERSION_HEADER for current build
else
    rm $VERSION_HEADER
    mv $VERSION_HEADER_TEMP $VERSION_HEADER
    echo Make file $VERSION_HEADER for version $GIT_SOURCE_ID at date $DATE_BUILD
fi

if cmp --ignore-initial=110:110 -s $VERSION_FILE $VERSION_FILE_TEMP; then
    rm $VERSION_FILE_TEMP
    echo Use file $VERSION_FILE for current build
else
    rm $VERSION_FILE
    mv $VERSION_FILE_TEMP $VERSION_FILE
    echo Make file $VERSION_FILE for version $GIT_SOURCE_ID at date $DATE_BUILD
fi

$root/contrib/text2cpp/output/bin/text2cpp $root/build_options.txt  $root/lib/build_options.data.temp  newlang_build_options c
check_new_file $root/lib/build_options.data

$root/contrib/text2cpp/output/bin/text2cpp $root/lib/include_h.i        $root/lib/include_h_i.data.temp    newlang_include_h_i c
check_new_file $root/lib/include_h_i.data
