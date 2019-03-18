#!/usr/bin/env bash
#
#  This file compiles the .proto descriptor file for the proto packet that is sent from the pod to the PDS.
#  It handles moving the created files to the appropriate folders for the flight computer.

# WARNING it currently does not move the appropriate files for the PDS.


if ! type "protoc" > /dev/null; then
  echo "To use this script you must have the protobuf compiler installed"
  echo "Please see https://developers.google.com/protocol-buffers/ for installation instructions"
  exit
fi

#Fix pycharm default working directory
#cd ../../ProtoBufs

# For any `*.proto` file, will create a folder, along with nested `cpp` & `py` folders, and compile `.proto` file
for f in *.proto
do
    dir=${f%.*}
    mkdir -p $dir/cpp $dir/py
	protoc --cpp_out=$dir/cpp $f
	protoc --python_out=$dir/py $f
done

# Move files accordingly
flightComputer="../OnPod/FlightComputer/include/ProtoBuffer/"
flightComputerSrc="../OnPod/FlightComputer/src/"

PDS="../OnLand/PDS/"
NODESIM="../Testing/NodeSimulator/Protobuf"

# Force copy the files, replacing existing ones
cp -f NodeTelem/cpp/*.pb.h $flightComputer
cp -f NodeTelem/cpp/*.pb.cc $flightComputerSrc

cp -f States/cpp/*.pb.h $flightComputer
cp -f States/cpp/*.pb.cc $flightComputerSrc

cp -f PodTelem/cpp/*.pb.h $flightComputer
cp -f PodTelem/cpp/*.pb.cc $flightComputerSrc


cp -f PodTelem/py/* $PDS
cp -f PodTelem/py/* $NODESIM
cp -f States/py/* $NODESIM

echo "Update Complete"
