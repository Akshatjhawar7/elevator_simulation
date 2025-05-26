#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QPlainTextEdit>

class Logger {
private:
    QTextEdit* console;

public:
    explicit Logger(QTextEdit* consoleWidget);
    void appendMessage(const QString& message);
    QTextEdit* getConsole();
};

#endif // LOGGER_H
