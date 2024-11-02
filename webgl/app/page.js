"use client";

export default function Home() {
  return (
    <div>
      <div className="emscripten" suppressHydrationWarning>
        <figure style={{ overflow: "visible" }} id="spinner" suppressHydrationWarning>
          <div className="spinner"></div>
          <center style={{ marginTop: "0.5em" }}><strong>emscripten</strong></center>
        </figure>
        <div id="status" suppressHydrationWarning>Downloading...</div>
        <progress value="0" max="100" id="progress" hidden></progress>
        <div className="emscripten_border">
          <canvas className="emscripten" id="canvas" onContextMenu={(event) => event.preventDefault()} tabIndex="-1" suppressHydrationWarning ></canvas>
        </div>
      </div>

      <script async src="base.js" type='text/javascript'></script>
      <script async src="index.js" type="text/javascript"></script>
    </div>
  );
}
