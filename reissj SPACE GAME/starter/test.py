from room import Room
from player import Player
from item import Item
from monster import Monster
import os
import updater
worldL = []
worldsize = 4
i = 0
while i < worldsize:
    worldL.append([])
    z = [0] * (worldsize)
    worldL[i].extend(z)
    i += 1

def clear():
    os.system('cls' if os.name == 'nt' else 'clear')

def createWorld():
    i = 0
    while i < worldsize:
        worldL.append([])
        z = [0] * (worldsize)
        worldL[i].extend(z)
        i += 1
    r00 = Room("You are in empty space.")
    addRoom(r00, worldL, worldsize, 0, 0)
    r00 = Room("You are in empty space. There is a nebula to the North.")
    addRoom(r00, worldL, worldsize, 1, 0)
    r00 = Room("You are in empty space. There is a nebula to the East.")
    addRoom(r00, worldL, worldsize, 0, 1)
    r00 = Room("You are in a nebula. It is difficult to make out anything.")
    addRoom(r00, worldL, worldsize, 1, 1)

    print(worldL)

def addRoom(name, worldL, worldsize, x, y):
    worldL[x][y] = name
    if(x > 0):
        if (worldL[x-1][y] != 0):
            Room.connectRooms(name, "west", worldL[x - 1][y], "east")
    if(x < worldsize - 1):
        if (worldL[x][y + 1] != 0):
            Room.connectRooms(name, "north", worldL[x][y + 1], "south")
    if(y > 0):
        if (worldL[x][y - 1] != 0):
            Room.connectRooms(name, "south", worldL[x][y - 1], "north")
    if(y < worldsize - 1):
        if (worldL[x + 1][y] != 0):
            Room.connectRooms(name, "east", worldL[x + 1][y], "west")