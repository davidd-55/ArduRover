# ArduRover: A (soon-to-be) GPS-Powered Smart Rover
#### *By David D'Attile*

| ArduRover Platform|
|:-----------------:|
| ![ArduRover Platform](https://github.com/davidd-55/ArduRover/blob/main/docs/media/ArduRover_Profile.jpeg?raw=true) |

###### *[- Link to test drive video](https://youtu.be/Pe0to9lpWN0)*
###### *[- Link to repository](https://github.com/davidd-55/ArduRover)*

## Contents:
- [1. Introduction](#1-introduction)
- [2. Related Works](#2-related-works)
- [3. Project Goals](#4-project-goals)
- [4. Project Status](#4-project-status)
- [5. Reflection](#5-reflection)

## 1. Introduction

 As a Computer Science major at Pomona College, I have had plenty of classroom and real-world opportunities to explore software and its applications. However, I have not been nearly as exposed to interesting hardware problems throughout my education. This project was a personal attempt to fill that void while also focusing on topics that interest me. As a result, I decided to pursue a rover robotics project: ArduRover. 
 
 ArduRover is a GPS-enabled smart rover based on the Arduino hardware platform. This project served as a way to explore robotics, electronics, and software-for-hardware topics in a fun and accessible manner. This project's status was documented with bi-weekly [reports](https://github.com/davidd-55/ArduRover/tree/main/reports) over the course of the Fall 2022 semester, and this writeup serves as a final, extended report before the semester draws to a close.

## 2. Related Works

 Before diving headfirst into acquiring parts and learning the basics of electronics, I first brushed up on similar projects and related works taken on by others. Listed below are a few papers and projects I read through and my notes are linked [here](https://github.com/davidd-55/ArduRover/blob/main/literature/ArduRoverSourcesNotes.pdf).

 **Related Works:**
 1. "[Global Positioning System – Working and its Applications](https://link.springer.com/chapter/10.1007/978-1-4020-8735-6_84)" - Ahmad Raza, Shafqat hameed, and Tim Macintyre
 2. "[An Arduino UNO application: GPS Guided Unmanned Ground Vehicle](https://ieeexplore.ieee.org/document/8266283)" - Tugay Bozik and Guray Gurkan
 3. "[Tracing Path with Arduino Uno using GPS and GPRS/GSM](https://ieeexplore.ieee.org/document/8674953)" - Tarun Agrawal and Mohamaad Abdul Qadeer

 **Related Projects:**
 1. "[Arduino Powered Autonomous Vehicle](https://www.instructables.com/Arduino-Powered-Autonomous-Vehicle/)" - CPARKTX
 2. "[Arduino UNO Autonomous Car](https://create.arduino.cc/projecthub/hannahmcneelyy/arduino-uno-autonomous-car-c45fd1)" - hannahmcneelyy

 These related works and projects definitely streamlined the development and testing process for me, and their relevant contributions (especially for the projects) have been called out in current [code snippets](https://github.com/davidd-55/ArduRover/tree/main/code/DemoCode).

## 3. Project Goals

 After getting a better feel for what work had *already* been done in this space, I felt comfortable setting some project goals for myself while working on ArduRover. I split the goals into two groups, functional and personal, so that I could look back at the end of the semester and see what I was able to achieve in both areas.

 #### 3a. Functional Goals
 - Create a parts list of interopable components compass unit, GPS receiver, motor controller, etc.
 - Assemble a fully-functional (can drive, turn, etc.) robotic car chassis 
 - Control car chassis with an Arduino microcontroller
 - Enable autonomous driving capabilities with GPS + compass data

 #### 3b. Personal Goals
 - Develop and stick achievable milestone deadlines
 - Learn how to navigate open-source hardware docs 
 - Learn how to write software for hardware
 - Gain basic hardware engineering skills (soldering, wiring, etc.)
 - Work with Prof. Clark since this is his area of expertise!

## 4. Project Status

 #### 4a. Current Status

 #### 4c. Setbacks Encountered

 Somehow, I only encountered one major setback throughout the duration of this project (a huge win compared to what I expected). In November, I unfortunately fried the GPS shield I had purcahsed by providing too much voltage on accident from a plug-in power supply. This stalled my progress a bit since I was missing a functional GPS shield and probably contributed to my inability to get a fully-functioning navigation system up and running in time. Regardless, I was able to order a new GPS shield and am now using it to make progress on navigation software!

 #### 4c. Upcoming Work

 Unfortunately, I was not able to built out autonomous driving capabilities prior to the end of the Fall 2022 semester. Even though I replaced the fried GPS shield and had all the required hardware on-hand, I simply ran out of time to implement and test the software necessary for GPS waypoint navigation. Over the coming weeks, I hope to finalize the navigation software necessary for this feature as the rest of my workload winds down.

## 5. Reflection

 Despite ArduRover not being able to autonomously navigate between GPS waypoints (yet), I am still incredibly happy with how this project turned out! In the end, I accomplished all of the personal goals outlined in 3b and every functional goal with the exception of autonomous navigation capabilities. Altogether, I was able to gain valuable skills and knowledge in working on electronics and robotics projects. Further, I was able to reasonably self-guide this project and set appropriate timelines in the context of my other academic commitments throughout the semester.

 Overall, I'm very happy with how the project turned out, and I intend to keep working on it for fun during my final semester at Pomona. Finally, I want to give a huge thank you to Prof. Clark for the guidance and advice throughout the semester! :)

## 6. Gallery
 
