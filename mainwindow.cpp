#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QSignalBlocker>
#include <string>

//для кота
#include <QDialog>
#include <QVBoxLayout>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
#include <QUrl>

QString MainWindow::errorText(Status errorCode){
    QString text;

    switch (errorCode) {
    case STATUS_OK:
        text = "";
        break;
    case ERR_EMPTY_INPUT:
        text = "input is empty.";
        break;
    case ERR_INVALID_CHAR:
        text = "input string doesn't match selected base format.";
        break;
    case ERR_INVALID_BASE:
        text = "base must be in range [2..36].";
        break;
    case ERR_OVERFLOW:
        if (context.inputBase == DECIMAL_BASE) {
            text = "input value is outside int32 range.";
        } else {
            text = "input value is outside uint32 range.";
        }
        break;
    case ERR_NO_MEMORY:
        text = "not enough memory.";
        break;
    case ERR_INTERNAL:
        text = "internal error.";
        break;
    default:
        break;
    }

    return text;
}

void MainWindow::renderFromContext(){
    isRenderingFromContext = true;
    ui->inputBaseSpin->setValue(context.inputBase);
    ui->outputBaseSpin->setValue(context.outputBase);

    if (context.inputNum != nullptr)
        ui->inputNumEdit->setText(QString::fromUtf8(context.inputNum));
    else
        ui->inputNumEdit->clear();

    ui->errorLabel->setText(errorText(context.errorCode));

    if (context.resultNum != nullptr)
        ui->resultEdit->setText(QString::fromUtf8(context.resultNum));
    else
        ui->resultEdit->clear();
    isRenderingFromContext = false;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->resultEdit->setReadOnly(true);
    ui->errorLabel->clear();

    doOperation(&context, INIT, nullptr);

    connect(ui->inputNumEdit, &QLineEdit::textChanged, this, &MainWindow::onInputChanged);
    connect(ui->inputBaseSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onInputChanged); //
    connect(ui->outputBaseSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onInputChanged);

    connect(ui->convertButton, &QPushButton::clicked, this, &MainWindow::onConvertClicked);
    connect(ui->swapButton, &QPushButton::clicked, this, &MainWindow::onSwapClicked);
    connect(ui->copyButton, &QPushButton::clicked, this, &MainWindow::onCopyClicked);

    renderFromContext();
}

MainWindow::~MainWindow()
{
    freeContext(&context);
    delete ui;
}

void MainWindow::onInputChanged(){
    if (!isRenderingFromContext){
        AppParams param;
        std::string str = ui->inputNumEdit->text().toStdString();

        context.inputBase = ui->inputBaseSpin->value();
        context.outputBase = ui->outputBaseSpin->value();
        param.inputNum = str.c_str();

        doOperation(&context, INPUT_CHANGED, &param);
        renderFromContext();
    }
}

void MainWindow::onConvertClicked(){
    std::string str = ui->inputNumEdit->text().toStdString();
    AppParams param = {
        .inputNum = str.c_str()
    };
    context.inputBase = ui->inputBaseSpin->value();
    context.outputBase = ui->outputBaseSpin->value();

    doOperation(&context, CONVERT_CLICKED, &param);
    renderFromContext();
}

void MainWindow::onSwapClicked(){
    doOperation(&context, SWAP_CLICKED, nullptr);
    renderFromContext();
}

void MainWindow::onCopyClicked(){
    QClipboard *cb = QGuiApplication::clipboard();
    cb->setText(ui->resultEdit->text());
}

void MainWindow::on_catButton_clicked(){
    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle("cat");
    dlg->setModal(true);
    dlg->resize(800, 450);

    QVideoWidget *video = new QVideoWidget(dlg);
    QVBoxLayout *lay = new QVBoxLayout(dlg);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->addWidget(video);

    QMediaPlayer *player = new QMediaPlayer(dlg);
    QAudioOutput *audio = new QAudioOutput(dlg);

    player->setAudioOutput(audio);
    player->setVideoOutput(video);
    player->setSource(QUrl("qrc:/media/help.MP4"));

    audio->setVolume(0.6);

    connect(player, &QMediaPlayer::mediaStatusChanged, dlg,
            [dlg](QMediaPlayer::MediaStatus status){
                if (status == QMediaPlayer::EndOfMedia) {
                    dlg->accept();
                }
            });

    player->play();
    dlg->exec();
    dlg->deleteLater();
}
