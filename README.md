# AlgosRechercheCpp

Algorithmes de recherche dans des chaînes de caractères, en C++.

## ComparaisonMots

`DeuxMots.cpp` : vérifie si les lettres de `mot2` se trouvent dans `mot1`, dans le même ordre
(exemple : `bonjour` contient `bjr`). Deux versions sont comparées, avec leur temps de réponse :
deux indices et `string::find`.

```
g++ ComparaisonMots/DeuxMots.cpp -o DeuxMots
```

## RechercheFichier

`RechercheDansFichier.cpp` : charge les mots d'un fichier texte et y cherche un mot de quatre façons,
en mesurant le temps de chacune :

1. mot exact, recherche linéaire
2. mot exact, recherche dichotomique
3. mots qui contiennent le mot collé (`find`)
4. mots qui contiennent ses lettres dans l'ordre

`texte_test.txt` sert de fichier d'essai.

```
g++ RechercheFichier/RechercheDansFichier.cpp -o recherche
```
