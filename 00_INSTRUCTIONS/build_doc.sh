#!/bin/bash

# Build doxygen documentation (generate html files)
# you need to install doxygen
# 

echo ""
echo "===================="
echo "GENERATE DOXYGEN    "

# this would be the genertion command for a single langage:
#doxygen doxygen_config.txt 2> dox_error.log

mkdir -p html_english/
mkdir -p html_french/

#generate english documentation
( echo "OUTPUT_LANGUAGE=English"; echo "OUTPUT_DIRECTORY=html_english/"; cat doxygen_config.txt ) | doxygen - 2> dox_error.log
#generate french documentation
( echo "OUTPUT_LANGUAGE=French"; echo "OUTPUT_DIRECTORY=html_french/"; cat doxygen_config.txt ) | doxygen - 2> dox_error.log

#sometimes doxygen does not copy image file properly.
cp ./images/*.png ./html_english/html/.
cp ./images/*.png ./html_french/html/.

echo ""
echo "===================="
#read -r -p "Stroke enter to exit " response
exit