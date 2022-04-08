#!/bin/bash

aws lambda create-function \
    --function-name publish-to-mqtt \
    --runtime nodejs14.x \
    --zip-file fileb://function.zip \
    --handler app.handler \
    --role arn:aws:iam::xxxxxxxxxxxx:role/mqtt-pub-sub

