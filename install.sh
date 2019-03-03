echo Installing library
sudo install -m 0755 libmili.so /usr/local/lib
sudo ln -sf /usr/local/lib/libmili.so /usr/local/lib/libmili.so.1
sudo ln -sf /usr/local/lib/libmili.so /usr/local/lib/libmili.so.1.0
sudo ldconfig

echo Installing headers
sudo install -m 0644 *.h /usr/local/include
