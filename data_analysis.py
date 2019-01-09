#import
import matplotlib.pyplot as plt
import math
import matplotlib
import matplotlib.patches as mpatches

#variable declaration

def init_data():
    pm25_data = open("pmdata/PM25.TXT", "r")
    pm10_data = open("pmdata/PM10.TXT", "r")
    return [pm25_data, pm10_data]


def read_lines(pm25, pm10): #read data files
    i = 1 #index
    pm25_arr = []
    pm10_arr = []

    for line in pm25.readlines():
        if i % 2 == 0 and i>=10:
            line = line[6:]
            line = line[:5]
            pm25_arr.append(float(line)) #append lines to array
        i += 1

    for line in pm10.readlines():
        if i % 2 == 0 and i>=10:
            line = line[6:]
            line = line[:5]
            pm10_arr.append(float(line))  # append lines to array
        i += 1

    return [pm25_arr, pm10_arr]

def time_split(arr, period):
    s_array = []
    temp_array = []
    ratio = 1440/period

    for i in range(len(arr)):
        if (i+1)%ratio == 0:
            s_array.append(temp_array)
            temp_array = []
        temp_array.append(arr[i])

    return s_array

def data_analyse(dataset):
    average = math.fsum(dataset)/len(dataset) #sum/n (average)
    max = max(dataset)
    min = min(dataset)
    return {
        "max": max,
        "min": min,
        "average": average
    }
def plotdata(arr25, arr10, time, dayi, dayf):
    plt.style.use('ggplot') #custom styling
    pm25_arr = []
    pm10_arr = []
    for i in range(dayi-1, dayf-1): #takes data from the range of days and appends to one array
        for t in arr25[i]:
            pm25_arr.append(t)
    for i in range(dayi - 1, dayf - 1):  # takes data from the range of days and appends to one array
        for t in arr10[i]:
            pm10_arr.append(t)
    plt.plot(pm25_arr, color='cyan')
    plt.plot(pm10_arr, color='magenta')
    plt.ylabel('μg/m³')
    plt.xlabel("Time Period: " + str(time) + " minutes/point")
    pm25_lable = mpatches.Patch(color='cyan', label='PM25')
    pm10_lable = mpatches.Patch(color='magenta', label='PM10')
    plt.legend(handles=[pm25_lable, pm10_lable])
    plt.grid(b=True, which='major', color='w', linestyle='--', zorder=10)
    plt.show()

#main
data = init_data()
pm25 = data[0]
pm10 = data[1]
data_arr = read_lines(pm25, pm10)
pm25_arr = time_split(data_arr[0], 5)
pm10_arr = time_split(data_arr[1], 5)
plotdata(pm25_arr, pm10_arr, 5, 4, 7)
print(plt.style.available)
print(matplotlib.get_configdir(),)
