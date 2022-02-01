FROM ubuntu:18.04


RUN apt-get update -y &&\
    apt-get upgrade -y

RUN apt-get install -y python python3 python3-pip git build-essential python3-setuptools python3-dev python3-tk git zlib1g-dev libjpeg-dev swig

