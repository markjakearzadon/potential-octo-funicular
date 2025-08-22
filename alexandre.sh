#!/usr/bin/bash

set -e

sudo apt update
sudo apt install -y \
    build-essential \
    g++ \
    cmake \
    libglfw3 \
    libglfw3-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libglew-dev \
    libglm-dev \
    mesa-utils \
    libx11-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev
sudo rm -rf /var/lib/apt/lists/*
