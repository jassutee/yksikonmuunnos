/*  Esittelytiedosto pääohjelman perusapufunktioille. */

#ifndef APUFUNKTIOT_HH
#define APUFUNKTIOT_HH

#include <string>

using namespace std;

string kysy(const string& kysymys);
void virhe(const string& syote);
void tiedostovirhe(const string& syote);
bool vastaus_oikeaan_muotoon(string& vastaus, double& muuttuja,
                             string& lahtoyksikko, string& kohdeyksikko);

#endif // APUFUNKTIOT_HH
