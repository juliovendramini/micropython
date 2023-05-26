from machine import UART, Pin, ADC, Timer, I2C
import wifi
import ssd1306
from time import sleep
import webrepl
import menu
import mpu6050
import sensorLego
# ESP32 Pin assignment 
#i2c = SoftI2C(scl=Pin(22), sda=Pin(21))
#i2c = I2C(1,scl=Pin(22), sda=Pin(21), freq=400000)


import stepMotor


#1;velocidade_direito -> #1;200
#2;velocidade_esquerdo -> #2;300
#3;pulsos_direito -> 
#4;pulsos_esquerdo
#5;velocidade_direito;velocidade_esquerdo
#6;pulsos_direito;pulsos_esquerdo
#7;  <- estado dos motores 
#8;  <- retorna sensor barreira

motor1 = stepMotor.MotorPasso(1)
motor2 = stepMotor.MotorPasso(2)


def funcTimer(t):
    global motor1, motor2
    motor1.pulsa()
    motor2.pulsa()


tim0 = Timer(0)
tim0.init(mode=Timer.PERIODIC, freq=10000, callback=funcTimer)


motor1.setVelocidade(-500)
motor2.setVelocidade(500)
motor2.setPulsos(20)
motor1.setPulsos(20)


def serialLeInt(serial):
    x = serial.read(1)
    if x == None:   
        x = 0
    y = int.from_bytes(bytes(x), "big")
    return y


def interpretaSerial(serial):
    retorno = serial.readline()
    if(retorno != None):
        print(retorno)
        textoRecebido = retorno.decode('uft-8')
        print(textoRecebido)
        lista = textoRecebido.split(";")
        if lista[0] == "#1":
            motor2.setVelocidade(int(lista[1]))
            print("Velocidade alterada", lista[1])
        if lista[0] == "#2":
            motor1.setVelocidade(int(lista[1]))
            print("Velocidade alterada", lista[1])
        elif lista[0] == "#3":
            motor2.setPulsos(abs(int(lista[1])))
            print("pulso alterado", lista[1])
        elif lista[0] == "#4":
            motor1.setPulsos(abs(int(lista[1])))
            print("pulso alterado", lista[1])
        elif lista[0] == "#5":
            motor1.setVelocidade(int(lista[1]))
            print("Velocidade alterada", lista[1])
            motor2.setVelocidade(int(lista[2]))
            print("Velocidade alterada", lista[2])
        elif lista[0] == "#6":
            motor1.setPulsos(abs(int(lista[1])))
            print("pulso alterado", lista[1])
            motor2.setPulsos(abs(int(lista[2])))
            print("pulso alterado", lista[2])
        elif lista[0] == "#7":
            motor1Girando = "0"
            motor2Girando = "0"
            if motor1.estaGirando():
                motor1Girando = "1"
            if motor2.estaGirando():
                motor2Girando = "1"
            serial.write("#7;" + motor1Girando + ";" + motor2Girando + ";")
    return

    #def pulsarMotores(pD,pE):
        #serial.write("#6+;"+pD+";"+pE)
#  def velocidadeMotores(vD,vE):
#  serial.write("#5+;"+vD+";"+vE+"\n")
portaSerial = None
serial = UART(1, baudrate=38400, tx=32, rx=34)
serial.init(baudrate=38400, bits=8, parity=None, stop=1)
#serial.write("testando serial...\n")
#serial.write("testando serial...\n")
while(True):
    #interpretaSerial(serial)
    botoesMenu = ADC(Pin(36))
    botoesMenu.atten(ADC.ATTN_11DB)
    opcao = botoesMenu.read()
    print(opcao)
    sleep(1)


while(True):
    x = serial.read(10)
    if x == None:   
        serial.write("nada na tela")
        pass
    else:
        print(int(x))
        if  x != 0:
            motor2.setPulsos(int(x))
    time.sleep(1)
    

def velocidadeMotores(mD,mE):
    serial.write("#5;"+mD+";"+mE)

def velocidadeMotorDireito(mD):
    serial.write("#1;"+mD)



#motor1.setPulsos(4000)
#motor2.setPulsos(2000)


#tim0 = Timer(0)
#tim0.init(mode=Timer.PERIODIC, freq=10000, callback=funcStep)

#print("era pra estar funcionando....")
