from sre_constants import SUCCESS
import sys

LINE_CLEAR = '\x1b[2K'

class bcolors:
    ENDC      = '\033[0m'
    BOLD      = '\033[1m'
    UNDERLINE = '\033[4m'
    COUNTER   = '\033[7m'
    BLUE      = '\033[34m'
    SUCCESS   = '\033[42m'
    DARK      = '\033[90m'
    FAIL      = '\033[91m'
    OKGREEN   = '\033[92m'
    WARNING   = '\033[93m'
    OKBLUE    = '\033[94m'
    HEADER    = '\033[95m'
    OKCYAN    = '\033[96m'

def clear():
    print(end=LINE_CLEAR)


def dynamic_color_output(bcolor, text):
    print(bcolor + text + bcolors.ENDC, end='\r')
    print(end=LINE_CLEAR)

def static_color_output(bcolor, text):
    print(bcolor + text + bcolors.ENDC)
    # print(end=LINE_CLEAR)

# def warning(text):
    # dynamic_color_output(bcolors.WARNING, text)

def sucsess(text):
    print(bcolors.OKGREEN + '[SUCSESS]' + bcolors.ENDC, end=3*' ')
    static_color_output(bcolors.HEADER, text)

# def counter(text):
    # print(bcolors.COUNTER + text + bcolors.ENDC, end='\r')
    