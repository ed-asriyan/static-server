FROM fedora

MAINTAINER Ed Asriyan <ed-asriyan@protonmail.com>

RUN yum install -y gcc-c++
RUN yum install -y boost-devel
RUN yum install -y cmake
RUN yum install -y make

ADD . /app
WORKDIR /app

RUN cmake CMakeLists.txt
RUN make

CMD ./static_server 80 /static

EXPOSE 80
