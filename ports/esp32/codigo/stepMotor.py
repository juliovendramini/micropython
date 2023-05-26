#Criado por Julio Cesar Goldner Vendramini
from machine import Pin, Timer


class MotorPasso:
    __velocidadeAtual = 0
    __mudancaDeVelocidade = False
    __aceleracao = 50
    def __init__(self, porta):
        if porta == 1:
            self.step_pin = Pin(26, mode=Pin.OUT, pull=None) #pyb.Pin(step_pin, pyb.Pin.OUT_PP)
            self.dir_pin = Pin(27, mode=Pin.OUT, pull=None) #pyb.Pin(dir_pin, pyb.Pin.OUT_PP)
        elif porta == 2:
            # VERIFICAR ESSAS PORTAS
            self.step_pin = Pin(18, mode=Pin.OUT, pull=None) #pyb.Pin(step_pin, pyb.Pin.OUT_PP)
            self.dir_pin = Pin(19, mode=Pin.OUT, pull=None) #pyb.Pin(dir_pin, pyb.Pin.OUT_PP)
        else:
             raise OSError('Porta invalida')
        self.dir = 0
        self.__velocidadeAtual = 0
        self.count = 0
        self.velocidade = 0
        self.__aceleracao = 1
        self.pulsos = 0
        #self.timer = Timer(porta)
        #self.timer.init(mode=Timer.PERIODIC, freq=10000, callback=self.pulsa) #100us
 

    def pulsa(self):   # called by timer interrupt every 100us
        #nao remover o T. Precisa dele no parametro
        if self.__mudancaDeVelocidade == True:
            if(self.velocidade > self.__velocidadeAtual):
                self.__velocidadeAtual = self.__aceleracao #comeco com o valor da aceleracao
            self.__mudancaDeVelocidade = False
        if self.velocidade == 0 or self.__velocidadeAtual == 0:
            return
        if self.__velocidadeAtual > self.velocidade:
            self.__velocidadeAtual = self.velocidade
        if self.dir == 0 or self.pulsos == 0:
            self.__velocidadeAtual = 0
            return
        self.count = (self.count+1)%(self.velocidade*4 - self.__velocidadeAtual*2)
        if self.count == 0:
            self.pulsos = self.pulsos - 1
            self.step_pin.value(1)
            pass
            self.step_pin.value(0)
            self.__velocidadeAtual = self.__velocidadeAtual + self.__aceleracao
        
    def setVelocidade(self, velocidade): #called periodically
        if(self.velocidade != velocidade):
            self.__mudancaDeVelocidade = True
        self.velocidade = velocidade
        # set direction
        if self.velocidade>0:
            self.dir = 1
            self.dir_pin.value(1)
        elif self.velocidade<0:
            self.dir = -1
            self.dir_pin.value(0)
        else:
            self.dir = 0
        if abs(self.velocidade)>0:
            self.velocidade = 10000//abs(self.velocidade)
            self.__velocidadeAtual = self.__aceleracao
    
    def setPulsos(self, distancia):
        if(self.pulsos == 0):
            self.__mudancaDeVelocidade = True
        self.pulsos = distancia

    def estaGirando(self):
        if self.pulsos != 0:
            return True
        return False
       

                
            
        
            
    
