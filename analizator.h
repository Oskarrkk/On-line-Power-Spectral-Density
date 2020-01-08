#ifndef ANALIZATOR_H
#define ANALIZATOR_H

#include <QMainWindow>
QT_BEGIN_NAMESPACE
class QAudioInput;
QT_END_NAMESPACE
class XYSeriesIODevice;
namespace Ui {
class Analizator;
}

class Analizator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Analizator(QWidget *parent = nullptr);
    ~Analizator();

private:
    Ui::Analizator *ui;
    QAudioInput *m_audioInput;
    XYSeriesIODevice *m_device;

};

#endif // ANALIZATOR_H
