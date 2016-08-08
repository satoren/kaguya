
Preprocessor Options
==================================

This option must be same between translation units.

.. _preprocessor-use-cpp11:
* KAGUYA_USE_CPP11

  | If defined 1, kaguya use C++11 feature.
  | If defined 0, kaguya use C++03 only (need boost library).
  | default is auto detect (using _MSC_VER or __cplusplus).

|

* KAGUYA_NO_USERDATA_TYPE_CHECK

  If defined 1, Skip type check for userdata created without kaguya.

  .. warning::

    This option is dangerous to be used in conjunction with other Lua library.

    example:

     .. code-block:: lua

       --io.stdin is created by lua standard libray.
       kaguya_binded_receive_userdata_fn(io.stdin) -- Error not detected. this is undefined behavior.

|

* KAGUYA_NO_VECTOR_AND_MAP_TO_TABLE

  If difined, std::map and std::vector will not be converted to a lua-table

|

* KAGUYA_NO_STD_VECTOR_TO_TABLE

  If difined, std::vector will not be converted to a lua-table

|

* KAGUYA_NO_STD_MAP_TO_TABLE

  If difined, std::map will not be converted to a lua-table

|

* KAGUYA_FUNCTION_MAX_ARGS

  Define max argument count for binding function. default is 9.

  .. note::

    Effect in the C++03 only

|

* KAGUYA_FUNCTION_MAX_OVERLOADS

  Define max overloads function count for binding functions. default is 9.

  .. note::

    Effect in the C++03 only
