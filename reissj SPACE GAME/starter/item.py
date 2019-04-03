#the only changes i made to this file were:
#   give weight attribute
#   give type attribute
#   give equippable attribute
#   give monetary value
#   give 2 stat attributes - one for main stat, one for editing accuracy.
import os

def clear():
    os.system('cls' if os.name == 'nt' else 'clear')

class Item:
    def __init__(self, name, desc, weight, value, type = "misc", equippable = False, stat = 0, stat2 = 0):
        self.name = name
        self.desc = desc
        self.loc = None
        self.type = type
        self.weight = weight
        self.equippable = equippable
        self.value = value
        self.stat = stat
        self.stat2 = stat2
    def describe(self):
        clear()
        print(self.desc)
        print()
        input("Press enter to continue...")
    def putInRoom(self, room):
        self.loc = room
        room.addItem(self)
