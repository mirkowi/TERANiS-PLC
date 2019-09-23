#ifndef _INC_PLATFORM_H
#define _INC_PLATFORM_H

#ifdef ARDUINO
  #define MSTRING String
  #define DEBUGPRINT(X) Serial.print(X);
  #define DEBUGPRINTHEX(X) Serial.print(X,HEX);
  #define DEBUGPRINTLN(X) Serial.println(X);
#else
  #ifdef __BORLANDC__
    #include <vcl.h>
    #pragma hdrstop
    // Warnung bei Vergleich von signed und unsigned abschalten
    // Das sollte so ziemlich global wirken...
    #pragma warn -csu
  #endif
  #include <string>
  #include <iostream>
  #define MSTRING std::string
  #define DEBUGPRINT(X) std::cout << X;
  #define DEBUGPRINTHEX(X) std::cout << std::hex << (unsigned)(X);
  #define DEBUGPRINTLN(X) std::cout << X << std::endl;
#endif

#if defined(__BORLANDC__)
#define EXCEPTION_MESSAGE_STRING(exception) std::string(AnsiString(exception.Message).c_str())
#else
class Exception {
    public:
      MSTRING Message;
      Exception(MSTRING message) {Message=message;};
   };
#define EXCEPTION_MESSAGE_STRING(exception) exception.Message
#endif

class TMException : public Exception
{
  public:
    TMException(MSTRING message) : Exception(message.c_str()) {};
    TMException(MSTRING message, int param1) : Exception(message.c_str()) {};
    TMException(MSTRING message, int param1, int param2) : Exception(message.c_str()) {};
};

#endif
