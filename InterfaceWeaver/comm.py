from tkinter import *
from tkinter import filedialog as fd
import serial.tools.list_ports
import communication_functions as cf


connected = False
monitor : cf.SerialMonitor

def create_comm_ui(parent):

    frm_comm = Frame(parent)
    frm_comm.pack()

    monitor = cf.SerialMonitor()

    config_file = StringVar()
    
    var_status = StringVar()
    
    SPEEDS = [9600, 115200]
    var_speed = IntVar()
    var_speed.set(SPEEDS[1])

    PORTS = []
    for port in serial.tools.list_ports.comports():
        PORTS.append(port.name)
    var_port = StringVar()
    var_port.set(PORTS[0])

    def get_config_file():
        config_file.set(fd.askopenfilename())


    frm_settings = LabelFrame(frm_comm, text="Connection Settings")
    frm_settings.grid(sticky="ew")

    lbl_port = Label(frm_settings, text="Port")
    lbl_port.grid(column=0,row=0)

    opt_port = OptionMenu(frm_settings, var_port, *PORTS)
    opt_port.grid(column=1, row=0, sticky="ew")

    lbl_speed = Label(frm_settings, text="Baudrate")
    lbl_speed.grid(column=0, row = 1)

    opt_speed = OptionMenu(frm_settings, var_speed, *SPEEDS)
    opt_speed.grid(column=1, row=1, sticky="ew")

    frm_upload = LabelFrame(frm_comm, text="Upload Config File")
    frm_upload.grid(pady=10, ipadx=5)

    lbl_file = Label(frm_upload, text="Config file")
    lbl_file.grid(column=0, row=0, sticky="w")
    #lbl_file.pack()

    btn_file = Button(frm_upload, text="Select File", command=get_config_file)
    btn_file.grid(column=1, row=0, sticky="e")

    ent_file = Entry(frm_upload, text=config_file)
    ent_file.grid(column=0, row = 1, columnspan=2, sticky="nesw")
    #lbl_file.pack()

    btn_upload = Button(frm_upload, text="Upload", height="2", command=lambda: var_status.set(cf.uploadConfigFile(var_port.get(), var_speed.get(), config_file.get())))
    btn_upload.grid(column=0, row=2, columnspan=2, sticky="nesw", pady=5)
    #btn_upload.pack()

    chk_save = Checkbutton(frm_upload, text="Save to board")
    chk_save.grid(column=0, row=3, columnspan=2)
    #chk_save.pack()

    chk_mon = Checkbutton(frm_upload, text="Start monitor after upload")
    chk_mon.grid(column=0, row=4, columnspan=2)
    #chk_mon.pack()

    frm_mon = LabelFrame(frm_comm, text="Monitor")
    frm_mon.grid(column=0, row=3, sticky="ew")

    btn_mon_start = Button(frm_mon, text="Start", command=monitor.start)
    btn_mon_start.grid(column=0, row=0, sticky="ew")
    btn_mon_stop = Button(frm_mon, text="Stop", command=monitor.stop)
    btn_mon_stop.grid(column=1, row= 0, sticky="ew")

    lst_mon = Listbox(frm_mon, height=10)
    lst_mon.grid(column=0, row=1, columnspan=2, sticky="nesw")

    ent_status = Entry(frm_comm, text=var_status, state='disabled')
    ent_status.grid(column=0, row=4, sticky="ew")

    return frm_comm