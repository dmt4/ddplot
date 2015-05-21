#include <ctype.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "strings.h"


void *xmalloc (size_t size)
{
  register void *value = malloc(size);
  if (value == 0) {
    std::cout << "Virtual memory exhausted" << std::endl;
    exit(0);
  }
  return value;
}


void *xrealloc (void *ptr, size_t size)
{
  register void *value = realloc(ptr,size);
  if (value == 0) {
    std::cout << "Virtual memory exhausted" << std::endl;
    exit(0);
  }
  return value;
}


/*--------------------------------------------------------------------------------------------------------- 
  class TString - DEFINICE
---------------------------------------------------------------------------------------------------------*/ 
 
TString::TString() 
{ 
  size = 128;                         // alokuje se fixni delka 127 byte + '\0' 
  data = (char *) xmalloc(size); 
  data[0] = '\0'; 
} 
 
 
// copy constructor 
TString::TString(TString &s) 
{ 
  size = s.size;                        // alokuje se fixni delka 127 byte + '\0' 
  data = (char *) xmalloc(size); 
  strcpy(data,s.data); 
} 
 
 
TString::TString(int len) 
{ 
  size = len+1; 
  data = (char *) xmalloc(size); 
  data[0] = '\0'; 
} 
 
 
TString::TString(char *s) 
{ 
  size = strlen(s)+1; 
  data = (char *) xmalloc(size); 
  strcpy(data,s); 
} 
 
 
TString::~TString() 
{ 
  if (data!=NULL) { 
    free(data); 
    data = NULL; 
  } 
} 
 
 
TString &TString::operator+(TString &s) 
{ 
  PrepareSpace(Length()+s.Length()+1); 
  strcat(data,s.data); 
  return *this; 
} 
 
 
TString &TString::operator+(char *s) 
{ 
  PrepareSpace(Length()+strlen(s)+1); 
  strcat(data,s); 
  return *this; 
} 
 
 
TString &TString::operator=(char *s) 
{ 
  PrepareSpace(strlen(s)+1); 
  strcpy(data,s); 
  return *this; 
} 
 
 
TString &TString::operator=(TString &s) 
{ 
  PrepareSpace(s.size); 
  strcpy(data,s.data); 
  return *this; 
} 


//--- private methods ------------------------------------------------------------------

bool TString::PrepareSpace(size_t newsize)
{
  bool need_realloc;

  need_realloc = (size!=newsize);
  if (need_realloc) {
    size = newsize;
    data = (char *) xrealloc(data,size);
  }

  return need_realloc;
}


//--- public methods -------------------------------------------------------------------

void TString::Copy(TString &source, int from, int count)
{
  PrepareSpace(count+1);
  memcpy(data,&source.data[from],count);
  data[count] = '\0';
}


void TString::DoubleToStr(double value)
{
  char buf[128];

  sprintf(buf,"%*g",5,value);
  PrepareSpace(strlen(buf)+1);
  strcpy(data,buf);
}


void TString::IntToStr(int value)
{
  char buf[128];

  sprintf(buf,"%d",value);
  PrepareSpace(strlen(buf)+1);
  strcpy(data,buf);
}


void TString::LFrac(int topos)
{
  int count;
  char *keep;

  if (topos<0)
    return;

  keep = (char *) xmalloc(size);
  strcpy(keep,data);

  count = Length()-topos;
  PrepareSpace(count);
  memcpy(data,&keep[topos+1],count);

  free(keep);
}



void TString::RFrac(int frompos)
{
  char *keep;

  if (frompos>Length()-1)
    return;

  keep = (char *) xmalloc(size);
  strcpy(keep,data);

  PrepareSpace(frompos+1);
  if (frompos>1)
    memcpy(data,keep,frompos);
  data[frompos] = '\0';

  free(keep);
}


void TString::LTrim(void)
{
  int len,i;

  len = Length();
  for (i=0; i<len; i++)
    if (data[i]!=' ')
      break;

  if (i>0)
    LFrac(i-1);
}


void TString::RTrim(void)
{
  int len,i;

  len = Length();
  for (i=len-1; i>=0; i--)
    if (data[i]!=' ')
      break;

  if (i<len-1)
    RFrac(i+1);
}


int TString::Pos(int c)
{
  char *ptr;

  ptr = strchr(data,c);
  if (ptr==NULL)
    return -1;
  else
    return ptr-data;
}


void TString::ShrinkToEnd(int c)
{
  int idx;

  idx = Pos(c);     
  if (idx>-1)
    RFrac(idx);
}


void TString::sprintf(char* format, ...)
{
  va_list args;
  char buf[128];

  va_start(args,format);
  vsprintf(buf,format,args);
  PrepareSpace(strlen(buf)+1);
  strcpy(data,buf);
  va_end(args);
}


double TString::ToDouble(void)
{
  return strtod(data,NULL);
}


int TString::ToInt(void)
{
  return strtol(data,NULL,10);
}


long TString::ToLong(void)
{
  return strtol(data,NULL,10);
}


void TString::UpStr(void)
{
  TString s;
  int len,i;

  if (*this!="") {
    len = Length();
    for (i=0; i<=len-1; i++) {
      data[i] = toupper(data[i]);
    }
  }
}


std::ostream &operator<<(std::ostream &outp, TString &t)
{
  outp << t.str();
  return outp;
}
