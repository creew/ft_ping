FROM debian:10.9
RUN adduser --quiet --disabled-password --gecos "User" user
RUN echo "user:password" | chpasswd
RUN apt-get update && apt-get install -y sudo rsync gdb build-essential openssh-server cmake valgrind git vim
RUN echo "user ALL=PASSWD: ALL" > /etc/sudoers.d/user
RUN mkdir -p /var/run/sshd \
  && sed -i "s/UsePrivilegeSeparation.*/UsePrivilegeSeparation no/g" /etc/ssh/sshd_config \
  && sed -i 's/PermitRootLogin without-password/PermitRootLogin yes/' /etc/ssh/sshd_config \
  && touch /root/.Xauthority \
  && true
RUN echo "Defaults lecture = never" >> /etc/sudoers.d/privacy
EXPOSE 22
CMD /usr/sbin/sshd -D
