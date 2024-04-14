#!/bin/bash

echo "Wait while we is setting your compiler"

echo ""

git clone https://github.com/0GuguH9/Nust-C.git

cd Nust-C

echo ""

echo "Compiling"

echo ""

make

echo ""

echo "Creating local folder"

echo ""

sudo mkdir /usr/local/nct

echo ""

if [[ ":$PATH:" != *":/usr/local/nct/:"* ]]; then
    echo "Adding to PATH and moving the compiler"
    echo ""
    export PATH="/usr/local/nct/:$PATH" >> ~/.bashrc
    
else
    echo "Updating the compiler"
    echo ""
fi

source ~/.bashrc

./nct.out -nlib -r stdlib /lib/

echo ""

sudo mv -f nct.out /usr/local/nct/nct
sudo mv -f stdlib.nlib /usr/local/nct/stdlib.nlib

echo ""

echo "Finished! Reload your user or use 'source ~/.bashrc
' or 'source ~/.zshrc', depending on your Linux."
