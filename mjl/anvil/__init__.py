import yaml


class Loader: 

    def __init__(self, filename):
        # body of the constructor
        """Reads yaml form file and verifies it fits anvil format"""

        with open(filename, 'r') as stream:
            try:
                print(yaml.safe_load(stream))
            except yaml.YAMLError as exc:
                print(exc)