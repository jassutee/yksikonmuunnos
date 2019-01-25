// Muunnosohjelma. Tehnyt Jasmin Tuominen, 226329.
// Valmistunut 2.1.2015 klo 02:07.

//*************************************************//
// Älä muuta rivejä, jotka ovat tämän rivin        //
                                                   //
   #ifdef SOURCEDIR                                //
     #include <unistd.h>                           //
     const int cd_status{ chdir(SOURCEDIR) };      //
   #endif                                          //
                                                   //
// ja tämän rivin välissä. Nämä koodirivit         //
// liittyvät siihen, että ohjelmaa suoritettaessa  //
// sen työkansio on sama kansio, jossa lähdekoodi  //
// sijaitsee: jos testisyötetiedostot ovat samassa //
// kansiossa, ne voi avata luettavaksi pelkällä    //
// tiedoston nimellä, kuten luentomonisteessa on   //
// kuvattu.  Tätä ratkaisua ei ole testattu kuin   //
// virtuaalityöpöydän Linuxissa: muissa järjestel- //
// missä toiminnalle ei anneta takuuta!            //
//*************************************************//

/* Pääohjelma.*/

#include "apufunktiot.hh"
#include "muunnosverkko.hh"

#include <iostream>
#include <iomanip>

using namespace std;


int main() {
    Muunnosverkko muunnosverkko;
    string tiedosto{""};
    tiedosto = kysy("Syota saantotiedoston nimi: ");

    if (not muunnosverkko.lue_tiedosto(tiedosto)) return false;

    string vastaus{""};
    double muuttuja{0.0};
    string lahtoyksikko{""};
    string kohdeyksikko{""};
    while(true) {
        vastaus = kysy("muunnospyynto> ");
        if (vastaus.length() == 0) return false;
        if (not vastaus_oikeaan_muotoon(vastaus, muuttuja,
                                        lahtoyksikko, kohdeyksikko)) {
            continue;
        }
        muunnosverkko.muunna(muuttuja, lahtoyksikko, kohdeyksikko);
        cout << setprecision(3) << fixed << muuttuja << endl;

    }
}
