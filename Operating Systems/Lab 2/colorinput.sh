#! /bin/bash
echo Please enter the first color
read -e color1
echo Please enter the second color
read -e color2
setterm -term linux -back $color2 -fore $color1 -clear all
