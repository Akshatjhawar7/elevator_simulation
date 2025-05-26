#include "GUI.h"
#include "mainwindow.h"
#include <QCoreApplication>
#include <QEventLoop>
#include <QTimer>
#include <iostream>
#include <QDebug>

GUI::GUI(MainWindow* mainWindowInstance) : mainWindow(mainWindowInstance) {}

void GUI::logger(const std::string& message) {
    QEventLoop loop;
    QTimer::singleShot(1000, &loop, SLOT(quit()));
    loop.exec();
    qDebug() << "[LOG] " << QString::fromStdString(message);
}
