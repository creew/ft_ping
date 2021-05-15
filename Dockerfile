FROM debian:latest
RUN apt-get update && apt-get install -y sudo openssh-server cmake
RUN service ssh start
ENTRYPOINT /bin/bash
