# Aging simulation of Network

## Summary

This is the code to obtain the results of the following paper.

* R. Sekikawa and H. Watanabe, "Structural Changes and Percolation Transition in Networks after Aging Processes", J. Phys. Soc. Jpn. **94**, 044004 (2025).
* [https://doi.org/10.7566/JPSJ.94.044004](https://doi.org/10.7566/JPSJ.94.044004)
* [arXiv:2412.20904](https://arxiv.org/abs/2412.20904)

The initial network is constructed using the Bianconi-Barabási model, and the network is examined for changes over time by repeatedly deleting and adding nodes. We introduced two tunable parameters, the deleting parameter $\alpha$ and the adding parameter $\beta$, and observed the aging behavior for the various parameter sets.

The data obtained by executing this code is available in the following repository.

[https://isspns-gitlab.issp.u-tokyo.ac.jp/kaityo256/aging_network_data](https://isspns-gitlab.issp.u-tokyo.ac.jp/kaityo256/aging_network_data)

## How to use

### How to build

```sh
git clone --recursive https://github.com/kaityo256/aging_network_cpp.git
cd aging_network_cpp
make
```

### How to run

Prepare a parameter file.

```sh:sample.cfg
system_size = 10000
alpha = 0.0
beta = 0.0
n_sample = 1
data_dir = data
percolation_sample = 100
use_BA_model = False
```

The meaning of each parameter is as follows

| Parameter              | Value    | Description |
|------------------------|---------|-------------|
| `system_size`         | 10000   | Size of the system, e.g., number of nodes|
| `alpha`              | 0.0     | Deleting parameter |
| `beta`               | 0.0     | Adding parameter|
| `n_sample`           | 1       | Number of samples (e.g., number of trials in an simulation) |
| `data_dir`           | `data`  | Directory for storing data |
| `percolation_sample` | 100     | Number of percolation samples |
| `use_BA_model`       | False   | Whether to use the Barabási–Albert (BA) model |

See [this paper](https://arxiv.org/abs/2412.20904) for the meaning of the parameters  and details of the calculations.

Here is how to execute this program.

```sh
$ ./aging_simulaiton sample.cfg
data/degree_distribution_N10000_a00_b00.dat
data/fitness_average_N10000_a00_b00.dat
data/degree_average_N10000_a00_b00.dat
data/degree_variance_N10000_a00_b00.dat
data/percolation_N10000_a00_b00.dat
```

You will have the following results.

| File Name                                         | Description |
|--------------------------------------------------|-------------|
| `data/degree_distribution_N10000_a00_b00.dat`   | Degree distribution data of the system |
| `data/fitness_average_N10000_a00_b00.dat`       | Time evolution of average fitness|
| `data/degree_average_N10000_a00_b00.dat`        | Time evolution of average of the degree distribution|
| `data/degree_variance_N10000_a00_b00.dat`       | Time evolution of variance of the degree distribution |
| `data/percolation_N10000_a00_b00.dat`           | Percolation data of the system |

### Parameter parallel computation using CPS

You can use [CPS](https://github.com/kaityo256/cps) to perform parameter parallel calculations.

Build cps.

```sh
cd cps
make
cd ..
```

Prepare parameter files and a task list.

```sh
python3 makeparam.py
```

You will obtain the following parameter files.

```txt
N10000_a-10_b20.cfg
N10000_a0_b20.cfg
N10000_a10_b20.cfg
N10000_a30_b20.cfg
N10000_a15_b-10.cfg
N10000_a15_b0.cfg
N10000_a15_b10.cfg
N10000_a15_b20.cfg
N10000_a-15_b20.cfg
N10000_a30_b25.cfg
N10000_a-15_b-15.cfg
N10000_a20_b-10.cfg
```

You also obtain a task list `task.sh`.

```sh
./aging_simulation N10000_a-10_b20.cfg
./aging_simulation N10000_a0_b20.cfg
./aging_simulation N10000_a10_b20.cfg
./aging_simulation N10000_a30_b20.cfg
./aging_simulation N10000_a15_b-10.cfg
./aging_simulation N10000_a15_b0.cfg
./aging_simulation N10000_a15_b10.cfg
./aging_simulation N10000_a15_b20.cfg
./aging_simulation N10000_a-15_b20.cfg
./aging_simulation N10000_a30_b25.cfg
./aging_simulation N10000_a-15_b-15.cfg
./aging_simulation N10000_a20_b-10.cfg
```

Launch `cps` as follows.

```sh
mpirun --oversubscribe  -np 5 ./cps/cps task.sh 
```

In this example, cps is started in five processes, but one process is used for management, so it is executed in four processes in parallel.

After the calculation is completed, refer to `cps.log` for the calculation time. Here is an example of output.

```txt
Number of tasks : 12
Number of processes : 5
Total execution time: 957.304 [s]
Elapsed time: 303.608 [s]
Parallel Efficiency : 0.788273

Task list:
Command : Elapsed time
./aging_simulation N10000_a-10_b20.cfg : 65.265 [s]
./aging_simulation N10000_a0_b20.cfg : 49.161 [s]
./aging_simulation N10000_a10_b20.cfg : 143.255 [s]
./aging_simulation N10000_a30_b20.cfg : 69.48 [s]
./aging_simulation N10000_a15_b-10.cfg : 59.816 [s]
./aging_simulation N10000_a15_b0.cfg : 54.664 [s]
./aging_simulation N10000_a15_b10.cfg : 61.644 [s]
./aging_simulation N10000_a15_b20.cfg : 92.111 [s]
./aging_simulation N10000_a-15_b20.cfg : 67.39 [s]
./aging_simulation N10000_a30_b25.cfg : 172.475 [s]
./aging_simulation N10000_a-15_b-15.cfg : 70.132 [s]
./aging_simulation N10000_a20_b-10.cfg : 51.911 [s]
```

The log shows that 12 tasks were executed in parallel by 5 processes (actually 4 processes because of the management process), with an overall computation time of 957 seconds and an actual wall time of 304 seconds.


## License

MIT

We appreciate your citing the following reference when you publish the results using our code.

* R. Sekikawa and H. Watanabe, "Structural Changes and Percolation Transition in Networks after Aging Processes", J. Phys. Soc. Jpn. **94**, 044004 (2025).
* [https://doi.org/10.7566/JPSJ.94.044004](https://doi.org/10.7566/JPSJ.94.044004)
