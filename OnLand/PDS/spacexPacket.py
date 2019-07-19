import struct

def spacex_packet(pod_data):
    team_id = 'Paradigm'
    podState = pod_data['podState']
    acceleration = pod_data['podAccelerationX']
    position = pod_data['podPosition']
    velocity = pod_data['podVelocity']
        
    if podState == 0:
        status = 1
    elif podState == 1:
        status = 1
    elif podState == 2:
        status = 1
    elif podState == 3:
        status = 1
    elif podState == 4:
        status = 2
    elif podState == 5:
        status = 3
    elif podState == 6:
        status = 4
    elif podState == 7:
        status = 5
    elif podState == 8:
        status = 1
    elif podState == 9:
        status = 0
    else:
        status = 0
        
    return struct.pack(">BB7iI", team_id, int(status), int(acceleration), 
                       int(position), int(velocity), 0, 0, 0, 0, 0)
