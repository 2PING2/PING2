""" Ce fichier permet de récupérer les information de volume, mode de jeu, la difficulté choisi par l'utilisateur et de les transmettre à la classe Game. """
import serial
import pyudev
import threading

class Communication :
    def __init__(self):
        self.device = {}
        self.running = True

    def detect_and_connect(self):
        """
        Détecte les périphériques série branchés et les connecte automatiquement.
        """
        context = pyudev.Context()
        for device in context.list_devices(subsystem='tty'):
            port = device.device_node
            if port not in self.devices:  # Si le port n'est pas encore connecté
                try:
                    conn = serial.Serial(port=port, baudrate=9600, timeout=1)
                    self.devices[port] = {'connection': conn, 'data': None}
                    print(f"Connecté au périphérique sur {port}")
                except Exception as e:
                    print(f"Impossible de se connecter au port {port}: {e}")

    def read_from_device(self, port):
        """
        Lit les données d'un périphérique spécifique.
        """
        conn = self.devices[port]['connection']
        while self.running:
            try:
                if conn.in_waiting > 0:
                    data = conn.readline().decode('utf-8').strip()
                    self.devices[port]['data'] = data
                    print(f"{port}: {data}")
            except Exception as e:
                print(f"Erreur de lecture sur {port}: {e}")

    def start_reading(self):
        """
        Lance un thread pour lire les données de chaque périphérique connecté.
        """
        for port in self.devices:
            thread = threading.Thread(target=self.read_from_device, args=(port,))
            thread.daemon = True
            thread.start()

    def monitor_usb_events(self):
        """
        Surveille les événements de branchement/débranchement USB pour connecter ou déconnecter les périphériques.
        """
        context = pyudev.Context()
        monitor = pyudev.Monitor.from_netlink(context)
        monitor.filter_by(subsystem='tty')

        for action, device in monitor:
            port = device.device_node
            if action == 'add' and port not in self.devices:
                self.connect_device(port)
            elif action == 'remove' and port in self.devices:
                self.disconnect_device(port)

    def connect_device(self, port):
        """
        Connecte un nouveau périphérique sur un port série.
        """
        try:
            conn = serial.Serial(port=port, baudrate=9600, timeout=1)
            self.devices[port] = {'connection': conn, 'data': None}
            print(f"Nouveau périphérique connecté sur {port}")
            # Démarre la lecture sur ce port
            self.start_reading()
        except Exception as e:
            print(f"Erreur lors de la connexion sur {port}: {e}")

    def disconnect_device(self, port):
        """
        Déconnecte un périphérique en fermant sa connexion série.
        """
        if port in self.devices:
            self.devices[port]['connection'].close()
            del self.devices[port]
            print(f"Périphérique déconnecté sur {port}")

    def get_data(self, port):
        """
        Récupère les données reçues sur un port série spécifique.
        """
        return self.devices[port]['data'] if port in self.devices else None

    def close(self):
        """
        Ferme toutes les connexions série.
        """
        self.running = False
        for port in list(self.devices.keys()):
            self.disconnect_device(port)
    
    def __str__(self):
        return "Communication"
    def parse_data(self, data):
        """
        Parse les données reçues 
        """
        action, value = data.split('/')
        return action, value