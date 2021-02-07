# asar-unpackfile
Unpack a specific file to memory from an asar archive

Neutralinojs uses Electron's `.asar` file format to bundle all resources. This module helps Neutralinojs to extract the binary content of a file to the physical memory when the URL is given. For example, `assets/app.js` will return the contents of the `app.js` file from `neu.asar` resource file.
