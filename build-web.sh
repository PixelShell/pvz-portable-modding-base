#!/usr/bin/env bash
set -euo pipefail

BUILD_TYPE="${1:-Release}"
BUILD_TYPE_LOWER=$(echo "$BUILD_TYPE" | tr '[:upper:]' '[:lower:]')
EMSDK_DIR="${EMSDK:-$HOME/emsdk}"
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="build/wasm-$BUILD_TYPE_LOWER"

if [ ! -d "$EMSDK_DIR" ]; then
    echo "==> Cloning and installing emsdk (this only happens once)..."
    git clone https://github.com/emscripten-core/emsdk.git "$EMSDK_DIR"
    "$EMSDK_DIR/emsdk" install latest
    "$EMSDK_DIR/emsdk" activate latest
else
    echo "==> emsdk already present at $EMSDK_DIR"
fi
source "$EMSDK_DIR/emsdk_env.sh"

SYSROOT="$(em-config EMSCRIPTEN_ROOT)/cache/sysroot"
if [ ! -f "$SYSROOT/lib/libopenmpt.a" ]; then
    echo "==> Building libopenmpt for Emscripten..."
    OPENMPT_TAG=$(git ls-remote --tags https://github.com/OpenMPT/openmpt.git 'refs/tags/libopenmpt-*' \
        | sed 's|.*refs/tags/libopenmpt-||' \
        | grep -E '^[0-9]+\.[0-9]+\.[0-9]+$' | sort -V | tail -1 || true)
    [ -z "$OPENMPT_TAG" ] && OPENMPT_TAG="0.8.4"
    echo "==> Using libopenmpt version: $OPENMPT_TAG"
    TMP_SRC=$(mktemp -d)
    git clone --depth=1 --branch "libopenmpt-${OPENMPT_TAG}" https://github.com/OpenMPT/openmpt.git "$TMP_SRC"
    (
        cd "$TMP_SRC"
        emmake make CONFIG=emscripten \
            STATIC_LIB=1 SHARED_LIB=0 \
            EXAMPLES=0 OPENMPT123=0 TEST=0 \
            NO_ZLIB=1 NO_MPG123=1 NO_OGG=1 NO_VORBIS=1 NO_VORBISFILE=1 \
            NO_PORTAUDIO=1 NO_PORTAUDIOCPP=1 NO_PULSEAUDIO=1 \
            NO_SDL2=1 NO_FLAC=1 NO_SNDFILE=1 \
            -j"$(nproc)"
        emmake make CONFIG=emscripten \
            STATIC_LIB=1 SHARED_LIB=0 \
            EXAMPLES=0 OPENMPT123=0 TEST=0 \
            PREFIX="$SYSROOT" install
    )
    rm -rf "$TMP_SRC"
else
    echo "==> libopenmpt already built in Emscripten sysroot, skipping"
fi

echo "==> Configuring WebAssembly build ($BUILD_TYPE)..."
emcmake cmake -G Ninja -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="$PROJECT_ROOT/$BUILD_DIR/result" \
    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY="$PROJECT_ROOT/$BUILD_DIR/result"

echo "==> Building..."
cmake --build "$BUILD_DIR" -j"$(nproc)"

echo "==> Done. Output in: $BUILD_DIR/result/ (index.html, index.js, index.wasm, index.data)"
echo "==> To test locally: cd $BUILD_DIR/result && python3 -m http.server 8000"
