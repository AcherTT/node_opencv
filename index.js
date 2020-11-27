const os = require('os');
let image;
// console.log(os.type)
if (os.type() === 'Windows_NT') {
  image = require('./windows/index')
} else if (os.type() === 'Linux') {
  image = require('./linux/index')
} else {
  throw new Error('你的系统暂不支持')
}

module.exports = image;