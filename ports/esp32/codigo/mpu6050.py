import machine
from time import sleep
import _thread

class Mpu6050():
    x = None
    y = None
    x = None
    ESCALA_GIRO = 512
    ESCALA_ACELEROMETRO = 2048
    def __init__(self, i2c, addr=0x68):
        self.iic = i2c
        self.addr = addr
        #self.iic.start()
        b = bytearray(2)
        b[0] = 107
        b[1] = 0
        self.iic.writeto(self.addr, b)

        #configuracao do giroscopio
        b[0] = 0x1B
        #b[1] = 0x18
        b[1] = 0
        self.iic.writeto(self.addr, b)
        #configuracao da taxa de atualizacao 
        b[0] = 0x1A
        b[1] = 0x06
        self.iic.writeto(self.addr, b) #4hz de atualizacao
        b[0] = 0x19
        b[1] = 250
        self.iic.writeto(self.addr, b)
        self.x = 0
        self.y = 0
        self.z = 0
        self._x_c = 0
        self._y_c = 0
        self._z_c = 0
        #self._write(gr[gyro_range], 0x1B, self.mpu_addr)
        #self.iic.stop()
        _thread.start_new_thread(self._atualizaGiroThread, ())

    def _get_raw_values(self):
        a = self.iic.readfrom_mem(self.addr, 0x3B, 14)
        return a

    def _atualizaGiroThread(self):
        while True:
            raw_ints = self._get_raw_values()
            vals = {}
            vals["GyX"] = self.bytes_toint(raw_ints[8], raw_ints[9])/self.ESCALA_GIRO
            vals["GyY"] = self.bytes_toint(raw_ints[10], raw_ints[11])/self.ESCALA_GIRO
            vals["GyZ"] = self.bytes_toint(raw_ints[12], raw_ints[13])/self.ESCALA_GIRO
            self.x = self.x + (vals["GyX"] - self._x_c)
            self.y = self.y + (vals["GyY"] - self._y_c)
            self.z = self.z + (vals["GyZ"] - self._z_c)
            sleep(0.25)

    def get_ints(self):
        b = self._get_raw_values()
        c = []
        for i in b:
            c.append(i)
        return c

    def bytes_toint(self, firstbyte, secondbyte):
        if not firstbyte & 0x80:
            return firstbyte << 8 | secondbyte
        return - (((firstbyte ^ 255) << 8) | (secondbyte ^ 255) + 1)

    def get_values(self):
        raw_ints = self._get_raw_values()
        vals = {}
        vals["AcX"] = self.bytes_toint(raw_ints[0], raw_ints[1])/self.ESCALA_ACELEROMETRO
        vals["AcY"] = self.bytes_toint(raw_ints[2], raw_ints[3])/self.ESCALA_ACELEROMETRO
        vals["AcZ"] = self.bytes_toint(raw_ints[4], raw_ints[5])/self.ESCALA_ACELEROMETRO
        vals["Tmp"] = self.bytes_toint(raw_ints[6], raw_ints[7]) / 340.00 + 36.53
        vals["GyX"] = self.bytes_toint(raw_ints[8], raw_ints[9])/self.ESCALA_GIRO
        vals["GyY"] = self.bytes_toint(raw_ints[10], raw_ints[11])/self.ESCALA_GIRO
        vals["GyZ"] = self.bytes_toint(raw_ints[12], raw_ints[13])/self.ESCALA_GIRO
        return vals  # returned in range of Int16
        # -32768 to 32767

    def getGiro(self):
        '''
        raw_ints = self._get_raw_values()
        vals = {}
        vals["GyX"] = self.bytes_toint(raw_ints[8], raw_ints[9])/self.ESCALA_GIRO
        vals["GyY"] = self.bytes_toint(raw_ints[10], raw_ints[11])/self.ESCALA_GIRO
        vals["GyZ"] = self.bytes_toint(raw_ints[12], raw_ints[13])/self.ESCALA_GIRO
        self.x = self.x + (vals["GyX"] - self._x_c)
        self.y = self.y + (vals["GyY"] - self._y_c)
        self.z = self.z + (vals["GyZ"] - self._z_c)
        '''
        vals = {}
        vals["x"] = self.x
        vals["y"] = self.y
        vals["z"] = self.z
        return vals  # returned in range of Int16
        # -32768 to 32767

    def getAceleracao(self):
        raw_ints = self._get_raw_values()
        vals = {}
        vals["x"] = self.bytes_toint(raw_ints[0], raw_ints[1])/self.ESCALA_ACELEROMETRO
        vals["y"] = self.bytes_toint(raw_ints[2], raw_ints[3])/self.ESCALA_ACELEROMETRO
        vals["z"] = self.bytes_toint(raw_ints[4], raw_ints[5])/self.ESCALA_ACELEROMETRO
        return vals
        
    def val_test(self):  # ONLY FOR TESTING! Also, fast reading sometimes crashes IIC
        while 1:
            print(self.getGiro())
            sleep(0.250)

    def resetaPosicao(self):
        self.x = 0
        self.y = 0
        self.z = 0

    def reseta(self):
        b = bytearray(2)
        b[0] = 107
        b[1] = 128
        self.iic.writeto(self.addr, b)
        sleep(0.100)
        b[0] = 107
        b[1] = 0
        self.iic.writeto(self.addr, b)

    def calibracao(self):
        raw_ints = self._get_raw_values()
        vals = {}
        i=0
        QUANTIDADE = 30
        self._x_c = self._y_c = self._z_c = 0
        while i < QUANTIDADE:
            vals["GyX"] = self.bytes_toint(raw_ints[8], raw_ints[9])/self.ESCALA_GIRO
            vals["GyY"] = self.bytes_toint(raw_ints[10], raw_ints[11])/self.ESCALA_GIRO
            vals["GyZ"] = self.bytes_toint(raw_ints[12], raw_ints[13])/self.ESCALA_GIRO
            self._x_c = self._x_c + vals["GyX"]
            self._y_c = self._y_c + vals["GyY"]
            self._z_c = self._z_c + vals["GyZ"]
            sleep(0.25)
            i=i+1
        vals = {}
        self._x_c = vals["x_c"] = self._x_c / QUANTIDADE
        self._y_c = vals["y_c"] = self._y_c / QUANTIDADE
        self._z_c = vals["z_c"] = self._z_c / QUANTIDADE
        return vals 
