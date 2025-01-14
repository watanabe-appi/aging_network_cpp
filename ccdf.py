import sys
import os


def main(filename):
    k = []
    with open(filename) as f:
        for line in f:
            k.append(float(line))
    s = sum(k)
    v = s
    ccdf_file = os.path.splitext(filename)[0] + ".ccdf"
    with open(ccdf_file, "w") as f:
        for i in range(len(k)):
            f.write(f"{i} {v/s}\n")
            v = v - k[i]
    print(f"{filename}->{ccdf_file}")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("usage: ccdf.py data.dat")
    filename = sys.argv[1]
    main(filename)
