from room import Room
from player import Player
from item import Item
from monster import Monster
from monster import Shopkeep
from monster import Animal
import random
import os
import updater

#defines player object
player = Player()
#defines empty list and variable for creating world
worldsize = 64
worldL = []
#empty list for storing random items
randitems = []
#empty list for storing random asteroids
asteroids = []

def createWorld():
    #makes worldL list into worldL[worldsize][worldsize]
    i = 0
    while i < worldsize:
        worldL.append([])
        z = [0] * (worldsize)
        worldL[i].extend(z)
        i += 1

    #randomly define world attributes
    randomworld(0, 0)

    #place player in middle of world
    plx = worldsize // 2
    player.location = worldL[plx][plx]
    worldL[plx][plx].desc = "You are in empty space. There is a strange lifeform north of here."

    #place items at player spawn
    tinhull0 = Item("Tin Hull", "Adds 2 armor.", 1, 5, "armor", True, 2)
    repairkit1 = Item("Repair Kit I", "Restores 10 HP.", 5, 20, "healing", True, 10)
    tinhull0.putInRoom(player.location)
    repairkit1.putInRoom(player.location)

    #add asteroids and regen
    addAsteroids(0, 0)

    #randomly create some items
    randomItemsCreate(worldsize)

    #randomly place items
    randomItemsPlace(0, 0)

    #ramdomly generate some monsters
    generateMonsters(worldsize)

#randomly chooses qnty traders
def generateTraders(qnty):
    #kinds of traders i've defined
    lifeformtypes = 100;

    i = 1
    while i <= qnty:
        chosen = random.randint(1, 100)
        xco = random.randint(0, (worldsize - 1))
        yco = random.randint(0, (worldsize - 1))
        if (chosen == 1):
            name = "Rare Dave the Trader"
        elif (chosen > 1) and (chosen <= 10):
            name = "Andy the Trader"
            chosen = 2
        elif (chosen <= 15) and (chosen > 10):
            chosen = 3
            name = "Danny the Trader"
        elif (chosen < 25) and (chosen > 15):
            name = "Stephen the Trader"
            chosen = 4
        elif (chosen >= 25) and (chosen < 100):
            chosen = 6
            name = "Patrick the Trader"
        elif (chosen == 100):
            name = "Starguy the Selling Man"
            print("STARGUY APPEARS")
        if(name != "Starguy the Selling Man"):
            #if not starguy, give the trader as many items as their name calls for
            randomItemsCreate(chosen)
            Shopkeep(name, worldL[xco][yco], 20, randitems[0: (chosen)])
            randomItemsEmpty()
        else:
            #if starguy, give the trader 9 items and a pristine hyperspace engine
            randomItemsCreate(9)
            bup = Item("Pristine Hyperspace Engine", "Provides speed +150", 15, 10, "engine", True, 150)
            randitems.append(bup)
            Shopkeep(name, worldL[xco][yco], 20, randitems[0: (10)])
            randomItemsEmpty()
        i += 1;

#randomly generates and places monsters
def generateMonsters(qnty):
    #kinds of monsters and animals i've defined
    lifeformtypes = 16

    i = 1
    while i <= qnty:
        #this if else chunk spawns monsters in tiers
        #   if player level is 1-5, spawn tier 1.
        #   if player level is 6-9, 25% chance of tier 1, 75% tier 2
        #   if player level is 11+, 10% chance tier 1, 40% tier 2, 50% tier 3 
        tier = random.random()

        #tier 2
        if((player.lvl <= 10) and (player.lvl > 5) and (tier < .75)):
            chosen = random.randint(6, 10)
        
        #tier 3
        elif(tier < .90) and (player.lvl > 10):
            if(tier < .5):
                chosen = random.randint(11, 15)
            else:
                chosen = random.randint(6, 10)
        else:
            chosen = random.randint(0, 5)

        xco = random.randint(0, (worldsize - 1))
        yco = random.randint(0, (worldsize - 1))

        #if the room chosen has > 2 monsters, move through all rooms on same x
        #if all rooms are full, give up
        if(len(worldL[xco][yco].monsters) > 2):
            chosen = 16

        #tier 1
        if (chosen == 0): #Space Whale
            randomItemsCreate(1)
            Animal("Space Whale", 20, worldL[xco][yco], 5, [randitems[0]], .75, .1, 2, 3, 3, .01)
            randomItemsEmpty()
        elif (chosen == 1): #shooty cruiser
            randomItemsCreate(1)
            Monster("Shooty Cruiser", 5, worldL[xco][yco], 2, [randitems[0]], .1, .9, 5, 5, 1, .7)
            randomItemsEmpty()
        elif (chosen == 2): #blasty cruiser
            randomItemsCreate(1)
            Monster("Blasty Cruiser", 10, worldL[xco][yco], 10, [randitems[0]], .5, .75, 5, 7, 5, .7)
            randomItemsEmpty()
        elif (chosen == 3): #speedy cruiser
            randomItemsCreate(1)
            Monster("Speedy Cruiser", 1, worldL[xco][yco], 5, [randitems[0]], .25, .9, 10, 3, 1, .9)
            randomItemsEmpty()
        elif (chosen == 4): #explodey cruiser
            randomItemsCreate(1)
            Monster("Explodey Cruiser", 1, worldL[xco][yco], 5, [randitems[0]], .1, .75, 10, 1, 10, .75)
            randomItemsEmpty()
        elif (chosen == 5): #cargo cruiser
            randomItemsCreate(2)
            Monster("Cargo Cruiser", 1, worldL[xco][yco], 5, randitems[0:2], .9, .9, 5, 1, 1, .01)
            randomItemsEmpty()
        #tier 2
        elif (chosen == 6): #shooty cruiser
            randomItemsCreate(1)
            Monster("Shooty Spaceship", 20, worldL[xco][yco], 4, [randitems[0]], .1, .9, 50, 40, 10, .7)
            randomItemsEmpty()
        elif (chosen == 7): #blasty cruiser
            randomItemsCreate(1)
            Monster("Blasty Spaceship", 40, worldL[xco][yco], 20, [randitems[0]], .5, .75, 60, 80, 20, .7)
            randomItemsEmpty()
        elif (chosen == 8): #speedy cruiser
            randomItemsCreate(1)
            Monster("Speedy Spaceship", 4, worldL[xco][yco], 10, [randitems[0]], .25, .9, 70, 64, 5, .9)
            randomItemsEmpty()
        elif (chosen == 9): #explodey cruiser
            randomItemsCreate(1)
            Monster("Explodey Spaceship", 4, worldL[xco][yco], 10, [randitems[0]], .1, .75, 60, 32, 40, .75)
            randomItemsEmpty()
        elif (chosen == 10): #cargo cruiser
            randomItemsCreate(3)
            Monster("Cargo Spaceship", 4, worldL[xco][yco], 10, randitems[0:3], .9, .9, 40, 32, 5, .01)
            randomItemsEmpty()
        #tier 3
        elif (chosen == 11): #shooty cruiser
            randomItemsCreate(1)
            Monster("Shooty Destroyer", 40, worldL[xco][yco], 8, [randitems[0]], .1, .9, 100, 100, 16, .7)
            randomItemsEmpty()
        elif (chosen == 12): #blasty cruiser
            randomItemsCreate(1)
            Monster("Blasty Destroyer", 80, worldL[xco][yco], 40, [randitems[0]], .5, .75, 120, 320, 80, .7)
            randomItemsEmpty()
        elif (chosen == 13): #speedy cruiser
            randomItemsCreate(1)
            Monster("Speedy Destroyer", 8, worldL[xco][yco], 20, [randitems[0]], .25, .9, 140, 120, 16, .9)
            randomItemsEmpty()
        elif (chosen == 14): #explodey cruiser
            randomItemsCreate(1)
            Monster("Explodey Destroyer", 8, worldL[xco][yco], 20, [randitems[0]], .1, .75, 100, 64, 160, .75)
            randomItemsEmpty()
        elif (chosen == 15): #cargo cruiser
            randomItemsCreate(5)
            Monster("Cargo Freigher", 8, worldL[xco][yco], 20, randitems[0:5], .9, .9, 80, 80, 16, .01)
            randomItemsEmpty()
        i += 1;

#creates as many random items as it's told
#then puts them all in randitems[]
def randomItemsCreate(qnty):
    #kinds of items i've defined
    itemtypes = 22;
    bup = 0;

    i = 1
    while i <= qnty:
        #this if-else chunk makes it so that
        #   if the player is level 1-2, only spawn tier 1 items
        #   if the player is level 3-7, 50% tier 1 items, 50% tier 2
        #   if the player is level 8-14, 25% tier 1 items, 25% tier 2, 50% tier 3
        #   if the player is level 15+, 10% tier 1, 20% tier 2, 50% tier 3, 5% tier 4
        tier = random.random()

        #tier 2
        if((player.lvl <= 7) and (player.lvl > 2) and (tier < .5)):
            chosen = random.randint(7, 12)
        
        #tier 3
        elif(player.lvl < 15) and (tier < .75) and (player.lvl > 7):
            if(tier < .5):
                chosen = random.randint(12, 16)
            else:
                chosen = random.randint(7, 12)
        
        #tier max
        elif(tier < .9) and (player.lvl >= 15):
            if(tier > .95):
                chosen = random.randint(17, 22)
            elif(tier < .5):
                chosen = random.randint(12, 16)
            else:
                chosen = random.randint(7, 12)
        else:
            chosen = random.randint(0, 6)

        if (chosen == 0): #space rock
            bup = Item("Space Rock", "This is just a rock. It probably broke off an asteroid.", 1, 1)
            randitems.append(bup)
        elif(chosen == 1): #Tin Hull
            bup = Item("Tin Hull", "Adds 2 shields.", 1, 5, "armor", True, 2)
            randitems.append(bup)
        #tier 1 items
        elif(chosen == 2): #Iron Hull
            bup = Item("Iron Hull", "Adds 5 shields.", 5, 10, "armor", True, 5)
            randitems.append(bup)
        elif(chosen == 3): #Hydra Cannon I
            bup = Item("Hydra Cannon I", "Prodides accuracy +3, damage +20", 15, 20, "weapon", True, 20, 3)  
            randitems.append(bup)
        elif(chosen == 4): #Quantum Drive I
            bup = Item("Quantum Drive I", "Provides speed +5", 15, 30, "engine", True, 5)
            randitems.append(bup)
        elif(chosen == 5): #Space Lazer I
            bup = Item("Space Lazer I", "Provides accuracy +7 damage +7", 10, 25, "weapon", True, 7, 7)
            randitems.append(bup)
        elif(chosen == 6): #Repair Kit I
            bup = Item("Repair Kit I", "Restores 10 HP.", 5, 10, "healing", True, 10)
            randitems.append(bup)

        #tier 2 items
        elif(chosen == 7): #Repair Kit II
            bup = Item("Repair Kit II", "Restores 20 HP.", 5, 20, "healing", True, 20)
            randitems.append(bup)
        elif(chosen == 8): #Space Lazer II
            bup = Item("Space Lazer II", "Provides accuracy +10 damage +10", 10, 50, "weapon", True, 10, 10)
            randitems.append(bup)
        elif(chosen == 9): #Quantum Drive II
            bup = Item("Quantum II", "Provides speed +10", 15, 60, "engine", True, 10)
            randitems.append(bup)
        elif(chosen == 10): #Hydra II
            bup = Item("Hydra II", "Prodides accuracy +5, damage +40", 15, 40, "weapon", True, 40, 5)  
            randitems.append(bup)
        elif(chosen == 11): #Steel Hull
            bup = Item("Steel Hull", "Adds 10 shields.", 5, 20, "armor", True, 10)
            randitems.append(bup)

        #tier 3 items
        elif(chosen == 12): #Repair Kit III
            bup = Item("Repair Kit III", "Restores 50 HP.", 5, 50, "healing", True, 50)
            randitems.append(bup)
        elif(chosen == 13): #Space Lazer III
            bup = Item("Space Lazer III", "Provides accuracy +25 damage +25", 10, 100, "weapon", True, 25, 25)
            randitems.append(bup)
        elif(chosen == 14): #Quantum Drive III
            bup = Item("Quantum III", "Provides speed +25", 15, 100, "engine", True, 25)
            randitems.append(bup)
        elif(chosen == 15): #Hydra III
            bup = Item("Hydra III", "Prodides accuracy +10, damage +75", 15, 100, "weapon", True, 75, 10)  
            randitems.append(bup)
        elif(chosen == 16): #Titanium Hull
            bup = Item("Titanium Hull", "Adds 30 shields.", 5, 50, "armor", True, 30)
            randitems.append(bup)

        #MAX TIER ITEMS
        elif(chosen == 17): #Full Repair
            bup = Item("Full Repair", "Restores 100 HP.", 5, 100, "healing", True, 100)
            randitems.append(bup)
        elif(chosen == 18): #Space Blaster
            bup = Item("Space Blaster", "Provides accuracy +50 damage +50", 10, 250, "weapon", True, 50, 50)
            randitems.append(bup)
        elif(chosen == 19): #Impulse Drive II
            bup = Item("Battered Hyperspace Engine", "Provides speed +50", 15, 250, "engine", True, 50)
            randitems.append(bup)
        elif(chosen == 20): #Scatterblaster
            bup = Item("Hydra Scatterblaster", "Prodides accuracy +25, damage +150", 15, 250, "weapon", True, 150, 25)  
            randitems.append(bup)
        elif(chosen == 21): #Platinum Hull
            bup = Item("Platinum Hull", "Adds 50 shields.", 5, 100, "armor", True, 50)
            randitems.append(bup)
        elif(chosen == 22): #ENDGAME ENGINE
            bup = Item("Pristine Hyperspace Engine", "Provides speed +100", 15, 250, "engine", True, 50)
            randitems.append(bup)

        i += 1;

#randomly places all of the items in randitems and gets them out of randitems
#places between [xvl-worldsize][yvl-worldsize]
def randomItemsPlace(xvl, yvl):
    z = len(randitems)
    while (z > 0):
        xplace = random.randint(xvl, (worldsize - 1))
        yplace = random.randint(yvl, (worldsize - 1))
        itemchosen = random.randint(0, (z - 1))
        #if the room chosen has > 3 items, place it.
        #otherwise, give up
        if(len(worldL[xplace][yplace].items) < 3):
            randitems[itemchosen].putInRoom(worldL[xplace][yplace])
        randitems.remove(randitems[itemchosen])
        z = len(randitems)

#empty out randitems
def randomItemsEmpty():
    for i in range(len(randitems)):
        randitems.remove(randitems[0])

#for all rooms between [xvl-worldsize][yvl-worldsize],
#assign flavor text.
def randomworld(xvl, yvl):
    r00 = 1
    xv = xvl
    while xv < worldsize:
        yv = yvl
        while yv < worldsize:
            descv = random.randint(0,6)
            if(descv == 0):
                descp = "You are in empty space."
            elif(descv == 1):
                descp = "You are in an asteroid field. There is debris everywhere."
            elif(descv == 2):
                descp = "You are in a nebula. It is difficult to make out anything."
            elif(descv == 3):
                descp = "You are near a refueling station. You will regenerate health much faster here."
            elif(descv == 4):
                descp = "You are near a planet. It looks uninhabitable."
            elif(descv == 5):
                descp = "You are near a star. The inside of your cabin is very hot."
            else:
                descp = "You are in empty space. The stars are bright, and you can go where you please."
            
            #make all rooms at [0][y] and [x][0] say they are the edge
            if(yv == 0) and (xv == 0):
                descp = "You are at the edge of the universe. You cannot go any further south or west."
            elif(yv == 0):
                descp = "You are at the edge of the universe. You cannot go any further south."
            elif(xv == 0):
                descp = "You are at the edge of the universe. You cannot go any further west."
            addRoom(r00, descp, worldL, worldsize, xv, yv)
            yv += 1
        xv += 1

#if the flavor text for any room is "you are in an asteroid field..."
#place 0-3 asteroids
#if the flavor text for any room is refueling station
#make it so the area gives you 10hp/turn instead of 1
def addAsteroids(xvl, yvl):
    asttot = 0
    xv = xvl
    while xv < worldsize:
        yv = yvl
        while yv < worldsize:
            if(worldL[xv][yv].desc == "You are in an asteroid field. There is debris everywhere."):
                asts = random.randint(0, 3)
            else:
                z = random.random()
                if z > .9:
                    asts = 1
                else:
                    asts = 0
            i = 0
            while i < asts:
                r01 = Item("Space Rock", "This is just a rock. It probably broke off an asteroid.", 1, 1)
                asteroids.append(r01)
                asteroids[asttot].putInRoom(worldL[xv][yv])
                asttot += 1
                i += 1
            if(worldL[xv][yv].desc == "You are near a refueling station. You will regenerate health much faster here."):
                worldL[xv][yv].regen = 10
            yv += 1
        xv += 1

#create a room and connect it to all adjacent rooms
def addRoom(name, description, worldL, worldsize, x, y):
    name = Room(description)
    worldL[x][y] = name
    if(x > 0):
        if (worldL[x-1][y] != 0):
            Room.connectRooms(name, "west", worldL[x - 1][y], "east")
    if(y < worldsize - 1):
        if (worldL[x][y + 1] != 0):
            Room.connectRooms(name, "north", worldL[x][y + 1], "south")
    if(y > 0):
        if (worldL[x][y - 1] != 0):
            Room.connectRooms(name, "south", worldL[x][y - 1], "north")
    if(x < worldsize - 1):
        if (worldL[x + 1][y] != 0):
            Room.connectRooms(name, "east", worldL[x + 1][y], "west")

#wipe the screen of all text
def clear():
    os.system('cls' if os.name == 'nt' else 'clear')

#print-
#   the room's flavor text
#   the player's coordinates
#   all monsters in room
#   all items in room
#   available move directions
def printSituation():
    clear()
    print(player.location.desc)
    print("Coordinates: " + str(player.xloc) + ", " + str(player.yloc))
    print()
    if player.location.hasMonsters():
        print("Scanners detect the following liveforms:")
        for m in player.location.monsters:
            print(m.name)
        print()
    if player.location.hasItems():
        print("Scanners detect the following items:")
        for i in player.location.items:
            print(i.name)
        print()
    print("You can go in the following directions:")
    for e in player.location.exitNames():
        print(e)
    print()

def showHelp():
    clear()
    print("go <direction>\t\t-- moves you in the given direction")
    print("inventory\t\t-- opens your inventory")
    print("pickup <item>\t\t-- picks up the item")
    print("drop <item>\t\t-- drops the item")
    print("attack <lifeform>\t-- attacks the lifeform")
    print("equip <item>\t\t-- equips the item")
    print("use <item>\t\t-- uses healing items and equips other items")
    print("dequip <item>\t\t-- dequips the item")
    print("trade <lifeform>\t-- initiates trade with the lifeform")
    print("wait <#>\t\t-- waits # turns")
    print("inspect <item>\t\t-- inspects the item")
    print("talk <lifeform>\t\t-- allows you to speak with traders")
    print("clear\t\t\t-- clears screen")
    print("abbreviations\t\t-- shows abbreviatons")
    print("exit\t\t\t-- quits game")
    print("escape galaxy\t\t-- if your speed is 150 or more, type this to win.")
    print()
    input("Press enter to continue...")

def showAbbrevs():
    clear()
    print("go <direction>\t\t-- can be given \"n\" or \"e\" instead of \"north\" or \"east\"")
    print("inventory\t\t-- can be written as \"inv\"")
    print("abbreviations\t\t-- can be written as \"abbrevs\"")
    print("attack <lifeform>\t-- can be written as \"atk\"")
    print()
    input("Press enter to continue...")

#look through the items in the room. if it wouldn't put you over max weight, grab it.
def pickup(command):
    targetName = command[7:]
    target = player.location.getItemByName(targetName)
    if target != False:
        if(target.weight + player.currweight < player.weightcap):
            player.pickup(target)
            return True
        else:
            print()
            print("Your cargo bay cannot take that much weight.")
            return False
    else:
        print()
        print("No such item.")
        return False

#if you have the item, throw it into the room
def drop(command):
    target = command[5:]
    dropped = False
    for i in player.items:
        itname = i.name
        if target.lower() == itname.lower():
            player.drop(i)
            dropped = True
    if (dropped == False):
        print()
        print("No such item.")
        return False
    else:
        return True

#print all of your relevant stats (except your coordinates)
def me():
    clear()
    print("What next, Captain?\n:me\n")
    print("Health:\t\t\t" + str(player.health) + "/" + str(player.maxhealth))
    if(player.armor > 0):
        print("Shields:\t\t" + str(player.armor))
    print("Speed:\t\t\t" + str(player.speed))
    print("Strength:\t\t" + str(player.stre))
    print("Accuracy:\t\t" + str(player.acc))
    print("Money:\t\t\t" + str(player.money))
    print("\n")
    print("Max Weight:\t\t" + str(player.weightcap))
    print("Current Weight:\t\t" + str(player.currweight))
    print("\n")
    print("Current Level:\t\t" + str(player.lvl))
    print("Current XP:\t\t" + str(player.xp))
    print("XP for Next Level:\t" + str(player.lvlupthresh))
    print("\n")
    input("Press enter to continue...")

#show all items in inventory/equipped
#allow player to inspect them
def inventory():
    notdone = True
    while notdone:
        player.showInventory()  
        command = input("You can inspect items, or tap enter to get going.\n:")
        commandWords = command.split()
        if not commandWords:
            notdone = False                
        elif commandWords[0].lower() == "inspect":
            target = command[8:].lower()
            found = False
            for i in player.items:
                itname = i.name
                if target == itname.lower():
                    found = True
                    target = i
                    print()
            if(not found):
                for i in player.equipped:
                    itname = i.name
                    if target == itname.lower():
                        found = True
                        target = i
                        print()
            if found == True:
                print(target.desc)
                print("It weighs " + str(target.weight))
                print("It is valued at " + str(target.value) + " credits.")
                input("Press enter to continue...")
            else:
                print()
                print("No such item.")
                input("Press enter to continue...")
        else:
            notdone = False

#increase health at end of turn
def regen():
    if(player.health < player.maxhealth):
        player.health += (player.regen + player.location.regen)
    if(player.health > player.maxhealth):
        player.health = player.maxhealth

#check if ready to level up. if so, level up and
#   increase strength, speed, acc, regen by lvl * 1.25
#   increase health by lvl * 2.5
#   increase weight cap by 10
#   increase level xp cap by 5
def lvlup():
    if(player.xp >= player.lvlupthresh):
        player.lvl += 1
        player.xp -= player.lvlupthresh
        lvlupmod = player.lvl * 1.25
        player.stre += lvlupmod
        player.stre = int(player.stre)
        player.speed += lvlupmod
        player.speed = int(player.speed)
        player.acc += lvlupmod
        player.acc = int(player.acc)
        player.maxhealth += (2 * lvlupmod)
        player.maxhealth = int(player.maxhealth)
        player.health = player.maxhealth
        player.regen += lvlupmod
        player.regen = int(player.regen)
        player.weightcap += 10
        player.lvlupthresh = (10 + (5 * (player.lvl - 1)))
        print()
        print("You leveled up! You are now level " + str(player.lvl) + ".")
        input("Press enter to continue...")

#if you don't have an item of the type equipped, and you have the item, equip it
#if it's a healing item, just use it.
def equip(target):
    equipped = False
    target = target.lower()
    for i in player.items:
        itname = i.name
        if target == itname.lower():
            if(i.equippable == True):
                if(i.type == "weapon" and player.wepequipped == True):
                    print("\nYou already have a weapon equipped!")
                    commandSuccess = False
                    equipped = True
                elif(i.type == "armor" and player.armequipped == True):
                    print("\nYou already have an armor equipped!")
                    commandSuccess = False
                    equipped = True
                elif(i.type == "engine" and player.engequipped == True):
                    print("\nYou already have an engine equipped!")
                    commandSuccess = False
                    equipped = True
                else:
                    player.equip(i)
                    if(i.type == "healing"):
                        print("\nYou used " + i.name + " and gained " + str(i.stat) + "hp.")
                        regen()
                    else:
                        print("\nYou equipped " + i.name + ".")
                    equipped = True
                    commandSuccess = True
                    input("Press enter to continue...")
                    break;
            else:
                print()
                print("You cannot equip that item.")
                commandSuccess = False
                equipped = True
    if (equipped == False):
        print()
        print("No such equippable item.")
        commandSuccess = False
    return commandSuccess

#on each turn...
#50% chance of spawning monsters
#20% chance of spawning trader
#check for level up
#regen your health
#let monsters try to fight back
#50% chance to spawn an item
#if Strange Trader doesn't have 5 items yet, give him an item.
def waitstuff():
    updater.updateAll()
    traderspawn = random.random()
    if (traderspawn > .6):
        generateTraders(1)
    monsterspawn = random.random()
    if (monsterspawn > .5):
        leqa = player.lvl // 2
        if(leqa == 0):
            leqa = 1
        leqa *= (worldsize * worldsize)
        leqa = leqa // 2
        generateMonsters(leqa)
    regen()
    while(player.xp >= player.lvlupthresh):
        lvlup()
    z = range(len(player.location.monsters))
    for i in z:
        try:
            if(player.alive):
                player.location.monsters[i].fightback(player)
                z = range(len(player.location.monsters))
                lvlup()
        except IndexError:
            break;
    itemspawn = random.random()
    if (itemspawn > .5):
        randomItemsCreate(1)
        randomItemsPlace(0, 0)
        randomItemsEmpty()
    if(len(StrangeGuy.items) <= 5):
        randomItemsCreate(1)
        StrangeGuy.items.append(randitems[0])
        randomItemsEmpty()

#print intro
#allow player to define worldsize.
#if their input is invalid, just make it 64.
#if it's < 3, just make it 3.
#if it's > 512, just make it 512.
clear()
w = input("How big do you want your world to be?\nI recommend a size of 64. It must be between 3 and 512.\nBear in mind the bigger the world, the slower the game gets.\nYou can also just hit enter to default to 64.\n:")
try:
    worldsize = int(w)
except ValueError:
    worldsize = 64
if(worldsize < 3):
    worldsize = 3
if(worldsize > 512):
    worldsize = 512
clear()
print("You are the captain of a spaceship.")
input("Press enter to continue...")
print("\nIt's pretty awesome.")
input("Press enter to continue...")
print("\nYour hyperdrive has broken down, and you have no way to repair it.")
print("\nYou must upgrade your ship until it can move at 150 lightyears/sec. \nThen you can escape this foreign galaxy and go home.")
print("In other words: you need your speed stat to reach 150.")
print("\nGood luck.")
input("Press enter to continue...")

#create the world
createWorld()

#place strange guy one y above player.
randomItemsCreate(1)
StrangeGuy = Shopkeep("Strange Shopkeep", worldL[worldsize // 2][(worldsize // 2) + 1], 20, [randitems[0]])
randomItemsEmpty()

#play the game
playing = True
while playing and player.alive:
    printSituation()
    commandSuccess = False
    timePasses = False
    while not commandSuccess:
        commandSuccess = True
        command = input("What next, Captain?\n:")
        commandWords = command.split()

        if not commandWords:
            commandSuccess = False

        #go in a direction. can use news instead of northeastwestsouth
        #update player x/y accordingly
        elif commandWords[0].lower() == "go":   #cannot handle multi-word directions
            if(len(commandWords) > 1):
                if(commandWords[1].lower() == "n"):
                    commandWords[1] = "north"
                elif(commandWords[1].lower() == "w"):
                   commandWords[1] = "west"
                elif(commandWords[1].lower() == "e"):
                   commandWords[1] = "east"
                elif(commandWords[1].lower() == "s"):
                    commandWords[1] = "south"
                if player.location.getDestination(commandWords[1].lower()):
                    player.goDirection(commandWords[1].lower())
                    if(commandWords)[1] == "east":
                        player.xloc += 1
                    elif(commandWords)[1] == "west":
                        player.xloc -= 1
                    elif(commandWords)[1] == "south":
                        player.yloc -= 1
                    elif(commandWords)[1] == "north":
                        player.yloc += 1
                    timePasses = True
                else:
                    print("You can't go that direction!\n")
                    commandSuccess = False
            else:
                print()
                print("Go where?")
                commandSuccess = False

        elif commandWords[0].lower() == "pickup":  #can handle multi-word objects
            commandSuccess = pickup(command)

        elif commandWords[0].lower() == "drop":
            commandSuccess = drop(command)

        elif (commandWords[0].lower() == "inventory") or commandWords[0].lower() == "inv":
            inventory()

        elif commandWords[0].lower() == "help":
            showHelp()

        elif commandWords[0].lower() == "exit":
            playing = False

        #attack the monster, then try to level up
        elif (commandWords[0].lower() == "attack") or (commandWords[0].lower() == "atk"):
            if(commandWords[0].lower() == "attack"):
                targetName = command[7:]
            else:
                targetName = command[4:]
            target = player.location.getMonsterByName(targetName)
            if target != False:
                player.attackMonster(target)
                timePasses = False
            else:
                print()
                print("No such monster.")
                commandSuccess = False
            lvlup()

        #trade if tradable
        elif commandWords[0].lower() == "trade":
            targetName = command[6:]
            target = player.location.getMonsterByName(targetName)
            if target != False:
                if isinstance(target, Shopkeep):
                    target.trade(player)
                else:
                    print("\nThey don't understand the economy!\n")
                    commandSuccess = False
            else:
                print()
                print("No such monster.")
                commandSuccess = False

        #wait x number of turns. run through waitstuff on each turn.
        elif commandWords[0].lower() == "wait":
            if(len(commandWords) > 1):
                try:
                    waitTime = int(command[5:])
                    while (waitTime > 0) and (player.alive):
                        printSituation()
                        waitstuff()
                        if(player.alive):
                            print("You are waiting " + str(waitTime - 1) + " more turns.")
                            input("Press enter to continue...")
                        waitTime -= 1
                except ValueError:
                    print()
                    print("The format is \'wait #\'")
                    print()
                    commandSuccess = False
            else:
                print()
                print("The format is \'wait #\'")
                print()
                commandSuccess = False

        elif commandWords[0].lower() == "me":
            me()

        elif (commandWords[0].lower() == "abbreviations") or (commandWords[0].lower() == "abbrevs"):
            showAbbrevs()

        elif commandWords[0].lower() == "equip":
            commandSuccess = equip(command[6:])

            #same as equip
        elif commandWords[0].lower() == "use":
            commandSuccess = equip(command[4:])

            #move equipped item into inventory
        elif commandWords[0].lower() == "dequip":
            target = command[7:]
            target = target.lower()
            dequipped = False
            for i in player.equipped:
                itname = i.name
                if target == itname.lower():
                    print("\nYou dequipped " + i.name + ".")
                    player.dequip(i)
                    dequipped = True
                    input("Press enter to continue...")
            if (dequipped == False):
                print()
                print("No such item.")
                commandSuccess = False

                #print item attributes
        elif commandWords[0].lower() == "inspect":
            targetName = command[8:]
            target = player.location.getItemByName(targetName)
            if target != False:
                print()
                print(target.desc)
                print("It weighs " + str(target.weight))
                print("It is valued at " + str(target.value) + " credits.")
                input("Press enter to continue...")
            else:
                print()
                print("No such item.")
                commandSuccess = False

        elif commandWords[0].lower() == "clear":
            clear()

            #open dialogue if trader is nearby
        elif commandWords[0].lower() == "talk":
            targetName = command[5:]
            target = player.location.getMonsterByName(targetName)
            if target != False:
                if isinstance(target, Shopkeep):
                    target.speak(player)
                else:
                    print("\nThey don't want to talk to you!\n")
                    commandSuccess = False
            else:
                print()
                print("No such person.")
                commandSuccess = False

                #win game if player.speed >= 150
        elif commandWords[0].lower() == "escape":
            if(player.speed < 150):
                print("\nYour engines are not fast enough yet!\n")
                commandSuccess = False
            else:
                clear()
                print("\n\n\n\n")
                print("\tYou fire up your hyperdrive and blast off into the unknown.")
                print("\t\tYou don't know where you're going,")
                print("\t\tbut it has to be better than this.")
                input("\n\tPress enter to continue...")
                print()
                playing = False

        #debug commands for testing errors
        #elif commandWords[0].lower() == "debug":
        #    player.health = player.maxhealth
        #    player.acc += 500
        #    player.stre += 500
        #    player.speed += 500
        #   player.money += 1000
        #    player.weightcap += 500

        #elif commandWords[0].lower() == "xp":
        #    player.xp += int(commandWords[1])
        #    timePasses = True

        #elif commandWords[0].lower() == "hurtself":
        #    player.health -= 20;

        else:
            print()
            print("Not a valid command")
            commandSuccess = False

    #run through waitstuff if time passes
    if timePasses == True:
        waitstuff()