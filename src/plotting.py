import json
import matplotlib.pyplot as plt
import os
import re

dataSet_f = []
dataSet_d = []
dataSet_fOpt = []
dataSet_dOpt = []
reports_directory = '../report'
pattern1 = r'f.json'
pattern2 = r'd.json'
pattern3 = r'fOpt.json'
pattern4 = r'dOpt.json'

def main():
    initialiseDataSets('report16f.json')
    print(dataSet_f)
    for filename in os.listdir(reports_directory):
        if re.search(pattern1, filename):
            print(filename)
            addFloatData(filename)
    print(dataSet_f)


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
            
            
def addFloatData(filename):
    file_path = os.path.join(reports_directory, filename)
    with open(file_path, 'r') as file:
        data = json.load(file)
        for benchmark in data['benchmarks']:
            if 'aggregate_name' in benchmark and benchmark['aggregate_name'] == 'mean':
                benchmark_name = benchmark['name'].split('/')[0]
                for item in dataSet_f:
                    if benchmark_name in item:
                        item[benchmark_name].append((extractNumber(filename),benchmark['real_time']))
      
                    
def extractNumber(filename):
    match = re.search(r'report(\d+)[a-zA-Z]?(\w*).json', filename)
    if match:
        number = int(match.group(1))
        return number
    else:
        return None                    


main()