import network
import os
import time
class Wifi:
    tela = None
    ssid = "robo_esp"
    senha = "12345678"
    ip = "192.168.1.1"
    mascara = "255.255.255.0"
    gateway = "192.168.1.1"
    dns = "192.168.1.1"
    wlan = None
    modo = None

    

    def __init__(self,oled):
        self.tela = oled
        network.phy_mode('MODE_11N');
        network.country('BR');
        #self.wlan.active(False)
        #verifica se tem arquivo de configuracao
        #if os.path.exists("wificonfig.txt"):
        if 'wificonfig.txt' in os.listdir():
            print("arquivo de configuracao wifi encontrado")
            arquivo = open("wificonfig.txt", "r")
            self.ssid = arquivo.readline()
            self.ssid = self.ssid.rstrip("\n")
            self.senha = arquivo.readline()
            self.senha = self.senha.rstrip("\n")
            self.ip = arquivo.readline()
            self.ip = self.ip.rstrip("\n")
            self.mascara = arquivo.readline()
            self.mascara = self.mascara.rstrip("\n")
            self.gateway = arquivo.readline()
            self.gateway = self.gateway.rstrip("\n")
            self.dns = arquivo.readline()
            self.dns = self.dns.rstrip("\n")
            arquivo.close()
        
        
#wlan.config('mac')      # get the interface's MAC address
#wlan.ifconfig()         # get the interface's IP/netmask/gw/DNS addresses
    def clearWifiConfiguration(self):
        os.remove("wificonfig.txt")


    def criaConexao(self):
        if self.ssid != "robo_esp":
            self.wlan = network.WLAN(network.STA_IF) # create station interface
            self.wlan.active(True)
            self.wlan.ifconfig((self.ip, self.mascara, self.gateway, self.dns))
            self.wlan.connect(self.ssid, self.senha)
            self.modo = "STA"
        #wlan.ifconfig(('192.168.3.4', '255.255.255.0', '192.168.3.1', '8.8.8.8'))
        else:
            self.wlan = network.WLAN(network.AP_IF) # se nao tiver configuracao poe no modo ap
            self.modo = "AP"
            self.wlan.ifconfig((self.ip, self.mascara, self.gateway, self.dns))
            self.wlan.config(ssid=self.ssid, key=self.senha)
            #self.wlan.connect(self.ssid, self.senha)
        self.tela.limpaLinha(0)
        if(self.modo == "STA"):
            self.tela.text('Conectando...', 0, 0)
        if(self.modo == "AP"):
            self.tela.text('Modo AP', 0, 0)
        self.tela.show()

    def estaConectado(self):
        conectado = self.wlan.isconnected()
        if(conectado == True):
            self.tela.limpaLinha(0)
            self.tela.text('Conectado', 0, 0)
            self.tela.show()
        return conectado

    def conecta(self):
        i=4 #tenta 4 segundos
        self.criaConexao()
        if(self.modo == "STA"):
            time.sleep(1)
            while(not self.estaConectado()):
                i = i - 1
                if(i == 0):
                    self.tela.limpaLinha(0)
                    self.tela.text('Sem Wifi', 0, 0)
                    self.tela.show()
                    return False
                time.sleep(1)
        return True
    
    def desconecta(self):
        self.wlan.active(False)
        self.tela.limpaLinha(0)
        self.tela.text('Wifi Desligado', 0, 0)
        self.tela.show()
 