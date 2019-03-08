from abc import ABCMeta, abstractmethod


class MessageParser(metaclass=ABCMeta):

    def __init__(self):
        self.values = dict()
        self.values['state'] = None

    @abstractmethod
    def parseMessage(self):
        return self.values


class ProtoParser(MessageParser):

    def parseMessage(self, serialized_message):

