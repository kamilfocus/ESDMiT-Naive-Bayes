import math

def gaussian_pdf(x, mean, stdev):
    if stdev > 0:
        e = math.exp(-((x-mean)**2)/(2*(stdev**2)))
        a = 1/(math.sqrt(2*math.pi)*stdev)
        return a * e
    else:
        if x == mean:
            return 1.0
        else:
            return 0.0
  
if __name__ == "__main__":
    x = 10
    mean = 11.5
    stdev = 5
    print compute_gaussian_probability(x, mean, std)
