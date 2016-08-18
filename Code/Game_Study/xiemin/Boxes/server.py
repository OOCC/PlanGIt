from time import sleep
import PodSixNet.Server
import PodSixNet.Channel


class  ClientChannel(PodSixNet.Channel.Channel):
    def Network(self, data):
        print data

    def Network_place(self, data):
        #horizontal or vertical
        hv = data["is_horizontal"]

        #x of placed line
        x = data["x"]

        #y of placed line
        y = data["y"]

        #player number (1 or 0)
        num = data["num"]

        #id of game given by server at of start of game
        self.gameid = data["gameid"]

        #tells server to place line
        self._server.placeLine(hv, x, y, data, self.gameid, num)

class BoxesServer(PodSixNet.Server.Server):
    def __init__(self, *args, **kwargs):
        PodSixNet.Server.Server.__init__(self, *args, **kwargs)
        self.games = []
        self.queue = None
        self.curIndex = 0

    channelClass = ClientChannel
    def Connected(self, channel, addr):
        print "new connection:", channel

        if self.queue == None:
            self.curIndex += 1
            channel.gameid = self.curIndex
            self.queue = Game(channel, self.curIndex)
        else:
            #print {"new player join in:":self.curIndex}
            channel.gameid = self.curIndex
            self.queue.player1 = channel
            self.queue.player0.Send({"action":"startgame", "player":0, "gameid":self.queue.gameid})
            self.queue.player1.Send({"action":"startgame", "player":1, "gameid":self.queue.gameid})
            self.games.append(self.queue)
            self.queue = None

    def placeLine(self, is_h, x, y, data, gameid, num):
        game = [a for a in self.games if a.gameid == gameid]
        if len(game) == 1:
            game[0].placeLine(is_h, x, y, data, num)

    def tick(self):
        #1
        index = 0
        change = 3

        #2
        for game in self.games:
            change = 3
            for time in range(2):
                #3
                for y in range(6):
                    for x in range(6):
                        #4
                        if game.boardh[y][x] and game.boardv[y][x] and game.boardh[y+1][x] and game.boardv[y][x+1] and not game.owner[x][y]:
                            if self.games[index].turn == 0:
                                self.games[index].owner[x][y] = 2
                                game.player1.Send({"action":"win", "x":x, "y":y})
                                game.player0.Send({"action":"lose", "x":x, "y":y})
                                change = 1
                            else:
                                self.games[index].owner[x][y] = 1
                                game.player0.Send({"action": "win", "x": x, "y": y})
                                game.player1.Send({"action": "lose", "x": x, "y": y})
                                change = 0
            #5
            self.games[index].turn = change if change != 3 else self.games[index].turn
            game.player1.Send({"action":"youturn", "torf":True if self.games[index].turn==1 else False})
            game.player0.Send({"action": "youturn", "torf": True if self.games[index].turn == 0 else False})
            index += 1

        self.Pump()

class Game:
    def __init__(self, player0, curIndex):
        #print "create a new game."
        #print {"curIndex":curIndex}

        #whose turn(1 or 0)
        self.turn = 0

        #owner map
        self.owner = [[False for x in range(6)] for y in range(6)]

        #seven lines in each direction to make a six by six grid
        self.boardh = [[False for x in range(6)] for y in range(7)]
        self.boardv = [[False for x in range(7)] for y in range(6)]

        #initialize the players including the one who started the game
        self.player0 = player0
        self.player1 = None

        #gameid of game
        self.gameid = curIndex

    def placeLine(self, is_h, x, y, data, num):
        #make sure is their turn
        if num == self.turn:
            self.turn = 0 if self.turn else 1
            
            #place line in game
            if is_h:
                self.boardh[y][x] = True
            else:
                self.boardv[y][x] = True
            
            #send data and turn data to each player
            self.player0.Send(data)
            self.player1.Send(data)
			
            #tell players who turn
            self.player1.Send({"action":"youturn", "torf":True if self.turn==1 else False})
            self.player0.Send({"action":"youturn", "torf":True if self.turn==0 else False})

print "STARTING SERVER ON LOCALHOST"
BoxesSvr = BoxesServer()
while True:
        #BoxesSvr.Pump()
        BoxesSvr.tick()
        sleep(0.01)

