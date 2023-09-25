#include "adatBeolvas.h"

vector<string> getSubdirectories(const string& directoryPath) {
    vector<string> subdirectories;

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directoryPath + "/*").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return subdirectories; // Ha a mappa nem l�tezik, vagy hiba t�rt�nik, �res list�t adunk vissza
    }

    do {
        const string fileOrDirName = findFileData.cFileName;
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // Kihagyjuk a "." �s ".." mappaneveket
            if (fileOrDirName != "." && fileOrDirName != "..") {
                subdirectories.push_back(fileOrDirName);
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return subdirectories;
}

vector<string> getFiles(const string& directoryPath) {
    vector<string> files;

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directoryPath + "/*").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return files; // Ha a mappa nem l�tezik, vagy hiba t�rt�nik, �res list�t adunk vissza
    }

    do {
        const string fileOrDirName = findFileData.cFileName;
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            // Ha nem mappa, hozz�adjuk a list�hoz
            files.push_back(fileOrDirName);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return files;
}

vector<string> osszesReszveny(){
    string directoryPath = gyoker+"\\stocks";
    vector<string> subdirectories = getSubdirectories(directoryPath);
    return subdirectories;
}

vector<string> osszesCsoport(){
    string directoryPath = gyoker+"\\groups";
    vector<string> subdirectories = getFiles(directoryPath);
    return subdirectories;
}

string createNewReszveny(string str){
    string path = gyoker+"\\stocks";
    transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return toupper(c); });

    string directoryName = str;

    string fullPath = path + "\\" + directoryName;

    if (CreateDirectory(fullPath.c_str(), NULL)) {
        return "Letrehozva!";
    } else {
        return "Hiba!";
    }
}

string createNewCsoport(string str){
    string path = gyoker+"\\groups";
    string fileName = str;

    string fullPath = path + "\\" + fileName;
    ifstream f(fullPath);
    if (f.is_open()) return "Mar letezik!";
    ofstream file(fullPath);
    if (!file) {
        return "Hiba!";
    }
    return "Letrehozva!";
}

bool benneVanAzStr (string miben, string mi){
    string str = miben;
    string toFind = mi;

    // Mindk�t stringet kisbet�ss� alak�tjuk
    transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return tolower(c); });
    transform(toFind.begin(), toFind.end(), toFind.begin(), [](unsigned char c){ return tolower(c); });

    // Megkeress�k a substringet
    size_t found = str.find(toFind);
    if (found != string::npos) {
        return true;
        //cout << "A substring megtal�lhat� az indexen: " << found << endl;
    } else {
        return false;
        //cout << "A substring nem tal�lhat� meg." << endl;
    }
}

bool DeleteDirectory(const std::string &directoryPath) {
    // Rekurz�van t�r�lj�k a k�nyvt�r tartalm�t
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA((directoryPath + "\\*").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return false;
    }

    do {
        const std::string fileOrDirName = findFileData.cFileName;

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (fileOrDirName != "." && fileOrDirName != "..") {
                // Rekurz�van t�r�lj�k az almapp�kat
                DeleteDirectory(directoryPath + "\\" + fileOrDirName);
            }
        } else {
            // T�r�lj�k a f�jlokat
            DeleteFileA((directoryPath + "\\" + fileOrDirName).c_str());
        }
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);

    // T�r�lj�k az �res k�nyvt�rat
    return RemoveDirectoryA(directoryPath.c_str()) != 0;
}

string deleteReszveny(string str){
    transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return toupper(c); });

    string path = gyoker+"\\stocks\\"+str;
    bool siker = DeleteDirectory(path);
    if (siker) return "Siker!";
    return "Hiba!";
}

string deleteCsoport(string str){
    string filePath = gyoker+"\\groups\\"+str; // V�ltoztassa meg a saj�t �tvonal�ra

    if (std::remove(filePath.c_str()) == 0) {
        return "Siker!";
    } else {
        return "Hiba!";
    }
}

bool elemeAzStr ( vector<string> v, string mi){
    vector<string> vec = v;
    string elementToFind = mi;

    auto it = find(vec.begin(), vec.end(), elementToFind);

    return (it != vec.end());
}

vector<string> csoportReszvenyei(string str){
    string path = gyoker+"\\groups\\"+str;
    ifstream file(path);
    vector<string> lista;
    while (!file.eof()){
        string temp;
        file>>temp;
        if (temp.size()!=0)
            lista.push_back(temp);
    }
    return lista;
}

string csoportFrissites(vector<string> vec, string csop){
    if (csop.size()==0) return "Ures!";
    string path = gyoker+"\\groups\\"+csop;
    ofstream file(path);
    if (!file.is_open()){return "Hiba!";}
    for (size_t i=0; i<vec.size(); i++){
        file<<vec[i]<<" ";
        if (i%5==4) file<<endl;
    }
    return "Siker!";
}


