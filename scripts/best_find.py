
THRES = 10

if __name__ == "__main__":
    res = []
    with open('logs.txt', 'r') as log:
        for line in log:
            prec   = float(line.split(' ')[-2])
            mask   = int(line.split(' ')[0], 16)
            res.append((mask, prec))
    res = sorted(res, key = lambda res: res[1], reverse = True)

    final = res[:THRES]
    for item in final:
        output = "Best mask: 0x%05X with accuracy: %f %%" % (item[0], item[1])
        print(output)
    
    common_mask = 0x3FFFF
    total_mask  = 0x00000
    for item in final:
        common_mask = common_mask & item[0]
        total_mask  = total_mask  | item[0]
    output = "Common mask: 0x%05X\t Total mask: 0x%05X" % (common_mask, total_mask)
    print(output)