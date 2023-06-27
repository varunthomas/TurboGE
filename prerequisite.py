import subprocess
import os
import sys

python_path = os.path.dirname(sys.executable)
#python_libs = python_path + "\libs"
#os.environ["PYTHON_LIBS"] = python_libs

print(os.getenv("PYTHON_LIBS"))

packageNames = ["requests"]  # Replace with your package names

for packageName in packageNames:
    try:
        subprocess.check_call(['python', '-m', 'pip', 'install', packageName])
        print(f"Successfully installed {packageName}")
    except subprocess.CalledProcessError as e:
        print(f"Failed to install {packageName}: {e}")