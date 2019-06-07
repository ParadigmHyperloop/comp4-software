#!/bin/bash

:'
Requirments: 
 	git
	 	sudo apt-get install git
	python3
		sudo apt-get install python3
	pip
		sudo apt install python-pip
	virtualenv
		sudo apt-get install python-virtualenv
	Protobuf
		https://github.com/protocolbuffers/protobuf/releases/tag/v3.8.0
		extract and enter folder
		./configure
		make
		make check
 		sudo make install
		sudo ldconfig

You will also likely have to:
	chmod u+x paradigm.sh

Search for your systems list of "startup applications" and add this script to it
'

file="/comp4-software/"
if [ ! -f "$file"]
then 
	continue
else
	echo "setting up project"
	git clone https://github.com/ParadigmHyperloop/comp4-software.git
	virtualenv -p python3 comp4-software/OnLand
	touch comp4-software/OnLand/Paradigm_pb2.py
fi

echo "updating project"
git pull
source comp4-software/OnLand/bin/activate
./comp4-software/ProtoBufs/UpdateProtos.sh
comp4-software/OnLand/bin/pip install -r comp4-software/OnLand/requirements.txt

echo "running project"
cd comp4-software/OnLand
python3 paradigm.py
