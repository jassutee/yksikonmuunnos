#include "merkkijonoapu.hh"

#include <sstream>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;


// Palauttaa parametrin <<mjono>> reaaliluvuksi muutettuna
// tai arvon NaN (Not a Number), jos <<mjono>> ei ole reaaliluku
// tai jos se sisältää jotain ylimääräistä. <<mjono>>:n alussa
// ja lopussa saa kuitenkin olla välilyöntimerkkejä.
double muuta_string_doubleksi(const string& mjono) {
    istringstream virta(mjono);

    double tulos{0.0};
    virta >> ws >> tulos >> ws;

    if ( virta.fail() or virta.peek() != EOF ) {
        return stod("NaN");
    } else {
        return tulos;
    }
}


// Funktio palauttaa true, jos parametrimerkkijono <<mjono>>
// esittää reaalilukua. Muussa tapauksessa paluuarvo on false.
bool onko_double_string(const string& mjono) {
    double dbl{ muuta_string_doubleksi(mjono) };

    if ( dbl != dbl ) {  // NaN on erisuuri kuin NaN
        return false;
    } else {
        return true;
    }
}


// Funktio poistaa parametrista <<mjono>> kaikki välilyöntimerkit.
// Paitsi että tulos välittyy kutsujalle viiteparametrin arvona,
// funktio myös palauttaa muuttuneen merkkijonon.
string poista_valilyonnit(string& mjono) {
    string::size_type indeksi{0};
    while ( indeksi < mjono.length() ) {
        if ( mjono.at(indeksi) == ' ' ) {
            mjono.erase(indeksi, 1);
        } else {
            ++indeksi;
        }
    }

    return mjono;
}


// Poistaa välilyöntimerkit <<mjono>>:n alusta ja lopusta.
// muuttunut arvo välittyy kutsujalle sekä viiteparametrina että
// varsinaisena paluuarvona.  Toteutus on kömpelö ja tehoton,
// eikä siitä kannata ottaa mallia omaan elämään.
string poista_valilyonnit_alusta_ja_lopusta(string& mjono) {
    while ( not mjono.empty() and mjono.at(mjono.length() - 1) == ' ' ) {
        mjono.pop_back();
    }

    while ( not mjono.empty() and mjono.at(0) == ' ' ) {
        mjono.erase(0, 1);
    }

    return mjono;
}


// Tämä funktio on kehittyneempi versio paloittelufunktiosta,
// joka toteutettiin viikkoharjoituksissa ja joka osasi paloitella
// merkkijonon osiin tietyn erotinmerkin kohdalta.
// Tämä funktio toimii seuraavasti: jos funkiota kutsutaan:
//
//     paloittele_rivi( "C <=  0.555556 * F – 17.777778",
//                      { "<=", "*", "-" }, tuloskentat );
//
// Sen paluuarvoksi tulee true, koska käsilteltävänä olevasta
// merkkijonosta <<mjono>> löytyy alimerkkijonot "<=", "*" ja "-".
// Lisäksi se täyttää parametrin <<tuloskentat>> alkioiksi
// merkkijonot "C", "<=", "0.555556", "*", "F", "-" ja "17.777778".
// Eli tulosmerkkijonoon päätyvät sekä varsinaiset kentät että käytetyt
// erotinmerkkijonot siinä järjestyksessä kun ne olivat <<mjono>>:ssa.
bool paloittele_rivi(const string& mjono,
                     const vector<string>& erottimet,
                     vector<string>& kentat) {
    kentat.clear();

    string::size_type alkukohta{0};
    vector<string>::size_type kasiteltava_indeksi{0};

    while ( kasiteltava_indeksi < erottimet.size() ) {
        string erotin{ erottimet.at(kasiteltava_indeksi) };
        string::size_type erotinkohta{ mjono.find(erotin, alkukohta) };

        if ( erotinkohta == string::npos) {
            kentat.clear();
            return false;
        }

        kentat.push_back(mjono.substr(alkukohta, erotinkohta - alkukohta));
        kentat.push_back(erotin);
        alkukohta = erotinkohta + erotin.length();

        ++kasiteltava_indeksi;
    }

    kentat.push_back(mjono.substr(alkukohta));

    for ( auto& arvo: kentat ) {
        poista_valilyonnit_alusta_ja_lopusta(arvo);
    }

    return true;
}


//-----------------------------------------------------------------------------
// Loppukoodi sisältää yksikertaisia testejä valmiiden funktioiden
// oikean toiminnan varmistamiseksi.  Harjoitustyötä koodatessa
// '#ifdef TESTAA_MERKKIJONOAPU'- ja '#endif'-rivien välisistä
// koodiriveistä ei ole tarpeen välittää.

#ifdef TESTAA_MERKKIJONOAPU

#include <iostream>


void testaa_doublet(const string& mjono) {
    cout << __func__ << " [" << mjono << "] "
         << boolalpha << onko_double_string(mjono) << " "
         << muuta_string_doubleksi(mjono) << endl;
}


void testaa_valilyonnit(string mjono) {
    cout << __func__ << " [" << mjono << "] -> [";
    cout << poista_valilyonnit(mjono) << "]" << endl;
}


void testaa_paloittelu(const string& mjono, const vector<string>& erottimet) {
    cout << __func__ << " [" << mjono << "]" << endl << "    -> ";

    vector<string> tulos;
    bool paluuarvo{ paloittele_rivi(mjono, erottimet, tulos) };
    cout << boolalpha << paluuarvo << " {";
    for ( auto arvo: tulos ) {
        cout << "  \"" << arvo << "\"";
    }
    cout << "  }" << endl;
}


int main() {
    testaa_doublet("0");
    testaa_doublet("1.23");
    testaa_doublet("1.23e45");
    testaa_doublet("  0  ");
    testaa_doublet("  1.23  ");
    testaa_doublet("  1.23e45  ");
    testaa_doublet("");
    testaa_doublet("   ");
    testaa_doublet("a");
    testaa_doublet("a1.23");
    testaa_doublet("1.23a");

    testaa_valilyonnit("");
    testaa_valilyonnit(" ");
    testaa_valilyonnit("   ");
    testaa_valilyonnit("a");
    testaa_valilyonnit("ab");
    testaa_valilyonnit("abc");
    testaa_valilyonnit(" a b c ");
    testaa_valilyonnit("  aa  bb  cc  ");
    testaa_valilyonnit("   aaa   bbb   ccc   ");

    testaa_paloittelu("",
                      { } );
    testaa_paloittelu("kPa<=0.001*Pa",
                      { "<=", "*" } );
    testaa_paloittelu("kPa <= 0.001 * Pa",
                      { "<=", "*" } );
    testaa_paloittelu("   kPa    <=    0.001   *    Pa",
                      { "<=", "*" } );
    testaa_paloittelu("C<=0.55*F-17.7",
                      { "<=", "*", "-" } );
    testaa_paloittelu("C <= 0.55 * F - 17.7",
                      { "<=", "*", "-" } );
    testaa_paloittelu("   C   <=   0.55   *   F   -   17.7  ",
                      { "<=", "*", "-" } );

}

#endif
