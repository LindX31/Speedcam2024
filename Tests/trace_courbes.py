import matplotlib.pyplot as plt
import numpy as np

file_name = 'cam_data.txt'

tableau = np.genfromtxt(file_name, delimiter=';')

temps = tableau[:, 0]
xRobot = tableau[:, 1]
yRobot = tableau[:, 2]
xCible = tableau[:, 3]
yCible = tableau[:, 4]

def trace():
    plt.plot(temps, xRobot, color='r', label='Robot')
    plt.plot(temps, xCible, color='g', label='Cible')
    plt.xlabel("Temps")
    plt.ylabel("Position")
    plt.title("Entrée et réponse du système à une consigne de position (caméra)")

    plt.legend()
    plt.show()

#print(temps)
trace()