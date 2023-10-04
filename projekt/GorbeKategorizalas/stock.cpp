#include "stock.h"

#include <windows.h>
#include <iostream>
#include <vector>
#include <cctype> // isdigit, isspace
#include <cstdlib> // strtol, strtof

int npB(string path, set<Nap> &osszesNap, bool reset=true){
    // Fájl megnyitása CHAT GPT
    cout<<path<<endl;
    HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Nem sikerült megnyitni a fájlt" << std::endl;
        return 1;
    }

    // Fájl méretének lekérdezése
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        std::cerr << "Nem sikerült lekérdezni a fájl méretét" << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    // Fájl memóriába mappelése
    HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (hMap == NULL) {
        std::cerr << "Nem sikerült létrehozni a memóriatérképet" << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    char* addr = static_cast<char*>(MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0));
    if (addr == NULL) {
        std::cerr << "Nem sikerült betölteni a fájlt a memóriába" << std::endl;
        CloseHandle(hMap);
        CloseHandle(hFile);
        return 1;
    }

    // Beolvasott értékek tárolása
    std::vector<int> intValues;
    std::vector<float> floatValues;

    // Fájl tartalmának feldolgozása és értékek kinyerése
    char* end;
    int fieldCount = 0;
    for (char* ptr = addr; ptr < addr + fileSize; ++ptr) {
        // Szám elejének keresése
        if (std::isdigit(*ptr) || *ptr == '-' || *ptr == '+') {
            if (fieldCount < 5) {
                int value = std::strtol(ptr, &end, 10);
                intValues.push_back(value);
            } else {
                float value = std::strtof(ptr, &end);
                floatValues.push_back(value);
                if (fieldCount == 9) {
                    fieldCount = -1; // Reset field count for next row
                }
            }
            ptr = end;
            fieldCount++;
        }
    }

    // Memória térkép eltávolítása és fájl bezárása
    UnmapViewOfFile(addr);
    CloseHandle(hMap);
    CloseHandle(hFile);

    for (size_t i=0; i<intValues.size()/5;i++){
        int intChange=i*5, floatChange=i*5;

        Nap nap(intValues[intChange+0],intValues[intChange+1],intValues[intChange+2]);
        set<Nap>::iterator it = osszesNap.find(nap);
        if (it == osszesNap.end()) {
            osszesNap.insert(nap);
            it = osszesNap.find(nap);
        }

        Arfolyam arf(intValues[intChange+3],intValues[intChange+4],floatValues[floatChange+0],floatValues[floatChange+1],floatValues[floatChange+2],floatValues[floatChange+3],floatValues[floatChange+4]);
        set<Arfolyam>::iterator itP = (*it).percek.find(arf);
        if (itP == (*it).percek.end()) {
            (*it).percek.insert(arf);
            itP = (*it).percek.find(arf);
        }
    }


    return 0;
}

void loadStock(string name, Stock &stock){
    stock.adatokBetoltese(name);
    stock.adatokFeldolgozasa();
}

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
                if (benneVanAzStr(line,"totalRevenue")){
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

    for (size_t i=0; i<fajlok.size(); i++){
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
            string ommcv; ss>>ommcv;

            vector<string> zzz;
            string s;
            stringstream ss2(ommcv);
            while (getline(ss2, s, ',')) {
                // store token string in the vector
                zzz.push_back(s);
            }
            float open=stof(zzz[0]), maxi=stof(zzz[1]), mini=stof(zzz[2]), close=stof(zzz[3]), vol=stof(zzz[4]);
            Nap nap(year,month,day);
            set<Nap>::iterator it = osszesNap.find(nap);
            if (it == osszesNap.end()) {
                osszesNap.insert(nap);
                it = osszesNap.find(nap);
            }
            (*it).valid=true;
            (*it).minimum=min((*it).minimum,mini);
            (*it).maximum=max((*it).maximum,maxi);
            (*it).volumen+=vol;
            Arfolyam arf(hour,minute,open,close,mini,maxi,vol);
            set<Arfolyam>::iterator itP = (*it).percek.find(arf);
            if (itP == (*it).percek.end()) {
                (*it).percek.insert(arf);
                itP = (*it).percek.find(arf);
            }
        }
        file.close();
    }


    return true;
}

bool napiBetoltes2(string path, set<Nap> &osszesNap, bool reset=true){
    string honapokPath=path+"\\months";
    vector<string> fajlok = getFiles(honapokPath);
    if (reset)
        osszesNap.clear();

    /*
    for (int i=0; i<fajlok.size(); i++){
        npB(honapokPath+"\\"+fajlok[i],osszesNap,false);
    }
    */
    npB("AMDminutes.txt",osszesNap,false);

    return true;
}

bool Stock::adatokBetoltese(string stock){
    name=stock;
    string path = Config.getRootDirectory() + "stocks";
    if (!elemeAzStr(getSubdirectories(path),stock))
        return false;
    path = Config.getRootDirectory() + "stocks\\"+stock;

    clock_t t = clock();
    if (!jelentesBetoltes(path,negyedevek)) return false;
    cout<<(clock()-t)<<endl;
    t=clock();
    if (!bevetelBetoltes(path,negyedevek,false)) return false;
    cout<<(clock()-t)<<endl;
    t=clock();
    if (!napiBetoltes(path,mindenNap,true)) return false;
    cout<<(clock()-t)<<endl;
    t=clock();

    return true;
}

bool Stock::adatokBetoltese2Teszt(string stock){
    name=stock;
    string path = Config.getRootDirectory() + "\\stocks";
    if (!elemeAzStr(getSubdirectories(path),stock))
        return false;
    path = Config.getRootDirectory() + "\\stocks\\"+stock;

    clock_t t = clock();
    if (!jelentesBetoltes(path,negyedevek)) return false;
    cout<<(clock()-t)<<endl;
    t=clock();
    if (!bevetelBetoltes(path,negyedevek,false)) return false;
    cout<<(clock()-t)<<endl;
    t=clock();
    if (napiBetoltes2(path,mindenNap,true)) return false;
    cout<<(clock()-t)<<endl;
    t=clock();

    return true;
}

void Stock::adatokKiirasaFajlba (string fajlNev){
    ofstream file(fajlNev+"earnings.txt");

    file<<"datum, tenyeleges datum, jelentettEPS, becsultEPS, meglepetes, meglepetesSzazalek, earn, income"<<endl;
    for (const auto& elem : negyedevek) {
        file << elem.idoszakVege.year << ' ';
        file << elem.idoszakVege.month << ' ';
        file << elem.idoszakVege.day << " \t";
        file << elem.tenylegesJelentes.year << ' ';
        file << elem.tenylegesJelentes.month << ' ';
        file << elem.tenylegesJelentes.day << " \t";
        file << elem.jelentettEPS << " \t";
        file << elem.becsultEPS << " \t";
        file << elem.meglepetes << " \t";
        file << elem.meglepetesSzazalek << "\t";
        file << elem.earn << " \t";
        file << elem.income << " \t";
        file << endl;
    }
    file << endl;
    file.close();


    ofstream file2(fajlNev+"minutes.txt");
    file2<<"datum, idopont, nyitás, zárás, minimum, maximum, volumen"<<endl;
    for (const auto& elem : mindenNap) {
        for (const auto& elem2 : elem.percek){
            file2 << endl;
            file2 << elem.datum.year << ' ';
            file2 << elem.datum.month << ' ';
            file2 << elem.datum.day << " \t";
            file2 << elem2.idopont.ora << ' ';
            file2 << elem2.idopont.perc << " \t";
            file2 << elem2.open << " \t";
            file2 << elem2.close << " \t";
            file2 << elem2.minimum << " \t";
            file2 << elem2.maximum << " \t";
            file2 << (long long)elem2.volumen;
        }
    }
    file2.close();
}


void Stock::adatokFeldolgozasa(){

    Nap ntm;
    Arfolyam itm;
    const Nap *legutobbiNap = &ntm;
    for (const Nap& nap : mindenNap) {

        /// ha létezik az előző nap
        if (legutobbiNap->datum.year>1800){ /// akkor összelinkelem őket
            nap.elozoNap=legutobbiNap->datum;
            legutobbiNap->kovetkezoNap=nap.datum;
        }

        /// beállítom statikusan a nyitást zárást
        nap.idoNyitas=Idopont(9,30);
        nap.idoZaras=Idopont(15,59);
        nap.valid=true;

        /// és lekérdezem, hogy a héz melyik napja
        nap.hetMelyikNapja=hetNapja(nap.datum);

        const Arfolyam *legutobbiPerc = &itm;
        bool ny=false, z=false;
        vector<Arfolyam> pluszPercek;

        for (const Arfolyam& perc : nap.percek){
            /// amennyiben van előző perc
            if (legutobbiPerc->idopont.ora>2){
                /// és elmúlt a nyitás vagy a zárás
                if (!(perc.idopont<nap.idoNyitas) && !ny){
                    if (legutobbiPerc->idopont<nap.idoNyitas)
                        nap.nyitas=legutobbiPerc->close;
                    else
                        nap.nyitas=legutobbiPerc->open;
                    ny=true;
                }
                if (!(perc.idopont<nap.idoZaras) && !z){
                    nap.zaras=legutobbiPerc->close;
                    z=true;
                }
            }


            /// ha nyitva van a piac, a napi adatot tornáztatjuk
            Idopont zarasP = nap.idoZaras; zarasP.kovetkezoPerc(); /// hoz az 15:59 is benne legyen
            if (!(perc.idopont<nap.idoNyitas) && (perc.idopont<nap.idoZaras)){
                nap.minimum=min(nap.minimum,perc.minimum);
                nap.maximum=max(nap.maximum,perc.maximum);
                nap.volumen+=perc.volumen;
            }


            /// hiányzó percek pótlása
            if (legutobbiPerc->idopont.ora>2){ /// ha valid az utolsó perc
                if (legutobbiPerc->idopont<nap.idoZaras && nap.idoNyitas<perc.idopont){ ///
                    int elteltPercek = perc.idopont-legutobbiPerc->idopont;
                    if (elteltPercek>1){
                        Idopont utobbiP = legutobbiPerc->idopont;
                        for (int i=1; i<elteltPercek; i++){
                            utobbiP.kovetkezoPerc();
                            Arfolyam temp(utobbiP.ora,utobbiP.perc,legutobbiPerc->close,legutobbiPerc->close,legutobbiPerc->close,legutobbiPerc->close,0);
                            pluszPercek.push_back(temp);
                        }
                    }
                }
            }
            else if (nap.idoNyitas<perc.idopont){ /// ha nem, de a jelenlegi már nyitáson túl van
                Idopont tempI = nap.idoNyitas;
                int kulonb = perc.idopont-nap.idoNyitas;
                for (int i=0; i<kulonb; i++){
                    Arfolyam tempA(tempI.ora,tempI.perc,perc.open,perc.open,perc.open,perc.open,0);
                    pluszPercek.push_back(tempA);
                    tempI.kovetkezoPerc();
                }
            }

            legutobbiPerc=&perc;
        }

        /// he zárás előtt van még az utolsó adat
        if (legutobbiPerc->idopont<nap.idoZaras){
            int elteltPercek = nap.idoZaras-legutobbiPerc->idopont;
            Idopont utobbiP = legutobbiPerc->idopont;
            for (int i=1; i<elteltPercek+1; i++){ /// +1 mert a zárás idejét is bele kell venni (így logikus a ciklus fejléc)
                utobbiP.kovetkezoPerc();
                Arfolyam temp(utobbiP.ora,utobbiP.perc,legutobbiPerc->close,legutobbiPerc->close,legutobbiPerc->close,legutobbiPerc->close,0);
                pluszPercek.push_back(temp);
            }
        }

        /// plusz percek hozzáadása
        for (size_t i=0; i<pluszPercek.size(); i++)
            nap.percek.insert(pluszPercek[i]);


        /// madj elmenti a kiugrast
        if (legutobbiNap->datum.year>1800)
            nap.elozohozKiugras=nap.nyitas-legutobbiNap->zaras;


        /// nap nyitása és zárása
        Arfolyam arf(nap.idoNyitas.ora,nap.idoNyitas.perc);
        set<Arfolyam>::iterator it = nap.percek.find(arf);
        if (it == nap.percek.end()) {
            cout<<"nagy a baj : Arfolyam adatfeldolgozo iterator 1"<<endl;
        }
        else {
            nap.nyitas=(*it).open;
        }
        arf = Arfolyam(nap.idoZaras.ora,nap.idoZaras.perc);
        it = nap.percek.find(arf);
        if (it == nap.percek.end()) {
            cout<<"nagy a baj : Arfolyam adatfeldolgozo iterator 2"<<endl;
        }
        else {
            nap.zaras=(*it).close;
        }

        legutobbiNap=&nap;
    }

    /// megvannak a perc adatok és a napok összegzése
    /// kell a nagyedévek összehangolása is
    vector<Negyed> olvasztottNegyedek;
    vector<Negyed> ujNegyedek;
    for (const Negyed& negyed: negyedevek){
        /// ha nincs valahol tényleges jelentési dátum
        if (negyed.tenylegesJelentes.year==-1){
            /// akkor az egy magányos bevételi adatos rekord
            /// kell találni hozzá egy párt
            for (const Negyed& keresettNegyed: negyedevek){
                Datum keresett = keresettNegyed.idoszakVege;
                Datum talalt = negyed.idoszakVege;
                if (keresett.tavolsag(talalt)<10 && /// közel legyen max. egy hétre
                    keresett.tavolsag(talalt)!=0){ /// de ne ugyan az
                    olvasztottNegyedek.push_back(negyed);
                    olvasztottNegyedek.push_back(keresettNegyed);
                    Negyed ujNegyed = keresettNegyed;
                    ujNegyed.income=negyed.income;
                    ujNegyed.earn=negyed.earn;
                    ujNegyedek.push_back(ujNegyed);
                    break; /// nem kell másikat keresni
                }
            }
        }
    }
    for (size_t i=0; i<olvasztottNegyedek.size(); i++){ /// beolvasztott negyedek törlése
        negyedevek.erase(negyedevek.find(olvasztottNegyedek[i]));
    }
    for (size_t i=0; i<ujNegyedek.size(); i++){ /// új negyedek beillesztése
        negyedevek.insert(ujNegyedek[i]);
    }
}

float arfolyamGetMaxErtek(vector<Arfolyam> &v){
    float ret = 0;
    for (size_t i=0; i<v.size(); i++){
        ret=max(v[i].maximum,ret);
    }
    return ret;
}

float arfolyamGetMinErtek(vector<Arfolyam> &v){
    float ret = 10000000;
    for (size_t i=0; i<v.size(); i++){
        ret=min(v[i].minimum,ret);
    }
    return ret;
}

