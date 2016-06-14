set -e
build_test_versions=("lua-5.3.3" "lua-5.2.4" "lua-5.1.5")
build_test_cxx_flags=("-std=c++03" "-std=c++11")

build_and_exec_test(){
  if [ ! -e "build$2_$1" ]; then
    mkdir "build$2_$1"
  fi
  cd "build$2_$1"
  cmake ../ -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DCMAKE_CXX_FLAGS=$2 -DLUA_SEARCH_LIB_NAME=$1 -DCOVERAGE=${TEST_COVERAGE}
  make -j4
  CTEST_OUTPUT_ON_FAILURE=1 make test
  cd ../
}

for luaversion in "${build_test_versions[@]}"
do
  if [ ! -e $luaversion ]; then
    if [ ! -e ${luaversion}.tar.gz ]; then
      wget https://www.lua.org/ftp/${luaversion}.tar.gz
    fi
    tar zxf ${luaversion}.tar.gz
  fi
  for cxxflag in "${build_test_cxx_flags[@]}"
  do
    build_and_exec_test $luaversion $cxxflag
  done
done

test_versions=("luajit")
for luaversion in "${test_versions[@]}"
do
  for cxxflag in "${build_test_cxx_flags[@]}"
  do
    build_and_exec_test $luaversion $cxxflag
  done
done
