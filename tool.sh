#/bin/bash
set -e

TOOL_NAME=$1

if [ ! -f tool/$TOOL_NAME.c ]; then
    echo "The tool $TOOL_NAME doesn't exist!"
    exit 1
fi

TEMP_COMP_DIR=$(mktemp -d)

clean_up () {
    rm -rf $TEMP_COMP_DIR
}
trap clean_up EXIT

clang -o $TEMP_COMP_DIR/$TOOL_NAME tool/$TOOL_NAME.c

$TEMP_COMP_DIR/$TOOL_NAME ${@:2}