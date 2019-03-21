from Sources import NodeTelem_pb2


breakNodeData = NodeTelem_pb2.brakeNodeData()
try:
    with open("nodeTelem.pb", "rb") as infile:
        breakNodeData.ParseFromString(infile.read())
except:
    print("failed to open nodeTelem.pb")
    raise

print(breakNodeData)
