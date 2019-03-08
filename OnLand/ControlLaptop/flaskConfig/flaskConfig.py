import json


def read_config():
    with open('flaskConfig.txt', 'r') as infile:
        config = json.load(infile)

    return config


def write_config(config):
    with open('flaskConfig.txt', 'w') as outfile:
        json.dump(config, outfile)


if '__main__':
    config = dict()
    config['key1'] = 'value1'
    config['key2'] = 'value2'
    config['key3'] = ['value3.1', 'value3.2', 'value3.3']
    config['key4'] = 'value4'

    write_config(config)
    assert(read_config() == config)
