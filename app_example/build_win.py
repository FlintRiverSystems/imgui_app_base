
import os, sys, shutil

print (f'FRSPROJ : {os.getenv("FRSPROJ")}')
print (f'FRSTPSRC: {os.getenv("FRSTPSRC")}')

if os.path.isdir('build.win'):
	shutil.rmtree('build.win');

os.mkdir('build.win')
os.chdir('build.win')

print ("Configuring...")
rc = os.system('cmake -A x64 ..')
if rc != 0: sys.exit(rc)

print ("Building...")
#rc = os.system('cmake --build . --config Release')
rc = os.system('cmake --build .')
if rc != 0: sys.exit(rc)

print ("Done")


