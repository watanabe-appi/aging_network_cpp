def save_param(N, alpha, beta):
    param = f"""
system_size = {N}
alpha = {alpha}
beta = {beta}
n_sample = 1
percolation_sample = 100
"""
    ia = int(alpha * 10)
    ib = int(beta * 10)
    filename = f"N{N:05d}_a{ia}_b{ib}.cfg"
    print(filename)
    with open(filename, "w") as f:
        f.write(param)


def main():
    save_param(10000, -2.2, 1.0)


if __name__ == "__main__":
    main()
