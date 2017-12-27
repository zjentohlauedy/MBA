#! /usr/bin/env bash
#
TEAM=$1
BASE_PATH=$( cd ${BASH_SOURCE%/*}; pwd -P )
IMAGE_FILE="$BASE_PATH/jerseys/${TEAM}_Road_Front.png"

if [[ ! -f "$IMAGE_FILE" ]];
then
    echo "Jersey image $IMAGE_FILE does not exist!";
    exit;
fi

viewnior $IMAGE_FILE &
