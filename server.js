const http = require('http');
const fs = require('fs');
const path = require('path');

const filePath = path.join(__dirname, 'aarav.html');

const server = http.createServer((req, res) => {
    fs.readFile(filePath, (err, data) => {
        if (err) {
            res.writeHead(500);
            res.end('Error loading aarav.html');
        } else {
            res.writeHead(200, { 'Content-Type': 'text/html' });
            res.end(data);
        }
    });
});

server.listen(3000, '127.0.0.1', () => {
    console.log('Server running at http://127.0.0.1:3000/');
    console.log('Open this URL in your browser to test the code!');
});
