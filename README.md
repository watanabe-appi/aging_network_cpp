# Aging simulation of Network

## Summary

This is the code to obtain the results of the following paper.

Ryuho Sekikawa, Hiroshi Watanabe, "Structural Changes and Percolation Transition in Networks after Aging Processes", [arXiv:2412.20904](https://arxiv.org/abs/2412.20904).

The initial network is constructed using the Bianconi-Barabási model, and the network is examined for changes over time by repeatedly deleting and adding nodes. We introduced two tunable parameters, the deleting parameter $\alpha$ and the adding parameter $\beta$, and observed the aging behavior for the various parameter sets.

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

Here is the Markdown table template for the given output files:

```markdown
| File Name                                         | Description |
|--------------------------------------------------|-------------|
| `data/degree_distribution_N10000_a00_b00.dat`   | Degree distribution data of the system |
| `data/fitness_average_N10000_a00_b00.dat`       | Time evolution of average fitness|
| `data/degree_average_N10000_a00_b00.dat`        | Time evolution of average of the degree distribution|
| `data/degree_variance_N10000_a00_b00.dat`       | Time evolution of variance of the degree distribution |
| `data/percolation_N10000_a00_b00.dat`           | Percolation data of the system |
```

## License

MIT
