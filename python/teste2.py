import tkinter as tk
import os


class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.master = master
        self.master.geometry("800x500")
        self.master.title("Password Generator")
        self.pack()
        self.create_widgets()

    def create_widgets(self):
        self.generate_button = tk.Button(self)
        self.generate_button["text"] = "Generate Password"
        self.generate_button["command"] = self.generate_password
        self.generate_button.pack(side="top")

        self.password_text = tk.Text(self)
        self.password_text.pack(side="bottom")

    def generate_password(self):
        password = PasswordGenerator.generate_password()
        self.password_text.delete('1.0', tk.END)
        self.password_text.insert(tk.END, password)


class PasswordGenerator:
    @staticmethod
    def generate_password():
        return "testpassword"


if __name__ == "__main__":
    root = tk.Tk()
    app = Application(master=root)
    app.mainloop()
