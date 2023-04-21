import tkinter as tk
import comm

root = tk.Tk()
root.title("Interface Weaver")

comm.create_comm_ui(root)

root.mainloop()