import socket
import threading
import globalVars

def starts_connection_server(filePath, filesize):
    print(10)
    print(filesize)

    s = socket.socket()
    host = ''
    port = 40555
    s.bind((host, port))
    
    fl=open(filePath,'rb')   
    d=fl.read(10240)
    
    file_n_list = filePath.split('/')
    index = len(file_n_list)
    filenameInBytes = file_n_list[index-1].encode('utf-8')
    
    s.listen(1)
    
    c , addr=s.accept()
    c.send(filenameInBytes)
    c.send((str(filesize)).encode('utf-8'))
    while d!=b'':            
         c.send(d)
         globalVars.bytesSent+=len(d)
         d=fl.read(10240)
    c.send(b'')
    c.close()  
    

def starts_listening_client(l):
    print(11)
    s = socket.socket()
    host = socket.gethostname()
    port = 40555
    s.connect((host,port))

    filenameInBytes = s.recv(1024)
    filename = filenameInBytes.decode('utf-8')
    
    globalVars.filesize = float(s.recv(1024).decode('utf-8'))

    fl=open(filename,'wb')
    d=s.recv(10240)              
    while d!=b'':
        l.acquire()
        fl.write(d)
        globalVars.bytesRecieved+=len(d)
        d=s.recv(10240)
        l.release()
    fl.close()
    s.close()
    
    
