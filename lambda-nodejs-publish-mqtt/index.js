var AWS = require('aws-sdk');
var iotdata = new AWS.IotData({ endpoint: 'xxxxxxxxxxxx-ats.iot.us-east-2.amazonaws.com' });

exports.handler = async(event) => {
  console.log("Event => " + JSON.stringify(event));
  var params = {
    topic: "esp32/sub",
    payload: JSON.stringify(event),
    qos: 0
  };

  return iotdata.publish(params, function(err, data) {
    if (err) {
        console.log("ERROR => " + JSON.stringify(err));
    }
    else {
      console.log("Successsss");
    }
    return { message: 'Nothing to see here' };
  }).promise();
};

