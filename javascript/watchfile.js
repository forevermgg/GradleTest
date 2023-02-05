// Node.js program to demonstrate the
// fs.watch() method
// 如果您想终止某个特定的节点进程，可以转到命令行路由并输入：
// ps aux | grep node
// 获取所有节点进程ids的列表。现在，您可以获取进程id(pid)，然后执行以下操作：
// kill -9 PID
// 如果你想杀死所有的节点进程，那就这样做：
// killall -9 node
// ps aux | grep node | awk '$12~/watchfile.js/{print $2}'
// Import the filesystem module
const fs = require('fs');
console.log(`Process ID: ${process.pid}`);
process.on('SIGHUP', () => console.log('Received: SIGHUP'));
process.on('SIGINT', () => console.log('Received: SIGINT'));
fs.watch("/Users/centforever/AndroidStudioProjects/GradleTest/sdk.json", (eventType, filename) => {
  console.log("The file", filename, "was modified,The type of change was:", eventType);
});