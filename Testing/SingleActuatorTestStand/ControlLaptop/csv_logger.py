import os
import csv


class CSVLogger:

    # create a general directory to hold all test data
    def make_test_data_dir(self):
        current_directory = os.path.dirname(os.path.realpath(__file__))
        os.chdir(current_directory)
        if not os.path.isdir('TestData'):
            os.mkdir('TestData')
        os.chdir('TestData')

    # create a specific directory for the current test
    def make_test_dir(self):
        if not os.path.isdir(f'test_{self.test_num}'):
            os.mkdir(f'test_{self.test_num}')
        os.chdir(f'test_{self.test_num}')
        self.test_dir = os.getcwd()

    # creates a .txt file and logs the user's notes inside of it
    def log_notes(self, notes):
        notes_path = self.test_dir + f'/test_{self.test_num}_notes.txt'
        with open(notes_path, 'w') as notefile:
            for note in notes:
                notefile.write(note + '\n')

    # creates a .csv file and logs all test data inside of it
    def log_as_csv(self, results):
        csv_path = self.test_dir + '/test_{}.csv'.format(self.test_num)
        with open(csv_path, 'w', newline='\n') as csvfile:
            writer = csv.writer(csvfile, delimiter=',')
            writer.writerow(results['columns'])
            for row in results['values']:
                writer.writerow(row)

    # deletes the test's data and note files
    def delete_test_files(self):
        if self.test_dir:
            os.remove(self.test_dir)
