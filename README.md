# Telefonkönyv3D

## A program használata
A WASD gombok segítségével tudunk mozogni, körbenézni az egeret mozgatva, lőni pedig kattintással tudunk. A játék célja minden ellenséges objektum megsemmisítése. Ha a közelünkbe érnek, sebződést kapunk. Amennyiben elfogy az életerőnk, vagy eliminálunk minden ellenséget, a játék bezárul és véget ér.

## Bemenet és formátum
A program futásához két szöveges fájl szükséges: texture.txt és palya.txt. Mindkettőt a játék gyökérmappájába kell helyezni. A fájlok tartalma és formátuma a következő:

* palya.txt
Ez a fájl tartalmazza a pálya méretét, formáját, az entitások és a játékos kezdeti pozícióját. A sorok sorra az alábbiak:

| Formátum                                                   | Leírás                                                                                                                                                      |
|------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------|
| \<x> \<y>                                                  | Játékos kezdeti pozíciója                                                                                                                                   |
| \<x> \<y>                                                  | A pálya mérete                                                                                                                                              |
| \<x0y0> \<x0y1> \<...><br>\<x1y0> \<...><br>\<...> \<xnyn> | A pálya mátrixa<br>A cella értéke határozza meg a hozzá rendelt textúrát.<br>0 jelöli a levegőt<br>A játékbeli pálya ennek az átlójára vett tükörképe lesz. |
| \<n>                                                       | Entitások száma                                                                                                                                             |
| \<id1> \<x0> \<y0><br>\<id2> \<x1> \<y1><br>\<...>         | Entitások azonosítója,<br>ez határozza meg a hozzájuk rendelt textúrát,<br>és kezdeti pozíciójuk                                                            |

* texture.txt
E fájl alapján tölti be a játék a textúrákat. A megadott nevű fájlokat sorrendben rendeli hozzá a program a pálya celláihoz


| Formátum                       | Leírás                                                                                                     |
|--------------------------------|------------------------------------------------------------------------------------------------------------|
| \<path>                        | Az alkalmazásikon relatív elérési útja                                                                     |
| \<abgr_hex_to_dec>             | A padló színe                                                                                              |
| \<abgr_hex_to_dec>             | A plafon színe<br>Mindkét esetben az ABGR formátumú hexadecimális színkódot kell átalakítanunk decimálissá |
| \<n>                           | Megkülönböztetett faltextúrák száma                                                                        |
| \<path1><br>\<path2><br>\<...> | Az adott faltextúrák és relatív elérési útjaik                                                             |
| \<n>                           | Megkülönböztetett entitástextúrák száma                                                                    |
| \<path1><br>\<path2><br>\<...> | Az adott entitástextúrák és relatív elérési útjaik                                                         |

## Optimalizáció
Függőleges szinkronizációt használ, az FPS ezért nem megy a képernyő képfrissítési sebessége fölé. Az SDL korlátai miatt egy processzormagot használ. Amit csak lehet megpróbáltam SDL-es függvények segítségével grafikus kártyára átirányítani (textúrák), ettől függetlenül a program az egyszerűség kedvéért mohó, képkockánként újragenerál mindent. A program sebessége közel sem optimális, a használt grafikus könyvtárból fakadóan, hisz az nem erre lett kitalálva.

## Felhasználás
Prog2 nagyházinak nem ajátnlott beadni, én már megtettem. :3

## Képek
![:3](https://raw.githubusercontent.com/Gilgames32/cpp3d/master/ass/screenshot.png)