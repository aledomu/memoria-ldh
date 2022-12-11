FROM docker.io/library/gcc:12.2.0 AS build
ARG GIT_COMMIT=093afa0a8a573bda2bb8084b473778795c526532
ADD https://github.com/mysensors/MySensors/archive/$GIT_COMMIT.zip /MySensors.zip
WORKDIR /usr/src
RUN unzip /MySensors.zip
RUN mv /usr/src/MySensors-093afa0a8a573bda2bb8084b473778795c526532 /usr/src/mysensors
WORKDIR /usr/src/mysensors
RUN ./configure --my-transport=rf24 --my-rf24-channel=30 --my-gateway=ethernet --my-port=5003
RUN make

FROM gcr.io/distroless/cc-debian11:latest
COPY --from=build --chmod=755 /usr/src/mysensors/bin/mysgw /app/mysgw
COPY mysensors.conf /etc/mysensors.conf
EXPOSE 5003
ENTRYPOINT ["/app/mysgw"]
