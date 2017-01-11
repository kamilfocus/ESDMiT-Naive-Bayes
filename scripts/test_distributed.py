import subprocess, os, re, sys, signal, random, time

feature_mask = '0x3ffff'
root_path    = '../../ecg-classification/data/data_unique_labels'

learn_cmd = '"../Program/bayes" -d "%s/train_data.txt" -d "%s/train_label.txt" -r -l -m -f ' + feature_mask
test_cmd  = '"../Program/bayes" -d "%s/test_data.txt" -d "%s/test_label.txt" -t -m -f ' + feature_mask

def bayes_call(learn_cmd, test_cmd):
    learn_proc = subprocess.Popen(learn_cmd, env=os.environ, stdout=subprocess.PIPE)
    learn_proc.wait()
    time_patt  = 'time:(.*)\['
    learn_time = float(re.findall(time_patt, learn_proc.stdout.read())[0])
    test_proc  = subprocess.Popen(test_cmd, env=os.environ, stdout=subprocess.PIPE)
    res  = test_proc.stdout.read()
    return [re.findall('accuracy:(.*)%', res)[0],    \
            re.findall('Sensitivity:(.*)%', res)[0], \
            re.findall('Specificity:(.*)%', res)[0], \
            str(learn_time + float(re.findall(time_patt, res)[0]))]

# Package label: Accuracy | Sensitivity | Specificity | Execution time
def report_generate(data_label, data):
    output = str(data_label)
    output += ": "
    gap_width = 10
    for item in data:
        output += str(item) + (gap_width - len(str(item)))*" "
    return output

if __name__ == "__main__":
    with open('report.txt', 'w') as report:
        for root, dirs, files in os.walk(root_path):
            for dir in dirs:
                path = os.path.join(root, dir)
                data = bayes_call(learn_cmd % (path, path), test_cmd % (path, path))
                report.write(report_generate(dir, data) + '\n')
