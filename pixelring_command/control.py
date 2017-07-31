#!/usr/bin/python
# usage: reset_arduino <DEVICE>
# where <DEVICE> is typically some /dev/ttyfoobar
import sys
import serial
import thread

def readserial(ser):
  while ser.readable():
    sys.stdout.write(ser.read_all())
    sys.stdout.flush()

def main():
  ser = serial.Serial()
  ser.port=sys.argv[1]
  ser.baudrate=9600
  ser.open()

  thread.start_new_thread(readserial, (ser,))

  ser.write('\n')
  while True:
    ser.write(sys.stdin.readline())

if __name__ == '__main__':
  main()
