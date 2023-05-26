# Define a classe Menu para representar o menu completo
INICIO_TELA_Y = 17
TAMANHO_LINHA = 8
MAX_LINHA = 6
import os
SUBMENU = ("Executa", "Exclui", "Padrao", "Voltar")
class Menu:
    tela = None
    selectedItem = 0
    selectedSubitem = None
    optionSelect = False
    #fiz como tupla ( foi o que sabia)
    items = (
            #("Opcao", ("Executa", "Exclui", "Padrao")),
            #("Opcao 2", ("Subopcao 2.1", "Subopcao 2.2", "Subopcao 2.3")),
            #("Opcao 3", ("Subopcao 3.1", "Subopcao 3.2", "Subopcao 3.3")),
            #("Opcao 4", ("Subopcao 4.1", "Subopcao 4.2", "Subopcao 4.3")),
            #("Opcao 5", ("Subopcao 5.1", "Subopcao 5.2", "Subopcao 5.3")),
            #("Opcao 6", ("Subopcao 6.1", "Subopcao 6.2", "Subopcao 6.3"))
    )
    def __init__(self, tela):
        self.tela = tela
        self.selectedItem = 0
        self.selectedSubitem = None
        #crio os itens de menu com base nas pastas
        pastas = os.ilistdir(".")
        for pasta in pastas:
            if pasta[1] == 0x4000:
                self.items+=((pasta[0],SUBMENU),)


    def drawMenu(self):
        i=2
        while i < 8:
            self.tela.limpaLinha(i)
            i=i+1
        #self.tela.text("Menu:", 0, INICIO_TELA_Y)
        #desenho o menu principal se nao tiver seleção de subitem
        if self.selectedSubitem is None:
            i = self.selectedItem
            self.tela.text(">",0,INICIO_TELA_Y)
            linha = 0
            while i < len(self.items) and linha < MAX_LINHA:
                self.tela.text(self.items[i][0],10,INICIO_TELA_Y+linha*TAMANHO_LINHA)
                i = i + 1
                linha = linha + 1
        else:
            optionSelected = self.items[self.selectedItem][0] #nome do menu
            listaSubmenu = self.items[self.selectedItem][1] #lista com submenu
            #imprime opcao selecionada
            self.tela.limpaLinha(0)
            self.tela.text(optionSelected,0,0)
            i = self.selectedSubitem
            linha = 0
            self.tela.text(">",0,INICIO_TELA_Y)
            while i < len(listaSubmenu) and linha < MAX_LINHA:
                self.tela.text(listaSubmenu[i],10,INICIO_TELA_Y+linha*TAMANHO_LINHA)
                i = i + 1
                linha = linha + 1
        self.tela.show()

    def up(self):
        if self.selectedSubitem is not None:
            if self.selectedSubitem > 0:
                self.selectedSubitem -= 1
        else:
            if self.selectedItem > 0:
                self.selectedItem -= 1
                self.selectedSubitem = None
        self.drawMenu()

    def down(self):
        if self.selectedSubitem is not None:
            if self.selectedSubitem < len(self.items[self.selectedItem][1]) - 1:
                self.selectedSubitem += 1
        else:
            if self.selectedItem <  len(self.items) - 1:
                self.selectedItem += 1
                self.selectedSubitem = None
        self.drawMenu()


    #tenho que analisar a opcao que esta de submenu quando é selecionada
    def select(self):
        if self.selectedSubitem is None:
            self.selectedSubitem = 0
            self.drawMenu()
            return -1
        else:
            self.optionSelect = True
            return self.selectedSubitem


    def back(self):
        self.selectedSubitem = None
        self.drawMenu()

    def retornaApp(self): 
        module = __import__(self.items[self.selectedItem][0]+".main")
        classe = getattr(module,"main")
        app = classe.Main(self.tela)
        return app
        #app = 
        #return app