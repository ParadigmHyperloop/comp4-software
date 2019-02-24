from database import Database
import math
from random import randint

def main():
    someDatabase = Database("localhost", 8086, "classtest")

    print("Press enter to log data...")
    while True:
        if input() == "q":
            someDatabase.close()
            print("Test finished")
            break;
        value = randint(0,9999)
        someDatabase.log("sensor_data", value)
        print("Value of sensor_date value " + str(value))


main()
