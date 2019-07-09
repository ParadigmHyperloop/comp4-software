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

# Get sudo privileges
sudo -s
# Dependencies to check 
dependencies=(git python3 python-pip python-virtualenv wget unzip)

# Install missing dependencies
install_dependency () {
	sudo -s
	apt-get install $@
}

# Check for dependencies
for i in "${dependencies[@]}";
    to_install=()
	do version="$('$i' --version)" &> /dev/null
	if [ [ $version =~ 'command not found' ]];
		echo "Package is NOT installed! Attempting to install it"
		to_install+=("$i")
	else
	    echo "Package is installed!"
	fi;
done

# Separately check for protoc
version="$(protoc --version)" > /dev/null
if [[ $version =~ 'command not found' ]]; 
    echo 'Protobuf is not installed. Attempting to install it.'
    wget https://github.com/protocolbuffers/protobuf/releases/download/v3.8.0/protobuf-cpp-3.8.0.zip
    unzip protobuf-cpp-3.8.0.zip && cd protobuf-3.8.0
	./configure
	make && make check
	sudo make install
	sudo ldconfig
else
    echo 'Protobuf is installed'
fi

file="~/comp4-software/"
if [ ! -f "$file"]
then 
	continue
else
	echo "setting up project"
	cd ~ && git clone https://github.com/ParadigmHyperloop/comp4-software.git
	virtualenv -p python3 comp4-software/OnLand/.virtualenv
	~/comp4-software/OnLand/ProtoBufs/UpdateProtos.sh
fi

# Creates the script that will be ran hourly by cron, and if a manual update is needed you can just run it
echo '
cd ~/comp4-software
git pull
source OnLand/.virtualenv/bin/activate
./ProtoBufs/UpdateProtos.sh
./OnLand/.virtualenv/bin/pip intall -r OnLand/requirements.txt
' >> ~/update.sh
chmod +x ~/update.sh

# create the cron job, that will run hourly
# REMEMBER TO CHANGE THE USER NAME
crontab -l > mycron
echo ' 0 * * * * /home/REPLACE_HERE/update.sh' >> cronjob
crontab cronjob

# Creates the system service that will run the OnLand software on boot
echo '
[Unit]
Description=Paradigm Hyperloop subsystem

[Service]
Type=simple
ExecStart=~/comp4-software/OnLand/.virtualenv/bin/python ~/comp4-software/OnLand/paradigm.py
StandardOutput= inherit

[Install]
WantedBy=multi-user.target
' > /etc/systemd/system/ called paradigm.service