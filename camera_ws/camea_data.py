import cv2
import numpy as np
from threading import Thread
import time
import os
from datetime import datetime

class CSICamera:
    def __init__(self, capture_width=1280, capture_height=720, display_width=1280, display_height=720, framerate=30, flip_method=0):
        # CSI 카메라를 위한 GStreamer 파이프라인 구성
        self.gstreamer_pipeline = (
            f"nvarguscamerasrc ! "
            f"video/x-raw(memory:NVMM), "
            f"width=(int){capture_width}, height=(int){capture_height}, "
            f"format=(string)NV12, framerate=(fraction){framerate}/1 ! "
            f"nvvidconv flip-method={flip_method} ! "
            f"video/x-raw, width=(int){display_width}, height=(int){display_height}, format=(string)BGRx ! "
            f"videoconvert ! "
            f"video/x-raw, format=(string)BGR ! appsink"
        )
        
        # 카메라 초기화
        self.cap = cv2.VideoCapture(self.gstreamer_pipeline, cv2.CAP_GSTREAMER)
        self.grabbed, self.frame = self.cap.read()
        self.stopped = False
        
        # 저장 디렉토리 생성
        self.save_dir = 'captured_frame2'
        os.makedirs(self.save_dir, exist_ok=True)
        
        # 프레임 카운터
        self.frame_count = 0

    def start(self):
        Thread(target=self.update, args=()).start()
        return self

    def update(self):
        while True:
            if self.stopped:
                return
            self.grabbed, self.frame = self.cap.read()

    def read(self):
        return self.frame

    def save_frame(self, frame):
        # 현재 시간을 포함한 파일명 생성
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S_%f")
        filename = f"{self.save_dir}/frame_{self.frame_count:06d}_{timestamp}.jpg"
        cv2.imwrite(filename, frame)
        self.frame_count += 1
        return filename

    def stop(self):
        self.stopped = True
        self.cap.release()

def preprocess_for_yolo(frame, target_size=(640, 640)):
    """프레임을 YOLO 입력 형식으로 전처리"""
    original_height, original_width = frame.shape[:2]
    
    ratio = min(target_size[0] / original_width, target_size[1] / original_height)
    new_size = (int(original_width * ratio), int(original_height * ratio))
    
    resized = cv2.resize(frame, new_size, interpolation=cv2.INTER_LINEAR)
    
    pad_w = target_size[0] - new_size[0]
    pad_h = target_size[1] - new_size[1]
    top, bottom = pad_h // 2, pad_h - (pad_h // 2)
    left, right = pad_w // 2, pad_w - (pad_w // 2)
    
    image = cv2.copyMakeBorder(resized, top, bottom, left, right, 
                              cv2.BORDER_CONSTANT, value=(114, 114, 114))
    
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    image = image.astype(np.float32) / 255.0
    image = np.transpose(image, (2, 0, 1))
    image = np.expand_dims(image, axis=0)
    
    return image, ratio, (pad_w, pad_h)

def main():
    # CSI 카메라 초기화 및 시작
    camera = CSICamera().start()
    print("카메라 초기화 중...")
    time.sleep(2.0)  # 카메라 웜업
    
    try:
        while True:
            # 프레임 획득
            frame = camera.read()
            if frame is None:
                continue
            
            # YOLO 전처리
            processed_frame, ratio, padding = preprocess_for_yolo(frame)
            
            # 프레임 저장
            saved_file = camera.save_frame(frame)
            print(f"프레임 저장됨: {saved_file}")
            
            # 원본 프레임 표시
            cv2.imshow('CSI Camera Feed', frame)
            
            # 'q' 키를 누르면 종료
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
            
            # 프레임 캡처 간격 (필요한 경우 조절)
            time.sleep(0.25)  # 0.1초 간격으로 캡처
            
    finally:
        camera.stop()
        cv2.destroyAllWindows()
        print("프로그램 종료")

if __name__ == '__main__':
    main()