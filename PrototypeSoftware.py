
## ----------------------
## Purpose: this program is a prototype for the zone selection algorithum implimented on the Arduino (see in the /SheetPressControl directory)
## Author: Connor Povoledo
## ----------------------

## INITIAL CONDITIONS:

# define which zones are requesting power
requesting_power = [0,0,1,1,0,0]

# define which zones are recieving power
recieving_power = [1,0,0,0,0,0]
active_zones = [0,0]

count = 0
for i,e in enumerate(recieving_power):
    if e != 0:
        if count == 0:
            active_zones[0] = i
            active_zones[1] = i
            count += 1
        else: 
            active_zones[1] = i

active_zones.sort()
print(active_zones)

## FIND NEXT ZONES:
power_needed_1 = False
power_needed_2 = False

# reset power source 1
for i in range(1,7,1):

    new_zone_index_1 = (active_zones[0] + i)%6

    if requesting_power[new_zone_index_1] == 0: continue
    if new_zone_index_1 == active_zones[1]: continue

    power_needed_1 = True
    break

# reset power source 2
for i in range(1,7,1):

    new_zone_index_2 = (active_zones[1] + i)%6

    if requesting_power[new_zone_index_2] == 0: continue
    if power_needed_1 == True: 
        if new_zone_index_2 == new_zone_index_1: continue

    power_needed_2 = True
    break

## WRITE TO ZONES:
output_vector = [0,0,0,0,0,0]
print(power_needed_1)
print(power_needed_2)

if power_needed_1 and power_needed_2:
    output_vector[new_zone_index_1] = 1
    output_vector[new_zone_index_2] = 1

elif power_needed_1:
    output_vector[new_zone_index_1] = 1

elif power_needed_2:
    output_vector[new_zone_index_2] = 1

print(output_vector)