if [ ! -n "$1" ]
then
	echo "usage: test.sh debug | release"
	exit
fi

if [ "$1" = "release" ] 
then
	GENGISBUILDTYPE=RELEASE
elif [ "$1" = "debug" ] 
then
	GENGISBUILDTYPE=DEBUG
else
	echo "must specify debug or release"
	exit
fi

# Creating and populating the 'Plugins' directory
cp -Rf ../../../../plugins/ BUILD/$GENGISBUILDTYPE/Plugins/
