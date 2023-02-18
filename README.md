# headless-opengl
Headless OpenGL on AWS

A project to use OpenGL headless on AWS EC2 instances with or without GPU.
A main file calls a scene file that renders an OpenGL scene and saves the output to an image file and publishes it on a web server for viewing.

# HOWTO
Start an AWS EC2 instance with or without a GPU, e.g. t4g.nano or g4ad.xlarge
For non_GPU, use the default AMI
For GPU, use the "Amazon Linux 2 AMI with Nvidia Tesla/AMD Radeon Pro driver" AMI
Make sure ports 22 and 80 are open.

Install git and clone this repo.
Source setup.sh to install packages and start an nginx docker image.
Source run.sh to run an example file and publish the output on the web server.

# Files
## setup.sh
Run this one to install necessary packages and start an nginx docker image

## basic_scene.c
Example scene containing a single triangle.

## template_scene.c
A template file that you can use for your own experiments.
The scene files must contain 2 functions:
void compileShaders(); // compiles the shaders
void render(); // renders the scene

## default.conf
Configuration file used by the nginx docker image.
Enables autoindex to allow browsing of the directory.

## glad/
OpenGL loader files for OpenGl 4.6
Generated on https://glad.dav1d.de/

## html/
Directory used by the ngins docker image.

## LICENSE
MIT license information.

## main.c
The main file that sets up EGL and OpenGL and calls the functions in the scene file and saves the output to a png file.
Takes 1 optional parameter: <filename>
<filename> is the filename of the output image file. Default: image.png

## png.c
Functions for saving png files

## png.h
Header file for png.c

## README.md
This file

## run.sh
Source this to build and run the code and copy the image file to the html directory.
Takes 2 optional positional parameters: <scene> and <filename>
<scene> is the filename of your scene file. Default: basic_scene
<filename> is the filename of the output image file. Default: image.png

# TODO
Fix compilation warnings
Add more example scene, e.g. with a geometry shader
Add instructions or script to generate glad files
Test on instance with GPU
Test on docker instance without GPU
