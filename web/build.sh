#!/bin/bash

SCRIPT_DIR=$(realpath $(dirname $0))
PROJECT_DIR=$(realpath $SCRIPT_DIR/..)
BUILD_DIR=$SCRIPT_DIR/build
BUILD_OUT_DIR=$SCRIPT_DIR/out
BUILD_TYPE=RelWithDebInfo
BUILD_EXAMPLE_OUT_DIR=$SCRIPT_DIR/app_picker_example/src/wasm

# 检查是否传递了 --incremental 参数
INCREMENTAL_BUILD=false
for arg in "$@"; do
  if [ "$arg" == "--incremental" ]; then
    INCREMENTAL_BUILD=true
    break
  fi
done

echo "[*] SCRIPT_DIR: ${SCRIPT_DIR}"
echo "[*] INCREMENTAL_BUILD: ${INCREMENTAL_BUILD}"

# 如果不是增量构建，则删除构建目录
if [ "$INCREMENTAL_BUILD" == "false" ]; then
  rm -rf $BUILD_DIR
fi

# 如果不是增量构建，则执行 emcmake cmake
if [ "$INCREMENTAL_BUILD" == "false" ]; then
  echo "[*] Configuring project with emcmake ..."
  emcmake cmake -G Ninja \
    -B $BUILD_DIR \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DTGFX_BUILD_SVG=ON \
    -DTGFX_BUILD_LAYERS=ON \
    -DEMSCRIPTEN_PTHREADS=ON \
    -DCMAKE_C_FLAGS="-fPIC -pthread -fno-rtti" \
    -DCMAKE_CXX_FLAGS="-fPIC -pthread -fno-rtti" \
    -S $PROJECT_DIR
else
  echo "[*] Using existing build configuration (incremental build)."
fi

echo "[*] Building ..."
cmake \
  --build $BUILD_DIR \
  --config $BUILD_TYPE \
  --target SeatCraftAppPicker \
  -j 8

# 创建输出目录
mkdir -p $BUILD_OUT_DIR
# mkdir -p $BUILD_EXAMPLE_OUT_DIR
# 复制构建产物
cp -f $BUILD_DIR/src/app_picker/SeatCraftAppPicker.* $BUILD_OUT_DIR
# cp -f $BUILD_DIR/src/app_picker/SeatCraftAppPicker.* $BUILD_EXAMPLE_OUT_DIR

echo "[*] Build completed."