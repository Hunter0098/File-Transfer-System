from tkinter import *
from threading import *
from tkinter import filedialog
from tkinter import ttk
import os

def chooseFile():
    filePath = filedialog.askopenfilename() #!filetypes=(("All files", "*.*")) Why this is not working?
    entry1.delete(0,END)
    entry1.insert(0, filePath)
    print(entry1.get())
    filesize = os.path.getsize(entry1.get())
    print("Filesize: ", end="")
    print(filesize)
    fobject = open("filesize.txt", "w")
    fobject.write(str(filesize))
    fobject.close()

def savefilepath():
        window.destroy()

def checkFileValidity():
    if os.path.isfile(entry1.get()):
        entry1.config(fg="green")
        btn2.config(state=NORMAL)
        
        btn2.flash()    #Flash button when ready to share
    else:
        entry1.config(fg="red")

def loopEvents():
    checkFileValidity()
    window.after(1000, loopEvents)

#************Main***************

l=Lock()
window = Tk()
window.title("Jalex-p2p")
window.geometry("600x500+150+50")

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
btn2 = Button(window, text="Save", font=("Arial",12), command=savefilepath)
btn2.pack()
frame5.pack(fill="both", pady=10)

window.after(1000, loopEvents)
window.mainloop()
        
