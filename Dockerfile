FROM ubuntu:bionic
RUN apt update && apt install -y shellinabox git cmake gcc

WORKDIR /demo
RUN git clone --recursive https://github.com/marcinbor85/microshell

WORKDIR /demo/microshell/build
RUN cmake ..
RUN make && make test

RUN groupadd -g 999 microshell && useradd -r -u 999 -g microshell microshell
USER microshell

CMD /usr/bin/shellinaboxd -p 4200 --css /etc/shellinabox/options-available/00_White\ On\ Black.css --no-beep -t -s /:microshell:microshell:/:/demo/microshell/build/bin/demo
