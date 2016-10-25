# quiz40

[![Build Status](https://travis-ci.org/arnaud-ramey/quiz40.svg)](https://travis-ci.org/arnaud-ramey/quiz40)

A minimalistic quiz program,
based on pairs of corresponding question/answer images.

Licence
=======

LGPL v3, check file ```LICENCE```.

Dependencies
============

You need the following libraries before compiling :
 * cmake  ( sudo apt-get install cmake ),
 * OpenCV ( sudo apt-get install libopencv-dev )

How to build the program
========================

The project is based on a CMakeLists.
It is easy to build the program on a Unix computer.
Go in the source folder and type:
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

For Windows users, some instructions are available on OpenCV website:
http://opencv.willowgarage.com/wiki/Getting_started .

How to use the program
======================


Synopsis:
```bash
  quiz40 FOLDER [NQUESTIONS]
```

First, the question image is displayed. Press a key to show the answer.
Then, press "1" or "y" if your answer was correct, any other key otherwise.
Your score is displayed in the image.

Authors
=======

Arnaud Ramey <arnaud.a.ramey@gmail.com>
  -- Robotics Lab, University Carlos III of Madrid
