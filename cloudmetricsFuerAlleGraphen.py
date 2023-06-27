import cloudmetrics
import numpy as np
import wradlib as wrl
import xarray as xr
import matplotlib.pyplot as plt
import pandas as pd
import datetime
import matplotlib as mpl
import glob
import cv2
import os
import shutil
import matplotlib.colors as mcolors
import math
import seaborn as sns
import re
import gc

#for cartesian coordinates
def cartesianIdToCartesianCoor(gridIdIn):
    
    coordinates = np.empty(3, dtype=int) 
    
    timestep = 0
    while (gridIdIn-(1501*1501)) >= 0:
        timestep += 1
        gridIdIn -= (1501*1501)
    coordinates[0] = timestep
    
    x = 0
    while gridIdIn - 1501 >= 0:
        x += 1
        gridIdIn -= 1501
    coordinates[1] = x
    
    y = gridIdIn
    coordinates[2] = y
    return coordinates

def readInTxtFile(path):
    print("Read in text für Plotting routine")
    dataFromTxt = np.loadtxt(path, dtype=int)
    dataArray = np.full((288, 1501, 1501), 0)
    print("dim array zero: " + str(dataArray.ndim))
    i = 0
    maxI = dataFromTxt.size
    firstTimestamp = 0
    lastTimestamp = 0
    print("size of txtFile " + str(maxI))
    while i < maxI:
        if(i%100000 == 0):
            print(i)
        polarCoor = cartesianIdToCartesianCoor(dataFromTxt[i])
        #print("t: " + str(polarCoor[0]) + " a: " + str(polarCoor[1]) + " r: " + str(polarCoor[2]))
        dataArray[polarCoor[0]][polarCoor[1]][polarCoor[2]] = 1
        if i == 0:
            print("FST")
            firstTimestamp = polarCoor[0]
        i += 2
        if i >= maxI:
            print("LST")
            lastTimestamp = polarCoor[0]
        #del polarCoor
        #gc.collect()
    #dataArray = data.astype('float32')
    print("dim array first: " + str(dataArray.ndim))
    print("first timestamp: " + str(firstTimestamp))
    print("last timestamp: " + str(lastTimestamp))
    del dataFromTxt
    gc.collect()
    return dataArray, firstTimestamp, lastTimestamp

def saveCloudmetricsOnGraphsToTxtFile(cloudmetricsOnGraphs, bezeichner):
    path = r"C:\Users\Jonathan Langer\OneDrive\Bachelorarbeit\Experimentdaten\cloudmetrics"
    with open(path+'\\'+bezeichner+'.txt', 'w') as f:
        for m in cloudmetricsOnGraphs:
            f.write(str(m) + ' ')
        f.close()


def calculateGraphMetricsForGraphs():
    
    directoryGraphComponents = r"C:\Users\Jonathan Langer\OneDrive\Bachelorarbeit\Experimentdaten\comparingProcessedData\GraphDateienNummeriert"
    i = 0
    for filename in os.listdir(directoryGraphComponents):

        cloudmetricsOnGraphs = np.full((9), 0.0)
        graphNr = re.findall(r'graphNr(\d{1,2})', str(filename))[0]
        print("calculate metrics for " + filename)
        print("graphNr: " + str(graphNr))
        graphMasks, firstT, lastT = readInTxtFile(directoryGraphComponents+"\\"+filename)
        metricZero = 0.0
        metricOne = 0.0
        #metricTwo = 0.0
        #metricThree = 0.0
        metricFour = 0.0
        metricFive = 0.0
        metricSix = 0.0
        metricSeven = 0.0
        metricEight = 0.0
        metricNine = 0.0
        metricTen = 0.0
        T = lastT-firstT+1
        print("first " + str(firstT) + " T " + str(T))
        for t in range(T):
            #auf graphMasks[t] metrics anwenden und speichern
            arr = graphMasks[t+firstT]
            metricZero += cloudmetrics.mask.cloud_fraction(arr)
            metricOne += cloudmetrics.mask.fractal_dimension(arr)
            #metricTwo += cloudmetrics.mask.orientation(arr)
            #metricThree += cloudmetrics.mask.objects.cop_objects(arr, periodic_domain=False)
            metricFour += cloudmetrics.mask.objects.iorg_objects(arr, periodic_domain=False)
            metricFive += cloudmetrics.mask.objects.max_object_length_scale(arr, periodic_domain=False)
            #metricFive += 0
            metricSix += cloudmetrics.mask.objects.mean_object_eccentricity(arr, periodic_domain=False)
            metricSeven += cloudmetrics.mask.objects.mean_object_length_scale(arr, periodic_domain=False)
            metricEight += cloudmetrics.mask.objects.mean_object_perimeter_length(arr, periodic_domain=False)
            metricNine += cloudmetrics.mask.objects.mean_object_length_scale(arr, periodic_domain=False)
            metricTen += cloudmetrics.mask.objects.scai_objects(arr, periodic_domain=False)
            del arr 
            gc.collect()
        cloudmetricsOnGraphs[0] = metricZero/T
        cloudmetricsOnGraphs[1] = metricOne/T
        #cloudmetricsOnGraphs[2] = metricTwo /T
        #cloudmetricsOnGraphs[3] = metricThree/T
        cloudmetricsOnGraphs[2] = metricFour/T
        cloudmetricsOnGraphs[3] = metricFive/T
        cloudmetricsOnGraphs[4] = metricSix/T
        cloudmetricsOnGraphs[5] = metricSeven/T
        cloudmetricsOnGraphs[6] = metricEight/T
        cloudmetricsOnGraphs[7] = metricNine/T
        cloudmetricsOnGraphs[8] = metricTen/T

        del graphMasks
        gc.collect()
        
        saveCloudmetricsOnGraphsToTxtFile(cloudmetricsOnGraphs, "graphNr"+str(graphNr)+"_cloudmetrics")
        del cloudmetricsOnGraphs
        gc.collect()
        del metricZero 
        gc.collect()
        del metricOne
        gc.collect()
        del metricFour
        gc.collect()
        del metricFive
        gc.collect()
        del metricSix
        gc.collect()
        del metricSeven
        gc.collect()
        del metricEight
        gc.collect()
        del metricNine
        gc.collect()
        del metricTen
        gc.collect()


calculateGraphMetricsForGraphs()