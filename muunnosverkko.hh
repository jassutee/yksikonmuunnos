/*  Esittelytiedosto luokalle Muunnosverkko, johon
    lisätään kaikki tiedostosta luetut muunnokset.
    Luokassa olevia muunnoksia voi sitten käsitellä
    muualla ohjelmassa. */

#ifndef MUUNNOSVERKKO_HH
#define MUUNNOSVERKKO_HH

#include <string>
#include <vector>
#include <map>

using namespace std;
// kaava sisältää kertoimen ja mahdollisesti ynnättävän luvun.
using kaava = vector<double>;
using yksikko = string;

class Muunnosverkko {

public:
    Muunnosverkko();
    // Tiedostonlukuun liittyvät metodit:
    bool lue_tiedosto(const string& tiedoston_nimi);
    void lisaa_muunnos(const string& yksikko1, const string& yksikko2,
                       const vector<double>& kaava);
    vector<double> luo_kaanteismuunnos(const vector<double>& kaava) const;
    // Käyttäjän pyytämään muunnokseen liittyvät metodit:
    void muunna(double& muuttuja, const string& lahtoyksikko,
                  const string& kohdeyksikko) const;
    vector<string> etsi_muunnos(const string& lahtoyksikko,
                                const string& kohdeyksikko) const;

private:
    map<yksikko, map<yksikko, kaava>> muunnokset_;

};

#endif // MUUNNOSVERKKO_HH
