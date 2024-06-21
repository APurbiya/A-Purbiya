######## Webcam Object Detection Using Tensorflow-trained Classifier #########
#
# Author: Evan Juras
# Author: Arnav Purbiya
# Date: 10/27/19
# Update: 5/6/2024
# Description: 
# This program uses a TensorFlow Lite model to perform object detection on a live webcam
# feed. It draws boxes and scores around the objects of interest in each frame from the
# webcam. To improve FPS, the webcam object runs in a separate thread from the main program.
# This script will work with either a Picamera or regular USB webcam.
#
# This code is based off the TensorFlow Lite image classification example at:
# https://github.com/tensorflow/tensorflow/blob/master/tensorflow/lite/examples/python/label_image.py
#
# I added my own method of drawing boxes and labels using OpenCV.

# Import packages
#version 1.1
import os
import argparse
import cv2
import numpy as np
import sys
import time
from threading import Thread
import importlib.util
import serial

#------------------------------------------------------------ADDED-------------------------------------------------------------
import RPi.GPIO as GPIO

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(11, GPIO.OUT, initial=GPIO.HIGH) #Red
GPIO.setup(13, GPIO.OUT, initial=GPIO.HIGH) #Green
GPIO.setup(15, GPIO.OUT, initial=GPIO.HIGH) #Red

GPIO.setup(8, GPIO.OUT, initial=GPIO.HIGH)
GPIO.setup(10, GPIO.OUT, initial=GPIO.HIGH)
GPIO.setup(12, GPIO.OUT, initial=GPIO.HIGH)

ser = serial.Serial('/dev/ttyUSB0', 9600) #/dev/ttyUSB0
    

# Function to send data to Arduino based on conditions
def send_data_to_arduino(direction, box_size):
    # Send direction and box size to Arduino
    data_string = f"{direction},{box_size}"
    ser.write(data_string.encode())
    print(f"Sent data to Arduino: {data_string}")

# Define VideoStream class to handle streaming of video from webcam in separate processing thread
# Source - Adrian Rosebrock, PyImageSearch: https://www.pyimagesearch.com/2015/12/28/increasing-raspberry-pi-fps-with-python-and-opencv/
class VideoStream:

    """Camera object that controls video streaming from the Picamera"""
    def __init__(self,resolution=(640,480),framerate=30):
        # Initialize the PiCamera and the camera image stream
        self.stream = cv2.VideoCapture(0)
        ret = self.stream.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*'MJPG'))
        ret = self.stream.set(3,resolution[0])
        ret = self.stream.set(4,resolution[1])
            
        # Read first frame from the stream
        (self.grabbed, self.frame) = self.stream.read()

        # Variable to control when the camera is stopped
        self.stopped = False

    def start(self):
        # Start the thread that reads frames from the video stream
        Thread(target=self.update,args=()).start()
        return self

    def update(self):
        # Keep looping indefinitely until the thread is stopped
        while True:
            # If the camera is stopped, stop the thread
            if self.stopped:
                # Close camera resources
                self.stream.release()
                return

            # Otherwise, grab the next frame from the stream
            (self.grabbed, self.frame) = self.stream.read()

    def read(self):
        # Return the most recent frame
        return self.frame

    def stop(self):
        # Indicate that the camera and thread should be stopped
        self.stopped = True

# Define and parse input arguments
parser = argparse.ArgumentParser()
parser.add_argument('--modeldir', help='Folder the .tflite file is located in',
                    required=True)
parser.add_argument('--graph', help='Name of the .tflite file, if different than detect.tflite',
                    default='detect.tflite')
parser.add_argument('--labels', help='Name of the labelmap file, if different than labelmap.txt',
                    default='labelmap.txt')
parser.add_argument('--threshold', help='Minimum confidence threshold for displaying detected objects',
                    default=0.5)
parser.add_argument('--resolution', help='Desired webcam resolution in WxH. If the webcam does not support the resolution entered, errors may occur.',
                    default='1280x720')
parser.add_argument('--edgetpu', help='Use Coral Edge TPU Accelerator to speed up detection',
                    action='store_true')

args = parser.parse_args()

MODEL_NAME = args.modeldir
GRAPH_NAME = args.graph
LABELMAP_NAME = args.labels
min_conf_threshold = float(args.threshold)
resW, resH = args.resolution.split('x')
imW, imH = int(resW), int(resH)
use_TPU = args.edgetpu

# Import TensorFlow libraries
# If tflite_runtime is installed, import interpreter from tflite_runtime, else import from regular tensorflow
# If using Coral Edge TPU, import the load_delegate library
pkg = importlib.util.find_spec('tflite_runtime')
if pkg:
    from tflite_runtime.interpreter import Interpreter
    if use_TPU:
        from tflite_runtime.interpreter import load_delegate
else:
    from tensorflow.lite.python.interpreter import Interpreter
    if use_TPU:
        from tensorflow.lite.python.interpreter import load_delegate

# If using Edge TPU, assign filename for Edge TPU model
if use_TPU:
    # If user has specified the name of the .tflite file, use that name, otherwise use default 'edgetpu.tflite'
    if (GRAPH_NAME == 'detect.tflite'):
        GRAPH_NAME = 'edgetpu.tflite'       

# Get path to current working directory
CWD_PATH = os.getcwd()

# Path to .tflite file, which contains the model that is used for object detection
PATH_TO_CKPT = os.path.join(CWD_PATH,MODEL_NAME,GRAPH_NAME)

# Path to label map file
PATH_TO_LABELS = os.path.join(CWD_PATH,MODEL_NAME,LABELMAP_NAME)

# Load the label map
with open(PATH_TO_LABELS, 'r') as f:
    labels = [line.strip() for line in f.readlines()]

# Have to do a weird fix for label map if using the COCO "starter model" from
# https://www.tensorflow.org/lite/models/object_detection/overview
# First label is '???', which has to be removed.
if labels[0] == '???':
    del(labels[0])

# Load the Tensorflow Lite model.
# If using Edge TPU, use special load_delegate argument
if use_TPU:
    interpreter = Interpreter(model_path=PATH_TO_CKPT,
                              experimental_delegates=[load_delegate('libedgetpu.so.1.0')])
    print(PATH_TO_CKPT)
else:
    interpreter = Interpreter(model_path=PATH_TO_CKPT)

interpreter.allocate_tensors()

# Get model details
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()
height = input_details[0]['shape'][1]
width = input_details[0]['shape'][2]

floating_model = (input_details[0]['dtype'] == np.float32)

input_mean = 127.5
input_std = 127.5

# Initialize frame rate calculation
frame_rate_calc = 1
freq = cv2.getTickFrequency()

# Initialize video stream
videostream = VideoStream(resolution=(imW,imH),framerate=30).start()
time.sleep(1)

#for frame1 in camera.capture_continuous(rawCapture, format="bgr",use_video_port=True):
while True:

    # Start timer (for calculating frame rate)
    t1 = cv2.getTickCount()

    # Grab frame from video stream
    frame1 = videostream.read()

    # Acquire frame and resize to expected shape [1xHxWx3]
    frame = frame1.copy()
    frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    frame_resized = cv2.resize(frame_rgb, (width, height))
    input_data = np.expand_dims(frame_resized, axis=0)

    # Normalize pixel values if using a floating model (i.e. if model is non-quantized)
    if floating_model:
        input_data = (np.float32(input_data) - input_mean) / input_std

    # Perform the actual detection by running the model with the image as input
    interpreter.set_tensor(input_details[0]['index'],input_data)
    interpreter.invoke()

    # Retrieve detection results
    boxes = interpreter.get_tensor(output_details[0]['index'])[0] # Bounding box coordinates of detected objects
    classes = interpreter.get_tensor(output_details[1]['index'])[0] # Class index of detected objects
    scores = interpreter.get_tensor(output_details[2]['index'])[0] # Confidence of detected objects
    #num = interpreter.get_tensor(output_details[3]['index'])[0]  # Total number of detected objects (inaccurate and not needed)
    object_name = ""
    object_center_x = 0
    frame_center_x = 0
    center_x = 0
    ymin = 0
    xmin = 0
    ymax = 0
    xmax = 0
                
    # Loop over all detections and draw detection box if confidence is above minimum threshold
    for i in range(len(scores)):
        if ((scores[i] > min_conf_threshold) and (scores[i] <= 1.0)):
            # Get bounding box coordinates
            object_name = labels[int(classes[i])]

            # Check if the detected object is one of the classes of interest
            if object_name in ["person", "car", "truck", "bicycle"]:
                ymin = int(max(1,(boxes[i][0] * imH)))
                xmin = int(max(1,(boxes[i][1] * imW)))
                ymax = int(min(imH,(boxes[i][2] * imH)))
                xmax = int(min(imW,(boxes[i][3] * imW)))
                
                # Calculate the center of the detected object
                object_center_x = (xmin + xmax) // 2
                
                # Calculate the center of the frame
                frame_center_x = imW // 2
                
                # Calculate the center of the frame
                center_x = imW // 2

                # Draw a line at the center of the frame
                cv2.line(frame, (center_x, 0), (center_x, imH), (255, 0, 0), 2)

                # Determine if the person is on the left or right
                
                
                    
                # Set PIN 11 (GPIO output) based on person detection
            
                
                # Draw bounding box and label
                cv2.rectangle(frame, (xmin,ymin), (xmax,ymax), (10, 255, 0), 2)
                object_name = labels[int(classes[i])]
                label = '%s: %d%%' % (object_name, int(scores[i]*100))
                labelSize, baseLine = cv2.getTextSize(label, cv2.FONT_HERSHEY_SIMPLEX, 0.7, 2)
                label_ymin = max(ymin, labelSize[1] + 10)
                cv2.rectangle(frame, (xmin, label_ymin-labelSize[1]-10), (xmin+labelSize[0], label_ymin+baseLine-10), (255, 255, 255), cv2.FILLED)
                cv2.putText(frame, label, (xmin, label_ymin-7), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 0), 2)


    # Draw framerate in corner of frame
    cv2.putText(frame,'FPS: {0:.2f}'.format(frame_rate_calc),(30,50),cv2.FONT_HERSHEY_SIMPLEX,1,(255,255,0),2,cv2.LINE_AA)

    # All the results have been drawn on the frame, so it's time to display it.
    #cv2.imshow('Object detector', frame)

    #------------------------------------------------------------ADDED-------------------------------------------------------------
    # Define thresholds for bounding box sizes (in pixels)
    far_threshold = 5000  # Example threshold for far distance
    near_threshold = 200000  # Example threshold for near distance

    # Get the height of the bounding box (assuming it's a square box)
    box_width = xmax - xmin  # Calculate the height of the bounding box
    box_height = ymax - ymin  # Calculate the height of the bounding box
    
    box_sizes = box_height * box_width
    # Classify the distance based on the size of the bounding box
    

    
    #you will need to edit this object name check to something that actually exists in the TensorFlow model
    direction = ""

    center_margin = 80  # Margin around the center of the frame to consider the object as "centered"

    if object_name in ["person", "car", "truck", "bicycle"]:
        

        #GPIO.output(11, GPIO.HIGH)
        if abs(object_center_x - frame_center_x) < center_margin:
            direction = "C"  # Object is relatively in the center
        elif object_center_x > frame_center_x + center_margin:
            direction = "R"  # Object is on the right
        elif object_center_x < frame_center_x - center_margin:
            direction = "L"  # Object is on the left

        
        send_data_to_arduino(direction, box_sizes)
        
        if direction == "C":
            if box_sizes < far_threshold:
                GPIO.output(8, GPIO.LOW)#green
                GPIO.output(10, GPIO.HIGH)#red
                GPIO.output(12, GPIO.HIGH)#blue
                
                GPIO.output(15, GPIO.HIGH)#blue
                GPIO.output(11, GPIO.HIGH)#red
                GPIO.output(13, GPIO.LOW)#green

                time.sleep(0.5)
                GPIO.output(15, GPIO.HIGH)#blue
                GPIO.output(11, GPIO.HIGH)#red
                GPIO.output(13, GPIO.HIGH)#green

                GPIO.output(8, GPIO.HIGH)#green
                GPIO.output(10, GPIO.HIGH)#red
                GPIO.output(12, GPIO.HIGH)#blue
                print(f"g")

            elif far_threshold <= box_sizes < near_threshold:
                GPIO.output(15, GPIO.LOW)#blue
                GPIO.output(11, GPIO.HIGH)#red
                GPIO.output(13, GPIO.HIGH)#green

                GPIO.output(8, GPIO.HIGH) #green
                GPIO.output(10, GPIO.HIGH) #red
                GPIO.output(12, GPIO.LOW)#blue

                time.sleep(0.4)

                GPIO.output(12, GPIO.HIGH)#blue
                GPIO.output(15, GPIO.HIGH)#blue

                print(f"b")

            else:
                GPIO.output(15, GPIO.HIGH) #blue
                GPIO.output(11, GPIO.HIGH) #green
                GPIO.output(13, GPIO.LOW) #red

                GPIO.output(8, GPIO.LOW) #green
                GPIO.output(10, GPIO.HIGH) #red
                GPIO.output(12, GPIO.HIGH) #blue

                time.sleep(0.2)

                GPIO.output(13, GPIO.HIGH) #red
                GPIO.output(8, GPIO.HIGH) #red
                print(f"r")

        elif direction == "L":
            if box_sizes < far_threshold:
                GPIO.output(8, GPIO.LOW)#green
                GPIO.output(10, GPIO.HIGH)#red
                GPIO.output(12, GPIO.HIGH)#blue
                
                GPIO.output(15, GPIO.HIGH)#blue
                GPIO.output(11, GPIO.HIGH)#red
                GPIO.output(13, GPIO.HIGH)#green

                time.sleep(0.5)
                GPIO.output(15, GPIO.HIGH)#blue
                GPIO.output(11, GPIO.HIGH)#red
                GPIO.output(13, GPIO.HIGH)#green

                GPIO.output(8, GPIO.HIGH)#green
                GPIO.output(10, GPIO.HIGH)#red
                GPIO.output(12, GPIO.HIGH)#blue

            elif far_threshold <= box_height < near_threshold:
                GPIO.output(15, GPIO.HIGH)#blue
                GPIO.output(11, GPIO.HIGH)#red
                GPIO.output(13, GPIO.HIGH)#green

                GPIO.output(8, GPIO.HIGH) #green
                GPIO.output(10, GPIO.HIGH) #red
                GPIO.output(12, GPIO.LOW)#blue

                time.sleep(0.4)

                GPIO.output(12, GPIO.HIGH)#blue
                GPIO.output(15, GPIO.HIGH)#blue
            else:
                GPIO.output(15, GPIO.HIGH) #blue
                GPIO.output(11, GPIO.HIGH) #red
                GPIO.output(13, GPIO.HIGH) #green

                GPIO.output(8, GPIO.LOW) #green
                GPIO.output(10, GPIO.HIGH) #red
                GPIO.output(12, GPIO.HIGH) #blue

                time.sleep(0.2)

                GPIO.output(11, GPIO.HIGH) #red
                GPIO.output(8, GPIO.HIGH) #red

        elif direction == "R":
            if box_sizes < far_threshold:
                GPIO.output(8, GPIO.HIGH)#green
                GPIO.output(10, GPIO.HIGH)#red
                GPIO.output(12, GPIO.HIGH)#blue
                
                GPIO.output(15, GPIO.HIGH)#blue
                GPIO.output(11, GPIO.LOW)#green
                GPIO.output(13, GPIO.HIGH)#red

                time.sleep(0.5)
                GPIO.output(15, GPIO.HIGH)#blue
                GPIO.output(11, GPIO.HIGH)#red
                GPIO.output(13, GPIO.HIGH)#green

                GPIO.output(8, GPIO.HIGH)#green
                GPIO.output(10, GPIO.HIGH)#red
                GPIO.output(12, GPIO.HIGH)#blue
                print(f"1")
            elif far_threshold <= box_sizes < near_threshold:
                GPIO.output(15, GPIO.LOW)#blue
                GPIO.output(11, GPIO.HIGH)#red
                GPIO.output(13, GPIO.HIGH)#green

                GPIO.output(8, GPIO.HIGH) #green
                GPIO.output(10, GPIO.HIGH) #red
                GPIO.output(12, GPIO.HIGH)#blue

                time.sleep(0.4)

                GPIO.output(12, GPIO.HIGH)#blue
                GPIO.output(15, GPIO.HIGH)#blue
                print(f"2")
            else:
                GPIO.output(15, GPIO.HIGH) #blue
                GPIO.output(11, GPIO.HIGH) #Green
                GPIO.output(13, GPIO.LOW) #red

                GPIO.output(8, GPIO.HIGH) #green
                GPIO.output(10, GPIO.HIGH) #red
                GPIO.output(12, GPIO.HIGH) #blue

                time.sleep(0.2)

                GPIO.output(13, GPIO.HIGH) #red
                GPIO.output(8, GPIO.HIGH) #red
                print(f"r")
                print(f"3")
                
    else:
        direction = "X"
        send_data_to_arduino(direction, 0)
        GPIO.output(8, GPIO.HIGH)
        GPIO.output(15, GPIO.HIGH) #blue
        GPIO.output(10, GPIO.HIGH)
        GPIO.output(11, GPIO.HIGH) #red
        GPIO.output(12, GPIO.HIGH)
        GPIO.output(13, GPIO.HIGH) #green
        

    # Print or use the distance category as needed
    #print("Distance Category: ", distance_category)
    #print("Distance Num " , box_height , "  " , box_height, " ", box_width)


    # Calculate framerate
    t2 = cv2.getTickCount()
    time1 = (t2-t1)/freq
    frame_rate_calc= 1/time1

    # Press 'q' to quit
    #if cv2.waitKey(1) == ord('q'):
    #    break

# Clean up
GPIO.output(8, GPIO.HIGH)
GPIO.output(15, GPIO.HIGH) #blue
GPIO.output(10, GPIO.HIGH)
GPIO.output(11, GPIO.HIGH) #red
GPIO.output(12, GPIO.HIGH)
GPIO.output(13, GPIO.HIGH) #green

#cv2.destroyAllWindows()
#videostream.stop()
#6.1
