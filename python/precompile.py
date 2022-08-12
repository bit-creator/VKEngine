from enum import Enum, auto
from genericpath import isfile
import os
import subprocess
from time import sleep
import threading
import syscall
import output
import sys

def name_of_module(string):
    return string.partition('export module ')[2].partition(';')[0]
    
def name_of_import(string):
    return string.partition('import ')[2].partition(';')[0]


def ignore_module(module):
    ignore_list = [
        'Vulkan',
        'GLFW',
        'Image'
    ]

    for mod in ignore_list:
        if module == mod:
            return True
    
    if module.find('<') != -1:
        return True
    if module.find('std') != -1:
        return True
    

def import_locator(lines):
    name = ''
    dependency =[]
    for line in lines:
        if line.find('export module') != -1:
            name = name_of_module(line)
    
    for line in lines:
        if line.find('import') != -1:
            imp = name_of_import(line)
            if not ignore_module(imp):
                dependency.append(imp)
    return (name, dependency)

rootList = ['src', 'Mathon/src']
modules = {}
pathes = {}
status = {}

class Status(Enum):
    Undefined   = auto()
    Located     = auto()
    Ready       = auto()
    Precompiled = auto()

def iterate_dir(root):
    for filename in os.listdir(root):
        if os.path.isfile(os.path.join(root, filename)):
            input = open(os.path.join(root, filename))
            name, dependency = import_locator(input.readlines())
            modules[name] = dependency
            pathes[name] = str(os.path.join(root, filename))
        else: iterate_dir(os.path.join(root, filename))

def generate_status(module, dependency):
    for name in dependency:
        generate_status(name, modules[name])
    status[module] = Status.Undefined;

for root in rootList:
    iterate_dir(root)

def wait_dep(dependency):
    if len(dependency) == 0:
        return

    dep = []
    for depend in dependency:
        dep.append(status[depend])

    while(dep.count(Status.Precompiled) != len(dep)):
        dep = []
        for depend in dependency:
            dep.append(status[depend])

dynamic_line_buff = {}

def start_if_ready(module, dependency, thread):
    if status[module] == Status.Ready: return
    if status[module] == Status.Precompiled: return

    if len(dependency) == 0:
        update_dynamic_line('compile module ' + module)
        thread.start()
        status[module] = Status.Ready
        return

    dep = []
    for depend in dependency:
        dep.append(status[depend])
    if dep.count(Status.Precompiled) == len(dep):
        update_dynamic_line('compile module ' + module)
        thread.start()
        status[module] = Status.Ready
        return

threads = {}

print('\033[?25l', end="")

def update_dynamic_line(text):
    count_preb = str(list(status.values()).count(Status.Precompiled))
    counter_line = output.bcolors.COUNTER + '[' + count_preb + ' / ' + str(len(status)) + ']' + output.bcolors.ENDC
    dynamic_line = counter_line + ' '*3 + output.bcolors.OKCYAN + text + output.bcolors.ENDC
    print(dynamic_line, end='\r')
    output.clear()
    dynamic_line_buff['xxxxx'] = dynamic_line

def erase_precompiled():
    updated = {}
    for module in threads.keys():
        if status[module] != Status.Precompiled:
            updated[module] = threads[module]
        else:
            output.sucsess('precompiled module ' + module)
            print(dynamic_line_buff['xxxxx'], end='\r')
            output.clear()
    return updated

def precompile(module, dependency):
    status[module] = Status.Located
    for name in dependency:
        if status[name] == Status.Undefined:
            precompile(name, modules[name])

    def pc(module, path):
        # subprocess.run(['./echo.sh', module])
        syscall.prebuild(module, path)
        status[module] = Status.Precompiled
        
    threads[module] = threading.Thread(target=pc, args=[module, pathes[module]])
    

generate_status('Main', modules['Main'])
precompile('Main', modules['Main'])

while status['Main'] != Status.Precompiled:
    for [module, thread] in threads.items():
        start_if_ready(module, modules[module], thread)
    threads = erase_precompiled()

print('\033[?25h', end="")