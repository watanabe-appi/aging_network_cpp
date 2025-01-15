import os


def calculate_moment(filename):
    data = []
    with open(filename) as f:
        for line in f:
            data.append(int(line))
    num = sum(data)
    k_ave = 0.0
    k2_ave = 0.0
    for i in range(len(data)):
        k_ave += i * data[i]
        k2_ave += i * i * data[i]
    k_ave /= num
    k2_ave /= num
    return k_ave / k2_ave


def calculate_p(filename):
    data = []
    with open(filename) as f:
        for line in f:
            data.append(float(line))
    return data[20]


def last_variance(filename):
    data = []
    with open(filename) as f:
        for line in f:
            data.append(float(line))
    return data[-1]


def save_file(filename, data):
    print(filename)
    with open(filename, "w") as f:
        for alpha, beta in data:
            f.write(f"{alpha} {beta}\n")


def load_all(N):
    moment_dic = {}
    percolation_dic = {}
    variance_dic = {}
    for a in range(10):
        for b in range(10):
            alpha = -1.5 + a * 0.5
            beta = -1.5 + b * 0.5
            ia = int(alpha * 10)
            ib = int(beta * 10)
            param = (alpha, beta)
            basename = f"N{N:05d}_a{ia:02d}_b{ib:02d}.dat"
            percolation_dic[param] = calculate_p("percolation_" + basename)
            moment_dic[param] = calculate_moment("degree_distribution_" + basename)
            variance_dic[param] = last_variance("degree_variance_" + basename)
    return moment_dic, percolation_dic, variance_dic


def make_phase_diagram():
    N = 1000
    moment_dic, percolation_dic, variance_dic = load_all(N)
    exp_finite = []  # 分布が指数関数かつ相転移点有限
    exp_zero = []  # 分布が指数関数かつ相転移点がゼロ
    power_finite = []  # 分布がベキ関数かつ相転移点有限
    power_zero = []  # 分布がベキ関数かつ相転移点がゼロ
    for a in range(10):
        for b in range(10):
            alpha = -1.5 + a * 0.5
            beta = -1.5 + b * 0.5
            param = (alpha, beta)
            # print(f"{param} {variance_dic[param]}")
            if variance_dic[param] > 200:
                continue
            if moment_dic[param] > 0.1:
                if percolation_dic[param] > 0.2:
                    exp_zero.append(param)
                else:
                    exp_finite.append(param)
            else:
                if percolation_dic[param] > 0.2:
                    power_zero.append(param)
                else:
                    power_finite.append(param)
    save_file("exp_finite.dat", exp_finite)
    save_file("exp_zero.dat", exp_zero)
    save_file("power_finite.dat", power_finite)
    save_file("power_zero.dat", power_zero)


if __name__ == "__main__":
    make_phase_diagram()
