#!/usr/bin/env python3

import argparse
import os
import datetime
from run_base import *

# constants
binary = 'CTRouter'

# argparse
parser = argparse.ArgumentParser()
parser.add_argument('benchmarks', choices=all_benchmarks.get_choices(), nargs='+', metavar='BENCHMARK',
                    help='Choices are ' + ', '.join(all_benchmarks.get_choices()))
parser.add_argument('-p', '--benchmark_path',default="../benchmark")
parser.add_argument('--topo', choices=['NS', 'CL', 'RGM'], default=['NS'])
# parser.add_argument('--metric', type=int ,default=2) # L1 or L2 distance
# parser.add_argument('--refine',dest='refine', action='store_true')#apply refinement or not

parser.add_argument('--outfile', type=str,default="CTR.sol")

args = parser.parse_args()


# seleted benchmarks
bms = all_benchmarks.get_selected(args.benchmarks)
bm_path = args.benchmark_path
if bm_path is None:
    bm_path = os.environ.get('BENCHMARK_PATH')
    if bm_path is None:
        print('Benchmark path is not specified.')
        quit()


# run
if True:
    now = datetime.datetime.now()
    log_dir = 'run{:02d}{:02d}'.format(now.month, now.day)

run('mkdir -p {}'.format(log_dir))
print('The following benchmarks will be run: ', bms)


def run_binary():
    run('/usr/bin/time -v ./{0} {1} {2} dummyTask2Out {3}  |& tee {4}.log'.format(
        binary, file_name, args.outfile, args.metric, bm.full_name))

    run('mv *.log  {} 2>/dev/null'.format(bm_log_dir))

for bm in bms:
    bm_log_dir = '{}/{}'.format(log_dir, bm.abbr_name)
    file_name = '{0}/{1}'.format(bm_path, bm.full_name)
    run('mkdir -p {}'.format(bm_log_dir))
    run_binary()
