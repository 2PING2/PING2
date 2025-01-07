import serial
import serial.tools.list_ports
import struct
import pandas as pd

# Fonction pour détecter et sélectionner le port série
def select_serial_port(baud_rate=115200):
    ports = serial.tools.list_ports.comports()
    ports = [port for port in ports if "Bluetooth" not in port.description]
    
    if len(ports) == 1:
        print("Automatically using the only available serial port:")
        print(f"-> {ports[0].device}")
        return serial.Serial(ports[0].device, baud_rate)
    elif len(ports) > 1:
        print("Please select the serial port to use:")
        for i, port in enumerate(ports):
            print(f"{i}: {port.device} - {port.description}")
        port_index = int(input("Port index: "))
        return serial.Serial(ports[port_index].device, baud_rate)
    else:
        raise Exception("No available serial ports detected. Check your connection.")

# Paramètres du script
BAUD_RATE = 115200
SAMPLE_SIZE = 15  # Taille d'un échantillon (8 + 4 + 1 + 2 octets)

# Initialisation des variables
header_read = False
data = []
header = {}

# Connexion série avec sélection automatique
try:
    ser = select_serial_port(BAUD_RATE)
    print("If the test does not start by itself, press the reset button on the ESP32.")
except Exception as e:
    print(e)
    exit()


# Lecture des données
try:
    while True:
        raw_line = ser.readline().rstrip()
        if not header_read:
            # Lire le header
            header_line = raw_line.decode("utf-8").strip()
            if header_line.startswith("HEADER:"):
                print(f"Header received: {header_line}")
                
                # Analyser le header et stocker les informations sur les types de données
                header_details = header_line[len("HEADER:"):].split(',')
                for item in header_details:
                    var_name, var_type = item.split('<')
                    header[var_name] = var_type.strip('>')

                header_read = True
        else:
            # Lire les données binaires
            if len(raw_line) == SAMPLE_SIZE:
                # Décoder les données en fonction du header dynamique
                data_values = {}
                offset = 0
                for var_name, var_type in header.items():
                    if var_type == "int32":
                        value = struct.unpack('<i', raw_line[offset:offset+4])[0]
                        offset += 4
                    elif var_type == "uint8":
                        value = struct.unpack('<B', raw_line[offset:offset+1])[0]
                        offset += 1
                    elif var_type == "uint16":
                        value = struct.unpack('<H', raw_line[offset:offset+2])[0]
                        offset += 2
                    elif var_type == "int64":
                        value = struct.unpack('<q', raw_line[offset:offset+8])[0]
                        offset += 8
                    data_values[var_name] = value

                data.append(data_values)

            # Vérifier si la fin de la transmission est détectée
            if raw_line==b"END_OF_TRANSMISSION":
                break

except KeyboardInterrupt:
    print("Data acquisition interrupted by user.")
finally:
    # Fermer le port série proprement
    ser.close()

# Création du DataFrame
df = pd.DataFrame(data)
print(df.head())

# Sauvegarde dans un fichier CSV (optionnel)
df.to_csv("stall_guard_data.csv", index=False)
