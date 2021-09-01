# T-time

I run this on my PC that has Ubuntu 20.04 installed

## About

Getting a tee time at my local golf course has been very hard and annoying. I found myself constantly checking the website throughout the day to see if any tee times had opened up because the only way you can get a good tee time is if someone drops theirs, so you need to be constantly checking. Having to check all the time is annoying so I spent a few days writing this program written in C and if you run the script every morning the program will automatically run every 5 minutes from 11am to midnight checking to see if a tee time has become availabe and if a tee time has become available then a desktop notification will be sent letting you know.

## Dependencies
- at
```
$ sudo apt install -y at
$ sudo systemctl enable --now atd.service
```
- libcurl-dev
```
$ sudo apt install libcurl-dev
```

## Running
Open a terminal and navigate to this directory
```
$ make
```
Then every morning... in the same directory...
```
$ bash script.sh
```
