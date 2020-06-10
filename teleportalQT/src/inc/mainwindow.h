#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QMainWindow>
#include <QtWidgets>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QQuickWidget>

#include "VideoReceiver.h"

namespace AS
{
#include "ardusub_api.h"
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void resizeWindowsManual();

private slots:
    void updateVehicleData();
    void manualControl();

    void on_actionVideo_triggered();

    void on_actionMenu_triggered();

    void on_modeBt_clicked();               //Changing Combobox to Button

    void modeComboBox_currentIndexChanged(int index);

    void armCheckBox_stateChanged(int state);

private:
    Ui::MainWindow *ui;

    VideoReceiver *videoReceiver;

    QCheckBox *armCheckBox;
    QPushButton *modeComboBox;               //Changing Combobox to Button
    //QComboBox *modeComboBox;
    
    // status toolbar
    QLabel *yawLabelValue;
    QLabel *pitchLabelValue;
    QLabel *rollLabelValue;
    QLabel *depthLabelValue;

    // timer
    QTimer vehicleDataUpdateTimer;
    QTimer manualControlTimer;

    int m_modeIndex; 
    uint IdleTime;                        //Changin Combobox to Button
	bool	firstRun;                     // first run flag 2020/02/20

    typedef struct
    {
        int16_t x;
        int16_t y;
        int16_t z;
        int16_t r;
        uint16_t buttons;
    } manual_control_t;

    manual_control_t manual_control;

    typedef struct
    {
        bool W = false;
        bool S;
        bool A;
        bool D;
        bool Up;
        bool Down;
        bool Left;
        bool Right;
    } pressedKey_t;

    pressedKey_t pressedKey;

    typedef struct
    {
        int16_t forward;
        int16_t backward;
        int16_t leftward;
        int16_t rightward;
        int16_t upward;
        int16_t downward;
        int16_t turnLeft;
        int16_t turnRight;
        int16_t turnLeftM;
        int16_t turnRightM;

    } keyControlValue_t;

    keyControlValue_t keyControlValue;

    uint8_t currentVehicle;
    AS::Vehicle_Data_t *vehicle_data;

    void setupToolBars();
    void setupTimer();

    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};
#endif // MAINWINDOW_H
