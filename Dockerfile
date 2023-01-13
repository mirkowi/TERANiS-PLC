# mraa ppa does not have newer packages than "focal"
FROM ubuntu:focal as build

# install system dependencies
    # speed up apt downloads
    RUN sed -i 's/htt[p|ps]:\/\/archive.ubuntu.com\/ubuntu\//mirror:\/\/mirrors.ubuntu.com\/mirrors.txt/g' /etc/apt/sources.list
    RUN echo $'\
    reload yes \
    precedence ::ffff:0:0/96 100 \
    precedence ::/0 10' >> /etc/gai.conf
    RUN echo "Aquire::ForceIPv4 false;" > /etc/apt/apt.conf.d/99force-ipv4
    # make apt usable with https
    RUN apt-get update 
    RUN apt-get install -y --fix-missing ca-certificates
    RUN update-ca-certificates
    # add eclipse mraa repo and install mraa
    RUN apt-get install -y --fix-missing software-properties-common
    RUN add-apt-repository ppa:mraa/mraa
    RUN apt-get update
    RUN apt-get install -y libmraa2 libmraa-dev mraa-tools
    # install compiler and dev tools
    RUN apt-get install -y \
        git \
        build-essential \
        cmake 

# download and install code dependencies
    # resolve open62541 opc/ua library    
    COPY download-open62541.sh .
    COPY compile-open62541-posix.sh .
    RUN chmod +x download-open62541.sh compile-open62541-posix.sh
    RUN bash download-open62541.sh
    RUN bash compile-open62541-posix.sh

# actual build
    # add source files and folders
    COPY avicstd avicstd
    COPY opcua opcua
    COPY modbus modbus
    COPY CMakeLists.txt ./
    COPY *.cpp ./
    COPY *.h ./
    # compile application
    RUN cmake .
    RUN make

# would be nice to have mraa for alpine...
FROM ubuntu:focal as prod
    # install system dependencies
    RUN apt-get update \
        && apt-get install -y ca-certificates software-properties-common \
        && update-ca-certificates \
        && add-apt-repository ppa:mraa/mraa \
        && apt-get update \
        && apt-get -y install libmraa2 libmraa-dev mraa-tools \
        && apt-get clean
    # install binary artefact
    COPY --from=build TERANiS_PLC /bin/teranis/plc

    # setup default configuration
    STOPSIGNAL SIGTERM
    ENV TERANIS_PLC_STOP_ON_SIGINT true
ENTRYPOINT ["/bin/teranis/plc"]
