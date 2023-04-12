

# Mom, can we get CMAKE?
# No, sweetie, we have CMAKE at home.
# The CMAKE at home:

import os
import re
import subprocess
import threading
import queue
from typing import List, Tuple, Dict

from hashlib import md5
from _hashlib import HASH

DOING = ""

class DependancyTree:
    def __init__(self) -> None:
        self.map: Dict[str, List[str]] = {}
    def add(self, obj: str, depends_on: str) -> None:
        if depends_on not in self.map:
            self.map[depends_on] = []
        if obj not in self.map[depends_on]:
            self.map[depends_on].append(obj)
    def getDependants(self, obj: str) -> List[str]:
        if obj not in self.map:
            return []
        return self.map[obj]

def get_all_files_by_ext(directory: str, exts: List[str] | Tuple[str]) -> List[str]:
    out = []

    for cont in os.listdir(directory):
        path = os.path.join(directory, cont)

        if os.path.isfile(path):
            if path.endswith(exts):
                out.append(path)
        elif os.path.isdir(path):
            out += get_all_files_by_ext(path, exts)
    
    return out

def filter_to_out_of_date_hashes(files: List[str]) -> List[Tuple[str, str, str]]:
    out = []
    for file in files:
        with open(file, 'rb') as f:
            hash = md5(f.read(), usedforsecurity=False).hexdigest()
        hash_path = os.path.basename(file) + '.md5'
        if os.path.exists(hash_path):
            with open(hash_path, 'r') as f:
                if f.read() != hash:
                    out.append((hash, hash_path, file))
        else:
            out.append((hash, hash_path, file))
    return out

def save_hashes(files: List[Tuple[str, str, str]]) -> None:
    for hash_file in files:
        hash, hash_path, file = hash_file
        with open(hash_path, 'w') as f:
            f.write(hash)

def build_parser():
    global DOING

    _cwd = os.getcwd()

    yacc_path = os.path.join(os.path.dirname(__file__), 'flex', 'parser.yacc')
    lex_path = os.path.join(os.path.dirname(__file__), 'flex', 'parser.lex')

    os.chdir(os.path.join(os.path.dirname(__file__), 'build_objects'))
    hashes = filter_to_out_of_date_hashes([ yacc_path, lex_path ])

    if len(hashes) > 0:
        print("Building flex/bison")
        os.chdir(os.path.join(os.path.dirname(__file__), 'flex'))
        for f in [ "lex.yy.c", "y.tab.c", "y.tab.h" ]:
            if os.path.exists(f):
                os.remove(f)

        DOING = "building parser"
        subprocess.check_call([ 'bison', '-dy', yacc_path ]) # If errors, run with the -v flag
        DOING = "building lexical analyzer"
        subprocess.check_call([ 'flex', lex_path ])

        os.chdir(os.path.join(os.path.dirname(__file__), 'build_objects'))
        save_hashes(hashes)

    os.chdir(_cwd)

def construct_linker_list(files):
    return [ os.path.splitext(os.path.basename(f))[0] + '.o' for f in files ]

def appendToDepTree(dep: DependancyTree, file: str) -> List[str]:
    with open(file, encoding='utf8') as f:
        for match in re.finditer(r'#include\s*"([^"]+)"', f.read(), re.UNICODE):
            dep.add(os.path.basename(file), os.path.basename(match.group(1)))

def filterToMatching(files: List[str], buildlist: List[str]):
    return [ f for f in files if os.path.basename(f) in buildlist ]

def build_server():
    global DOING

    deps = DependancyTree()

    # Find all files
    base_dir = os.path.dirname(__file__)
    cwd = os.getcwd()
    os.chdir(os.path.join(base_dir, 'build_objects'))
    cfiles = get_all_files_by_ext(base_dir, ('.c'))
    cppfiles = get_all_files_by_ext(base_dir, ('.cpp'))

    allfiles = get_all_files_by_ext(base_dir, ('.h', '.hpp')) + cfiles + cppfiles

    changed_files = filter_to_out_of_date_hashes(allfiles)

    if len(changed_files) > 0:
        # Build dep tree
        to_build = [ os.path.basename(f[2]) for f in changed_files ]

        for file in allfiles:
            appendToDepTree(deps, file)

        index = 0
        while index < len(to_build):
            to_build += [ d for d in deps.getDependants(to_build[index]) if d not in to_build ]
            index += 1

        # Required libraries
        libs = [
            "ws2_32"
        ]

        errors = queue.Queue()
        to_build_c = filterToMatching(cfiles, to_build)
        to_build_cpp = filterToMatching(cppfiles, to_build)

        def cmd_build_c(file):
            try:
                subprocess.check_call([ 'gcc', '-g', '-std=c99', '-c', file, '-I', base_dir ])
            except Exception as e:
                errors.put(e)
        def cmd_build_cpp(file):
            try:
                subprocess.check_call([ 'g++', '--std=c++17', '-g', '-c', file, '-I', base_dir ])
            except Exception as e:
                errors.put(e)
        
        proc_link = [ 'g++', '-o', os.path.join(base_dir, 'server.exe'), *construct_linker_list(cppfiles + cfiles), *[ f'-l{lib}' for lib in libs ] ]

        DOING = "building C/C++ files"
        procs = [  ]
        if len(to_build_c) > 0:
            print("Building C Files", *filterToMatching(cfiles, to_build), sep='\n')
            for file in to_build_c:
                procs.append(threading.Thread(target=cmd_build_c, args=(file,)))
        if len(to_build_cpp) > 0:
            print("Building C++ Files", *filterToMatching(cppfiles, to_build), sep='\n')
            for file in to_build_cpp:
                procs.append(threading.Thread(target=cmd_build_cpp, args=(file,)))
        
        for proc in procs:
            proc.start()
        for proc in procs:
            proc.join()
        
        if not errors.empty():
            raise errors.get()
        
        DOING = "linking object files"
        print("Linking")
        subprocess.check_call(proc_link)
        
    save_hashes(changed_files)

    os.chdir(cwd)

def build():
    try:
        build_parser()
        build_server()
    except subprocess.CalledProcessError:
        print(f"Build failed while {DOING}")

if __name__ == "__main__":
    build()
