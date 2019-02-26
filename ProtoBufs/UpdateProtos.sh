#!/usr/bin/env bash
#
#  This file compiles the .proto descriptor file for the proto packet that is sent from the pod to the PDS.
#  It handels moving the created files to the appropriate folders for the flight computer.

# WARNING it currently does not move the appropriate files for the PDS.


if ! type "protoc" > /dev/null; then
  echo "To use this script you must have the protobuf compiler installed"
  echo "Please see https://developers.google.com/protocol-buffers/ for installation instructions"
  exit
fi

#Fix pycharm defualt working directory
#cd ../../ProtoBufs

# C++
protoc --cpp_out=. NodeTelem.proto

rm ../OnPod/FlightComputer/include/ProtoBuffer/NodeTelem.pb.h
cp NodeTelem.pb.h ../OnPod/FlightComputer/include/ProtoBuffer/
rm ../OnPod/FlightComputer/src/NodeTelem.pb.cc
cp NodeTelem.pb.cc ../OnPod/FlightComputer/src

echo "Update Complete"

# TODO: Python
