#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMetaObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , videoReceiver(new VideoReceiver(this))
    , currentVehicle(1)
    , vehicle_data(new AS::Vehicle_Data_t)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/assets/icons/main.svg"));
    QCoreApplication::setOrganizationName("Teleportal");
    QCoreApplication::setOrganizationDomain("");
    QCoreApplication::setApplicationName("Teleportal");


    //SETUP STARTING VALUES FOR KEYBOARD INPUT

    manual_control.x = 0;
    manual_control.y = 0;
    manual_control.z = 500;
    manual_control.r = 0;
    manual_control.buttons = 0;

    pressedKey.W = false;
    pressedKey.S = false;
    pressedKey.A = false;
    pressedKey.D = false;
    pressedKey.Up = false;
    pressedKey.Down = false;
    pressedKey.Left = false;
    pressedKey.Down = false;

    keyControlValue.forward = 700;
    keyControlValue.backward = -700;
    keyControlValue.leftward = -700;
    keyControlValue.rightward = 700;
    keyControlValue.upward = 900;
    keyControlValue.downward = 100;
    keyControlValue.turnLeft = -700;
    keyControlValue.turnRight = 700;
    keyControlValue.turnLeftM = -300;
    keyControlValue.turnRightM = 300;


    //SETUP MISC VARIABLES

    m_modeIndex = 2;  
    IdleTime = 1;   
    firstRun = false;
   

    //SETUP VIDEO AND TOOLBAR

    on_actionVideo_triggered();
    setupToolBars();


    //SETUP FOCUS POLICY

    setFocusPolicy(Qt::StrongFocus);
    setFocus();                     


    //INITILIZE & CONNECT TO ROBOT

    std::string ip("192.168.2.");
    AS::as_api_init(ip.c_str(), F_THREAD_NAMED_VAL_FLOAT | F_STORAGE_NONE);


    //START MAIN LOOP

    setupTimer();
    videoReceiver->start(ui->quickWidget);

    //Load MAPS
    connect(ui->quickWidget_2,SIGNAL(statusChanged(QQuickWidget::Status)),this,SLOT(on_statusChanged(QQuickWidget::Status)));
    ui->quickWidget_2->setSource(QUrl(QStringLiteral("qrc:/assets/maps.qml")));
    pingLink=new PingSensor(this);
    PrevTime=QTime::currentTime();
    connect(pingLink,SIGNAL(distanceConfidenceChanged()),this,SLOT(on_updateConfidence()));
    rollLPitchCheckTimer.setInterval(10000);
    connect(&rollLPitchCheckTimer, &QTimer::timeout, this, [this] {
        CheckRollOrPitchChang(true);
    });
    rollLPitchCheckTimer.start();

}


MainWindow::~MainWindow()
{
	//ON SHUTDOWN DISARM ROBOT

	armCheckBox->setChecked(false);
	armCheckBox_stateChanged(Qt::Unchecked);
    delete ui;
}

void MainWindow::setupToolBars()
{
	//SETUP TOOLBAR

    QList<QAction *> actionListDisarm;
    ui->vehicleToolBar->setFocusPolicy(Qt::NoFocus);
    ui->vehicleToolBar->addActions(actionListDisarm);

    armCheckBox = new QPushButton(this);
     armCheckBox->setText("CLICK TO START");
     armCheckBox->setCheckable(true);
     armCheckBox->setChecked(false);
    armCheckBox->setStyleSheet("color: rgb(0, 206, 0);font: 87 12pt \"Arial\";");
    armCheckBox->setFocusPolicy(Qt::NoFocus);
    AddToolBarSpacer(ui->vehicleToolBar);
    ui->vehicleToolBar->addWidget(armCheckBox);
    AddToolBarSpacer(ui->vehicleToolBar);
    connect(armCheckBox,SIGNAL(clicked(bool)),
                     this,SLOT(armCheckBox_stateChanged(bool)));
  //  ui->vehicleToolBar->addSeparator();
    AddToolBarSpacer(ui->tabsToolBar,100);
    QLabel *modeLable = new QLabel(this);
    modeLable->setText("Mode: ");
    ui->tabsToolBar->addWidget(modeLable);
    modeComboBox = new QPushButton(this);
    modeComboBox->setText("Depth Hold");
    modeComboBox->setFocusPolicy(Qt::NoFocus);
    modeComboBox->show();
    connect (modeComboBox , SIGNAL(clicked()) , this , SLOT(on_modeBt_clicked()) );
    ui->tabsToolBar->addWidget(modeComboBox);

    SonarLabel=new QLabel("Sonar: ");
    SonarlValue = new QLabel("21.0m(95%)   ");
    SonarlValue->setFocusPolicy(Qt::NoFocus);

    QLabel *yawLabel = new QLabel("Compass: ", this);
    yawLabel->setFocusPolicy(Qt::NoFocus);  
    yawLabelValue = new QLabel("0.00", this);
    yawLabelValue->setFocusPolicy(Qt::NoFocus);

    QLabel *pitchLabel = new QLabel("Pitch: ", this);
    pitchLabel->setFocusPolicy(Qt::NoFocus);
    pitchLabelValue = new QLabel("0.00", this);
    pitchLabelValue->setFocusPolicy(Qt::NoFocus);

    QLabel *rollLabel = new QLabel("Roll: ", this);
    rollLabel->setFocusPolicy(Qt::NoFocus);
    rollLabelValue = new QLabel("0.00", this);
    rollLabelValue->setFocusPolicy(Qt::NoFocus);
  

    //HIDE UNUSED ROLL & PITCH VALUES

    pitchLabel->setVisible(false);
    pitchLabelValue->setVisible(false);
    rollLabel->setVisible(false);
    rollLabelValue->setVisible(false);
    yawLabel->setVisible(false);
    yawLabelValue->setVisible(false);

    QLabel *depthLabel = new QLabel("Depth: ", this);
    depthLabel->setFocusPolicy(Qt::NoFocus);
    depthLabelValue = new QLabel("0.00", this);
    depthLabelValue->setFocusPolicy(Qt::NoFocus);

    yawLabelValue->setFixedWidth(50);
    pitchLabelValue->setFixedWidth(50);
    rollLabelValue->setFixedWidth(50);
    depthLabelValue->setFixedWidth(50);	
   

    AddToolBarSpacer(ui->statusToolBar);
    ui->statusToolBar->addWidget(SonarLabel);
    ui->statusToolBar->addWidget(SonarlValue);
    ui->statusToolBar->addWidget(yawLabel);
    ui->statusToolBar->addWidget(yawLabelValue);
    ui->statusToolBar->addWidget(pitchLabel);
    ui->statusToolBar->addWidget(pitchLabelValue);
    ui->statusToolBar->addWidget(rollLabel);
    ui->statusToolBar->addWidget(rollLabelValue);
    ui->statusToolBar->addWidget(depthLabel);
    ui->statusToolBar->addWidget(depthLabelValue);

    QLabel *bannerLabel = new QLabel(this);
    bannerLabel->setFixedWidth(145);
    bannerLabel->setFixedHeight(15);
    bannerLabel->setStyleSheet("border-image: url(:/assets/logo/Logo-Large.png);");
    ui->statusToolBar->addWidget(bannerLabel);

    ui->menuPage->setStyleSheet("border-image: url(:/assets/keyboardControls.png);");  //DISPLAY KEYBOARD CONTROLS HELP IMAGE

    ui->tabsToolBar->setStyleSheet("QToolBar { border-left-style: none; border-right-style: none; }");
    ui->vehicleToolBar->setStyleSheet("QToolBar { border-left-style: none; border-right-style: none; }");
    ui->statusToolBar->setStyleSheet("QToolBar { border-left-style: none; border-right-style: none; }");
    strRollValue=rollLabelValue->text();
    strPitchValue=pitchLabelValue->text();
    ResizeToolBar();
}


void MainWindow::on_modeBt_clicked(){

	//CHANGES MODE BUTTON ON CLICK
    m_modeIndex ++;
    m_modeIndex %= 3;
    modeComboBox_currentIndexChanged(m_modeIndex);
}


void MainWindow::setupTimer()
{
    //MAIN TIMER LOOP

    QObject::connect(&vehicleDataUpdateTimer, &QTimer::timeout, this, &MainWindow::updateVehicleData);
    vehicleDataUpdateTimer.setInterval(25);
    vehicleDataUpdateTimer.start();

    QObject::connect(&manualControlTimer, &QTimer::timeout, this, &MainWindow::manualControl);
    manualControlTimer.setInterval(50);
    manualControlTimer.start();
}


void MainWindow::updateVehicleData()
{
	//UPDATES ROBOT VARIABLES

    if (!AS::as_api_check_vehicle(currentVehicle))
    {
        return;
    }

	if (!firstRun)
	{
		//CHANGE MODE TO DEPTH HOLD ON STARTUP

         	armCheckBox->setChecked(false);
            m_modeIndex = 2;
            armCheckBox_stateChanged(true);
            modeComboBox_currentIndexChanged(m_modeIndex);
		    firstRun = true;
	}

    AS::as_api_get_vehicle_data2(currentVehicle, vehicle_data);

    if (vehicle_data == nullptr)
    {
        return;
    }

    float yaw = 0, roll = 0, pitch = 0, depth = 0;

    const float degreePerRad = 180.0f / 3.1415926f;
    yaw = vehicle_data->yaw * degreePerRad;
    roll = vehicle_data->roll * degreePerRad;
    pitch = vehicle_data->pitch * degreePerRad;
    depth = vehicle_data->alt / 1000.0f;

    ui->qADI->setData(roll, pitch);
    ui->qCompass->setYaw(yaw);
    ui->qCompass->setAlt(depth);

    rollLabelValue->setNum(round(roll * 100) / 100.0);
    pitchLabelValue->setNum(round(pitch * 100) / 100.0);
    double yawLableCompass=round(yaw * 100) / 100.0;
    yawLabelValue->setNum(yawLableCompass);
    depthLabelValue->setNum(round(depth * 100) / 100.0);

    ui->qCompass->setAlt(yawLableCompass);//2020/06/19
    ui->qCompass->setYaw(yawLableCompass);
    if(ui->quickWidget_2->status()==QQuickWidget::Ready)
    {
        QQuickItem* pImgItem=ui->quickWidget_2->rootObject()->findChild<QQuickItem*>("markerimg");
        if(pImgItem)
        {
            pImgItem->setRotation(yawLableCompass);

        }
    }
    //IF USER IS IDLE FOR 180 SEC DISARM ROBOT

        LASTINPUTINFO LastInput = {}; 
        LastInput.cbSize = sizeof(LastInput); 
        ::GetLastInputInfo(&LastInput); 
        uint IdleTime = (::GetTickCount() - LastInput.dwTime)/1000;
        if ((IdleTime > 180) == true)
        {
            armCheckBox->setChecked(false);
            armCheckBox_stateChanged(Qt::Unchecked);
            
        }
    CheckRollOrPitchChang(false);

}


void MainWindow::manualControl()
{
        //MAIN ROBOT CONTROL LOOP

    if (armCheckBox->isChecked()&&
        AS::as_api_check_vehicle(currentVehicle))
    {
        AS::as_api_manual_control(manual_control.x, manual_control.y,
                                  manual_control.z, manual_control.r,
                                  manual_control.buttons, currentVehicle);
    }
}


void MainWindow::resizeWindowsManual()
{
    int m_width = ui->videoPage->width();

    ui->quickWidget->setGeometry(0, 0, m_width, ui->videoPage->height());

    ui->qCompass->setGeometry(m_width - 160, 0, 160, 160);
    ui->qADI->setGeometry(m_width - 160, 160, 160, 160);
    ResizeToolBar();
}


void MainWindow::ResizeToolBar()
{
    ui->tabsToolBar->setFixedWidth( ui->centralwidget->width()/3);
    ui->vehicleToolBar->setFixedWidth( ui->centralwidget->width()/3);
    ui->statusToolBar->setFixedWidth( ui->centralwidget->width()/3);
}


void MainWindow::AddToolBarSpacer(QToolBar *pToolBar,int width)
{
    QWidget *spacer = new QWidget(this);
    spacer->setFocusPolicy(Qt::NoFocus);
    if(width<0)
    {
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    }
    else
    {
        spacer->setFixedWidth(width);

    }
    pToolBar->setFocusPolicy(Qt::NoFocus);
    pToolBar->addWidget(spacer);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    resizeWindowsManual();
    QMainWindow::resizeEvent(event);
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //KEYBOARD CONTROLS FOR ROBOT

    if (event->isAutoRepeat())
    {
        return;
    }

    if (event->key() == Qt::Key_W)
    {
        if (!armCheckBox->isChecked())		//CHECK IF ROBOT IS ARMED, IF NOT REARM ROBOT
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
        
        qDebug() << "You Pressed Key W";
        pressedKey.W = true;
        manual_control.z = keyControlValue.upward;		//SEND COMMAND TO ROBOT
    }
    else if (event->key() == Qt::Key_S)
    {
        if (!armCheckBox->isChecked())		//CHECK IF ROBOT IS ARMED, IF NOT REARM ROBOT
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
        qDebug() << "You Pressed Key S";
        if(!SonarAlarm)
        {
            pressedKey.S = true;
            manual_control.z = keyControlValue.downward;		//SEND COMMAND TO ROBOT
        }
    }
    else if (event->key() == Qt::Key_A)
    {
        if (!armCheckBox->isChecked())		//CHECK IF ROBOT IS ARMED, IF NOT REARM ROBOT
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
        qDebug() << "You Pressed Key A";
        pressedKey.A = true;
        if (m_modeIndex == 0)			//CHECK MODE OF ROBOT AND GIVE CORRECT VALUE BASED ON MODE
        {
            manual_control.r = keyControlValue.turnLeftM;     
        }
        else if (m_modeIndex == 1)
        {
            manual_control.r = keyControlValue.turnLeft;     
        }
        else if (m_modeIndex == 2)
        {
            manual_control.r = keyControlValue.turnLeft;     
        }
    }
    else if (event->key() == Qt::Key_D)
    {
        if (!armCheckBox->isChecked())		//CHECK IF ROBOT IS ARMED, IF NOT REARM ROBOT
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
        qDebug() << "You Pressed Key D";
        pressedKey.D = true;
        if (m_modeIndex == 0)		//CHECK MODE OF ROBOT AND GIVE CORRECT VALUE BASED ON MODE
        {
            manual_control.r = keyControlValue.turnRightM;     
        }
        else if (m_modeIndex == 1)
        {
           manual_control.r = keyControlValue.turnRight;     
        }
        else if (m_modeIndex == 2)
        {
            manual_control.r = keyControlValue.turnRight;     
        }
    }
    else if (event->key() == Qt::Key_Up)
    {
        if (!armCheckBox->isChecked())		//CHECK IF ROBOT IS ARMED, IF NOT REARM ROBOT
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
        qDebug() << "You Pressed Key Up";
        if(!SonarAlarm)
        {
            pressedKey.Up = true;
            manual_control.x = keyControlValue.forward;		//SEND COMMAND TO ROBOT
        }

    }
    else if (event->key() == Qt::Key_Down)
    {
        if (!armCheckBox->isChecked())		//CHECK IF ROBOT IS ARMED, IF NOT REARM ROBOT
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
        qDebug() << "You Pressed Key Down";
        pressedKey.Down = true;
        manual_control.x = keyControlValue.backward;		//SEND COMMAND TO ROBOT
    }
    else if (event->key() == Qt::Key_Left)
    {
        if (!armCheckBox->isChecked())		//CHECK IF ROBOT IS ARMED, IF NOT REARM ROBOT
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
        qDebug() << "You Pressed Key Left";
        pressedKey.Left = true;
        manual_control.y = keyControlValue.leftward;		//SEND COMMAND TO ROBOT
    }
    else if (event->key() == Qt::Key_Right)
    {
        if (!armCheckBox->isChecked())		//CHECK IF ROBOT IS ARMED, IF NOT REARM ROBOT
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
        qDebug() << "You Pressed Key Right";
        pressedKey.Right = true;
        manual_control.y = keyControlValue.rightward;		//SEND COMMAND TO ROBOT
    }
	else if (event->key() == Qt::Key_R)
     {
        if (!armCheckBox->isChecked())		//CHECK IF ROBOT IS ARMED, IF NOT REARM ROBOT
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
         qDebug() << "You Pressed Key R";
         manual_control.buttons = 1024;		//SEND COMMAND TO ROBOT
     }
     else if (event->key() == Qt::Key_F)
     {
        if (!armCheckBox->isChecked())		//CHECK IF ROBOT IS ARMED, IF NOT REARM ROBOT
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
         qDebug() << "You Pressed Key F";
         manual_control.buttons = 512;		//SEND COMMAND TO ROBOT
     }
   else if (event->key() == Qt::Key_T)
     {
        if (!armCheckBox->isChecked())		//CHECK IF ROBOT IS ARMED, IF NOT REARM ROBOT
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
         qDebug() << "You Pressed Key Plus";
         manual_control.buttons = 16384;		//SEND COMMAND TO ROBOT
     }
     else if (event->key() == Qt::Key_G)
     {
        if (!armCheckBox->isChecked())		//CHECK IF ROBOT IS ARMED, IF NOT REARM ROBOT
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
         qDebug() << "You Pressed Key Minus";
         manual_control.buttons = 8192;		//SEND COMMAND TO ROBOT
     }

     	//KEYBOARD COMMANDS TO CHANGE MODE OF ROBOT

      else if (event->key() == Qt::Key_1)
     {
         qDebug() << "You Pressed Key 1";
         manual_control.buttons = 2;		//SEND COMMAND TO ROBOT
     }
   else if (event->key() == Qt::Key_2)
     {
         qDebug() << "You Pressed Key 2";
         manual_control.buttons = 8;		//SEND COMMAND TO ROBOT
     }
     else if (event->key() == Qt::Key_3)
     {
         qDebug() << "You Pressed Key 3";
         manual_control.buttons = 4;		//SEND COMMAND TO ROBOT
     }
    else
    {
        qDebug() << "You Pressed NOT supported Key";
    }

    qDebug() << "x: " << manual_control.x;
    qDebug() << "y: " << manual_control.y;
    qDebug() << "z: " << manual_control.z;
    qDebug() << "r: " << manual_control.r;
}


void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
 	//ON KEY RELEASE CHANGE ROBOT COMMANDS BACK TO IDLE VALUES

    if (event->isAutoRepeat())
    {
        return;
    }

    if (event->key() == Qt::Key_W)
    {
        qDebug() << "You Released Key W";
        if (pressedKey.S)
        {
            manual_control.z = keyControlValue.downward;
        }
        else
        {
            manual_control.z = 500;
        }
        pressedKey.W = false;
    }
    else if (event->key() == Qt::Key_S)
    {
        qDebug() << "You Released Key S";
        if (pressedKey.W)
        {
            manual_control.z = keyControlValue.upward;
        }
        else
        {
            manual_control.z = 500;
        }
        pressedKey.S = false;
    }
    else if (event->key() == Qt::Key_A)
    {
        qDebug() << "You Released Key A";
        if (pressedKey.D)
        {
            manual_control.r = keyControlValue.turnRight;
        }
        else
        {
            manual_control.r = 0;
        }
        pressedKey.A = false;
    }
    else if (event->key() == Qt::Key_D)
    {
        qDebug() << "You Released Key D";
        if (pressedKey.A)
        {
            manual_control.r = keyControlValue.turnLeft;
        }
        else
        {
            manual_control.r = 0;
        }
        pressedKey.D = false;
    }
    else if (event->key() == Qt::Key_Up)
    {
        qDebug() << "You Released Key Up";
        if (pressedKey.Down)
        {
            manual_control.x = keyControlValue.backward;
        }
        else
        {
            manual_control.x = 0;
        }
        pressedKey.Up = false;
    }
    else if (event->key() == Qt::Key_Down)
    {
        qDebug() << "You Released Key Down";
        if (pressedKey.Up)
        {
            manual_control.x = keyControlValue.forward;
        }
        else
        {
            manual_control.x = 0;
        }
        pressedKey.Down = false;
    }
    else if (event->key() == Qt::Key_Left)
    {
        qDebug() << "You Released Key Left";
        if (pressedKey.Right)
        {
            manual_control.y = keyControlValue.rightward;
        }
        else
        {
            manual_control.y = 0;
        }
        pressedKey.Left = false;
    }
    else if (event->key() == Qt::Key_Right)
    {
        qDebug() << "You Released Key Right";
        if (pressedKey.Left)
        {
            manual_control.y = keyControlValue.leftward;
        }
        else
        {
            manual_control.y = 0;
        }
        pressedKey.Right = false;
    }
            //ADAM: Added new buttons
 else if (event->key() == Qt::Key_R)
    {
        qDebug() << "You Released Key R";
        manual_control.buttons = 0;
        
    }
 else if (event->key() == Qt::Key_F)
    {
        qDebug() << "You Released Key F";
        manual_control.buttons = 0;
        
    }
 else if (event->key() == Qt::Key_T)
    {
        qDebug() << "You Released Key T";
        manual_control.buttons = 0;
        
    }
 else if (event->key() == Qt::Key_G)
    {
        qDebug() << "You Released Key G";
        manual_control.buttons = 0;
        
    }
    else
    {
        qDebug() << "You Released NOT supported Key";
    }

    qDebug() << "x: " << manual_control.x;
    qDebug() << "y: " << manual_control.y;
    qDebug() << "z: " << manual_control.z;
    qDebug() << "r: " << manual_control.r;
}


void MainWindow::on_actionVideo_triggered()
{
    ui->mainStackedWidget->setCurrentIndex(0);
    ui->actionVideo->setChecked(true);
    ui->actionVideo->setDisabled(true);
    ui->actionMenu->setChecked(false);
    ui->actionMenu->setDisabled(false);
    ui->actionSonarGps->setChecked(false);
    ui->actionSonarGps->setDisabled(false);
}


void MainWindow::on_actionMenu_triggered()
{
    ui->mainStackedWidget->setCurrentIndex(1);
    ui->actionMenu->setChecked(true);
    ui->actionMenu->setDisabled(true);
    ui->actionVideo->setChecked(false);
    ui->actionVideo->setDisabled(false);
    ui->actionSonarGps->setChecked(false);
    ui->actionSonarGps->setDisabled(false);
}


void MainWindow::modeComboBox_currentIndexChanged(int index)
{
            //CHANGE MODE OF ROBOT WHEN BUTTON IS CLICKED

    if (!AS::as_api_check_vehicle(currentVehicle))
    {
        qDebug() << "vehicle: " << currentVehicle << "is not ready!";
        return;
    }

    switch (index)
    {
    case 0:
        manual_control.buttons = 2;
        modeComboBox->setText("Manual");
        break;

    case 1:
        manual_control.buttons = 8;
        modeComboBox->setText("Stabilise");
        break;

    case 2:
        manual_control.buttons = 4;
        modeComboBox->setText("Depth Hold");
        break;

    default:
        break;
    }
}


void MainWindow::armCheckBox_stateChanged(bool checked)
{
            //ARM & DISARM ROBOT

    if (!AS::as_api_check_vehicle(currentVehicle))
    {
        qDebug() << "vehicle: " << currentVehicle << "is not ready!";
        armCheckBox->setChecked(false);
        armCheckBox->setStyleSheet("color: rgb(0, 206, 0);font: 87 12pt \"Arial\";");
        armCheckBox->setText("CLICK TO START");
        return;
    }

    if (armCheckBox->isChecked())
    {
        AS::as_api_vehicle_arm(currentVehicle, 1);
        armCheckBox->setStyleSheet("color: rgb(255, 0, 0);font: 87 12pt \"Arial\";");
        armCheckBox->setText("ARMED");
        qDebug() << "ARM";
    }
    else
    {
        manual_control.x = 0;
        manual_control.y = 0;
        manual_control.z = 500;
        manual_control.r = 0;
        manual_control.buttons = 0;

        AS::as_api_vehicle_disarm(currentVehicle, 1);
        armCheckBox->setStyleSheet("color: rgb(0, 206, 0);font: 87 12pt \"Arial\";");
        armCheckBox->setText("CLICK TO START");
        qDebug() << "DISARM";
    }
}

void MainWindow::on_actionSonarGps_triggered()
{
    ui->mainStackedWidget->setCurrentIndex(2);
    ui->actionSonarGps->setChecked(true);
    ui->actionSonarGps->setDisabled(true);
    ui->actionMenu->setChecked(false);
    ui->actionMenu->setDisabled(false);
    ui->actionVideo->setChecked(false);
    ui->actionVideo->setDisabled(false);

}

void MainWindow::on_updateConfidence()
{
    //SonarlValue
    float fDistance=pingLink->getDistance()/1000.0;
    float fConfidence=pingLink->getConfidence();
    QString strValue=QString("%1m(%2\%)   ").arg(fDistance).arg(fConfidence);
    SonarlValue->setText(strValue);
    if(fConfidence<ConfidenceSetting)
        return;
    QString strLabelName="Sonar: ";//normal
    QString strNormalsty="color: rgb(0, 0, 0);";
    QTime tcurrent=QTime::currentTime();
    if(fDistance>WarnDistance)
    {
        SonarAlarm=false;
        strLabelName="SONAR: ";
        strNormalsty="color: rgb(0, 85, 0);";
        PrevTime=tcurrent;
    }
    else if(fDistance>MinDistance&&fDistance<WarnDistance)
    {
        SonarAlarm=false;
        strLabelName="WARNING SONAR: ";
        strNormalsty="color: rgb(245, 81, 0);";
        PrevTime=tcurrent;
    }
    else if(fDistance<MinDistance)
    {
        SonarAlarm=true;
        strLabelName="--DANGER-- SONAR: ";
        strNormalsty="color: rgb(255, 0, 0);";
        manual_control.x = 0;
        manual_control.y = 0;
        manual_control.z = 500;
        manual_control.r = 0;
        manual_control.buttons = 0;

        if(PrevTime.msecsTo(tcurrent)/1000>AlarmSetting)
        {
            armCheckBox->setChecked(false);
            armCheckBox_stateChanged(true);
            PrevTime=tcurrent;
        }
    }
    SonarLabel->setText(strLabelName);
    SonarLabel->setStyleSheet(strNormalsty);
    SonarlValue->setStyleSheet(strNormalsty);
}

void MainWindow::on_statusChanged(QQuickWidget::Status status)
{
    if(status==QQuickWidget::Ready)
    {
        qmlTimer=ui->quickWidget_2->rootObject()->findChild<QObject*>("qmlTimer");


    }
}
void MainWindow::on_mainStackedWidget_currentChanged(int arg1)
{

    if(qmlTimer)
    {
        if(arg1==2)
        {
            QMetaObject::invokeMethod(qmlTimer,"start",Qt::QueuedConnection);
        }
        else
        {
            QMetaObject::invokeMethod(qmlTimer,"stop",Qt::QueuedConnection);
        }

    }
}
void MainWindow::CheckRollOrPitchChang(bool bTimerOut)
{

    QString strRoll=rollLabelValue->text();
    QString strPitch=pitchLabelValue->text();

    if(strRollValue!=strRoll||strPitchValue!=strPitch)
    {
        strRollValue=strRoll;
        strPitchValue=strPitch;

        //timer restart
        rollLPitchCheckTimer.start();
    }
    else
    {
        if(bTimerOut)
        {
            //restart ping
            RestartNetWork();
        }
    }
}

void MainWindow::RestartNetWork()
{
    rollLPitchCheckTimer.stop();
    std::string ip("192.168.2.");
    AS::as_api_init(ip.c_str(), F_THREAD_NAMED_VAL_FLOAT|F_STORAGE_NONE);
    armCheckBox->setChecked(false);
    armCheckBox_stateChanged(Qt::Unchecked);
    //rest connect
    pingLink->connectLink();
    rollLPitchCheckTimer.start();
}
