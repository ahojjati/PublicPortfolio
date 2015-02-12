
import os
import sys


os.system('gcc /home/dev/mp6/vulnerable.c -g -std=c99 -o /home/dev/mp6/vulnerable')
os.system('cd /home/dev/mp6')
os.system('env - gdb /home/dev/mp6/vulnerable')
