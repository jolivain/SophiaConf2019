#! /bin/sh

if [ "$#" -ne 2 ] ; then
    echo "Usage: $0 <video-file> <fragment-shader-file>" >&2
    exit 1
fi

set -eu

VIDEO_FILE="$1"
SHADER_FILE="$2"

exec gst-launch-1.0 -v \
  filesrc location="${VIDEO_FILE}" ! \
    decodebin ! \
    glupload ! \
    glcolorconvert ! \
    glshader vertex="\"$(cat gst-shader.vert)\"" fragment="\"$(cat "$SHADER_FILE" | sed 's/\"/\\"/g' )\""  ! \
    glimagesinkelement
