import numpy as np

def packetprep(packetstring):
    """
    Takes a `string` and returns a list containing all values as well as the brackets.
    """
    packetstring = packetstring.replace("[",",[,")
    packetstring = packetstring.replace("]",",],")
    packetstring = packetstring.replace(",,",",")
    packetstring = packetstring.replace(",,",",")
    packet = packetstring.split(",")[1:-1]

    for i in range(len(packet)):
        if packet[i] not in ("[","]"):
            packet[i] = int(packet[i])

    return packet

def packetcomp(iL,iR,left_packet,right_packet):
    """
    Recursively compare two packets. `iL` and `iR` mark the indices we are currently comparing. We check all scenarios consecutively:
    * Are there `[`s at both indices, i.e. did we find two lists? Then increment `iL` and `iR` and continue.
    * Is there a `[` at only one of the two indices? Then turn the value at the other index into a list (by
    inserting `[` and `]` before and after, respectively) and continue.
    * Is one list ending before the other? Then return `True` or `False` depending on which list ended first.
    * Are both lists ending? Then increment `iL` and `iR` and continue.
    
    If all of the above doesn't apply, we're simply comparing two numbers.

    Note that currently, the behaviour is undefined when no decision can be made - this case was not mentioned
    in the problem description.
    """

    if left_packet[iL] == "[" and right_packet[iR] == "[":
        # a list in both packets
        return packetcomp(iL+1,iR+1,left_packet,right_packet)
    
    if left_packet[iL] != "]" and right_packet[iR] == "]":
        # the right list ended before the left list - they are not in order
        return False
    
    if left_packet[iL] == "]" and right_packet[iR] != "]":
        # the left list ended before the left list - they are in order
        return True
    
    if left_packet[iL] == "[" and right_packet[iR] != "[":
        # a list in the left packet - we need to turn the element in the right packet into a list
        right_packet = right_packet[:iR] + ["[",] + [right_packet[iR],] + ["]",] + right_packet[iR+1:]
        return packetcomp(iL,iR,left_packet,right_packet)
    
    if left_packet[iL] != "[" and right_packet[iR] == "[":
        # a list in the right packet - we need to turn the element in the left packet into a list
        left_packet = left_packet[:iL] + ["[",] + [left_packet[iL],] + ["]",] + left_packet[iL+1:]
        return packetcomp(iL,iR,left_packet,right_packet)
    
    if left_packet[iL] == "]" and right_packet[iR] == "]":
        # a list in both packets ends
        return packetcomp(iL+1,iR+1,left_packet,right_packet)

    # if we got to this point, both entries are numbers which we can compare normally

    if left_packet[iL] == right_packet[iR]:
        # the numbers are equal, we cannot make a decision here
        return packetcomp(iL+1,iR+1,left_packet,right_packet)
    
    # the numbers are uneqal - now we're just comparing
    if left_packet[iL] < right_packet[iR]:
        return True
    else:
        return False

# opening the file
file = open("day13_packets.txt","r")

line = "dummy"
pairindex = 0
result = 0

while(line != ""):
    # reading in the packet strings
    left_packet  = packetprep(file.readline())
    right_packet = packetprep(file.readline())
    line = file.readline()
    pairindex += 1

    # comparing the packets:
    if packetcomp(0,0,left_packet,right_packet):
        # print("Pair {}: Right order".format(pairindex))
        result += pairindex
    # else:
    #     print("Pair {}: Failed".format(pairindex))

print("The result is {}".format(result))

file.close()