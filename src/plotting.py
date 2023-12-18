import json
import matplotlib.pyplot as plt
import os
import re

reports_directory = '../reports'

# Data sets that will contain the data to plot coming from the report files in the report directory.
# Each data set is a list of dictionaries. Each dictionary contains the name of the benchmark as key 
# and a list of tuples (matrix size, time) as value.
dataSet_f = []
dataSet_d = []
dataSet_fOpt = []
dataSet_dOpt = []

# Patterns used to identify the report files containing the data to plot.
pattern1 = r'f.json'
pattern2 = r'd.json'
pattern3 = r'fOpt.json'
pattern4 = r'dOpt.json'

def main():
    initialiseDataSets('report.json')
    for filename in os.listdir(reports_directory):
            addData(filename)
    plotData(dataSet_f, '../plots/plot_f.png')
    plotData(dataSet_d, '../plots/plot_d.png')
    plotData(dataSet_fOpt, '../plots/plot_fOpt.png')
    plotData(dataSet_dOpt, '../plots/plot_dOpt.png')


def initialiseDataSets(filename):
    """
    This function reads a reference JSON file and extracts the benchmark names from it. 
    It then initializes four data sets (dataSet_f, dataSet_d, dataSet_fOpt, dataSet_dOpt) 
    with empty lists for each unique benchmark name. The data sets are used for storing data 
    related to different benchmarks.
    
    Args:
        filename (str): The name of the reference JSON file.
        
    Returns:
        None
    """
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
    """
    Add the data contained in the json file filename to the data sets.
    
    Args:
        filename (str): The name of the json file containing the data to add. 
    
    Returns:
        None   
    """
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
    """
    Choose the appropriate data set depending on the filaname.
    
    Args:
        filename (str): The name of the json file.
        
    Returns:
        list: The appropriate data set.
    """
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
    """
    Extract the matrix size from the filename.
    
    Args:
        filename (str): The name of the json file.
    
    Returns:
        int: The matrix size.
    """
    match = re.search(r'report(\d+)[a-zA-Z]?(\w*).json', filename)
    if match:
        number = int(match.group(1))
        return number
    else:
        return None


def orderData(dataSet):
    """
    Order the data in the data set by matrix size.
    
    Args:
        dataSet (list): The data set to order.
    
    Returns:    
        None
    """
    for item in dataSet:
        for key, value in item.items():
            value.sort(key=lambda tup: tup[0])


def plotData(dataSet, output_file_name):
    """
    Plot the data contained in the data set in loglog scale.
    
    Args:
        dataSet (list): The data set to plot.
        output_file_name (str): The name of the output file.
    
    Returns:
        None
    """
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
        plt.loglog(x, y, label=key)
    plt.xlabel('Matrix size')
    plt.ylabel('Time (ns)')
    plt.title(typeName(dataSet) + ' Matrices multiplication')
    plt.legend()
    plt.show()
    plt.savefig(output_file_name)                


def typeName(dataSet):
    """
    Return the type of data contained in the data set.
    
    Args:
        dataSet (list): The data set.
    
    Returns:
        str: The type of data.
    """
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
