# F1-Championship-Calculator
Calculates the number of F1 championship permutations won by each driver given
the score system (number of available places) and driver's points (number of
drivers).

Small efficiency adjustments matter a lot because the number of total permutations
is ((n!/(n-r)!)*n)^c where n is the number of positions, r is the number of drivers,
and c is the number of races.

E.g., 20 positions with 3 championship contenders and 2 races left is
((20!/(20-3)!) * 20) ^ 2 = 18,714,240,000 permutations
