FROM ubuntu:22.04
LABEL Description="CPP Build"

ARG TCAMAKE_VERSION=v24.03.03

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

RUN mkdir -p /opt/tcanetpp && \
    useradd -m --uid 1000 tdh && \
    chown -R tdh:tdh /opt/tcanetpp && \
    curl https://github.com/tcarland/tcamake/archive/refs/tags/${TCAMAKE_VERSION}.tar.gz -L -o /tmp/tcamake.tar.gz && \
    tar -xzf /tmp/tcamake.tar.gz && \
    mv tcamake-* tcamake && 
    rm /tmp/tcamake.tar.gz

COPY . /opt/tcanetpp

RUN cd tcanetpp && source .resources/release_mt.env && \
    make arlib solib cmdbuf && make install && make distclean

USER tdh

ENTRYPOINT ["/usr/bin/tini", "--"]