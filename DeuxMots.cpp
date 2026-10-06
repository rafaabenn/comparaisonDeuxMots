#include <iostream>
#include <string>
#include <chrono>
using namespace std;

// ============================================================
// VERSION 1 : avec deux indices
// ============================================================
bool contient(const string& mot1, const string& mot2) {

    size_t i = 0;   // i = ou on en est dans mot1 (on lit mot1 lettre par lettre)
    size_t j = 0;   // j = la lettre de mot2 qu'on attend

    // On continue tant qu'il reste des lettres a lire dans mot1
    // ET qu'il reste des lettres a trouver dans mot2
    while (i < mot1.size() && j < mot2.size()) {

        // Si la lettre lue dans mot1 est celle qu'on attend
        if (mot1[i] == mot2[j]) {
            j++; //elle est trouvee : on attend maintenant la lettre suivante de mot2
        }

        i++; //Dans tous les cas, on passe a la lettre suivante de mot1
    }
    // Si j est arrive au bout de mot2, on a trouve toutes ses lettres
    return j == mot2.size();
}

// ============================================================
// VERSION 2 : avec la fonction find
// ============================================================
bool contientFind(const string& mot1, const string& mot2) {

    size_t pos = 0;   // pos = a partir d'ou on cherche dans mot1

    // On prend les lettres de mot2 une par une
    for (size_t j = 0; j < mot2.size(); j++) {

        // On cherche la lettre mot2[j] dans mot1, a partir de la position pos
        pos = mot1.find(mot2[j], pos);

        // npos veut dire "pas trouve" : la lettre n'existe plus dans mot1
        if (pos == string::npos) {
            return false;
        }

        // La lettre suivante devra se trouver APRES celle-ci
        pos++;
    }

    // On est sorti de la boucle : toutes les lettres ont ete trouvees
    return true;
}

// ============================================================
// Programme principal
// ============================================================
int main() {
    string mot1, mot2;
    cout << "Entrez mot1 : ";
    cin >> mot1;
    cout << "Entrez mot2 : ";
    cin >> mot2;
    cout << endl;

    // Un seul appel est trop rapide pour etre mesure :
    // on repete chaque fonction N fois et on divise le temps total par N
    const int N = 1000000;
    bool resultat1 = false, resultat2 = false;

    // ---- Mesure de la version 1 ----
    auto debut1 = chrono::steady_clock::now();
    for (int k = 0; k < N; k++) {
        resultat1 = contient(mot1, mot2);
    }
    auto fin1 = chrono::steady_clock::now();
    double temps1 = chrono::duration<double, nano>(fin1 - debut1).count() / N;

    // ---- Mesure de la version 2 ----
    auto debut2 = chrono::steady_clock::now();
    for (int k = 0; k < N; k++) {
        resultat2 = contientFind(mot1, mot2);
    }
    auto fin2 = chrono::steady_clock::now();
    double temps2 = chrono::duration<double, nano>(fin2 - debut2).count() / N;

    cout << "Version deux indices : " << (resultat1 ? "oui" : "non")
         << "  (temps moyen : " << temps1 << " ns)" << endl;
    cout << "Version find         : " << (resultat2 ? "oui" : "non")
         << "  (temps moyen : " << temps2 << " ns)" << endl;

    return 0;
}