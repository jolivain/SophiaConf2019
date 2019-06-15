#! /bin/sh

if [ "$#" -ne 1 ] ; then
    echo "Usage: $0 <fragment-shader-file>" >&2
    exit 1
fi

set -eu

SHADER_FILE="$1"

exec gst-launch-1.0 -v \
    videotestsrc ! \
    glupload ! \
    glcolorconvert ! \
    glshader vertex="\"$(cat gst-shader.vert)\"" fragment="\"$(cat "$SHADER_FILE" | sed 's/\"/\\"/g' )\""  ! \
    glimagesinkelement
