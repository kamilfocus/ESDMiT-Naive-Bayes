import subprocess, os, re, sys, signal, random, time

feature_mask = '0x3ffff'
root_path    = '../../ecg-classification/data/data_unique_labels'

learn_cmd = '"../Program/bayes" -d "%s/train_data.txt" -d "%s/train_label.txt" -r -l -m -f ' + feature_mask
test_cmd  = '"../Program/bayes" -d "%s/test_data.txt" -d "%s/test_label.txt" -t -m -f ' + feature_mask

def Nan_handle(item):
    if 'IND' in str(item):
        item = 'NaN'
    else:
        item = float(item)
    return item

def bayes_call(learn_cmd, test_cmd):
    learn_proc = subprocess.Popen(learn_cmd, env=os.environ, stdout=subprocess.PIPE)
    learn_proc.wait()

    test_proc  = subprocess.Popen(test_cmd, env=os.environ, stdout=subprocess.PIPE)
    res  = test_proc.stdout.read()

    return [float(re.findall('accuracy:(.*)%', res)[0]),    \
            Nan_handle(re.findall('Sensitivity:(.*)%', res)[0]), \
            Nan_handle(re.findall('Specificity:(.*)%', res)[0])]

# Package label: Accuracy | Sensitivity | Specificity
def report_generate(data_label, data):
    output =  str('\t\t\\hline\n\t\t')
    output += str(data_label)
    output += " & "
    gap_width = 10
    for i, item in enumerate(data):
        float_str = str(item)
        if isinstance(item, float):
            float_str = '%3.2f' % item
        output += str(float_str) + (gap_width - len(str(float_str)))*" "
        if i != len(data)-1:
            output += ' & '
        else:
            output += ' \\\\'
    return output

if __name__ == "__main__":
    with open('report.txt', 'w') as report:
        sum     = 3*[0]
        samples = len(sum)*[0]
        for root, dirs, files in os.walk(root_path):
            for dir in dirs:
                path = os.path.join(root, dir)
                data = bayes_call(learn_cmd % (path, path), test_cmd % (path, path))
                for i, item in enumerate(data):
                    if not isinstance(item, str):
                        sum[i]     += item
                        samples[i] += 1
                report.write(report_generate(dir, data) + '\n')
        mean = len(sum)*[0]
        for i, item in enumerate(sum):
            mean[i] = item/samples[i]
        report.write(report_generate('Srednia', mean) + '\n')
