# Přiřazení pořadí preorder vrcholům

Pomocí knihovny Open MPI implementujte v jazyce C++ úlohu Přiřazení pořadí preorder vrcholům tak, jak byla uvedena na přednášce PRL.

## Deadline

1\. 6. 2022

## Vstup a výstup

Vstupem je řetěz, který reprezentuje hodnoty uzlu binárního stromu zapsaného v poli. Indexujme znaky v řetězci od 1 a uvažujme uzel, která má svou hodnotu na indexu i. Pak levý potomek tohoto uzlu má hodnotu na indexu 2*i a hodnota jeho pravého potomka se bude nacházet na indexu (2*i)+1.

Na výstupu bude řetězec se stejnými znaky jako na vstupu, ale v preorder pořadí (tj. jakoby každý uzel stromu při preorder průchodu vytiskl na výstup svůj znak).

## Příklad vstupu

ABCDEFG

## Příklad výstupu

ABDECFG

## Postup

Vytvořte testovací skript test, který přeloží a spustí program. Tento skript bude mít tyto vlastnosti:

Bude pojmenován test nebo test.sh.
Bude přijímat právě 1 parametr a to řetězec reprezentující hodnoty uzlu binárního stromu.

## Dokumentace

Obdobné jako v projektu 2.
Hodnotí se i vzhled a jazyková úroveň.
Obvyklý rozsah 3 strany (nepište zadání, úvodní stranu, obsah, pseudokódy).
Krom popisu samotného Přiřazení pořadí preorder vrcholům nezapomeňte také popsat Eulerovu cestu a spočtení sumy suffixů.

## Implementace

C++ (přeložitelné pomocí mpic++ kvůli jednotnému skriptu test.sh, ne nutně objektově)
Open MPI
nic jiného
při implementaci sumy suffixů využívejte jen funkce MPI_Send a MPI_Recv, tedy využívání funkcí, které by vám implementaci příliš zjednodušily (jako MPI_Scan nebo MPI_Reduce) je zakázané (jiné kolektivní operace použít můžete)

## Doplňkové informace

Výstupy programu budou strojově kontrolovány, mějte to prosím na paměti při implementaci.
Budou testovány především korektní vstupy.
Při implementaci se zaměřte zejména na důkladné pochopení toho, jak algoritmus funguje a co jednotlivé procesy dělají.

## Odevzdání

Do WISu se odevzdává jeden archiv xlogin00.{tar|tgz|zip}, který nepřesáhne 1MB a obsahuje:

zdrojový kód: pro.cpp
hlavička: pro.h (pokud ji využijete)
testovací shellový skript- {test|test.sh}
dokumentaci: xlogin00.pdf
nic jiného (binárky, obrázky, testovací soubory,...)

## Hodnocení

Hodnocení konzultujte emailem se svým opravujícím. V případě nutnosti je možné domluvit osobní konzultaci.
