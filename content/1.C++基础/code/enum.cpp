#include <iostream>

using namespace std;

//#define LOG(x) cout << x <<endl

class Log{
public:
    //enum，枚举类型
    enum Level: unsigned char
    {
        LevelError,LevelWarning,LevelInfo
    };
private:
    Level m_lev = LevelInfo;
public:
    void SetLevel(Level lev){
        m_lev = lev;
    }

    void Error(const char* message){
        if(m_lev >= LevelError)
            cout << "[ERROR:]" << message << endl;
    }

    void Warning(const char* message){
        if(m_lev >= LevelWarning)
            cout << "[WARNNING:]" << message << endl;
    }

    void Info(const char* message){
        if(m_lev >= LevelInfo)
            cout << "[INFO:]" << message << endl;
    }
};



int main(){
    Log log;
    //在类中定义了enum，可以看做enum是在class命名空间中的static变量，可以直接通过类名访问
    log.SetLevel(Log::LevelError);
    log.Info("hello world!");
    log.Warning("hello world!");
    log.Error("hello world!");


}