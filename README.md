# ClockTreeRouting
Project for CENG4120

Use Hierarchical Clustering to build topology and then apply Deferred-Merge Embedding to assign real routes.


To do hierarchical clustering, ALGLIB library is used.  
Ref: https://www.alglib.net/


## Compile

```bash
$ ./scripts/build.py -o release
```

## Run
```bash
$ cd ClockTreeRouting/run
$ ./CTRouter <input file path> <task1 output filename> <task2 output filename>
```
Or use script to run
```bash
$ cd ClockTreeRouting/run
$ ./run.py <benchmark_name...|all> [option...]
$ ./run.py t1 
$ ./run.py t1 --outfile <filename>
$ ./run.py t1 -p <benchmark path> --outfile filename
```
