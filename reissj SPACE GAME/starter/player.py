#for explanation of new variables/attributes,
#look at the section about combat.
import os
import random
from item import Item
from collections import defaultdict #had some weird showinventory() issues. this fixed them.

def clear():
    os.system('cls' if os.name == 'nt' else 'clear')

class Player:
    def __init__(self):
        #give player a starting weapon/engine and set their default stats
        basicphotonlazer = Item("Photon Lazer", "Prodides accuracy +5, damage +5", 5, 10, "weapon", True, 2, 2)  
        basicimpulsedrive = Item("Impulse Drive", "Provides speed +5", 5, 10, "engine", True, 5)
        self.location = None
        self.items = []
        self.equipped = [basicphotonlazer, basicimpulsedrive]
        self.health = 15
        self.maxhealth = self.health
        self.armor = 0
        self.weightcap = 50
        self.currweight = 0
        self.alive = True
        self.speed = 5
        self.acc = 5
        self.stre = 5
        self.money = 0
        self.regen = 1
        self.xp = 0
        self.lvl = 1
        self.lvlupthresh = (10 + (5 * (self.lvl - 1)))
        self.wepequipped = True
        self.armequipped = False
        self.engequipped = True
        self.xloc = 0
        self.yloc = 0

    #moves player.location
    def goDirection(self, direction):
        self.location = self.location.getDestination(direction)

    #adds item to player.items
    def pickup(self, item):
        self.items.append(item)
        item.loc = self
        self.location.removeItem(item)
        self.currweight += item.weight

    #removes item from player.items, adds to room
    def drop(self, item):
        item.putInRoom(self.location)
        self.items.remove(item)
        self.currweight -= item.weight

    #removes item from player.items, adds to player.equipped, updates stats
    def equip(self, item):
        self.items.remove(item)
        if(item.type != "healing"):
            self.equipped.append(item)
        self.currweight -= item.weight
        if(item.type == "armor"):
            self.armor += item.stat
            self.armequipped = True
        if(item.type == "weapon"):
            self.stre += item.stat
            self.acc += item.stat2
            self.wepequipped = True
        if(item.type == "engine"):
            self.speed += item.stat
            self.engequipped = True
        if(item.type == "healing"):
            self.health += (item.stat - 1)

    #opposite of equip
    def dequip(self, item):
        self.equipped.remove(item)
        self.items.append(item)
        self.currweight += item.weight
        if(item.type == "armor"):
            self.armor -= item.stat
            self.armequipped = False
        if(item.type == "weapon"):
            self.stre -= item.stat
            self.acc -= item.stat2
            self.wepequipped = False
        if(item.type == "engine"):
            self.speed -= item.stat
            self.engequipped = False

#prints item name, quantity, weight, and value for all in items
#prints item name and stat boost for all in equipped
#this function is a little weird.
#   first, all of the items in self.items are added to a dictionary.
#       the dictionary assigns each key three values: quantity, weight, and value.
#       this accomplishes the stacking items goal by calculating quantity.
#   then, print out all of the values in itdict.
#   second, print out all of the names and stat values for self.equipped
    def showInventory(self):
        itdict = defaultdict(list)
        clear()
        print("You are currently carrying:")
        print()
        for i in self.items:
            if(str(i.name) in itdict):
                itdict[str(i.name)][0] += 1
            else:
                itdict[str(i.name)].append(1)
                itdict[str(i.name)].append(i.weight)
                itdict[str(i.name)].append(i.value)
        i = 0
        for k, v in itdict.items():
            inds = "\t\t"
            if(len(str(k)) > 15):
                inds = "\t"
            print(str(k) + inds + "x" + str(v[0]) + "\tWeight: " + str(v[1]) +"\tValue: " + str(v[2]))
            i += 1
        print()
        print("You have equipped:")
        print()
        for i in self.equipped:
            inds = "\t\t"
            if(len(str(i.name)) > 15):
                inds = "\t"
            print((i.name) + inds + "Stat Boost: " + str(i.stat))
        print()

#combat works like so:
#   whoever has more speed goes first
#   the attacker rolls for accuracy.
#       this roll is (0-10) + their acc stat
#   the defender rolls for dodge.
#       this roll is (0-10) + their speed stat
#   if the attack hits, roll for damage
#       this roll is (0-10) + their stre stat - enemy's armor stat
#   if the player dies, they lose the game.
#   if the monster dies, the player gains xp and money
#       they get (monster xp value // 2) money.
#this is more or less how combat works in Dungeons and Dragons.

    #initiate combat
    def attackMonster(self, mon):
        clear()
        print("You are attacking a " + mon.name + ".")
        print()
        print("Your health is " + str(self.health) + ".")
        print(mon.name + "'s health is " + str(mon.health) + ".")
        print()
        if self.speed > mon.speed:
            self.attacking(mon)
            if(mon.health > 1):
                self.attacked(mon)
            else:
                self.xp += mon.xp
                self.money += (mon.xp // 2)
                mon.die()
        else:
            self.attacked(mon)
            if(self.health > 1):
                self.attacking(mon)
                if(mon.health < 1):
                    self.xp += mon.xp
                    self.money += (mon.xp // 2)
                    mon.die()
        print()
        input("Press enter to continue...")

    #if (randint 0-10) + your acc > (randint 0-10) + monster speed
    #then do (randint 0-10) + your strength damage
    def attacking(self, mon):
        playeracc = random.randint(0, 10) + self.acc
        monsterdodge = random.randint(0, 10) + mon.speed
        #print(playeracc)
        #print(monsterdodge)
        if(playeracc > monsterdodge):
            playerstre = random.randint(0, 10) + self.stre
            if(playerstre < 0):
                playerstre = 0
            mon.health -= playerstre
            print("You did " + str(playerstre) + " damage.")
        else:
            print("Your attack missed!\n")
        if(mon.health < 1):
            print("\nYou killed the " + mon.name + ".\nYou gained " + str(mon.xp) + "xp and " + str(mon.xp * 2) + " credits.\nYour health is now " + str(self.health) + ".")

        else:
            print("The " + mon.name + "\'s health is now " + str(mon.health) + ".")

    #do same rolls for accuracy
    #take same damage roll, then subtract your armor from enemy damage
    def attacked(self, mon):
        monsteracc = random.randint(0, 10) + mon.acc
        playerdodge = random.randint(0, 10) + self.speed
        if(monsteracc > playerdodge):
            monsterstre = ((random.randint(0, 10) + mon.stre) - self.armor)
            if (monsterstre < 0):
                monsterstre = 0
            self.health -= monsterstre
            print("\nYou took " + str(monsterstre) + " damage.")
        else:
            print("\nEnemy attack missed!\n")
        if(self.health < 1):
            print("\nYour ship exploded.")
            print("GAME OVER. YOU LOSE.")
            self.alive = False
        else:
            print("Your health is now " + str(self.health) + ".\n")
