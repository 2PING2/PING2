import os
import paramiko
import socket
import time
import platform
import concurrent.futures

# Configuration
base_ip = "192.168.250."
username = "pi"  # Change selon ton setup
password = "raspberry"  # Remplace par ton mot de passe
timeout = 0.3  # Timeout du ping (en secondes)

def ping_ip(ip):
    """Teste si une IP est en ligne via un ping rapide."""
    param = "-n 1 -w 300" if platform.system().lower() == "windows" else "-c 1 -W 0.3"
    result = os.system(f"ping {param} {ip} > nul 2>&1" if platform.system().lower() == "windows" else f"ping {param} {ip} > /dev/null 2>&1")
    return ip if result == 0 else None

def test_ssh(ip):
    """Teste si l'IP est accessible via SSH."""
    try:
        client = paramiko.SSHClient()
        client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        client.connect(ip, username=username, password=password, timeout=2)
        print(f"✅ Connexion SSH réussie à {ip} !")
        client.close()
        return True
    except (socket.timeout, paramiko.ssh_exception.NoValidConnectionsError, paramiko.AuthenticationException):
        return False
    finally:
        time.sleep(1)  # Pause pour éviter le bannissement

def find_raspberry_pi():
    """Scanne le réseau avec ping rapide et multithreading, puis teste SSH sur IPs actives."""
    print("🔍 Scan du réseau en cours...")
    
    # Exécuter les pings en parallèle
    with concurrent.futures.ThreadPoolExecutor(max_workers=50) as executor:
        futures = [executor.submit(ping_ip, base_ip + str(i)) for i in range(1, 255)]
        active_ips = [f.result() for f in concurrent.futures.as_completed(futures) if f.result()]

    if not active_ips:
        print("❌ Aucune IP active trouvée.")
        return None

    print(f"🔍 IPs actives détectées : {active_ips}")

    for ip in active_ips:
        print(f"🛠 Test SSH sur {ip}...")
        if test_ssh(ip):
            print(f"🎯 La Raspberry Pi est probablement à : {ip}")
            return ip

    print("❌ Aucune Raspberry Pi trouvée sur ce réseau.")
    return None

# Lancer le scan
find_raspberry_pi()
