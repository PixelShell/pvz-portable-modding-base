#!/usr/bin/env bash
set -euo pipefail

ANDROID_NDK="${ANDROID_NDK:-$HOME/Android/Sdk/ndk/27.0.12077973}"
VCPKG_ROOT="${VCPKG_ROOT:-$HOME/vcpkg}"
ANDROID_API="24"
ABI="arm64-v8a"
SDL2_TAG="2.32.10"
BUILD_TYPE="${1:-Debug}"
BUILD_TYPE_LOWER=$(echo "$BUILD_TYPE" | tr '[:upper:]' '[:lower:]')
export ANDROID_NDK
export ANDROID_NDK_HOME="$ANDROID_NDK"
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_ROOT"

BUILD_DIR="build/android-$BUILD_TYPE_LOWER-$ABI"

echo "==> Project root: $PROJECT_ROOT"
echo "==> Build type: $BUILD_TYPE"
echo "==> Build dir: $BUILD_DIR"

if [ ! -d sdl2-src ]; then
    echo "==> Downloading SDL2 $SDL2_TAG source..."
    wget -q "https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_TAG}/SDL2-${SDL2_TAG}.tar.gz"
    tar xf "SDL2-${SDL2_TAG}.tar.gz"
    mv "SDL2-${SDL2_TAG}" sdl2-src
    rm -f "SDL2-${SDL2_TAG}.tar.gz"
else
    echo "==> sdl2-src already present, skipping download"
fi

if [ ! -f sdl2-android/lib/libSDL2.so ]; then
    echo "==> Building SDL2 shared library for Android..."
    cmake -B build-sdl2 \
        -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK/build/cmake/android.toolchain.cmake" \
        -DANDROID_ABI="$ABI" \
        -DANDROID_PLATFORM="android-$ANDROID_API" \
        -DSDL_SHARED=ON \
        -DSDL_STATIC=OFF \
        -DCMAKE_INSTALL_PREFIX="$PROJECT_ROOT/sdl2-android" \
        -DCMAKE_BUILD_TYPE=Release \
        sdl2-src
    cmake --build build-sdl2 -j"$(nproc)"
    cmake --install build-sdl2
else
    echo "==> SDL2 Android shared lib already built, skipping"
fi

echo "==> Configuring native library build..."
cmake -B "$BUILD_DIR" \
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
    -DVCPKG_TARGET_TRIPLET=arm64-android \
    -DVCPKG_CMAKE_SYSTEM_VERSION="$ANDROID_API" \
    -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE="$ANDROID_NDK/build/cmake/android.toolchain.cmake" \
    -DANDROID_ABI="$ABI" \
    -DANDROID_PLATFORM="android-$ANDROID_API" \
    -DSDL2_DIR="$PROJECT_ROOT/sdl2-android/lib/cmake/SDL2" \
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="$PROJECT_ROOT/$BUILD_DIR/result" \
    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY="$PROJECT_ROOT/$BUILD_DIR/result" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

echo "==> Building native library..."
cmake --build "$BUILD_DIR" -j"$(nproc)"

echo "==> Collecting shared libraries..."
JNILIBS="android/app/src/main/jniLibs/$ABI"
mkdir -p "$JNILIBS"
cp -L "$BUILD_DIR/result/libmain.so" "$JNILIBS/"
cp -L sdl2-android/lib/libSDL2*.so "$JNILIBS/"
for so in "$BUILD_DIR"/vcpkg_installed/arm64-android/lib/*.so; do
    [ -f "$so" ] && cp -L "$so" "$JNILIBS/"
done

echo "==> Bundling mod resources into APK assets..."
rm -rf android/app/src/main/assets/resources
if [ -d resources ] && [ "$(ls -A resources 2>/dev/null)" ]; then
    mkdir -p android/app/src/main/assets
    cp -r resources android/app/src/main/assets/resources
fi

echo "==> Building APK with Gradle..."
cd android
GRADLE_TASK="assembleDebug"
[ "$BUILD_TYPE" = "Release" ] && GRADLE_TASK="assembleRelease"
./gradlew "$GRADLE_TASK" -PusePrebuiltLibs=true
cd "$PROJECT_ROOT"

echo "==> Collecting APK..."
APK_SRC=$(find android -name "*.apk" -path "*/$BUILD_TYPE_LOWER/*" | head -1)
if [ -n "$APK_SRC" ]; then
    cp "$APK_SRC" "$BUILD_DIR/result/"
    echo "==> APK copied to: $BUILD_DIR/result/$(basename "$APK_SRC")"
else
    echo "==> WARNING: could not locate built APK automatically; check android/app/build/outputs/apk/"
fi

echo "==> Done. Output in: $BUILD_DIR/result/"
