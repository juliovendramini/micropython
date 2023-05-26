from machine import UART, Pin
import time
import _thread
# Código para usar Sensor Serial do Lego
# Criado por Julio Cesar Goldner Vendramini


#wait(2000)
class SensorLego:
    sincronizado = None
    portaSerial = None
    valor = None
    contadorErro = 0
    REFLEXAO = 0
    RGB = 4
    
    BYTE_ESPERA = 0
    def __init__(self,porta,portaSerial,tipoSensor):
        
        if (tipoSensor == "COR"):
            self.BYTE_ESPERA = 237
        elif (tipoSensor == "ULTRASSONICO"):
            self.BYTE_ESPERA = 235
        else: 
            raise OSError('Tipo de Sensor Invalido')
        
        if(portaSerial < 1 or portaSerial > 2):
            raise OSError('Porta Serial invalida')
        self.portaSerial = portaSerial

        if(porta == 1):
            self.tx = 32
            self.rx = 34
            #self.serial = UART(portaSerial, baudrate=2400, tx=32, rx=34)
        elif(porta == 2):
            self.tx = 14
            self.rx = 35
            #self.serial = UART(portaSerial, baudrate=2400, tx=14, rx=35)
        elif(porta == 3):
            self.tx = 15
            self.rx = 33
            #self.serial = UART(portaSerial, baudrate=2400, tx=15, rx=33)
        elif(porta == 4):
            self.tx = 17
            self.rx = 16
            #self.serial = UART(portaSerial, baudrate=2400, tx=17, rx=16)
        else:
            raise OSError('Porta invalida')

        #self.serial = UART(portaSerial, baudrate=2400, tx=self.tx, rx=self.rx)
        self.sincronizado = False
        _thread.start_new_thread(self._leituraThread, ())

    def _leituraThread(self):
        while True:
            if self.sincronizado == True:
                self._sendAck()
                i=0
                #self.serial.flush()
                #self.serial.readline()
                while(i<20):
                    i=i+1
                    y = self._serialLeInt()
                    if(y==192):
                        break
                if(i == 20):
                    if(self.contadorErro == 10):
                        self.valor = -1
                    else:
                        self.contadorErro = self.contadorErro + 1
                else:
                    valor = self._serialLeInt()
                    temp = self._serialLeInt() #ignoro o checksum
                    self.serial.write(bytes([2])) #envio nack
                    self.valor = valor
                    self.contadorErro = 0
                    #while(temp != None):
                    #    temp = self.serial.read(1)
                    #limpo o restante
                    self.serial.read()
            time.sleep(0.010)


    #le apenas um byte por vez
    def _serialLeInt(self):
        x = self.serial.read(1)
        if x == None:   
            x = 0
        y = int.from_bytes(bytes(x), "big")
        return y

    def _sendSelect(self,mode):
        checksum = 255 ^ 67
        checksum = checksum ^ mode
        self.serial.write(bytes([67]));
        self.serial.write(bytes([mode]));
        self.serial.write(bytes([checksum]));


    #o ack é 4, mas tem mais 4 nas msgs, então vou esperar BYTE_ESPERA-4
    def _waitAck(self):
        while(True):
            y = self._serialLeInt()
            if(y == self.BYTE_ESPERA):
                break
        y = self._serialLeInt()
        if(y != 4):
            return False
        #y = serial_le_int()
        #if(y != 0):
        #    return False;
        return True


    def _waitTypeData(self):
        valida = [0,0,0,0]
        while(True):
            x = self.serial.read()
            y = int.from_bytes(x, "big")
            if(y == 64):
                valida[0] = y
                break
        x = self.serial.read()
        y = int.from_bytes(x, "big")
        valida[1] = y
        x = self.serial.read()
        y = int.from_bytes(x, "big")
        valida[2] = y
        check = 255 ^ valida[0]
        check = check ^ valida[1]
        valida[3] = check
        print(valida)
        if(check == y):
            return True
        else:
            return False

    def _waitModeData(self):
        valida = [0,0,0,0,0]
        check = 255 ^ 73
        while(True):
            x = self.serial.read()
            y = int.from_bytes(x, "big")
            if(y == 73):
                valida[0] = y
                break

        x = self.serial.read()
        y = int.from_bytes(x, "big")
        check = check ^ y
        valida[1] = y

        x = self.serial.read()
        y = int.from_bytes(x, "big")
        check = check ^ y
        valida[2] = y

    #checkum
        x = self.serial.read()
        y = int.from_bytes(x, "big")
    #    check = check ^ y
        valida[3] = y
        valida[4] = check
        #print(valida)
        if(check == y):
            return True
        else:
            return False

    def _sendAck(self):
        self.serial.write(bytes([4]))
        #ser.write(bytes([0]))


    def _sendSpeed(self):
        cmd = 82 #valor de speed
        checksum = 255 ^ cmd
        self.serial.write(bytes([82]))
        bb = [0,225,0,0]
        i=0
        while(i < 4):
            #for(int i=0;i<4;i++) {
            b = bb[i]
            checksum = checksum ^ b;
            self.serial.write(bytes([b]))
            i=i+1
        self.serial.write(bytes([checksum]))

    def _sendWrite(self,bb, len):
        # CMD_WRITE = 0x44 -> 68
        # CMD_LLL_SHIFT = 3
        #byte b = CMD_WRITE | (len << CMD_LLL_SHIFT);
        b = 68 | (len << 3)
        checksum = 255 ^ b;
        self.serial.write(bytes([b]));
        i=0
        while(i<len):
        #for(int i=0;i<len;i++) {
            self.serial.write(bytes([bb[i]]));
            checksum = checksum ^ bb[i];
            i=i+1
        
        self.serial.write(bytes([checksum]));


    def _waitSpeedData(self):
        global ser
        valida = [0,0,0,0,0,0,0]
        check = 255 ^ 82
        while(True):
            y = self._serialLeInt()
            if(y == 82):
                valida[0] = y
                break
        i=0
        while(i < 4):
            #for(int i=0;i<4;i++) {
            b = self._serialLeInt()
            check = check ^ b;
            valida[i+1] = b;
            i=i+1

        #checksum
        y = self._serialLeInt()
        valida[5] = y
        #check = check ^ y
        valida[6] = check
        print (valida)
        if(check == y):
            return True
        else:
            return False

    #_sendAck()
    #_sendSelect(0)
    #_sendSpeed()


    # while(True):
    #     x = ser.read()
    #     y = int.from_bytes(x, "big")
    #     print(y, end="-")

    # while(not _waitTypeData()):
    #     continue
    # while(not _waitModeData()):
    #     continue
    # while(not _waitSpeedData()):
    #     continue
    # while(not _waitAck()):
    #     continue

    # while(not _waitAck_ultrassom()):
    #     continue

    def modo(self,modo):
        self.sincronizado = False
        self.serial = UART(self.portaSerial, baudrate=2400, tx=self.tx, rx=self.rx)
        self.serial.init(baudrate=2400, bits=8, parity=None, stop=1)
        x = self._waitAck()
        while(not self._waitAck()):
            continue
        time.sleep(0.010)
        self._sendAck()
        time.sleep(0.050)
        del self.serial
        self.serial = UART(self.portaSerial, baudrate=57600, tx=self.tx, rx=self.rx)#UARTDevice(1,57600,1000)
        self._sendSelect(modo)
        self._sendAck()
        self.sincronizado=True
            #print(le_distancia(),end="-")
            #print(le_intensidade(),end="-")
        # while(True):
        #     print(le_intensidade(),end="-")

    def teste(self):
        while(True):
            print (self.valor)
            time.sleep(0.1)


'''
    def rgb_para_hsv(r, g, b):
            #r, g, b = r/255.0, g/255.0, b/255.0
            mx = max(r, g, b)
            mn = min(r, g, b)
            df = mx-mn
            print(mx,mn,df)
            if mx == mn:
                h = 0
            elif mx == r:
                print("r",end="")
                h = ((600*(g-b)//df) + 3600)//10 % 360
            elif mx == g:
                print("g",end="")
                h = ((600*(b-r)//df) + 1200)//10 % 360
            elif mx == b:
                print("b",end="")
                h = ((600*(r-g)//df) + 2400)//10 % 360
            if mx == 0:
                print("b",end="")
                s = 0
            else:
                s = (100*df//mx)
            v = 100*mx//255
            return h, s, v
'''

    # for i in range(16):
    #     for j in range(16):
    #         for k in range(16):
    #             x = cores[i*256+j*16+k] #rgb_para_hsv(i,j,k)
    #             #cores[i*256+j*16+k]


'''
    def le_intensidade():
        global ser
        ser.clear()
        i=0
        while(i<20):
            i=i+1
            y = serial_le_int()
            if(y==192):
                break
        if(i == 20):
            return -1
        valor = serial_le_int()
        y = serial_le_int() #ignoro o checksum
        ser.write(bytes([2])) #envio nack
        return valor
    # while(True):
    #     x = ser.read()
    #     y = int.from_bytes(x, "big")
    #     print(y,end="-")
    # #_sendSpeed()


    while(not _waitAck()):
        continue
    wait(10)
    _sendAck()
    #wait(50)
    del ser
    ser = UARTDevice(Port.S1,57600,1000)
    _sendSelect(0)

    while(True):
        #print(le_distancia(),end="-")
        print(le_intensidade(),end="-")
    # while(True):
    #     print(le_intensidade(),end="-")


    while(True):
        x = ser.read()
        y = int.from_bytes(x, "big")
        print(y, end="-")
'''