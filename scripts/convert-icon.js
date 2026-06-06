const toIco = require('to-ico');
const fs = require('fs');
const path = require('path');

const inputPath = path.join(__dirname, '../public/icon.png');
const outputPath = path.join(__dirname, '../public/icon.ico');

console.log('Converting PNG to ICO...');
console.log('Input:', inputPath);
console.log('Output:', outputPath);

const pngBuffer = fs.readFileSync(inputPath);

toIco([pngBuffer], {
  resize: true,
  sizes: [16, 24, 32, 48, 64, 128, 256]
})
  .then(buf => {
    fs.writeFileSync(outputPath, buf);
    console.log('✓ Successfully converted icon.png to icon.ico');
  })
  .catch(err => {
    console.error('✗ Error converting icon:', err);
    process.exit(1);
  });
