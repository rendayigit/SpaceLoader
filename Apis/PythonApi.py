from subprocess import Popen, PIPE
import threading

def commandsThread(p):
    while(True):
        command = input() + "\n"
        p.stdin.write(command)
        p.stdin.flush()

def receiverThread(p):
    while(True):
        line = p.stdout.readline()
        if not line:
            break
        print(line)

location = "../scripts/run client.bat"

p = Popen(location, stdin = PIPE, stdout = PIPE, stderr = PIPE, encoding = "UTF8")

x = threading.Thread(target = commandsThread, args=(p,))
x.start()

y = threading.Thread(target = receiverThread, args=(p,))
y.start()