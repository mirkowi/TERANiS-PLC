//---------------------------------------------------------------------------
#include "MRegExp.h"
#include "platform.h"

// TException wahlweise ausblenden
#define _NO_EXEPTOBJ

#ifdef _NO_EXEPTOBJ
#define MEXCEPTION ;
#else
#include "exeptobj.h"
#endif

//---------------------------------------------------------------------------

bool TMRegExp::SetExpression(string value, int maxmatch)
{
  if(FExpression != value)
  {
    FExpression = value;
    #ifdef PERLREGEX
    if (maxmatch==0)
    {
      // Das ist eine ganz einfache Methode um die Komplexitaet
      // des regulaeren Ausdrucks abzuschaetzen
      // Es werden auf jeden Fall mehr matches gebraucht als
      // Zeichenketten am Ende zurueckgeliefert werden
      // Deshalb wird einfach die doppelte Menge angenommen...
      for (unsigned i=0; i<FExpression.length(); i++)
      {
        if (FExpression[i]=='(' || FExpression[i]==')') maxmatch++;
      }
      if (maxmatch<10) maxmatch=10;
    }
    this->maxmatch=maxmatch;
    delete[] aRegMatch;
    aRegMatch = new regmatch_t[maxmatch];
    if (compiled) regfree(&aRegExp);
    int err = regcomp(&aRegExp, FExpression.c_str(), 0);
    if (!err) compiled = true;
    matched=false;
    // TODO: regerror
    #ifndef _NO_EXEPTOBJ
    if (err) MEXCEPTION(0,"Fehler beim Erzeugen des regulaeren Ausdrucks: '"+FExpression+"'","");
    #endif
    #endif
    #ifdef STDREGEX
    try {
      aRegExp = std::regex(FExpression);
      compiled = true;
    }
    catch (const std::regex_error& e) {
      #ifndef _NO_EXEPTOBJ
      MEXCEPTION(0,"Fehler beim Erzeugen des regulaeren Ausdrucks: '"+FExpression+"'",e.what());
      #endif
      compiled = false;
    }
    #endif
    matched=false;
  }
  return compiled;
}

string TMRegExp::GetExpression()
{
  return FExpression;
}

TMRegExp::TMRegExp(int maxmatch)
{
  #ifdef PERLREGEX
    this->maxmatch=maxmatch;
    if (maxmatch>0) aRegMatch = new regmatch_t[maxmatch];
    else aRegMatch = NULL;
  #endif
  matched=false;
  compiled = false;
}

TMRegExp::TMRegExp(string Expression, int maxmatch)
{
  #ifdef PERLREGEX
    this->maxmatch=0;
    aRegMatch = NULL;
  #endif
  matched=false;
  compiled = false;
  SetExpression(Expression,maxmatch);
}


TMRegExp::~TMRegExp()
{
  #ifdef PERLREGEX
    if (compiled) regfree(&aRegExp);
    delete[] aRegMatch;
  #endif
}

bool TMRegExp::Exec(string text)
{
  if (!compiled) return false;
  matched = false;
  #ifdef PERLREGEX
    matched = regexec(&aRegExp, text.c_str(), maxmatch, aRegMatch, 0)==0;
  #endif
  #ifdef STDREGEX
    matched = std::regex_search(text, aRegMatch, aRegExp);
  #endif
  if (matched) {
    FText = text;
  }
  return matched;
}

bool TMRegExp::Exec(string text, vector<string> &list)
{
  if (!compiled) return false;
  matched = Exec(text);
  if (matched) GetMatches(list);
  return matched;
}

bool TMRegExp::ExecExpression(string expression, string text)
{
  SetExpression(expression);
  if (!compiled) return false;
  return Exec(text);
}

void TMRegExp::GetMatches(vector<string> &list)
{
  if (!matched) return;
  list.clear();
  #ifdef PERLREGEX
  for (int i = 1; i < maxmatch; i++)
  {
    if (aRegMatch[i].rm_so >= 0)
    {
      list.push_back(FText.substr(aRegMatch[i].rm_so, aRegMatch[i].rm_eo - aRegMatch[i].rm_so));
    }
  }
  #endif
  #ifdef STDREGEX
  for (auto i=aRegMatch.begin()+1; i != aRegMatch.end(); i++) {
    list.push_back(*i);
  }
  #endif
}

string TMRegExp::GetMatch(unsigned num, unsigned &pos, unsigned &len)
{
  pos=0;
  if (!matched) return "";
  #ifdef PERLREGEX
  int n=0;
  for (int i = 1; i < maxmatch; i++)
  {
    if (aRegMatch[i].rm_so >= 0)
    {
      if (num==n)
      {
        if (aRegMatch[i].rm_eo < aRegMatch[i].rm_so)
        {
          #ifndef _NO_EXEPTOBJ
          MEXCEPTION(0,"Fehler beim Ermitteln einer Zeichenkette im regulaeren Ausdruck '"
            +FExpression+"'. Position "+int2string(aRegMatch[i].rm_so)+" - "+int2string(aRegMatch[i].rm_eo),"");
          #else
          return "";
          #endif
        }
        pos = aRegMatch[i].rm_so;
        return FText.substr(pos, aRegMatch[i].rm_eo - pos);
      }
      n++;
    }
  }
  #endif
  #ifdef STDREGEX
  num++; // auf Index 0 steht der gesamte String
  if (aRegMatch.size()>num) {
    pos = aRegMatch.position(num);
    len = aRegMatch.length(num);
    // komischerweise liefert aRegMatch[num] manchmal einen Leerstring wo keiner sein sollte
    // der Substring funktioniert aber
    // string s = aRegMatch[num];
    return FText.substr(pos,len);
  }
  #endif
  return "";
}

string TMRegExp::GetMatch(unsigned num, unsigned &pos)
{
    unsigned len;
    return GetMatch(num,pos, len);
}

string TMRegExp::GetMatch(unsigned num)
{
    unsigned pos, len;
    return GetMatch(num,pos, len);
}
