#
# Driver's Championship Permutations
# Calculates all championship permutations of the top 3 in the championship
# given 3 RACES_REMAINING
#
# To reduce computation time I make the assumption that all 3 drivers
# finish within the top 10, and I don't include fastest lap since it
# changes the results by maybe a couple hundredths of a percent but
# increases the computing time by a multiplicative
#

from math import factorial
import itertools
import time
from multiprocessing import Pool

RACES_REMAINING = 3

# each set is the standings of the top 3 in each division
DRIVER_STANDINGS = [
                    [134, 120, 79], [93, 82, 70], [147, 69, 62], [103, 90, 77], [94, 90, 83],
                    [98, 93, 82], [147, 144, 101], [123, 81, 80], [129, 100, 88], [124, 95, 90]
                   ]

# calculate number of permutations
def nPr(n, r):
    return int(factorial(n)/factorial(n-r))

#for div in range(len(DRIVER_STANDINGS)):
def permutations(DRIVER_STANDING):
    DRIVERX_POINTS = DRIVER_STANDING
    NUM_DRIVERS = len(DRIVERX_POINTS)
    
    SCORE_SYSTEM = [25, 18, 15, 12, 10, 8, 6, 4, 2, 1, 0]
    #NUM_PLACES = len(SCORE_SYSTEM)
    
    SCORE_SYSTEMS = []
    for x in range(NUM_DRIVERS):
        SCORE_SYSTEMS.append(SCORE_SYSTEM)
    
    #FASTEST_LAP = [1, 0]
    
    #NUM_OF_ONE_RACE_PERMS = nPr(NUM_PLACES, NUM_DRIVERS)
    
    # permutation of one race
    one_race_perm = []
    for i in itertools.product(*(SCORE_SYSTEMS)):
        if len(set(i)) == 3:
            one_race_perm.append(i)
    #print(one_race_perm)
    
    
    total = 0
    driver_wins = [0 for k in range(NUM_DRIVERS)]
    draws = 0
    
    drivers_points = []
    
    #itr = 0
    #progress = 0
    
    start = time.time()
    
    # permutations for one race
    for elt1 in one_race_perm:
        #itr += 1
        #progress = itr / NUM_OF_ONE_RACE_PERMS * 100
        #print(str(round(progress,2)) + '% complete')
        
        # permutations for two races
        for elt2 in one_race_perm:
            # permutations for three races
            for elt3 in one_race_perm:
                
                # have to account for a max of 20 later?
                # fastest lap too computationally expensive for results
                for fl in range(11):
                    
                    drivers_points = []
                    
                    for driver in range(NUM_DRIVERS):
                        points = DRIVERX_POINTS[driver] + elt1[driver] + elt2[driver] + elt3[driver]
                        
                        if fl != 11 and fl == driver:
                            points += 1
                        
                        drivers_points.append(points)
                        
                        
                    maxPoints = max(drivers_points)
                    maxCount = 0
                    for x in drivers_points:
                        if x == maxPoints:
                            maxCount += 1
                            
                    if maxCount == 1:
                        driver_wins[drivers_points.index(maxPoints)] += 1
                    else:
                        draws += 1
                        
                    total += 1
                
    end = time.time()
    
    print(str(DRIVERX_POINTS) + ' duration: ' + str(end - start))
    for x in driver_wins:
        print(str(x) + '/' + str(total) + ' = ' + str(round(x/total,4)))
        
    print(str(draws) + '/' + str(total) + ' = ' + str(round(draws/total,4)))
    print(total)

if __name__ == '__main__':
    p = Pool(10)
    p.map(permutations, DRIVER_STANDINGS)
    p.close()
    p.join()