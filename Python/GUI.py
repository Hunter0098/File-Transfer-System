from tkinter import *
from threading import *
from tkinter import filedialog
from tkinter import ttk
import os
import file_share
import globalVars

def chooseFile():
    filePath = filedialog.askopenfilename() #!filetypes=(("All files", "*.*")) Why this is not working?
    entry1.delete(0,END)
    entry1.insert(0, filePath)
    print(entry1.get())
    filesize = os.path.getsize(entry1.get())
    print("Filesize: ", end="")
    print(filesize)

def chooseRadio():
    if(var.get() == "Send"):
        entry1.config(state=NORMAL)
        btn1.config(state=NORMAL)
    else:
        entry1.config(state=DISABLED)
        btn1.config(state=DISABLED)

def startShare():
    if (var.get() == "Send"):
        global th1
        globalVars.filesize = os.path.getsize(entry1.get())
        print(globalVars.filesize)
        th1=Thread(target=file_share.starts_connection_server, args=(entry1.get(), globalVars.filesize))
        th1.start()
        
    else:
        global th2
        th2=Thread(target=file_share.starts_listening_client, args=(l,))
        th2.start()

def checkFileValidity():
    if os.path.isfile(entry1.get()):
        entry1.config(fg="green")
        btn2.config(state=NORMAL)
        
        btn2.flash()    #Flash button when ready to share
    else:
        entry1.config(fg="red")
        if(var.get() == "Send"):
            btn2.config(state=DISABLED)
        else:
            btn2.config(state=NORMAL)

def checkProgressbarStatus():
    if (var.get() == "Send"):
        percentage = (globalVars.bytesSent / globalVars.filesize) * 100
    elif (var.get() == "Recieve"):
        percentage = (globalVars.bytesRecieved / globalVars.filesize) * 100
    progressBar1['value'] = percentage

def loopEvents():
    checkFileValidity()
    checkProgressbarStatus()
    window.after(1000, loopEvents)

def pause_resume():
    print("Pause Resume Button clicked")
    if(btn3['text'] == "Pause"):
        btn3.config(text="Resume")
        l.acquire()
    else:
        btn3.config(text="Pause")
        l.release()


#************Main***************

l=Lock()
window = Tk()
window.title("Jalex-p2p")
window.geometry("600x500+150+50")

frame1 = Frame(window)
lbl1 = Label(frame1,text="Please choose an option",font = ("Arial",18))
lbl1.pack(side = LEFT)
frame1.pack(fill="x", padx=10, pady=10)

frame2 = Frame(window)
var = StringVar()
radio1 = Radiobutton(frame2, text="Send a file", variable=var, value="Send", font=("arial", 12), anchor="w", justify="left", command=chooseRadio)
radio1.pack(fill="x")
radio2 = Radiobutton(frame2, text="Recieve a file", variable=var, value="Recieve", font=("Arial",12), anchor="w", justify="left", command=chooseRadio)
radio2.pack(fill="x")
var.set("Send")
frame2.pack(fill="x",pady=10)

frame3 = Frame(window)
lbl2 = Label(frame3, text="Specify the file path here: ", font=("Arial",12))
lbl2.pack(fill="both", padx=10, pady=10, side=LEFT)
frame3.pack(fill="both")
 
frame4 = Frame(window)
entry1 = Entry(frame4, bd=1, font=("Arial",12), width=35)
entry1.pack(side=LEFT, anchor="n", padx=10, pady=10)
filePath = ""
btn1 = Button(frame4, text="Browse", font=("Arial",12), command=chooseFile)
btn1.pack(pady=10, side=LEFT)
print(filePath)
frame4.pack(fill="both")

frame5 = Frame(window)
btn2 = Button(window, text="Start sharing", font=("Arial",12), command=startShare)
btn2.pack()
frame5.pack(fill="both", pady=10)

frame6 = Frame(window)
btn3 = Button(frame6, text="Pause", font=("Arial",12), command=pause_resume)
btn3.pack()
frame6.pack(fill="both", pady=20)

frame7 = Frame(window)
progressBar1 = ttk.Progressbar(frame7, orient="horizontal", length=40, mode="determinate")
progressBar1.pack(fill="x", padx=20)
progressBar1['value']=0
progressBar1['maximum']=100
frame7.pack(fill="both", pady=20)

window.after(1000, loopEvents)
window.mainloop()
        
