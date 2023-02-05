# brew install inotify-tools
chmod a+x inotifywaitfile.sh

# 获取当前脚本的绝对路径，参数$0是当前脚本对象
current_dir=$(cd "$(dirname "$0")"; pwd)
echo "$current_dir"
# 监视的文件或目录
filename=$current_dir/sdk.json
echo "$filename"
#get the script directory
SCRIPT=$(readlink -f "$filename")
echo "$SCRIPT"
script_path=$(dirname "$SCRIPT")
echo "$script_path"
trap"pkill -9 inotifywait;($SCRIPT &);exit" 1 2 15
# nohup bash inotifywaitfile.sh &
# nohup sh inotifywaitfile.sh>monitor.txt &
