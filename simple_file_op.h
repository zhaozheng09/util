#ifndef ZZ_STRING_TO_FILE
#define ZZ_STRING_TO_FILE

#include <iostream>
#include <fstream>

using namespace std;

//only append .
class FileWriter {
public:
    FileWriter(const string& name) : ofstr_(name.c_str(), ios::app | ios::binary) {}
    template<class T>
    void WriteByType(const T& tmp) {
        ofstr_.write((const char*)&tmp, sizeof(T));
        ofstr_.flush();
    }
    void WriteString(const string& tmp) {
        WriteCString(tmp.c_str(), tmp.size());
        ofstr_.flush();
    }
    void WriteCString(const char *p, const size_t len) {
        WriteByType<size_t>(len);
        ofstr_.write(p, len);
        ofstr_.flush();
    }
    ~FileWriter() {ofstr_.close();}

    ofstream ofstr_;
};

//only orderly read ...
class FileReader {
public:
    FileReader(const string& name) : ifstr_(name.c_str(), ios::app) {}
    template<class T>
    bool ReadByType(T& ret) {
        ifstr_.read((char *)&ret, sizeof(T));    
        if (ifstr_.eof()) {
            return false;
        }
        return true;
    }
    bool ReadString(string& ret) {
        size_t len = 0;
        if(!ReadByType<size_t>(len)) {
            return false;
        }
        ret.resize(len);
        ifstr_.read((char *)ret.c_str(), len);
        if (ifstr_.eof()) {
            return false;
        }
        return true;
    }
    ~FileReader() {ifstr_.close();}

    ifstream ifstr_;
    size_t cur_pos = 0;
};

//int main()
//{
//    FileWriter writer("test.txt");
//    writer.WriteString("nihao");
//    FileReader reader("test.txt");
//    string str;
//    reader.ReadString(str);
//    return 0;
//}

#endif 
