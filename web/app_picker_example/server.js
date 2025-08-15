const express = require('express');
const path = require('path');

const app = express();
const port = 8080;

app.use((req, res, next) => {
  res.setHeader('Cross-Origin-Opener-Policy', 'same-origin');
  res.setHeader('Cross-Origin-Embedder-Policy', 'require-corp');
  next();
});

app.use('', express.static(path.join(__dirname, 'dist')));
app.use('', express.static(path.join(__dirname, 'dist/wasm')));

app.listen(port, () => {
  console.log(`Server running at http://localhost:${port}`);
});