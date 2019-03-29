#include <map>
#include <vector>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

void citire (vector<int> &Q, vector<char> &Sigma, vector<int> &F, int &q0, map <pair <int, char>, vector<int>> &delta)
{
    ifstream fin ("exemplu.in");
    int nrStari;
    fin >> nrStari;  /// Citirea starilor
    for(int i = 0; i < nrStari; i++)
        Q.push_back(i);

    char litera;
    do
    {
        fin >> litera;  /// Citirea alfabetului Sigma
        if (litera != ';')
            Sigma.push_back(litera);
    }
    while(litera != ';');

    fin >> q0;  /// Citirea starii initiale

    int nrFinale;  /// Citirea numarului de stari finale
    fin >> nrFinale; /// Citirea starilor finale
    for(int i = 0; i < nrFinale; i++)
    {
        int f;
        fin >> f;
        F.push_back(f);
    }

    int stare1, stare2;
    while(1)  /// Citirea tranzitiilor
    {
        fin >> stare1;
        if (stare1 == -1)
            break;
        fin >> litera >> stare2;
        delta[{stare1, litera}].push_back(stare2);
    }
    fin.close();
}

void afisare_delta(map <pair <int, char>, vector<int>> delta)
{
    for(map <pair <int, char>, vector<int>> :: iterator it = delta.begin(); it != delta.end(); it++)
    {
        cout << it -> first.first << " " << it -> first.second << " : ";

        for(int a : delta[{it -> first.first, it -> first.second}])
           cout << a << " ";
        cout<<"\n";
    }
}

vector<int> generare (int q0, string w, map <pair <int, char>, vector<int>> delta) ///principiul de la bfs (merg in vecini)
{
	int n = w.length(); ///lungimea starii introduse

	vector<int> stari;

    for(int j : delta[{q0, w[0]}]) ///starile unde poate merge dupa prima stare
        stari.push_back(j);
	n--;
	if (n == 0)
        return stari;
    int contor = 1;
	while(n)
	{
	    vector<int> aux;
        for(int i : stari) ///lucreaza ca v[i]
        {
            //cout << "i = " << i << "\n";
            for(int j : delta[{i, w[contor]}]) ///prin ce contine map-ul in pair de la v[i] si unde trebuie sa ajunga, daca are
                {
                    aux.push_back(j);
                    //cout << j << " cu " << w[contor] << ", ";
                }
            //cout << "\n";
        }
        n--;
        contor++; ///trece la urmatoarea stare din cele introduse
        //cout << "\nPasul pentru stari: " << w[contor-1] << "\n";
        //for(int k : stari)
          //  cout << k << " ";
        //cout << "\nPasul pt aux: " << w[contor-1] << "\n";
        //for(int k : aux)
          //  cout << k << " ";

        //cout << "\n---------------------\n";

        stari.clear();
        for(int i : aux)
            stari.push_back(i);
        aux.clear();
	}
    return stari;
}

int stareFinala(int p, vector<int> F)
{
    for(vector<int> :: iterator it = F.begin(); it != F.end(); it++)
        if (*it == p)
            return 1;
    return 0;
}

int finala(vector<int> p, vector<int> F)
{
    for(vector<int> :: iterator it = p.begin(); it != p.end(); it++)
        if(stareFinala(*it, F))
           return 1;
    return 0;
}

int main()
{
    vector <int> Q; /// multimea starilor initiale
    vector <char> Sigma; /// alfabet
    vector <int> F; /// multimea starilor finale
    int q0;   /// starea initiala
    map <pair <int, char>, vector<int>> delta; /// functia de tranzitie

    citire(Q, Sigma, F, q0, delta);
    afisare_delta(delta);

    char cuv[256];
    cout << "Introduceti starile: ";
    cin >> cuv;
    vector<int> p = generare(q0, cuv, delta);

    //for(int k : p)
        //cout << k << " ";
    cout << "\n";

    if(finala(p, F) || (cuv[0] == '$' && stareFinala(q0, F)))
        cout << "Acceptat";
    else
        cout << "Respins";
    return 0;
}
