import cv2

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
cap.set(cv2.CAP_PROP_FPS, 60)
while True:
    ret, frame = cap.read()
    cv2.imshow('frame', frame)
    print('frame shape = {}X{}'.format(str(frame.shape[0]), str(frame.shape[1])))
    print('fps = {}'.format(cap.get(cv2.CAP_PROP_FPS)))
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
cap.release()
cv2.destroyAllWindows()