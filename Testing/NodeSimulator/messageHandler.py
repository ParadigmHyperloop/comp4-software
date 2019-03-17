from abc import ABCMeta, abstractmethod
from Protobuf import PodTelem_pb2

class MessageHandler(metaclass=ABCMeta):

    def __init__(self):
        self.values = dict()
        self.values['state'] = None

    @abstractmethod
    def parse_flight_computer_message(self, serialized_message):
        return self.values


class BrakeNodeProtoHandler(MessageHandler):

    def parse_flight_computer_message(self, serialized_message):
        data = PodTelem_pb2.telemetry()
        data.parseFromString(serialized_message)
        return data.

