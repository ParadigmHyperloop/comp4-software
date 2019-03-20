```
├── nodeTelem.pb                # Local saved serialized protobuf for read/ write samples
├── ParseFromFile.py            # Parse sample Proto packet from file
├── protobufTesting.py          # Sample how to generate Protobuf packet
├── ReceiveFromSocket.py        # Sample file to listen for messages form pod and deserialize them
├── SendToSocket.py             # Sample creating, sending and writing packet
├── Sources/              
    ├── NodeTelem.proto         # Sample break node proto file
    ├── NodeTelem_pb2.py        # Magic Protobuf file generated from NodeTelem.proto
    ├── compileme.sh            # Bash script to compile any `*.proto` file in the folder, for Python 
```

### Protocol buffer
- Edit the `NodeTelem.proto` file to reflect changes on the Protobuf packet
- Compile the `.proto` file
    - just run the `compileme.sh`
- Launch the `ReceiveFromSocket.py` script, it'll wait for packets
- execute the `SendToSocket.py` script, it'l generate and send a protobuf packet 
    - to edit/change its behavior, check the inline comments 

### `SendToSocket.py`
Generates a Protobuf packet, sends it through UDP socket, and writes content to file

Notes:
- `createBreakTelem()` would have to be edited to reflect changes in the `.proto` files.

### `ProtobufTesting.py`
Demonstrates syntax to create Protobuf packet.

### `ReceiveFromSocket.py`
Opens UDP socket and accepts incoming packets, printing out data received.

### `ParseFromFile.py`
Parses content of a file according to `.proto` structure

### `NodeTelem.proto`
Structure of a `.proto` file, replace accordingly, and compile it to get a `_pb2.py` file (or one for C, or any other supported language). 
