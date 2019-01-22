# Documentation for examples


### `FlaskTableTesting.py`
Example for how to generate a table that follows the CSS attributes from Bootstrap.

Connects to DB to retrieve names of sensors, puts sensor name only on each row. JS then picks it up when rendering to add proper classes to each row/columns to allow for dynamic changes.

### `dbExampleWorking.py`
Demonstrates how to connect to Influx from Python, and adds dummy data. Refer to inline comments.

### `ProtoEmit.py`
2 available functions, `genNonProto` and `genProtoPacket`. Connects to UDP socket and sends packet through it.


Notes:
- `genProtoPacket` would have to be edited to reflect changes in the `.proto` files.

### `ProtobufTesting.py`
Demonstrates syntax to create Protobuf packet.

### `protoReceive.py`
Opens UDP socket and accepts incoming packets, printing out data received.

### `protoTest.proto`
Structure of a `.proto` file, replace accordingly, and compile it to get a `_pb2.py` file (or one for C, or any other supported language). 
