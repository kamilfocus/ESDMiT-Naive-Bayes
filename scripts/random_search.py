import subprocess, os, re, sys, signal, random, time

cmd = '"../Program/bayes" -d "../All/ConvertedQRSRawData_all.txt" -d "../All/Class_IDs_all.txt" -f %x -r'

def bayes_call(mask):
    proc = subprocess.Popen(cmd % mask, env=os.environ, stdout=subprocess.PIPE)
    res  = proc.stdout.read()
    return float(re.findall(':(.*)%', res)[0])

def random_search(masks):
    with open('logs.txt', 'w') as log:
        while len(masks) > 0:
            index = random.randrange(len(masks))
            mask  = masks.pop(index)
            prec  = bayes_call(mask)
            output = "0x%05X %f %%" % (mask, prec)
            log.write(output+'\n')
            log.flush()
            print(output)

if __name__ == "__main__":
    random_search(range(1, 0x3ffff + 1))