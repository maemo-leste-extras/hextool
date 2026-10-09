#!/bin/sh

RESOLUTION=`xrandr | fgrep '*' | awk '{print $1}'`
WIDTH=`echo $RESOLUTION | cut -d x -f 1`
HEIGHT=`echo $RESOLUTION | cut -d x -f 2-`
#echo $RESOLUTION
#echo $WIDTH
#echo $HEIGHT

if [ $HEIGHT -gt $WIDTH ]
then
HEIGHT=`echo $RESOLUTION | cut -d x -f 1`
WIDTH=`echo $RESOLUTION | cut -d x -f 2-`
fi

if [ $WIDTH -gt "800" ]
then
    #QU=`awk "BEGIN {print substr(($WIDTH/$HEIGHT)*10, 0, 2)}"`
    SCALEFW=`awk "BEGIN {print substr(($WIDTH/800)*10, 0, 2)}"`
    SCALEFH=`awk "BEGIN {print substr(($HEIGHT/480)*10, 0, 2)}"`
    #echo $SCALEFW
    #echo $SCALEFH
    #echo $QU
	   
    if [ $SCALEFW -ge $SCALEFH ]
    then
        SCALE=`awk "BEGIN {print substr($HEIGHT/480, 0, 3)}"`
        #echo "using height ${SCALE}"
    else
        SCALE=`awk "BEGIN {print substr($WIDTH/800, 0, 3)}"`
        #echo "using width ${SCALE}"
    fi

    QT_SCALE_FACTOR=$SCALE /usr/bin/hextool
else
    /usr/bin/hextool
fi
