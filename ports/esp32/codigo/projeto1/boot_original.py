from machine import Pin, ADC, Timer, I2C
import wifi
import ssd1306
import time
import webrepl
import menu
import mpu6050
import sensorLego
# ESP32 Pin assignment 
#i2c = SoftI2C(scl=Pin(22), sda=Pin(21))
i2c = I2C(1,scl=Pin(22), sda=Pin(21), freq=400000)
oled_width = 128
oled_height = 64
oled = ssd1306.SSD1306_I2C(oled_width, oled_height, i2c)
oled.fill(0)
oled.text("Iniciando...",0,0)



import stepMotor
motor1 = stepMotor.MotorPasso(1)
'''motor1.set_speed(50)
def funcStep(t):
    global motor1
    motor1.do_step()

tim0 = Timer(0)
tim0.init(mode=Timer.PERIODIC, freq=10000, callback=funcStep)
'''


wifi = wifi.Wifi(oled)
wifi.conecta()
menu = menu.Menu(oled)
menu.drawMenu()
botoesMenu = ADC(Pin(36))
botoesMenu.atten(ADC.ATTN_11DB)
opcao = botoesMenu.read()
while(True):
    opcao = botoesMenu.read()
    if(opcao < 300):
        retorno = menu.select()
        if retorno >= 0:
            if retorno == 3:
                menu.back()
            if retorno == 0:
                #chama a execucao do aplicativo
                app = menu.retornaApp()
                app.comeca()
    if(opcao > 1700 and opcao < 2000):
        menu.up()
    if(opcao > 1200 and opcao < 1600):
        menu.down()
#300 botao ok
#1700-2000 botao cima
#1200-1600 botao baixo
#900 - 1050 botao cima e baixo juntos
