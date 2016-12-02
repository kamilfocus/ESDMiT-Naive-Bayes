import math

def gaussian_pdf(x, mean, stdev):
    e = math.exp(-((x-mean)**2)/(2*(stdev**2)))
    a = 1/(math.sqrt(2*math.pi)*stdev)
    return a * e
  
if __name__ == "__main__":
    x = 10
    mean = 11.5
    stdev = 5
    print compute_gaussian_probability(x, mean, std)
