#include "lekerdezes.h"

using namespace std;

void Lekerdezes::process(){
    joPeldak.clear();
    for (int i=0; i<(int)reszvenyek.size(); i++){
        Stock stock; bool nap = true; bool b;
        loadStock(reszvenyek[i],ref(stock),ref(b));
        vector<set<Negyed>::iterator> vizsgaltNegyedek;
        vector<set<Nap>::iterator> vizsgaltNapok;
        /// azaz ha van egy negyed is, akkor ahhoz igazodik
        vector<int> napXek, negyedXek;
        for (int j=0; j<feltetelek.size(); j++){
            if (!feltetelek[j].nap){
                nap=false;
                set<Negyed>::iterator it = stock.negyedevek.begin();
                negyedXek.push_back(feltetelek[j].hanyadik);
                if (feltetelek[j].komper) negyedXek.push_back(feltetelek[j].hanyadikHoz);
                vizsgaltNegyedek.push_back(it);
            } else {
                set<Nap>::iterator it = stock.mindenNap.begin();
                napXek.push_back(feltetelek[j].hanyadik);
                if (feltetelek[j].komper) napXek.push_back(feltetelek[j].hanyadikHoz);
                vizsgaltNapok.push_back(it);
            }
        }
        /// van a részvényekre most már elegendõ mutatónk
        /// kéne a mutatókat rendezni (pl ne legyen negatív)
        int naXPMax=-999, naXPMin=999, neXPMax=-999, neXPMin=999;
        int negyedIdx=0;
        for (int j=0; j<napXek.size(); j++){
            naXPMax=max(naXPMax,napXek[j]);
            naXPMin=min(naXPMin,napXek[j]);
        }
        for (int j=0; j<negyedXek.size(); j++){
            neXPMax=max(neXPMax,negyedXek[j]);
            if (min(neXPMin,negyedXek[j])<neXPMin){
                neXPMin=min(neXPMin,negyedXek[j]);
                negyedIdx=j; /// érdekes, hogy melyik a 0. negyed
                                /// ahhoz igazítjuk a napokat
            }
        }
        if (negyedXek.size()==0){ /// ha nincsenek negyedek, akkor nem negatív
            for (int j=0; j<vizsgaltNapok.size(); j++){
                napXek[j]-=naXPMin;
                for (int k=0; k<napXek[j]; k++)
                    if (vizsgaltNapok[j]!=stock.mindenNap.end())
                        vizsgaltNapok[j]++;
            }
        }
        else {  /// különben a napok maradnak, a negyedek viszont nem negatív
            for (int j=0; j<vizsgaltNegyedek.size(); j++){
                negyedXek[j]-=neXPMin;
                for (int k=0; k<negyedXek[j]; k++)
                    if (vizsgaltNegyedek[j]!=stock.negyedevek.end())
                        vizsgaltNegyedek[j]++;
            }
        }


    }
}

void lekerdezesProc(Lekerdezes lek, vector<Pelda> &peldak){
    lek.process();
    peldak=lek.joPeldak;
}
