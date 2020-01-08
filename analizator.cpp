#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioInput>
#include "analizator.h"
#include "ui_analizator.h"
#include "xyseriesiodevice.h"

Analizator::Analizator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Analizator)
{
    ui->setupUi(this);



    ///Urządzenie Audio
    QAudioFormat formatAudio;
    formatAudio.setSampleRate(44100);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleSize(8);
    formatAudio.setCodec("audio/pcm");
    formatAudio.setByteOrder(QAudioFormat::LittleEndian);
    formatAudio.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo inputDevices = QAudioDeviceInfo::defaultInputDevice();
    m_audioInput = new QAudioInput(inputDevices,formatAudio, this);

    m_device = new XYSeriesIODevice(ui->chart_1, ui->chart_2, this);
    m_device->open(QIODevice::WriteOnly);

    m_audioInput->start(m_device);
}

Analizator::~Analizator()
{
    m_audioInput->stop();
    m_device->close();
    delete ui;
}
