#!/bin/sh

find . -name ".DS_Store" -print -exec rm {} ";"
find . -name "*~" -print -exec rm {} ";"
find . -name ".*~" -print -exec rm {} ";"
find . -name "._*" -print -exec rm {} ";"
