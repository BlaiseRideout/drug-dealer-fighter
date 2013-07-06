#!/bin/bash

echo -n "Name: "
read name

sed "s/divamia/$name/g" makefile-template > makefile
