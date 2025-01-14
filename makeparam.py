def save_param(N, alpha, beta, n_sample, data_dir):
    param = f"""
system_size = {N}
alpha = {alpha}
beta = {beta}
n_sample = {n_sample}
data_dir = {data_dir}
percolation_sample = 100
"""
    ia = int(alpha * 10)
    ib = int(beta * 10)
    filename = f"N{N:05d}_a{ia}_b{ib}.cfg"
    print(filename)
    with open(filename, "w") as f:
        f.write(param)
    return filename


def distribution():
    N = 10000
    n_sample = 100
    params = []
    params.append((-1.0, 2.0))
    params.append((0.0, 2.0))
    params.append((1.0, 2.0))
    params.append((2.0, 2.0))
    params.append((3.0, 2.0))

    params.append((1.5, -1.0))
    params.append((1.5, 0.0))
    params.append((1.5, 1.0))
    params.append((1.5, 2.0))
    # params.append(( 1.5,  3.0)) //これだけ遅いので

    params.append((-1.5, 2.0))
    params.append((3.0, 2.5))
    params.append((-1.5, -1.5))
    params.append((2.0, -1.0))
    paramfiles = []
    for alpha, beta in params:
        filename = save_param(N, alpha, beta, n_sample, "data")
        paramfiles.append(filename)
    with open("task.sh", "w") as f:
        for filename in paramfiles:
            f.write(f"./aging_simulation {filename}\n")


def save_param_sampling(N, alpha, beta, seed):
    param = f"""
system_size = {N}
alpha = {alpha}
beta = {beta}
n_sample = 1
percolation_sample = 100
sampling = true
data_dir = sampling
seed = {seed}
"""
    ia = int(alpha * 10)
    ib = int(beta * 10)
    filename = f"N{N:05d}_a{ia}_b{ib}_s{seed:02d}.cfg"
    print(filename)
    with open(filename, "w") as f:
        f.write(param)
    return filename


def sampling(alpha, beta, num):
    N = 10000
    paramfiles = []
    for i in range(num):
        filename = save_param_sampling(N, alpha, beta, i)
        paramfiles.append(filename)
    with open("task.sh", "w") as f:
        for filename in paramfiles:
            f.write(f"./aging_simulation {filename}\n")


def finite_size():
    params = []
    params.append((2500, -1.5, 2.0, 1))
    params.append((2500, 3.0, 2.5, 1))
    params.append((2500, -1.5, -1.5, 1))
    params.append((2500, 2.0, -1.0, 1))
    params.append((5000, -1.5, 2.0, 1))
    params.append((5000, 3.0, 2.5, 1))
    params.append((5000, -1.5, -1.5, 1))
    params.append((5000, 2.0, -1.0, 1))
    params.append((7500, -1.5, 2.0, 1))
    params.append((7500, 3.0, 2.5, 1))
    params.append((7500, -1.5, -1.5, 1))
    params.append((7500, 2.0, -1.0, 1))
    paramfiles = []
    for N, alpha, beta, n_sample in params:
        filename = save_param(N, alpha, beta, n_sample, "finite_size")
        paramfiles.append(filename)
    with open("task.sh", "w") as f:
        for filename in paramfiles:
            f.write(f"./aging_simulation {filename}\n")


def main():
    # sampling(1.5, 3.0, 100)
    finite_size()


if __name__ == "__main__":
    main()
