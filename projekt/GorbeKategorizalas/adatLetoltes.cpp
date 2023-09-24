#include "adatLetoltes.h"
#include "adatBeolvas.h"

clock_t utolsoLetoltes=0;

mutex letM;
int aktMin = 0;
int downloadCnt = 0;

int counter = 0;

clock_t t = clock();

string errorMsg = "";


int getActMin(){
    /// CGPT
    auto now = chrono::system_clock::now();
    time_t now_t = chrono::system_clock::to_time_t(now); /// Időpont átalakítása time_t típusra
    tm* now_tm = localtime(&now_t); /// Időpont átalakítása tm struktúrává
    return now_tm->tm_min;
}

int getActYear(){
    /// CGPT
    auto now = chrono::system_clock::now();
    time_t now_t = chrono::system_clock::to_time_t(now); /// Időpont átalakítása time_t típusra
    tm* now_tm = localtime(&now_t); /// Időpont átalakítása tm struktúrává
    return now_tm->tm_year+1900;
}

int getActMonth(){
    /// CGPT
    auto now = chrono::system_clock::now();
    time_t now_t = chrono::system_clock::to_time_t(now); /// Időpont átalakítása time_t típusra
    tm* now_tm = localtime(&now_t); /// Időpont átalakítása tm struktúrává
    return now_tm->tm_mon+1;
}

void waitForAPI(){
    if (!(downloadCnt++<maxLetoltesPerMin)){
        while (aktMin==getActMin()){
            Sleep(10);
        }
    }
    int minute = getActMin();
    if (minute!=aktMin){
        aktMin=minute;
        downloadCnt=1;
    }
    //cout<<"dcn: "<<downloadCnt<<endl;
}


bool debug = false;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string& s) {
    t=clock();
    size_t newLength = size * nmemb;
    s+=(string)(char*)contents;
    if (debug) cout<<"dt: "<<(clock()-t)<<" "<<endl;
    if (debug) t=clock();
    return newLength;
}

string getAPI_Key(){
    ifstream file(gyoker+"API_Key.txt");
    string key; file>>key;
    file.close();
    return key;
}

string getLinkDaily(string stock){
    string API_Key = getAPI_Key();
    string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&datatype=csv";
    url+="&symbol="+stock;
    url+="&apikey="+API_Key;
    url+="&outputsize=full";
    return url;
}

string getLinkEarnings(string stock){
    string API_Key = getAPI_Key();
    //return "https://www.alphavantage.co/query?function=EARNINGS&symbol=IBM&apikey=demo";
    string url = "https://www.alphavantage.co/query?function=EARNINGS";
    url+="&symbol="+stock;
    url+="&apikey="+API_Key;
    //url+="&outputsize=full";
    return url;
}

string getLinkIncome(string stock){
    string API_Key = getAPI_Key();
    //return "https://www.alphavantage.co/query?function=INCOME_STATEMENT&symbol=IBM&apikey=demo";
    string url = "https://www.alphavantage.co/query?function=INCOME_STATEMENT";
    url+="&symbol="+stock;
    url+="&apikey="+API_Key;
    //url+="&outputsize=full";
    return url;
}

string getLinkMonth(string stock, int year, int month){
    string API_Key = getAPI_Key();
    //return "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=IBM&interval=5min&apikey=demo&datatype=csv";
    string url = "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&interval=1min&datatype=csv";
    url+="&symbol="+stock;
    url+="&apikey="+API_Key;
    url+="&outputsize=full";
    stringstream ss; ss<<"&month="<<year<<"-"<<(month/10)<<(month%10);
    url+=ss.str();
    return url;
}

string downloadLink(string link) {
    clock_t t = clock();
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    readBuffer.reserve(500000);

    if (debug) cout<<"a: "<<(clock()-t)<<endl;
    if (debug) t=clock();

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (debug) cout<<"b: "<<(clock()-t)<<endl;
    if (debug) t=clock();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        if (debug) cout<<"c: "<<(clock()-t)<<" "<<counter<<endl;
        if (debug) t=clock();

        /// késleltetés ha szükséges (API hívás limit miatt)
        waitForAPI();

        res = curl_easy_perform(curl);


        if (debug) cout<<"d: "<<(clock()-t)<<" "<<counter<<endl;
        if (debug) t=clock();

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            cout<<"error: "<<endl<<readBuffer<<endl;
            return "Hiba!";
        } else {
            if (readBuffer.size()<1000){
                if (debug) cout<<"error: "<<endl<<readBuffer<<endl;
                cout<<"link: "<<link<<endl;
                errorMsg=readBuffer;
                //return "Túl kicsi a readBuffer";
                return "Hiba!";
            }
            //std::ofstream outFile("output.json");
            //outFile << readBuffer;
        }

        curl_easy_cleanup(curl);
    }

    if (debug) cout<<"e: "<<(clock()-t)<<endl;
    if (debug) t=clock();

    curl_global_cleanup();
    return readBuffer;
}

bool letezoReszveny (string stock){
    clock_t t = clock();
    string readBuf = downloadLink(getLinkEarnings(stock));
    cout<<"f: "<<(clock()-t)<<endl;
    if (readBuf!="Hiba!") return true;
    return false;
}

bool felkeszReszveny (string stock){
    bool chk = elemeAzStr(osszesReszveny(),stock);
    if (chk){
        string path = gyoker+"\\stocks\\"+stock+"\\months";
        return getFiles(path).size()!=0;
    }
    return false;
}

thread valami(){
    thread temp;
    return temp;
}


string reszvenyLetoltesChk(string stock){
    /// létezik-e ilyen részvény?
    if (!letezoReszveny(stock)){
        return "Nem letezo reszveny";
    }
    return "Siker!";
}

string reszvenyFrissitesChk(string stock){
    /// félkész-e ez a részvény?
    if (!felkeszReszveny(stock)){
        return "Nem letezo reszveny";
    }
    return "Siker!";
}

string csoportLetoltesChk(string group){
    return "Hiba!";
    /// létezik-e ilyen részvény?
    if (!letezoReszveny(group)){
        return "Nem letezo csoport";
    }
    ///
    thread m = valami();

    return "Siker!";
}

string csoportFrissitesChk(string group){
    return "Hiba!";
    /// létezik-e ilyen részvény?
    if (!letezoReszveny(group)){
        return "Nem letezo csoport";
    }
    ///
    thread m = valami();

    return "Siker!";
}

bool reszvenyNapiLetrehoz(string stock, string str){
    string path = gyoker+"\\stocks\\"+stock+"\\napi.txt";
    ofstream file(path);
    if (!file.is_open()) return false;
    file<<str;
    file.close();
    return true;
}

bool reszvenyJelentesLetrehoz(string stock, string str){
    string path = gyoker+"\\stocks\\"+stock+"\\jelentes.txt";
    ofstream file(path);
    if (!file.is_open()) return false;
    file<<str;
    file.close();
    return true;
}

bool reszvenyBevetelLetrehoz(string stock, string str){
    string path = gyoker+"\\stocks\\"+stock+"\\bevetel.txt";
    ofstream file(path);
    if (!file.is_open()) return false;
    file<<str;
    file.close();
    return true;
}

bool reszvenyHonapLetrehoz(string stock, string str, int year, int month){
    string path = gyoker+"\\stocks\\"+stock+"\\months\\";
    stringstream ss; ss<<path<<year<<"_"<<(month/10)<<(month%10)<<".csv";
    ofstream file(ss.str());
    if (!file.is_open()) return false;
    file<<str;
    file.close();
    return true;
}

void reszvenyAPILetoltes(string stock, bool &stopped, bool &inProc, function<void()> func){
    int state = 0;
    int year = 2000, month = 1;
    int celY = getActYear(), celM=getActMonth();
    string path = gyoker+"\\stocks\\"+stock;
    CreateDirectory(path.c_str(), NULL);
    path+="\\months";
    CreateDirectory(path.c_str(), NULL);
    while (true){
        if (!inProc) return;
        if (stopped) Sleep(1);
        else {
            string str;
            if (state==0) { /// még nincs letöltve a napi
                str = downloadLink(getLinkDaily(stock));
                if (str!="Hiba!"){
                    reszvenyNapiLetrehoz(stock,str);
                    state=1;
                    func();
                } else {
                    cout<<"Hiba: "<<stock<<", nem tolti le a napit"<<endl;
                }
            }
            else if (state==1) { /// még nincs letöltve a pénzügyi jelentése
                str = downloadLink(getLinkEarnings(stock));
                if (str!="Hiba!"){
                    reszvenyJelentesLetrehoz(stock,str);
                    state=2;
                    func();
                } else {
                    cout<<"Hiba: "<<stock<<", nem tolti le a jeletest"<<endl;
                }
            }
            else if (state==2) { /// még nincs letöltve a bevételei
                str = downloadLink(getLinkIncome(stock));
                if (str!="Hiba!"){
                    reszvenyBevetelLetrehoz(stock,str);
                    state=3;
                    func();
                } else {
                    cout<<"Hiba: "<<stock<<", nem tolti le a bevetelt"<<endl;
                    cout<<getLinkIncome(stock)<<endl;
                }
            }
            else if (state==3) { /// még nincsenek letöltve a hónapok
                str = downloadLink(getLinkMonth(stock,year,month));

                if (str!="Hiba!" || benneVanAzStr(errorMsg,"Invalid API call.")){
                    if (str!="Hiba!"){
                        reszvenyHonapLetrehoz(stock,str,year,month);

                    } else {
                        downloadCnt--;
                    }
                    month++;
                    func();
                    if (month>12){ month=1; year++; }
                    if (year>celY || (year==celY && month>celM)) return;
                } else {
                    cout<<"Hiba: "<<stock<<", nem tolti le a honapot"<<endl;
                    cout<<getLinkMonth(stock,year,month)<<endl;
                }
            }
        }
    }
}

int legregebbiReszvenyHonap(string stock) { /// return YYYYMM
    string path = gyoker+"\\stocks\\"+stock+"\\months";
    vector<string> honapok = getFiles(path);
    int maxY = 0, maxM = 0;
    for (size_t i=0; i<honapok.size(); i++){
        stringstream ss; ss<<honapok[i];
        int y = 0, m = 0;
        char c;
        string str;
        ss>>y>>c>>m>>str;
        if (c=='_' && str==".csv" && y>=1998 && y<=getActYear() && m>=1 && m<=12){
            if (y>maxY){
                maxY=y; maxM=m;
            } else if (y==maxY && m>maxM){
                maxM=m;
            }
        }
    }
    return maxY*100+maxM;
}

void reszvenyAPIFrissites(string stock, bool &stopped, bool &inProc, function<void()> func){
    int state = 0;
    int YYYYMM=legregebbiReszvenyHonap(stock);
    int year = YYYYMM/100, month = YYYYMM%100;
    cout<<year<<" "<<month<<endl;
    if (true){
        int y = 2000, m=1;
        while (year!=y || m!=month){
            func();
            m++;
            if (m>12) {m=1; y++;}
        }
    }
    int celY = getActYear(), celM=getActMonth();
    string path = gyoker+"\\stocks\\"+stock;
    CreateDirectory(path.c_str(), NULL);
    path+="\\months";
    CreateDirectory(path.c_str(), NULL);
    while (true){
        if (!inProc) return;
        if (stopped) Sleep(1);
        else {
            string str;
            if (state==0) { /// még nincs letöltve a napi
                str = downloadLink(getLinkDaily(stock));
                if (str!="Hiba!"){
                    reszvenyNapiLetrehoz(stock,str);
                    state=1;
                    func();
                } else {
                    cout<<"Hiba: "<<stock<<", nem tolti le a napit"<<endl;
                }
            }
            else if (state==1) { /// még nincs letöltve a pénzügyi jelentése
                str = downloadLink(getLinkEarnings(stock));
                if (str!="Hiba!"){
                    reszvenyJelentesLetrehoz(stock,str);
                    state=2;
                    func();
                } else {
                    cout<<"Hiba: "<<stock<<", nem tolti le a jeletest"<<endl;
                }
            }
            else if (state==2) { /// még nincs letöltve a bevételei
                str = downloadLink(getLinkIncome(stock));
                if (str!="Hiba!"){
                    reszvenyBevetelLetrehoz(stock,str);
                    state=3;
                    func();
                } else {
                    cout<<"Hiba: "<<stock<<", nem tolti le a bevetelt"<<endl;
                    cout<<getLinkIncome(stock)<<endl;
                }
            }
            else if (state==3) { /// még nincsenek letöltve a hónapok
                str = downloadLink(getLinkMonth(stock,year,month));
                if (str!="Hiba!"){
                    reszvenyHonapLetrehoz(stock,str,year,month);
                    month++;
                    func();
                    if (month>12){ month=1; year++; }
                    if (year>celY || (year==celY && month>celM)) return;
                } else {
                    cout<<"Hiba: "<<stock<<", nem tolti le a napot"<<endl;
                    cout<<getLinkMonth(stock,year,month)<<endl;
                }
            }
        }
    }
}

void csoportAPILetoltes(string group, bool &stopped, bool &inProc, function<void()> func){

}


void csoportAPIFrissites(string group, bool &stopped, bool &inProc, function<void()> func){

}


