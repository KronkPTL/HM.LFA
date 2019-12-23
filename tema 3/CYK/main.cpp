#include <bits/stdc++.h>

using namespace std;

class Productie
{
    string prod;

public:
    Productie (string);
    ~Productie ();
    string getProd () const;
    void setProd (const string&);
    friend istream& operator >> (istream&, Productie&);
    friend Productie operator + (const Productie&, const Productie&); // reuniune
    friend Productie operator * (const Productie&, const Productie&); // produs cartezian

};

Productie :: Productie (string _prod = "") : prod (_prod)
{

}

Productie :: ~Productie ()
{
    prod.erase();
}

string Productie :: getProd () const
{
    return prod;
}

void Productie :: setProd (const string &_prod)
{
    this-> prod = _prod;
}

istream& operator >> (istream &in, Productie &P)
{
    in >> P.prod;
    return in;
}

///ca sa imi concateneze 2 intre ele in cazul in care sunt mai multe de adaugat pe o singura pozitie
Productie operator + (const Productie &P, const Productie &Q)
{
    Productie _Rez;
    _Rez.setProd (P.prod + " " + Q.prod);
    return _Rez;
};

///ca sa fie mai usor de scris in momentul in care "inmultesc" coloana cu diagonala
Productie operator * (const Productie &P, const Productie &Q)
{

    if (P.getProd() == "" || P.getProd() == " ") ///vid sau gol
        return Q;

    if (Q.getProd() == "" || Q.getProd() == " ") ///vid sau gol
        return P;

    string str = "";

    for (register unsigned i = 0; i < P.prod.length(); ++i) ///"inmultirea" propriu-zisa
        for (register unsigned j = 0; j < Q.prod.length(); ++j)
        {
            str.push_back (P.prod[i]);
            str.push_back (Q.prod[j]);
            str.push_back (' ');
        }

    Productie _Rez;
    _Rez.setProd (str);

    return _Rez;
}

class CYK
{
    map <char, vector<Productie> > cykArr; //matricea
    string productieCauta (const string&); //pt a gasit un cuvant si sa il puna in sir

public:

    CYK ();
    ~CYK ();
    friend istream& operator >> (istream&, CYK&);
    string productieMinusSir (const char*); // const char* ca sa pot face conversie string -> char* si apoi strtok pe char*
    bool CYKAlg (const string&);
};

CYK :: CYK ()
{

}

CYK :: ~CYK ()
{

}
///imi spune cine a generat o productie
string CYK :: productieCauta (const string &cuvant)
{
    string _Rez;

    for (map<char, vector<Productie> >::iterator it = cykArr.begin(); it != cykArr.end(); it++)
    {
        for (Productie _prod : it->second)
        {
            if (_prod.getProd() == cuvant)
               _Rez.push_back(it->first);
        }
    }

    return _Rez;
}

/// const char* ca sa pot face conversie string -> char* si apoi strtok pe char*
string CYK :: productieMinusSir (const char* cuvant)
{
    string _Rez = "";

    char *_cuvant;
    memcpy ((char*)(&_cuvant), (const char*)(&cuvant), sizeof(cuvant) + 1);

    char *_element = strtok (_cuvant, " ");


    while (_element)
    {
        _Rez += productieCauta (_element);
        _element = strtok (NULL, " ");
    }

    return _Rez;
}

istream& operator >> (istream &in, CYK &cyk)
{
    int nr;
    in >> nr;

    for (int i = 0; i < nr; ++i)
    {
        char start;
        in >> start;

        char ch;
        while (ch != '>')
            in >> ch;

        Productie _prod;
        do
        {
            in >> _prod;

            if (_prod.getProd() != " " && _prod.getProd() != ";" && _prod.getProd() != "->" && _prod.getProd() != "|")
                cyk.cykArr[start].push_back(_prod);

        } while (_prod.getProd() != ";");

    }

    return in;
}


bool CYK :: CYKAlg (const string& cuv)
{
    /// Alocare memorie

    Productie **V = new Productie* [cuv.length() + 1];

    for (register unsigned i = 0; i <= cuv.length(); ++i)
        V[i] = new Productie [cuv.length() + 1];

    /// Prima "linie" din tabel

    for (register unsigned i = 1; i <= cuv.length(); ++i)
    {
        string _str;
        _str.push_back (cuv [i - 1]);
        V[i][1].setProd (productieCauta(_str));
    }

    /// Completez stiind ca Vij = Uk(>0,<j) (Vik x V(i+k)(j-k))

    ///de sus in jos dupa diagonala secundara
    for (register unsigned j = 2; j <= cuv.length(); ++j)
        for (register unsigned i = 1; i <= cuv.length() - j + 1; ++i)
        {
            V[i][j].setProd("");

            for (register unsigned k = 1; k < j; ++k)
                 V[i][j].setProd((V[i][j] + V[i][k] * V[i+k][j-k]).getProd());


            const char* cstr = V[i][j].getProd().c_str(); ///sa fie pe bucati
            V[i][j].setProd (productieMinusSir(cstr));

        }

    for(register unsigned j = 1; j <= cuv.length(); ++j)
        {
            for(register unsigned int i = 1; i <= cuv.length() - j + 1; ++i)
                if(V[i][j].getProd() == "" || V[i][j].getProd() == " ")
                    cout << "# ";
                else
                    cout << V[i][j].getProd() << " ";
            cout << "\n";
        }
    string _auxStr = V[1][cuv.length()].getProd();

    ///daca se afla S prin cele finale
    for (string::iterator it = _auxStr.begin(); it != _auxStr.end(); ++it)
        if (*it == 'S')
            return true;

    return false;

}

int main()
{
    CYK fnc;

    ifstream fin ("cyk2.in");
    fin >> fnc;

    fin.close();

    cout << "Introduceti cuvinte de verificat. Pentru a opri, tastati stop\n\n";

    string cuvant;
    do
    {
        cin >> cuvant;

        if (fnc.CYKAlg(cuvant))
            cout << "Acceptat\n\n";
        else
            cout << "Respins\n\n";

    } while (cuvant != "stop");

    return 0;
}
