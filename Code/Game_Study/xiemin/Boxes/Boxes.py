import pygame
import math
from PodSixNet.Connection import ConnectionListener, connection
from time import sleep

class BoxesGame(ConnectionListener):
    def initGraph(self):
        self.normallinev = pygame.image.load("./resources/normalline.png")
        self.normallineh = pygame.transform.rotate(pygame.image.load("./resources/normalline.png"),-90)

        self.bar_donev = pygame.image.load("./resources/bar_done.png")
        self.bar_doneh = pygame.transform.rotate(pygame.image.load("./resources/bar_done.png"),-90)

        self.hoverlinev = pygame.image.load("./resources/hoverline.png")
        self.hoverlineh = pygame.transform.rotate(pygame.image.load("./resources/hoverline.png"),-90)

        self.separators = pygame.image.load("./resources/separators.png")
        self.redindicator = pygame.image.load("./resources/redindicator.png")

        self.greenplayer = pygame.image.load("./resources/greenplayer.png")
        self.blueplayer = pygame.image.load("./resources/blueplayer.png")

        self.winningscreen = pygame.image.load("./resources/youwin.png")
        self.gameover = pygame.image.load("./resources/gameover.png")
        self.score_panel = pygame.image.load("./resources/score_panel.png")

        self.greenindicator = pygame.image.load("./resources/greenindicator.png")

    def drawBoard(self):
        for x in range(6):
            for y in range(7):
                if not self.boardh[y][x]:
                    self.screen.blit(self.normallineh, [(x)*64+5, (y)*64])
                else:
                    self.screen.blit(self.bar_doneh, [(x)*64+5, (y)*64])

        for x in range(7):
            for y in range(6):
                if not self.boardv[y][x]:
                    self.screen.blit(self.normallinev, [(x)*64, (y)*64+5])
                else:
                    self.screen.blit(self.bar_donev, [(x)*64, (y)*64+5])

        #draw separators
        for x in range(7):
            for y in range(6):
                self.screen.blit(self.separators, [x*64, y*64])

        #draw HUD
        self.drawHUD()

    def drawHUD(self):
        #draw the background for button
        self.screen.blit(self.score_panel, [0, 389])

        #create font
        myfont = pygame.font.SysFont(None, 32)

        #create text surface
        label = myfont.render("Your Turn:", 1, (255,255,255))

        #draw surface
        self.screen.blit(label, (10, 400))

        #draw pointer picture
        self.screen.blit(self.greenindicator if self.turn else self.redindicator, (130, 395))

        #add player score txt
        myfont64 = pygame.font.SysFont(None, 64)
        myfont20 = pygame.font.SysFont(None, 20)

        scoreme = myfont64.render(str(self.me), 1, (255,255,255))
        scoreother = myfont64.render(str(self.otherplayer), 1, (255,255,255))
        scoretxtme = myfont20.render("You", 1, (255,255,255))
        scoretxtother = myfont20.render("Other Player", 1, (255,255,255))

        self.screen.blit(scoretxtme, (10, 425))
        self.screen.blit(scoreme, (10,435))
        self.screen.blit(scoretxtother, (280, 425))
        self.screen.blit(scoreother, (340, 435))

    def Network_place(self, data):
        print "Network_place"
		
        #get attributes
        x = data["x"]
        y = data["y"]
        hv = data["is_horizontal"]

        #horizontal or vertical
        if hv:
            self.boardh[y][x] = True
        else:
            self.boardv[y][x] = True
			
    def Network_startgame(self, data):
        print "Network_startgame"
        self.running = True
        self.num = data["player"]
        self.gameid = data["gameid"]

    def Network_youturn(self, data):
        print "Network_youturn"
        self.turn = data["torf"]

    def Network_win(self, data):
        print "Network_win"
        self.owner[data["x"]][data["y"]] = "win"
        self.boardh[data["y"]][data["x"]]  = True
        self.boardv[data["y"]][data["x"]] = True
        self.boardh[data["y"]+1][data["x"]] = True
        self.boardv[data["y"]][data["x"]+1] = True

        #add one point to my score
        self.me += 1

    def Network_lose(self, data):
        print "Network_lose"
        self.owner[data["x"]][data["y"]] = "lose"
        self.boardh[data["y"]][data["x"]] = True
        self.boardv[data["y"]][data["x"]] = True
        self.boardh[data["y"] + 1][data["x"]] = True
        self.boardv[data["y"]][data["x"] + 1] = True

        #add one to other players socre
        self.otherplayer += 1

    def __init__(self):
        self.num = None
        self.gameid = None
        self.justplaced = 10
        self.boardh = [[False for x in range(6)] for y in range(7)]
        self.boardv = [[False for x in range(7)] for y in range(6)]

        #init
        width, height = 389, 489
        pygame.init()

        #init HUD
        pygame.font.init()

        #init the screen
        self.screen = pygame.display.set_mode((width, height))
        pygame.display.set_caption("Fighting Boxes")

        #init pygame clock
        self.clock = pygame.time.Clock()

        #show board
        self.initGraph()

        #init PodSixNet client
        self.Connect()

        #point who
        self.turn = True

        #create player score file
        self.me = 0
        self.otherplayer = 0
        self.didiwin=False

        #player cube counter
        self.owner = [[0 for x in range(6)] for y in range(6)]

        #waitting for server send start_game signal
        self.running = False
        while not self.running:
            connection.Pump()
            self.Pump()
            sleep(0.01)

        #determine attributes from player
        if self.num == 0:
            self.turn = True
            self.marker = self.greenplayer
            self.othermarker = self.blueplayer
        else:
            self.turn = False
            self.marker = self.blueplayer
            self.othermarker = self.greenplayer  #warn: word spell error. because othermarker is not a English-word

    def update(self):
        #control player put line at least 10 frame
        self.justplaced -= 1

        #connect server
        connection.Pump()
        self.Pump()

        #sleep to make the game 60 fps
        self.clock.tick(60)

        #clear the screen
        self.screen.fill(0)

        #draw board
        self.drawBoard()

        #quit if the quit button was pressed
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                exit()

        #draw line
        mouse = pygame.mouse.get_pos()
        xpos = int(math.ceil((mouse[0]-32)/64.0))
        ypos = int(math.ceil((mouse[1]-32)/64.0))
        is_horizontal = abs(mouse[1] - ypos*64) < abs(mouse[0] - xpos*64)

        ypos = ypos - 1 if mouse[1] - ypos*64 < 0 and not is_horizontal else ypos
        xpos = xpos - 1 if mouse[0] - xpos*64 < 0 and is_horizontal else xpos
        board = self.boardh if is_horizontal else  self.boardv
        isoutofbounds = False

        try:
            if not board[ypos][xpos]: self.screen.blit(self.hoverlineh if is_horizontal else self.hoverlinev, [xpos*64+5 if is_horizontal else xpos*64, ypos*64 if is_horizontal else ypos*64+5])
        except:
            isoutofbounds=True
            pass
        if not isoutofbounds:
            alreadyplaced=board[ypos][xpos]
        else:
            alreadyplaced=False

        #make player get line when mouse left click, and send to server
        if pygame.mouse.get_pressed()[0] and not alreadyplaced and not isoutofbounds and self.turn == True and self.justplaced <= 0:
            if is_horizontal:
                self.boardh[ypos][xpos]=True
                self.Send({"action":"place", "x": xpos, "y":ypos, "is_horizontal":is_horizontal, "gameid":self.gameid, "num":self.num})
            else:
                self.boardv[ypos][xpos]=True
                self.Send({"action":"place", "x": xpos, "y":ypos, "is_horizontal":is_horizontal, "gameid":self.gameid, "num":self.num})
            self.justplaced = 10
        
        #draw owner map
        self.drawOwnerMap()
		
        #update the screen
        pygame.display.flip()

    def drawOwnerMap(self):
        for x in range(6):
            for y in range(6):
                if self.owner[x][y] != 0:
                    if self.owner[x][y] == "win":
                        self.screen.blit(self.marker, (x*64+5, y*64+5))
                    if self.owner[x][y] == "lose":
                        self.screen.blit(self.othermarker, (x*64+5, y*64+5))

    def finished(self):
        self.screen.blit(self.gameover if not self.didiwin else self.winningscreen, (0,0))
        while 1:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    exit()
            pygame.display.flip()

#__init__ is called right here
bg = BoxesGame()
while 1:
    bg.update()