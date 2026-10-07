#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

// Crée un fichier de test avec N chaînes : chaine00000001, chaine00000002, ...
// La dernière ligne est "zzzzfin"
int main() {
    string nomFichier;
    int n;

    cout << "Nom du fichier a creer (ex: texte_10M.txt) : ";
    getline(cin, nomFichier);
    cout << "Nombre de chaines (ex: 10000000) : ";
    cin >> n;

    ofstream fichier(nomFichier);
    if (!fichier) {
        cerr << "Erreur : impossible de creer \"" << nomFichier << "\"." << endl;
        return 1;
    }

    for (int i = 1; i < n; i++) {
        fichier << "chaine" << setw(8) << setfill('0') << i << '\n';   // ex: chaine00000042
    }
    fichier << "zzzzfin" << '\n';

    cout << n << " chaines ecrites dans " << nomFichier << endl;
    return 0;
}
