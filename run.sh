#!/bin/bash

# Exit when any command fails
set -e

help() {
  echo "USAGE: $0 [options]"
  echo "OPTIONS:"
  echo "  -r  release  (no argument)  Use Release as the CMake build type"
  exit 1
}

# Parse command-line arguments
while getopts ghr flag
do
  case "${flag}" in
    g) generate=true ;;
    h) help ;;
    r) release=true ;;
  esac
done

echo "
#######################################
# Setting up...                       #
#######################################
"
if [[ "$(pwd)" == */scripts ]]; then
  cd ..
fi

if [[ $release == true ]]; then
  buildType="Release"
else
  buildType="Debug"
fi
echo "-- Using build type [${buildType}]"

workingDirectory="$(pwd)"
echo "-- Working from directory [${workingDirectory}]"

sourceDirectory="${workingDirectory}"
echo "-- Using CMake source directory [${sourceDirectory}]"

buildDirectory="${workingDirectory}/build/$(echo "${buildType}" | awk '{print tolower($0)}')"
echo "-- Using CMake build directory [${buildDirectory}]"

binDirectory="${workingDirectory}/bin/$(echo "${buildType}" | awk '{print tolower($0)}')"
echo "-- Using CMake output directory [${binDirectory}]"

if [[ ! -d "${buildDirectory}" ]]; then
  generate=true
  if which ninja >/dev/null; then
    generator="Ninja"
    echo "-- Using CMake generator [${generator}]"
  fi
fi

if [[ "${generate}" == true ]]; then
  echo "
#######################################
# Generating build files...           #
#######################################
"
  command="cmake"
  command+=" -D CMAKE_EXPORT_COMPILE_COMMANDS=1"
  command+=" -D CMAKE_BUILD_TYPE=${buildType}"
  command+=" -S ${sourceDirectory}"
  command+=" -B ${buildDirectory}"
  if [[ -n "${generator}" ]]; then
     command+=" -G \"${generator}\""
  fi
  eval "${command}"
fi

# Symlink compile commands to make it work with Clang LSP (Neovim), regardless of build type
compileCommandsFile="${buildDirectory}/compile_commands.json"
echo "-- Symlinked [$(eval ln -svf "${compileCommandsFile}" "${sourceDirectory}")]"

echo "
#######################################
# Building binary...                  #
#######################################
"
cmake --build "${buildDirectory}" --config "${buildType}"

echo "
#######################################
# Running binary...                   #
#######################################
"
cd "${binDirectory}"
chmod +x "2d-game"
./2d-game
