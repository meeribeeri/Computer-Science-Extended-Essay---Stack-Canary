from pwn import *
import time

executable = input("Input the name of the file to be run. This is 5 tests: ")

times = []

for i in range(5):
    start_time = time.time()
    p = process(f"{executable}.exe")
    p.recvuntil(b"Input: ")
    p.sendline("1")
    p.recvuntil("Final Time: ")
    p.kill()
    end_time = time.time()
    times.append([start_time,end_time,end_time-start_time])

print(times)
