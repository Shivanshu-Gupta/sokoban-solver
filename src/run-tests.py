import subprocess
import itertools

for n in [6, 7, 8, 9, 10]:
    for b, i in list(itertools.product([1, 2, 3, 4], [1, 2])):
        test_file = '../samples/test_cases/n{0}/n{0}b{1}_{2}'.format(n, b, i)
        process = subprocess.run(['./main', test_file])

