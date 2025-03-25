# 🛒 UMM (Useful Market Mobility) – Autonomous Shopping Assistant for the Visually Impaired

An autonomous mobile robot designed to support **independent shopping for visually impaired individuals**.  
By combining SLAM, voice recognition, and object detection technologies, this mobility solution enhances accessibility and autonomy.


![image](https://github.com/user-attachments/assets/2224144b-39f6-4499-9f6f-8f1b7bc80df0)

---

## 📌 Project Overview

- **Project Title:** Autonomous Mobility for Visually Impaired Shoppers
- **Team Name:** EMOM
- **Department:** Mechanical Engineering, Dankook University
- **Team Members:** Byeongchun Park, Sang Yoon, Hansol Jang, Joonmo Han
- **Event:** 2024 UMM Campus Capstone Design Competition

---

## 🔧 Hardware Components

- 🔹 2D LiDAR
- 🔹 Mecanum wheels + Encoder motors (PD control)
- 🔹 Arduino Uno & Mega
- 🔹 Jetson Nano
- 🔹 Camera (YOLOv5 object detection)
- 🔹 RFID Reader
- 🔹 Bluetooth module (voice control via smartphone)

---

## 🧠 Software Stack

- 🐍 Python (Jetson-side)
- ⚙️ Arduino (C) for motor & sensor control
- 🤖 ROS Melodic
- 🧭 SLAM with Cartographer + LaserScanMatcher
- 📷 YOLOv5 for object recognition
- 📐 AutoCAD & Fusion 360 for hardware design

---

## 🗺️ How It Works

1. **Voice-Based Shopping Request**  
   - The user speaks the desired item into a smartphone app connected via Bluetooth.

2. **Localization & Navigation**  
   - Cartographer SLAM is used to build a map.
   - Real-time position is estimated using LiDAR + Encoder Odometry.
   - `move_base` navigates to the item's location.

3. **Item Identification**  
   - At the destination, the robot uses YOLOv5 to scan and detect the product via camera.
   - RFID is scanned for detailed product info.

4. **Information Delivery**  
   - The item’s name and info are sent to the smartphone and **spoken aloud** via Bluetooth audio.

5. **Return to Checkout**  
   - After scanning, the robot autonomously returns to the cashier area.

---

## 🧪 Key Innovations

- **SLAM-based Navigation:** Obstacle-aware indoor mapping and localization.
- **PD Motor Control:** Smooth omnidirectional mobility using Mecanum wheels.
- **Voice-Driven UX:** Full flow from voice command to voice feedback.
- **YOLOv5 + RFID Fusion:** Robust and accurate product identification.

---

## 🎥 Demo Video

[![Watch the demo video](https://img.youtube.com/vi/2G5HKrH4HdA/0.jpg)](https://youtu.be/2G5HKrH4HdA)

---

## ✍️ Reflections

> “This was my very first experience using ROS and Linux, and I struggled a lot at first.  
> I had no one to ask, and no one on the team was familiar with ROS. So I had to learn and build almost everything from scratch.  
> During the final month, I worked almost every night—sleeping very little—but I poured my heart into this project.  
> Looking back, this is the project that made me choose robotics as my career path.”
