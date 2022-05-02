#!/usr/bin/env python3
import csv
import string
import subprocess
from subprocess import run

nums = 20
proc = 1

with open('results.csv', 'w', newline='') as csvfile:
    spamwriter = csv.writer(csvfile, delimiter=',',
                            quotechar='|', quoting=csv.QUOTE_MINIMAL)
    spamwriter.writerow(['processors', 'tree-size', 'time'])
    string.ascii_lowercase
    for tree_size in range(1, 26):
        time_spent = 0.0
        experiment_count = 50
        for i in range(experiment_count):
            result = run('time ./test.sh {}'.format(string.ascii_uppercase[:tree_size]),
                         shell=True,
                         check=True,
                         universal_newlines=True,
                         capture_output=True, text=True)

            time_spent += float(str(result.stderr).split('key')[1].split('user')[0])
            print (result.stdout, time_spent)
            # time_spent = float(str(result.stderr).split('\n')[1].split('\t')[1].split('m')[1].split('s')[0])
            # print(result, time_spent)
            # time_spent += float(str(result.stderr).split("\n")[-1])
        spamwriter.writerow([2*tree_size-2, tree_size, time_spent/experiment_count])