cd `dirname $0`
SCRIPTDIR=`pwd`
cd -

rm -rf $SCRIPTDIR/../build
rm -rf $SCRIPTDIR/../.cache
rm -rf $SCRIPTDIR/../Logs
rm -rf $SCRIPTDIR/../bin/Logs
rm -rf $SCRIPTDIR/../bin/SERVER
rm -rf $SCRIPTDIR/../bin/CONSOLE
rm -rf $SCRIPTDIR/../bin/GUI