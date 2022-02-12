#ifndef LOG_H
#define LOG_H

#include <ostream>
class Log
{
    enum Level {
        Info,

    };

  public:
    ~Log();
    void setLogStream(std::ostream &stream);

    static Log &get();

  private:
    std::ostream *m_logstream;
};
#endif // !LOG_H
