// Recherche les lettres d'un mot dans un fichier texte, dans le même ordre.
// Les lettres n'ont pas besoin d'être côte à côte.
// Compilation : g++ -O2 -o RechercheDansFichier.exe RechercheDansFichier.cpp
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

bool contientDansFichier(ifstream& fichier, const string& motRecherche) {
    size_t position = 0;
    char caractere;

    while (fichier.get(caractere)) {
        if (caractere == motRecherche[position]) {
            position++;
            if (position == motRecherche.size()) return true;
        }
    }

    return false;
}

int main() {
    string nomFichier;
    string motRecherche;

    cout << "Nom du fichier .txt (ex: texte.txt) : ";
    getline(cin, nomFichier);

    cout << "Mot a rechercher : ";
    cin >> motRecherche;

    ifstream fichier(nomFichier);
    if (!fichier.is_open()) {
        cerr << "Erreur : impossible d'ouvrir le fichier \"" << nomFichier << "\"." << endl;
        return 1;
    }

    const chrono::steady_clock::time_point debut = chrono::steady_clock::now();
    const bool resultat = contientDansFichier(fichier, motRecherche);
    const chrono::steady_clock::time_point fin = chrono::steady_clock::now();
    const chrono::duration<double, milli> duree = fin - debut;

    if (resultat) {
        cout << "Les lettres de \"" << motRecherche
             << "\" ont ete trouvees dans le fichier, dans le meme ordre." << endl;
    } else if (fichier.bad()) {
        cerr << "Erreur pendant la lecture du fichier." << endl;
        return 1;
    } else {
        cout << "Les lettres de \"" << motRecherche
             << "\" n'ont pas ete trouvees dans le fichier dans cet ordre." << endl;
    }

    cout << "Temps de recherche : " << duree.count() << " ms" << endl;

    return 0;
}
