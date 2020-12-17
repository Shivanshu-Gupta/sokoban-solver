import os
import itertools
for n in [6, 7, 8, 9, 10]:
    os.makedirs('n{}'.format(n), exist_ok=True)
    with open('n{}.txt'.format(n)) as inf:
        lines = inf.readlines()
    b = list(itertools.product([1, 2, 3, 4], [1, 2]))
    boards = [list(group) for k, group in itertools.groupby(lines, lambda x: x == '\n') if not k]
    for (b, i), board in zip(b, boards):
        with open('n{0}/n{0}b{1}_{2}.txt'.format(n, b, i), 'w') as outf:
            for line in board:
                outf.write(line)