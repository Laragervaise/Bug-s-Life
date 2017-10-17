#!/bin/bash

SOURCE_FOLDER="../source/"
RENDU_PATH="../source/rendu1.x"

cp "Makefile" $SOURCE_FOLDER
make clean
make -C $SOURCE_FOLDER depend
make -C $SOURCE_FOLDER
cp $RENDU_PATH "."

if [ -f "rendu1.x" ];
then

    i=1
    while [ $i -le 9 ]
    do
        echo "Running E$i.txt!"
        "./"rendu1.x Error "E0$i.txt" > "out_E0$i.txt"
        let "i++"
        echo "Done!"
    done

else
    echo "File rendu1.x does not exists. Please use make first"
fi



