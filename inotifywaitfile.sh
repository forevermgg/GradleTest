#!/bin/sh
# 获取当前脚本的绝对路径，参数$0是当前脚本对象
current_dir=$(cd "$(dirname "$0")"; pwd)
echo "$current_dir"
# 监视的文件或目录
filename=$current_dir/sdk.json
echo "$filename"
# 监视发现有增、删、改时执行的脚本
script="test"
echo "$script"

# shellcheck disable=SC2162
inotifywait -mrq --format '%e' --event create,delete,modify  "$filename" | while read event
do
   case $event in MODIFY|CREATE|DELETE) bash "$script" ;;
   esac
done