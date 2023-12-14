#include "lekerdezes.h"

using namespace std;

void Lekerdezes::lStock(int reszIdx){
    //cout<<reszIdx<<" "<<reszvenyek[reszIdx]<<endl;
    loadStock(reszvenyek[reszIdx],ref(stock),ref(bStock));
}

void Lekerdezes::feltetelekLekFeltek(){
    for (int j=0; j<feltetelek.size(); j++){
        LekFelt lkf; lkf.feltetelIdx=j;
        if(feltetelek[j].nap){
            lkf.day=true;
            lkf.index1=feltetelek[j].hanyadik; lkf.nap=stock.mindenNap.begin();
            vector<int>idx;
            idx.push_back(lkf.index1);
            if (feltetelek[j].komper){
                lkf.index2=feltetelek[j].hanyadikHoz; lkf.komper=true;
                lkf.naphoz=stock.mindenNap.begin();
                idx.push_back(lkf.index2);
            }
            napXek.push_back(idx);
            vizsgaltNap.push_back(lkf);
        } else if (feltetelek[j].negyed) {
            lkf.day=false;
            lkf.index1=feltetelek[j].hanyadik; lkf.negyed=stock.negyedevek.begin();
            vector<int>idx;
            idx.push_back(lkf.index1);
            if (feltetelek[j].komper){
                lkf.index2=feltetelek[j].hanyadikHoz; lkf.komper=true;
                lkf.negyedhez=stock.negyedevek.begin();
                idx.push_back(lkf.index2);
            }
            csakNapok=false;
            negyedXek.push_back(idx);
            vizsgaltNegy.push_back(lkf);
        }
    }
}

void Lekerdezes::elemezendoLekFeltekHozzaadasa(){
    for (int j=0; j<elemezendoek.size(); j++){
        LekFelt lkf; lkf.elemezendoIdx=j; lkf.felt=false;
        if(elemezendoek[j].nap){
            lkf.day=true;
            lkf.index1=elemezendoek[j].hanyadik; lkf.nap=stock.mindenNap.begin();
            vector<int>idx;
            idx.push_back(lkf.index1);
            if (elemezendoek[j].komper){
                lkf.index2=elemezendoek[j].hanyadikHoz; lkf.komper=true;
                lkf.naphoz=stock.mindenNap.begin();
                idx.push_back(lkf.index2);
            }
            napXek.push_back(idx);
            vizsgaltNap.push_back(lkf);
        } else if (elemezendoek[j].negyed) {
            lkf.day=false;
            lkf.index1=elemezendoek[j].hanyadik; lkf.negyed=stock.negyedevek.begin();
            vector<int>idx;
            idx.push_back(lkf.index1);
            if (elemezendoek[j].komper){
                lkf.index2=elemezendoek[j].hanyadikHoz; lkf.komper=true;
                lkf.negyedhez=stock.negyedevek.begin();
                idx.push_back(lkf.index2);
            }
            csakNapok=false;
            negyedXek.push_back(idx);
            vizsgaltNegy.push_back(lkf);
        }
    }
}

void Lekerdezes::eltolasMertekeLekFelt(){
    /// ha van egy negyed is, akkor ahhoz igazodik
    /// van a r�szv�nyekre most m�r elegend� mutat�nk
    /// k�ne a mutat�kat rendezni (pl ne legyen negat�v)
    naXPMax=-999; naXPMin=999; neXPMax=-999; neXPMin=999;
    for (int j=0; j<napXek.size(); j++){
        naXPMax=max(naXPMax,napXek[j][0]);
        naXPMin=min(naXPMin,napXek[j][0]);
        if (vizsgaltNap[j].komper){
            naXPMax=max(naXPMax,napXek[j][1]);
            naXPMin=min(naXPMin,napXek[j][1]);
        }
    }
    for (int j=0; j<negyedXek.size(); j++){
        neXPMax=max(neXPMax,negyedXek[j][0]);
        neXPMin=min(neXPMin,negyedXek[j][0]);
        if (negyedXek[j][0]==0) {nullNegyedIdx=j; hez=false;}
        if (vizsgaltNegy[j].komper){
            neXPMax=max(neXPMax,negyedXek[j][1]);
            neXPMin=min(neXPMin,negyedXek[j][1]);
            if (negyedXek[j][1]==0) {nullNegyedIdx=j; hez=true;}
        }
    }
}

void Lekerdezes::eltolasLekFelt(){
    /// mindegyiket 0-hoz k�zel�tj�k
    if (negyedXek.size()!=0){
        if (neXPMin>0){ /// ha nincs nulladik negyed, akkor hozz�adunk egyet
            LekFelt lkf; lkf.felt=false; lkf.day=false;
            lkf.index1=0; lkf.negyed=stock.negyedevek.begin();
            nullNegyedIdx=vizsgaltNegy.size();
            vizsgaltNegy.push_back(lkf);
        } /// ehhez fogjuk a napokat rendelni
        else if (neXPMin<0) {
            for (int i=0; i<vizsgaltNegy.size(); i++){
                int eltolas = -neXPMin;
                vizsgaltNegy[i].index1+=eltolas;
                if (vizsgaltNegy[i].komper) vizsgaltNegy[i].index2+=eltolas;
            }
        }
    }
    else {
        for (int i=0; i<vizsgaltNap.size(); i++){
            int eltolas = -naXPMin;
            vizsgaltNap[i].index1+=eltolas;
            if (vizsgaltNap[i].komper) vizsgaltNap[i].index2+=eltolas;
        }
    }
    /// m�r egyik sem negat�v
}

bool Lekerdezes::tenylegesEltolasLekFelt(){
    bool baj = false;
    if (vizsgaltNegy.size()!=0){
        for (int i=0; i<vizsgaltNegy.size(); i++){
            for (int k=0; k<vizsgaltNegy[i].index1;k++){
                vizsgaltNegy[i].negyed++;
                if (vizsgaltNegy[i].negyed==stock.negyedevek.end()) {
                    baj=true;
                    break;
                }
            }
            for (int k=0; vizsgaltNegy[i].komper && k<vizsgaltNegy[i].index2;k++){
                vizsgaltNegy[i].negyedhez++;
                if (vizsgaltNegy[i].negyedhez==stock.negyedevek.end()) {
                    baj=true;
                    break;
                }
            }
        }
    }
    else {
        for (int i=0; i<vizsgaltNap.size(); i++){
            for (int k=0; k<vizsgaltNap[i].index1;k++){
                vizsgaltNap[i].nap++;
                if (vizsgaltNap[i].nap==stock.mindenNap.end()) {
                    baj=true;
                    break;
                }
            }
            for (int k=0; vizsgaltNap[i].komper && k<vizsgaltNap[i].index2;k++){
                vizsgaltNap[i].naphoz++;
                if (vizsgaltNap[i].naphoz==stock.mindenNap.end()) {
                    baj=true;
                    break;
                }
            }
        }
    }
    return baj;
}

void Lekerdezes::process(){
    joPeldak.clear();
    for (int i=0; i<vizsgaltNap.size(); i++){

    }

        ///cout<<"e"<<endl;
        /// m�r minden b�bu a hely�n
        /// kezd�thet a vizsg�l�d�s


    //}
}


void Lekerdezes::naposVizsgalat(){
    joPeldak.clear();
    set<Nap>::iterator endIt = stock.mindenNap.end();
    while (true){
        Pelda pelda; pelda.reszNev=stock.name;
        Cimke* c;
        bool baj = false; /// ha az egyik felt�tel nem teljes�l
        bool datumError = false;
        Datum nap;
        Datum naphoz;
        for (int i=0; i<vizsgaltNap.size(); i++){
            nap = vizsgaltNap[i].nap->datum;
            if (vizsgaltNap[i].komper) naphoz = vizsgaltNap[i].naphoz->datum;
            vector<Datum> ketNap; ketNap.push_back(nap);
            if (vizsgaltNap[i].komper) ketNap.push_back(naphoz);
            for (int k=0; k<ketNap.size(); k++){ /// ha b�rmelyik nap
                if (ketNap[k]<mettol || meddig<ketNap[k]){ /// a megadott intervallumon k�v�l van
                    baj=true; /// akkor az szint�n rossz p�lda
                    break;
                }
            }
            if (baj) {
                datumError=true;
                //cout<<"nap hiba "<<nap.year<<" "<<nap.month<<" "<<nap.day<<endl;
                break;
            }
            vector<Cimke*> cimkek;
            if (vizsgaltNap[i].felt) cimkek = feltetelek[vizsgaltNap[i].feltetelIdx].cimkek;
            else cimkek = elemezendoek[vizsgaltNap[i].elemezendoIdx].cimkek;

            vector<Ertek> elemezendoekB;
            for (int k=0; k<cimkek.size(); k++){ /// a felt�tel minden c�mk�j�n v�gigmegy
                Ertek ertek; ertek.cimkeNeve=cimkek[k]->name; /// ha elemezend� akkor lesz �tr�k
                if (cimkek[k]->check(stock,nap,naphoz)) { /// ha teljes�l a c�mke
                    if (!vizsgaltNap[i].felt) { /// �s ez egy elemezend�
                        ertek.siker=true; elemezendoekB.push_back(ertek); /// akkor �rt�ke igaz �s push_back
                    } else {
                        pelda.felteteliDatumok.push_back(ketNap); /// k�l�nben, hogy melyik napokr�l besz�l�nk
                    }
                    continue; /// j�het a k�vetkez� c�mke
                } /// k�l�nben ha felt�tel, akkor nem lesz p�lda �s �lljon le a ciklus
                if (vizsgaltNap[i].felt) {baj=true; break;}
                else { /// ha csak elemezend�, akkor csak elmentj�k, hogy ez negat�v
                    ertek.siker=false; elemezendoekB.push_back(ertek);
                }
            }
            /// mikor v�gig n�zt�k az adott d�tum(ok)ra az �sszes hozz�tartoz� c�mk�t
            if (baj) break; /// akkor ha elakadtunk valamelyik c�mk�n�l, akkor nem kell vizsg�lni a tov�bbi felt�teleket
            /// k�l�nben kell �s a tal�lt
            pelda.datumok.push_back(ketNap);
            pelda.elemezendoekTeljesulese.push_back(elemezendoekB);
        }

        if (!baj) {
            joPeldak.push_back(pelda);
        } else if (!datumError && false){
            cout<<"nem teljesul a feltetel "<<nap.year<<" "<<nap.month<<" "<<nap.day<<endl;
            cout<<vizsgaltNap[0].feltetelIdx<<endl;
            cout<<feltetelek[vizsgaltNap[0].feltetelIdx].hanyadik<<" "<<feltetelek[vizsgaltNap[0].feltetelIdx].nap<<endl;
            cout<<feltetelek[vizsgaltNap[0].feltetelIdx].cimkek[0]->name<<" "<<feltetelek[vizsgaltNap[0].feltetelIdx].cimkek[0]->IDname<<endl;
        }

        for (int i=0; i<vizsgaltNap.size(); i++){ /// l�p�nk egyet el�re a napokon
            vizsgaltNap[i].nap++;
            if (vizsgaltNap[i].komper) vizsgaltNap[i].naphoz++;
            if (vizsgaltNap[i].nap==endIt) return;
            if (vizsgaltNap[i].komper && vizsgaltNap[i].naphoz==endIt) return;
        }
    }
}

void Lekerdezes::negyedevesVizsgalat(){
    joPeldak.clear();
    for (int i=0; i<vizsgaltNap.size(); i++){

    }
}

void lekProc(Lekerdezes lek, int i, vector<Pelda> &peldak){
    clock_t t = clock();
    cout<<"start ido2: "<<clock()<<endl;
    lek.lStock(i);
    cout<<"eltelt ido2: "<<clock()-t<<endl;
    lek.feltetelekLekFeltek();
    lek.elemezendoLekFeltekHozzaadasa();
    lek.eltolasMertekeLekFelt();
    lek.eltolasLekFelt();
    if (lek.tenylegesEltolasLekFelt()) return;
    if (lek.csakNapok) lek.naposVizsgalat();
    else lek.negyedevesVizsgalat();
    //lek.process();
    cout<<"eltelt ido3: "<<clock()-t<<endl;
    peldak = lek.joPeldak;
}

void lekerdezesProc(Lekerdezes lek, vector<Pelda> &peldak, ProgressBar &progBar){
    clock_t t = clock();
    int thCnt = 16;
    vector<thread> szalak; szalak.resize(thCnt);
    vector<vector<Pelda>> joPeldak; joPeldak.resize(thCnt);
    cout<<"start ido: "<<clock()<<endl;
    for (int i=0; i<lek.reszvenyek.size();){
        int maxJ = 0;
        for (int j=0; j<szalak.size() && i<lek.reszvenyek.size();j++){
            szalak[j] = thread(lekProc,lek,i,ref(joPeldak[j]));
            i++;
            maxJ=j;
        }
        for (int j=0; j<szalak.size() && j<=maxJ;j++){
            szalak[j].join();
            std::copy(joPeldak[j].begin(), joPeldak[j].end(), std::back_inserter(lek.joPeldak));
            progBar.elemFeldolgozva(1);
        }
    }
    //lek.process();
    peldak=lek.joPeldak;
    cout<<"eltelt ido: "<<clock()-t<<endl;

    for (int i=0; i<peldak.size() && false; i++){
        Datum d = peldak[i].felteteliDatumok[0][0];
        d = peldak[i].datumok[0][0];
         cout<<peldak[i].reszNev<<" "<<d.year<<" "<<d.month<<" "<<d.day<<endl;
        /*
        for (int j=0; j<peldak[i].size(); j++){

        }*/
    }
}
