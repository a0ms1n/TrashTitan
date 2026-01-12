# "IPCam" or "ESP32Cam"
CameraMode = "IPCamera"

# MQTT
class MQTT_CONFIG:
    _client_id = "a0ms1n-mqtt-server"
    _username = "a0ms1n"
    _password = "D$N-676769"
    _broker = ("broker.hivemq.com", 1883, 60)

# ESP32
class ESP32CAM_CONFIG:
    _ip = "192.168.1.190"
    _port = 6767


# IP-CAM
class IPCAM_CONFIG:
    _login_username = "a0ms1n"
    _login_password = "6767"
    _ip = "10.10.9.71"
    _port = 6767
