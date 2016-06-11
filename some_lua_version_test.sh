build_test_versions=("lua-5.3.3" "lua-5.2.4" "lua-5.1.5")

for version in "${build_test_versions[@]}"
do
  if [ ! -e $version ]; then
    if [ ! -e ${version}.tar.gzn ]; then
      wget https://www.lua.org/ftp/${version}.tar.gz
    fi
    tar zxf ${version}.tar.gz
  fi

  if [ ! -e "build$version" ]; then
    mkdir "build$version"
  fi
  cd "build$version"
  cmake ../ -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DCMAKE_CXX_FLAGS=${CXX_FLAGS} -DLOCAL_LUA_DIRECTORY=$version -DCOVERAGE=${TEST_COVERAGE}
  make -j4
  CTEST_OUTPUT_ON_FAILURE=1 make test
  cd ../
done

test_versions=("luajit")
for version in "${test_versions[@]}"
do
  if [ ! -e "build$version" ]; then
    mkdir "build$version"
  fi
  cd "build$version"
  cmake ../ -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DCMAKE_CXX_FLAGS=${CXX_FLAGS} -DLUA_SEARCH_LIB_NAME=$version -DCOVERAGE=${TEST_COVERAGE}
  make -j4
  CTEST_OUTPUT_ON_FAILURE=1 make test
  cd ../
done
