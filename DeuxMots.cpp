// DeuxMots.cpp — mot1 contient-il les lettres de mot2, dans le même ordre,
// sans qu'elles soient forcément collées ?
// Deux versions : avec deux indices, et avec string::find.
// Compilation : g++ -O2 -o DeuxMots.exe DeuxMots.cpp
#include <iostream>
#include <string>
using namespace std;

// ---------------------------------------------------------------
// Version 1 : classe string et deux indices
//   i parcourt mot1 (avance à chaque tour)
//   j désigne la lettre de mot2 qu'on attend (avance quand on la trouve)
// ---------------------------------------------------------------
bool contientDeuxIndices(const string& mot1, const string& mot2) {
    size_t i = 0;
    size_t j = 0;
    while (i < mot1.size() && j < mot2.size()) {
        if (mot1[i] == mot2[j]) j++;      // lettre trouvée : on attend la suivante
        i++;                              // on avance toujours dans mot1
    }
    return j == mot2.size();              // a-t-on trouvé toutes les lettres de mot2 ?
}

// ---------------------------------------------------------------
// Version 2 : avec string::find
//   Pour chaque lettre de mot2, on la cherche dans mot1 à partir de
//   la position qui suit la lettre précédente.
// ---------------------------------------------------------------
bool contientFind(const string& mot1, const string& mot2) {
    size_t pos = 0;                                 // où commencer à chercher dans mot1
    for (size_t j = 0; j < mot2.size(); j++) {
        pos = mot1.find(mot2[j], pos);              // cherche la lettre à partir de pos
        if (pos == string::npos) return false;      // introuvable : c'est non
        pos++;                                      // la suivante doit être après celle-ci
    }
    return true;                                    // toutes les lettres ont été trouvées
}

// Affiche le résultat des deux versions pour un couple de mots
void tester(const string& mot1, const string& mot2) {
    bool r1 = contientDeuxIndices(mot1, mot2);
    bool r2 = contientFind(mot1, mot2);
    cout << "\"" << mot1 << "\" contient \"" << mot2 << "\" ?   "
         << "deux indices : " << (r1 ? "oui" : "non")
         << "   find : " << (r2 ? "oui" : "non");
    if (r1 != r2) cout << "   <-- RESULTATS DIFFERENTS";
    cout << endl;
}

int main() {
    
    string mot1, mot2;
    cout << "Entrez mot1 : ";
    cin >> mot1;
    cout << "Entrez mot2 : ";
    cin >> mot2;
    tester(mot1, mot2);

    return 0;
}