import json
import os
import errno


# TODO: add db stuff in future?
class FlaskConfig:

    def __init__(self, data=None, json_name='flask_config', db=None):
        """data - dict of config options to be written to JSON file"""

        self.json = json_name
        self.db = db
        self.path = 'localStorage/'

        try:
            os.mkdir(self.path)
        except OSError as e:
            if e.errno != errno.EEXIST:
                raise e

        with open(self.path + self.json + '.json', 'w') as outfile:
            json.dump(data, outfile)

    def read_config(self):
        with open(self.path + self.json + '.json', 'r') as infile:
            config = json.load(infile)

        return config

    def update_config(self, data):
        """
         - Updates current config values to match config provided
         - Appends any new values to the config
         - Does not alter values in the config.json that are not contained
           in input config
        """

        # Load current config
        current_config = self.read_config()

        # Append new & overlaping config values to current config
        for new_key, new_val in data.items():
            current_config[new_key] = new_val

        # Dump back to JSON file
        with open(self.path + self.json + '.json', 'w') as outfile:
            json.dump(data, outfile)

    def delete(self):
        try:
            os.remove(self.path + self.json + '.json')
        except Exception as e:
            print("failed to remove config file")
            raise e


if __name__ == '__main__':
    config = dict()
    config['key1'] = 'value1'
    config['key2'] = 'value2'
    config['key3'] = ['value3.1', 'value3.2', 'value3.3']
    config['key4'] = 'value4'

    # test read_config()
    local_config = FlaskConfig(config)
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
