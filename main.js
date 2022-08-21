const wasmFile = await fetch('./main-optimized.wasm');
const wasmBuffer = await wasmFile.arrayBuffer();
const wasmInstance = await WebAssembly.instantiate(wasmBuffer);
const wasmMemory = new Uint8Array(wasmInstance.instance.exports.memory.buffer);
const imageDataPointer = wasmInstance.instance.exports.getScreenImageData();
const screenWidth = wasmInstance.instance.exports.getScreenWidth();
const screenHeight = wasmInstance.instance.exports.getScreenHeight();
const canvasEl = document.getElementById('main-canvas');
if (!canvasEl) {
  throw new Error('canvas not found');
}
canvasEl.width = screenWidth;
canvasEl.height = screenHeight;
const ctx = canvasEl.getContext('2d');
if (!ctx) {
  throw new Error('canvas context not found');
}
ctx.clearRect(0, 0, screenWidth, screenHeight);

wasmInstance.instance.exports.init()

let prevTime = performance.now();
const update = () => {
  const time = performance.now();
  const delta = (time - prevTime) / 1000;

  wasmInstance.instance.exports.update(delta);
  const imageData = new ImageData(
    new Uint8ClampedArray(
      wasmMemory.subarray(
        imageDataPointer,
        imageDataPointer + screenWidth * screenHeight * 4)
      ),
      screenWidth, screenHeight
  );
  ctx.putImageData(imageData, 0, 0);

  prevTime = time;
  requestAnimationFrame(update);
};
requestAnimationFrame(update);

