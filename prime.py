from pwn import *
import time

executable = input("Input the name of the file to be run. This is 5 tests: ")

times = []

for i in range(5):
    
    p = process(f"./{executable}")
    p.recv()
    p.sendline(b"3")
    p.recv()
    time.sleep(2)
    p.close()

    start_time = time.time()
    p = process(f"./{executable}")
    p.recv()
    p.sendline(b"1")
    p.recvuntil(b"Final Time: ")
    p.close()
    end_time = time.time()
    times.append([start_time,end_time,end_time-start_time])

print(times)
try:
    file = open(f"{executable}-prime-data.txt","w")
    file.write("Start Time\t\t\tEnd Time\t\t\tDelta Time\n")
    ave = 0
    for data in times:
        file.write(f"{data[0]}\t{data[1]}\t{data[2]}\n")
        ave+=data[2]
    file.write(f"Average Delta Time: {ave/5}\n")
    file.close()
except:
    print("Tests must be redone")
