import json
import matplotlib.pyplot as plt
import os
import re

reports_directory = '../report'

dataSet_f = []
dataSet_d = []
dataSet_fOpt = []
dataSet_dOpt = []

pattern1 = r'f.json'
pattern2 = r'd.json'
pattern3 = r'fOpt.json'
pattern4 = r'dOpt.json'

def main():
    initialiseDataSets('report.json')
    # print(dataSet_f)
    for filename in os.listdir(reports_directory):
            addData(filename)
    # print(dataSet_f)
    plotData(dataSet_f, 'plot_f.png')
    plotData(dataSet_d, 'plot_d.png')
    plotData(dataSet_fOpt, 'plot_fOpt.png')
    plotData(dataSet_dOpt,  'plot_dOpt.png')


def initialiseDataSets(filename):
    benchmark_list = []
    file_path = os.path.join(reports_directory, filename)
    with open(file_path, 'r') as file:
        data = json.load(file)
        for benchmark in data['benchmarks']:
            benchmark_name = benchmark['name'].split('/')[0]
            if benchmark_name not in benchmark_list:
                dataSet_f.append({benchmark_name:[]})
                dataSet_d.append({benchmark_name:[]})
                dataSet_fOpt.append({benchmark_name:[]})
                dataSet_dOpt.append({benchmark_name:[]})
            benchmark_list.append(benchmark_name)
            
            
def addData(filename):
    file_path = os.path.join(reports_directory, filename)
    with open(file_path, 'r') as file:
        data = json.load(file)
        for benchmark in data['benchmarks']:
            if 'aggregate_name' in benchmark and benchmark['aggregate_name'] == 'mean':
                benchmark_name = benchmark['name'].split('/')[0]
                for item in chooseDataSet(filename):
                    if benchmark_name in item:
                        item[benchmark_name].append((extractNumber(filename),benchmark['real_time']))
                     
                        
def chooseDataSet(filename):
    if re.search(pattern1, filename):
        return dataSet_f
    elif re.search(pattern2, filename):
        return dataSet_d
    elif re.search(pattern3, filename):
        return dataSet_fOpt
    elif re.search(pattern4, filename):
        return dataSet_dOpt
    else:
        return None
                    
def extractNumber(filename):
    match = re.search(r'report(\d+)[a-zA-Z]?(\w*).json', filename)
    if match:
        number = int(match.group(1))
        return number
    else:
        return None


def orderData(dataSet):
    for item in dataSet:
        for key, value in item.items():
            value.sort(key=lambda tup: tup[0])


def plotData(dataSet, output_file_name):
    plt.figure()
    orderData(dataSet)
    for item in dataSet:
        #print(item)
        for key, value in item.items():
            x = []
            y = []
            for pair in value:
                x.append(pair[0])
                y.append(pair[1])
        plt.plot(x, y, label=key)
    plt.xlabel('Matrix size')
    plt.ylabel('Time (ns)')
    plt.title(typeName(dataSet) + ' Matrices multiplication')
    plt.legend()
    plt.show()
    plt.savefig(output_file_name)                


def typeName(dataSet):
    if dataSet == dataSet_f:
        return 'Float '
    elif dataSet == dataSet_d:
        return 'Double '
    elif dataSet == dataSet_fOpt:
        return 'Float optimised '
    elif dataSet == dataSet_dOpt:
        return 'Double optimised '
    else:
        return None

main()