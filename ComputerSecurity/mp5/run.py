
import os
import sys

if len(sys.argv) > 1 and sys.argv[1] == 'bonus':
    os.system('gcc /home/dev/mp5/vulnerable.c -z execstack -fstack-protector -g -o /home/dev/mp5/vulnerable')
else:
    os.system('gcc /home/dev/mp5/vulnerable.c -z execstack -fno-stack-protector -g -o /home/dev/mp5/vulnerable')
os.system('cd /home/dev/mp5')
os.system('env - gdb /home/dev/mp5/vulnerable')
