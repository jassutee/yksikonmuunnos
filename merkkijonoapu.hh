#ifndef MERKKIJONOAPU_HH
#define MERKKIJONOAPU_HH

#include <string>
#include <vector>

using namespace std;

double muuta_string_doubleksi(const string& mjono);
bool onko_double_string(const string& mjono);
string poista_valilyonnit(string& mjono);
string poista_valilyonnit_alusta_ja_lopusta(string& mjono);
bool paloittele_rivi(const string& mjono,
                     const vector<string>& erottimet,
                     vector<string>& kentat);


#endif // MERKKIJONOAPU_HH
