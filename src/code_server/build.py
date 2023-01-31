

# Mom, can we get CMAKE?
# No, sweetie, we have CMAKE at home.
# The CMAKE at home:

import os
import subprocess
from typing import List, Tuple

DOING = ""

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


def build_parser():
    global DOING

    _cwd = os.getcwd()
    os.chdir(os.path.join(os.path.dirname(__file__), 'flex'))

    for f in [ "lex.yy.c", "y.tab.c", "y.tab.h" ]:
        if os.path.exists(f):
            os.remove(f)

    DOING = "building parser"
    subprocess.check_call([ 'bison', '-dy', os.path.join(os.path.dirname(__file__), 'flex', 'parser.yacc') ]) # If errors, run with the -v flag
    DOING = "building lexical analyzer"
    subprocess.check_call([ 'flex', os.path.join(os.path.dirname(__file__), 'flex', 'parser.lex') ])

    os.chdir(_cwd)

def construct_linker_list(files):
    return [ os.path.splitext(os.path.basename(f))[0] + '.o' for f in files ]

def build_server():
    global DOING

    # Find all files
    base_dir = os.path.dirname(__file__)
    cwd = os.getcwd()
    os.chdir(os.path.join(base_dir, 'build_objects'))
    cfiles = get_all_files_by_ext(base_dir, ('.c'))
    cppfiles = get_all_files_by_ext(base_dir, ('.cpp'))

    # Required libraries
    libs = [
        "ws2_32"
    ]

    proc_compile_c = [ 'gcc', '-std=c99', '-c', *cfiles, '-I', base_dir ]
    proc_compile_cpp = [ 'g++', '-c', *cppfiles, '-I', base_dir ]
    proc_link = [ 'g++', '-o', os.path.join(base_dir, 'server.exe'), *construct_linker_list(cppfiles + cfiles), *[ f'-l{lib}' for lib in libs ] ]

    DOING = "building C files"
    subprocess.check_call(proc_compile_c)
    DOING = "building C++ files"
    subprocess.check_call(proc_compile_cpp)
    DOING = "linking object files"
    subprocess.check_call(proc_link)
    
    os.chdir(cwd)

def build():
    try:
        build_parser()
        build_server()
    except subprocess.CalledProcessError:
        print(f"Build failed while {DOING}")

if __name__ == "__main__":
    build()
