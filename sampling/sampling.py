def main():
    basename = "degree_distribution_N10000_a15_b30"
    data = []
    for i in range(100):
        filename = basename + f"_s{i:02d}.dat"
        print(filename)
        d = []
        with open(filename) as f:
            for line in f:
                d.append(int(line))
        data.append(d)
    max_size = max([len(d) for d in data])
    print(max_size)
    num = len(data)
    ninv = 1.0 / num
    average = [0] * (max_size + 1)
    for d in data:
        for i in range(len(d)):
            average[i] += d[i] * ninv
    outputfile = basename + ".dat"
    with open(outputfile, "w") as f:
        for i in range(max_size):
            f.write(f"{average[i]}\n")
    print(outputfile)


if __name__ == "__main__":
    main()
