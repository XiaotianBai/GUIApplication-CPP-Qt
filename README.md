# GUIApplication-CPP-Qt
Simulator &amp; controller of a 6-DOF robotic arm

This project is my undergraduate graduation project.

The initial target of this project is to design a control application for a virtual robot with multiple degrees of freedom (it later becomes the Simulator). 

I created a dialog based MFC application for the simulator. For the coordinates/angle transformation I used Edit boxes to pass data, buttons to trigger event and some check boxes and radio buttons for different options. 

For the 3D model, I used OpenGL API to draw an arm with six joints, the drawArmBody function has six angle parameters which can be passed by the solution of the transformation. Note that there could be at most 8 sets of possible angles for a hand position, so I put 8 solution lines on the dialog and put a display button after each of them. I just needed to add several arrow buttons to move the hand to finish this virtual controller.

Later, I was asked by my supervisor to dig further and design an application to control a real robot. The second application is a wrapper of the SerialPort.write function that sends data to robot servos. I also added a lot of other useful functions.

For the new controller I used Qt5 for the potential of migration to mobile OSes. I linked it with the Simulator using QFile and QProcess API.

The application can be used to manually control the servos or read commands from the calculated solution file and execute, also, users can record actions and play it back or loop it.


The graduation project was graded A. For more details please read thesis.pdf (mostly written in Chinese).
