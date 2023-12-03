FROM haiku54/cvclient_base

COPY ./ /app/docker run --rm --privileged docker/binfmt:820fdd95a9972a5308930a2bdfb8573dd4447ad3 

RUN cd app && mkdir build && cd /app/build && cmake .. && make -j8

WORKDIR /app/build

RUN chmod +x /app/build/Camera/Camera

# Run your program
CMD ["./Camera/Camera"]
