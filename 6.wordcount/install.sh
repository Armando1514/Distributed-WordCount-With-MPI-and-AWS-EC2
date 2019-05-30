#!/bin/sh

echo "apt-get update"
sudo apt-get update

echo "install vim"
sudo apt-get -y install vim 

echo "install htop"
sudo apt-get -y install htop 

echo "install build-essential"
sudo apt-get -y install build-essential

echo "install ssh"
sudo apt-get -y install openssh-client openssh-server 

echo "install OpenMPI"
wget https://download.open-mpi.org/release/open-mpi/v4.0/openmpi-4.0.0.tar.gz
gunzip -c openmpi-4.0.0.tar.gz | tar xf -
cd openmpi-4.0.0
./configure --prefix=/usr/local
sudo make all install

echo "export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH" >> /home/pcpc/.bashrc
 
echo "clear"
cd ..
sudo rm -rf openmpi-4.0.0
rm openmpi-4.0.0.tar.gz

echo "Installation completed"

echo "create user pcpc"
sudo adduser pcpc
sudo usermod -aG sudo pcpc
echo "user pcpc created"


echo "configure ssh..."

#Change here using your private key
echo "" >> id_rsa

#Change here using you public key
echo "" >> id_rsa.pub 

#Change here using you public key
echo "" >> authorized_keys 

chmod 600 id_rsa
chmod 640 id_rsa.pub

sudo mkdir  -p /home/pcpc/.ssh

sudo mv id_rsa /home/pcpc/.ssh
sudo mv id_rsa.pub /home/pcpc/.ssh
sudo mv authorized_keys /home/pcpc/.ssh

sudo chown pcpc:pcpc /home/pcpc/.ssh
sudo chown pcpc:pcpc /home/pcpc/.ssh/id_rsa
sudo chown pcpc:pcpc /home/pcpc/.ssh/id_rsa.pub
sudo chown pcpc:pcpc /home/pcpc/.ssh/authorized_keys
echo "ssh configurated"

echo "ldconfig on user pcpc"
sudo -u pcpc sudo ldconfig
