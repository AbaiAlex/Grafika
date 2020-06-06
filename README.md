# Grafika

---

# HF_1

Az első házi feladatban egy **klasszikus analóg órá**t kellet implementálni. Az óra számlapja és a mutatók egyaránt körök segítségével kerülnek megjelenítésre. Továbbá, az óra számlapját alkotó minden egyes kör annak megfelelően színeződik, hogy az a másodpercmutatóra illeszkedő egyenes melyik oldalára esik. (bugos)

---

# HF_2

A második házi feladat egy olyan program, amelyben két egymáshoz kapcsolt, egérrel valós időben mozgatható görbe kerül megvalósításra. A görbék kontrollpontjait egérkattintások segítségével lehet lerakni a képernyőre. Az egyik görbe egy **Bézier-görbe, ami C1 folytonosan csatlakozik a másik, 4 pontjával megadott Hermite-ívhez**.

### Használata:

Bal egér kattintással a pontokat lehet lerakni, jobb egér kattintással ki lehet rajzolni a görbéket, amennyiben levan rakva 8 pont.

---

# HF_3

A harmadik házi feladat egy **megfelelően árnyalt gömb**öt rajzol ki, amit különböző billentyű interakciók hatására forgatunk, illetve mozgatunk. Kétféle árnyalást használunk, amelyeket attól függően alkalmazunk a gömb különböző lapjaira, hogy az adott lap egy, a képernyő közepén húzódó függőleges szakasz melyik oldalára esik. Ehhez szükség van egyrészt a gömb paraméteres felületként való meghatározására, másrészt láthatósági vizsgálatokra és árnyalási technikákra.

### Használata:

'w' a gömb fölfele forog ,'s' a gömb lefele forog,'e' a gömb jobbra forog, 'q' a gömb balra forog, 'a' a gömb balra mozdul, 'd' a gömb jobbra mozdul, 'r' a gömb hátra mozdul, 'f' a gömb előre mozdul, '-' a gömb felosztása csökken, '+' a gömb felosztása nő, 'c' a képernyő jobb oldalán megjelennek a gömböt alkotó lapok középpontjai, 'v' a középpontok eltűnnek, 'y' megjelenik a képernyő bal oldalán a gömböt alkotó háló, 'x' a gömb háló eltűnik.
