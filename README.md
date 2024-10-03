# NanoRos
Nano is a robot built from scratch for navigation in various complex environments, with the ability to be controlled from a personal computer. It features a camera for data collection and live broadcasting from the Nano itself.


## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Features](#features)

## installation
this project need to install ,ros noitec, serial arduino pkg, TCP pkg for esp8266

## usage
you can run the project from launch file in nano pkg

## features
  Feature 1: nano used rrt path planing algorithm for generating path in maps
  Feature 2: nano used RVIZ for simulation nano path and rrt path and we can published points in map to let nano go to this points
  Feature 3: nano used esp8266 to send feedback data to ros rof batter control and tarcking
  Feature 4: nano used arduino uno R3 for control two stepper motor and send nano location to the esp8266
  Feature 5: nano have a camera for live broadcasting and data collection
