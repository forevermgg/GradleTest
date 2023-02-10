#!/usr/bin/env ruby
# encoding: utf-8
# https://droidyue.com/blog/2022/01/23/how-to-check-json-validity-in-ruby/
# 一个检测 Json 合法性的脚本
# 越来越多的配置都是使用 json 的格式，当我们修改好，最好是进行一下 json 合法性校验。
# checkJson.rb sdk.json
require 'json'
file = ARGV[0]


def is_json_valid(value)
  result = JSON.parse(value)
    result.is_a?(Hash) || result.is_a?(Array)
  rescue JSON::ParserError, TypeError
    false
  end


result = is_json_valid(File.open(file).read)
puts "json is valid(#{result})"