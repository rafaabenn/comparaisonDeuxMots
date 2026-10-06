#include <algorithm>
#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Enlève la ponctuation autour du mot et le met en minuscules
// Exemple : "Bonjour," -> "bonjour"
string nettoyer(const string& mot) {
    size_t debut = 0;
    size_t fin = mot.size();

    while (debut < fin && ispunct((unsigned char)mot[debut])) debut++;   // ponctuation au début
    while (fin > debut && ispunct((unsigned char)mot[fin - 1])) fin--;   // ponctuation à la fin

    string resultat = mot.substr(debut, fin - debut);
    for (size_t i = 0; i < resultat.size(); i++)
        resultat[i] = (char)tolower((unsigned char)resultat[i]);         // en minuscules
    return resultat;
}

// Lit tous les mots du fichier
// Renvoie false si le fichier n'existe pas
bool chargerFichier(const string& nomFichier, vector<string>& mots) {
    ifstream fichier(nomFichier);
    if (!fichier) return false;

    string mot;
    while (fichier >> mot) {            // lit un mot à la fois
        mot = nettoyer(mot);
        if (!mot.empty()) mots.push_back(mot);
    }
    return true;
}

// Recherche 1 : mot exact, on regarde chaque mot un par un     O(n)
int compterMotExact(const vector<string>& mots, const string& cherche) {
    int compteur = 0;
    for (size_t i = 0; i < mots.size(); i++) {
        if (mots[i] == cherche) compteur++;
    }
    return compteur;
}

// Recherche 2 : mot exact, par dichotomie (tableau trié)        O(log n)
// Renvoie 1 si trouvé, 0 sinon
int rechercheDichotomique(const vector<string>& motsTries, const string& cherche) {
    int gauche = 0;
    int droite = (int)motsTries.size() - 1;

    while (gauche <= droite) {
        int milieu = (gauche + droite) / 2;
        if (motsTries[milieu] == cherche) return 1;           // trouvé
        if (motsTries[milieu] < cherche)  gauche = milieu + 1; // chercher à droite
        else                              droite = milieu - 1; // chercher à gauche
    }
    return 0;                                                 // absent
}

// Recherche 3 : mots qui contiennent "cherche" collé
// Exemple : "programmation" contient "gram"
int compterSousChaine(const vector<string>& mots, const string& cherche) {
    int compteur = 0;
    for (size_t i = 0; i < mots.size(); i++) {
        if (mots[i].find(cherche) != string::npos) compteur++;
    }
    return compteur;
}

// Vrai si les lettres de mot2 sont dans mot1, dans le même ordre
bool contientDansOrdre(const string& mot1, const string& mot2) {
    size_t i = 0;   // position dans mot1
    size_t j = 0;   // lettre attendue de mot2
    while (i < mot1.size() && j < mot2.size()) {
        if (mot1[i] == mot2[j]) j++;    // trouvée : lettre suivante
        i++;
    }
    return j == mot2.size();
}

// Recherche 4 : mots qui ont les lettres de "cherche" dans l'ordre
// Exemple : "programmation" contient "pain"
int compterDansOrdre(const vector<string>& mots, const string& cherche) {
    int compteur = 0;
    for (size_t i = 0; i < mots.size(); i++) {
        if (contientDansOrdre(mots[i], cherche)) compteur++;
    }
    return compteur;
}

// Type "pointeur vers une des 4 recherches"
typedef int (*Recherche)(const vector<string>&, const string&);

const int REPETITIONS = 20;

// Lance la recherche plusieurs fois, renvoie le temps moyen (en µs)
double mesurer(Recherche f, const vector<string>& mots, const string& cherche, int& resultat) {
    double total = 0;
    for (int r = 0; r < REPETITIONS; r++) {
        auto debut = chrono::steady_clock::now();
        resultat = f(mots, cherche);
        auto fin = chrono::steady_clock::now();
        total += chrono::duration<double, micro>(fin - debut).count();
    }
    return total / REPETITIONS;
}

int main() {
    string nomFichier, cherche;

    cout << "Nom du fichier (ex: roman.txt) : ";
    getline(cin, nomFichier);
    cout << "Mot a rechercher : ";
    cin >> cherche;
    cherche = nettoyer(cherche);

    // 1. Lire le fichier
    vector<string> mots;
    if (!chargerFichier(nomFichier, mots)) {
        cerr << "Erreur : impossible d'ouvrir \"" << nomFichier << "\"." << endl;
        return 1;
    }
    cout << endl << mots.size() << " mots charges." << endl << endl;
    if (mots.empty() || cherche.empty()) return 0;

    // 2. Trier une copie (pour la dichotomie)
    vector<string> motsTries = mots;
    sort(motsTries.begin(), motsTries.end());

    // 3. Lancer et chronométrer les 4 recherches
    int nbExact = 0, trouveDicho = 0, nbColle = 0, nbOrdre = 0;
    double tempsExact = mesurer(compterMotExact,       mots,      cherche, nbExact);
    double tempsDicho = mesurer(rechercheDichotomique, motsTries, cherche, trouveDicho);
    double tempsColle = mesurer(compterSousChaine,     mots,      cherche, nbColle);
    double tempsOrdre = mesurer(compterDansOrdre,      mots,      cherche, nbOrdre);

    // 4. Afficher les résultats
    cout << "Recherche de \"" << cherche << "\" :" << endl;
    cout << "  1. Mot exact, lineaire      : " << nbExact << " fois"
         << "   (" << tempsExact << " us)" << endl;
    cout << "  2. Mot exact, dichotomique  : " << (trouveDicho ? "present" : "absent")
         << "   (" << tempsDicho << " us)" << endl;
    cout << "  3. Mots qui le contiennent colle (find)   : " << nbColle
         << "   (" << tempsColle << " us)" << endl;
    cout << "  4. Mots qui ont ses lettres dans l'ordre  : " << nbOrdre
         << "   (" << tempsOrdre << " us)" << endl;

    return 0;
}
