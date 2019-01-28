#import
import matplotlib.pyplot as plt
import math
import matplotlib
import matplotlib.patches as mpatches

def init_data(): #initialize data
    pm25_data = open("dat/PM25.TXT", "r")
    pm10_data = open("dat/PM10.TXT", "r")
    try:
        humidity = open("dat/humidity.txt", "r")
    except:
        humidity = 'NA'
    return [pm25_data, pm10_data, humidity]

def read_lines(pm25, pm10, humidity): #read data files
    pm25_arr = []
    pm10_arr = []
    humidity_arr = []

    for i in pm25:
        pm25_arr.append(i)

    for i in pm10:
        pm10_arr.append(i)

    if humidity != 'NA':
        for i in humidity:
            humidity_arr.append(i)
    else:
        humidity_arr = 'NA'
    return [pm25_arr, pm10_arr, humidity_arr]

def time_split(arr, period):
    if arr == 'NA':
        return 'NA'
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

def plotdata(arr25, arr10, humidity, time, dayi, dayf):
    plt.style.use('ggplot') #custom styling
    pm25_arr = []
    pm10_arr = []
    humidity_arr = []
    if dayi == dayf and dayi == 0: #use all data if dayi and dayf == 0
        for i in arr25:
            for n in i:
                pm25_arr.append(n)
        for i in arr10:
            for n in i:
                pm10_arr.append(n)
        if humidity != 'NA':
            for i in humidity:
                for n in i:
                    humidity_arr.append(n)
    else:
        for i in range(dayi-1, dayf-1): #takes data from the range of days and appends to one array
            for t in arr25[i]:
                pm25_arr.append(t/1.099)
        for i in range(dayi - 1, dayf - 1):  # takes data from the range of days and appends to one array
            for t in arr10[i]:
                pm10_arr.append(t)
        if humidity != 'NA':
            for i in range(dayi-1, dayf-1):
                for t in humidity[i]:
                    humidity_arr.append[t/1.33]


    plt.plot(pm25_arr, color='cyan')
    #plt.plot(pm10_arr, color='magenta')
    if humidity_arr != "NA":
        plt.plot(humidity_arr, color='black')
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
pm10 = data[1] #data contains 2 arrays, array index 0 is pm25 and index 1 is pm10
humidity = data[2]
data_arr = read_lines(pm25, pm10, humidity)
pm25_arr = time_split(data_arr[0], 5)
pm10_arr = time_split(data_arr[1], 5)
humidity_arr = time_split(data_arr[2], 5)
plotdata(pm25_arr, pm10_arr, humidity_arr, 5, 0, 0) #draw graph
print(plt.style.available)
print(matplotlib.get_configdir(),)
