FROM ubuntu:22.04
LABEL Description="CPP Build Environment"

ARG TCAMAKE_VERSION=v24.02.19

ENV HOME /root
ENV TCAMAKE_PROJECT /opt
ENV TCAMAKE_HOME /opt/tcamake
ENV TCAMAKE_PREFIX /usr

shell ["/bin/bash", "-c"]

RUN apt-get update && apt-get -y --no-install-recommends install \
    build-essential \
    ca-certificates \
    curl \
    libtool \
    liblz4-dev \
    libssl-dev \
    libxml2-dev \
    libzstd-dev \
    net-tools \
    pkg-config \
    rsync \
    tini 

WORKDIR /opt

RUN curl https://github.com/tcarland/tcamake/archive/refs/tags/${TCAMAKE_VERSION}.tar.gz -L -o /tmp/tcamake.tar.gz && \
    tar -xzf /tmp/tcamake.tar.gz && \
    mv tcamake-* tcamake && \
    rm /tmp/tcamake.tar.gz

RUN mkdir -p /opt/tcanetpp
COPY . /opt/tcanetpp

RUN cd tcanetpp && source resources/tcanetpp_release_mt && \
    make arlib solib cmdbuf && make install && make distclean

ENTRYPOINT ["/usr/bin/tini", "--"]