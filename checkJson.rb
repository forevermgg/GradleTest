#!/usr/bin/env ruby
# encoding: utf-8
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