import serial
import time

arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.1)
