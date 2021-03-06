#ifndef ROBOCONTROLLERSDK_H
#define ROBOCONTROLLERSDK_H

#include <RoboControllerSDK_global.h>

#include <QThread>
#include <QTimer>
#include <QMutex>
#include <QString>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QTcpSocket>

#define ROBOT_CONFIG_INI_FILE "./robotConfig.ini"

#define SERVER_REPLY_TIMEOUT_MSEC 1000

#define UDP_PING_TIME_MSEC 1000

namespace roboctrl
{

class ROBOCONTROLLERSDKSHARED_EXPORT RoboControllerSDK : public QThread
{
    Q_OBJECT

public:
    explicit RoboControllerSDK(QString serverAddr=QString("127.0.0.1"),
                               quint16 udpStatusPortSend=14550,
                               quint16 udpStatusPortListen=14555,
                               quint16 udpControlPort=14560,
                               quint16 tcpPort=14500);

    virtual ~RoboControllerSDK();

    /** @brief Searches for the server on the local network
     *
     *
     * @returns IP of the server or an empty QString
     */
    static QString findServer(quint16 udpSendPort=14550, quint64 udpListenPort=14555 );

    /** @brief Send a request for motor speed.
     *         The reply is received with /ref newMotorSpeedValue
     *         signal
     *
     * @param motorIdx Index of the motor (0 or 1)
     */
    void getMotorSpeed( quint16 motorIdx );

    /** @brief Sets the speed of the motor in m/sec
     *
     * @param motorIdx Index of the motor (0 or 1)
     * @param speed The speed in m/sec - Speed range: [-32.768/+32.767] m/sec
     *
     * @note This function works only when @ref mMotorCtrlMode is
     *       equal to @ref mcPid, else it does nothing
     */
    void setMotorSpeed( quint16 motorIdx, double speed );

    /** @brief Send a request for motor pwm.
     *         The reply is received with /ref newMotorPwmValue
     *         signal
     *
     * @param motorIdx Index of the motor (0 or 1)
     */
    void getMotorPWM( quint16 motorIdx );

    /** @brief Sets the PWM of the motor in m/sec
     *
     * @param motorIdx Index of the motor (0 or 1)
     * @param pwm PWM range: [-2048/2047]
     *
     * @note This function works only when @ref mMotorCtrlMode is
     *       equal to @ref mcDirectPWM, else it does nothing
     */
    void setMotorPWM( quint16 motorIdx, int pwm );

    /** @brief Sets motor PID Controllers parameters.
     *
     * @param motorIdx Index of the motor (0 or 1)
     * @param Kp Proportional Action gain
     * @param Ki Integral Action gain
     * @param Kd Derivative Action gain
     */
    void setMotorPidGains( quint16 motorIdx, quint16 Kp, quint16 Ki, quint16 Kd );

    /** @brief Send a request for motor PID gains.
     *         The reply is received with @ref newMotorPIDGains
     *         signal
     *
     * @param motorIdx Index of the motor (0 or 1)
     */
    void getMotorPidGains( quint16 motorIdx );

    /** @brief Gets current board status (@ref BoardStatus)
     *         The reply is received with @ref newBoardStatus
     */
    void getBoardStatus();

    /** @brief Sets a new @ref BoardStatus
     *
     * @param status The new status to be set
     */
    void setBoardStatus( BoardStatus &status );

    /** @brief Load the Robot Configuration from ini file
     *         The Robot configuration is not stored on
     *         Robot EEPROM unless saving is active.
     *
     * @param iniFile The path of the ini file
     *
     * @return true if everything is ok
     */
    bool getRobotConfigurationFromIni( QString iniFile = ROBOT_CONFIG_INI_FILE );

    /** @brief Load the Robot Configuration from Robot EEPROM
     *         The Robot configuration is retrieved from Robot
     */
    void getRobotConfigurationFromEeprom( );

    /** @brief Save the Robot Configuration to Robot EEPROM
     *         The Robot configuration is saved on the Robot
     *
     * @param iniFile The path of the ini file
     */
    void saveRobotConfigurationToIni( QString iniFile = ROBOT_CONFIG_INI_FILE );

    /** @brief Save the Robot Configuration to Robot EEPROM
     *         The Robot configuration is saved on the Robot
     */
    void saveRobotConfigurationToEeprom( );

    /** @brief Set a new Robot Configuration without saving to EEPROM
     */
    void setRobotConfiguration( RobotConfiguration& roboConfig );

    /** @brief Try to take control of the robot for driving
     */
    void getRobotControl();

    /** @brief Release the control of the robot for other clients
     */
    void releaseRobotControl();

    /** @brief Gets the current charge value of battery
     *  The reply is received with @ref newBatteryValue
     *  signal
     */
    void getBatteryChargeValue();

    /** @brief Calibrates the Battery charge value
     *
     * @param maxChargeVal Maximum charge value in Volts
     * @param curChargeVal Current value of charge read with a tester connected to the battery
     */
    void setBatteryCalibrationParams(double maxChargeVal, double curChargeVal);

    //TODO: Implementare getWatchDogTime e setWatchDogTime

    /** @brief Disables the Communication Watchdog
     *         WatchDog if active stops motors if communication is lost
     */
    //void disableWatchdog();

    /** @brief Enables the Communication Watchdog
     *         WatchDog if active stops motors if communication is lost
     */
    //void enableWatchdog();

protected:
    /// Thread function
    virtual void run();

private:
    /// Operations performed during communication
    //void commThread();

    /// TCP Connection
    void connectToTcpServer();
    /// TCP Disconnection
    void disconnectTcpServer();

    /// UDP Connection
    void connectToUdpServers();
    /// UDP Disconnection
    void disconnectUdpServers();

    /// Processes a reply message
    void processReplyMsg(QDataStream *inStream );

    /// Updates Robot Configuration from data stream
    void updateRobotConfigurationFromDataStream( QDataStream* inStream );

    /// Sends a command to TCP server
    void sendBlockTCP( quint16 msgCode, QVector<quint16> &data );

    /// Sends a command to UDP server
    void sendBlockUDP( QUdpSocket *socket, QHostAddress addr, quint16 port, quint16 msgCode, QVector<quint16> &data, bool waitReply=false );

protected slots:
    /// Processes data from TCP Socket
    void onTcpReadyRead();
    /// Handles errors on TCP socket
    void onTcpError(QAbstractSocket::SocketError err);
    /// Handles the "Host Found" status
    void onTcpHostFound();

    /// Processes data from UDP Status Socket
    void onUdpStatusReadyRead();
    /// Processes data from UDP Control Socket
    //void onUdpControlReadyRead();
    /// Handles errors on UDP Status Socket
    void onUdpStatusError( QAbstractSocket::SocketError err );
    /// Handles errors on UDP Control Socket
    void onUdpControlError( QAbstractSocket::SocketError err );

    /// Send Test ping to UDP Servers
    void onUdpTestTimerTimeout();

    /// Ping Timer handler
    void onPingTimerTimeout();

signals:
    /// Signal emitted when TCP Socket is connected
    void tcpConnected();
    /// Signal emitted when TCP Socket is disconnected
    void tcpDisconnected();
    /// Signal emitted when UDP Socket is connected
    void udpConnected();
    /// Signal emitted when UDP Socket is disconnected
    void udpDisconnected();

    /// Signal emitted when a new PWM value is received
    void newMotorPwmValue( quint16 motorIdx, quint16 value );
    /// Signal emitted when a new SPEED value is received (speed is in m/sec)
    void newMotorSpeedValue( quint16 motorIdx, double value );
    /// Signal emitted when a new PWM value is received
    void newMotorPwmValue( quint16 motorIdx, double value );
    /// Signal emitted when new PID gains are received
    void newMotorPIDGains( quint16 motorIdx, quint16 Kp, quint16 Ki, quint16 Kd );
    /// Signal emitted when a new Robot Configuration is received from robot
    void newRobotConfiguration( RobotConfiguration& robConf );
    /// Signal emitted when a new Board Status is ready
    void newBoardStatus(BoardStatus& status);
    /// Signal emitted when a new Battery Value is available
    void newBatteryValue( double batChargeVal );

    /// Signal emitted when client takes Robot Control successfully
    void robotControlTaken();
    /// Signal emitted when client try to take Robot Control, but fails
    void robotControlNotTaken();
    /// Signal emitted when client releases the Robot Control
    void robotControlReleased();

private:
    qint64 mLastServerReqTime; /**< Information about last connection time */

    int mUdpStatusPortSend;      /**< Socket UDP Status Port to send datagrams*/
    int mUdpStatusPortListen;    /**< Socket UDP Status Port to bind to receive datagrams*/
    int mUdpControlPortSend;     /**< Socket UDP Control Port */
    int mTcpPort;          /**< Socket TCP Port */

    QString mServerAddr;   /**< Server address */

    QMutex mConnMutex;  /**< Mutex to share connection between functions safely */

    QTcpSocket* mTcpSocket;         /**< TCP Socket for secure communications */
    QUdpSocket* mUdpStatusSocket;   /**< UDP Socket for status communications */
    QUdpSocket* mUdpControlSocket;   /**< UDP Socket for control communications */

    quint16 mNextTcpBlockSize;      /**< Used to recover incomplete TCP block */
    quint16 mNextUdpStBlockSize;    /**< Used to recover incomplete UDP Status block */
    quint16 mNextUdpCtrlBlockSize;  /**< Used to recover incomplete UDP Control block */

    bool mTcpConnected;     /**< Indicates if TCP Socket is connected */
    bool mUdpConnected;     /**< Indicates if UDP Socket is connected */

    MotorCtrlMode mMotorCtrlMode; /**< Current motor control mode */
    BoardStatus mBoardStatus; /**< Current board status */

    RobotConfiguration mRobotConfig; /**< Configuration of the robot */

    int mBoardIdx;          /**< Id of the connected board */

    QString mLastTcpErrorMsg; /**< Last TCP Socket Error */
    QString mLastUdpErrorMsg; /**< Last UDP Socket Error */

    bool mStopped; /**< Thread stopped */
    quint64 mWatchDogTimeMsec; /**< Board WatchDog Time in millisecond*/
    QTimer mPingTimer; /** Id of the ping timer. Ping is called with a time smaller of 10% than watchdog time to mantain the board active */

    bool            mWatchDogEnable;
    bool            mNewStatusBit1Received;
    quint16         mStatusBits1;
    bool            mNewStatusBit2Received;
    quint16         mStatusBits2;

    quint16         mMsgCounter; /**< Counts the number of message sent */

    bool mReceivedStatus2; /**< Indicates if a new WORD_STATUSBIT2 has been received.
                                If both @ref mReceivedStatus2 and @ref mReceivedRobConfig
                                are true a @ref newRobotConfiguration signal can be emitted.*/
    bool mReceivedRobConfig; /**< Indicates if a new RobotConfig has been received from EEPROM.
                                  If both @ref mReceivedStatus2 and @ref mReceivedRobConfig
                                  are true a @ref newRobotConfiguration signal can be emitted. */

    QTimer mUdpPingTimer; /**< Timer of Udp Servers testing */
};

}

#endif // ROBOCONTROLLERSDK_H
