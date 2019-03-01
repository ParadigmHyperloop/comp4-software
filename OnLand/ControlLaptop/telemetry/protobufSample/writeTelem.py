import NodeTelem_pb2

# Create a Break Node Telem Object
def createBreakTelem(telem):
    telem.id = 1
    telem.state = 5
    
    telem.sol1 = 2;
    telem.sol2 = 3;
    telem.sol3 = 4;
    telem.sol4 = 5;
    telem.sol5 = 6;
    telem.sol6 = 7;
    
    telem.hp  = 8;
    telem.lp1 = 9;
    telem.lp2 = 10;
    telem.lp3 = 11;
    telem.lp4 = 12;
  
    telem.temp = 13;
    
    return telem


if "__main__":
    breakNodeData = NodeTelem_pb2.brakeNodeData()
    try:
        with open("nodeTelem.pb", "rb") as infile:
            breakNodeData.ParseFromString(infile.read())
    except:
        print("failed to open nodeTelem.pb")
        raise
    
    # Add telem object
    createBreakTelem(breakNodeData)
    with open("nodeTelem.pb", "wb") as outfile:
        outfile.write(breakNodeData.SerializeToString())
    