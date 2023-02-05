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
const shell = require('shelljs');
const fs = require('fs');
const chokidar = require('chokidar')
const human = require('human-time')
console.log(`Process ID: ${process.pid}`);
const interval = 500
// 在按ctrl+c退出进程时也要杀死创建过的子进程
// 此时需要监听SININT事件,在事件处理函数中杀死子进程并退出主进程。如下所示：
process.on("SIGINT", () => {
  console.log('Received: SIGINT')
  kill(child, () => {
    process.exit();
  })
})
file="/Users/centforever/AndroidStudioProjects/GradleTest/sdk.json";
fs.watch(file,{ interval }, (eventType, filename) => {
  if (eventType == 'change') {
    console.log("The file", filename, "was modified,The type of change was:", eventType);
  }
});
const watcher = chokidar.watch(file, {
  alwaysStat: true,
})

watcher.on('ready', () => {
  watcher
    .on('add', (file, stat) => {
      console.log(`${file} created ${human(stat.birthtime)}`)
    })
    .on('unlink', (file) => {
      console.log(`${file}:removed`)
    })
    .on('change', (file, stat) => {
      const msg = +stat.ctime === +stat.mtime ? 'updated' : 'modified'
      console.log(`${file}:${msg}:${human(stat.ctime)}`)
    })
    .on('addDir', (dir, stat) => {
      console.log(`${dir} 文件夹created:${human(stat.birthtime)}`)
    })
    .on('unlinkDir', (dir) => {
      console.log(`${dir}:文件夹:removed`)
    })
})
shell.exec("ps aux | grep node | awk '$12~/watchfile.js/{print $2}'", function(code, stdout, stderr) {
  console.log('ps aux | grep node:', code);
  if (code===0) {
    console.log('ps aux | grep node program output:', stdout);
    node_pids = stdout
    node_pids.split(/\r?\n/).forEach(line =>  {
      if (line != "") {
        console.log(`Line from file: ${line}`);
      }

      if (line != "" && line != process.pid) {
        console.log(`kill -9 ${line}`);
        shell.exec(`kill -9 ${line}`)
      }
    });
    console.log('ps aux | grep node 成功')
  } else {
    console.log('ps aux | grep node program stderr:', stderr);
    console.log('ps aux | grep node 失败')
  }
});