import os

# run command


def run(command):
    print(command)
    os.system('/bin/bash -c \'{0}\''.format(command))
def run_diff_seed(command, cur_seed):
    print(command)
    os.system('/bin/bash -c \'{0}\' > seed_{1}_log'.format(command,cur_seed))

# run modes
modes = ['gdb', 'valgrind', 'vgdb', 'gprof']
mode_prefixes = {
    None: '',
    'gprof': '',
    'gdb': 'gdb --args',
    'valgrind': 'valgrind',
    'vgdb': 'valgrind --vgdb-error=0'
}

# benchmarks


class Benchmark:
    def __init__(self, full_name, abbr_name):
        self.full_name = full_name
        self.abbr_name = abbr_name

    def __repr__(self):
        return self.full_name


class Benchmarks:
    def __init__(self):
        self.__bms = []
        self.__bm_sets = dict()

    def add_single(self, full_name, abbr_name):
        self.__bms.append(
            Benchmark(full_name, abbr_name))

    def add(self, full_name_pat, abbr_name_pat, ids):
        for id in ids:
            self.__bms.append(
                Benchmark(full_name_pat.format(id), abbr_name_pat.format(id)))

    def add_with_id_ailas(self, full_name_pat, abbr_name_pat, ids, id_alias):
        for (id, ali) in zip(ids, id_alias):        
            self.__bms.append(
                Benchmark(full_name_pat.format(ali), abbr_name_pat.format(id)))

    def get_bm(self, name):
        # TODO: can be faster by using hash table
        for bm in self.__bms:
            if name == bm.abbr_name or name == bm.full_name:
                return bm
        print('Error: benchmark', name, 'cannot be found')
        quit()

    def add_set(self, set_name, bm_names):
        bms = []
        for name in bm_names:
            bms.append(self.get_bm(name))
        self.__bm_sets[set_name] = bms

    def get_choices(self):
        choices = []
        for bm in self.__bms:
            choices.append(bm.full_name)
            choices.append(bm.abbr_name)
        for bm_set in self.__bm_sets:
            choices.append(bm_set)
        choices.append('all')
        return choices

    def get_selected(self, names):
        if 'all' in names:
            return self.__bms
        else:
            selected = []
            for name in names:
                if name in self.__bm_sets:
                    selected += self.__bm_sets[name]
                else:
                    selected.append(self.get_bm(name))
            return selected


######################################
# Below is project-dependant setting #
######################################


# all benchmarks
all_benchmarks = Benchmarks()
all_benchmarks.add('test{}.txt', 't{}', ['1', '2', '3','4','5','6','7','8','9','10'])
# all_benchmarks.add_with_id_ailas('test{}.txt', 't{}', ['1', '2', '3'], ['1', '2', '3'])



all_benchmarks.add_set('all', ['t1', 't2','t3','t4','t5','t6','t7','t8','t9','t10'])
