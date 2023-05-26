import hcsr04

class Main:
    
    tela = None
    sensor1 = None
    def __init__(self,tela):
        self.tela = tela
        self.sensor1 = hcsr04.HCSR04(trigger_pin=32, echo_pin=34,echo_timeout_us=1000000)

    def comeca(self):
        self.tela.limpaLinha(0)
        self.tela.text('prog2 chamado', 0, 0)
        self.tela.show()

    def distancia(self):
        self.sensor1._send_pulse_and_wait()
        print(self.sensor1.distance_mm())