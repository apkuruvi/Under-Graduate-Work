import random
import sys
import math
import os
import time

# code given to us by professor modified for assignment
def Read_Points_From_Command_Line_File():
    points = []
    number_of_args = len(sys.argv)
    file = open(sys.argv[1],"r")

    for line in file:
        line.strip()
        x_y_z = line.split(", ")
        for i in range(0, len(x_y_z)):
            points.append(float(x_y_z[i]))
    return points

# code given to us by professor modified to just return length of row
def Read_Points_From_Command_Line_File1():
    points = []
    number_of_args = len(sys.argv)
    file = open(sys.argv[1],"r")

    for line in file:
        line.strip()
        x_y_z = line.split(", ")
    return len(x_y_z)
    
    
def Write_to_File(filename, s,final1,a16,a15): # file array number index size- used for 
    output = open(filename ,'w')
    output.write("Min seam is: ")
    output.write("%s\n" % final1)
    counter = len(s) - 1
    for i in range(0,len(s)):
        output.write("[ ")
        output.write("%s" % counter)
        counter = counter - 1
        output.write(", ")
        temp = a16[i]
        while temp >= a15:
            temp = temp - a15
        counter90 = a15 - 1
        counter91 = 0
        for x in range(0, a15):
            if temp == counter90:
                pos = counter91
                x = a15 - 1
            else:
                counter90 = counter90 - 1
                counter91 = counter91 + 1
        output.write("%s" % pos)
        output.write(", ")
        output.write("%s" % s[i])
        output.write("%s\n" % " ]")  # for reading bot to top adjusting index 


def Write_to_File1(filename, s,final1,a16,a15): # file array number index size
    output = open(filename ,'w')
    output.write("Min seam is: ")
    output.write("%s\n" % final1)
    counter = 0
    for i in range(0,len(s)):
        output.write("[ ")
        output.write("%s" % counter)
        counter = counter + 1
        output.write(", ")
        temp = a16[i]
        while temp >= a15:
            temp = temp - a15
        output.write("%s" % temp)
        output.write(", ")
        output.write("%s" % s[i])
        output.write("%s\n" % " ]")

# brute force way of solving seam through column
def domath(a20,index,a1,a15,a16,a):
    index1 = index + 1
    indexkeeper = 0
    counter70 = 0
    counter71 = 0
    counter72 = 0
    ans = float('inf')
    if index1 % a15 == 1:
          if((index + (a15 + 1)) <= len(a)):
            b = a20[index + a15]
            bright = a20[index + (a15 + 1)]
            ans = min(b,bright)
            if ans == b:
                ans = a[index + a15]
                indexkeeper = index + a15
            else:
                ans = a[index + a15 + 1]
                indexkeeper = index + a15 + 1
            a1.append(ans)
            a16.append(indexkeeper)
    elif index1 % a15 == 0:
        if((index + a15) <= len(a)):
            b = a20[index + a15]
            bright = a20[index + (a15 - 1)]
            ans = min(b,bright)
            if ans == b:
                ans = a[index + a15]
                indexkeeper = index + a15
            else:
                ans = a[index + (a15 - 1)]
                indexkeeper = index + (a15 - 1)
            a1.append(ans)
            a16.append(indexkeeper)
    elif((index1 + (a15 + 1)) <= len(a)):
        c = a20[index + (a15 - 1)]
        b = a20[index + a15]
        bright = a20[index + (a15 + 1)]
        ans = min(c,b,bright)
        if ans == b:
            ans = a[index + a15]
            indexkeeper = index + a15
        elif ans == c:
            ans = a[index +  (a15 - 1)]
            indexkeeper = index + (a15 - 1)
        else:
            ans = a[index + a15  + 1]
            indexkeeper = index + a15 + 1
        a1.append(ans)
        a16.append(indexkeeper)
    if indexkeeper == 0:
        return (a1,a16)            
    else:
        return domath(a20,indexkeeper,a1,a15,a16,a)

# calls the minseam algorithm iterating through the first row down
def domath1(a20,a15,a):
    counter = 0
    a2 = []
    a1= []
    a3 = []
    a4 = []
    a5 = []
    counter2 = float('inf')
    for i in range(0, a15):
        a4 = []
        a3 = []
        counter = 0;
        a3.append(a[i])
        a4.append(i)
        a1,a4 = domath(a20,i,a3,a15,a4,a) # list, index, array to hold val, num col, array to hold index, original array
        for j in range(0, len(a1)):
            counter = counter + a1[j]  # get value of returned array of energies
        if counter2 > counter:
            counter2 = counter
            a2 = a1 
            a5 = a4
    return (a2,a5)
    
# go through list adding up list energies    
def setlist(a,index,a15,b):
    index1 = index + 1
    for i in range(a15, len(b)):
        ans = 0
        temp = 0
        temp1 = 0
        temp2 = 0
        if index1 % a15 == 1:
             if((index - a15) >= 0):
                 temp = b[index - a15]
                 temp1 = b[index - a15 + 1]
                 ans = min(temp,temp1)
                 b[index] = b[index] + ans
        elif index1 % a15 == 0:
            if((index - a15 - 1) >= 0):
                temp = b[index - a15]
                temp1 = b[index - a15 - 1]
                ans = min(temp,temp1)
                b[index] = b[index] + ans
        elif((index1 - (a15 - 1)) >= 0):
            temp = b[index - a15]
            temp1 = b[index - a15 - 1]
            temp2 = b[index - a15 + 1]
            ans = min(temp,temp1,temp2)
            b[index] = b[index] + ans
        index = index + 1
        index1 = index1 + 1
    return b        
    
    
# code modified from professor
if len(sys.argv) < 2:
    print("Proper usage: python nearest_neighbor.py example.txt")
    exit()
  
# get input list
list = Read_Points_From_Command_Line_File()
a = Read_Points_From_Command_Line_File()
b = list
# get name of file
test1 = sys.argv[1]
test2 = ""
counter = 0
# loop to get everything up to '.'
for i in range(0,len(test1)):
    if test1[i] != '.' and counter == 0:
        test2 = test2 + test1[i] 
    else:
        counter = 1
# add "_distance.txt"
test2 = test2 + '_output.txt'

a15 = Read_Points_From_Command_Line_File1()
a20 = setlist(a,a15,a15,b)
a20.reverse()
a.reverse()
p,a16 = domath1(a20,a15,a)

tmpc = 0
final = []
final1 = 0
for j in range(0, len(p)):
    tmpc = tmpc + p[j]  # get value of returned array of energies
final1 = tmpc
final = p
# write to file, code given by professor but modified to fit assignment
Write_to_File(test2, final,final1,a16,a15) # file array  number index, col
