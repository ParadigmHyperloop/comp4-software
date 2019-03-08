import json

# TODO: add db stuff in future?
class FlaskConfig:
    
    def __init__(self, json, db):
        self.json = json
        self.db = db
    
    
    def read_config(self):
        """
        pre - assumes a config.json exists
        returns - dict of json config
        """
        
        # Open config and read JSON
        with open('flaskConfig.json', 'r') as infile:
            config = json.load(infile)
    
        return config
    
    
    def new_config(self, config):
        """
        config - dict of config options to be written to JSON file
        
         - Ignores current config and overwrites it entirely with config provided
        """
                        
        # Dump to JSON file
        with open('flaskConfig.json', 'w') as outfile:
            json.dump(config, outfile)
    
    
    def update_config(self, config):
        """
        pre - assumes a config.json exists
        config - dict of config options to be written to JSON file
        
         - Updates current config values to match config provided
         - Appends any new values to the config
         - Does not alter values in the config.json that are not contained 
           in input config
        """
        
        # Load current config
        current_config = self.read_config()
        
        # Append new & overlaping config values to current config
        for new_key, new_val in config.items():
            current_config[new_key] = new_val
                        
        # Dump back to JSON file
        with open('flaskConfig.json', 'w') as outfile:
            json.dump(config, outfile)
            
    json = False
    db = False

if '__main__':
    config = dict()
    config['key1'] = 'value1'
    config['key2'] = 'value2'
    config['key3'] = ['value3.1', 'value3.2', 'value3.3']
    config['key4'] = 'value4'

    # test new_config()
    local_config = FlaskConfig()
    local_config.new_config(config)
    assert(local_config.read_config() == config)
    
    # test update_config
    old_config = local_config.read_config()
    config['key4'] = 'value4.0'
    config['key5'] = 'value5'
    local_config.update_config(config)
    assert(local_config.read_config() == config)
    assert(local_config.read_config()['key4'] != old_config['key4'])
    assert(local_config.read_config()['key3'] == old_config['key3'])