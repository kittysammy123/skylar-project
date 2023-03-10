//
// Created by xs on 3/10/23.
//

#ifndef SKYLAR_PROJECT_LOG_H
#define SKYLAR_PROJECT_LOG_H


#include <string>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>

namespace skylar {

//日志事件
    class LogEvent {
    public:
        typedef std::shared_ptr<LogEvent> ptr;
        LogEvent();
    private:
        const char* m_file = nullptr;   //文件名
        int32_t m_line = 0;             //行号
        uint32_t m_elapse = 0;          //程序启动到现在的毫秒数
        int32_t m_threadId = 0;         //线程id
        int32_t m_fiberId = 0;          //协程id
        int64_t m_time = 0;             //时间戳
        std::string m_content;
    };

//日志级别
    class LogLevel {
    public:
        enum Level {
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5
        };
    };

//日志格式器
    class LogFormatter {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;
        LogFormatter(const std::string& pattern);

        std::string format(LogEvent::ptr event);
    private:
        class FormatItem {
        public:
            typedef std::shared_ptr<FormatItem> ptr;
            virtual ~FormatItem() {}
            virtual void format(std::stringstream& os,LogEvent::ptr event) = 0;
        };

        void init();
    private:
        std::string m_pattern;
        std::vector<FormatItem::ptr> m_items;
    };

//日志输出器
    class LogAppender {
    public:
        typedef std::shared_ptr<LogAppender> ptr;
        virtual ~LogAppender(){};

        virtual void log(LogLevel::Level level,LogEvent::ptr event) = 0;

        void setFormatter(LogFormatter::ptr formatter) {m_formatter = formatter;}
    protected:
        LogLevel::Level m_level;
        LogFormatter::ptr m_formatter;
    };

//日志器
    class Logger {
    public:
        typedef std::shared_ptr<Logger> ptr;
        Logger(const std::string& name = "root");

        void debug(LogEvent::ptr event);
        void info(LogEvent::ptr event);
        void warn(LogEvent::ptr event);
        void error(LogEvent::ptr event);
        void fatal(LogEvent::ptr event);

        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);

        LogLevel::Level getLevel() const {return m_level;}
        void setLevel(LogLevel::Level level) {m_level = level;}

    private:
        void log(LogLevel::Level level,LogEvent::ptr evevnt);

    private:
        std::string m_name;                         //日志名称
        LogLevel::Level m_level;                    //日志级别
        std::list<LogAppender::ptr> m_appenders;    //Appender集合
    };



//输出到控制台的Appender
    class StdoutLogAppender : public LogAppender {
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;
        void log(LogLevel::Level level,LogEvent::ptr event) override;
    private:

    };

//定义输出到文件的Appender
    class FileLogAppender : public LogAppender {
    public:
        typedef std::shared_ptr<FileLogAppender> ptr;
        FileLogAppender(const std::string& filename) {m_filename = filename;}
        void log(LogLevel::Level level,LogEvent::ptr event) override;

        bool reopen();
    private:
        std::string m_filename;
        std::ofstream m_filestream;
    };

}   //end namespace skylar

#endif //SKYLAR_PROJECT_LOG_H
