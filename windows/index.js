const image = require('./src/build/Release/image');
const fs = require('fs');
const argumentCheckDir = (imageSrc, outputSrc) => {
  if (typeof imageSrc !== "string" || typeof outputSrc !== "string") {
    throw new Error('参数类型错误');
  }
  if (!fs.existsSync(imageSrc)) {
    throw new Error('图片不存在');
  }
  const array = outputSrc.split('/');
  let str = '';
  for (let index = 0; index < array.length - 1; index++) {
    str = str + array[index] + '/';
  }
  if (!fs.existsSync(str)) {
    throw new Error('输出地址不存在');
  }
}

const argumentCheckBase64 = (str) =>{
  if (typeof str !== "string") {
    throw new Error('参数类型错误');
  }
  // if(!isBase64(str)){
  //   throw new Error('base64图像编码格式不对');
  // }
}

const FrameCorrection = (imageSrc, outputSrc) => {
  argumentCheckDir(imageSrc, outputSrc);
  let result = image.FrameCorrection(imageSrc, imageSrc.length, outputSrc, outputSrc.length);
  if(result !== 'FrameCorrection'){
    throw new Error('图片矫正失败');
  }
}

const CharacterCorrection = (imageSrc, outputSrc) => {
  argumentCheckDir(imageSrc, outputSrc);
  let result = image.CharacterCorrection(imageSrc, imageSrc.length, outputSrc, outputSrc.length)
  if(result !== 'CharacterCorrection'){
    throw new Error('图片矫正失败');
  }
}
const DistortionCorrection = (imageSrc, outputSrc) => {
  argumentCheckDir(imageSrc, outputSrc);
  let result = image.DistortionCorrection(imageSrc, imageSrc.length, outputSrc, outputSrc.length)
  if(result !== 'DistortionCorrection'){
    throw new Error('图片矫正失败');
  }
}

const FrameCorrectionBase64 = (base64Data, type) => {
  argumentCheckBase64(base64Data);
  let result = image.FrameCorrectionBase64(base64Data, base64Data.length, type, type.length);
  // argumentCheckBase64(result);
  return result;
}

const CharacterCorrectionBase64 = (base64Data, type) => {
  argumentCheckBase64(base64Data);
  let result = image.CharacterCorrectionBase64(base64Data, base64Data.length, type, type.length);
  // argumentCheckBase64(result);
  return result;
}
const DistortionCorrectionBase64 = (base64Data, type) => {
  argumentCheckBase64(base64Data);
  let result = image.DistortionCorrectionBase64(base64Data, base64Data.length, type, type.length);
  // argumentCheckBase64(result);
  return result;
}

module.exports = {
  FrameCorrection,
  CharacterCorrection,
  DistortionCorrection,
  FrameCorrectionBase64,
  CharacterCorrectionBase64,
  DistortionCorrectionBase64
}