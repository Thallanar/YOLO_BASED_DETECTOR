from ultralytics import YOLO
import cv2

#baixando o modelo
model = YOLO('yolov8n.pt')

model.export(format="onnx")

print(cv2.getBuildInformation())