#!/bin/bash

PWD="$(pwd)"
cd ..
cd ..
BASE_DIR="$(pwd)"
ARDUPI_DIR="$BASE_DIR/arduPi"
ARDUPIAPI_DIR="$BASE_DIR/arduPi-api"
EXAMPLES_DIR="$BASE_DIR/examples/LoRa"
LIBRARY_DIR="$BASE_DIR/libraries/arduPiLoRa"


#compile file
cd code/LoRa
file="./$1"

  if [ -e $file ]; then
    if [ "$1" != "" ]; then
    echo "Compiling file..."
       
    g++ -lrt -lpthread -lstdc++ "$1" \
      "$LIBRARY_DIR/arduPiLoRa.o" \
      "$ARDUPIAPI_DIR/arduPiUART.o" \
      "$ARDUPIAPI_DIR/arduPiUtils.o" \
      "$ARDUPIAPI_DIR/arduPiMultiprotocol.o" \
      "$ARDUPI_DIR/arduPi.o" \
      -I"$ARDUPI_DIR" \
      -I"$ARDUPIAPI_DIR" \
      -I"$LIBRARY_DIR" \
      -o "$1_exe" 
    else
      echo "---------------HELP------------------"
      echo "Compiling: ./mak.sh filetocompile.cpp"
      echo "-------------------------------------"
    fi
  else
    echo "ERROR No such file or directory: $file"
  fi

 exit 0
