#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QLabel>
#include <robocontrollersdk.h>

// >>>>> INI names
#ifdef ANDROID
#define INI_FOLDER "RobotGUI"
#endif
#define ROB_IP "robot_ip"
#define ROB_TCP_PORT "robot_tcp_port"
#define ROB_UDP_CTRL_PORT "robot_udp_control_port"
#define ROB_UDP_STAT_PORT_SEND "robot_udp_status_port_send"
#define ROB_UDP_STAT_PORT_LISTEN "robot_udp_status_port_listen"
#define PID_ENABLED "pid_enabled"
// <<<<< INI names


using namespace roboctrl;

namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();
    
private slots:
    void on_actionPidEnabled_triggered();
    void onConnectButtonClicked();
    void onFindServerButtonClicked();
    void onNewJoypadValues(float x, float y);
    void onNewMotorSpeed( quint16 mot, double speed );
    void onNewRobotConfiguration( RobotConfiguration& robConf );
    void onNewBatteryValue( double battVal);

    void on_actionRobot_Configuration_triggered();

protected:
    virtual void resizeEvent(QResizeEvent * ev) Q_DECL_OVERRIDE;
    virtual bool eventFilter(QObject *obj, QEvent *ev) Q_DECL_OVERRIDE;

    virtual void timerEvent( QTimerEvent* event ) Q_DECL_OVERRIDE;

private:
    Ui::CMainWindow *ui;

    // >>>> GUI Objects
    QSettings* mIniSettings;
    QString mRobIpAddress;
    QLineEdit* mRobIpLineEdit;
    int mRobTcpPort;
    int mRobUdpControlPort;
    int mRobUdpStatusPortSend;
    int mRobUdpStatusPortListen;
    QPushButton* mPushButtonConnect;
    QPushButton* mPushButtonFindServer;

    QLabel* mStatusLabel;
    QLabel* mBatteryLabel;
    // <<<< GUI Objects

    bool mPidEnabled;
    bool mOpenRobotConfig; /*!< Indicates that Robot Configuration dlg must be opened when receiving a new configuration message */
    bool mRobotConfigValid; /*!< Indicates that a new valid Robot Configuration has been received */

    bool mMotorSpeedLeftValid;
    bool mMotorSpeedRightValid;
    double mMotorSpeedRight;
    double mMotorSpeedLeft;

    RoboControllerSDK* mRoboCtrl; /*!< Pointer to RoboControllerSDK object */
    RobotConfiguration mRoboConf; /*!< Robot Configuration */

    float mMaxMotorSpeed; /*!< Max linear speed for each motor */

    int mSpeedReqTimer;
    int mStatusReqTimer;
};

#endif // CMAINWINDOW_H
