import cv2

login_username = "a0ms1n"
login_password = "6767"
ip = "192.168.1.101"
port = 6767

url = "http://{}:{}@{}:{}/video".format(login_username, login_password, ip, port)

# Open the stream
cap = cv2.VideoCapture(url)

if not cap.isOpened():
    print("Cannot open video stream")
    exit()

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
