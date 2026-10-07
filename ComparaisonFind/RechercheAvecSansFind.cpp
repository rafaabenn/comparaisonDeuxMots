#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Compare deux façons de compter les mots qui contiennent "cherche" (lettres collées) :
//   1. avec string::find
//   2. sans find : deux boucles écrites à la main
//
// Complexité (n = nombre de mots, L = longueur d'un mot, m = longueur de "cherche") :
//   les deux sont en O(n * L * m) dans le pire cas, O(n * L) en pratique.
//   La différence de temps vient seulement des constantes :
//   - find saute directement aux positions de la première lettre (memchr),
//     mais coûte un appel de fonction par mot ;
//   - la boucle à la main teste chaque position, mais s'arrête tout de suite
//     si le mot cherché est au début.

// Lit tous les mots du fichier
// Renvoie false si le fichier n'existe pas
bool chargerFichier(const string& nomFichier, vector<string>& mots) {
    ifstream fichier(nomFichier);
    if (!fichier) return false;

    string mot;
    while (fichier >> mot) mots.push_back(mot);
    return true;
}

// Recherche 1 : avec find
int compterAvecFind(const vector<string>& mots, const string& cherche) {
    int compteur = 0;
    for (size_t i = 0; i < mots.size(); i++) {
        if (mots[i].find(cherche) != string::npos) compteur++;   // npos = pas trouvé
    }
    return compteur;
}

// Vrai si "cherche" apparaît dans "mot", lettres collées
// On essaie chaque position de départ possible dans "mot"
bool contientColle(const string& mot, const string& cherche) {
    if (cherche.size() > mot.size()) return false;

    for (size_t debut = 0; debut + cherche.size() <= mot.size(); debut++) {
        size_t j = 0;
        while (j < cherche.size() && mot[debut + j] == cherche[j]) j++;   // compare lettre par lettre
        if (j == cherche.size()) return true;                           // toutes les lettres égales
    }
    return false;
}

// Recherche 2 : sans find
int compterSansFind(const vector<string>& mots, const string& cherche) {
    int compteur = 0;
    for (size_t i = 0; i < mots.size(); i++) {
        if (contientColle(mots[i], cherche)) compteur++;
    }
    return compteur;
}

// Type "pointeur vers une des 2 recherches"
typedef int (*Recherche)(const vector<string>&, const string&);

const int REPETITIONS = 5;

// Lance la recherche plusieurs fois, renvoie le temps moyen (en ms)
double mesurer(Recherche f, const vector<string>& mots, const string& cherche, int& resultat) {
    double total = 0;
    for (int r = 0; r < REPETITIONS; r++) {
        auto debut = chrono::steady_clock::now();
        resultat = f(mots, cherche);
        auto fin = chrono::steady_clock::now();
        total += chrono::duration<double, milli>(fin - debut).count();
    }
    return total / REPETITIONS;
}

int main() {
    string nomFichier, cherche;

    cout << "Nom du fichier (ex: texte_10M.txt) : ";
    getline(cin, nomFichier);
    cout << "Mot a rechercher : ";
    cin >> cherche;

    // 1. Lire le fichier
    vector<string> mots;
    auto debutChargement = chrono::steady_clock::now();
    if (!chargerFichier(nomFichier, mots)) {
        cerr << "Erreur : impossible d'ouvrir \"" << nomFichier << "\"." << endl;
        return 1;
    }
    auto finChargement = chrono::steady_clock::now();
    cout << endl << mots.size() << " mots charges en "
         << chrono::duration<double, milli>(finChargement - debutChargement).count() << " ms" << endl << endl;

    // 2. Lancer et chronométrer les 2 recherches
    int nbAvec = 0, nbSans = 0;
    double tempsAvec = mesurer(compterAvecFind, mots, cherche, nbAvec);
    double tempsSans = mesurer(compterSansFind, mots, cherche, nbSans);

    // 3. Afficher les résultats
    cout << "Mots qui contiennent \"" << cherche << "\" (moyenne sur " << REPETITIONS << " essais) :" << endl;
    cout << "  1. Avec find : " << nbAvec << " mots   (" << tempsAvec << " ms)" << endl;
    cout << "  2. Sans find : " << nbSans << " mots   (" << tempsSans << " ms)" << endl;
    if (nbAvec != nbSans) cout << "  Attention : les deux methodes ne donnent pas le meme resultat !" << endl;
    cout << "  Rapport sans / avec : " << tempsSans / tempsAvec << endl;

    return 0;
}
