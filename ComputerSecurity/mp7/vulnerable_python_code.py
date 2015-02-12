import os,sys

BOARD_SIZE = 8
PATH = ""

class BailOut(Exception):
    pass

def set_var():
    uname = sys.argv[1]
    os.environ["BOARDSIZE"] = "8"
    os.environ["USERNAME"] = uname

def get_var():
    global BOARD_SIZE
    global PATH
    BOARD_SIZE = int(os.environ["BOARDSIZE"])
    PATH = os.environ["PATH"]

def validate(queens):
    left = right = col = queens[-1]
    for r in reversed(queens[:-1]):
        left, right = left-1, right+1
        if r in (left, col, right):
            raise BailOut

def add_queen(queens):
    for i in range(BOARD_SIZE):
        test_queens = queens + [i]
        try:
            validate(test_queens)
            if len(test_queens) == BOARD_SIZE:
                return test_queens
            else:
                return add_queen(test_queens)
        except BailOut:
            pass
    raise BailOut

def error_message_queen():
    print ("\nError: Incorrect input argument.\n\nUsage: python <this executable script> <printable Upper Case ASCII string as Username>\n\n")

set_var()
print ("CURRENT WORKING DIR IS: ")
os.system("pwd")

get_var()
print ("CURRENT PATH IS: ")
print (PATH)

queens = add_queen([])
print(queens)
print("\n".join(". "*q + "Q " + ". "*(BOARD_SIZE-q-1) for q in queens))
