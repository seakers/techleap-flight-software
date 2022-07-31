FROM ubuntu:20.04

ENV TZ="Etc/UTC"

RUN apt-get update -y &&\
    apt-get upgrade -y

RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get install -y python python3 python3-pip git build-essential python3-setuptools python3-dev python3-tk git zlib1g-dev libjpeg-dev swig

RUN pip3 install conan pandas numpy matplotlib pytest Pillow parse cmake wheel

RUN echo 'alias cleanbuild="python3 /basilisk/conanfile.py --clean --vizInterface False --buildProject True --pathToExternalModules /app/cmodules"' >> ~/.bashrc
RUN echo 'alias normalbuild="python3 /basilisk/conanfile.py --vizInterface False --buildProject True --pathToExternalModules /app/cmodules"' >> ~/.bashrc
RUN echo 'alias stepbuild="python3 /basilisk/conanfile.py --vizInterface False --buildProject False --pathToExternalModules /app/cmodules; cd /basilisk/dist3; make -j12"' >> ~/.bashrc







