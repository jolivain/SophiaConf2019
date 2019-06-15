#! /bin/sh
#
# Install all the tools needed for the SophiaConf 2019 workshop
# It is assumed here a default install of Ubuntu 18.04.2 LTS, from:
# http://releases.ubuntu.com/18.04.2/ubuntu-18.04.2-desktop-amd64.iso
# Package names may change on another system.

set -eux

apt-get update
apt-get install -y \
        git \
        python \
        curl \
        make \
        gcc \
        autoconf \
        automake \
        pkg-config \
        libpng-dev \
        libx11-dev \
        libegl1-mesa-dev \
        libgles2-mesa-dev \
        \
        gstreamer1.0-libav \
        \
        device-tree-compiler \
        libncurses5-dev \
        qemu-system-arm \
        qt5-default \
        libgtk-3-dev \
        libvte-2.91-dev \
        libgbm-dev \
        libvirglrenderer-dev
