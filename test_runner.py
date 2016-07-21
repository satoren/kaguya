# -*- coding: utf-8 -*-
import shlex,subprocess
import os
import urllib
import tarfile



boost_dir=os.environ.get("BOOST_PATH")
if not boost_dir:
    boost_dir='D:\\boost_1_60_0'

lua_versions = ["lua-5.3.3","lua-5.2.4","lua-5.1.5"]

test_msvc_vers = [("msvc2015","Visual Studio 14 2015","")
    ,("msvc2015win64","Visual Studio 14 2015 Win64","")
    ,("msvc2013","Visual Studio 12 2013","")
    ,("msvc2013win64","Visual Studio 12 2013 Win64","")
    ,("msvc2012","Visual Studio 11 2012","-DADDITIONAL_INCLUDE_PATH="+boost_dir)
    ,("msvc2010","Visual Studio 10 2010","-DADDITIONAL_INCLUDE_PATH="+boost_dir)
    ,("msvc2008","Visual Studio 9 2008","-DADDITIONAL_INCLUDE_PATH="+boost_dir)]

test_compilers = [
    ('gcc-3.3','g++-3.3','')
    ,('gcc-4.4','g++-4.4','')
    ,('gcc-4.5','g++-4.5','')
    ,('gcc-4.6','g++-4.6','-DCMAKE_CXX_FLAGS=-std=c++03')
    ,('gcc-4.7','g++-4.7','-DCMAKE_CXX_FLAGS=-std=c++03')
    ,('gcc-4.7','g++-4.7','-DCMAKE_CXX_FLAGS=-std=c++11')
    ,('gcc-4.8','g++-4.8','-DCMAKE_CXX_FLAGS=-std=c++03')
    ,('gcc-4.8','g++-4.8','-DCMAKE_CXX_FLAGS=-std=c++11')
    ,('gcc-4.9','g++-4.9','-DCMAKE_CXX_FLAGS=-std=c++03')
    ,('gcc-4.9','g++-4.9','-DCMAKE_CXX_FLAGS=-std=c++11')
    ,('gcc-5','g++-5','-DCMAKE_CXX_FLAGS=-std=c++03')
    ,('gcc-5','g++-5','-DCMAKE_CXX_FLAGS=-std=c++11')
    ,('gcc-6','g++-6','-DCMAKE_CXX_FLAGS=-std=c++03')
    ,('gcc-6','g++-6','-DCMAKE_CXX_FLAGS=-std=c++11')
    ,('gcc','g++','')
    ,('clang','clang++','-DCMAKE_CXX_FLAGS=-std=c++03')
    ,('clang','clang++','-DCMAKE_CXX_FLAGS=-std=c++11')]

def build_and_exec_test(compiler,lua_version,build_type,dir_opt):
    ccompiler = compiler[0]
    cxxcompiler = compiler[1]
    addopt = compiler[2]
    if os.system(cxxcompiler+' -v 2> /dev/null')!=0: return

    buildpath = "_build/"+compiler[0]+"_"+lua_version+"_"+build_type+"_"+dir_opt
    if not os.path.exists(buildpath):
        os.makedirs(buildpath)
    os.chdir(buildpath)
    ret = os.system('CC='+ccompiler+' CXX='+cxxcompiler+' cmake ../../ -DCMAKE_BUILD_TYPE=Debug '+addopt+' -DLUA_SEARCH_LIB_NAME='+lua_version+' -DCMAKE_BUILD_TYPE='+build_type)
    if ret != 0: raise Exception("cmake error at"+buildpath)
    ret = os.system('make -j')
    if ret != 0: raise Exception("build error at"+buildpath)
    ret = os.system('ctest --output-on-failure')
    if ret != 0: raise Exception("test error at"+buildpath)
    os.chdir("../../")

def build_with_target_compiler(lua_version):
    for i,compiler in enumerate(test_compilers):
        build_and_exec_test(compiler,lua_version,"Debug",str(i))
        build_and_exec_test(compiler,lua_version,"Release",str(i))

def build_msvc_and_exec_test(msvcver,lua_version,build_type):
    buildpath = '_build/'+msvcver[0]+'_'+lua_version
    if not os.path.exists(buildpath):
        os.makedirs(buildpath)
    os.chdir(buildpath)
    ret = os.system('cmake ../../ -DLUA_SEARCH_LIB_NAME='+lua_version+' -G "'+msvcver[1]+'" '+msvcver[2])
    if ret != 0: raise Exception("cmake error at"+buildpath)
    ret = os.system('cmake --build . --config '+build_type)
    if ret != 0: raise Exception("build error at"+buildpath)
    ret = os.system('ctest --output-on-failure -C '+build_type)
    if ret != 0: raise Exception("test error at"+buildpath)
    os.chdir("../../")

def build_with_msvc_ver(lua_version):
    for msvcver in test_msvc_vers:
        build_msvc_and_exec_test(msvcver,lua_version,'Debug')
        build_msvc_and_exec_test(msvcver,lua_version,'Release')



for luaversion in lua_versions:
    if not os.path.exists(luaversion):
        if not os.path.exists(luaversion+".tar.gz"):
            lua_url = "https://www.lua.org/ftp/"+luaversion+".tar.gz"
            urllib.urlretrieve(lua_url,"{0}".format(luaversion+".tar.gz"))
        tf = tarfile.open(luaversion+".tar.gz", 'r')
        tf.extractall("./")

    if os.name == 'nt':
        build_with_msvc_ver(luaversion)
    else:
        build_with_target_compiler(luaversion)
