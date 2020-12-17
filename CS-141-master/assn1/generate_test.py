import random
import sys

if len(sys.argv) < 2:
    print("Proper usage: python generate_test.py #")
    exit()

n = sys.argv[1]

test_file = open('input'+str(n)+'.txt','w')

XMIN=YMIN=-500
XMAX=YMAX=500

random.SystemRandom()
for i in range(int(n)):
    test_file.write(str(random.uniform(XMIN,XMAX))+' '+str(random.uniform(YMIN,YMAX)))
    test_file.write('\n')

