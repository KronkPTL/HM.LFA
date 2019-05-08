#include <iostream>
#include <fstream>
#include <string.h>

#define dim 100

using namespace std;

typedef struct{
    char stare_grup[dim];
}grupare;

///cautarea unui caracter intr-un sir si returnarea pozitiei
int poz(char ch, char s[])
{
    for(int i = 0; i < strlen(s); i++)
        if(s[i] == ch)
            return i;
    return -1;
}

///stergerea unei stari dintr-o pozitie din graf
void stergeStareCurenta(int index, char stari[], char ch, char finale[], char nefinale[], int viz[])
{
    int i;
    for(i = index; i < strlen(stari) - 1; i++)
        viz[i] = viz[i + 1];

    for(i = 0; i < strlen(stari); i++)
        if(stari[i] == ch)
            strcpy(stari + i, stari + i + 1);

    for(i = 0; i < strlen(finale); i++)
        if(finale[i] == ch)
            strcpy(finale + i, finale + i + 1);

    for(i = 0; i < strlen(nefinale); i++)
        if(nefinale[i] == ch)
            strcpy(nefinale + i, nefinale + i + 1);

}

void DFS(char index, int viz[], char stari[], char alfabet[], char mat_transpozitii[][dim])
{
    if(viz[poz(index, stari)] == 0)
        viz[poz(index, stari)] = 1;

    for(int i = 0; i < strlen(alfabet); i++)
        if(mat_transpozitii[poz(index, stari)][i] != '-' && viz[poz(mat_transpozitii[poz(index, stari)][i], stari)] == 0) //daca exista drum si nu a mai fost vizitat
            DFS(mat_transpozitii[poz(index, stari)][i], viz, stari, alfabet, mat_transpozitii);
}

char* pereche(int nr, grupare sir[], char x)
{
    int j;
    for (j = 0; j < nr; j++)
        if (strchr(sir[j].stare_grup, x) != 0)
            return sir[j].stare_grup;

}

void citire(char stari[], char alfabet[], char finale[], char &init, char mat_transpozitii[][dim], int &nrtranz)
{
    ifstream fin("in.txt");
    int nrStari, nrAlfabet, nrFinale, i, j;
    char x;
    ///citirea starilor
    fin >> nrStari;
    for(i = 0; i < nrStari; i++)
    {
        fin >> x;
        stari[i] = x;
    }
    stari[nrStari] = NULL;

    ///citirea alfabetului
    fin >> nrAlfabet;
    for(i = 0; i < nrAlfabet; i++)
    {
        fin >> x;
        alfabet[i] = x;
    }
    alfabet[nrAlfabet] = NULL;

    ///citirea starilor finale
    fin >> nrFinale;
    for(i = 0; i < nrFinale; i++)
    {
        fin >> x;
        finale[i] = x;
    }
    finale[nrFinale] = NULL;

    ///citirea starii initiale
    fin >> init;

    ///citirea matricei transpozitiilor
    for(i = 0; i < strlen(stari); i++)
        for(j = 0; j < strlen(alfabet); j++)
            mat_transpozitii[i][j]  = '-'; //pentru existenta "drumului"

    fin >> nrtranz;
    //cout << "nrtranzitii: " << nrtranz << "\n";
    for(i = 0; i < nrtranz; i++)
    {
        char x, y, z;
        fin >> x >> y >> z;
        //cout << x << " " << y << " " << z << "\n";
        mat_transpozitii[poz(x, stari)][poz(y, alfabet)] = z;
        cout << poz(x, stari) <<" cu " << poz(y, alfabet) << " au "<< mat_transpozitii[poz(x, stari)][poz(y, alfabet)] << "\n";
    }
    for(i = 0; i < strlen(stari); i++)
    {
        for(j = 0; j < strlen(alfabet); j++)
            cout << mat_transpozitii[i][j] << " ";
        cout << "\n";
    }
    cout <<"\ncitire pana aici\n\n";
}

void afis(int nr, grupare sir[], char stari[], char alfabet[], char mat_transpozitii[][dim], char finale[])
{
    ofstream fout("min.txt");
    int i, j, k;
    for(i = 0; i < nr; i++)
    {
        char y, x;
        int index, k;
        y = sir[i].stare_grup[1];
        for(k = 0; k < strlen(stari); k++)
            if(stari[k] == y)
                index = k;
        for(j = 0; j < strlen(alfabet); j++)
        {
            int ok = 0;
            x = mat_transpozitii[index][j];
            fout << "Starea " << sir[i].stare_grup << "cu " << alfabet[j] << " ajunge in: " << pereche(nr, sir, x);

            ///afisare daca e final
            int l;
            for(l = 0; l < strlen(sir[i].stare_grup) && ok == 0; l++)
            {
                int m;
                for(m = 0; m < strlen(finale); m++)
                    if(sir[i].stare_grup[j] == finale[m])
                    {
                        ok = 1;
                        fout << "-> grupul curent este o STARE FINALA";
                        break;
                    }
            }
            fout << "\n";
        }

    }
}
int main()
{
    char alfabet[dim], stari[dim], finale[dim], nefinale[dim], mat_transpozitii[dim][dim], init, c;
    int i, j, k, nrtranz, minimal_dfa[dim][dim];

    ///citire
    citire(stari, alfabet, finale, init, mat_transpozitii, nrtranz);
    //cout << stari << "\n";
    //cout << alfabet << "\n";
    //cout << finale << "\n";
    //cout << init << "\n";


    ///folosesc teorema Myhill Nerode
    ///pasul 1 - construiesc vectorul cu stari nefinale pentru a verifica, ulterior, daca una este finala si cealalta nu
    int n = 0;
    for(i = 0; i < strlen(stari); i++)
    {
        if(strchr(finale, stari[i]) == 0)
        {
            nefinale[n] = stari[i];
            n++;
        }
    }

    ///pasul 2 - starile la care nu se pot ajunge in graf sunt eliminate
    int viz[strlen(stari)];
    for(i = 0; i < strlen(stari); i++)
    {
        viz[i] = 0;
    }

    ///parcurgerea grafului pentru marcarea viz-ului pentru a verifica unde nu ajunge
    DFS(init, viz, stari, alfabet, mat_transpozitii);

    ///stergerea propriu-zisa
    i = 0;
    while(stari[i] != NULL)
    {
        if(viz[i] == 0)
        {
            stergeStareCurenta(i, stari, stari[i], finale, nefinale, viz);
            int j, k;
            for(j = i; j < strlen(stari) - 1; j++)
                for(k = 0; k < strlen(alfabet); k++)
                    mat_transpozitii[j][k] = mat_transpozitii[j + 1][k];
        }
        else
        {
            i++;
        }
    }

    ///pasul 3 - contruiesc matricea pentru minimizarea dfa-ului
    for(i = 0; i < strlen(stari); i++)
        for(j = 0; j < strlen(stari); j++)
            minimal_dfa[i][j] = 0;

    for(i = 1; i < strlen(stari); i++) //pana in diagonala principala, fara ea
        for(j = 0; j < i; j++)
            {
                int ok = 1;
                ///verificare conditiei de la pasul 1 de mai sus (daca una e finala si cealalta nu)
                if((strchr(finale, stari[i]) != 0 && strchr(finale, stari[j]) != 0) || (strchr(nefinale, stari[i]) != 0 && strchr(nefinale, stari[j]) != 0))
                    ok = 0;

                if(ok == 1)
                    minimal_dfa[i][j] = 1;
            }

    int oke;
    do ///repeta pana nu se mai pot marca in matrice
    {
        oke = 0;
        for (i = 1; i < strlen(stari); i++)
            for (j = 0; j < i; j++)
                if (minimal_dfa[i][j] == 0)
                {
                    int k, ok = 0;
                    for (k = 0; k < strlen(alfabet); k++) ///pentru fiecare componenta din alfabet verificam unde "le duce drumul"
                    {
                        char x, y;
                        x = mat_transpozitii[i][k];
                        y = mat_transpozitii[j][k];
                        if (x < y)
                        {
                            char aux = x;
                            x = y;
                            y = aux;
                        }

                        if (minimal_dfa[poz(x, stari)][poz(y, stari)] == 1) ///daca au ajuns intr-o stare marcata cu 1
                            ok = 1;
                    }
                    if (ok == 1)
                    {
                        minimal_dfa[i][j] = 1;
                        oke = 1;
                    }
                }
    }while(oke != 0);

    //afisare pt verificare
    for(i = 0; i < strlen(stari); i++)
    {

        for(j = 0; j < i; j++)
            cout << minimal_dfa[i][j] << " ";
        cout << "\n";
    }

    char v[strlen(stari)];
    for(i = 0; i < strlen(stari); i++)
        v[i] = '-';

    ///pasul 4 - "grupam" sau nu
    int nr = 0;
    for(i = 1; i < strlen(stari); i++)
        for(j = 0; j < i; j++)
            if(minimal_dfa[i][j] == 0)
            {
                char x, y;
                x = stari[i];
                y = stari[j];
                if(v[j] == '-')
                {
                    v[i] = y;
                    v[j] = y;
                    nr++;
                }
                else
                {
                    v[i] = v[j];
                }

            }

    ///care n-au fost grupate anterior
    for(i = 0; i < strlen(stari); i++)
        if(v[i] == '-')
        {
            v[i] = stari[i];
            nr++;
        }

    //afisare pentru verificare
    for(i = 0; i < strlen(stari); i++)
        cout << v[i] << " ";
    cout << "\n";

    grupare sir[nr];
    n = 0;
    int poz  = 0;
    while(n < nr) ///gruparea
    {
        int m = 0;
        char k = v[poz];
        sir[n].stare_grup[m++] = '{';
        for(i = poz; i < strlen(stari); i++)
        {
            if(v[i] == k)
            {
                sir[n].stare_grup[m] = stari[i];
                m++;
                sir[n].stare_grup[m] = ',';
                m++;
            }
            else
            {
                poz = i;
                break;
            }

        }

        sir[n].stare_grup[m - 1] = '}';
        sir[n].stare_grup[m++] = ' ';
        sir[n].stare_grup[m] = '\0';
        n++;
    }

    ///afisare
    afis(nr, sir, stari, alfabet, mat_transpozitii, finale);

    return 0;
}
