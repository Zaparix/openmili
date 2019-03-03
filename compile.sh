g++ -Ofast -mfpu=vfp -mfloat-abi=hard -march=armv7-a -mtune=arm1176jzf-s --shared -I/usr/local/include -L/usr/local/lib -lrf24-bcm PL1167_nRF24.cpp MiLightRadio.cpp mili.cpp -o libmili.so
