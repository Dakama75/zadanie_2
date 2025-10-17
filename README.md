Zadanie 1:
Wykorzystałem Algorytm Grahama

Najpierw szukam punktu startowego, którym jest ten z najmniejszym y i jęsli jest kilka takich punktów to wybiera ten z najmnijeszym x
Następnie sortuję po kącie polarnym, czyli kącie miedzy prostą poprowadzoną z punktu starowego do danego punktu, a osią Ox mierzony od punktu startowego
Następnie dodaje do stasu punkty dla których (bx - ax) * (cy - ay) - (by-ay)*(cx-ax) jest większy lub równy zero, czyli tylko te punkty, które tworzą otoczkę 

Przy tym zadaniu na chwilę utknołem, bo miałem problem z porównywaniem wartości double, i dopiero jak dowiedziałem się o stałej ESP, która niweluje ten problem, to zaczeły wychodzi prawidłowe wyniki

Zadanie 2:
Wykorzystuję punkty tworzące otoczkę z zadania 1
oblicza wszystkie proste obejmujące pozostałe punkty i zachowuje tylką tą najmnijeszą 

Zadanie 3:
Oblicza najmnijeszą odległosć miedzy punktami
Wykorzystuje algorytm dziel i rządź, który najpierw sortuje zbiór po wartości x, następnie dzieli zbiór na pól i szuka najbliższych punktów w obu połowach oraz na lini ich podziału
dzięki temu nie trzeba sprawdzać każdego punktu z kązdym, sortowanie jest najbardziej wymagające czasowo 
