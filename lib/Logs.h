#ifndef LOGS_HEADER
#define LOGS_HEADER

#include <stdio.h>

const char DEFAULT_LOG_NAME[] = "Logs.html";

class Logs
{
    private:

        FILE* file_;

    public:

        explicit Logs       ();
        explicit Logs       (const char* name);

        ~Logs ();
        
        
        void    error       (const char* fmt, ...);
        void    warn        (const char* fmt, ...);
        void    debug       (const char* fmt, ...);
        void    trace       (const char* fmt, ...);

        void    print       (const char* fmt, ...);

        FILE*   getFile     ();

        char*   getTimeString (char* buf);
};

extern Logs Logs;

#endif
