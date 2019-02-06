from Examples.WebApp import protoTest_pb2


message = protoTest_pb2.telemetry()
message.podState = 132
message.sensor1Temp = 32
message.sensor2Temp = 32
message.sensor3Temp = 32
message.sensor4Temp = 32
message.sensor5Temp = 32
message.sensor6Temp = 32
message.sensor7Temp = 32
message.sensor8Temp = 32
message.sensor9Temp = 32
message.sensor10Temp = 32

print(message.ByteSize())
message.SerializeToString()
print(message.ByteSize())