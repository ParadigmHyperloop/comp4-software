"""
@brief Hyperloop team mock receiver program
@author Harmjan Treep <harmjan@boringcompany.com>

Subject to the existing rights of third parties, THE BORING COMPANY
is the owner of the copyright in this work and no portion thereof is
to be copied, reproduced or communicated to any person without
written permission.
"""

from argparse import ArgumentParser
from enum import IntEnum
import os
import struct
import socket
import select
from time import time
from collections import deque
from statistics import mean

class MockServer:
    """An example server to receive, print and store values send by an Hyperloop pod"""

    format = ">BB7iI"
    column_separator = ","

    status_map = {
            0: "Fault",
            1: "Safe to approach",
            2: "Ready to launch",
            3: "Launching", 
            4: "Coasting",
            5: "Braking",
            6: "Crawling"
        }

    team_map = {
            0: "SpaceX",
            1: "The Boring Company",
            2: "Tesla",
            3: "Neuralink",
            # Add your own team here using:
            # team_id: team_name
        }

    message_times = deque()

    team_id = -1
    status = -1

    acceleration = 0
    position = 0
    velocity = 0
    battery_voltage = 0
    battery_current = 0
    battery_temperature = 0
    pod_temperature = 0
    stripe_count = 0

    highest_velocity = 0

    packet_error = ""

    def __init__(self, host, port, tube_length):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.socket.bind((host,port))

        self.file = None

        self.tube_length = tube_length

    def start_recording(self, output_filename, print_header):
        """Start recording received samples to a file"""

        if self.file is not None:
            raise Exception("Mock server was already recording")

        self.file = open(output_filename, "a")
        # Add the column headers if requested
        if print_header:
            columns = [
                    "Time (seconds since GNU Epoch)",
                    "Team id",
                    "Status",
                    "Acceleration (cm/s/s)",
                    "Position (cm)",
                    "Velocity (cm/s)",
                    "Battery voltage (mV)",
                    "Battery current (mA)",
                    "Battery temperature (d°C)",
                    "Pod temperature (d°C)",
                    "Stripe count"
                ]
            self.file.write(self.column_separator.join(map(lambda column_title: "\""+column_title+"\"", columns)))
            self.file.write("\n")

    def print_screen(self):
        """Print received information to the screen

        This function takes the received information in this class and prints
        it to the terminal.
        """

        # Clear the screen
        os.system('cls' if os.name=='nt' else 'clear')

        # Print the team name, if it's not in the team_map print the id
        print("Team {}".format(self.team_map.get(self.team_id, str(self.team_id))))
        print("Pod status: {}".format(self.status_map.get(self.status, "Unknow status {}".format(self.status))))
        print()
        print("Pod position:     {:>7.2f} meter from start of tube ({:3.0f}%)".format(self.position/100, self.position / self.tube_length * 100))
        print("Pod velocity:     {:>7.2f} meter/second (highest velocity {:.2f} meter/second)".format(self.velocity/100, self.highest_velocity/100))
        print("Pod acceleration: {:>7.2f} meter/second/second".format(self.acceleration/100))

        print()
        if len(self.message_times) < 2:
            print("No message received yet")
        elif self.message_times[-1] < time() - 1:
            print("No message received in {:.1f} seconds".format(time()-self.message_times[-1]))
        else:
            # Calculate the average period between all saved 
            send_periods = []
            for i in range(len(self.message_times)-1):
                send_periods.append(self.message_times[i+1] - self.message_times[i])
            message_frequency = 1/mean(send_periods)
            print("Message frequency: {:.2f}hz".format(message_frequency))
            if message_frequency < 10:
                print("\tFrequency should be higher than 10hz!")
            if message_frequency > 50:
                print("\tFrequency should be lower than 50hz!")

        if self.packet_error != "":
            print()
            print(self.packet_error)

    def handle_packet(self, data):
        """Handle the data in a received packet

        This function extracts the data send to the server, stores it into this class and if
        configured to write it to file it writes the data to file.
        """

        if len(data) > struct.calcsize(self.format):
            # Since we only receive upto struct.calcsize(self.format)+1 bytes don't
            # we know the exact received packet length so don't print the length in
            # the error message.
            self.packet_error = "Payload length too long, should be {} bytes".format(struct.calcsize(self.format))
            return
        elif len(data) < struct.calcsize(self.format):
            self.packet_error = "Payload length too short, current length is {} bytes but should be {} bytes".format(len(data), struct.calcsize(self.format))
            return
        else:
            self.packet_error = ""

        self.team_id, self.status, self.acceleration, self.position, self.velocity, self.battery_voltage, self.battery_current, self.battery_temperature, self.pod_temperature, self.stripe_count = struct.unpack(self.format, data)

        self.highest_velocity = max(self.highest_velocity, self.velocity)

        if self.file != None:
            columns = [time(), self.team_id, self.status, self.acceleration, self.position, self.velocity, self.battery_voltage, self.battery_current, self.battery_temperature, self.pod_temperature, self.stripe_count]
            # Convert the values to a semicolon separated line
            line = self.column_separator.join(map(str, columns))
            # Write the line to the file
            self.file.write("{}\n".format(line))

    def run(self):
        """Run the server

        This function doesn't return, if a message is received the internal values are
        updated and the values are saved to file if the start_recording function has
        been called.

        The screen is updated at at least 10 fps when no messages are being received and
        the screen is updated for each message upto a 24 fps update rate.
        """

        previous_draw_time = time() - 1
        while True:
            # Sleep until the socket is ready or the timeout expires
            ready_sockets, _, _ = select.select([self.socket], [], [], 0.1)

            new_time = time()

            if len(ready_sockets) > 0:
                received_data = ready_sockets[0].recv(struct.calcsize(self.format)+1)
                self.handle_packet(received_data)

                self.message_times.append(new_time)
                while len(self.message_times) > 10:
                    self.message_times.popleft()

            # Don't redraw the screen more often than at 24fps to prevent the text from flashing
            if previous_draw_time < new_time - 1/24:
                self.print_screen()
                previous_draw_time = new_time

if __name__ == "__main__":
    # Parse some arguments
    parser = ArgumentParser(description="Mock the receiving server for the Hyperloop competition")
    parser.add_argument("--host", default="", help="The host to accept packets from, if set to localhost the server only accepts packets from localhost. The default option allows packets from all sources.")
    parser.add_argument("--port", type=int, default=3000, help="The port to list on for packets")
    parser.add_argument("--tube_length", type=int, default=125000, help="The length of the tube in centimeters")
    parser.add_argument("--output_filename", help="The filename to store received data in")
    parser.add_argument("--print_header", action="store_true", help="If at the start of the file a header should be added explaining the columns")

    args = parser.parse_args()

    mock_server = MockServer(args.host, args.port, args.tube_length)
    if args.output_filename != None:
        mock_server.start_recording(args.output_filename, args.print_header)

    mock_server.run()