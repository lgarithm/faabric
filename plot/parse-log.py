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
        cell = '%0.3f' % (np.mean(xs))
        cols.append(cell)

    return cols


def g(filename):
    # print(filename)
    groups = dict()
    for line in open(filename):
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



def parse_ids(filename = 'ids.txt'):
    id_names = []
    for line in open(filename):
        if line.startswith('#'):
            continue
        parts = line.strip().split()
        id = parts[0]
        name = parts[2]
        id_names.append((id, name))
    return dict(id_names)



def add_ratio(rows):
    names = parse_ids()
    row0, tail = rows[0], rows[1:]
    new_rows = []
    for row in tail:
        cols = []
        for x, b in zip(row, row0):
            if b == 'mpi':
                cols.append(x + ' # ' + names[x])
            else:
                x = float(x)
                b = float(b)
                cols.append('%.3f (%.2f)' % (x, x / b))
        new_rows.append(cols)

    return [row0] + new_rows


def main(args):
    args = ['mpi'] + args
    rows = []
    for id in args:
        cols = f(id)
        rows.append(cols)

    # show_table([th] + rows)
    show_table([th] + add_ratio(rows))
    # print(args)


main(sys.argv[1:])
