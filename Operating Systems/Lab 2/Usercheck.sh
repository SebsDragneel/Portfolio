#! /bin/bash
echo Please enter your username
read -e musername
sudo apt-get install finger 
finger -l $musername > userdata
