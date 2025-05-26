#ifndef GUI_H
#define GUI_H

#include <QString>
#include <QStringListModel>
#include <QListView>
#include <QObject>

class MainWindow;

class GUI{
private:
    MainWindow* mainWindow;
public:
    GUI(MainWindow* mainWindowInstance);
    void logger(const std::string& message);
};

#endif // GUI_H
