/* Apufunktiot-moduli. Täällä määritellään sekä pääohjelmassa, että
luokassa tarvittavia funktioita. Eniten tarvitaan kysy- ja vastaus_
oikeaan_muotoon-funktioita. Laitoin virheet omiksi funktioikseen,
koska niitä saattaa tulla niin monessa eri kohdassa.*/

#include "merkkijonoapu.hh"
#include "apufunktiot.hh"

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

// Käyttäjältä kysymis-funktio. Palauttaa vastauksen sellaisenaan.
string kysy(const string& kysymys) {
    string vastaus{""};
    cout << kysymys;
    getline(cin, vastaus);
    return vastaus;
}

// Käyttäjän syötteestä aiheutuva virheviesti.
void virhe(const string &syote) {
    cout << "Virhe kayttajan syotteessa: " << syote << endl;
}

// Tiedostosta luettaessa aiheutuva virheviesti.
void tiedostovirhe(const string &syote) {
    cout << "Virhe syotetiedoston lukemisessa: " << syote << endl;
}

// Paloittelee vastauksen osiin ja tarkistaa, että se on muotoa reaaliluku
// yksikko yksikko. merkkijonoapua käyttäen etsitään vastauksen alusta alkaen
// palaset poistamalla joka välissä välilyönnit alusta ja lopusta.
bool vastaus_oikeaan_muotoon(string &vastaus, double &muuttuja,
                             string &lahtoyksikko, string &kohdeyksikko) {
    poista_valilyonnit_alusta_ja_lopusta(vastaus);
    vector<string> vastaukset{};
    // Ensin paloitellaan vastaus ensimmäisen välilyonnin kohdalta, eli
    // erotellaan vain ensimmäinen kenttä lopuista. Muutetaan se doubleksi.
    paloittele_rivi(vastaus, {" "}, vastaukset);
    if (not onko_double_string(vastaukset.at(0))) {
        virhe("Syotteen ensimmainen kentta ei ole reaaliluku!");
        return false;
    }
    muuttuja = muuta_string_doubleksi(vastaukset.at(0));
    // Sitten erotetaan seuraava kenttä lopusta tekstistä. Jos käyttäjä on
    // syöttänyt oikein, ei kenttiä ole enempää eroteltavana.
    string yksikot{poista_valilyonnit_alusta_ja_lopusta(vastaukset.at(2))};
    paloittele_rivi(yksikot, {" "}, vastaukset);
    poista_valilyonnit_alusta_ja_lopusta(vastaukset.at(2));
    // Jos viimeisessä kentässä on vielä välilyonti, se tarkoittaa että
    // käyttäjä on syottänyt liian monta erillistä kenttää.
    if (vastaukset.at(2).find(" ") != string::npos) {
        virhe("Syotteessa ei ole tasan kolmea kenttaa!");
        return false;
    } if (onko_double_string(vastaukset.at(0)) or
            onko_double_string(vastaukset.at(2))){
        virhe("Syote ei ole muotoa reaaliluku yksikko yksikko!");
        return false;
    }
    lahtoyksikko = vastaukset.at(0);
    kohdeyksikko = vastaukset.at(2);
    return true;
}
