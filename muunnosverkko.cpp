/*  Määrittelytiedosto luokalle Muunnosverkko. */

#include "muunnosverkko.hh"
#include "merkkijonoapu.hh"
#include "apufunktiot.hh"

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;


// Rakentajafunktio. Ei tarvitse tehdä mitään.
Muunnosverkko::Muunnosverkko(){ }

// Tiedostonlukija. Saa nimen pääohjelmalta. Tekee virhetarkastelun tiedostolle
// ja kutsuu lisaa_muunnos-metodia.
bool Muunnosverkko::lue_tiedosto(const string& tiedoston_nimi) {
    ifstream tiedosto_olio{tiedoston_nimi};
    if (not tiedosto_olio) {
        tiedostovirhe("Tiedostoa ei saatu avattua!");
        return false;
    }
    string rivi{""};
    vector<string> kentat{};
    vector<string> erottimet{"<=", "*"};
    vector<double> muunnosvakiot{};
    double kerroin{0.0};
    /* Käydään tiedosto rivi riviltä läpi. Paloitellaan rivit, jotka eivät ala
    #-merkillä eivätkä ole tyhjiä, ja tarkistetaan, että ne ovat sallittua
    muotoa. Riviltä haetaan muistiin vain yksiköt, kerroin ja mahdollinen vakio.
    Luvut (kerroin ja vakio) tallennetaan vektoriin, jonka pituutta tarkastele-
    malla saadaan selville, onko kaavassa vakiota vai ei.*/
    while (getline(tiedosto_olio, rivi)) {
        poista_valilyonnit(rivi);
        if (rivi.front() == '#' or rivi.length() == 0) continue;
        if (rivi.find("-") != string::npos) {
            erottimet.push_back("-");
        } else if (rivi.find("+") != string::npos) {
            erottimet.push_back("+");
        }
        // Virhetarkastelua... Kaikki palauttavat käyttäjälle virheviestin ja
        // pääohjelmalle falsen.
        if (not paloittele_rivi(rivi, erottimet, kentat)) {
            tiedostovirhe("Tiedostossa vaaraa muotoa oleva rivi!");
            return false;
        } if (not onko_double_string(kentat.at(2)) or
                onko_double_string(kentat.at(0)) or
                onko_double_string(kentat.at(4))) {
            tiedostovirhe("Tiedostossa vaaraa muotoa oleva rivi!");
            return false;
        } if (kentat.at(0) == kentat.at(4)){
            tiedostovirhe("Tiedostossa virheellinen muunnos!");
            return false;
        }

        kerroin = muuta_string_doubleksi(kentat.at(2));
        muunnosvakiot.push_back(kerroin);

        if (kentat.size() == 7) {
            // Vielä yksi virhetarkastelu tälle erikoisriville.
            if (not onko_double_string(kentat.at(6))) {
                tiedostovirhe("Tiedostossa virheellinen rivi!");
                return false;
            }
            double vakio{muuta_string_doubleksi(kentat.at(6))};
            if (kentat.at(5) == "-") vakio = -vakio;
            muunnosvakiot.push_back(vakio);
        }
        lisaa_muunnos(kentat.at(0), kentat.at(4), muunnosvakiot);
        erottimet = {"<=", "*"};
        muunnosvakiot.clear();
    }
    tiedosto_olio.close();
    return true;
}

// Lisää uuden muunnosparin muunnokset_-mappiin. Eli sekä annetun muunnoksen,
// että sen käänteismuunnoksen. Jos kyseinen muunnos on jo mapissa, se korvataan
// uudella, eikä se tuota virhettä. Luokan tärkein tietorakenne muunnokset_ on
// muotoa           map< string, map< string, vector< double >>>.
void Muunnosverkko::lisaa_muunnos(const string &yksikko1, const string& yksikko2,
                                  const vector<double> &kaava) {
    // Luodaan käänteismuunnoksen kaava ja tietorakenteeseen tarvittavat mapit.
    vector<double> kaanteiskaava{luo_kaanteismuunnos(kaava)};
    map<string, vector<double>> yksikko1_pari{{yksikko2, kaava}};
    map<string, vector<double>> yksikko2_pari{{yksikko1, kaanteiskaava}};

    /* Selvitetään, ovatko lisättävät yksiköt jo valmiiksi mapissa. Helpoin
    tapaus on se, ettei ole, ja yksinkertaisesti vain lisätään muunnos
    ja käänteismuunnos. Jos vain toinen yksikkö löytyy, tiedetään, ettei
    niiden välillä voi olla vielä muunnosta, joten se ja toinen yksikkö
    lisätään. Jos molemmat yksiköt löytyvät, täytyy tarkistaa onko niillä
    jo muunnos keskenään. Muunnoksen löytyessä se korvataan uudella.*/
    if (muunnokset_.find(yksikko1) == muunnokset_.end() and
        muunnokset_.find(yksikko2) == muunnokset_.end()) { // Ei löydy
        muunnokset_.insert({yksikko1, yksikko1_pari});
        muunnokset_.insert({yksikko2, yksikko2_pari});
    } else if (muunnokset_.find(yksikko1) == muunnokset_.end()) {
        muunnokset_.insert({yksikko1, yksikko1_pari});
        muunnokset_.at(yksikko2).insert({yksikko1, kaanteiskaava});
    } else if (muunnokset_.find(yksikko2) == muunnokset_.end()) {
        muunnokset_.insert({yksikko2, yksikko2_pari});
        muunnokset_.at(yksikko1).insert({yksikko2, kaava});
    } else { // Kumpikin löytyy
        if (muunnokset_.at(yksikko1).find(yksikko2) ==
                muunnokset_.at(yksikko1).end()) { // Yhteistä muunnosta ei ole
            muunnokset_.at(yksikko1).insert({yksikko2, kaava});
            muunnokset_.at(yksikko2).insert({yksikko1, kaanteiskaava});
        } else { // Muunnos on jo, mutta se korvataan uudella
            muunnokset_.at(yksikko1).find(yksikko2) -> second = kaava;
            muunnokset_.at(yksikko2).find(yksikko1) -> second = kaanteiskaava;
        }
    }
}

// Laskee kaavalle käänteisen version ja palauttaa sen. Kerroin muutetaan
// käänteisluvuksi ja vakion vastaluku jaetaan kertoimella.
vector<double>
Muunnosverkko::luo_kaanteismuunnos(const vector<double> &kaava) const {
    vector<double> kaanteiskaava;
    double kerroin{kaava.at(0)};
    kaanteiskaava.push_back(1/kerroin);
    if (kaava.size() == 2) {
        double vakio{kaava.at(1)};
        vakio /= -kerroin;
        kaanteiskaava.push_back(vakio);
    }
    return kaanteiskaava;
}


// Tekee itse muunnoksen käyttäjän pyynnöstä. Lyhyin muunnosketju haetaan
// etsi_muunnos-metodista ja lasketaan tässä.
void Muunnosverkko::muunna(double &muuttuja, const string &lahtoyksikko,
                           const string &kohdeyksikko) const {
    vector<string> muunnosketju{etsi_muunnos(lahtoyksikko, kohdeyksikko)};
    // Virhetilanteessa etsi_muunnos palauttaa tyhjän vektorin.
    if (muunnosketju.size() == 0) {
        return;
    }
    /* Muunnosketju käydään läpi while-loopilla. Koska vektori on kohdeyksiköstä
    lähtöyksikköön, voidaan sijoitukset tehdä suoraan ja joka kierroksen lopussa
    poistaa viimeinen alkio. Muuttuja on siis vektorin viimeistä yksikköä ja se
    muutetaan aina viimeistä edeltäväksi yksiköksi. Toiseksi viimeisen hakuavai-
    mella löytyvästä mapista löytyy viimeisen hakuavaimella tarvittava kaava.*/
    vector<double> kaava;
    string nykyinen;
    string seuraava;
    while (muunnosketju.size() != 1) {
        nykyinen = muunnosketju.at(muunnosketju.size()-1);
        seuraava = muunnosketju.at(muunnosketju.size()-2);
        kaava = muunnokset_.at(seuraava).at(nykyinen);
        muuttuja *= kaava.at(0);
        if (kaava.size() == 2) {
            muuttuja += kaava.at(1);
        }
        muunnosketju.pop_back();
    }
}


// Etsii ja palauttaa muunnosreitin. Reitti tallennetaan vektoriin takaperin
// (kohdeyksiköstä lähtöyksikköön) helpomman käsittelyn ja kaavojen muodon
// vuoksi. Jos reittiä ei löydy, vektori palautetaan tyhjänä.
vector<string> Muunnosverkko::etsi_muunnos(const string &lahtoyksikko,
                                           const string &kohdeyksikko) const {
    vector<string> reitti{};
    // Virhetilanteessa palautetaan tyhjä reitti.
    if (muunnokset_.find(lahtoyksikko) == muunnokset_.end() or
        muunnokset_.find(kohdeyksikko) == muunnokset_.end()) {
        virhe("Molempia yksiköitä ei löytynyt!");
        return reitti;
    }
    /* Reitti etsitään käymällä läpi mappia muunnokset_. Läpikäynti aloitetaan
    kohdeyksiköstä alkaen kohti lähtöyksikköä. Ensin käydään läpi kaikki
    kohdeyksikölle löytyvät parit ja tallennetaan ne muistiin reitti-vektoriin.
    Sitten käydään kaikki niiden parit jne. Takaisinpäin meneminen estetään
    tarkistamalla joka kierroksella, että onko kyseinen yksikkö jo tallennettuna
    reitti-vektoriin. Vektoriin tulee näin ylimääräisiä yksiköitä, jotka
    poistetaan myöhemmin.*/
    reitti.push_back(kohdeyksikko);
    int lapikaydyt{0};
    // r_iter = reitti-iteraattori
    vector<string>::iterator r_iter{nullptr};
    while (find(reitti.begin(), reitti.end(), lahtoyksikko) == reitti.end()) {
        /* Vektori muuttuu tämän while-loopin sisällä, joten iteraattori
        joudutaan alustamaan aina uudestaan, ettei se hajoaisi. lapikaydyt
        pitää kirjaa siitä kuinka pitkällä ollaan. advance:n avulla voidaan
        siirtyä suoraan seuraavaan alkioon eikä tarvitse käydä turhia
        kierroksia loopissa. */
        r_iter = reitti.begin();
        advance(r_iter, lapikaydyt);
        for (auto muunnoskaava: muunnokset_.at(*r_iter)) {
            if (find(reitti.begin(), reitti.end(), muunnoskaava.first) ==
                                                            reitti.end()) {
                reitti.push_back(muunnoskaava.first);
                if (muunnoskaava.first == lahtoyksikko) break;
            }
        }
        ++r_iter;
        if (r_iter == reitti.end()) break;
        ++lapikaydyt;
    }
    // Virhetarkastelu tilanteelle, kun kyseistä muunnosta ei ole. Esim. jos
    // yrittää muuntaa cm -> kg: reittiä ei pitäisi löytyä ja päädytään tähän.
    if (find(reitti.begin(), reitti.end(), lahtoyksikko) == reitti.end()) {
        virhe("Tuntematon muunnos!");
        reitti.clear();
        return reitti;
    }
    /* Nyt käydään reitti läpi lopusta alkuun päin ja poistetaan ylimääräiset
    yksiköt välistä. Apumuuttujaan pituus tallennetaan yksiköiden määrä, jota
    voidaan hyödyntää etäisyyksien selvittämisessä. Vektorista etsitään lopusta
    alkaen, mitkä liittyvät viimeiseen, eli lähtöyksikköön, ja kaikki turhat
    välissä olevat yksiköt poistetaan.*/
    r_iter = reitti.end()-1;
    string::size_type pituus{1};
    while (true) {
        if (muunnokset_.at(reitti.at(reitti.size()-pituus)).find(*r_iter) !=
                muunnokset_.at(reitti.at(reitti.size()-pituus)).end()) {
            reitti.erase(r_iter+1, reitti.end()-pituus);
            if (*r_iter == kohdeyksikko) break;
            ++pituus;
        }
        --r_iter;
    }
    return reitti;
}

