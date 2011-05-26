#!/bin/bash

BUILTOK=""
BUILTFAIL=""



for dir in $(find . -maxdepth 1 -type d | grep -v "\.$" );
do
	echo "Making in $dir "
	cd $dir
	make CFG=$1 $2
	if [ $? -eq 0 ]; then
		BUILTOK="$(echo $BUILTOK $dir)"
	else
		BUILTFAIL="$(echo $BUILTFAIL $dir)"
	fi
	cd ..
done

echo
echo
echo "OK : $BUILTOK"
echo
echo "FAIL : $BUILTFAIL"
