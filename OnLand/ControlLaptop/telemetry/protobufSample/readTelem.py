import NodeTelem_pb2
from google.protobuf.json_format import MessageToDict

def readProto(filename):
    breakNodeData = NodeTelem_pb2.brakeNodeData()

    try:
        with open(filename, "rb") as infile:
            breakNodeData.ParseFromString(infile.read())
    except:
        print("error opening", filename)
        raise
        
    return breakNodeData


if '__main__':
    breakNodeData = readProto("nodeTelem.pb")
        
    data = MessageToDict(breakNodeData)
    for key, value in data.items():
        print(key, "-", value)

