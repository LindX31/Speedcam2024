import serial
import time

# Il faut ici éventuellement remplacer 'COM4' par celui indiqué en bas à droite de la 
# fenêtre arduino, on ajustera le baudrate plus tard pour implémenter le vrai programme
arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.1)

def write_read(x):
    arduino.write(bytes(x, 'utf-8')) # envoie x sous forme binaire à la carte
    time.sleep(0.05) # on laisse à arduino le temps de traiter l'information
    data = arduino.readline() # lit le message affiché par arduino dans le moniteur série
    return data

while True:
    num = input("Saisir un nombre: ")
    value = write_read(num)
    print(value) 