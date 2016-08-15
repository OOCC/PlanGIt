#!/usr/bin/python
# -*- coding: utf-8 -*-
import pygame


def getPos():
    # find the line click by mouse
    mouse = pygame.mouse.get_pos()

    # 求商和余数,利用余数来判断鼠标指的位置是不是在线上。
    x, xm = divmod(mouse[0], 64.0)

    #如果x大于6，说明超过了网格线，直接复制为无效值。
    if x > 6:
        x = 100

    # 如果余数大于5，则鼠标不在线上，将x赋值为100，100定为无效值。
    if xm > 5:
        x = 100

    # 这里有一种特殊情况，当鼠标指向的是第一根线时，无需判断余数，直接判断鼠标位置。(覆盖余数大于5的结果)
    if mouse[0] <= 5:
        x = 0


    #y坐标的获得同理
    y, ym = divmod(mouse[1], 64)
    if y > 6:
        y = 100

    if ym > 5:
        y = 100

    if mouse[1] <= 5:
        y = 0

    return x, y, mouse