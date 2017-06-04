# -*- coding: utf-8 -*-
import shlex,subprocess
import os
import urllib
import tarfile



boost_dir=os.environ.get("BOOST_PATH")
if not boost_dir:
    boost_dir='D:\\boost_1_60_0'

lua_versions = ["lua-5.3.3","lua-5.2.4","lua-5.1.5","luajit"]
maijor_test_lua_versions = ["lua-5.3.3"]

test_msvc_vers = [("msvc2015","Visual Studio 14 2015","",True)
    ,("msvc2015win64","Visual Studio 14 2015 Win64","",True)
    ,("msvc2013","Visual Studio 12 2013","",False)
    ,("msvc2013win64","Visual Studio 12 2013 Win64","",False)
    ,("msvc2012","Visual Studio 11 2012","-DADDITIONAL_INCLUDE_PATH="+boost_dir,False)
    ,("msvc2010","Visual Studio 10 2010","-DADDITIONAL_INCLUDE_PATH="+boost_dir,False)
    ,("msvc2008","Visual Studio 9 2008","-DADDITIONAL_INCLUDE_PATH="+boost_dir,True)
    ,("msvc2015","Visual Studio 14 2015","",True)
    ,("msvc2015win64","Visual Studio 14 2015 Win64","-DKAGUYA_USE_SHARED_LUAREF=1",False)
    ,("msvc2013","Visual Studio 12 2013","-DKAGUYA_USE_SHARED_LUAREF=1",False)
    ,("msvc2013win64","Visual Studio 12 2013 Win64","-DKAGUYA_USE_SHARED_LUAREF=1",False)
    ,("msvc2012","Visual Studio 11 2012","-DKAGUYA_USE_SHARED_LUAREF=1 -DADDITIONAL_INCLUDE_PATH="+boost_dir,False)
    ,("msvc2010","Visual Studio 10 2010","-DKAGUYA_USE_SHARED_LUAREF=1 -DADDITIONAL_INCLUDE_PATH="+boost_dir,False)
    ,("msvc2008","Visual Studio 9 2008","-DKAGUYA_USE_SHARED_LUAREF=1 -DADDITIONAL_INCLUDE_PATH="+boost_dir,False)]

test_compilers = [
    ('gcc-3.3','g++-3.3','',False)
    ,('gcc-4.4','g++-4.4','',False)
    ,('gcc-4.5','g++-4.5','',False)
    ,('gcc-4.6','g++-4.6','-DCMAKE_CXX_FLAGS=-std=c++03',False)
    ,('gcc-4.7','g++-4.7','-DCMAKE_CXX_FLAGS=-std=c++03',False)
    ,('gcc-4.8','g++-4.8','-DCMAKE_CXX_FLAGS=-std=c++03',False)
    ,('gcc-4.8','g++-4.8','-DCMAKE_CXX_FLAGS=-std=c++11',False)
    ,('gcc-4.9','g++-4.9','-DCMAKE_CXX_FLAGS=-std=c++03',False)
    ,('gcc-4.9','g++-4.9','-DCMAKE_CXX_FLAGS=-std=c++11',False)
    ,('gcc-5','g++-5','-DCMAKE_CXX_FLAGS=-std=c++03',False)
    ,('gcc-5','g++-5','-DCMAKE_CXX_FLAGS=-std=c++11',False)
    ,('gcc-6','g++-6','-DCMAKE_CXX_FLAGS=-std=c++03',False)
    ,('gcc-6','g++-6','-DCMAKE_CXX_FLAGS=-std=c++11',False)
    ,('gcc-6','g++-6','-DCMAKE_CXX_FLAGS=-std=c++14',False)
    ,('gcc','g++','-DCMAKE_CXX_FLAGS=-std=c++03',True)
    ,('gcc','g++','-DCMAKE_CXX_FLAGS=-std=c++11',True)
    ,('gcc','g++','-DCMAKE_CXX_FLAGS=-std=c++03 -DKAGUYA_SINGLE_HEADER_VERSION=1',True)
    ,('gcc','g++','-DCMAKE_CXX_FLAGS=-std=c++11 -DKAGUYA_SINGLE_HEADER_VERSION=1',True)
    ,('clang','clang++','-DCMAKE_CXX_FLAGS=-std=c++03',True)
    ,('clang','clang++','-DCMAKE_CXX_FLAGS=-std=c++11',True)
    ,('clang-3.5','clang++-3.5','-DCMAKE_CXX_FLAGS=-std=c++03',False)
    ,('clang-3.6','clang++-3.6','-DCMAKE_CXX_FLAGS=-std=c++03',False)
    ,('clang-3.7','clang++-3.7','-DCMAKE_CXX_FLAGS=-std=c++03',False)
    ,('clang-3.8','clang++-3.8','-DCMAKE_CXX_FLAGS=-std=c++03',False)
    ,('clang-3.9','clang++-3.9','-DCMAKE_CXX_FLAGS=-std=c++03',False)
    ,('clang-4.0','clang++-4.0','-DCMAKE_CXX_FLAGS=-std=c++03',False)
    ,('clang-3.5','clang++-3.5','-DCMAKE_CXX_FLAGS=-std=c++11',False)
    ,('clang-3.6','clang++-3.6','-DCMAKE_CXX_FLAGS=-std=c++11',False)
    ,('clang-3.7','clang++-3.7','-DCMAKE_CXX_FLAGS=-std=c++11',False)
    ,('clang-3.8','clang++-3.8','-DCMAKE_CXX_FLAGS=-std=c++11',False)
    ,('clang-3.9','clang++-3.9','-DCMAKE_CXX_FLAGS=-std=c++11',False)
    ,('clang-4.0','clang++-4.0','-DCMAKE_CXX_FLAGS=-std=c++11',False)
    ,('clang-3.8','clang++-3.8','-DCMAKE_CXX_FLAGS=-std=c++14',False)
    ,('clang-3.9','clang++-3.9','-DCMAKE_CXX_FLAGS=-std=c++14',False)
    ,('clang-4.0','clang++-4.0','-DCMAKE_CXX_FLAGS=-std=c++14',False)
    ,('clang-3.8','clang++-3.8','-DCMAKE_CXX_FLAGS=-std=c++03 -DKAGUYA_USE_SHARED_LUAREF=1',False)
    ,('clang-3.8','clang++-3.8','-DCMAKE_CXX_FLAGS=-std=c++11 -DKAGUYA_USE_SHARED_LUAREF=1',False)
    ,('clang-3.8','clang++-3.8','-DCMAKE_CXX_FLAGS=-std=c++14 -DKAGUYA_USE_SHARED_LUAREF=1',False)
    ,('gcc-6','g++-6','-DCMAKE_CXX_FLAGS=-std=c++03 -DKAGUYA_USE_SHARED_LUAREF=1',False)
    ,('gcc-6','g++-6','-DCMAKE_CXX_FLAGS=-std=c++11 -DKAGUYA_USE_SHARED_LUAREF=1',False)
    ,('gcc-6','g++-6','-DCMAKE_CXX_FLAGS=-std=c++14 -DKAGUYA_USE_SHARED_LUARE=1F',False)
    ]

def build_and_exec_test(compiler,lua_version,build_type,dir_opt):
    ccompiler = compiler[0]
    cxxcompiler = compiler[1]
    addopt = compiler[2]
    if os.system(cxxcompiler+' -v 2> /dev/null')!=0: return
    buildpath = os.path.join('_build',compiler[0]+"_"+lua_version+"_"+build_type+"_"+dir_opt)
    if not os.path.exists(buildpath):
        os.makedirs(buildpath)
    os.chdir(buildpath)
    ret = os.system('CC='+ccompiler+' CXX='+cxxcompiler+' cmake ../../  -DKAGUYA_BUILD_EXAMPLES=OFF '+addopt+' -DLUA_SEARCH_LIB_NAME='+lua_version+' -DCMAKE_BUILD_TYPE='+build_type)
    if ret != 0:#pass through cmake failed. e.g. not found lua
        if lua_version in maijor_test_lua_versions:
            raise Exception("cmake error at"+buildpath)
        os.chdir("../../")
        return
    ret = os.system('make -j 2')
    if ret != 0: raise Exception("build error at"+buildpath)
    ret = os.system('ctest --output-on-failure')
    if ret != 0: raise Exception("test error at"+buildpath)
    os.chdir("../../")

def build_with_target_compiler(lua_version):
    for i,compiler in enumerate(test_compilers):
        if not compiler[3] and lua_version not in maijor_test_lua_versions:
            continue
        build_and_exec_test(compiler,lua_version,"Debug",str(i))
        if compiler[3]:
            build_and_exec_test(compiler,lua_version,"Release",str(i))

def build_msvc_and_exec_test(msvcver,lua_version,build_type):
    buildpath = os.path.join('_build',msvcver[0]+'_'+lua_version)
    if not os.path.exists(buildpath):
        os.makedirs(buildpath)
    os.chdir(buildpath)
    ret = os.system('cmake ../../ -DLUA_SEARCH_LIB_NAME='+lua_version+' -G "'+msvcver[1]+'" '+msvcver[2])
    if ret != 0:#pass through cmake failed. e.g. not found lua
        if lua_version in maijor_test_lua_versions:
            raise Exception("cmake error at"+buildpath)
        os.chdir("../../")
        return
    ret = os.system('cmake --build . --config '+build_type)
    if ret != 0: raise Exception("build error at"+buildpath)
    ret = os.system('ctest --output-on-failure -C '+build_type)
    if ret != 0: raise Exception("test error at"+buildpath)
    os.chdir("../../")

def build_with_msvc_ver(lua_version):
    for msvcver in test_msvc_vers:
        if not msvcver[3] and lua_version not in maijor_test_lua_versions:
            continue
        build_msvc_and_exec_test(msvcver,lua_version,'Debug')
        if msvcver[3]:
            build_msvc_and_exec_test(msvcver,lua_version,'Release')



for i,luaversion in enumerate(lua_versions):
    if not os.path.exists(luaversion) and luaversion != 'luajit':
        if not os.path.exists(luaversion+".tar.gz"):
            lua_url = "https://www.lua.org/ftp/"+luaversion+".tar.gz"
            urllib.urlretrieve(lua_url,"{0}".format(luaversion+".tar.gz"))
        tf = tarfile.open(luaversion+".tar.gz", 'r')
        tf.extractall("./")

    if os.name == 'nt':
        build_with_msvc_ver(luaversion,)
    else:
        build_with_target_compiler(luaversion)
