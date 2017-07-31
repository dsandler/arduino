#!/usr/bin/python

import sys
import serial
import time
import urllib

from BaseHTTPServer import BaseHTTPRequestHandler,HTTPServer

device = None

class handler(BaseHTTPRequestHandler):
  def do_GET(self):
    global device
    if self.path:
      self.send_response(200)
      self.send_header('Content-type', 'text/plain')
      self.end_headers()
      device.write(urllib.unquote(self.path[1:])+'\n')
      time.sleep(0.1)
      self.wfile.write(device.read_all())

def readserial(device):
  while device.readable():
    sys.stdout.write(device.read_all())
    sys.stdout.flush()

def main():
  global device
  device = serial.Serial()
  device.port=sys.argv[1]
  device.baudrate=9600
  device.open()

  http_port = int(sys.argv[2])

  server = HTTPServer(('', http_port), handler)

  try:
    server.serve_forever()
  except KeyboardInterrupt:
    print '^C received, shutting down the web server'
    server.socket.close()
    device.close()

if __name__ == '__main__':
  main()
