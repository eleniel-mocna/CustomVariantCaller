FROM rocker/rstudio
LABEL NAME="cvc" Version="1.1"
LABEL author="Samuel Soukup"
LABEL contact="soukup.sam(at)gmail.com"

RUN mkdir cvc
COPY Makefile cvc/Makefile
COPY cvc /cvc/cvc
COPY tools cvc/tools
# COPY reference /reference # Reference should be mounted via -v argument
COPY Dockerfile /

RUN cd cvc && make
RUN apt-get update && \
    apt-get install -y samtools bwa && \
    apt-get clean

CMD ["/init"]

RUN echo 'source("/cvc/tools/cvc.R")' >> /usr/local/lib/R/etc/Rprofile.site
