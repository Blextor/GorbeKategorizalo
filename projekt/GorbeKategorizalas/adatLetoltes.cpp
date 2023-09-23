#include "adatLetoltes.h"
#include "adatBeolvas.h"

clock_t utolsoLetoltes=0;

int counter = 0;

clock_t t = clock();


bool debug = false;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string& s) {
    t=clock();
    size_t newLength = size * nmemb;
    s+=(string)(char*)contents;
    if (debug) cout<<"dt: "<<(clock()-t)<<" "<<counter<<endl;
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
    // https://www.alphavantage.co/query?function=INCOME_STATEMENT&symbol=IBM&apikey=demo
    string url = "https://www.alphavantage.co/query?function=INCOME_STATEMENT";
    url+="&symbol="+stock;
    url+="&apikey="+API_Key;
    //url+="&outputsize=full";
    return url;
}

string getLinkMonth(string stock, int year, int month){
    string API_Key = getAPI_Key();
    /// https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=IBM&interval=5min&apikey=demo&datatype=csv
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

        res = curl_easy_perform(curl);


        if (debug) cout<<"d: "<<(clock()-t)<<" "<<counter<<endl;
        if (debug) t=clock();

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

            return "Hiba!";
        } else {
            if (readBuffer.size()<1000){
                cout<<"alma: "<<endl<<readBuffer<<endl;
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
