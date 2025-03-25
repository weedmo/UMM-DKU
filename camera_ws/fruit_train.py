# -*- coding: utf-8 -*-
import torch
from pathlib import Path
from PIL import Image
import cv2
import numpy as np
import os

def train_model():
    os.chdir('yolov5')  # yolov5 디렉토리로 이동
    from train import train
    
    train(
        data="../fruit_detection.v1i.yolov5pytorch/data.yaml",  # 상대 경로로 변경
        imgsz=640,
        batch_size=16,
        epochs=500,
        weights="yolov5s.pt"
    )
    os.chdir('..')  # 원래 디렉토리로 복귀

def main():
    train_model()
    best_weights = 'yolov5/runs/train/exp/weights/best.pt'
    print("Training completed. Best weights saved at: {}".format(best_weights))

if __name__ == '__main__':
    main()