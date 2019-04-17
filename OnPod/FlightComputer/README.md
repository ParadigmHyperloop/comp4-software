## Unit Tests
Unit testing is done through the [Catch](https://github.com/catchorg/Catch2) Library. 

[Catch Getting Started](https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md#top)

### Updating Protobuf Packages
To update the proto packet, navigate to the comp4-software/ProtoBufs folder in your terminal
Execute the UpdateProtos.sh scripts
    
    $ ./UpdateProtos.sh

This will run the compiler and update all proto files in the project

### Running Tests
Open a terminal
⋅⋅* Navigate to FlightComputer > tests 
⋅⋅* run command "make"

### Adding Test File
⋅⋅* See test example in FlightComputer > examples
⋅⋅* Add new file name to FlightComputer>tests>makeFile (see comments)

