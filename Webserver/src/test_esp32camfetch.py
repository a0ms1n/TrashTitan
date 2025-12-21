import cv2

url = "http://192.168.1.190:6767/stream"
cap = cv2.VideoCapture(url)

if not cap.isOpened():
    print("Cannot open stream")
    exit()

while True:
    ret, frame = cap.read()
    if not ret:
        break
    cv2.imshow("ESP32-CAM Stream", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
