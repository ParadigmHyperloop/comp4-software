from database import Database
import math
from random import randint

def main():
    someDatabase = Database("localhost", 8086, "pod-data")
    someDatabase.client.create_retention_policy('some-data', '3d', 3, "pod-data", default=True)

    print("Press enter to log data...")
    while True:
        # Exit test with q
        if input() == "q":

            query = "SELECT time FROM sensor-data"
            result = someDatabase.client.query(query)
            print(result)

            someDatabase.client.close()
            print("\nTest finished\n")
            break;

        # Input some random integer into sensor_data
        value = randint(0,9999)
        measurement = "sensor_data"
        someDatabase.log(measurement, value)
        print("Value of " + measurement + " value " + str(value))


main()
