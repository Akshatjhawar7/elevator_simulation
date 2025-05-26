#include "Logger.h"
#include <QTextEdit>

Logger::Logger(QTextEdit* consoleWidget) : console(consoleWidget) {}

void Logger::appendMessage(const QString& message) {
    if(console){
        console->append("[LOG] " + message);
    }

}

QTextEdit* Logger::getConsole(){
    return console;
}
