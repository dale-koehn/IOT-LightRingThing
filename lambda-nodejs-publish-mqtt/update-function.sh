#!/bin/bash

npm run build
rm function.zip
cp dist/app.js .
zip -r function.zip app.js
rm app.js

aws lambda update-function-code --function-name publish-to-mqtt --zip-file fileb://function.zip

