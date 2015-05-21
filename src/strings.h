#ifndef STRINGS_H
#define STRINGS_H

#include <iostream>
#include <stdlib.h>
#include <string.h>


class TString {
  public:
    size_t size;
    char *data;

    bool PrepareSpace(size_t newsize);

  public:
    TString();
    TString(TString &s);    
    TString(int len);
    TString(char *s);
    ~TString();

    char *str(void) const { return data; }
    char &operator[](int idx) { return data[idx]; }
    TString &operator+(TString &s);
    TString &operator+(char *s);
    TString &operator=(char *s);
    TString &operator=(TString &s); 
    bool operator==(char *s) { return strcmp(data,s)==0; }
    bool operator!=(char *s) { return strcmp(data,s)!=0; }

    void Copy(TString &source, int from, int count);
    void LFrac(int topos);
    void RFrac(int frompos); 
    void LTrim(void); 
    void RTrim(void); 
    void DoubleToStr(double value); 
    void IntToStr(int value); 
    int Length(void) { return strlen(data); } 
    int Pos(int c); 
    void ShrinkToEnd(int c); 
    void sprintf(char* format, ...); 
    double ToDouble(void); 
    int ToInt(void); 
    long ToLong(void);
    void UpStr(void); 
}; 
 
std::ostream &operator<<(std::ostream &outp, TString &t); 
 
#endif

