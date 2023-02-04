#!/usr/bin/env ruby
# encoding: utf-8
require 'find'
require 'digest/md5'

#  通常为项目的路径
targetDirToSearch = ARGV[0]
$hashesFiles = {}
$sizeCanBeSaved = 0

def getFileMd5Checksum(file)
  return Digest::MD5.hexdigest(File.read(file))
end

def shouldCheckThisFile(f)
  isFile = File.file?(f)
  isGitFile = f.include? ".git/"
  isGradleFile = f.include? ".gradle/"
  isIdeFile = f.include? ".idea/"
  return isFile && !isGitFile && !isGradleFile && !isIdeFile
end

def getFilesByMd5(md5Value)
  existingFiles = $hashesFiles[md5Value]
  if (existingFiles == nil)
      existingFiles = []
  end
  return existingFiles
end

def recordFile(f)
  md5 = getFileMd5Checksum(f)
  $hashesFiles[md5] = getFilesByMd5(md5).push(f)
end

def printHashesFiles()
  $hashesFiles.values.select {
      |array| array.size > 1
  }.sort_by {
      |files| File.size(files[0])
  }.each {
      |array|
          fileSize = File.size(array[0])
          puts "Duplicated files size=#{format_mb(fileSize)}"
          array.each {
              |f| puts f
          }
          $sizeCanBeSaved += fileSize * (array.size - 1)
          puts ""
  }

end

def format_mb(size)
  conv = [ 'b', 'kb', 'mb', 'gb', 'tb', 'pb', 'eb' ];
  scale = 1024;

  ndx=1
  if( size < 2*(scale**ndx)  ) then
    return "#{(size)} #{conv[ndx-1]}"
  end
  size=size.to_f
  [2,3,4,5,6,7].each do |ndx|
    if( size < 2*(scale**ndx)  ) then
      return "#{'%.3f' % (size/(scale**(ndx-1)))} #{conv[ndx-1]}"
    end
  end
  ndx=7
  return "#{'%.3f' % (size/(scale**(ndx-1)))} #{conv[ndx-1]}"
end

def getFileSize(f)
  return format_mb(File.size(f))
end

def start(dirToSearch)
  Find.find(dirToSearch).select {
      |f| shouldCheckThisFile(f)
  }.each {
      |f| puts "Checking file #{f}"
          recordFile(f)
  }
  printHashesFiles()
  puts "Size can be saved #{format_mb($sizeCanBeSaved)}"
end

start(targetDirToSearch)