#!/bin/bash

git clone https://github.com/0GuguH9/Nust-C.git

cd Nust-C

echo "Compiling"

make

echo "Creating local folder"

sudo mkdir /usr/local/nct

echo "Moving archives"

./nct.out -nlib -r stdlib /lib/

sudo mv nct.out /usr/local/nct/nct
sudo mv stdlib.nlib /usr/local/nct/stdlib.nlib

echo "Adding to PATH"

export PATH="/usr/local/nct:$PATH" >> ~/.bashrc

echo "Finished! Reload your user or use 'source ~/.bashrc
' or 'source ~/.zshrc', depending on your Linux."
