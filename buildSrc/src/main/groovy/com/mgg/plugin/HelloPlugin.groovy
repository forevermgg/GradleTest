package com.mgg.plugin

import org.gradle.api.Plugin
import org.gradle.api.Project
// https://www.jianshu.com/p/f902b51e242b
class HelloPlugin implements Plugin<Project> {
    @Override
    void apply(Project project) {
        println("HelloPlugin:" + project.name.toString())
    }
}