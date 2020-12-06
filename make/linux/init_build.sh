#!/bin/bash

if [ ! -d $1 ]
then 
    mkdir $1
    mkdir $1/obj
    mkdir $1/obj/libsurface
    mkdir $1/obj/libdas
    mkdir $1/obj/libcommon
    mkdir $1/obj/dam
    mkdir $1/obj/deng
    mkdir $1/deng
    mkdir $1/deng/lib
else
    if [ ! -d $1/obj ] 
    then 
        mkdir $1/obj 
    fi

    if [ ! -d $1/obj/libsurface ] 
    then 
        mkdir $1/obj/libsurface 
    fi

    if [ ! -d $1/obj/libdas ] 
    then 
        mkdir $1/obj/libdas 
    fi
    
    if [ ! -d $1/obj/libcommon ]
    then
        mkdir $1/obj/libcommon
    fi

    if [ ! -d $1/obj/deng ] 
    then 
        mkdir $1/obj/deng 
    fi

    if [ ! -d $1/obj/das ] 
    then 
        mkdir $1/obj/das 
    fi

    if [ ! -d $1/deng ] 
    then 
        mkdir $1/deng 
    fi

    if [ ! -d $1/deng/lib ] 
    then 
        mkdir $1/deng/lib
    fi
fi