import asyncio
from config import *
import cv2

def generate_url():
    url = ""
    if CameraMode == "IPCamera":
        url = "http://{}:{}@{}:{}/video".format(IPCAM_CONFIG._login_username,IPCAM_CONFIG._login_password,IPCAM_CONFIG._ip,IPCAM_CONFIG._port)
    else:
        url = "http://{}:{}/stream".format(ESP32CAM_CONFIG._ip,ESP32CAM_CONFIG._port)
    return url

# Write Computing, and add model here, return object detected.
def start_server():
    url = generate_url()
    print("Connecting to ",url)
    cap = cv2.VideoCapture(url)

    if not cap.isOpened():
        print("Cannot open video stream")
        yield "STOP"
        return
    
    yield "START"

    while True:
        ret, frame = cap.read()
        if not ret:
            print("Failed to grab frame")
            break

        cv2.imshow("IP Webcam Stream", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

        

    cap.release()
    cv2.destroyAllWindows()
    yield "STOP"