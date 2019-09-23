#include "MSocket.h"
#include "MRegExp.h"
#include "platform.h"

//---------------------------------------------------------------------------
int TMSocket::RefCount = 0;

void TMSocket::Start()
{
  #if (_Windows)
  if (RefCount==0)
  {
    WSADATA wsa;
    long rc = WSAStartup(MAKEWORD(2,0),&wsa);
    if (rc!=0)
    {
      throw Exception("WinSock laesst sich nicht starten.");
    }
  }
  RefCount++;
  #endif
}

void TMSocket::Stop()
{
  #if (_Windows)
  if (RefCount==1)
  {
    WSACleanup();
  }
  if (RefCount>0) RefCount--;
  #endif
}

TMSocket::TMSocket()
{
  Start();
}

TMSocket::~TMSocket()
{
}

void TMSocket::Hostport2HostPort(std::string hostport, std::string &host, int &port)
{
  // Hilfsfunktion zur Trennung Host:Port
  TMRegExp rx("^(\\S+)\\:(\\d+)$");
  if (!rx.Exec(hostport))
  {
    throw Exception(("Fehler bei '"+hostport+"'. Host und Port muessen in der Form Hostname:Portnummer angegeben werden.").c_str());
  }
  host = rx.GetMatch(0);
  port = atoi(rx.GetMatch(1).c_str());
}

std::string TMSocket::Host2Ip(std::string host)
{
  // Hilfsfunktion zur Namensaufloesung eines Hostnamens
  TMRegExp rx("^(\\d+\\.\\d+\\.\\d+\\.\\d+)$");
  if (!rx.Exec(host))
  {
    // TODO: Namensaufloesung mit gethostbyname
    throw Exception(("'"+host+"' muss eine gueltige IP-V4-Adresse sein.").c_str());
  }
  return host;
}

std::string TMSocket::GetLastError()
{
  #if (_Windows)
    wchar_t *s = NULL;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
               NULL, WSAGetLastError(),
               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
               (LPWSTR)&s, 0, NULL);
    std::wstring retval = s;
    LocalFree(s);
    return retval;
  #else
    std::string s = strerror(errno);
    return s;
  #endif
}
