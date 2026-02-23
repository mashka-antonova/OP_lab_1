#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

extern "C"{
#include "appcontext.h"
#include "entrypoint.h"
#include "memory.h"
#include "logic.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onInputChanged();
    void onConvertClicked();
    void onSwapClicked();
    void onCopyClicked();

    void on_catButton_clicked();

private:
    Ui::MainWindow *ui;
    AppContext context;
    bool isRenderingFromContext = false;

    void renderFromContext();
    QString errorText(Status errorCode);

};
#endif // MAINWINDOW_H
