#!/usr/bin/python
# -*- coding: utf-8 -*-
import pygame
import math
from get_pos import *


class BoxesGame():
    def initGraphics(self):
        self.normallinev = pygame.image.load('./resource/normalline.png')
        self.normallineh = pygame.transform.rotate(pygame.image.load('./resource/normalline.png'), -90)

        self.bar_donev = pygame.image.load('./resource/bar_done.png')
        self.bar_doneh = pygame.transform.rotate(pygame.image.load('./resource/bar_done.png'), -90)

        self.hoverlinev = pygame.image.load('./resource/hoverline.png')
        self.hoverlineh = pygame.transform.rotate(pygame.image.load('./resource/hoverline.png'), -90)

    def __init__(self):
        pass
        pygame.init()
        width, height = 389, 489

        #initialize the screen
        self.screen = pygame.display.set_mode((width, height))
        pygame.display.set_caption("Boxes")

        #initialize pygame clock
        self.clock = pygame.time.Clock()
        self.boardh = [[False for x in range(6)] for y in range(7)]
        self.boardv = [[False for x in range(7)] for y in range(6)]
        self.initGraphics()




    def update(self):
        #sleep to make the game 60 fps
        self.clock.tick(60)

        #clear the screen
        self.screen.fill(0)

        #draw the broad
        self.drawBorad()

        for event in pygame.event.get():
            #quit if the quit button was pressed
            if event.type == pygame.QUIT:
                exit()

        #find the line by mouse

        #xpos, ypos, mouse = getPos()
        # 1
        mouse = pygame.mouse.get_pos()

        # 2
        xpos = int(math.ceil((mouse[0] - 32) / 64.0))
        ypos = int(math.ceil((mouse[1] - 32) / 64.0))

        # 3
        is_horizontal = abs(mouse[1] - ypos * 64) < abs(mouse[0] - xpos * 64)

        # 4
        ypos = ypos - 1 if mouse[1] - ypos * 64 < 0 and not is_horizontal else ypos
        xpos = xpos - 1 if mouse[0] - xpos * 64 < 0 and is_horizontal else xpos

        # 5
        board = self.boardh if is_horizontal else self.boardv
        isoutofbounds = False

        # 6
        try:
            if not board[ypos][xpos]: self.screen.blit(self.hoverlineh if is_horizontal else self.hoverlinev,
                                                       [xpos * 64 + 5 if is_horizontal else xpos * 64,
                                                        ypos * 64 if is_horizontal else ypos * 64 + 5])
        except:
            isoutofbounds = True
            pass
        if not isoutofbounds:
            alreadyplaced = board[ypos][xpos]
        else:
            alreadyplaced = False


        #update the screen
        pygame.display.flip()

    def drawBorad(self):
        for x in range(6):
            for y in range(7):
                if not self.boardh[y][x]:
                    self.screen.blit(self.normallineh, [x*64+5, y*64])
                else:
                    self.screen.blit(self.bar_doneh,  [x*64+5, y*64])
        for x in range(7):
            for y in range(6):
                if not self.boardv[y][x]:
                    self.screen.blit(self.normallinev, [x*64, y*64+5])
                else:
                    self.screen.blit(self.bar_doneh, [x*64+5, y*64])

bg = BoxesGame()
while 1:
    bg.update()



