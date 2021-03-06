#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QProcess>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <QString>
#include <unistd.h>
#include <ios>
#include <fstream>
#include <string>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
using namespace std;
  QObject *parent;
  QProcess* launch = new QProcess(parent);

std::string com1()
{
    string cmd="docker ps";
    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
      while (!feof(stream))
        if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
      pclose(stream);
    }
  cout << "LS: " << data << endl;

    const std::string s = data;
    return s;
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    const QString s1 = QString::fromStdString( com1() );
    ui -> textBrowser_2->append(s1);

    QProcess p0;
    p0.QProcess::start("docker images");
    p0.waitForFinished(-1);
    ui->textBrowser->setText(p0.readAllStandardOutput());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    system("notify-send 'Methlab - Launcher' \"Starting container in VNC mode...\"");
    QString path="/bin/bash";
    QString command="docker run -it --rm -p 5901:5901 -p 6080:6080 --shm-size=2G mathworks/matlab:r2022a -vnc";
    launch->start(path,command.split(" "));
    launch->waitForFinished(-1);
    ui->textBrowser_2->setText(launch->readAllStandardOutput());
    ui->statusbar->showMessage("Container started...");
}


void MainWindow::on_pushButton_3_clicked()
{
    launch->terminate();
    system("notify-send 'Methlab - Launcher' \"Container stopped.\"");
    ui->statusbar->showMessage("Container stopped...");
}


void MainWindow::on_pushButton_4_clicked()
{
    QProcess p;
    p.QProcess::start("docker ps");
    p.waitForFinished(-1);
    ui->textBrowser_2->setText(p.readAllStandardOutput());
}


void MainWindow::on_pushButton_2_clicked()
{
    system("notify-send 'Methlab - Launcher' \"Opening VNC Viewer...\"");
    QProcess p1;
    p1.QProcess::start("vncviewer -useaddressbook matlab");
    p1.waitForFinished(-1);

}

