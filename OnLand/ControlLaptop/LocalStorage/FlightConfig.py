import json
import os


class FlightConfig:
    _path = 'LocalStorage/'
    _file_name = 'ConfigStorage'
    _file_path = None
    _flight_config_instance = None

    """DO NOT CALL"""
    def __init__(self):
        FlightConfig._flight_config_instance = self

        # If file doesn't exist, create one with the default form values
        self._file_path = 'ControlLaptop/LocalStorage/' + self._file_name + '.json'
        if not os.path.isfile(self._file_path):
            with open(self._file_path, 'w') as outfile:
                json.dump(DEFAULT_FLIGHT_CONFIGURATION, outfile)

    def read_config(self):
        with open(self._file_path, 'r') as infile:
            config = json.load(infile)

        return config

    @staticmethod
    def get_flight_config_instance():
        if FlightConfig._flight_config_instance is None:
            FlightConfig()
        return FlightConfig._flight_config_instance

    def update_config(self, data):
        """
         - Updates current config values to match config provided
         - Appends any new values to the config
         - Does not alter values in the config.json that are not contained
           in input config
        """

        current_config = self.read_config()

        # Append new & overlaping config values to current config
        for new_key, new_val in data.items():
            current_config[new_key] = new_val

        # Dump back to JSON file
        with open(self._file_path, 'w') as outfile:
            json.dump(data, outfile)

    def delete(self):
        try:
            os.remove(self._path + self._file_name + '.json')
        except Exception as e:
            print("failed to remove config file")
            raise e


# Default Sample Configuration
DEFAULT_FLIGHT_CONFIGURATION = {
    'retrieval_timeout': 98,
    'max_flight_time': 82,
    'motor_speed': 123,
    'telemetry_port': 1234,
    'command_port': 4321,
    'flight_length': 60,
    'heartbeat_timeout': 2000,
    'pod_address': '192.168.5.4',
    'pod_driver': False,
}

if __name__ == '__main__':
    config = dict()
    config['key1'] = 'value1'
    config['key2'] = 'value2'
    config['key3'] = ['value3.1', 'value3.2', 'value3.3']
    config['key4'] = 'value4'

    # test read_config()
    local_config = FlightConfig(config)
    assert(local_config.read_config() == config)

    # test update_config
    old_config = local_config.read_config()
    config['key4'] = 'value4.0'
    config['key5'] = 'value5'
    local_config.update_config(config)
    assert(local_config.read_config() == config)
    assert(local_config.read_config()['key4'] != old_config['key4'])
    assert(local_config.read_config()['key3'] == old_config['key3'])

    # test delete()
    local_config.delete()
