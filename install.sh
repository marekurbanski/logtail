#!/bin/bash


#######################################################
#
#       Installer 'logtail'
#
#       info and suggestions: marek@www2.pl
#
#######################################################

echo "Installer 'logtail'"
echo ""



function check_package {

    # res=`dpkg -l $1 | grep $1 | wc -l`
    # if [ $res == '1' ]
    echo ""
    echo "------------------------------- check --------------------------------"
    echo "|                                                                     |"
    echo "|                      Checking package: '$1'"
    echo "|                                                                     |"
    echo "----------------------------------------------------------------------"
    echo ""
    if [ -n "$(command -v $1)" ]
    then
        #true
        echo "OK - '$1' it`s installed"
        return 0
    else
        #false
        echo "I didn`t found package '$1' - I will install it"
    if [ -n "$(command -v apt-get)" ]
        then
        if [ -n "$(command -v sudo)" ]
	then
	echo "using APT-GET... Type root password bellow:"
	sudo apt-get update
	sudo apt-get install $1
	else
	apt-get update
	apt-get install $1
	fi
        fi

    if [ -n "$(command -v yum)" ]
        then
        if [ -n "$(command -v sudo)" ]
	then
	echo "using YUM... Type root password bellow:"
            sudo yum install $1
	else
	yum install $1
	fi
        fi


    if [ -n "$(command -v brew)" ]
        then
	echo "using BREW... Type root password bellow:"
        brew install $1
        fi

        return 1
    fi
    }


# checking packeges
check_package gcc-c++


cd src/

g++ -std=c++11 -O3 -Wall -pedantic logtail.cc -o ../binaries/logtail

cd ../

sudo cp binaries/logtail /usr/bin/
chmod 777 /usr/bin/logtail



