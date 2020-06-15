#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ping-message-all.h>
#include <ping-parser.h>
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

    //SETUP STARTING VALUES

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
	
	firstRun = false;  // first run flag 2020/02/20

    keyControlValue.forward = 500;
    keyControlValue.backward = -500;
    keyControlValue.leftward = -500;
    keyControlValue.rightward = 500;
    keyControlValue.upward = 800;
    keyControlValue.downward = 200;
    keyControlValue.turnLeft = -600;
    keyControlValue.turnRight = 600;
    keyControlValue.turnLeftM = -300;
    keyControlValue.turnRightM = 300;

    m_modeIndex = 1;  
    IdleTime = 1;                          //Changing Combobox to Buttton
   


    on_actionVideo_triggered();

    setupToolBars();
    setFocusPolicy(Qt::StrongFocus);    //FOCUS FIX
    setFocus();                         //FOCUS FIX
    std::string ip("192.168.2.");
    AS::as_api_init(ip.c_str(), F_THREAD_NAMED_VAL_FLOAT | F_STORAGE_NONE);

    setupTimer();
	
    videoReceiver->start(ui->quickWidget);
	
}

MainWindow::~MainWindow()
{
	// On Shutdown - DisArm Vehicle & Set Manual Mode

	armCheckBox->setChecked(false);
    //modeComboBox->setCurrentIndex(1);
    m_modeIndex = 0;                                     //Changing Combobox for Button
	armCheckBox_stateChanged(Qt::Unchecked);
	//modeComboBox_currentIndexChanged(0);	
    modeComboBox_currentIndexChanged(m_modeIndex);      //Changing Combobox for Button
		
    delete ui;
}

void MainWindow::setupToolBars()
{
    //ui->vehicleToolBar->addAction(new QAction(QIcon("../assets/icons/video.svg"),"SonarGps"));
    QList<QAction *> actionListDisarm;
    // actionListDisarm.append(ui->actionDisarm);
    ui->vehicleToolBar->setFocusPolicy(Qt::NoFocus);        //FOCUS FIX
    ui->vehicleToolBar->addActions(actionListDisarm);

    armCheckBox = new QPushButton(this);
     armCheckBox->setText("CLICK TO START");
     armCheckBox->setCheckable(true);
     armCheckBox->setChecked(false);
    armCheckBox->setStyleSheet("color: rgb(0, 206, 0);font: 87 12pt \"Arial\";");
    armCheckBox->setFocusPolicy(Qt::NoFocus);               //FOCUS FIX
    AddToolBarSpacer(ui->vehicleToolBar);
    ui->vehicleToolBar->addWidget(armCheckBox);
    AddToolBarSpacer(ui->vehicleToolBar);
    connect(armCheckBox,SIGNAL(clicked(bool)),
                     this,SLOT(armCheckBox_stateChanged(bool)));

    ui->vehicleToolBar->addSeparator();
    AddToolBarSpacer(ui->tabsToolBar,100);
    QLabel *modeLable = new QLabel(this);
    modeLable->setText("Mode: ");
    ui->tabsToolBar->addWidget(modeLable);

    //Vehicle Mode Change using ComboBox

    // modeComboBox = new QComboBox(this);
    // modeComboBox->addItem("Manual");
    // modeComboBox->addItem("Stabilize");
    // modeComboBox->addItem("Depth Hold");
    // modeComboBox->setFocusPolicy(Qt::NoFocus);               //FOCUS FIX
    // ui->vehicleToolBar->addWidget(modeComboBox);
    // QObject::connect(modeComboBox, 
    //                 static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
    //                 this, 
    //                 &MainWindow::modeComboBox_currentIndexChanged);

    //Vehicle Mode Change using Button
    modeComboBox = new QPushButton(this);
    modeComboBox->setText("Stabilise");
    modeComboBox->setFocusPolicy(Qt::NoFocus);
    modeComboBox->show();
    connect (modeComboBox , SIGNAL(clicked()) , this , SLOT(on_modeBt_clicked()) );
    ui->tabsToolBar->addWidget(modeComboBox);
   



    // status toolbar
    QLabel *yawLabel = new QLabel("Compass: ", this);
    yawLabel->setFocusPolicy(Qt::NoFocus);          //FOCUS FIX
    yawLabelValue = new QLabel("0.00", this);
    yawLabelValue->setFocusPolicy(Qt::NoFocus);     //FOCUS FIX

    QLabel *pitchLabel = new QLabel("Pitch: ", this);
    pitchLabel->setFocusPolicy(Qt::NoFocus);        //FOCUS FIX
    pitchLabelValue = new QLabel("0.00", this);
    pitchLabelValue->setFocusPolicy(Qt::NoFocus);      //FOCUS FIX

    QLabel *rollLabel = new QLabel("Roll: ", this);
    rollLabel->setFocusPolicy(Qt::NoFocus);             //FOCUS FIX
    rollLabelValue = new QLabel("0.00", this);
    rollLabelValue->setFocusPolicy(Qt::NoFocus);        //FOCUS FIX
  //hide
    pitchLabel->setVisible(false);
    pitchLabelValue->setVisible(false);
    rollLabel->setVisible(false);
    rollLabelValue->setVisible(false);

    QLabel *depthLabel = new QLabel("Depth: ", this);
    depthLabel->setFocusPolicy(Qt::NoFocus);            //FOCUS FIX
    depthLabelValue = new QLabel("0.00", this);
    depthLabelValue->setFocusPolicy(Qt::NoFocus);       //FOCUS FIX

    yawLabelValue->setFixedWidth(50);
    pitchLabelValue->setFixedWidth(50);
    rollLabelValue->setFixedWidth(50);
    depthLabelValue->setFixedWidth(50);	
   

    AddToolBarSpacer(ui->statusToolBar);
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

    ui->menuPage->setStyleSheet("border-image: url(:/assets/keyboardControls.png);");
    ResizeToolBar();
}


        //Changing from ComboBox to Button
void MainWindow::on_modeBt_clicked(){

    m_modeIndex ++;
    m_modeIndex %= 3;

    modeComboBox_currentIndexChanged(m_modeIndex);
}

void MainWindow::setupTimer()
{
    QObject::connect(&vehicleDataUpdateTimer, &QTimer::timeout, this, &MainWindow::updateVehicleData);
    vehicleDataUpdateTimer.setInterval(25);
    vehicleDataUpdateTimer.start();

    QObject::connect(&manualControlTimer, &QTimer::timeout, this, &MainWindow::manualControl);
    manualControlTimer.setInterval(50);
    manualControlTimer.start();
}

void MainWindow::updateVehicleData()
{
    if (!AS::as_api_check_vehicle(currentVehicle))
    {
        // qDebug() << "vehicle: " << currentVehicle << "is not ready!";
        return;
    }

	if (!firstRun)
	{
		// On Startup - Arm Vehicle & Set Stabilise Mode
		
        // Using Combobox
		 // armCheckBox->setChecked(true);
		 // modeComboBox->setCurrentIndex(1);		
		 // armCheckBox_stateChanged(Qt::Checked);
		 // modeComboBox_currentIndexChanged(1);

         	armCheckBox->setChecked(true);
            m_modeIndex = 1;                                     //Changing Combobox for Button
            armCheckBox_stateChanged(true);
            modeComboBox_currentIndexChanged(m_modeIndex);      //Changing Combobox for Button
		    firstRun = true;
	}

    AS::as_api_get_vehicle_data2(currentVehicle, vehicle_data);

    if (vehicle_data == nullptr)
    {
        // qDebug() << "no vehicle data!";
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
    yawLabelValue->setNum(round(yaw * 100) / 100.0);
    depthLabelValue->setNum(round(depth * 100) / 100.0);

    //Trying to Disarm after 120sec of idle
        LASTINPUTINFO LastInput = {}; 
        LastInput.cbSize = sizeof(LastInput); 
        ::GetLastInputInfo(&LastInput); 
        uint IdleTime = (::GetTickCount() - LastInput.dwTime)/1000;
        if ((IdleTime > 120) == true)
        {
            armCheckBox->setChecked(false);
            armCheckBox_stateChanged(Qt::Unchecked);
            
        }
}

void MainWindow::manualControl()
{
        //Main Vehicle Control Loop
    if (armCheckBox->isChecked()&&
        AS::as_api_check_vehicle(currentVehicle))
    {
        AS::as_api_manual_control(manual_control.x, manual_control.y,
                                  manual_control.z, manual_control.r,
                                  manual_control.buttons, currentVehicle);

        
        // qDebug() << "manual control:" << manual_control.x << manual_control.y << manual_control.z << manual_control.r;
        
	       //manual_control.buttons = 0;        //THIS IS BUG FROM ORIGINAL CODE
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
    //ui->vehicleToolBar->
    //ui->statusToolBar

    ui->tabsToolBar->setFixedWidth( ui->centralwidget->width()/3);
    ui->vehicleToolBar->setFixedWidth( ui->centralwidget->width()/3);
    ui->statusToolBar->setFixedWidth( ui->centralwidget->width()/3);
}

void MainWindow::AddToolBarSpacer(QToolBar *pToolBar,int width)
{
    QWidget *spacer = new QWidget(this);
    spacer->setFocusPolicy(Qt::NoFocus);                //FOCUS FIX
    if(width<0)
    {
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    }
    else
    {
        spacer->setFixedWidth(width);

    }
    pToolBar->setFocusPolicy(Qt::NoFocus);     //FOCUS FIX
    pToolBar->addWidget(spacer);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    resizeWindowsManual();

    QMainWindow::resizeEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
        //Keyboard Controls - Key Pressed
    if (event->isAutoRepeat())
    {
        return;
    }

    if (event->key() == Qt::Key_W)
    {
        if (!armCheckBox->isChecked())
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
        
        qDebug() << "You Pressed Key W";
        pressedKey.W = true;
        manual_control.z = keyControlValue.upward;
    }
    else if (event->key() == Qt::Key_S)
    {
        if (!armCheckBox->isChecked())
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
        qDebug() << "You Pressed Key S";
        pressedKey.S = true;
        manual_control.z = keyControlValue.downward;
    }
    else if (event->key() == Qt::Key_A)
    {
        if (!armCheckBox->isChecked())
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
        qDebug() << "You Pressed Key A";
        pressedKey.A = true;
        if (m_modeIndex == 0)
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
        if (!armCheckBox->isChecked())
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
        qDebug() << "You Pressed Key D";
        pressedKey.D = true;
        if (m_modeIndex == 0)
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
        if (!armCheckBox->isChecked())
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
        qDebug() << "You Pressed Key Up";
        pressedKey.Up = true;
        manual_control.x = keyControlValue.forward;
    }
    else if (event->key() == Qt::Key_Down)
    {
        if (!armCheckBox->isChecked())
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
        qDebug() << "You Pressed Key Down";
        pressedKey.Down = true;
        manual_control.x = keyControlValue.backward;
    }
    else if (event->key() == Qt::Key_Left)
    {
        if (!armCheckBox->isChecked())
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
        qDebug() << "You Pressed Key Left";
        pressedKey.Left = true;
        manual_control.y = keyControlValue.leftward;
    }
    else if (event->key() == Qt::Key_Right)
    {
        if (!armCheckBox->isChecked())
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
        qDebug() << "You Pressed Key Right";
        pressedKey.Right = true;
        manual_control.y = keyControlValue.rightward;
    }
     
	//ADAM- Added new keys for Camera Tilt, Lights, Mode Change
	
	else if (event->key() == Qt::Key_R)
     {
        if (!armCheckBox->isChecked())
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
         qDebug() << "You Pressed Key R";
         manual_control.buttons = 1024;
     }
     else if (event->key() == Qt::Key_F)
     {
        if (!armCheckBox->isChecked())
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
         qDebug() << "You Pressed Key F";
         manual_control.buttons = 512;
     }
   else if (event->key() == Qt::Key_T)
     {
        if (!armCheckBox->isChecked())
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
         qDebug() << "You Pressed Key Plus";
         manual_control.buttons = 16384;
     }
     else if (event->key() == Qt::Key_G)
     {
        if (!armCheckBox->isChecked())
        {
            armCheckBox->setChecked(true);
            armCheckBox_stateChanged(true);
            return;
         }
         qDebug() << "You Pressed Key Minus";
         manual_control.buttons = 8192;
     }
      else if (event->key() == Qt::Key_1)
     {
         qDebug() << "You Pressed Key 1";
         manual_control.buttons = 2;
     }
   else if (event->key() == Qt::Key_2)
     {
         qDebug() << "You Pressed Key 2";
         manual_control.buttons = 8;
     }
     else if (event->key() == Qt::Key_3)
     {
         qDebug() << "You Pressed Key 3";
         manual_control.buttons = 4;
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
            //Keyboard Controls - Key Released
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
        qDebug() << "You Released Key Plus";
        manual_control.buttons = 0;
        
    }
 else if (event->key() == Qt::Key_G)
    {
        qDebug() << "You Released Key Minus";
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
}

void MainWindow::on_actionMenu_triggered()
{
    ui->mainStackedWidget->setCurrentIndex(1);
    ui->actionMenu->setChecked(true);
    ui->actionMenu->setDisabled(true);
    ui->actionVideo->setChecked(false);
    ui->actionVideo->setDisabled(false);
}

void MainWindow::modeComboBox_currentIndexChanged(int index)
{
            //Change Mode of the Vehicle using manual_control.buttons
    if (!AS::as_api_check_vehicle(currentVehicle))
    {
        qDebug() << "vehicle: " << currentVehicle << "is not ready!";
        return;
    }

    switch (index)
    {
    case 0:
        //AS::as_api_set_mode(currentVehicle, AS::MANUAL);    //Mode changed using API
        manual_control.buttons = 2;
        modeComboBox->setText("Manual");                        //Vehicle Mode Change using Button
        break;

    case 1:
        //AS::as_api_set_mode(currentVehicle, AS::STABILIZE);   //Mode changed using API
        manual_control.buttons = 8;
        modeComboBox->setText("Stabilise");                     //Vehicle Mode Change using Button
        break;

    case 2:
        //AS::as_api_set_mode(currentVehicle, AS::ALT_HOLD);   //Mode changed using API
        manual_control.buttons = 4;
        modeComboBox->setText("Depth Hold");                //Vehicle Mode Change using Button
        break;

    default:
        break;
    }
}

void MainWindow::armCheckBox_stateChanged(bool checked)
{
            //Change vehicle arm / disarm
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
        // set manual control value to zero
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

}
