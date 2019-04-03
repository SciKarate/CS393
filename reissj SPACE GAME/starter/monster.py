import random
import updater
import os
from collections import defaultdict

#define base class Animal.
#   movefreq is the chance from 0.0-1.0 that they will move as time passes
#   speed is their speed stat
#   acc is their acc stat
#   stre is their strength stat
#   loot is what items they can drop when killed
#   droprate is how often they drop loot
#   aggro is the chance from 0.0-1.0 that they will attack the player on a turn
class Animal:
    def __init__(self, name, health, room, xp, loot = [], droprate = .5, movefreq = .1, speed = 1, acc = 1, stre = 1, aggro = 0):
        self.name = name
        self.health = health
        self.room = room
        self.movefreq = movefreq 
        self.speed = speed
        self.acc = acc
        self.stre = stre
        self.xp = xp
        self.loot = loot
        self.droprate = droprate
        self.aggro = aggro
        room.addMonster(self)
        updater.register(self)
    def update(self):
        if random.random() < self.movefreq:
            self.moveTo(self.room.randomNeighbor())
    def moveTo(self, room):
        self.room.removeMonster(self)
        self.room = room
        room.addMonster(self)
    
    #on death, roll for droprate to see if loot will be placed. choose 1 loot randomly.
    def die(self):
        chance = random.random()
        if (chance < self.droprate):
            if(len(self.loot) > 0):
                cap = len(self.loot) - 1
                z = random.randint(0,cap)
                print()
                print(self.name + " dropped a " + self.loot[z].name + ".")
                self.loot[z].putInRoom(self.room)
        self.room.removeMonster(self)
        updater.deregister(self)

    #roll to see if monster will attack player
    def fightback(self, player):
        fighting = random.random()
        if (fighting < self.aggro):
            print("\nYou are being attacked by " + self.name + "!!!")
            input("Press enter to continue...")
            player.attackMonster(self)

#define monster class, monster is same as animal, but it defaults to having higher stats.
#this ended up not mattering since I ended up defining lots of monsters manually,
#but I originally was going to lazily let a random number generator assign stats.
#I realized this was a terrible idea pretty quickly:
#some monsters would die in one hit and give way too much xp
#other monsters would one-shot you and give you nothing.
#rather than write a really careful algorithm for this, I figured i'd do it manually.
class Monster(Animal):
    def __init__(self, name, health, room, xp, loot = [], droprate = .5, movefreq = .75, speed = 5, acc = 5, stre = 5, aggro = 0):
        Animal.__init__(self, name, health, room, xp, loot, droprate, movefreq, speed, acc, stre, aggro)

#shopkeeps are animals that don't move, have money, can talk, play an intro, and can trade
class Shopkeep(Animal):
    def __init__(self, name, room, money, items = [], movefreq = 0, intro = "What're ya buyin?"):
        Animal.__init__(self, name, 100, room, 5, items, 1, movefreq, 5, 20, 20, 0)
        self.items = items
        self.money = money
        self.intro = intro
#if the player wants to trade:
#   let them choose to buy or sell
#   if buy:
#       show shopkeepers items. let player buy one.
#   if sell:
#       show player's items. let them sell one.
#if the player wants to trade and they have previously stolen from the trader,
#   refuse all trade.
    def trade(self, playe):
        notdone = True
        if(self.money == -10000):
            print("\nBEGONE, THIEF!\n")
            notdone = False
        while notdone:  
            clear()
            command = input("You can buy, sell, or tap enter to get going.\n:")
            commandWords = command.split()
            if not commandWords:
                notdone = False                
            elif commandWords[0].lower() == "buy":
                self.buy(playe)
            elif commandWords[0].lower() == "sell":
                print()
                self.sell(playe)
            else:
                print("Invalid command.")
                notdone = False
        self.loot = self.items
        input("Press enter to continue...")

    def buy(self, playe):
        print()
        notdone = True
        while notdone:
            print(self.intro)
            print()
            storeinv(self)
            print()
            print("You have " + str(playe.money) + " dollars.")
            print()
            command = input("What do you want to buy?\n:")
            if not command:
                notdone = False
            else:
                found = False
                for i in self.items:
                    itname = i.name
                    if(command == itname.lower()):
                        ze = i
                        found = True
                if(found == True):
                    print()
                    if(playe.money >= ze.value):
                        print("Successfully purchased " + str(ze.name) + ".")
                        playe.money -= ze.value
                        playe.currweight += ze.weight
                        playe.items.append(ze)
                        self.items.remove(ze)
                    else:
                        print("You do not have enough money!")
                else:
                    print("\nThat is not an item they have.")
                notdone = False
        input("Press enter to continue...")

    def sell(self, playe):
        notdone = True
        while notdone:
            storeinv(playe)
            print()
            print("You have " + str(playe.money) + " dollars.")
            print()
            scommand = input("What do you want to sell?\n:")
            if not scommand:
                notdone = False
            else:
                found = False
                for i in playe.items:
                    itname = i.name
                    if(scommand.lower() == itname.lower()):
                        ze = i
                        found = True
                if(found == True):
                    print()
                    print("Successfully sold " + str(ze.name) + ".")
                    playe.money += ze.value
                    playe.currweight -= ze.weight
                    self.items.append(ze)
                    playe.items.remove(ze)
                else:
                    print("\nThat is not an item you have.")
                notdone = False
        input("Press enter to continue...")

#let the player move through a number-based dialogue tree.
#if the player steals from the trader, make it so the trader won't trade anymore.
    def speak(self, playe):
        notdone = True
        command = "0"
        command2 = "0"
        command3 = "0"
        if(self.money == -10000):
            print("BEGONE, THIEF!\n")
            notdone = False
            input("Press enter to continue...")
        else:
            clear()
        while notdone:
            if command == "0":
                print("Hey! Howdy! What do you want to talk about?")
                print()
                print("1 - money")
                print("2 - space")
                print("3 - the economy")
                print("4 - nothing")
                print("5 - goodbye!")
                command = input(":")
            elif(command == "1" or command == "3"):
                if command2 == "0":
                    clear()
                    print("Where do you think the money comes from?")
                    print("Is there a mint in space?")
                    print("Does the value of a credit inflate over time?")
                    print("I have " + str(self.money) + " credits, myself.")
                    print()
                    print("1 - give me your money")
                    print("2 - that's nice")
                    print("3 - give me all of your money now please")
                    print("4 - i will shoot you if you do not give me all of your money now please")
                    command2 = input(":")
                elif (command2 == "1"):
                    clear()
                    print("No!\n")
                    command2 = "0"
                    command = "0"
                elif (command2 == "2"):
                    clear()
                    print("yeah.\n")
                    command2 = "0"
                    command = "0"
                elif (command2 == "3" or command2 == "4"):
                    clear()
                    print("AHHHHHHHHHHHHHHHHHHHHH\n")
                    print("\nYou got " + str(self.money) + " credits.")
                    playe.money += self.money
                    self.money = -10000
                    command2 = "0"
                    command = "6"
                else:
                    clear()
                    command2 = "0"
                    command = "0"
            elif(command == "2"):
                if command3 == "0":
                    clear()
                    print("I like space. There are rocks here.")
                    print()
                    command3 = input("What about you?\n:")
                elif command3 == "I like space. There are rocks here.":
                    clear()
                    print("LITERALLY SAME\n")
                    print("You are about to level up.\n")
                    playe.xp = (playe.lvlupthresh - 1)
                    command3 = "0"
                    command = "0"
                else:
                    clear()
                    print("yeah me too\n")
                    command3 = "0"
                    command = "0"
            elif(command == "4"):
                print("\nWhy are you talking to me then?")
                print("Bye!\n")
                notdone = False
                input("Press enter to continue...")
            elif(command == "6"):
                print("\nYou can leave now.\n")
                notdone = False
                input("Press enter to continue...")
            else:
                print("\nOkay, bye!")
                notdone = False
                input("Press enter to continue...")

def clear():
        os.system('cls' if os.name == 'nt' else 'clear')

#print the inventory of the specified being (trader of player)
#do so in a format that is better suited to the store than the normal inventory function
#(i.e. doesn't show equipped items since you can't use them)
def storeinv(being):
    itdict = defaultdict(list)
#for explanation, look at player.showinventory()
    for i in being.items:
            if(str(i.name) in itdict):
                itdict[str(i.name)][0] += 1
            else:
                itdict[str(i.name)].append(1)
                itdict[str(i.name)].append(i.weight)
                itdict[str(i.name)].append(i.value)
    print("Item\t\tQty\tWeight\tValue")
    i = 0
    for k, v in itdict.items():
        print(str(k) + "\tx" + str(v[0]) + "\t" + str(v[1]) +"\t" + str(v[2]))
        i += 1
    itdict =  {k.lower(): v for k, v in itdict.items()}