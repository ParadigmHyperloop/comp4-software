import struct

def spacex_packet(pod_data, start_time):
    team_id = 0

    if pod_data is None:
        return struct.pack(">BB7iI", team_id, 0, 0, 0, 0, 0, 0, 0, 0, 0)

    podState = pod_data['podState']
    position = pod_data['podPosition']
    velocity = pod_data['podVelocity']

    if start_time is not None:
        current_time = time()
        accel_time = current_time - start_time
        acceleration = velocity/accel_time
    else:
        acceleration = 0
        
    if podState == 0:
        status = 1
    elif podState == 1:
        status = 1
    elif podState == 2:
        status = 2
    elif podState == 3:
        status = 2
    elif podState == 4:
        status = 2
    elif podState == 5:
        status = 3
    elif podState == 6:
        status = 4
    elif podState == 7:
        status = 5
    elif podState == 8:
        status = 0
    else:
        status = 0
        
    return struct.pack(">BB7iI", team_id, int(status), int(acceleration), 
                       int(position), int(velocity), 0, 0, 0, 0, 0)
