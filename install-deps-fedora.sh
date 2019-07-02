#! /bin/sh
#
# Install all the tools needed for the SophiaConf 2019 workshop
# It is assumed here a default install of Fedora 29 or 30, from:
# https://getfedora.org
# Package names may change on another system.

set -eux

dnf update -y

dnf install -y \
        git \
        python \
        curl \
        make \
        gcc \
        autoconf \
        automake \
        pkg-config \
        libpng-devel \
        libX11-devel \
        mesa-libEGL-devel \
        mesa-libGLES-devel \
        \
        gstreamer1-libav \
        \
        dtc \
        ncurses-devel \
        qemu-system-arm \
        qt5-qtbase-devel \
        gtk3-devel \
        vte291-devel \
        libgbm-devel \
        virglrenderer-devel
