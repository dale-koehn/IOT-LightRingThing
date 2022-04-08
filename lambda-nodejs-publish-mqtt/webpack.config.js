const path = require('path');

module.exports = {
  mode: 'production',
  //mode: 'development',
  target: 'node',
  devtool: false,
  entry: {
    app: './index.js',
  },
  output: {
    filename: 'app.js',
    libraryTarget: 'umd',
    path: path.resolve(__dirname, 'dist'),
  },
};

