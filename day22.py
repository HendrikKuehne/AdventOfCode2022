import numpy as np

def print_field(field):
    for i in range(len(field)):
        for j in range(len(field[i])):
            print(field[i][j],end="")
        print()

def read_field(filename):
    """
    Opens the file and reads the contents to create the field. The field is `" "`-padded to a rectangular shape
    with a one-wide frame of `" "`s for easier access later. The return value `field` is a list containing lists
    of strings.
    """
    file = open(filename,"r")

    # reading in the field
    field = [[" "]]
    line = "dummy"
    while line != "\n":
        line = file.readline()
        field += [[" "],]

        for spot in line:
            if spot != "\n":
                field[-1] += [spot,]
        field[-1] += [" ",]
    field += [[" "],]

    # padding the field in " " for easier processing later on
    max_len = 0
    for row in field:
        if len(row) > max_len: max_len = len(row)
    
    for i in range(len(field)):
        while len(field[i]) < max_len:
            field[i] += [" ",]
    
    file.close()

    return field

def read_instructions(filename):
    """
    Opens the file and extracts the moves to be made. The return value `instructions` is a list containing strings.
    """
    file = open(filename,"r")
    
    # reading in the file
    line = "dummy"
    while line != "\n": line = file.readline()
    
    # line containing the instructions
    line = file.readline()
    instructions = []

    i = 0
    while i < len(line):
        if line[i].isalpha():
            line = line[:i] + "," + line[i] + "," + line[i+1:]
            i += 1
        i += 1

    instructions = line.split(",")

    for i in range(len(instructions)):
        if instructions[i] not in ("L","R"):
            instructions[i] = int(instructions[i])

    file.close()

    return instructions

def check(pos,direction,field):
    """
    takes the location `int x` and `int y` on the field `field` and checks if it is possible to move in direction `direction`.
    `direction` must be one of `>`, `^`, `<` or `v`.
    """
    x = pos[1]
    y = pos[0]

    assert direction in (">","^","<","v")
    assert y in range(len(field))
    assert x in range(len(field[0]))

    # number of columns and rows, respectively
    n_y = len(field)
    n_x = len(field[0])

    if direction == ">":
        # moving to the next plot on the field
        while field[y][(x+1) % n_x] == " ": x = (x+1) % n_x
        
        if field[y][(x+1) % n_x] != "#":
            return True
        else:
            return False
        
    if direction == "^":
        # moving to the next plot on the field
        while field[(y - 1 + n_y) % n_y][x] == " ": y = (y - 1 + n_y) % n_y

        if field[(y - 1 + n_y) % n_y][x] != "#":
            return True
        else:
            return False
    
    if direction == "<":
        # moving to the next plot on the field
        while field[y][(x - 1 + n_x) % n_x] == " ": x = (x - 1 + n_x) % n_x

        if field[y][(x - 1 + n_x) % n_x] != "#":
            return True
        else:
            return False
    
    if direction == "v":
        # moving to the next plot on the field
        while field[(y+1) % n_y][x] == " ": y = (y+1) % n_y

        if field[(y+1) % n_y][x] != "#":
            return True
        else:
            return False
    
    return False

def turn(direction="",turn=""):
    """
    Returns the new direction when we turn in the direction `turn`. `direction` must be one of `>`, `^`, `<` or `v`. `turn` must be `L` or `R`.
    """
    assert direction in (">","^","<","v")
    assert turn in ("L","R")

    if direction == ">":
        if turn == "L": return "^"
        if turn == "R": return "v"
    if direction == "^":
        if turn == "L": return "<"
        if turn == "R": return ">"
    if direction == "<":
        if turn == "L": return "v"
        if turn == "R": return "^"
    if direction == "v":
        if turn == "L": return ">"
        if turn == "R": return "<"

def move(pos,direction,field):
    """
    takes the location `int x` and `int y` on the field `field` and moves in direction `direction`.
    `direction` must be one of `>`, `^`, `<` or `v`.
    """
    x = pos[1]
    y = pos[0]

    assert direction in (">","^","<","v")
    assert y in range(len(field))
    assert x in range(len(field[0]))

    # number of columns and rows, respectively
    n_y = len(field)
    n_x = len(field[0])

    if direction == ">":
        # moving to the next plot on the field
        while field[y][(x+1) % n_x] == " ": x = (x+1) % n_x
        
        return [y,(x+1) % n_x]
        
    if direction == "^":
        # moving to the next plot on the field
        while field[(y - 1 + n_y) % n_y][x] == " ": y = (y - 1 + n_y) % n_y

        return [(y - 1 + n_y) % n_y,x]
    
    if direction == "<":
        # moving to the next plot on the field
        while field[y][(x - 1 + n_x) % n_x] == " ": x = (x - 1 + n_x) % n_x

        return [y,(x - 1 + n_x) % n_x]
    
    if direction == "v":
        # moving to the next plot on the field
        while field[(y+1) % n_y][x] == " ": y = (y+1) % n_y

        return [(y+1) % n_y,x]

def password(pos,direction):
    """
    Calculates the password based upon position and direction based on the problem description.
    """
    x = pos[1]
    y = pos[0]

    assert direction in (">","^","<","v")
    assert y in range(len(field))
    assert x in range(len(field[0]))

    if direction == ">": return 1000 * y + 4 * x
    if direction == "^": return 1000 * y + 4 * x + 3
    if direction == "<": return 1000 * y + 4 * x + 2
    if direction == "v": return 1000 * y + 4 * x + 1

filename = "day22_field.txt"
field = read_field(filename)
instructions = read_instructions(filename)

# finding the initial position and direction
pos = [1,0]
while field[pos[0]][pos[1]] == " ": pos[1] += 1
direction = ">"

# making our moves
for iMove in range(len(instructions)):
    if instructions[iMove] in ("L","R"):
        # the current instruction is a turn
        direction = turn(direction,instructions[iMove])
    else:
        # the current instruction is a move
        nSteps = instructions[iMove]
        field[pos[0]][pos[1]] = direction

        while nSteps > 0 and check(pos,direction,field):
            field[pos[0]][pos[1]] = direction

            # moving
            pos = move(pos,direction,field)

            nSteps -= 1

print_field(field)

print("The password is {}".format(password(pos,direction)))