from .Sources import NodeTelem_pb2


message = NodeTelem_pb2.brakeNodeData()
message.id = 1
message.state = 5

message.sol1 = 2
message.sol2 = 3
message.sol3 = 4
message.sol4 = 5
message.sol5 = 6
message.sol6 = 7

message.hp = 8
message.lp1 = 9
message.lp2 = 10
message.lp3 = 11
message.lp4 = 12

message.temp = 13

print(message.ByteSize())
message.SerializeToString()
print(message.ByteSize())
