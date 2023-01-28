import subprocess

if __name__ == "__main__":
    subprocess.call([
        'python',
        'run-clang-format.py', '-r', 'src', 'include'
    ])
    subprocess.call([
        'python',
        'run-clang-format.py', '-r', '-i', 'src', 'include'
    ])
