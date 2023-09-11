import tkinter as tk
import time
import random
import pyautogui

def test_passwords(network, password_generator):
    for password in password_generator:
        pyautogui.click(50, 50) # Clica em um lugar qualquer para garantir o foco na caixa de texto
        pyautogui.write(password, interval=0.1)
        pyautogui.press("enter")
        time.sleep(2) # Espera alguns segundos para verificar se a conexão foi bem sucedida
        if pyautogui.pixel(120, 120)[0] == 255: # Verifica se o pixel da conexão está azul
            print(f"Senha correta encontrada para a rede {network}: {password}")
            break
class PasswordGenerator:
    def __init__(self, length, character_set):
        self.length = length
        self.character_set = character_set

    def __iter__(self):
        return self

    def __next__(self):
        password = "".join(random.choices(self.character_set, k=self.length))
        return password


def start_testing():
    network_name = network_entry.get()
    password_length = int(password_length_entry.get())
    character_set = characters_entry.get()
    password_generator = PasswordGenerator(password_length, character_set)

    test_passwords(network_name, password_generator)

# Interface Gráfica
window = tk.Tk()
window.title("Teste de Senhas Wi-Fi")
window.geometry("400x250")

network_label = tk.Label(window, text="Nome da Rede Wi-Fi:")
network_label.pack()
network_entry = tk.Entry(window)
network_entry.pack()

password_length_label = tk.Label(window, text="Comprimento da Senha:")
password_length_label.pack()
password_length_entry = tk.Entry(window)
password_length_entry.pack()

characters_label = tk.Label(window, text="Caracteres da Senha:")
characters_label.pack()
characters_entry = tk.Entry(window)
characters_entry.pack()

start_button = tk.Button(window, text="Iniciar Teste", command=start_testing)
start_button.pack()

window.mainloop()
