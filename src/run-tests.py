import subprocess
import itertools
import sys

program = './main' if len(sys.argv) == 1 else sys.argv[1]

for n in [6, 7, 8, 9, 10]:
    for b, i in list(itertools.product([1, 2, 3, 4], [1, 2])):
        test_file = '../samples/test_cases/n{0}/n{0}b{1}_{2}.txt'.format(n, b, i)
        print("solving {}".format(test_file))
        process = subprocess.run([program, test_file])

