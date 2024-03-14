#!/usr/bin/env python3
import sys
import numpy as np


unit = 'GiB/s'
small = '384000B'
large = '1.144GiB'
keys = [small, large]

def pad(s, n):
    return s + ' ' * max(0, n - len(s))


def show_table(rows):
    ws = [0] * len(rows[0])
    for row in rows:
        for i, c in enumerate(row):
            if len(c) > ws[i]:
                ws[i] = len(c)
    hr = '-' * (sum(ws) + len(ws) * 2)
    for i, row in enumerate(rows):
        if i < 2:
            print(hr)
        fields = []
        for i, c in enumerate(row):
            fields.append(pad(c, ws[i]))
        print('  '.join(fields))




def show(groups, name=''):
    if name:
        print(name)
    # print(ks)
    for k in keys:
        xs = np.array(groups[k])
        msg = '%10s : %0.3f'  %(k, np.mean(xs))
        print(msg)
        # print('{:10}: {:0.3}'.format(k, np.mean(xs)))
        # print()


def to_row(groups):
    cols = []
    for k in keys:
        xs = np.array(groups[k])
        cell = '%0.3f'  %(np.mean(xs))
        cols.append(cell)

    return cols


def g(filename):
    # print(filename)
    groups = dict()
    for line in open(filename).readlines():
        # print(line)
        parts = line.strip().split()
        # print(parts)
        # if len(parts)  =
        if parts[0].startswith('bench_allreduce'):
            # print(parts)
            w = parts[5].replace(',', '')
            x = float(parts[7].replace(unit, ''))
            groups.setdefault(w, []).append(x)
        else:
            # print(filename, parts)
            pass

    return groups

th = [
    'local/S', 'local/L',
    'remote/S', 'remote/L',
    'commit',
]

def f(id):
    local = g(f'logs/{id}/1.txt')
    remote = g(f'logs/{id}/2.txt')

    print(id)
    cols = to_row(local) + to_row(remote)
    # line= ' '.join(cols)
    # print(line)
    # print(' ',jo)
    # show(local, 'local')
    # show(remote, 'remote')
    return cols + [id]

def main(args):
    rows = [th]
    for id in args:
        cols = f(id)
        rows.append(cols)
    show_table(rows)
    # print(args)


main(sys.argv[1:])
