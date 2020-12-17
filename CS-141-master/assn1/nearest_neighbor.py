import random
import sys
import math
import os
import time

# code given to us by professor
class Point :
    def __init__(self, x_val, y_val):
        self.x = x_val
        self.y = y_val

    def __repr__(self):
        return "(%.2f, %.2f)" % (self.x, self.y)

def Read_Points_From_Command_Line_File():
    points = []
    number_of_args = len(sys.argv)
    file = open(sys.argv[1],"r")

    for line in file:
        line.strip()
        x_y = line.split(" ")
        points.append(Point(float(x_y[0]),float(x_y[1])))

    return points

def Write_to_File(filename, s):
    output = open(filename ,'w')
    output.write(str(s))
    output.write('\n')


# end of professor's code


# brute force way of solving closest points
def domath(a):
    counter = float('inf')
    for i in range(0, len(a)):
        for j in range(i + 1, len(a)):
            u = math.sqrt(pow((a[j].x - a[i].x), 2) + pow((a[j].y  - a[i].y), 2))
            if counter > u:
                counter = u
    return counter
        
# updated brute force way of solving closest points    
def domath2(a): 
    counter = float('inf')
    u = float('inf')
    for i in range(0, len(a)):
        for j in range(i + 1, i + 8):  # checking next 7 only
            if j < len(a):
                u = math.sqrt(pow((a[j].x - a[i].x), 2) + pow((a[j].y  - a[i].y), 2))
            if counter > u:
                counter = u    
    return counter
        
# divide and conquer alogrithm 
def dacalgo(a):
    u = float('inf')
    # code to sort our x values in Point
    a = sorted(a, key=lambda Point: Point.x)  # code from stackoverflow tweaked with professor help
    length = len(a)  
    # get middle length
    mid1 = length / 2
    # get middle x num
    mid = a[mid1].x
    
    # base cases
    # base case of less than 2
    # base case of 2 = just do distance formula on it
    # else = divide and conquer recursion
    if len(a) < 2:
        return float('inf')
    elif len(a) == 2:
        u = math.sqrt(pow((a[1].x - a[0].x), 2) + pow((a[1].y  - a[0].y), 2))
        return u
    else:
        #break into two and call function again until size hits 1 or 2
        l = a[:mid1]    # spilt 
        r = a[mid1:]    # spilt
        l1 = dacalgo(l) # get smallest left pair
        r1 = dacalgo(r) # get smallest right pair
        d = min(l1,r1)  # get min between both
        # go through list and just get "square" points, set to inf other points
        for i in range(0, len(a)): 
            if a[i].x < (mid - d) and a[i].x > (mid + d): 
                a[i].x = float('inf')
                a[i].y = float('inf')
        #remove infinity from our list
        a = [x22 for x22 in a if x22.x != float('inf')] # code from stackoverflow that removes all instances of a number from a list
        # code to sort our points by y value
        a = sorted(a, key=lambda Point: Point.y) # code from stackoverflow tweaked with professor help
        b = domath2(a)
        d = min(d,b)
        return d
      
# code modified from professor
if len(sys.argv) < 2:
    print("Proper usage: python nearest_neighbor.py example.txt")
    exit()
  
# get input list
list = Read_Points_From_Command_Line_File()
a = list

#brute force method
b = domath(a) 
# divide and conquer method
c = dacalgo(a)
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
test2 = test2 + '_distance.txt'
# write to file, code given by professor
Write_to_File(test2, c)











