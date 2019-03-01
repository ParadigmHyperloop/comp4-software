```
├── telemetry.py                # Sample file to listen for messages form pod and deserialize them
├── NodeTelem_pb2.py            # Magic Protobuf file generated from NodeTelem.proto
├── protobufSample              
    ├── nodeTelem.pb            # Local saved serialized protobuf for read/ write samples
    ├── NodeTelem.proto         # Sample break node proto file
    ├── NodeTelem_pb2.py        # Magic Protobuf file generated from NodeTelem.proto
    ├── readTelem.py            # Sample reading telem from .pb file
    ├── writeTelem.py           # Sample writing telem to .pb file
    ├── sendTestTelemetry       # Send sample socket notification to telemetry.py
```
