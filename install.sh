#!/bin/bash

binary="bin/release/steam_login"

if [ $1 ]
then
	cp $binary $1
	exit 0
fi

# dirs where to put binary in order "most preferred to least preferred"
dirs=(
	"$HOME/.local/bin"
)

for dir in ${dirs[@]}
do
	if [ -d $dir ]
	then
		if [ -f $binary ]
		then
			cp $binary $dir
			echo "installed to '$dir'"
			exit 0
		else
			echo "please build the project first"
			exit 1
		fi
	fi
done

echo "No valid install directory has been found."
echo "Please run with './install.sh <directory>'"
echo "where directory is the directory you want to install"
echo "SteamLoginTherapy to."
exit 1
