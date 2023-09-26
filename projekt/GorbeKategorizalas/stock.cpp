#include "stock.h"

bool jelentesBetoltes(string path, set<Negyed> &negyedevek, bool reset=true){

    string jelentesPath=path+"\\jelentes.txt";
    ifstream file(jelentesPath);
    if (!file.is_open()) return false;
    std::string line;
    if (reset)
        negyedevek.clear();
    //set<Negyed> negyedevek;
    while (getline(file,line)){
        if (benneVanAzStr(line,"fiscalDateEnding")){
            /// megtudja, melyik idõszakról van szó
            stringstream ss; ss<<line;
            string temp; ss>>temp;
            char c;ss>>c;
            int year=0, month=0, day=0;
            ss>>year>>c>>month>>c>>day;

            /// létrehozza a negyedévet, ha nem létezik, egyébként meg azt csinálja
            Negyed negyed(year,month,day);
            set<Negyed>::iterator it = negyedevek.find(negyed);
            if (it == negyedevek.end()) {
                negyedevek.insert(negyed);
                it = negyedevek.find(negyed);
            }

            /// bekéri az utáne lévõ adatot(okat)
            getline(file,line);
            if (benneVanAzStr(line,"reportedEPS")){
                /// éves jelentések (nem foglalkozok jelenleg vele)
                /*
                stringstream ss2; ss<<line;
                ss2>>temp>>c;
                float f; ss2>>f;
                const_cast<Negyed&>(*it).jelentettEPS=f;
                */
            }
            else if (benneVanAzStr(line,"reportedDate")){
                stringstream ss2; ss2<<line;
                float repEPS, predEPS, sup, supP;
                ss2>>temp>>c>>year>>c>>month>>c>>day;
                (*it).tenylegesJelentes=Datum(year,month,day);

                getline(file,line);
                ss2.str(line);
                ss2>>temp>>c>>repEPS;

                getline(file,line);
                ss2.str(line);
                ss2>>temp>>c>>predEPS;

                getline(file,line);
                ss2.str(line);
                ss2>>temp>>c>>sup;

                getline(file,line);
                ss2.str(line);
                ss2>>temp>>c>>supP;

                (*it).jelentettEPS=repEPS;
                (*it).becsultEPS=predEPS;
                (*it).meglepetes=sup;
                (*it).meglepetesSzazalek=supP;
            } else {
                cout<<"EPS hiba"<<endl;
            }

        }
    }

    file.close();
    return true;
}

bool bevetelBetoltes(string path, set<Negyed> &negyedevek, bool reset=true){
    string bevetelPath=path+"\\bevetel.txt";
    ifstream file(bevetelPath);
    if (!file.is_open()) return false;
    std::string line;
    if (reset)
        negyedevek.clear();
    string temp; char c;
    while (getline(file,line)) if (benneVanAzStr(line,"quarterlyReports")) break;
    while (getline(file,line)){
        if (benneVanAzStr(line,"fiscalDateEnding")){
            int year=0, month=0, day=0;
            stringstream ss; ss<<line;
            ss>>temp>>c>>year>>c>>month>>c>>day;
            Negyed negyed(year,month,day);
            set<Negyed>::iterator it = negyedevek.find(negyed);
            if (it == negyedevek.end()) {
                negyedevek.insert(negyed);
                it = negyedevek.find(negyed);
            }
            while (getline(file,line)){
                if (benneVanAzStr(line,"nonInterestIncome")){
                    long long ll;
                    ss.str(line);
                    ss>>temp>>c>>ll;
                    (*it).earn=ll;
                    break;
                }
            }
            while (getline(file,line)){
                if (benneVanAzStr(line,"netIncome")){
                    long long ll;
                    ss.str(line);
                    ss>>temp>>c>>ll;
                    (*it).income=ll;
                    break;
                }
            }
        }
    }
    file.close();
    return true;
}

bool napiBetoltes(string path, set<Nap> &osszesNap, bool reset=true){
    string honapokPath=path+"\\months";
    vector<string> fajlok = getFiles(honapokPath);
    if (reset)
        osszesNap.clear();

    for (int i=0; i<fajlok.size(); i++){
        string fileP = honapokPath+"\\"+fajlok[i];
        ifstream file(fileP);
        string temp, line;
        char c;
        getline(file,line);
        /// 2023-09-25 19:59:00,176.1000,176.2300,176.0800,176.1000,3368
        while(getline(file,line)){
            stringstream ss; ss<<line;
            int year=0, month=0, day=0, hour=0, minute=0;
            ss>>year>>c>>month>>c>>day>>hour>>c>>minute;
            ss>>c>>c>>c>>c;
            float open, maxi, mini, close, vol;
            ss>>open>>c>>maxi>>c>>mini>>c>>close>>c>>vol;
            Nap nap(year,month,day);
            set<Nap>::iterator it = osszesNap.find(nap);
            if (it == osszesNap.end()) {
                osszesNap.insert(nap);
                it = osszesNap.find(nap);
            }
            (*it).valid=true;
            (*it).minimum=min((*it).minimum,mini);
            (*it).maximum=max((*it).minimum,maxi);
            (*it).volumen+=vol;
            Arfolyam arf(hour,minute,open,close,mini,maxi,vol);
            set<Arfolyam>::iterator itP = (*it).percek.find(arf);
            if (itP == (*it).percek.end()) {
                (*it).percek.insert(arf);
                itP = (*it).percek.find(arf);
            }


            /// TODO
        }
    }

    /*
    ifstream file(jelentesPath);
    if (!file.is_open()) return false;
    std::string line;
    if (reset)
        osszesNap.clear();
    string temp; char c;
    while (getline(file,line)){

    }

    file.close();
    */
    return true;
}

bool Stock::adatokBetoltese(string stock){
    string path = gyoker + "\\stocks";
    if (!elemeAzStr(getSubdirectories(path),stock))
        return false;
    path = gyoker + "\\stocks\\"+stock;

    if (!jelentesBetoltes(path,negyedevek)) return false;
    if (!bevetelBetoltes(path,negyedevek,false)) return false;
    if (!napiBetoltes(path,mindenNap,true)) return false;

    return false;
}
