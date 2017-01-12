import subprocess, os, re, sys, signal, random, time

root_path    = '../../ecg-classification/data/data_unique_labels'

python_logs = 'bayes_logger.txt'

python_learn_cmd = 'python "../Model/main.py" -d "%s/train_data.txt" -d "%s/train_label.txt" -r -l'
python_test_cmd  = 'python "../Model/main.py" -d "%s/test_data.txt" -d "%s/test_label.txt" -t'

cpp_learn_cmd = '"../Program/bayes" -d "%s/train_data.txt" -d "%s/train_label.txt" -r -l -m'
cpp_test_cmd  = '"../Program/bayes" -d "%s/test_data.txt" -d "%s/test_label.txt" -t -m'

def Nan_handle(item):
    if 'IND' in str(item):
        item = 'NaN'
    else:
        item = float(item)
    return item

def get_output(proc, cmd):
    output = str()
    if 'python' in cmd:
        with open(python_logs, 'r') as logs:
            output = logs.read()
    else:
        output = proc.stdout.read()
    return output

def bayes_call(learn_cmd, test_cmd):
    learn_proc = subprocess.Popen(learn_cmd, env=os.environ, stdout=subprocess.PIPE)
    learn_proc.wait()
    time_patt  = 'time:(.*)\['
    learn_time = float(re.findall(time_patt, get_output(learn_proc, learn_cmd))[0])

    test_proc  = subprocess.Popen(test_cmd, env=os.environ, stdout=subprocess.PIPE)
    test_proc.wait()

    return (learn_time + float(re.findall(time_patt, get_output(test_proc, test_cmd))[0]))

# Package label: Py Execution time | Cpp execution time
def report_generate(data_label, data):
    output =  str('\t\t\\hline\n\t\t')
    output += str(data_label)
    output += " & "
    gap_width = 10
    for i, item in enumerate(data):
        float_str = str(item)
        if isinstance(item, float):
            float_str = '%3.3f' % item
        output += str(float_str) + (gap_width - len(str(float_str)))*" "
        if i != len(data)-1:
            output += ' & '
        else:
            output += ' \\\\'
    return output

if __name__ == "__main__":
    with open('time_report.txt', 'w') as report:
        sum     = 2*[0]
        samples = list(sum)
        for root, dirs, files in os.walk(root_path):
            for dir in dirs:
                path = os.path.join(root, dir)
                python_time = bayes_call(python_learn_cmd % (path, path), python_test_cmd % (path, path))
                cpp_time = bayes_call(cpp_learn_cmd % (path, path), cpp_test_cmd % (path, path))
                data = [python_time, cpp_time]
                for i, item in enumerate(data):
                    if not isinstance(item, str):
                        sum[i]     += item
                        samples[i] += 1
                report.write(report_generate(dir, data) + '\n')
        mean = len(sum)*[0]
        for i, item in enumerate(sum):
            mean[i] = item/samples[i]
        report.write(report_generate('Srednia', mean) + '\n')
